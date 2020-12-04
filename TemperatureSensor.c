#include <p32xxxx.h>
#include "Buttons.h"
#include "TemperatureSensor.h"

int getTemperature(void) { // TODO make this work
    int finalTemperature = 0;
    if (!OW_reset_pulse()) {
        OW_write_byte(0xCC); // Skip ROM
        OW_write_byte(0x44); // Convert temperature in degrees
        Delay(100); // TODO what delay?
        OW_reset_pulse();
        OW_write_byte(0xCC); // Skip ROM
        OW_write_byte(0xBE); // Get all the memory
       
        finalTemperature = OW_read_byte() | (int)(OW_read_byte() << 8);
        
        //TEMPERATURE = TEMP_READ - 0.25 + (COUNT_PER_C - COUNT_REMAIN/COUNT_PER_C)
        // TODO we need the above fomula to calculate temp, but how?!
        OW_reset_pulse();
    }
    return finalTemperature;
}

void driveOW(char Bit) {
    PIN_DIRECTION = OUTPUT;
    WRITE_PIN = Bit;
}

int OW_reset_pulse(void) {
    int presence_detect = 0;
    driveOW(LOW); // pulling the 1-Wire bus low
    Delayus(480); // delay to go from transmit to receive mode
    driveOW(HIGH); // pulling the 1-Wire bus high, releases
    Delayus(70); //  transmits a presence pulse by pullng the 1-Wire bus low
    presence_detect = read_OW(); // pulled low for about 60?s to 240?s
    Delayus(410); // Complete the reset sequence recovery
    return presence_detect; // 0=presence, 1 = no part
}

/*
 * Writing to one wire
 */
void OW_write_byte(unsigned char data) {
    int loop;
    // Loop to write each bit in the byte, LS-bit first
    for (loop = 0; loop < 8; loop++) {
        //OW_write_bit((write_data >> loop) & 0x01);
        OW_write_bit(data & 0x01); //Sending LS-bit first
        data >>= 1; // shift the data byte for the next bit to send
    }
}

void OW_write_bit(unsigned char write_bit) {
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
unsigned char OW_read_byte(void) {
    char loop, result = 0;

    for (loop = 0; loop < 8; loop++) {
        result >>= 1; // shift the result to get it ready for the next bit to receive
        if (OW_read_bit())
            result |= 0x80; // if result is one, then set MS-bit
    }
    return result;
}

unsigned char OW_read_bit(void) {
   char result;

    driveOW(LOW); // Drive the bus low

    Delayus(6);
    
    driveOW(HIGH); // Release the bus
    
    Delayus(9);

    result = read_OW(); //Read the status of OW_PIN
    
    Delayus(55); // Complete the time slot and 10us recovery
    
    return result;
}

unsigned char read_OW(void) {
    PIN_DIRECTION = INPUT;
    return READ_PIN ? 1 : 0;
}

/*
 * End of reading from one wire
 */
 
void configurePrecision(unsigned char precision) {
    char value;
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
    
    OW_reset_pulse();
    OW_write_byte(0xCC); //Skip ROM
    OW_write_byte(0x4E); // Configure command
    OW_write_byte(0x00); // Alarm 
    OW_write_byte(0x00); // Alarm 
    OW_write_byte(value); // set precision 
}
