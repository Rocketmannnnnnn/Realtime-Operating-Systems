#include <stdint.h>
#include <stddef.h>
#include "register_def.h"
//Extra includes hieronder

/*
 *  ======== main ========
 */
int main(void)
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

    //GPIO Registers
    //Base: GPIOA1_BASE
    //Red LED
    HWREG(GPIOA1_BASE + GPIO_O_GPIO_DIR) = 0x02;
    HWREG(GPIOA1_BASE + (GPIO_O_GPIO_DATA + (0x02<<2)))= 0x02;

    //Yellow LED
    HWREG(GPIOA1_BASE + GPIO_O_GPIO_DIR) = 0x04;
    HWREG(GPIOA1_BASE + (GPIO_O_GPIO_DATA + (0x04<<2)))= 0x04;

    //Yellow LED
    HWREG(GPIOA1_BASE + GPIO_O_GPIO_DIR) = 0x08;
    HWREG(GPIOA1_BASE + (GPIO_O_GPIO_DATA + (0x08<<2)))= 0x08;

    while (1)
    {

    }
}
