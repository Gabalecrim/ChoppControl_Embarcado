#ifndef BotaoDriver_h
#define BotaoDriver_h

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

void Botao_Init(void);

bool Leitura_Botao(BotaoID id);

#endif