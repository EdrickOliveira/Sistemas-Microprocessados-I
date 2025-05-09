#include "stm32f4xx.h"

typedef enum{
    DIREITA=0, ESQUERDA
}sentido_t;

void inicializar();
void deslocar_led(sentido_t sentido);

/////////////////////////////////////////////////////////////////////
int main(){
    sentido_t sentido = ESQUERDA;
    uint32_t fim_linha;

    inicializar();

    while(1){
    	fim_linha = 0x2;
    	while(fim_linha<=0x80){
    		//entra aqui a cada 250ms
			if(TIM11->SR & 1){
				TIM11->SR &= ~1;

				deslocar_led(sentido);

				if((GPIOC->ODR & fim_linha) || (GPIOC->ODR & 1)){
					sentido^=1;
					if(GPIOC->ODR & fim_linha)	fim_linha = (fim_linha<<1);
				}
			}
    	}

    	fim_linha = 0x80;
    	while(fim_linha>=0x2){
			//entra aqui a cada 250ms
			if(TIM11->SR & 1){
				TIM11->SR &= ~1;

				deslocar_led(sentido);

				if((GPIOC->ODR & fim_linha) || (GPIOC->ODR & 1)){
					sentido^=1;
					if(GPIOC->ODR & 1)	fim_linha = (fim_linha>>1);
				}
			}
		}
    }
}
/////////////////////////////////////////////////////////////////////

void inicializar(){
    RCC->AHB1ENR = 0x87;
    GPIOA->MODER |= 0x28000000;

    //periodo = 250ms
    RCC->APB2ENR |= 0x40000;  //ativa o timer 11
    TIM11->ARR = 1999;      //ARR
    TIM11->PSC = 1999;      //PSC
    TIM11->CR1 |= 1;        //ativa o clock do timer 11

    GPIOC->MODER |= 0x5555;
    GPIOC->ODR = 1;
}

void deslocar_led(sentido_t sentido){
    switch(sentido){
        case ESQUERDA:
        	GPIOC->ODR = GPIOC->ODR << 1;
            break;
        case DIREITA:
        	GPIOC->ODR = GPIOC->ODR >> 1;
            break;
    }
}
