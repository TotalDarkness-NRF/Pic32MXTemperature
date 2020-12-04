#include <p32xxxx.h>
#include<stdio.h>

#pragma config FNOSC = FRCPLL       // Internal Fast RC oscillator (8 MHz) w/ PLL
#pragma config FPLLIDIV = DIV_2     // Divide FRC before PLL (now 4 MHz)
#pragma config FPLLMUL = MUL_20     // PLL Multiply (now 80 MHz)
#pragma config FPLLODIV = DIV_2     // Divide After PLL (now 40 MHz)
#pragma config FWDTEN = OFF         // Watchdog Timer Disabled
#pragma config ICESEL = ICS_PGx1    // ICE/ICD Comm Channel Select
#pragma config JTAGEN = OFF         // Disable JTAG
#pragma config FSOSCEN = OFF        // Disable Secondary Oscillator
 
/*
 *  Global Constants
 */
#define SYS_FREQ 40000000

void Delayus(unsigned int us);
void Delay(unsigned int ms);

main () {
    ANSELA = 0x00;
    ANSELB = 0x00;
    TRISA = 0x00;
    TRISB = 0x3F;
    Color();
    initLCD();
    clearDisplay();
    char LCDString[32];
    while (1) {
        sprintf(LCDString, "Temperature:    %d%cC", getTemperature(), 0xDF); // convert to string
        displayString(LCDString);
        Color(); // We need to deal with this using an interrupt, temp takes too long, makes this lag
        // TODO clear the screen every so often
    }
}

void Delayus(unsigned int us) {
    // Convert microseconds us into how many clock ticks it will take
    us *= (SYS_FREQ / 1000000 / 2); // Core Timer updates every 2 ticks
    _CP0_SET_COUNT(0); // Set Core Timer count to 0
    while (us > _CP0_GET_COUNT()); // Wait until Core Timer count reaches the number we calculated earlier
}

void Delay(unsigned int ms) {
    Delayus(ms * 1000);
}