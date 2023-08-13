
#include "Airspeed.h"
#include <Wire.h>
#include <stdint.h> // Standard C, Allows explicit data type declaration.


// fetch_airspeed_pressure is a function to do the I2C read and extraction of the three data fields
//
uint8_t fetch_airspeed_pressure(uint16_t &P_dat, uint16_t &T_dat)
{
uint8_t _status;
uint8_t Press_H;
uint8_t Press_L;
uint8_t Temp_H;
uint8_t Temp_L;

Wire.requestFrom(MS4525DAddress, static_cast<uint8_t>(4), static_cast<uint8_t>(true)); //Request 4 bytes, 2 pressure/status and 2 temperature
Press_H = Wire.read();
Press_L = Wire.read();
Temp_H = Wire.read();
Temp_L = Wire.read();

_status = (Press_H >> 6) & 0x03;
Press_H = Press_H & 0x3f;
P_dat = (((uint16_t)Press_H) << 8) | Press_L;

Temp_L = (Temp_L >> 5);
T_dat = (((uint16_t)Temp_H) << 3) | Temp_L;

return _status;

}