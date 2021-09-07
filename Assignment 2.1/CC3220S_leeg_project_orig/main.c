#include <stdint.h>
#include <stddef.h>
#include "register_def.h"
//Extra includes hier onder

int main(void)
{
    //ST reload to set values (default 80000)
    //Magic number 80000 has something to do with the clock frequention
    HWREG(NVIC_ST_RELOAD) = 80000;

    //ST ctrl (control) to register the required operation
    HWREG(NVIC_ST_CTRL) |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN | NVIC_ST_CTRL_ENABLE;

    while(1)
    {

    }
}
