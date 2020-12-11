#include "Thermometer.h"

#pragma config FNOSC = FRCPLL       // Internal Fast RC oscillator (8 MHz) w/ PLL
#pragma config FPLLIDIV = DIV_2     // Divide FRC before PLL (now 4 MHz)
#pragma config FPLLMUL = MUL_20     // PLL Multiply (now 80 MHz)
#pragma config FPLLODIV = DIV_2     // Divide After PLL (now 40 MHz)
#pragma config FWDTEN = OFF         // Watchdog Timer Disabled
#pragma config ICESEL = ICS_PGx1    // ICE/ICD Comm Channel Select
#pragma config JTAGEN = OFF         // Disable JTAG
#pragma config FSOSCEN = OFF        // Disable Secondary Oscillator
#pragma interrupt Timer1Interrupt ipl1soft vector 0  // Setup for an intterupt for timer1

int time = 0;
int viewMode = 1;
void main (void) {
    char tempPrescence, LCDString[32];
    
    T1CON = 0x8000;         // TMR1 on, prescaler 1:1
    PR1 = SYS_FREQ/2/60; // every ms
    TMR1 = 0;
    __asm__("EI"); // Enable interrupts
    IFS0CLR=_IFS0_T1IF_MASK; // Timer1 Interrupt Flag cleared
    IPC1bits.T1IP = 1; // Timer1 Interrupt Priority 1
    IEC0bits.T1IE = 1; // Timer1 Interrupt Enable
    
    ANSELA = 0x00;
    ANSELB = 0x00;
    TRISA = 0x00;
    TRISB = 0x3F; 
    initLCD();
    while (1) {
        if (ifSensorPresent()) {
            float temperature = (float) getTemperature() / 100;
            int precision = getPrecision();
            // For whatever reason compiler hates returning floating point numbers
            if (viewMode) {
                if (precision == 9) {
                    sprintf(LCDString, "Temperature:    %3.2f%c",temperature, getUnit());
                } else if (precision == 10) {
                    sprintf(LCDString, "Temperature:    %3.3f%c",temperature, getUnit());
                } else if (precision == 11) {
                    sprintf(LCDString, "Temperature:    %3.6f%c",temperature, getUnit());
                } else {
                    sprintf(LCDString, "Temperature:    %3.9f%c",temperature, getUnit());
                }
                displayString(LCDString);
            } else {
                int ms = time;
                int hours = ms / 3600000;
                ms -=  hours *3600000;
                int minutes = ms/ 60000;
                ms -= minutes * 60000;
                int seconds = ms / 1000;
                ms -= seconds * 1000;
                sprintf(LCDString, "%dh:%dm:%ds:%d", hours, minutes, seconds, ms);
                displayStringStart(0, LCDString);
                if (precision == 9) {
                    sprintf(LCDString, "%3.2f%c",temperature, getUnit());
                } else if (precision == 10) {
                    sprintf(LCDString, "%3.3f%c",temperature, getUnit());
                } else if (precision == 11) {
                    sprintf(LCDString, "%3.6f%c",temperature, getUnit());
                } else {
                    sprintf(LCDString, "%3.9f%c",temperature, getUnit());
                }
                displayStringStart(17, LCDString);
            }
            
            if (!tempPrescence) {
                tempPrescence = 1;
                setPrecision(precision);
                clearDisplay();
            }
        } else {
            sprintf(LCDString, "SENSOR NOT FOUND~~~~~ERROR!~~~~~");
            displayString(LCDString);
            tempPrescence = 0;
        }
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

void Timer1Interrupt(void) {
    time++;
    //Hacky button updating.
    Color();
    ChangePrecision();
    ChangeTemperatureUnit();
    viewMode = ChangeViewMode(viewMode);
    IFS0CLR=_IFS0_T1IF_MASK;  // Timer1 Interrupt Flag cleared
    TMR1 = 0;
}