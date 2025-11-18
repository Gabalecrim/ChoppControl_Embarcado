#ifndef CONFIG_H
#define CONFIG_H

// ========== WiFi Configuration ==========
#define WIFI_SSID "SEU_WIFI_SSID"
#define WIFI_PASSWORD "SUA_SENHA_WIFI"
#define WIFI_TIMEOUT_MS 20000
#define WIFI_RETRY_DELAY_MS 5000

// ========== MQTT Configuration ==========
#define MQTT_BROKER "broker.hivemq.com"  // ou seu broker MQTT
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "ChoppControl_ESP32"
#define MQTT_USERNAME ""  // deixe vazio se não usar autenticação
#define MQTT_PASSWORD ""
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 1
#define MQTT_RETAIN false

// ========== MQTT Topics ==========
#define MQTT_TOPIC_STATUS "choppcontrol/status"
#define MQTT_TOPIC_LATAS "choppcontrol/latas"
#define MQTT_TOPIC_LATAS_UPDATE "choppcontrol/latas/update"
#define MQTT_TOPIC_ERROR "choppcontrol/error"
#define MQTT_TOPIC_STATS "choppcontrol/stats"

// ========== Task Configuration ==========
#define TASK_MQTT_STACK_SIZE 4096
#define TASK_MQTT_PRIORITY 2
#define MQTT_PUBLISH_INTERVAL_MS 1000  // Intervalo de publicação dos dados

#endif