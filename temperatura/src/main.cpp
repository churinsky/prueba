#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(37, 40, 41, 38, 20, 21);

const int pinDatosDQ = 49;

// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

void setup()
{
  // Iniciamos la comunicación serie
  Serial.begin(9600);
  // Iniciamos el bus 1-Wire
  sensorDS18B20.begin();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  
}

void loop()
{
  // Mandamos comandos para toma de temperatura a los sensores
  Serial.println("Mandando comandos a los sensores");
  sensorDS18B20.requestTemperatures();

  // Leemos y mostramos los datos de los sensores DS18B20
  Serial.print("Temperatura sensor 0: ");
  Serial.print(sensorDS18B20.getTempCByIndex(0));
  lcd.setCursor(0, 0);
  lcd.print(sensorDS18B20.getTempCByIndex(0));
  Serial.println(" C");

  //lcd.clear();

  //delay(200);
//  lcd.clear();
  
}