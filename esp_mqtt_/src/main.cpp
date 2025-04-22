#include "config.h"
#include "mqtt_client.h"

// configuracion del tipo de maquina
#define VAL_TIPO_MAQUINA      0  // 0 = LAVADORA, 1 = SECADORA
#define VAL_MODELO_MAQUINA    0  // 0 = MAUTRA,   1 = BRIDGE
#define VAL_TIPO_ACTIVACION   1  // 1 = SIMPLE,   2 = DOBLE

void setup() {
    Serial.begin(115200);

    //modo de operacion
    ModoOperacion modo = detectarModo(VAL_TIPO_MAQUINA, VAL_MODELO_MAQUINA, VAL_TIPO_ACTIVACION);

    // estructura de config
    static HardwareConfig hw = {
        .id_maquina     = 5,
        .id_maquina_1   = 10,
        .wifi_ssid      = "Maquinas1",
        .wifi_pass      = "Lavan.2022",
        .mqtt_broker    = "192.168.0.200",
        .mqtt_port      = 1883,
        .pin_rele       = D1,
        .modo           = modo
    };

    mqtt_init(&hw); 
}

void loop() {
    mqtt_loop();
}
