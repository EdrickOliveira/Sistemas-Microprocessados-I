#include "stm32f4xx.h"

#define DEBOUNCE 25000
#define PRESS_N_HOLD 200000

int setting_alarm = 0, alarm_number = -1, time_number = 0, displaying_number;

void setup();
void TIM1_UP_TIM10_IRQHandler(void);
void TIM1_TRG_COM_TIM11_IRQHandler(void);
void TIM8_UP_TIM13_IRQHandler(void);
uint32_t convert_to_display(int num);
int get_digit(int num, uint32_t display);
int to_display_number(int num);
void alarm_settings();

//////////////////////////////////////////////////////////////////////////////////
int main(void)
{
  int button[] = {0, 0};

  setup();

  while(1){
	//turn alarm noise on
    if(time_number==alarm_number)   GPIOC->ODR |= 0x100;

    if((GPIOA->IDR & 1) && (GPIOA->IDR & 2)){
        button[0]++;
        button[1]++;

        //if both have been pressed for a second
        if(button[0]>PRESS_N_HOLD && button[1]>PRESS_N_HOLD){
            //if alarm is off
            if((GPIOC->IDR & 0x100) == 0){
                alarm_settings();
                //wait untill the buttons are unpressed
                while((GPIOA->IDR&1) || (GPIOA->IDR&2));

                button[0]=0;
                button[1]=0;
            }
            //if alarm is on
            else    GPIOC->ODR &= ~0x100;   //turn it off
            //and wait untill the buttons are unpressed
            while((GPIOA->IDR&1) || (GPIOA->IDR&2));
        }
    }

    //button 0 adds a minute
    else if(GPIOA->IDR & 1){
      button[0]++;

      if(button[0]>DEBOUNCE){
        time_number+=60;

        //if minutes equal 00
        if((time_number/60)%60 == 0)   time_number -= 3600; //subtract an hour

        while((GPIOA->IDR & 1) && ((GPIOA->IDR&2) == 0));
        button[0] = 0;
      }
    }

    //button 1 adds an hour
    else if(GPIOA->IDR & 2){
      button[1]++;

      if(button[1]>DEBOUNCE){
        time_number+=3600;

        if(to_display_number(time_number) >= 240000)  time_number -= (60*60*24);  //don't skip to the next day

        while((GPIOA->IDR & 2) && ((GPIOA->IDR&1) == 0));
        button[1] = 0;
      }
    }

  }
}
//////////////////////////////////////////////////////////////////////////////////

void setup(){
    RCC->AHB1ENR = 0x87;
    GPIOA->MODER = 0x28000400;

    //PA0 e PA1 = inputs
    GPIOA->MODER &= ~0xF;
    GPIOA->PUPDR |= 0xA;
    //PB0 a PB5 = outputs
    GPIOB->MODER = 0x555;
    GPIOB->ODR = 0b111110;
    //PC0 a PC8 = outputs   (display: 0-6; LED: 7; Alarm: 8)
    GPIOC->MODER = 0x15555;

    //set timer 10 to 1Hz
    RCC->APB2ENR |= (1<<17);    //clock timer 10 enable
    TIM10->PSC = 999;
    TIM10->ARR = 15999;
    TIM10->CR1 |= 0x01;  //enable timer 10 couting
    TIM10->DIER |= 0x01;  //enable timer 10 interuption

    NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1);    //Priority 1
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

    //set timer 11 to 400Hz
    RCC->APB2ENR |= (1<<18);    //clock timer 11 enable
    TIM11->PSC = 399;
    TIM11->ARR = 99;
    TIM11->CR1 |= 0x01; //enable timer 11 couting
    TIM11->DIER |= 0x01;    //enable timer 11 interuption

    NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 3);   //Priority 3
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);

    //set timer 13 to 2Hz
    RCC->APB1ENR |= 0x80;	//clock timer 13 enable
    TIM13->PSC = 999;
    TIM13->ARR = 7999;
    TIM13->CR1 |= 0x01; 	//enable timer 13 couting
    TIM13->DIER |= 0x01;    //enable timer 13 interuption

    NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 2);   //Priority 2
    NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
}

void TIM1_TRG_COM_TIM11_IRQHandler(void){
    //skip to the next display
    if((GPIOB->ODR & 0x3F) == 0b011111) GPIOB->ODR = 0b111110;
    else                        		GPIOB->ODR = ~(~(GPIOB->ODR) << 1);

    if(setting_alarm)   displaying_number = alarm_number;
    else                displaying_number = time_number;

    //print number
    GPIOC->ODR &= ~0x7F;
    GPIOC->ODR |= convert_to_display(get_digit(to_display_number(displaying_number), GPIOB->ODR & 0x3F));

    //if is in alarm settings, twinkle display by setting them off when LED is off
    if(setting_alarm && (GPIOC->IDR & 0x80)==0)	GPIOC->ODR &= ~0x7F;

    TIM11->SR &= ~0x01; //clear flag
    TIM11->CNT = 0;     //reset counter
}

void TIM1_UP_TIM10_IRQHandler(void){
    time_number++;

    TIM10->SR &= ~0x01; //clear flag
    TIM10->CNT = 0;     //reset counter
}

void TIM8_UP_TIM13_IRQHandler(void){
	//switch LED state
	if(GPIOC->IDR & 0x80)	GPIOC->ODR &= ~0x80;
	else                    GPIOC->ODR |= 0x80;

	TIM13->SR &= ~0x01; //clear flag
    TIM13->CNT = 0;     //reset counter
}

int get_digit(int num, uint32_t display){
    switch (display)
    {
    case (0b111110):
        return (num%10)/1;

    case (0b111101):
        return (num%100 - num%10)/10;

    case (0b111011):
        return (num%1000 - num%100)/100;

    case (0b110111):
        return (num%10000 - num%1000)/1000;

    case (0b101111):
        return (num%100000 - num%10000)/10000;

    case (0b011111):
        return (num%1000000 - num%100000)/100000;

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

int to_display_number(int num){
    int seconds = num%60;
    int minutes = (num/60)%60;
    int hours = (num/3600)%24;

    return (hours*10000)+(minutes*100)+(seconds);
}

void alarm_settings(){
    setting_alarm = 1;
    alarm_number = 0;

    //wait untill the buttons are unpressed
    while((GPIOA->IDR&1) || (GPIOA->IDR&2));

    int button[]={0, 0};
    while(1){
        //if both are pressed
        if((GPIOA->IDR & 1) && (GPIOA->IDR & 2)){
            button[0]++;
            button[1]++;

            //for a second
            if(button[0]>PRESS_N_HOLD && button[1]>PRESS_N_HOLD){
                //quit alarm settings
                setting_alarm = 0;
                break;
            }
        }

        //if button 0 is pressed
        else if(GPIOA->IDR & 1){
            button[0]++;

            if(button[0]>DEBOUNCE){
                alarm_number += 60; //increase alarm number a minute
                if(alarm_number%3600 == 0)    alarm_number -= 3600; //don't skip to the next hour

                for(int i=0; GPIOA->IDR&1; i++){
                    //if button is pressed for more than 3 seconds
                    if(i>(PRESS_N_HOLD*3)){
                        //quit and unset alarm
                        alarm_number = -1;
                        setting_alarm = 0;
                        break;
                    }
                }
                if(setting_alarm==0)    break;
                button[0]=0;
            }
        }

        //if button 1 is pressed
        else if(GPIOA->IDR & 2){
            button[1]++;

            if(button[1]>DEBOUNCE){
                alarm_number += 3600;   //increase alarm number an hour
                if(to_display_number(alarm_number) >= 240000) alarm_number -= (60*60*24); //don't skip to the next day

                for(int i=0; GPIOA->IDR&2; i++){
                    //if button is pressed for more than 3 seconds
                    if(i>(PRESS_N_HOLD*3)){
                        //quit and unset alarm
                        alarm_number = -1;
                        setting_alarm = 0;
                        break;
                    }
                }
                if(setting_alarm==0)    break;
                button[1]=0;
            }
        }
    }
}
