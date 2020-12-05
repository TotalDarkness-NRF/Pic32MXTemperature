#include "Thermometer.h"

uint8_t data[9];

int getTemperature(void) {
    double temperature;
    getScratchpad();
    uint16_t tempRead = data[0] | data[1] >> 8;
    tempRead >>= 1;
    tempRead = (tempRead*100) - 25;
    uint8_t  CountRemain = data[6];
    uint8_t CountPerC = data[7];
    temperature = tempRead + (((CountPerC - CountRemain) * 100)/ CountPerC);
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
 
void configurePrecision(unsigned char precision) {
    unsigned char value;
    switch(precision) {
        case 9:
            value = 0;
            break;
        case 10:
            value = 32;
            break;
        case 11:
            value = 64;
            break;
        defualt:
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

/**
 * Checks if the temperature is a reasonable number
 * @return true if sensor is present; else false
 */
int ifSensorPresent(void) {
    return !ResetPulse();
}

/**
 * Get the Scratchpad object
 * @return uint8_t containing the bytes that were read from the Scratchpad
 */
    void getScratchpad(void) {
    int i;
    WriteByte(SKIP_ROM);
    WriteByte(CONVERT_T);
    Delay(750); // TODO set delay based on precision
    ResetPulse();
    WriteByte(SKIP_ROM);
    WriteByte(READ_SCRATCHPAD);
    for (i = 0; i < 9; i++)
        data[i] = ReadByte();
}