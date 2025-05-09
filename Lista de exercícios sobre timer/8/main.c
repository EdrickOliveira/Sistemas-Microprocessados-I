#include "stm32f4xx.h"

typedef enum{
    DIREITA=0, ESQUERDA
}sentido_t;

sentido_t sentido = ESQUERDA;
uint32_t fim_linha;

void inicializar();
void deslocar_led(sentido_t sentido);
void efeito();

/////////////////////////////////////////////////////////////////////
int main(){
    inicializar();

    while(1){
    	GPIOC->ODR = 0;
    	efeito();
    }
}
/////////////////////////////////////////////////////////////////////

void inicializar(){
    RCC->AHB1ENR = 0x87;
    GPIOA->MODER |= 0x28000000;

    //Saidas: PC0 a PC7
    GPIOC->MODER |= 0x5555;
    //Entradas: PA0 e PA1
    GPIOA->MODER &= ~(0x3);
    GPIOA->PUPDR |= 0xA;	//pull-down

    //periodo = 250ms
	RCC->APB2ENR |= 0x40000;  //ativa o timer 11
	TIM11->ARR = 1999;      //ARR
	TIM11->PSC = 1999;      //PSC
	TIM11->CR1 |= 1;        //ativa o clock do timer 11
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

void efeito(){
	int operando_o_5 = 0;
	sentido = ESQUERDA;

	while((GPIOA->IDR & 0x3)==0){
		if(operando_o_5==0){
			GPIOC->ODR = 1;
			operando_o_5 = 1;
		}
		//entra aqui a cada 0.5s
		if(TIM11->SR & 1){
			TIM11->SR &= ~1;

			deslocar_led(sentido);
			//se o LED aceso estiver na ponta, inverte o sentido
			if((GPIOC->ODR & 0x80) || (GPIOC->ODR & 1))   sentido^=1;
		}
	}
	operando_o_5 = 0;

	while((GPIOA->IDR & 0x3)==1){
		GPIOC->ODR = 0;
		int i=0;

		do{
			if(TIM11->SR & 1){
				TIM11->SR &= ~1;

				GPIOC->ODR |= (1 << i);
				i++;
			}
		}while((GPIOC->ODR != 0xFF) && (GPIOA->IDR & 0x3)==1);

		i=0;
		do{
			if(TIM11->SR & 1){
				TIM11->SR &= ~1;

				GPIOC->ODR &= ~(0x80 >> i);
				i++;
			}
		}while(GPIOC->ODR != 0 && (GPIOA->IDR & 0x3)==1);
	}

	sentido = ESQUERDA;
	while((GPIOA->IDR & 0x3)==2){
		GPIOC->ODR = 1;
		fim_linha = 0x2;
		while(fim_linha<=0x80 && (GPIOA->IDR & 0x3)==2){
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
		while(fim_linha>=0x2 && (GPIOA->IDR & 0x3)==2){
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
