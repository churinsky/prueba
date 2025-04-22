#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// tipo de maquina
typedef enum {
    MODO_LAV_MAU_S = 1,
    MODO_LAV_BRI_S,
    MODO_LAV_BRI_D,
    MODO_SEC_MAU_S,
    MODO_SEC_BRI_S,
    MODO_SEC_BRI_D
} ModoOperacion;

// configuraciones
typedef struct {
    uint8_t id_maquina;
    uint8_t id_maquina_1;

    const char* wifi_ssid;
    const char* wifi_pass;
    const char* mqtt_broker;
    uint16_t    mqtt_port;

    uint8_t pin_rele;    
    ModoOperacion modo;  
} HardwareConfig;

//mqtt
typedef struct {
    String topic1;
    String topic2;
    String msg1;
    String msg2;
    String topic_publish;
} MqttConfig;


// dunciones
ModoOperacion detectarModo(uint8_t tipo_maquina, uint8_t modelo, uint8_t tipo_activacion);
bool modo_es_doble(ModoOperacion modo);
bool modo_usa_rele(ModoOperacion modo);
bool modo_es_secadora(ModoOperacion modo);

#endif
