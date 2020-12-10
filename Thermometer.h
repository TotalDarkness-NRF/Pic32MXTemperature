#ifndef THERMOMETER_H_INCLUDED
#define THERMOMETER_H_INCLUDED

#include <p32xxxx.h>
#include <stdio.h>

/*
 * Below is for LCD pins
 */
#define RS  _LATA2
#define R_W _LATA3
#define E   _LATA4
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
#define RED     _LATB9
#define GREEN   _LATB8
#define BLUE    _LATB7
/*
 * Below defines pins for temperature sensor
 */
#define PIN_DIRECTION   _TRISA0
#define WRITE_PIN  		_LATA0
#define READ_PIN		_RA0
/*
 * Below defines commands set for sensor
 */
#define CONVERT_T           0x44
#define READ_SCRATCHPAD     0xBE
#define WRITE_SCRATCHPAD    0x4E
#define COPY_SCRATCHPAD     0x48
#define RECALL_E            0xB8
#define READ_POWER          0xB4
/*
 * Below defines ROM commands
 */
#define SEARCH_ROM      0xF0
#define READ_ROM        0x33
#define MATCH_ROM       0x55
#define SKIP_ROM        0xCC
#define ALARM_SEARCH    0xEC
/*
 * Below is button pins
 */
#define BUTTON1 _RB0
#define BUTTON2 _RB1
#define BUTTON3 _RB2
#define BUTTON4 _RB3
#define BUTTON5 _RB4
#define BUTTON6 _RB5
/*
 * Below is for Constants
 */
#define HIGH        1
#define LOW         0
#define OUTPUT      0
#define INPUT       1
#define SYS_FREQ    40000000
/* 
 *  Prototypes for the Temperature Sensor functions 
 */
int getTemperature(void);
int convertTemperature(float temperature);
void DriveOW(unsigned char Bit);
int ResetPulse(void);
void getScratchpad(void);
unsigned char ReadByte(void);
unsigned char ReadBit(void);
unsigned char ReadOW(void);
void WriteByte(unsigned char write_data);
void WriteBit(unsigned char write_data);
void WriteBitOne(void);
void WriteBitZero(void);
int getCoversionDelay(void);
int ifSensorPresent(void);
void setPrecision(int type);
int getPrecision(void);
int getTemperatureAverage(void);
/* 
 *  Prototypes for the LCD functions 
 */
void initLCD(void);
void LCDdisplay(char line1[], char line2[]);
void displayStringStart(int start, char *string);
void displayString(char *string);
void write(char data);
void setPortB(char data);
void Nybble(void);
void command(char data);
void clearDisplay(void);
void home(void);
void nextLine(void);
void setCursorPosition(int position);
void setEntryMode(int cursorDirection, int blink);
void setDisplay(int display, int cursor, int blink);
void setShifting(int cursor, int display);
void setFunction(int DL, int N, int F);
/*
 *  Prototypes for the button functions
 */
void Color(void);
void ChangePrecision(void);
void ChangeTemperatureUnit(void);
int ChangeViewMode(int viewMode);
/*
 *  Prototypes for the delay functions
 */
void Delayus(unsigned int us);
void Delay(unsigned int ms);

#endif