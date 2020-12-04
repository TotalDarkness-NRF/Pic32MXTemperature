#ifndef TEMPERATURE_SENSOR_H_INCLUDED
#define TEMPERATURE_SENSOR_H_INCLUDED

#define PIN_DIRECTION 	_TRISA0
#define WRITE_PIN  		_LATA0
#define READ_PIN		_RA0

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
