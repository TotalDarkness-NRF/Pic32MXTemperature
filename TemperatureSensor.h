#ifndef TEMPERATURE_SENSOR_H_INCLUDED
#define TEMPERATURE_SENSOR_H_INCLUDED

#define PIN_DIRECTION 	_TRISA0
#define WRITE_PIN  		_LATA0
#define READ_PIN		_RA0

void driveOW (char Bit);
unsigned char read_OW (void);
void OW_write_bit (unsigned char write_data);
unsigned char OW_read_bit (void);
int OW_reset_pulse(void);
void OW_write_byte (unsigned char write_data);
unsigned char OW_read_byte (void);
int getTemp(void);
void configurePrecision(unsigned char precision);

#endif
