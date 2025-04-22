#include <Preferences.h>
#include "config.h"

Preferences prefs;

// Variables de red y MQTT
int machine_id = 24;
String mqtt_broker = "192.168.1.100";
int mqtt_port = 1883;
String mqtt_publish_topic = "kiosko/1";

char mqtt_topic[32];
char mqtt_message[32];

void construir_topicos() {
  snprintf(mqtt_topic, sizeof(mqtt_topic), "use/%d", machine_id);
  snprintf(mqtt_message, sizeof(mqtt_message), "m%d", machine_id);
}

void cargar_configuracion() {
  prefs.begin("maquina", true);
  machine_id = prefs.getInt("id", 24);
  mqtt_broker = prefs.getString("broker", "192.168.1.100");
  mqtt_port = prefs.getInt("port", 1883);
  mqtt_publish_topic = prefs.getString("pubtopic", "kiosko/1");
  prefs.end();
  construir_topicos();
}

void guardar_machine_id(int id) {
  prefs.begin("maquina", false);
  prefs.putInt("id", id);
  prefs.end();
  machine_id = id;
  construir_topicos();
}

void guardar_mqtt_broker(const char* broker) {
  prefs.begin("maquina", false);
  prefs.putString("broker", broker);
  prefs.end();
  mqtt_broker = broker;
}

void guardar_mqtt_port(int port) {
  prefs.begin("maquina", false);
  prefs.putInt("port", port);
  prefs.end();
  mqtt_port = port;
}

void guardar_mqtt_publish_topic(const char* topic) {
  prefs.begin("maquina", false);
  prefs.putString("pubtopic", topic);
  prefs.end();
  mqtt_publish_topic = topic;
}

// ==========================
// Configuración T1, T2, T3
// ==========================

ConfigNivelTemp config_t1 = { 28, 1, 0, 40, 55 };
ConfigNivelTemp config_t2 = { 28, 1, 0, 55, 65 };
ConfigNivelTemp config_t3 = { 28, 1, 0, 65, 75 };

void guardar_config_t1() {
  prefs.begin("t1", false);
  prefs.putInt("tiempo",     config_t1.tiempo);
  prefs.putInt("cooldown",   config_t1.cooldown);
  prefs.putInt("precio",     config_t1.precio);
  prefs.putInt("temp_baja",  config_t1.temp_baja);
  prefs.putInt("temp_alta",  config_t1.temp_alta);
  prefs.end();
}

void cargar_config_t1() {
  prefs.begin("t1", true);
  config_t1.tiempo     = prefs.getInt("tiempo", 28);
  config_t1.cooldown   = prefs.getInt("cooldown", 1);
  config_t1.precio     = prefs.getInt("precio", 0);
  config_t1.temp_baja  = prefs.getInt("temp_baja", 40);
  config_t1.temp_alta  = prefs.getInt("temp_alta", 55);
  prefs.end();
}

void guardar_config_t2() {
  prefs.begin("t2", false);
  prefs.putInt("tiempo",     config_t2.tiempo);
  prefs.putInt("cooldown",   config_t2.cooldown);
  prefs.putInt("precio",     config_t2.precio);
  prefs.putInt("temp_baja",  config_t2.temp_baja);
  prefs.putInt("temp_alta",  config_t2.temp_alta);
  prefs.end();
}

void cargar_config_t2() {
  prefs.begin("t2", true);
  config_t2.tiempo     = prefs.getInt("tiempo", 28);
  config_t2.cooldown   = prefs.getInt("cooldown", 1);
  config_t2.precio     = prefs.getInt("precio", 0);
  config_t2.temp_baja  = prefs.getInt("temp_baja", 55);
  config_t2.temp_alta  = prefs.getInt("temp_alta", 65);
  prefs.end();
}

void guardar_config_t3() {
  prefs.begin("t3", false);
  prefs.putInt("tiempo",     config_t3.tiempo);
  prefs.putInt("cooldown",   config_t3.cooldown);
  prefs.putInt("precio",     config_t3.precio);
  prefs.putInt("temp_baja",  config_t3.temp_baja);
  prefs.putInt("temp_alta",  config_t3.temp_alta);
  prefs.end();
}

void cargar_config_t3() {
  prefs.begin("t3", true);
  config_t3.tiempo     = prefs.getInt("tiempo", 28);
  config_t3.cooldown   = prefs.getInt("cooldown", 1);
  config_t3.precio     = prefs.getInt("precio", 0);
  config_t3.temp_baja  = prefs.getInt("temp_baja", 65);
  config_t3.temp_alta  = prefs.getInt("temp_alta", 75);
  prefs.end();
}
