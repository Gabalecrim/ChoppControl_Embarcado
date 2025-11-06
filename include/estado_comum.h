#ifndef estado_comum_h
#define estado_comum_h

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

typedef enum
{
    AGUARDANDO,
    AUTOMATICO,
    MANUAL,
    ERROR
} EstadoMaquina_t;

typedef struct
{
    bool saida_alimentacao;
    bool chegada_envase;
    bool passagem_tampa;
    bool final_esteira;
    bool chegada_guia_recrave;
    bool botao_ciclo;
} EstadoSensor_t;

typedef enum
{
    AGUARDANDOENVASE,
    ENVASANDO,
    AGUARDANDOTAMPA,
    TAMPANDO,
    AGUADANDORECRAVE,
    RECRAVANDO,
    FINALIZADA,
} EstadoLatas_t;

typedef struct
{
    uint32_t id;
    EstadoLatas_t etapa;
    TickType_t tempoEntrada;
    TickType_t tempoSaida;
    bool completa;
} Lata_t;

extern SemaphoreHandle_t xSemAlimentacao;
extern SemaphoreHandle_t xSemEnvase;
extern SemaphoreHandle_t xSemTampa;
extern SemaphoreHandle_t xSemFimEsteira;
extern SemaphoreHandle_t xSemRecrave;
extern SemaphoreHandle_t xMutexSensores;

extern TickType_t tempo_inicio_envase;

#endif