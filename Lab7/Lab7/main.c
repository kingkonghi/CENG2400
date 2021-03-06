#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
    uint32_t timer=0;
    char tempC[5];
    char tempF[5];

int main(void)
{

    uint32_t ui32ADC0Value[4];
    volatile uint32_t ui32TempAvg;
    volatile uint32_t ui32TempValueC;
    volatile uint32_t ui32TempValueF;

    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);
    ADCSequenceStepConfigure(ADC0_BASE,1,3,ADC_CTL_TS|ADC_CTL_IE|ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 1);

    while(1)
    {
       ADCIntClear(ADC0_BASE, 1);
       ADCProcessorTrigger(ADC0_BASE, 1);

       while(!ADCIntStatus(ADC0_BASE, 1, false))
       {
       }

       ADCSequenceDataGet(ADC0_BASE, 1, ui32ADC0Value);
       ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2)/4;
       ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096)/10;
       ui32TempValueF = ((ui32TempValueC * 9) + 160) / 5;
       //homework part


       SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
             SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
             GPIOPinConfigure(GPIO_PA0_U0RX);
             GPIOPinConfigure(GPIO_PA1_U0TX);
             GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
             UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                     (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
             IntMasterEnable(); //enable processor interrupts
                 IntEnable(INT_UART0); //enable the UART interrupt
                 UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
                 timer++;
                 int i=0;
                 if(timer ==20){
                     uint32_t C =ui32TempValueC;
                     uint32_t F =ui32TempValueF;
                     int numC = log10(C)+1;
                     int numF = log10(F)+1;
                     for (i = 0; i < numC; i++,C/=10)     {

                         tempC[i] = C%10 +48;
                     }
                     for (i = 0; i < numF; i++,F/=10)     {

                         tempF[i] = F%10 +48;
                     }
                     UARTCharPutNonBlocking(UART0_BASE, 'C');
                     UARTCharPutNonBlocking(UART0_BASE, ':');
                     for(i =numC-1;i>=0;i--){
                         UARTCharPutNonBlocking(UART0_BASE, tempC[i]);
                         }
                     UARTCharPutNonBlocking(UART0_BASE, ' ');
                     UARTCharPutNonBlocking(UART0_BASE, 'F');
                     UARTCharPutNonBlocking(UART0_BASE, ':');
                     for(i =numF-1;i>=0;i--){
                         UARTCharPutNonBlocking(UART0_BASE, tempF[i]);
                         }
                 UARTCharPutNonBlocking(UART0_BASE, '\r');
                 timer=0;}
    }
}
