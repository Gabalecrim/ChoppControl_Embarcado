#include "drivers/botao_driver.h"
#include "estado_comum.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <Arduino.h>

static const gpio_num_t botao_pins[BOTAO_COUNT] = {
    [BOTAO_START] = GPIO_NUM_14,
};

static EstadoBotao_t botoes[BOTAO_COUNT] = {
    {BOTAO_START, false},
};

static Debounce_t debounce[BOTAO_COUNT];
static const TickType_t debounce_delay = pdMS_TO_TICKS(50);

void Botao_Init(void)
{
    for (int i = 0; i < BOTAO_COUNT; i++)
    {
        pinMode(botao_pins[i], INPUT_PULLDOWN);
        debounce[i].estado_estavel = false;
        debounce[i].ultimo_leitura = false;
        debounce[i].ultimo_tempo = xTaskGetTickCount();
    }
    Serial.begin(115200);
}

bool Leitura_Botao(BotaoID id)
{
    bool leitura_atual = !digitalRead(botao_pins[id]);
    TickType_t agora = xTaskGetTickCount();

    if (leitura_atual != debounce[id].ultimo_leitura)
    {
        debounce[id].ultimo_tempo = agora;
        debounce[id].ultimo_leitura = leitura_atual;
    }

    if (leitura_atual && (xTaskGetTickCount() - debounce[id].ultimo_tempo) > debounce_delay)
    {
        botoes[id].estado = !botoes[id].estado;

        while (!digitalRead(botao_pins[id]))
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
    return botoes[id].estado;
}