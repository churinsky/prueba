#include <Arduino.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>

// Configuración del AP
const char *ap_ssid = "ESP32";     // red para la rimer configurtacion
const char *ap_password = "12345678"; // Contraseña de la red

// WebSocket sobre ek uerto puerto 81
WebSocketsServer webSocket(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_CONNECTED:
  { // conexion del cliente 
    IPAddress clientIP = webSocket.remoteIP(num);
    Serial.printf("[%u] Conectadose desde %s\n", num, clientIP.toString().c_str());
    // esto es lo que recibe el cliente si se hace la conexion bien
    webSocket.sendTXT(num, "Conectado al esp32");
    break;
  }
  case WStype_DISCONNECTED: //este es el mensaje que se manda cuando se desconecta el cliente
    Serial.printf("[%u] Desconectado\n", num);
    break;
  case WStype_TEXT:
  { // muestro el mensaje recibido delcleinte
    String message = String((char *)payload);
    Serial.printf("[%u] Mensaje recibido: %s\n", num, message.c_str());
    // le reennvio lo mismo que me mando
    webSocket.sendTXT(num, "Servidor recibió: " + message);
    break;
  }

  default:
    break;
  }
}

void setup()
{
  Serial.begin(9600);

  // modo AP configuracion
  WiFi.softAP(ap_ssid, ap_password);

  // dirección IP fija
  IPAddress apIP(192, 168, 4, 1);     
  IPAddress netMsk(255, 255, 255, 0); 
  WiFi.softAPConfig(apIP, apIP, netMsk);

  Serial.println("Nomre de la red:");
  Serial.printf("SSID: %s\n", ap_ssid);
  Serial.printf("Contraseña: %s\n", ap_password);
  Serial.printf("ESP32 acces point: %s\n", WiFi.softAPIP().toString().c_str());

  // servidor WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Esperando clientes...");
}

void loop()
{
  // loop WebSocket
  webSocket.loop();
}
