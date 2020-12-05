#include "Thermometer.h"

int precision = 9;
uint8_t scratchPad[9];

int getTemperature() {
    double temperature;
    getScratchpad();
    uint16_t tempRead = scratchPad[0];
    tempRead >>= 1; //truncate the LSB of the temperature reading by 0.5
    tempRead = (tempRead*100) - 25;
    uint8_t  CountRemain = scratchPad[6];
    uint8_t CountPerC = scratchPad[7];
    temperature = tempRead + (((CountPerC - CountRemain) * 100)/ CountPerC);
     if (scratchPad[1] >> 7)  // Check MSB for sign bit
         temperature = -temperature;
    ResetPulse();
    return temperature;
}

void driveOW(unsigned char bit) {
    PIN_DIRECTION = OUTPUT;
    WRITE_PIN = bit & 0x01;
}

int ResetPulse() {
    driveOW(LOW);
    Delayus(480);
    driveOW(HIGH);
    Delayus(70);
    int presenceDetect = ReadOW();
    Delayus(410);
    return presenceDetect; // 0 = presence, 1 = no part
}

void WriteByte(unsigned char data) {
    int i;
    for (i = 0; i < 8; i++) {
        WriteBit((data & 0x01));
        data >>= 1;
    }
}

void WriteBit(unsigned char write_bit) {
    if (write_bit)
        WriteBitOne();
    else
        WriteBitZero();
}

void WriteBitOne(void) {
    // Writing a 1:
    // Pull early sets wire to one followed by a delay of 6us then a HIGH
    driveOW(LOW);
    Delayus(6);
    driveOW(HIGH);
    Delayus(64); //The remainder of the time has to be completed for the cycle to total 70us
}
void WriteBitZero(void) {
    //Writing a Zero
    // Pull early sets wire to one followed by a delay of 6us then a HIGH
    driveOW(LOW);
    Delayus(60);
    driveOW(HIGH);
    Delayus(10); // The remainder of the time has to be completed for the cycle to total 70us
}

/*
 * End of writing to one wire
 */

/*
 * Read from one wire
 */
unsigned char ReadByte() {
    unsigned char i, result = 0;   // TODO can this be unsigned ?
    for (i = 0; i < 8; i++) {
        result >>= 1; // shift the result to get it ready for the next bit to receive
        if (ReadBit())
            result |= 0x80; // if result is one, then set MS-bit
    }
    return result;
}

unsigned char ReadBit() {
    unsigned char bit;
    driveOW(LOW); // Drive the bus low
    Delayus(6);
    driveOW(HIGH); // Release the bus
    Delayus(9);
    bit = ReadOW(); //Read the status of OW_PIN
    Delayus(55); // Complete the time slot and 10us recovery
    return bit;
}

unsigned char ReadOW() {
    PIN_DIRECTION = INPUT;
    return READ_PIN & 0x01;
}

/*
 * End of reading from one wire
 */
 
int getCoversionDelay() {
    switch(precision) {
        case 9:
            return 94;
        case 10:
            return 188;
        case 11:
            return 375;
        case 12:
            return 750;
    }
}

/**
 * Check if the sensor is present
 */
int ifSensorPresent(void) {
    return !ResetPulse();
}

/*
 * Get the contents of the scratchpad
 */
    void getScratchpad(void) {
    int i;
    WriteByte(SKIP_ROM);
    WriteByte(CONVERT_T);
    Delay(getCoversionDelay());
    ResetPulse();
    WriteByte(SKIP_ROM);
    WriteByte(READ_SCRATCHPAD);
    for (i = 0; i < 9; i++)
        scratchPad[i] = ReadByte();
}
    
    void setPrecision(int type) {
    int value;
    switch(type) {
        case 9:
            precision = 9;
            value = 0;
            break;
        case 10:
            precision = 10;
            value = 32;
            break;
        case 11:
            precision = 11;
            value = 64;
            break;
        case 12:
            precision = 12;
            value = 96;
            break;
    }
    ResetPulse();
    WriteByte(SKIP_ROM);
    WriteByte(WRITE_SCRATCHPAD);
    WriteByte(0x00); // Alarm 
    WriteByte(0x00); // Alarm 
    WriteByte(value); // set precision 
}
    
    int getPrecision() {
        return precision;
    }