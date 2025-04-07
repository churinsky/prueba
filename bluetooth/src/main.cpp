#include <Arduino.h>



unsigned int tiempo = 0;
bool connectionAccepted = false, runTime = false, stopp = 1, z = 0, reset_bills = 0;
String input = "";
void killSession()
{
  tiempo = 0;
  connectionAccepted = false;
  runTime = false;
}
void killConnection()
{
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
  if ((input.substring(0, 80)).equals("1cc149ae15564asfd616335664e9191ee7b9f348d3a8a782fa8412671e275"))
  {
  //  Serial.print("token_mio");
  //  Serial.print(input);
    connectionAccepted = 1;
    reset_bills = 1;
    Serial3.write("200");
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
   pinMode(13, OUTPUT);  //control de rel
}

void loop()
{

  if (Serial3.available())
  {
    Serial.println("entro en blutubi");
    input = Serial3.readString();
    // delay(100);
    // Serial.print("INPUT_valor:");
    // Serial.println((input.substring(0, 7)));
    // Serial.print("input_longitud:");
    // Serial.print(input.length());
    if (!Serial3.available())
    {
      // Serial.print(input);
      // Serial.print(input.length());
      if ((input.substring(0, 7)).equals("OK+CONN"))
      {
        // Serial.print("aceptime");
        acceptTime();
      }
      else if (input.equals("OK+LOST") && connectionAccepted)
      {
        //  Serial.println("kill_sesion");
        killSession();
      }
      else
      {
        // Serial.println("remplace_input");
        input.replace("OK+CONN", "");
        input.replace("OK+LOST", "");
        input.replace("OK", "");
        if (!input.equals("") && !connectionAccepted)
        {
          // Serial.print("comparacion");
          compareToken();
          // Serial.print(input);
        }
        else if (connectionAccepted)
        {
           Serial.println("CONECCIONON OK");
          // Serial.println(input.length());
          input == "100" ? Serial.println("2") : Serial.println("1");
          if (input == "100")
          {
          }
          else
          {

          }
        }
      }
    }
  }
}