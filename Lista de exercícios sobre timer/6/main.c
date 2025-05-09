#include "stm32f4xx.h"

void inicializar();

///////////////////////////////////////////////////
int main(){
    int i;
    
    inicializar();

    while(1){
        i=0;
        do{
            if(TIM11->SR & 1){
                TIM11->SR &= ~1;

                GPIOC->ODR |= (1 << i);
                i++;
            }
        }while(GPIOC->ODR != 0xFF);

        i=0;
        do{
            if(TIM11->SR & 1){
                TIM11->SR &= ~1;

                GPIOC->ODR &= ~(0x80 >> i);
                i++;
            }
        }while(GPIOC->ODR != 0);
    }
}
///////////////////////////////////////////////////

void inicializar(){
    RCC->AHB1ENR = 0x87;
    GPIOA->MODER |= 0x28000000;

    //periodo = 0,5s
    RCC->APB2ENR |= 0x40000;  //ativa o timer 11
    TIM11->ARR = 1999;
    TIM11->PSC = 3999;
    TIM11->CR1 |= 1;	//ativa o clock to timer 11

    GPIOC->MODER |= 0x5555;
    GPIOC->ODR = 0;
}
