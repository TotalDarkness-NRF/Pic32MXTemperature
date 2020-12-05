#include "Thermometer.h"

void displayString(char *string) {
  int i;
  home();
  for(i=0;string[i]!='\0';i++) {
      if (!(i %16) && i)  // check if line can fit on a line and is not 0
          nextLine();
      if (string[i] == 'C' && string[i + 1] == '\0') {
          write(0xDF); // put degree symbol
      }
      write(string[i]);
  }
}

void setPortB(char i) {
    DB4 = i & 0x01;
    i >>= 1;
    DB5 = i & 0x01;
    i >>= 1;
    DB6 = i & 0x01;
    i >>= 1;
    DB7 = i & 0x01;
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

void nextLine() {
	command(0xC0);
}

void clearDisplay() {
    command(0x01);
    Delay(10);
}

void home() {
    command(0x02);
    Delay(5);
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