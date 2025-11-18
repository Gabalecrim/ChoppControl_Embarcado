#ifndef TASK_MQTT_H
#define TASK_MQTT_H

#include <Arduino.h>

/**
 * @brief Inicializa a task MQTT
 * 
 * Cria a task que gerencia a conexão WiFi e MQTT,
 * e publica periodicamente os dados das latinhas
 */
void TaskMQTT_Init();

/**
 * @brief Verifica se está conectado ao WiFi
 * @return true se conectado, false caso contrário
 */
bool TaskMQTT_IsWiFiConnected();

/**
 * @brief Verifica se está conectado ao broker MQTT
 * @return true se conectado, false caso contrário
 */
bool TaskMQTT_IsMQTTConnected();

/**
 * @brief Publica mensagem em um tópico específico
 * @param topic Tópico MQTT
 * @param payload Dados a serem publicados
 * @return true se publicado com sucesso, false caso contrário
 */
bool TaskMQTT_Publish(const char* topic, const char* payload);

/**
 * @brief Força a publicação imediata dos dados das latinhas
 */
void TaskMQTT_PublishNow();

#endif
