#ifndef BotaoDriver_h
#define BotaoDriver_h
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "estado_comum.h"
#include <driver/gpio.h>
typedef enum
{
    BOTAO_START,
    BOTAO_COUNT
} BotaoID;

typedef struct
{
    BotaoID id;
    bool estado;
} EstadoBotao_t;

typedef struct
{
    bool estado_estavel;
    bool ultimo_leitura;
    TickType_t ultimo_tempo;
} Debounce_t;

void Botao_Init(void);

bool Leitura_Botao(BotaoID id);

#endif