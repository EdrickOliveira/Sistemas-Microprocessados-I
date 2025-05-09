#include "stm32f4xx.h"

void inicializar();

//---------------------------------
int main(void)
{
	inicializar();

	while (1)
	{
		if(TIM11->SR&1)	//se UIF == 1
		{
			GPIOC->ODR ^= 1;	//troca o estado do led (PC0)
			TIM11->SR &= ~1;	//UIF = 0
		}
	}
}
//---------------------------------

void inicializar()
{
	//Habilita o clock dos GPIOs
	RCC->AHB1ENR = 0x87;

	//Habilita portas do programador
	GPIOA->MODER |= 0x28000000;

	//PC0 saida
	GPIOC->MODER |= 1;

	RCC->APB2ENR |= 0x40000;	//Habilita o clock to timer 11 (bit 18)
	TIM11->PSC = 1599;			//PSC e 1600
	TIM11->ARR = 999;			//ARR e 1000
	TIM11->CR1 |= 0x01;			//Habilita contagem timer 11
}
