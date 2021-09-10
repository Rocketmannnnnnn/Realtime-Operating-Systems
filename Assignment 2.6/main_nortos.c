
#include <stdint.h>
#include <stddef.h>
#include<stdio.h>
#include <NoRTOS.h>
#include <ti/drivers/GPIO.h>

#include "ti_drivers_config.h"
#include <ti/devices/cc32xx/driverlib/systick.h>

enum LIGHT { red = 5, yellow = 1, green = 4 };

int current = red;
int flag = 1;
int timer = 0;

void INT(){
    timer++;
    if(timer >= 10 * current){
        flag = 1;
        timer = 0;
    }
}

int main(){
    Board_init();
    NoRTOS_start();

    GPIO_init();

    //set period. 1 milli second is 80000. Multiply by 1000 and get a second
    SysTickPeriodSet(80000 * 100);
    //enable
    SysTickEnable();
    //Register interrupt
    SysTickIntRegister(INT);

    while(1){
       if(flag == 1){
           flag = 0;
           switch (current){
               case green:
                   GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_ON);
                   GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_OFF);
                   GPIO_write(CONFIG_LED_2_GPIO, CONFIG_GPIO_LED_OFF);
                   current = red;
                   break;
               case red:
                  GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
                  GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_ON);
                  GPIO_write(CONFIG_LED_2_GPIO, CONFIG_GPIO_LED_OFF);
                  current = yellow;
                  break;
               case yellow:
                  GPIO_write(CONFIG_LED_0_GPIO, CONFIG_GPIO_LED_OFF);
                  GPIO_write(CONFIG_LED_1_GPIO, CONFIG_GPIO_LED_OFF);
                  GPIO_write(CONFIG_LED_2_GPIO, CONFIG_GPIO_LED_ON);
                  current = green;
                  break;
           }
       }
       PRCMSleepEnter();
    }
}
