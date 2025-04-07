#include <Arduino.h>


class Buttton
{

public:
    int _button;
    int state;
    unsigned long buttonDownMs;
    Buttton(int button)
    {
        _button = button;
        pinMode(_button, INPUT);
    }

    bool isPressed()
    {
   
        return (!digitalRead(_button));
    }

    int longpulse()
    {
        int prevState = state;
        state = digitalRead(_button);

        if (prevState == LOW && state == HIGH)
        {
            buttonDownMs = millis();
        }
        else if (prevState == HIGH && state == LOW)
        {
            if (millis() - buttonDownMs < 50)
            {

                // Serial.println("antirrebote");
            }
            else if (millis() - buttonDownMs < 250)
            {
                // Serial.println("click corto");
                return 1;
            }
            else
            {
                // Serial.println("click largo");
                return 2;
            }
        }
    }

private:

};
