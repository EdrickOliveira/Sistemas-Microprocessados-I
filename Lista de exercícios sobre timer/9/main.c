#include "stm32f4xx.h"

/*
1/Frequencia da nota = Periodo da nota

Periodo da nota/2 = periodo de ativacao do TIMxx->SR

ARR x PSC = 16.000.000 x periodo de ativacao

ARR fixo em 2:
PSC = (16.000.000 x periodo de ativacao)/2
TIMxx->PSC = PSC-1
*/
enum nota {
	OFF=0, DO=15293, DOS=14388, RE=13620, RES=12856, MI=12134,
	FA=11453, FAS=10810, SOL=10203, SOLS=9631, LA=9090,
	LAS=8580, SI=8098
};

int nota_a[] = {DO, DOS, RE, RES, MI, FA, FAS, SOL, SOLS, LA, LAS, SI, OFF};

void inicializar();
void atualizar_nota();

int main(void)
{
	inicializar();

	while(1) {
		atualizar_nota();

		if(TIM11->PSC == OFF)	GPIOA->ODR = 0;

		//quando passar o periodo de ativacao
		else if(TIM11->SR & 1) {
			GPIOA->ODR ^= 1;

			TIM11->SR &= ~(0x1);
			TIM11->CNT = 0;
		}
	}
}

void inicializar(){
    RCC->AHB1ENR = 0x87;
    GPIOA->MODER |= 0x28000000;

    //Entradas: PC0 a PC11
    GPIOC->MODER &= ~(0x555555);
    GPIOC->PUPDR |= 0xAAAAAA;	//pull-down

    //Saida: PA0
    GPIOA->MODER |= 1;

	RCC->APB2ENR |= 0x40000;	//ativa o timer 11
	TIM11->ARR = 1;         	//ARR = 2
	TIM11->PSC = OFF;       	//Nota = OFF
	TIM11->CR1 |= 1;        	//ativa o clock do timer 11
}

void atualizar_nota() {
	int i;

	for(i=0; i<12; i++){
		if((GPIOC->IDR & 0xFFF)==(1<<i)){
			TIM11->PSC = nota_a[i];
			break;
		}
	}
	if(i==12 && TIM11->PSC != OFF)	TIM11->PSC = OFF;
}
