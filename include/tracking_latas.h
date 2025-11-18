#ifndef TRACKING_LATAS_H
#define TRACKING_LATAS_H

#include "estado_comum.h"

uint32_t Tracking_AdicionarLatinha();

void Tracking_AtualizarEtapa(uint32_t id, EstadoLatas_t novaEtapa);

void Tracking_MarcarCompleta(uint32_t id);

void Tracking_RemoverCompletas();

const Lata_t* Tracking_ObterLista(uint16_t* total);

#endif
