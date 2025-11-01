#include "botao_driver.h"
#include "estado_comum.h"
#include <Arduino.h>

static const gpio_num_t botao_pins[BOTAO_COUNT] = {
    [BOTAO_START] = GPIO_NUM_14,
};

static EstadoBotao_t botoes[BOTAO_COUNT] = {
    {BOTAO_START, false},
};

void Botao_Init(void)
{
    for (int i = 0; i < BOTAO_COUNT; i++)
    {
        pinMode(botao_pins[i], INPUT_PULLDOWN);
    }
    Serial.begin(115200);
}

bool Leitura_Botao(BotaoID id)
{
    bool ultimo_estado_botao = false;
    TickType_t ultimo_tempo_mudanca = 0;
    const TickType_t debounce_delay = pdMS_TO_TICKS(50);

    bool leitura_atual = !digitalRead(botao_pins[id]);
    // Serial.println(leitura_atual);

    if (leitura_atual != ultimo_estado_botao)
    {
        botoes[id].estado = !botoes[id].estado;
        // Serial.println("Botao pressionado");

        // ultimo_tempo_mudanca = xTaskGetTickCount();
        ultimo_estado_botao = leitura_atual;
    }

    // if (leitura_atual && (xTaskGetTickCount() - ultimo_tempo_mudanca) > debounce_delay)
    // {
    //     botoes[id].estado = !botoes[id].estado;
    //     Serial.println("Botao pressionado");

    //     while (!digitalRead(botao_pins[id]))
    //     {
    //         vTaskDelay(pdMS_TO_TICKS(10));
    //     }
    // }

    return botoes[id].estado;
}