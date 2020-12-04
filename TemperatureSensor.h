#ifndef TEMPERATURE_SENSOR_H_INCLUDED
#define TEMPERATURE_SENSOR_H_INCLUDED

/*
 * Below defines pins for sensor
 */
#define PIN_DIRECTION 	_TRISA0
#define WRITE_PIN  		_LATA0
#define READ_PIN		_RA0

/*
 * Below defines commands set for sensor
 */
#define CONVERT_T                 0x44
#define READ_SCRATCHPAD     0xBE
#define WRITE_SCRATCHPAD    0x4E
#define COPY_SCRATCHPAD     0x48
#define RECALL_E                     0xB8
#define READ_POWER               0xB4

/*
 * Below defines ROM commands
 */
#define SEARCH_ROM              0xF0
#define READ_ROM                  0x33
#define MATCH_ROM                0x55
#define SKIP_ROM                    0xCC
#define ALARM_SEARCH            0xEC

void DriveOW(unsigned char Bit);
int ResetPulse();
unsigned char ReadByte();
unsigned char ReadBit();
unsigned char ReadOW();
void WriteByte(unsigned char write_data);
void WriteBit(unsigned char write_data);
int getTemperature();
void configurePrecision(unsigned char precision);

#endif
