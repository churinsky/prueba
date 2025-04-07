#include <Arduino.h>

int _datoAnterior2 = 0;
int segundos;

class Ciclo
{
private:
public:
    int timmer(int segundos)
    {
        // return _tiempo;
        if ((millis() / 1000) != _datoAnterior2)
        {
            segundos--;
        }
        _datoAnterior2 = (millis() / 1000);
        return segundos;
    }
};