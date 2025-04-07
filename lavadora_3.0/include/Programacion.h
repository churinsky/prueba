#ifndef _PROGRAMACION_H_
#define _PROGRAMACION_H_
#include <Arduino.h>
#include <TM1637Display.h>
#define CLK 4
#define DIO 2
TM1637Display pdisplay(CLK, DIO);

class Programacion
{
    int _menu;

public:
    void progg(int menu)
    {
        _menu = menu;
        switch (_menu)
        {
        case 0:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 1:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 2:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 3:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 4:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 5:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 6:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 7:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 8:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 9:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);
            break;
        case 10:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 11:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 12:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 13:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 14:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 15:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 16:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 17:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 18:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 19:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 20:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 21:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 22:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 23:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 24:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 25:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 26:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 27:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 28:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        case 29:
            pdisplay.clear();
            pdisplay.setBrightness(0x0f);

            break;
        }
    }
};
#endif