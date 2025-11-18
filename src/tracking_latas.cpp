#include "tracking_latas.h"

#define MAX_LATINHAS 50

static Lata_t lista_latinhas[MAX_LATINHAS];
static uint16_t total_latinhas = 0;

uint32_t Tracking_AdicionarLatinha()
{
    if (total_latinhas >= MAX_LATINHAS)
        return 0;

    Lata_t nova = {
        .id = total_latinhas + 1,
        .etapa = AGUARDANDOENVASE,
        .tempoEntrada = xTaskGetTickCount(),
        .tempoSaida = 0,
        .completa = false};

    lista_latinhas[total_latinhas++] = nova;
    return nova.id;
}

void Tracking_AtualizarEtapa(uint32_t id, EstadoLatas_t novaEtapa)
{
    for (int i = 0; i < total_latinhas; i++)
    {
        if (lista_latinhas[i].id == id)
        {
            lista_latinhas[i].etapa = novaEtapa;
            lista_latinhas[i].tempoEntrada = xTaskGetTickCount();
            break;
        }
    }
}

void Tracking_MarcarCompleta(uint32_t id)
{
    for (int i = 0; i < total_latinhas; i++)
    {
        if (lista_latinhas[i].id == id)
        {
            lista_latinhas[i].completa = true;
            lista_latinhas[i].tempoSaida = xTaskGetTickCount();
            break;
        }
    }
}

void Tracking_RemoverCompletas()
{
    uint16_t j = 0;
    for (int i = 0; i < total_latinhas; i++)
    {
        if (!lista_latinhas[i].completa)
            lista_latinhas[j++] = lista_latinhas[i];
    }
    total_latinhas = j;
}

const Lata_t* Tracking_ObterLista(uint16_t* total)
{
    if (total != nullptr)
        *total = total_latinhas;
    return lista_latinhas;
}
