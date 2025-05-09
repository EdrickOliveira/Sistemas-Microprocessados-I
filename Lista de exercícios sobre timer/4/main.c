#include "stm32f4xx.h"

typedef struct
{
    int verde, amarelo, vermelho;
} sinaleira_t;

void inicializar();
void atualizar_leds(sinaleira_t sinaleira, int offset);

//------------------------------------------------
int main(void)
{
    int tempo;  // tempo decorrido em decisegundos (0,1s)

    // cria e configura as sinaleiras
    sinaleira_t sinaleira[2];
    sinaleira[0].verde = 1;
    sinaleira[0].amarelo = 0;
    sinaleira[0].vermelho = 0;
    sinaleira[1].verde = 0;
    sinaleira[1].amarelo = 0;
    sinaleira[1].vermelho = 1;

    inicializar();

    while (1)
    {
        tempo = 0;  // tempo decorrido em decisegundos (0,1s)
        do
        {
            // quando passar 0,1s
            if (TIM11->SR & 1)
            {
                TIM11->SR &= ~1;
                tempo++;  // tempo soma 1 (100ms)

                switch (tempo)
                {
                // quando bater 500ms
                case 5:
                	sinaleira[0].amarelo = 1;	// liga o amarelo
                	atualizar_leds(sinaleira[0], 0);
					atualizar_leds(sinaleira[1], 3);
                    break;
                // quando bater 700ms
                case 7:
                    // sinaleira 0 para vermelho
                    sinaleira[0].verde = 0;
                    sinaleira[0].amarelo = 0;
                    sinaleira[0].vermelho = 1;

                    // sinaleira 1 para verde
                    sinaleira[1].verde = 1;
                    sinaleira[1].vermelho = 0;

                    atualizar_leds(sinaleira[0], 0);
					atualizar_leds(sinaleira[1], 3);

                    break;
                }
            }
        } while (tempo < 7);  // recomeca o ciclo com sinaleiras 0 e 1 invertidas

        tempo = 0;  // tempo decorrido em decisegundos (0,1s)

        //trocar os indices
		do
		{
			// quando passar 0,1s
			if (TIM11->SR & 1)
			{
				TIM11->SR &= ~1;
				tempo++;  // tempo soma 1 (100ms)

				switch (tempo)
				{
				// quando bater 500ms
				case 5:
					sinaleira[0].amarelo = 1;	// liga o amarelo
					atualizar_leds(sinaleira[0], 0);
					atualizar_leds(sinaleira[1], 3);
					break;
				// quando bater 700ms
				case 7:
					// sinaleira 0 para vermelho
					sinaleira[0].verde = 0;
					sinaleira[0].amarelo = 0;
					sinaleira[0].vermelho = 1;

					// sinaleira 1 para verde
					sinaleira[1].verde = 1;
					sinaleira[1].vermelho = 0;

					atualizar_leds(sinaleira[0], 0);
					atualizar_leds(sinaleira[1], 3);

					break;
				}
			}
		} while (tempo < 7);  // recomeca o ciclo com sinaleiras 0 e 1 invertidas
    }
}
//------------------------------------------------

void inicializar()
{
    // clock e programador
    RCC->AHB1ENR = 0x87;
    GPIOA->MODER |= 0x28000000;

    // PC0 a PC5 saídas
    GPIOC->MODER = 0x555;

    RCC->APB2ENR |= 0x40000;  // ativa o timer 11
    TIM11->ARR = 999;       // ARR = 1000
    TIM11->PSC = 1599;      // PSC = 1600
    TIM11->CR1 |= 0x1;        // ativa o clock do timer 11
}

void atualizar_leds(sinaleira_t sinaleira, int offset)
{
    // atualizar o valor de PC0 a PC2, ou PC3 a PC5 dependendo da sinaleira (offset é 0 ou 3)
    if (sinaleira.verde)
        GPIOC->ODR |= (1 << (0 + offset));
    else
        GPIOC->ODR &= ~(1 << (0 + offset));

    if (sinaleira.amarelo)
        GPIOC->ODR |= (1 << (1 + offset));
    else
        GPIOC->ODR &= ~(1 << (1 + offset));

    if (sinaleira.vermelho)
        GPIOC->ODR |= (1 << (2 + offset));
    else
        GPIOC->ODR &= ~(1 << (2 + offset));
}
