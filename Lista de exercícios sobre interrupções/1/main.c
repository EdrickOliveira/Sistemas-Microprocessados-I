#include "stm32f4xx.h"

int number = 0;

void setup();
void TIM1_UP_TIM10_IRQHandler(void);
void TIM1_TRG_COM_TIM11_IRQHandler(void);
uint32_t convert_to_display(int num);
int get_digit(int num, uint32_t display);

//////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    setup();

    while(1){

    }
}
//////////////////////////////////////////////////////////////////////////////////

void setup(){
    RCC->AHB1ENR = 0x87;
    GPIOA->MODER = 0x28000400;

    //PB0 a PB3 = saidas
    GPIOB->MODER = 0x55;
    GPIOB->ODR = ~(0b0001);
    //PC0 a PC6 = saidas
    GPIOC->MODER = 0x1555;

    //set timer11 to 200Hz
    RCC->APB2ENR |= (1<<18);    //clock timer 11 enable
    TIM11->PSC = 799;
    TIM11->ARR = 99;
    TIM11->CR1 |= 0x01; //enable timer 11 couting
    TIM11->DIER |= 0x01;    //enable timer 11 interuption

    NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 5);   //Priority 5
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);

    //set timer 10 to 2Hz
    RCC->APB2ENR |= (1<<17);    //clock timer 10 enable
	TIM10->PSC = 7999;
	TIM10->ARR = 999;
	TIM10->CR1 |= 0x01;  //enable timer 10 couting
	TIM10->DIER |= 0x01;  //enable timer 10 interuption
    /*
        //set timer 10 to 800Hz
        RCC->APB2ENR |= (1<<17);    //clock timer 10 enable
        TIM10->PSC = 99;
        TIM10->ARR = 199;
        TIM10->CR1 |= 0x01;  //enable timer 10 couting
        TIM10->DIER |= 0x01;  //enable timer 10 interuption
    */
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 2);    //Priority 2
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

void TIM1_TRG_COM_TIM11_IRQHandler(void){
    //skip to the next display
    if((GPIOB->ODR & 0xF) == (~0b1000 & 0xF))   GPIOB->ODR = ~(0b0001);
    else                        		        GPIOB->ODR = ~(~(GPIOB->ODR) << 1);

    //print number
    GPIOC->ODR = convert_to_display(get_digit(number, GPIOB->ODR & 0xF));

    TIM11->SR &= ~0x01; //clear flag
    TIM11->CNT = 0;     //reset counter
}

void TIM1_UP_TIM10_IRQHandler(void){
    //increment displayed number
    number++;
    if(number>=10000)    number=0;

    //print number
    GPIOC->ODR = convert_to_display(get_digit(number, GPIOB->ODR & 0xF));

    TIM10->SR &= ~0x01; //clear flag
    TIM10->CNT = 0;     //reset counter
}

int get_digit(int num, uint32_t display){
    switch (display)
    {
    case (0b1110):
        return (num%10)/1;

    case (0b1101):
        return (num%100 - num%10)/10;

    case (0b1011):
        return (num%1000 - num%100)/100;

    case (0b0111):
        return (num%10000 - num%1000)/1000;

    default:
        return 0;
    }
}

uint32_t convert_to_display(int num){
    switch (num)
    {
    case 0:
        return 0b1111110;

    case 1:
        return 0b0110000;

    case 2:
        return 0b1101101;

    case 3:
        return 0b1111001;

    case 4:
        return 0b0110011;

    case 5:
        return 0b1011011;

    case 6:
        return 0b1011111;

    case 7:
        return 0b1110000;

    case 8:
        return 0b1111111;

    case 9:
        return 0b1111011;

    default:
        return 0;
    }
}
