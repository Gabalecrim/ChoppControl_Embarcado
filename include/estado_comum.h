#ifndef estado_comum_h
#define estado_comum_h

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
    bool start_processo;
} EstadoSensor_t;

#endif