/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
// #include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

/* Driver configuration */
#include "ti_drivers_config.h"
#include <ti/drivers/apps/LED.h>
#include <ti/drivers/apps/Button.h>
#include <ti/display/Display.h>
/*
 *  ======== mainThread ========
 */

int count = 0;
int brightness = 50;
LED_Handle green;
LED_Handle yellow;
LED_Handle red;
Display_Handle hSerial;

void setLEDs();
void cycleBtn(Button_Handle handle, Button_EventMask events);
void lightBtn(Button_Handle handle, Button_EventMask events);

void *mainThread(void *arg0)
{
    /* Call driver init functions */
    //GPIO_init();
    //PWM_init();

    //Initialize LEDs
    LED_Params ledParams;
    // Assume our LED is configured to be a PWM LED
    LED_Params_init(&ledParams);
    green = LED_open(CONFIG_LED_0, &ledParams);
    yellow = LED_open(CONFIG_LED_1, &ledParams);
    red = LED_open(CONFIG_LED_2, &ledParams);

    //Initialize buttons with function pointers
    Button_Params params;
    Button_Handle handle;
    Button_Handle handle2;
    Button_Params_init(&params);
    handle = Button_open(CONFIG_BUTTON_0, cycleBtn, &params);
    handle2 = Button_open(CONFIG_BUTTON_1, lightBtn, &params);

    Display_init();
    hSerial = Display_open(Display_Type_UART, NULL);

    while (1)
    {
        sleep(1);
    }
}

void cycleBtn(Button_Handle handle, Button_EventMask events)
{
    Display_printf(hSerial, 1, 0, "Cycle btn");

    if (events & Button_EV_CLICKED){
        count++;
    }else if (events & Button_EV_DOUBLECLICKED ){
        count--;
    }

    count %= 8;
    if(count < 0 ){
        count = 7;
    }
    setLEDs();
}

void lightBtn(Button_Handle handle, Button_EventMask events)
{
    Display_printf(hSerial, 1, 0, "Light btn");
    if (events & Button_EV_CLICKED){
        brightness -= 25;
    } else if (events & Button_EV_DOUBLECLICKED ){
        brightness += 25;
    } else if (events & Button_EV_LONGPRESSED){
        brightness = LED_BRIGHTNESS_MAX;
    }
    setLEDs();
}


void setLEDs(){
    if(count == 1 || count == 3 || count == 5 || count == 7){
        LED_setOn(red, brightness);
    }
    else{
        LED_setOff(red);
    }

    if(count == 2 || count == 3 || count == 6 || count == 7){
        LED_setOn(yellow, brightness);
    }
    else{
        LED_setOff(yellow);
    }

    if(count > 3){
        LED_setOn(green, brightness);
    }
    else{
        LED_setOff(green);
    }
}
