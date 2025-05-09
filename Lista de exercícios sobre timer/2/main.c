#include "stm32f4xx.h"

void inicializar();
int config_freq(int e_anterior);

//---------------------------------
int main(void)
{
	int e_anterior= ~0;

	inicializar();

	while (1)
	{
		e_anterior = config_freq(e_anterior);
		if(TIM10->SR&1)	//se UIF == 1
		{
			GPIOB->ODR ^= 1;	//troca o estado do LED
			TIM10->SR &= ~1;	//UIF = 0
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

	//PB0 saida
	GPIOB->MODER |= 1;

	//pull-down no PC1
	GPIOC->PUPDR = 8;

	RCC->APB2ENR |= 0x20000;	//Habilita o clock do timer 10 (bit 17)
	TIM10->CR1 |= 0x01;			//Habilita contagem timer 10
	TIM10->ARR = 11428;			//ARR = 11429
}

int config_freq(int e_anterior)
{
	if(GPIOC->IDR == e_anterior)
	{
		return e_anterior;
	}

	//se a entrada for 1, freq = 2Hz
	if((GPIOC->IDR & 2)==2)
	{
		TIM10->PSC = 350;
	}
	//se a entrada for 0, freq = 7Hz
	else
	{
		TIM10->PSC = 100;
	}

	return GPIOC->IDR;
}
