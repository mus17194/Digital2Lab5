/*
 * File:   Lab03.c
 * Author: Vincenzo
 *
 *  LCD , USART y ADC
 * Created on February 5, 2020, 4:05 PM
 */


// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include <stdint.h>
#include "I2C.h"

#define _XTAL_FREQ 8000000


void main(void) {
    PORTA = 0;
    TRISA = 0;
    PORTB = 0;
    TRISB = 0;
    PORTC = 0;
    TRISC = 0;
    PORTD = 0;
    TRISD = 0;
    PORTE = 0;
    TRISE = 0;
    
    ANSEL = 0b00000000;
    ANSELH = 0;
    I2C_Master_Init(100000);  
    char savechain[16];

    int vcont1;
    int vpot1;
    int temp;
    
    startLCD();
    limpiarLCD();
    setLCD(1,1);
    escribirLCD("POT   CONT   SEN");

    while(1) {
        
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        vcont1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
         
        
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        vpot1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0xD0);
        I2C_Master_Write(0x11);
        I2C_Master_Stop();
        __delay_ms(10);
        
        I2C_Master_Start();
        I2C_Master_Write(0xD1);
        temp = I2C_Master_Read(0);
        I2C_Master_Stop();          //primero es el signo mas significativo los otros 7 son el entero de la tempeatura
        __delay_ms(10);
        
        sprintf(savechain, "%3.1i",vpot1);
        setLCD(2,1);
//        charLCD(vcont);
        escribirLCD(savechain);
        
        sprintf(savechain, "%2.1i",vcont1);
        setLCD(2,8);
//        charLCD(vcont);
        escribirLCD(savechain);
        
        sprintf(savechain, "%3.1i",temp);
        
        
//        charLCD(vcont);
        setLCD(2,12);
        escribirLCD(savechain);
        charLCD(223);
        charLCD('C');
        
        
        PORTA = vpot1;
    }
    
    
    return;
}
