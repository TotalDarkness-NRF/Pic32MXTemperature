#include <p32xxxx.h>
#include "LCD.h"
#include "Buttons.h"

void Nybble() {
    E = HIGH;
    Delay(1); //enable pulse width >= 300ns
    E = LOW; //Clock enable: falling edge
    Delay(1); //enable pulse width >= 300ns
}

void setPortB(char i) {
    PORTB;
    DB4 = i & 0x01;
    i >>= 1;
    DB5 = i & 0x01;
    i >>= 1;
    DB6 = i & 0x01;
    i >>= 1;
    DB7 = i & 0x01;
}

void command(char data) {
    setPortB(data >> 4); //put data on output Port
    RS = LOW; // Send command
    R_W = LOW; //write
    Nybble(); // Allow to send
    setPortB(data & 0x0F); //put data on output Port
    Nybble(); // Allow to send
}

void write(char data) {
    setPortB(data >> 4); // put data on output Port
    RS = HIGH; // Send data
    R_W = LOW; // write
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
    cursor = cursor? 2 : 0;
    blink = blink ? 1 : 0;
    command(0x08 + display + cursor + blink);
    Delay(1);
}

void setShifting(int cursor, int display) {
    cursor = cursor? 8 : 0;
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

void displayString(char *string){
  int i;
  home();
  for(i=0;string[i]!='\0';i++) {
      if (!(i %16) && i)  // check if line can fit on a line and is not 0
          nextLine();
      write(string[i]);
  }
}

void initLCD() {
    setPortB(0);
    Delay(100); //Wait >40 msec after power is applied
    setPortB(0x30); //put 0x30 on the output port
    Delay(30); //must wait 5ms, busy flag not available
    Nybble(); //command 0x30 = Wake up
    Delay(10); //must wait 160us, busy flag not available
    Nybble(); //command 0x30 = Wake up #2
    Delay(10); //must wait 160us, busy flag not available
    Nybble(); //command 0x30 = Wake up #3
    Delay(10); //can check busy flag now instead of delay
    setPortB(0x20); //put 0x20 on the output port
    Nybble(); //Function set: 4-bit interface

    setFunction(0,1,0); //Function set: 4-bit/2-line

    setShifting(0,0); //Set cursor

    setDisplay(1,0,0);  //Display ON; No cursor

    setEntryMode(1,0); //Entry Mode set
}