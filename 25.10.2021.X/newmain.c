/*
 * File:   newmain.c
 * Author: OAKBATI-HPW
 *
 * Created on 25 Ekim 2021 Pazartesi, 10:00
 */


#include <stdio.h>
#include <stdlib.h>
#include <pic18f45k22.h>
#include <htc.h>
#define _XTAL_FREQ 8000000
#pragma config FOSC = HSHP
#pragma config WDTEN = OFF

unsigned char seg_char[11] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x39};
unsigned char digits[4] = {0, 0, 0, 0};
unsigned int cnt = 0;
unsigned int cnt2 = 0;
unsigned int analog = 0;
unsigned long temp = 0;
unsigned char dig_cnt = 0;

int main(int argc, char** argv) {

    ANSELA = 0;
    TRISA = 0xF0;
    PORTA = 0x00;

    ANSELD = 0;
    TRISD = 0x00;
    PORTD = 0;

    ANSELE = 0x02;
    TRISE = 0x7F;

    T0CON = 0xC3;

    ADCON2 = 0xA1;
    ADCON1 = 0x00;
    ADCON0bits.CHS = 6;
    ADCON0bits.ADON = 1;
    GODONE = 1;

    TMR0IE = 1;
    PEIE = 1;
    GIE = 1;


    while (1) {
        if (!GODONE) {
            analog = (ADRESH & 0x03)*256 + ADRESL;
            GODONE = 1;
            temp = (50000 * analog) / 1023;
            if (temp > 9999) {
                temp = 9999;
            }
        }
        //temp=analog;



        digits[3] = temp / 1000;
        digits[2] = (temp - digits[3]*1000) / 100;
        digits[1] = (temp - digits[3]*1000 - digits[2]*100) / 10;
        digits[0] = temp % 10;




    }

    return (EXIT_SUCCESS);
}

void __interrupt(high_priority) isr() {
    if (TMR0IF) {
        TMR0IF = 0;
        TMR0 = 6;
        PORTA = 0x00;
        if (dig_cnt == 0) {
            PORTD = seg_char[10];
        } else {
            PORTD = seg_char[digits[dig_cnt]] + 128 * (dig_cnt == 2);
        }

        PORTA = (0x01) << dig_cnt;

        dig_cnt++;
        if (dig_cnt >= 4) {
            dig_cnt = 0;
        }

    }


}


