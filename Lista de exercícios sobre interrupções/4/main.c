#include "stm32f4xx.h"

#define DIS_SIZE 4

const char phrase[] = {"   marco sauer    "};
typedef struct
{
  char letters[DIS_SIZE];
  int end;
}displaying_t;

void setup();
void TIM1_UP_TIM10_IRQHandler(void);
void TIM1_TRG_COM_TIM11_IRQHandler(void);

void update_letters(char *letters, int end_index);
uint32_t convert_to_display(char letter);
char get_letter(char *letters, uint32_t display);

displaying_t displaying;

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

    NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 2);   //Priority 2
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);

    //set timer 10 to 3Hz
    RCC->APB2ENR |= (1<<17);    //clock timer 10 enable
    TIM10->PSC = 999;
    TIM10->ARR = 5332;
    TIM10->CR1 |= 0x01;  //enable timer 10 couting
    TIM10->DIER |= 0x01;  //enable timer 10 interuption

    NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1);    //Priority 1
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

    //set initial state
    for(int i=0; i<DIS_SIZE; i++){
    	displaying.letters[i] = phrase[i];
    }
    displaying.end = DIS_SIZE-1;
}

void TIM1_TRG_COM_TIM11_IRQHandler(void){
    //skip to the next display
    if((GPIOB->ODR & 0xF) == (~0b1000 & 0xF))   GPIOB->ODR = ~(0b0001);
    else                        		        GPIOB->ODR = ~(~(GPIOB->ODR) << 1);

    //print letter
    GPIOC->ODR = convert_to_display(get_letter(displaying.letters, GPIOB->ODR));

    TIM11->SR &= ~0x01; //clear flag
    TIM11->CNT = 0;     //reset counter
}

void TIM1_UP_TIM10_IRQHandler(void){
    //run next chunk of text
    displaying.end++;
    if(phrase[displaying.end] == '\0')  displaying.end = DIS_SIZE-1;
    update_letters(displaying.letters, displaying.end);

    TIM10->SR &= ~0x01; //clear flag
    TIM10->CNT = 0;     //reset counter
}

void update_letters(char *letters, int end_index){
  int j = 0;
  for(int i=(end_index-(DIS_SIZE-1)); i<=end_index; i++){
    *(letters+j) = phrase[i];
    j++;
  }
}

char get_letter(char *letters, uint32_t display){
    display &= 0xF;

	for(int i=0; i<DIS_SIZE; i++){
      if(display == ((~(1<<i)) & (0xF)))  return *(letters+(DIS_SIZE-1)-i);
    }
    return 1;
    
    //equivalent, easier for humans to understand:
      switch (display)
      {
      case (0b1110):
          return *(letters+(DIS_SIZE-1));

      case (0b1101):
          return *(letters+(DIS_SIZE-2));

      case (0b1011):
          return *(letters+(DIS_SIZE-3));

      case (0b0111):
          return *(letters+(DIS_SIZE-4));

      default:
          return 1;
      }
}

uint32_t convert_to_display(char letter){
    switch (letter)
    {
    case 'a':
        return 0b1110111;

    case 'b':
        return 0b0011111;
    
    case 'c':
        return 0b1001110;

    case 'd':
        return 0b0111101;

    case 'e':
        return 0b1001111;

    case 'f':
        return 0b1000111;

    case 'g':
        return 0b1111011;

    case 'h':
        return 0b0110111;

    case 'i':
        return 0b0110000;

    case 'j':
        return 0b0111000;

    case 'k':
        return 0b0000111;

    case 'l':
        return 0b0001110;

    case 'm':
        return 0b1010100;

    case 'n':
        return 0b1110110;

    case 'o':
        return 0b1111110;

    case 'p':
        return 0b1100111;

    case 'q':
        return 0b1110011;

    case 'r':
        return 0b1000110;

    case 's':
        return 0b1011011;

    case 't':
        return 0b0001111;

    case 'u':
        return 0b0111110;

    case 'v':
        return 0b0011100;

    case 'w':
        return 0b0101010;

    case 'x':
        return 0b0110001;

    case 'y':
        return 0b0111011;

    case 'z':
        return 0b1101101;
      
    case ' ':
        return 0b0000000;

    default:
        return 0b1111111;
    }
}
