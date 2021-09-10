
#include <stdint.h>
#include <stddef.h>

#include <NoRTOS.h>
#include <ti/drivers/GPIO.h>

#include "ti_drivers_config.h"
#include <ti/devices/cc32xx/driverlib/systick.h>

int flag = 0;
int timer = 0;

void INT(){
    timer++;
    if(timer >= 1000){
        flag = 1;
        timer = 0;
    }
}

int main(){
    Board_init();
    NoRTOS_start();

    GPIO_init();
    GPIO_write(CONFIG_GPIO_LED_2, CONFIG_GPIO_LED_ON);

    //set period. 1 milli second is 80000. Multiply by 1000 and get a second
    SysTickPeriodSet(80000);
    //enable
    SysTickEnable();
    //Register interrupt
    SysTickIntRegister(INT);

    while(1){
       if(flag == 1){
           flag = 0;
           GPIO_toggle(CONFIG_GPIO_LED_2);
       }
    }
}
