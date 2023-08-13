
#include <Arduino.h>
#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////////////
// MS4525D sensor characteristics (from the 4/2019 version of the data sheet)
//////////////////////////////////////////////////////////////////////////////////////

// MS4525D sensor I2C address (uncomment the Interface Type of the device you are using)
// Interface Type I
const uint8_t MS4525DAddress = 0x28;
// Interface Type J
//const uint8_t MS4525DAddress = 0x36;
// Interface Type K
//const uint8_t MS4525DAddress = 0x46;
// Interface Type 0
//const uint8_t MS4525DAddress = 0x48;

// MS4525D sensor full scale range and units
const int16_t MS4525FullScaleRange = 1; // 1 psi
//const int16_t MS4525FullScaleRange = 0.0689476; // 1 psi in Bar
//const int16_t MS4525FullScaleRange = 6895; // 1 psi in Pascal
//const int16_t MS4525FullScaleRange = 2; // 2 psi
//const int16_t MS4525FullScaleRange = 5; // 5 psi

// MS4525D Sensor type (A or B) comment out the wrong type assignments
// Type A (10% to 90%)
const int16_t MS4525MinScaleCounts = 1638;
const int16_t MS4525FullScaleCounts = 14746;
// Type B (5% to 95%)
//const int16_t MS4525MinScaleCounts = 819;
//const int16_t MS4525FullScaleCounts = 15563;
const int16_t MS4525Span=MS4525FullScaleCounts-MS4525MinScaleCounts;

//MS4525D sensor pressure style, differential or otherwise. Comment out the wrong one.
//Differential
const int16_t MS4525ZeroCounts=(MS4525MinScaleCounts+MS4525FullScaleCounts)/2;
// Absolute, Gauge
//const int16_t MS4525ZeroCounts=MS4525MinScaleCounts;

//////////////////////////////////////////////////////////////////////////////////////
// end of MS4525D sensor characteristics
//////////////////////////////////////////////////////////////////////////////////////


uint8_t fetch_airspeed_pressure(uint16_t &P_dat, uint16_t &T_dat);

