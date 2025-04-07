#include <Arduino.h>
#define RELE 74

unsigned int tiempo = 0;
bool connectionAccepted = false, runTime = false, stopp = 1, z = 0, reset_bills = 0;
String input = "";

void killSession()
{
  // Serial.println("kill2");
  tiempo = 0;
  connectionAccepted = false;
  runTime = false;
}
void killConnection()
{
  //  Serial.println("kill");
  killSession();
}
void acceptTime()
{
  tiempo = 0;
  tiempo = millis() / 1000;
  runTime = true;
}
void compareToken()
{
  if ((input.substring(0, 80)).equals("1cc149ae15564asfd616335664e9191ee7b9f348d3a8a782fa8412671e261"))
  {

    connectionAccepted = 1;
    Serial3.write("200");
    // input="";
  }
  else
  {
    Serial3.write("401");
    killConnection();
  }
}

void setup()
{
  Serial.begin(9600);
  Serial3.begin(9600);
  pinMode(RELE, OUTPUT); // control de rel
 // digitalWrite(RELE, HIGH);
}

void loop()
{
  if (Serial3.available())
  {
    input = Serial3.readString();
    // delay(100);
    // Serial.print("INPUT_valor:");
    Serial.println(input);

    compareToken();
    Serial.println(connectionAccepted);
    Serial.println(input.substring(0, 3));
    if ((input.substring(0, 3) == "100"))
    {
      Serial.println("XXXXXXXXXXXXXXXXXXXX");
      /*LAVADORAaaaaaaaaaaaaaaaaaaa*/
      digitalWrite(RELE, HIGH);
       delay(500);
       digitalWrite(RELE, LOW);
        delay(500);
           digitalWrite(RELE, HIGH);
       delay(500);
       /*
      delay(200);
      digitalWrite(RELE, LOW);
      delay(21);
      digitalWrite(RELE, HIGH);
      delay(200);
*/
    }

    // Serial.println(input.length());
  }
  delay(100);
}