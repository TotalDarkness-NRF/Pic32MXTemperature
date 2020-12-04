#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

/*
 * Below is for LCD pins
 */
#define RS _LATA2
#define R_W _LATA3
#define E _LATA4
/*
 * Below is for LCD bus
 */
#define DB4 _LATB15
#define DB5 _LATB14
#define DB6 _LATB13
#define DB7 _LATB11
/*
 * Below is for LCD color
 */
#define RED _LATB9
#define GREEN _LATB8
#define BLUE _LATB7

/* Prototypes for the functions */
void setPortB(char data);
void Nybble();
void initLCD();
void command(char data);
void LCDdisplay(char line1[], char line2[]);
void Color();

#endif
