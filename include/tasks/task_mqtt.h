// ===== task_mqtt.h =====
#ifndef TASK_MQTT_H
#define TASK_MQTT_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void TaskMQTT_Init();
    bool TaskMQTT_IsWiFiConnected();
    bool TaskMQTT_IsMQTTConnected();

    bool TaskMQTT_Send(const char *topic, const char *payload, bool retain = false);

    void TaskMQTT_PublishNow();

#ifdef __cplusplus
}
#endif

#endif