#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include "mqtt_client.h"


static const HardwareConfig *hw_config = nullptr;
static MqttConfig mqtt_config;

static WiFiClient espClient;
static PubSubClient client(espClient);

static char mqtt_buffer[128];
static bool shouldPublish = false;
static int push = 0;
static int numActivaciones = 0;

static void generarMqttConfig()
{
    mqtt_config.topic1 = "use/" + String(hw_config->id_maquina);
    mqtt_config.msg1 = "m" + String(hw_config->id_maquina);

    mqtt_config.topic2 = "use/" + String(hw_config->id_maquina_1);
    mqtt_config.msg2 = "m" + String(hw_config->id_maquina_1);

    mqtt_config.topic_publish = "kiosko/1";
}

static bool extraerNumeroActivaciones(const char *payload, int &valor)
{
    const char *start = strchr(payload, '{');
    const char *end = strchr(payload, '}');
    if (start && end && end > start)
    {
        valor = atoi(start + 1);
        return valor > 0;
    }
    return false;
}

static void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    if (length >= sizeof(mqtt_buffer))
        length = sizeof(mqtt_buffer) - 1;
    memcpy(mqtt_buffer, payload, length);
    mqtt_buffer[length] = '\0';

    int activaciones = 0;
    if (extraerNumeroActivaciones(mqtt_buffer, activaciones))
    {
        shouldPublish = true;
        numActivaciones = activaciones;

        if (modo_es_doble(hw_config->modo))
        {
            if (String(topic) == mqtt_config.topic2)
            {
                push = 2;
            }
            else
            {
                push = 1;
            }
        }
        else
        {
            push = 1;
        }
    }
}

void mqtt_init(const HardwareConfig *hw)
{
    hw_config = hw;
    generarMqttConfig();

    WiFi.begin(hw->wifi_ssid, hw->wifi_pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    client.setServer(hw->mqtt_broker, hw->mqtt_port);
    client.setCallback(mqtt_callback);
    client.setKeepAlive(60);
    // Configurar pin de relé si aplica
    if (modo_usa_rele(hw_config->modo))
    {
        pinMode(hw_config->pin_rele, OUTPUT);
        digitalWrite(hw_config->pin_rele, LOW); // Apagado por defecto
    }
}

void mqtt_loop()
{
    if (!client.connected())
    {
        String clientId = "ESPClient-" + String(WiFi.macAddress());
        while (!client.connected())
        {
            if (client.connect(clientId.c_str()))
            {
                client.subscribe(mqtt_config.topic1.c_str(), 1);
                if (modo_es_doble(hw_config->modo))
                {
                    client.subscribe(mqtt_config.topic2.c_str(), 1);
                }
            }
            else
            {
                delay(1000);
            }
        }
    }

    if (shouldPublish)
    {
        const char *mensaje = (push == 1) ? mqtt_config.msg1.c_str() : mqtt_config.msg2.c_str();
        client.publish(mqtt_config.topic_publish.c_str(), mensaje, 1);

        if (modo_usa_rele(hw_config->modo))
        {
            for (int i = 0; i < numActivaciones; i++)
            {
                digitalWrite(hw_config->pin_rele, LOW);
                delay(200);
                digitalWrite(hw_config->pin_rele, HIGH);
                delay(200);
                digitalWrite(hw_config->pin_rele, LOW);
                delay(200);
            }
        }
        else
        {
            Serial.println(mqtt_buffer);
        }

        shouldPublish = false;
        push = 0;
        numActivaciones = 0;
    }

    client.loop();
}


