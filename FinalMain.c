#include "Thermometer.h"

#pragma config FNOSC = FRCPLL       // Internal Fast RC oscillator (8 MHz) w/ PLL
#pragma config FPLLIDIV = DIV_2     // Divide FRC before PLL (now 4 MHz)
#pragma config FPLLMUL = MUL_20     // PLL Multiply (now 80 MHz)
#pragma config FPLLODIV = DIV_2     // Divide After PLL (now 40 MHz)
#pragma config FWDTEN = OFF         // Watchdog Timer Disabled
#pragma config ICESEL = ICS_PGx1    // ICE/ICD Comm Channel Select
#pragma config JTAGEN = OFF         // Disable JTAG
#pragma config FSOSCEN = OFF        // Disable Secondary Oscillator

void main (void) {
    char tempPrescence, LCDString[32];
    ANSELA = 0x00;
    ANSELB = 0x00;
    TRISA = 0x00;
    TRISB = 0x3F;
    initLCD();
    while (1) {
        if (ifSensorPresent()) {
            float temperature = (float) getTemperature() / 100.0;
            // For whatever reason compiler hates returning floating point numbers
            int precision = getPrecision();
            if (precision == 9) {
                sprintf(LCDString, "Temperature:    %3.2f%c",temperature, getUnit());
            } else if (precision == 10) {
                sprintf(LCDString, "Temperature:    %3.3f%c",temperature, getUnit());
            } else if (precision == 11) {
                sprintf(LCDString, "Temperature:    %3.6f%c",temperature, getUnit());
            } else {
                sprintf(LCDString, "Temperature:    %3.9f%c",temperature, getUnit());
            }
            if (!tempPrescence) {
                tempPrescence = 1;
                clearDisplay();
            }
        } else {
            sprintf(LCDString, "Temperature:    SENSOR NOT FOUND");
            tempPrescence = 0;
        }
        displayString(LCDString);
        Color();
        ChangePrecision();
        ChangeTemperatureUnit();
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