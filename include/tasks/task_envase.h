#ifndef TASK_ENVASE_H
#define TASK_ENVASE_H

#include "task_leitura.h"

typedef enum
{
    AGUARDANDO_LATA,
    ENVASE,
} EstadoEnvase_t;

void TaskEnvase_Init(void);

void task_envase(void *pvParameters);

#endif