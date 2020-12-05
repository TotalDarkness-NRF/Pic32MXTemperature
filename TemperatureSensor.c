#include <p32xxxx.h>
#include "Buttons.h"
#include "TemperatureSensor.h"

int getTemperature(void) {
    double temperature = -999999; // Impossible value
    if (!ResetPulse()) {
        WriteByte(SKIP_ROM);
        WriteByte(CONVERT_T);
        Delay(750); // TODO set delay based on precision
        ResetPulse();
        WriteByte(SKIP_ROM);
        WriteByte(READ_SCRATCHPAD);
       int i;
       uint8_t data[9];
        for (i = 0; i <9; i++) {
            data[i] = ReadByte();
        }
       
       uint16_t tempRead = data[0];
       tempRead >>= 1;
       tempRead = (tempRead*100) - 25;
       uint8_t  CountRemain = data[6];
       uint8_t CountPerC = data[7];
       temperature = tempRead + (((CountPerC - CountRemain) * 100)/ CountPerC);
       if (data[1] > 0x80) // sign bit set, temp is negative
            temperature = temperature * -1; 
        ResetPulse();
    }
    return (temperature);
}

void driveOW(unsigned char bit) {
    PIN_DIRECTION = OUTPUT;
    WRITE_PIN = bit ? 1 : 0;
}

int ResetPulse() {
    int presence_detect = 0;
    driveOW(LOW); // pulling the 1-Wire bus low
    Delayus(480); // delay to go from transmit to receive mode
    driveOW(HIGH); // pulling the 1-Wire bus high, releases
    Delayus(70); //  transmits a presence pulse by pullng the 1-Wire bus low
    presence_detect = ReadOW(); // pulled low for about 60?s to 240?s
    Delayus(410); // Complete the reset sequence recovery
    return presence_detect; // 0=presence, 1 = no part
}

/*
 * Writing to one wire
 */
void WriteByte(unsigned char data) {
    unsigned char i;
    // Loop to write each bit in the byte, LS-bit first
    for (i = 0; i < 8; i++) {
        WriteBit(data & 0x01); //Sending LS-bit first
        data >>= 1; // shift the data byte for the next bit to send
    }
}

void WriteBit(unsigned char write_bit) {
    if (write_bit) {
        //writing a bit '1'
        driveOW(LOW); // Drive the bus low
        Delayus(6); // pulls early becomes 1
        driveOW(HIGH); // Release the bus
        Delayus(64); // Complete the time slot and 10us recovery
    } else {
        //writing a bit '0'
        driveOW(LOW); // Drive the bus low
        Delayus(60); // Does nothing so becomes zero
        driveOW(HIGH); // Release the bus
        Delayus(10);
    }
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
