/*
 * lab3_2.c
 *
 *  Created on: 14 Sep 2021
 *      Author: rygao
 */


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

#define datapins GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
#define RS GPIO_PIN_5
#define RW GPIO_PIN_6
#define EN GPIO_PIN_7
#define ROW GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
#define COL GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6

void flushInput(uint32_t ui32Port, uint8_t ui8Pins);
void delayMs(int n);
void delayUs(int n);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
void LCD_position(int hor_x, int ver_y);
void LCD_display_message(char *message_str);
void interface(int n);
int entered =0;
char pwd[10];
char check[16];
int page = 1;

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, datapins);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, RS|RW|EN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, ROW);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, COL);
    GPIOPadConfigSet(GPIO_PORTC_BASE, COL,GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

    LCD_init();
    LCD_position(0,1);
    LCD_display_message("Please set PWD:     ");
    LCD_position(0,2);



    while(1)
    {

        GPIOPinWrite(GPIO_PORTE_BASE,ROW,0x1C); // first row
        if (!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_4)) {
             LCD_data('1');
             if(page==1)
                 strcat(pwd,"1");
             else if(page!=1)
                 strcat(check,"1");
             flushInput(GPIO_PORTC_BASE,GPIO_PIN_4);
        }
        else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_5)) {
            LCD_data('2');
            if(page==1)
                strcat(pwd,"2");
            else if(page!=1)
                strcat(check,"2");
            flushInput(GPIO_PORTC_BASE,GPIO_PIN_5);
        }
        else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_6)) {
            LCD_data('3');
            if(page==1)
            strcat(pwd,"3");
            else if(page!=1)
            strcat(check,"3");
            flushInput(GPIO_PORTC_BASE,GPIO_PIN_6);
        }

        GPIOPinWrite(GPIO_PORTE_BASE,ROW,0x1A); // second row
        if (!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_4)) {
            LCD_data('4');
            if(page==1)
            strcat(pwd,"4");
            else if(page!=1)
                        strcat(check,"4");
            flushInput(GPIO_PORTC_BASE,GPIO_PIN_4);
         }
         else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_5)) {
            LCD_data('5');
            if(page==1)
            strcat(pwd,"5");
            else if(page!=1)
                        strcat(check,"5");
            flushInput(GPIO_PORTC_BASE,GPIO_PIN_5);
         }
         else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_6)) {
            LCD_data('6');
            if(page==1)
            strcat(pwd,"6");
            else if(page!=1)
                        strcat(check,"6");
            flushInput(GPIO_PORTC_BASE,GPIO_PIN_6);
         }

        GPIOPinWrite(GPIO_PORTE_BASE,ROW,0x16); // third row
        if (!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_4)) {
            LCD_data('7');
            if(page==1)
            strcat(pwd,"7");
            else if(page!=1)
                        strcat(check,"7");
            flushInput(GPIO_PORTC_BASE,GPIO_PIN_4);
        }
        else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_5)) {
            LCD_data('8');
            if(page==1)
            strcat(pwd,"8");
            else if(page!=1)
                        strcat(check,"8");
            flushInput(GPIO_PORTC_BASE,GPIO_PIN_5);
        }
        else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_6)) {
            LCD_data('9');
            if(page==1)
            strcat(pwd,"9");
            else if(page!=1)
                        strcat(check,"9");
            flushInput(GPIO_PORTC_BASE,GPIO_PIN_6);

        }

        GPIOPinWrite(GPIO_PORTE_BASE,ROW,0x0E); // forth row
        if (!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_4)) {
            if(page==1){
            memset(pwd, 0, sizeof pwd);
            }else if(page !=1){
                memset(check, 0, sizeof check);
            }
            LCD_position(0,2);
            LCD_display_message("                    ");
            LCD_position(0,2);
            flushInput(GPIO_PORTC_BASE,GPIO_PIN_4);
        }
        else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_5)) {
            LCD_data('0');
            if(page==1)
            strcat(pwd,"0");
            else if(page!=1)
                        strcat(check,"0");
            flushInput(GPIO_PORTC_BASE,GPIO_PIN_5);
        }
        else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_6)) {
            LCD_position(0,2);
            LCD_display_message("                    ");
            LCD_position(0,2);
            flushInput(GPIO_PORTC_BASE,GPIO_PIN_6);
            entered=1;
            if(page!=3)
            page+=1;
            else page = 2;
        }
        if(entered==1){
            interface(page);
        }
    }
}

void interface(int n){
    switch(n) {
    int i,j;
    case 1:
            break;
    case 2:
        entered =0;
        LCD_position(0,1);
        LCD_display_message("Plz enter PWD:     ");
        LCD_position(0,2);

        break;
    case 3:

            entered =0;

            for (i = 0; i < strlen(check); ++i){
                if(pwd[i] != check[i]){
                    LCD_position(0,1);
                    LCD_display_message("Wrong! try again:     ");
                    LCD_position(0,2);
                    memset(check, 0, sizeof check);
                    break;
                }
                else if(strlen(check)!=strlen(pwd)){
                        LCD_position(0,1);
                        LCD_display_message("Wrong! try again:     ");
                        LCD_position(0,2);
                        memset(check, 0, sizeof check);
                        break;
                    }
                else if((pwd[strlen(check)] ==  '\0') && (check[strlen(check)] =='\0'))
                {LCD_position(0,1);
                LCD_display_message("Correct! enter again:     ");
                LCD_position(0,2);
                memset(check, 0, sizeof check);
                    break;}
                }

            for(i = 0;i < 300;i++){
                for(j = 0;j < 3180; j++){
                    GPIOPinWrite(GPIO_PORTE_BASE,ROW,0x1C); // first row
                            if (!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_4)) {
                                 LCD_data('1');
                                     strcat(check,"1");
                                 flushInput(GPIO_PORTC_BASE,GPIO_PIN_4);
                            }
                            else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_5)) {
                                LCD_data('2');
                                    strcat(check,"2");
                                flushInput(GPIO_PORTC_BASE,GPIO_PIN_5);
                            }
                            else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_6)) {
                                LCD_data('3');
                                strcat(check,"3");
                                flushInput(GPIO_PORTC_BASE,GPIO_PIN_6);
                            }

                            GPIOPinWrite(GPIO_PORTE_BASE,ROW,0x1A); // second row
                            if (!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_4)) {
                                LCD_data('4');
                                            strcat(check,"4");
                                flushInput(GPIO_PORTC_BASE,GPIO_PIN_4);
                             }
                             else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_5)) {
                                LCD_data('5');
                                            strcat(check,"5");
                                flushInput(GPIO_PORTC_BASE,GPIO_PIN_5);
                             }
                             else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_6)) {
                                LCD_data('6');
                                            strcat(check,"6");
                                flushInput(GPIO_PORTC_BASE,GPIO_PIN_6);
                             }

                            GPIOPinWrite(GPIO_PORTE_BASE,ROW,0x16); // third row
                            if (!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_4)) {
                                LCD_data('7');
                                            strcat(check,"7");
                                flushInput(GPIO_PORTC_BASE,GPIO_PIN_4);
                            }
                            else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_5)) {
                                LCD_data('8');
                                            strcat(check,"8");
                                flushInput(GPIO_PORTC_BASE,GPIO_PIN_5);
                            }
                            else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_6)) {
                                LCD_data('9');
                                            strcat(check,"9");
                                flushInput(GPIO_PORTC_BASE,GPIO_PIN_6);

                            }

                            GPIOPinWrite(GPIO_PORTE_BASE,ROW,0x0E); // forth row
                            if (!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_4)) {
                                    memset(check, 0, sizeof check);

                                LCD_position(0,2);
                                LCD_display_message("                    ");
                                LCD_position(0,2);
                                flushInput(GPIO_PORTC_BASE,GPIO_PIN_4);
                            }
                            else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_5)) {
                                LCD_data('0');
                                strcat(check,"0");
                                flushInput(GPIO_PORTC_BASE,GPIO_PIN_5);
                            }
                            else if(!GPIOPinRead(GPIO_PORTC_BASE,GPIO_PIN_6)) {
                                LCD_position(0,2);
                                LCD_display_message("                    ");
                                LCD_position(0,2);
                                flushInput(GPIO_PORTC_BASE,GPIO_PIN_6);
                                entered=1;
                            }
                            if(entered==1){interface(3);}
                }
            }
            LCD_position(0,1);
            LCD_display_message("Plz enter PWD:     ");
            LCD_position(strlen(check),2);
            delayMs(100);
            break;
            default:
                interface(2);

            }


    }



void flushInput(uint32_t ui32Port, uint8_t ui8Pins){
    /* wait until the key is release to avoid redundant inputs. */
    while(!GPIOPinRead(ui32Port, ui8Pins)) {
        delayMs(100);
    }
}


void delayMs(int n)
{
    int i,j;
    for(i = 0;i < n;i++)
        for(j = 0;j < 3180; j++)
            {}
}

void delayUs( int n)
{
    int i,j;
    for(i = 0;i < n;i++)
        for(j = 0;j < 3; j++)
            {}
}

void LCD_init(void)
{
    delayMs(20); /* initialization sequence */
    LCD_command(0x30);
    delayMs(50);
    LCD_command(0x30);
    delayUs(500);
    LCD_command(0x30);

    LCD_command(0x38); // set 8-bit data, 2-line, 5x7 font
    LCD_command(0x06); // cursor move direction: increase
    LCD_command(0x01); // display clear
    LCD_command(0x02); // cursor move to first digit
    LCD_command(0x0F); //turn on display, cursor blinking
}

void LCD_command(unsigned char command)
{
    GPIOPinWrite(GPIO_PORTA_BASE, RS, 0x00); //RS = 0->command mode
    GPIOPinWrite(GPIO_PORTA_BASE, RW, 0x00); //RW = 0->writing mode
    GPIOPinWrite(GPIO_PORTB_BASE, datapins, command); // Write the command to data pins
    GPIOPinWrite(GPIO_PORTA_BASE, EN, 0x80); // Set EN to high
    delayUs(500);
    GPIOPinWrite(GPIO_PORTA_BASE, EN, 0x00); // Set EN to low, a high-to-low pulse
    if (command < 4)
        delayMs(20); /* command 1 and 2 needs up to 1.64ms */
    else
        delayUs(400); /* all others 40 us */
}

void LCD_data(unsigned char data)
{
    GPIOPinWrite(GPIO_PORTA_BASE, RS, 0x20); //RS = 0->command mode
    GPIOPinWrite(GPIO_PORTA_BASE, RW, 0x00); //RW = 0->writing mode
    GPIOPinWrite(GPIO_PORTB_BASE, datapins, data); // Write the command to data pins
    GPIOPinWrite(GPIO_PORTA_BASE, EN, 0x80); // Set EN to high
    delayUs(500);
    GPIOPinWrite(GPIO_PORTA_BASE, EN, 0x00); // Set EN to low, a high-to-low pulse
}

void LCD_position(int hor_x, int ver_y) {
        switch(ver_y) {
    case 1:
                LCD_command(0x80+hor_x);
                delayMs(50);
        break;
    case 2:
                LCD_command(0xc0+hor_x);
                delayMs(50);
        break;
    case 3:
                LCD_command(0x94+hor_x);
                delayMs(50);
        break;
    case 4:
                LCD_command(0xd4+hor_x);
                delayMs(50);
        break;
        }
}

void LCD_display_message(char *message_str) {
    int n;

    n=0;
    while (message_str[n]!='\0') {
        LCD_data(message_str[n]);
        n++;
    }
}
