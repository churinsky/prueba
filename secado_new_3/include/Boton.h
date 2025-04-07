#include <Arduino.h>
#define buzzer 9
class Boton
{

    int _boton;
    int _led;

public:
    Boton(int btn, int led)
    {
        _boton = btn;
        pinMode(_boton, INPUT_PULLUP);
        _led = led;
        pinMode(_led, OUTPUT);
    }
    bool isPressed()
    {
        if (!digitalRead(_boton))
        {
            ton();
        }

        return (!digitalRead(_boton));
    }
    void ton2()
    {
        tone(buzzer, 2000);
        delay(6000);
        noTone(buzzer);
    }
    void led_on()
    {
        digitalWrite(_led, HIGH);
    }
    void led_off()
    {
        digitalWrite(_led, LOW);
    }
    void ton3()
    {
        tone(buzzer, 2000);
        delay(50);
        noTone(buzzer);
    }
    void ton4()
    {
        tone(buzzer, 2000);
        delay(100);
        noTone(buzzer);
        delay(50);
        tone(buzzer, 2000);
        delay(150);
        noTone(buzzer);
    }

private:
    void ton()
    {
        digitalWrite(_led, HIGH);
        tone(buzzer, 5000);
        delay(100);
        digitalWrite(_led, LOW);
        noTone(buzzer);
    }
};
