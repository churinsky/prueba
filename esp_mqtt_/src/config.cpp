#include "config.h"

ModoOperacion detectarModo(uint8_t tipo_maquina, uint8_t modelo, uint8_t tipo_activacion) {
    if (tipo_maquina == 0) { // LAVADORA
        if (modelo == 0 && tipo_activacion == 1) return MODO_LAV_MAU_S;
        if (modelo == 1 && tipo_activacion == 1) return MODO_LAV_BRI_S;
        if (modelo == 1 && tipo_activacion == 2) return MODO_LAV_BRI_D;
    }
    else if (tipo_maquina == 1) { // SECADORA
        if (modelo == 0 && tipo_activacion == 1) return MODO_SEC_MAU_S;
        if (modelo == 1 && tipo_activacion == 1) return MODO_SEC_BRI_S;
        if (modelo == 1 && tipo_activacion == 2) return MODO_SEC_BRI_D;
    }

    return MODO_LAV_MAU_S; // Default
}

bool modo_es_doble(ModoOperacion modo) {
    return (modo == MODO_LAV_BRI_D || modo == MODO_SEC_BRI_D);
}

bool modo_usa_rele(ModoOperacion modo) {
    return (modo == MODO_LAV_BRI_S || modo == MODO_LAV_BRI_D ||
            modo == MODO_SEC_BRI_S || modo == MODO_SEC_BRI_D);
}

bool modo_es_secadora(ModoOperacion modo) {
    return (modo == MODO_SEC_MAU_S || modo == MODO_SEC_BRI_S || modo == MODO_SEC_BRI_D);
}
