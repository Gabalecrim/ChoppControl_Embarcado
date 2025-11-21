#ifndef CONFIG_H
#define CONFIG_H

// ========== WiFi Configuration ==========
#define WIFI_SSID "Gab"
#define WIFI_PASSWORD "GabrielAlecrim"
#define WIFI_TIMEOUT_MS 20000
#define WIFI_RETRY_DELAY_MS 5000

// ========== MQTT Configuration ==========
#define MQTT_BROKER "192.168.137.219"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "ChoppControl_ESP32"
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 1
#define MQTT_RETAIN true

// ========== MQTT Topics ==========
#define MQTT_TOPIC_STATUS "choppcontrol/status"
#define MQTT_TOPIC_LATAS "choppcontrol/latas"
#define MQTT_TOPIC_LATAS_UPDATE "choppcontrol/latas/update"
#define MQTT_TOPIC_ERROR "choppcontrol/error"
#define MQTT_TOPIC_STATS "choppcontrol/stats"

// ========== Task Configuration ==========
#define TASK_MQTT_STACK_SIZE 12288
#define TASK_MQTT_PRIORITY 2
#define MQTT_PUBLISH_INTERVAL_MS 1000 // Intervalo de publicação dos dados

#endif