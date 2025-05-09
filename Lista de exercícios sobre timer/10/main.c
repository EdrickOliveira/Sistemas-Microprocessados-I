#include "stm32f4xx.h"

void inicializar(int primeira_nota);
int oitava_acima(int nota);
int oitava_abaixo(int nota);

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

int main(void)
{
	int MI_abaixo = oitava_abaixo(MI);
	int FA_abaixo = oitava_abaixo(FA);
	int SOL_abaixo = oitava_abaixo(SOL);
	int LA_abaixo = oitava_abaixo(LA);
	int LAS_abaixo = oitava_abaixo(LAS);
	int SI_abaixo = oitava_abaixo(SI);

	int DO_acima = oitava_acima(DO);
	int RE_acima = oitava_acima(RE);
	int MI_acima = oitava_acima(MI);
	int FA_acima = oitava_acima(FA);

	int waltz_n2[] = {LA_abaixo, LA_abaixo, LA_abaixo, LA_abaixo, LA_abaixo, LA_abaixo, MI_abaixo, MI_abaixo, MI_abaixo, MI_abaixo, MI_abaixo, MI_abaixo, LA_abaixo, LA_abaixo, LA_abaixo, LA_abaixo, LA_abaixo, LA_abaixo, MI_abaixo, MI_abaixo, MI_abaixo, MI_abaixo, MI_abaixo, MI_abaixo, LA, LA, LA, LA, LA, LA, FA, FA, FA, FA, MI, MI, RE, RE, RE, RE, RE, RE, RE, RE, RE, RE, MI, MI, FA, FA, RE, RE, FA, FA, LA, LA, LA, LA, LAS, LAS, LA, LA, LA, LA, LA, LA, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, MI, MI, MI, MI, RE, RE, DOS, DOS, DOS, DOS, DOS, DOS, DOS, DOS, LA_abaixo, LA_abaixo, DOS, DOS, MI, MI, DOS, DOS, MI, MI, SOL, SOL, LA, LA, LAS, LAS, SOLS, SOLS, SOLS, SOLS, SOLS, SOLS, LA, LA, LA, LA, LA, LA, FA_acima, FA_acima, FA_acima, FA_acima, FA_acima, FA_acima, MI_acima, MI_acima, MI_acima, MI_acima, RE_acima, RE_acima, DO_acima, DO_acima, DO_acima, DO_acima, LAS, LAS, SOL, SOL, SOL, SOL, SOL, SOL, MI_acima, MI_acima, MI_acima, MI_acima, MI_acima, MI_acima, RE_acima, RE_acima, RE_acima, RE_acima, DO_acima, DO_acima, DO_acima, DO_acima, DO_acima, DO_acima, LA, LA, FA, FA, FA, FA, SOL, SOL, LA, LA, LA, SOL, LA, LAS, SOL, SOL, SOL, FA, SOL, LA, FA, FA, FA, FA, LA, LA, LA, LA, FA, FA, SOL, SOL, LA, LA, LA, SOL, LA, LAS, SOL, SOL, SOL, FA, SOL, LA, FA, FA, FA, FA, DO, DO, FA, FA, FA, FA, DO, DO, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, MI, MI, RE, RE, DO, DO, LA_abaixo, LA_abaixo, DO, DO, MI, MI, MI, MI, MI, MI, MI, MI, MI, MI, MI, MI, RE, RE, DO, DO, LA_abaixo, LA_abaixo, FA_abaixo, FA_abaixo, SOL_abaixo, SOL_abaixo, LA_abaixo, LA_abaixo, RE, RE, RE, RE, RE, RE, DO, DO, DO, DO, DO, DO, DO, DO, LAS_abaixo, LAS_abaixo, LA_abaixo, LA_abaixo, SOL_abaixo, SOL_abaixo, FA_abaixo, FA_abaixo, SOL_abaixo, SOL_abaixo, LA_abaixo, LA_abaixo, LA_abaixo, LA_abaixo, DO, DO, SOL_abaixo, SOL_abaixo, SOL_abaixo, SOL_abaixo, DO, DO, LA_abaixo, LA_abaixo, LA_abaixo, LA_abaixo, DO, DO, FA, FA, FA, FA, SOL, SOL, LA, LA, LA, LA, LA, LA, LA, LA, LA, LA, LA, LA, LA, LA, SOL, SOL, FA, FA, MI, MI, DO, DO, MI, MI, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, SOL, FA, FA, MI, MI, RE, RE, LA_abaixo, LA_abaixo, RE, RE, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, FA, SOL, SOL, FA, FA, SOL, SOL, LA, LA, LA, LA, FA, FA, DO, DO, RE, RE, MI, MI, FA, FA, SOL, SOL, FA, FA, RES, RES, RE, RE, DOS, DOS, RE, RE, RE, RE, FA_abaixo, FA_abaixo, FA_abaixo, FA_abaixo, MI_abaixo, MI_abaixo, FA_abaixo, FA_abaixo, RE, RE, RE, RE, FA_abaixo, FA_abaixo, FA_abaixo, FA_abaixo, RES, RES, RE, RE, RE, RE, RE, RE, DO, DO, SI_abaixo, SI_abaixo, SI_abaixo, SI_abaixo, DO, DO, SOL, SOL, SOL, SOL, FA, FA, RES, RES, RE, RE, DOS, DOS, RE, RE, RE, RE, FA_abaixo, FA_abaixo, FA_abaixo, FA_abaixo, MI_abaixo, MI_abaixo, FA_abaixo, FA_abaixo, RE, RE, RE, RE, FA_abaixo, FA_abaixo, FA_abaixo, FA_abaixo, SOL_abaixo, SOL_abaixo, LA_abaixo, LA_abaixo, LAS_abaixo, LAS_abaixo, LAS_abaixo, LAS_abaixo, DO, DO, RE, RE, RE, RE, RE, RE, MI, MI, RE, RE, MI, MI, FA, MI, FA, SOL, LA, LA, LA, LA};

	inicializar(LA_abaixo);

	int i=0;
	while(1) {
		//quando passar 1/2 tempo, passar para a proxima nota
		if(TIM10->SR & 1){
			i++;
			TIM11->PSC = waltz_n2[i];

			TIM10->SR &= ~(0x1);
			TIM10->CNT = 0;
		}

		//quando passar o periodo de ativacao
		if(TIM11->SR & 1) {
			GPIOA->ODR ^= 1;

			if(i==(sizeof(waltz_n2) / sizeof(waltz_n2[0]))){
				i=0;
				TIM11->PSC = waltz_n2[i];
			}
			TIM11->SR &= ~(0x1);
			TIM11->CNT = 0;
		}
	}
}

void inicializar(int primeira_nota){
    RCC->AHB1ENR = 0x87;
    GPIOA->MODER |= 0x28000000;

    //Entradas: PC0 a PC11
    GPIOC->MODER &= ~(0x555555);
    GPIOC->PUPDR |= 0xAAAAAA;	//pull-down

    //Saida: PA0
    GPIOA->MODER |= 1;

	RCC->APB2ENR |= 0x40000;	//ativa o timer 11
	TIM11->ARR = 1;         	//ARR = 2
	TIM11->PSC = primeira_nota;		//primeira nota
	TIM11->CR1 |= 0x81;        	//ativa o clock do timer 11

	//150ms (meio tempo - um tempo e 300ms)
	RCC->APB2ENR |= 0x20000;	//ativa o timer 10
	TIM10->ARR = 2399;         	//ARR = 2400
	TIM10->PSC = 999;			//PSC = 1000
	TIM10->CR1 |= 0x81;        	//ativa o clock do timer 11
}

int oitava_acima(int nota){
	nota++;
	if(nota%2)	nota/=2;
	else{
		nota/=2;
		nota--;
	}

	return nota;
}

int oitava_abaixo(int nota){
	nota++;
	nota*=2;
	nota--;

	return nota;
}
