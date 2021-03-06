#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

#include "inc/hw_gpio.h"

uint8_t freq = 2;
uint8_t timer=1;
uint8_t magic_number=2;
uint8_t color=1;
uint8_t limit= 8;
void GPIO_PORtF_Handler(void);


void Timer0IntHandler(void)
{
    // Clear the timer interrupt

    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerLoadSet(TIMER0_BASE, TIMER_A, (SysCtlClockGet() / freq) /2 - 1);
    if(timer<60) timer+=1;
    else timer=2;

    limit = freq*4;


    if(timer==limit){
        if(magic_number==8)magic_number=2;
        else magic_number*=2;
    timer=0;}

    if(color==1)
    {GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, magic_number);
    color=0;}
    else
    {GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
    color=1;}

}



int main(void)
{

    uint32_t ui32Period;


    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    ui32Period = (SysCtlClockGet() / freq) /2;
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);

    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    IntMasterEnable();

    TimerEnable(TIMER0_BASE, TIMER_A);

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4) ;       // PF4 input
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU) ;
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4) ;          // interrupt enable
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_LOW_LEVEL) ; // low level interrupt, reference gpio.c to find more interrupt type!!!
    GPIOIntRegister(GPIO_PORTF_BASE, GPIO_PORtF_Handler) ;     // dynamic isr registering

    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    while(1)
    {
    }
}

void GPIO_PORtF_Handler(void) {
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4) ;
    if(timer >2){
    if(freq == 2){freq=8;}
    else if(freq == 4){freq=2;}
    else if(freq == 8){freq=4;}
    else {freq =2;return;}}
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4) ;
    timer=0;
}
