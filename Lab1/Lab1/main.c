/*
 * lab2_2.c
 *
 *  Created on: 14 Sep 2021
 *      Author: rygao
 */


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

uint8_t ui8PinData=2;
int32_t ButtonState = 0;

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |=0X01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    // Read the state of SW1 from PF4
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    // Since the button needs some sort of pull-up, we set pin4 as weak pull-up



    while(1)
    {
        ButtonState = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
        if (ButtonState == GPIO_PIN_4) {

            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
            SysCtlDelay(1000000);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            SysCtlDelay(1000000);
            if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}

        }
        else if(ButtonState == GPIO_PIN_0){

            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ui8PinData);
            SysCtlDelay(2000000);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            SysCtlDelay(2000000);
            if(ui8PinData==8) {ui8PinData=2;} else {ui8PinData=ui8PinData*2;}
        }
        else {
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
        }
    }
}
