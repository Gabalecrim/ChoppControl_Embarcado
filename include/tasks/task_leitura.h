#ifndef TASK_SENSORES_H
#define TASK_SENSORES_H

#include "estado_comum.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    SENSOR_SAIDA_ALIMENTACAO,
    SENSOR_CHEGADA_ENVASE,
    SENSOR_PASSAGEM_TAMPA,
    SENSOR_FINAL_ESTEIRA,
    SENSOR_CHEGADA_GUIA_RECRAVE,
    SENSOR_COUNT
} SensorID;

void TaskSensores_Init(void);

void TaskSensores(void *pvParameters);

EstadoSensor_t *Sensores_GetData(void);

#endif
