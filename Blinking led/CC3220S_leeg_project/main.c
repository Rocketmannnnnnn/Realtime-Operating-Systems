#include <stdint.h>
#include <stddef.h>
#include "register_def.h"
//Extra includes hieronder

/*
 *  ======== main ========
 */

//Enable leds throug memory adresses
void assignment1()
{
    //Enable clock module GPIOA1
        //Base adres GPIOA1: ARCM_BASE
        //Register offset: GPIOCLKEN = APPS_RCM_O_GPIO_B_CLK_GATING
        HWREG(ARCM_BASE + (APPS_RCM_O_GPIO_B_CLK_GATING)) |= 0x01;

        //Pin multiplexer
        //Base adres: OCP_SHARED_BASE
        //Register (GPIO PIN) offset: HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_9);
        HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_9) = 0x40;
        HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_10) = 0x40;
        HWREG(OCP_SHARED_BASE + OCP_SHARED_O_GPIO_PAD_CONFIG_11) = 0x40;
        //TODO
        HWREG(GPIOA1_BASE + GPIO_O_GPIO_DIR) = led;
        
        int c;
        int led = 0;

        while (1)
        {
            for(c = 0; c < 6000000; c++) { }

            if(led > 14){
                led = 0;
            }

            
            HWREG(GPIOA1_BASE + (GPIO_O_GPIO_DATA + (led << 2))) = led;
            led += 2;
        }
}


int main(void)
{
    assignment1();
}
