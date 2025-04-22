#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "config.h"

// inicializacion de mqtt
void mqtt_init(const HardwareConfig* hw);

// procesos de wifi xonexiones reconexiones mqtt
void mqtt_loop(void);


#endif
