#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// CONFIGURACIÓN DE RED Y MQTT
extern int machine_id;
extern String mqtt_broker;
extern int mqtt_port;
extern String mqtt_publish_topic;

extern char mqtt_topic[32];   // ej: "use/24"
extern char mqtt_message[32]; // ej: "m24"

// TIPOS Y COMANDOS DE MÁQUINA
enum TipoMaquina
{
    LAVADORA,
    SECADORA
};
enum ModeloMaquina
{
    MAUTRA,
    BRIDGE
};
enum TipoActivacion
{
    SIMPLE,
    DOBLE
};

typedef struct
{
    TipoMaquina tipo;
    ModeloMaquina modelo;
    TipoActivacion activacion;
} ComandoMaquina;

// PARÁMETROS: TIEMPOS Y TEMPERATURA POR NIVEL
typedef struct
{
    int tiempo;    // duración del ciclo (min o seg)
    int cooldown;  // tiempo de enfriamiento
    int precio;    // costo del nivel
    int temp_baja; // temperatura mínima
    int temp_alta; // temperatura máxima
} ConfigNivelTemp;

extern ConfigNivelTemp config_t1;
extern ConfigNivelTemp config_t2;
extern ConfigNivelTemp config_t3;

// CONFIGURACIÓN DE TIEMPOS Y MONEDAS
extern int tiempo_extra;  // tiempo por moneda extra
extern int tiempo_valor1; // tiempo por tipo de moneda 1
extern int moneda_valor1; // valor monetario 1
extern int tiempo_valor2;
extern int moneda_valor2;

// CONFIGURACIÓN DE GIRO DE CANASTA
typedef struct
{
    int tiempo_izq;
    int pausa;
    int tiempo_der;
} ConfigGiro;

extern ConfigGiro etapa_1;
extern ConfigGiro etapa_2;
extern ConfigGiro etapa_3;

// FUNCIONES DE CONFIGURACIÓN
// Carga general
void cargar_configuracion(); // Carga ID, MQTT y tópicos
void construir_topicos();    // Crea mqtt_topic/mensaje

// Guardar individualmente
void guardar_machine_id(int id);
void guardar_mqtt_broker(const char *broker);
void guardar_mqtt_port(int port);
void guardar_mqtt_publish_topic(const char *topic);

// FSM general (si la usas en lavadora también)
typedef struct
{
    int tiempo_bajo;
    int tiempo_medio;
    int tiempo_alto;
    float temp_min;
    float temp_max;
    int modo;
} ConfigMaquinaFSM;

extern ConfigMaquinaFSM config_maquina;

void guardar_config_maquina();
void cargar_config_maquina();

// Funciones por bloque de secadora
void guardar_config_t1();
void guardar_config_t2();
void guardar_config_t3();
void guardar_config_valores();
void guardar_config_giros();

void cargar_config_t1();
void cargar_config_t2();
void cargar_config_t3();
void cargar_config_valores();
void cargar_config_giros();

#endif
