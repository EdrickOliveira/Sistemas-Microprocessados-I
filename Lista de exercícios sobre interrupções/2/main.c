#include "stm32f4xx.h"

int number = 0;

void setup();
void TIM1_TRG_COM_TIM11_IRQHandler(void);
uint32_t convert_to_display(int num);
int get_digit(int num, uint32_t display);

//////////////////////////////////////////////////////////////////////////////////
int main(void)
{
  int button = 0;
  
  setup();

  while(1){
    if(GPIOA->IDR & 1){
      button++;

      if(button>50000){
        number++;
        if(number==10000) number = 0;

        while(GPIOA->IDR & 1);

        button = 0;
      }

    }
  }
}
//////////////////////////////////////////////////////////////////////////////////

void setup(){
    RCC->AHB1ENR = 0x87;
    GPIOA->MODER = 0x28000400;

    //PA0 = input
    GPIOA->MODER &= ~(1);
    GPIOA->PUPDR |= 2;
    //PB0 a PB3 = outputs
    GPIOB->MODER = 0x55;
    GPIOB->ODR = ~(0b0001);
    //PC0 a PC6 = outputs
    GPIOC->MODER = 0x1555;

    //set timer11 to 200Hz
    RCC->APB2ENR |= (1<<18);    //clock timer 11 enable
    TIM11->PSC = 799;
    TIM11->ARR = 99;
    TIM11->CR1 |= 0x01; //enable timer 11 couting
    TIM11->DIER |= 0x01;    //enable timer 11 interuption

    NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 5);   //Priority 5
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
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
