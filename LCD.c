#include "Thermometer.h"

void displayString(char *string) {
    displayStringStart(0, string);
}

void displayStringStart(char start, char *string) {
    if (start > 32)  return;
    home();
    setCursorPosition(start);
    int i;
    for(i= 0; string[i] != '\0'; i++) {
         if (!(i %16) && i && start<= 16) nextLine(); // check if line can fit on a line and is not 0
            if (string[i] == 'C' && string[i + 1] == '\0') {
                write(0xDF); // put degree symbol / /TODO I may not need this now!
                if (!((i + 1)%16) && start<= 16) nextLine(); // check if line can fit on a line and is not 0  
            }
         write(string[i]);
    }
}

void setPortB(char i) {
    DB4 = i & 1;
    DB5 = i >>1 & 1;
    DB6 = i >>2 & 1;
    DB7 = i >>3 & 1;
}

void write(char data) {
    setPortB(data >> 4);
    RS = HIGH; // Send data
    R_W = LOW; // write
    Nybble();
    setPortB(data & 0x0F);
    Nybble();
}

/*
 * Start of LCD commands
 */
void command(char data) {
    setPortB(data >> 4);
    RS = LOW; // Send command
    R_W = LOW; //write
    Nybble();
    setPortB(data & 0x0F);
    Nybble();
}

void clearDisplay() {
    command(0x01);
    Delay(10);
}

void home() {
    command(0x02);
    Delay(5);
}

void nextLine() {
	command(0xC0);
}

void setCursorPosition(char position) {
    if (position >= 0 && position <= 16)
        command(0x80 + position);
   else if (position > 16 && position <= 32)
       command(0xC0 + (position - 17));
}

void setEntryMode(int cursorDirection, int blink) {
    cursorDirection = cursorDirection ? 2 : 0;
    blink = blink ? 1 : 0;
    command(0x04 + cursorDirection + blink);
    Delay(1);
}

void setDisplay(int display, int cursor, int blink) {
    display = display ? 4 : 0;
    cursor = cursor ? 2 : 0;
    blink = blink ? 1 : 0;
    command(0x08 + display + cursor + blink);
    Delay(1);
}

void setShifting(int cursor, int display) {
    cursor = cursor ? 8 : 0;
    display = display ? 4 : 0;
    command(0x10 + cursor + display);
    Delay(1);
}

void setFunction(int DL, int N, int F) {
     DL = DL ? 16 : 0;
     N = N ? 8 : 0;
     F = F ? 4 : 0;
     command(0x20 + DL + N + F);
     Delay(1);
}
/*
 *  End of LCD commands
 */

void Nybble() {
    E = HIGH;
    Delay(1);
    E = LOW; //Clock enable: falling edge
    Delay(1);
}

void initLCD() {
    setPortB(0);
    Delay(100);
    setPortB(0x30);
    Delay(30);
    Nybble();
    Delay(10);
    Nybble();
    Delay(10);
    Nybble();
    Delay(10);
    setPortB(0x20);
    Nybble();
    // Above deals with waking up the LCD and insuring it will go in 4-bit mode
    
    setFunction(0,1,0); //Function set: 4-bit/2-line
    setShifting(0,0); //Set cursor
    setDisplay(1,0,0);  //Display ON; No cursor
    setEntryMode(1,0); //Entry Mode set
    clearDisplay(); 
    
    RED = HIGH;
    BLUE = LOW;
    GREEN = LOW;
}