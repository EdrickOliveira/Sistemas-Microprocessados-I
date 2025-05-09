#include "stm32f4xx.h"

void inicializar();
void TIM1_TRG_COM_TIM11_IRQHandler(void);
void TIM1_UP_TIM10_IRQHandler(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    inicializar();

    while (1)
    {

    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void inicializar(){
    RCC->AHB1ENR=0x87;    //habilitando o clock dos GPIOs
    GPIOA->MODER =0x28000400;    //configura PA5 como sa�da
    GPIOC->MODER=0x00;

    //configura timer11 para 1s
    RCC->APB2ENR|=1<<18;    //habilita clock timer 11
    TIM11->PSC=15999;
    TIM11->ARR=999;
    TIM11->CR1 |=0x01;  //Habilita contagem timer 11

    TIM11->DIER|=0x01;  //habilita a interrup��o do timer 11

    NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 5);   //Priority 5
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);


    RCC->APB2ENR|=1<<17;    //habilita clock timer 10
    TIM10->PSC=15999;
    TIM10->ARR=99;
    TIM10->CR1 |=0x01; //Habilita contagem timer 11

    TIM10->DIER|=0x01;    //habilita a interrup��o do timer 11

    NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 2);    //Priority 2
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

void TIM1_TRG_COM_TIM11_IRQHandler(void){
    GPIOA->ODR ^=0x20;    //troca o estado do led
    TIM11->SR &=~0x01;    //limpando o flag
}

void TIM1_UP_TIM10_IRQHandler(void){
    GPIOA->ODR ^=0x20;    //troca o estado do led
    TIM10->SR &=~0x01;    //limpando o flag
}
