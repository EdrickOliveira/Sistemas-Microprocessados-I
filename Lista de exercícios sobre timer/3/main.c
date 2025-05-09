#include "stm32f4xx.h"

void inicializar();

//---------------------------------------------------------------------------------
int main(void)
{
	inicializar();
	
	while(1){
		int codigo = (GPIOB->IDR&0x38)>>3;
		
		TIM11->CNT = 0;
	
		GPIOA->ODR |= 0x100;	//comeca ligado

		int i=1; 
		do{
			if(TIM11->SR & 1){
				i++;
				TIM11->SR &= ~1;	//zera o UIF
				
				if(i<=(codigo*2)){
					GPIOA->ODR ^= 0x100;	//inverte o estado de PA8
				}

			}
		}while(i<11);
	}
}
//---------------------------------------------------------------------------------

void inicializar()
{
	//Habilita o clock dos GPIOs
	RCC->AHB1ENR = 0x87;

	//Habilita portas do programador
	GPIOA->MODER |= 0x28000000;

	//Pull-down no PB3, 4 e 5
	GPIOB->PUPDR = 0xA80;
	
	//PA8 saida
	GPIOA->MODER |= 0x10000;

	RCC->APB2ENR |= 0x40000;	//Habilita o clock to timer 11 (bit 18)
	// 5Hz x 2
	TIM11->PSC = 1599;			//PSC e 1600
	TIM11->ARR = 999;			//ARR e 1000
	TIM11->CR1 |= 0x01;			//Habilita contagem timer 11
}
