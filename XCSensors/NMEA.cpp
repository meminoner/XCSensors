/*
  XCSensors http://XCSensors.org

  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/
#include <Arduino.h>
#include "NMEA.h"
#include <stdio.h>
#include <stdlib.h>


const char *hex = "0123456789ABCDEF";
char t_check[3];

NMEA::NMEA() {

}



void NMEA::setGforce(float gforce, float airspeed) { //$FBG does not exist, unable to find a gforce NMEA sentence

  /* Haven't found a NMEA sentence for G-Force. This one is made up.
     $XCSG,0,*hh/CR/LF

     Field Number:
      1) G-Force
      2) Checksum
  */

  char t_NmeaAgforce[15] = "$XCTOD,";
  char t_gforce[5];
  char t_airspeed[5];
  char t_tail[2] = ",";

  dtostrf(gforce, 2, 1, t_gforce);
  strcat(t_NmeaAgforce, t_gforce );

  strcat(t_NmeaAgforce, t_tail);

  sprintf(t_airspeed,"%d",(int)airspeed);
  // dtostrf(airspeed, 2, 1, t_airspeed);
  strcat(t_NmeaAgforce, t_airspeed);

  strcat(t_NmeaAgforce, t_tail);
  strcat(t_NmeaAgforce, "*");
  getCRC(t_NmeaAgforce);
  strcat(t_NmeaAgforce, t_check);

  strcpy(nmeaGforce, t_NmeaAgforce);
}



void NMEA::setPTAS1(double vario, double varioAv, double realAltitude) {
  //$PTAS1,xxx,yyy,zzzzz,aaa*CS<CR><LF>
  char t_nmeaPTAS1[44] = "$PTAS1,";
  char t_av[7];
  char t_cv[7];
  char t_altitudeF[10];
  char t_tail[2] = ","; //including tas
  char t_comma[2] = ",";

  //Convert to feet
  float cv = vario * 1.943 * 10 + 200;
  float av = 0;

  long altitudeF = realAltitude * 3.28 + 2000;

#if defined(PTASAVERAGE)
  av = varioAv * 1.943 * 10 + 200;
#endif


  dtostrf(cv, 3, 2, t_cv);
  strcat(t_nmeaPTAS1, t_cv);
  strcat(t_nmeaPTAS1, t_comma);

  dtostrf(av, 3, 2, t_av);
  strcat(t_nmeaPTAS1, t_av);
  strcat(t_nmeaPTAS1, t_comma);

  dtostrf(altitudeF, 6, 2, t_altitudeF);
  strcat(t_nmeaPTAS1, t_altitudeF);


  strcat(t_nmeaPTAS1, t_tail);
  strcat(t_nmeaPTAS1, "*");

  getCRC(t_nmeaPTAS1);
  strcat(t_nmeaPTAS1, t_check);

  strcpy(nmeaPTAS1, t_nmeaPTAS1);

}

void NMEA::setnmeaVarioLXWP0(double varioAlt, float a, float b, float c, float d, float e, float f) { //in m/s
  // $LXWP0,logger_stored, airspeed, airaltitude,
  //   v1[0],v1[1],v1[2],v1[3],v1[4],v1[5], hdg, windspeed*CS<CR><LF>
  //
  // 0 loger_stored : [Y|N] (not used in LX1600)
  // 1 IAS [km/h] ----> Condor uses TAS!
  // 2 baroaltitude [m]
  // 3-8 vario values [m/s] (last 6 measurements in last second)
  // 9 heading of plane (not used in LX1600)
  // 10 windcourse [deg] (not used in LX1600)
  // 11 windspeed [km/h] (not used in LX1600)
  //
  // e.g.:
  // $LXWP0,Y,222.3    ,1665.5 ,1.71 ,,,,,,239,174,10.1
  // $LXWP0, ,IAS(km/h),baroalt,vario,    ,    ,    ,    ,    ,,,
  // $LXWP0, ,45.1     ,167.77 ,0.00 ,0.00,0.08,0.08,0.08,0.08,,,*23
  // $PDGFTL1, QNE m, QNH m, vario cm/s, netto vario , IAS km/h, L/D ground tens, wind speed km/h , wind dir, volt battery hundreds, *chksum
  // $PDGFTL1, 150.15,150.12,0.0,0.0,45.1,10,0.0,0.0,720,

  char t_nmeaVarioLXWP0[60] = "$LXWP0,N,,";
  // char t_nmeaVarioLXWP0[60] = "$PDGFTL1,150.15,150.12,0.0,0.0,45.1,10,0.0,0.0,720,";
  char t_vario[5];
  char t_alt[9];

  char t_tail[3] = ",,";
  char t_comma[2] = ",";

  dtostrf(varioAlt, 5, 2, t_alt);
  strcat(t_nmeaVarioLXWP0, t_alt);
  strcat(t_nmeaVarioLXWP0, t_comma);
  dtostrf(a, 2, 2, t_vario);
  strcat(t_nmeaVarioLXWP0, t_vario);
  strcat(t_nmeaVarioLXWP0, t_comma);
  dtostrf(b, 2, 2, t_vario);
  strcat(t_nmeaVarioLXWP0, t_vario);
  strcat(t_nmeaVarioLXWP0, t_comma);
  dtostrf(c, 2, 2, t_vario);
  strcat(t_nmeaVarioLXWP0, t_vario);
  strcat(t_nmeaVarioLXWP0, t_comma);
  dtostrf(d, 2, 2, t_vario);
  strcat(t_nmeaVarioLXWP0, t_vario);
  strcat(t_nmeaVarioLXWP0, t_comma);
  dtostrf(e, 2, 2, t_vario);
  strcat(t_nmeaVarioLXWP0, t_vario);
  strcat(t_nmeaVarioLXWP0, t_comma);
  dtostrf(f, 2, 2, t_vario);
  strcat(t_nmeaVarioLXWP0, t_vario);
  strcat(t_nmeaVarioLXWP0, t_comma);

  strcat(t_nmeaVarioLXWP0, t_tail);
  strcat(t_nmeaVarioLXWP0, "*");

  getCRC(t_nmeaVarioLXWP0);

  strcat(t_nmeaVarioLXWP0, t_check);

  strcpy(nmeaVarioLXWP0, t_nmeaVarioLXWP0);


}


void NMEA::setNmeaVarioSentence(long rawPressure, double varioAlt, float climbRate, float temperature, float voltage) {
  //https://github.com/LK8000/LK8000/blob/master/Docs/LK8EX1.txt
  //float t_temp = temperature/100;// not correct anymore
  char t_nmeaVario[44] = "$LK8EX1,";
  char t_climbRate[6];
  char t_pressure[7];
  char t_alt[6];
  char t_temperature[6];
  char t_voltage[4];
  //char t_tail[2] = ",";
  char t_comma[2] = ",";

  dtostrf(rawPressure, 0, 0, t_pressure);
  strcat(t_nmeaVario, t_pressure);
  strcat(t_nmeaVario, t_comma);

  dtostrf(varioAlt, 0, 0, t_alt);
  strcat(t_nmeaVario, t_alt);
  strcat(t_nmeaVario, t_comma);


  dtostrf(climbRate, 0, 2, t_climbRate);
  strcat(t_nmeaVario, t_climbRate);
  strcat(t_nmeaVario, t_comma);

  dtostrf(temperature, 0, 2, t_temperature);
  strcat(t_nmeaVario, t_temperature);
  strcat(t_nmeaVario, t_comma);

  dtostrf(voltage, 0, 1, t_voltage);
  strcat(t_nmeaVario, t_voltage);

  //strcat(t_nmeaVario, t_tail);
  strcat(t_nmeaVario, "*");

  getCRC(t_nmeaVario);

  strcat(t_nmeaVario, t_check);

  strcpy(nmeaVario, t_nmeaVario);

}



void NMEA::setNmeaHumidSentence(int temperatureH11Kelvin, int humidity) {

  char t_nmeaHumid[22] = "$PDVVT,";
  char t_temp[6];
  char t_humidity[3];
  //  char t_tail[2] = ",";
  char t_comma[2] = ",";

  dtostrf(temperatureH11Kelvin, 4, 0, t_temp);
  strcat(t_nmeaHumid, t_temp);
  strcat(t_nmeaHumid, t_comma);

  dtostrf(humidity, 2, 2, t_humidity);
  strcat(t_nmeaHumid, t_humidity);


  // strcat(t_nmeaHumid, t_tail);
  strcat(t_nmeaHumid, "*");

  getCRC(t_nmeaHumid);

  strcat(t_nmeaHumid, t_check);
  strcpy(nmeaHumid, t_nmeaHumid);


}

//Using the C-probe sentence for LK8000
void NMEA::setNmeaPcProbeSentence(float aax, float aay, float aaz, int temperature, int humidity, int batVolt) {

  // $PCPROBE,T,Q0,Q1,Q2,Q3,ax,ay,az,temp,rh,batt,delta_press,abs_press,C,
  // - "T" after "$PCPROBE" indicates that the string contains data. Data are represented as signed,
  //  16-bit hexadecimal integers. The only exception is abs_press which is in signed 24-bits hex
  //  format.
  // - Q0, Q1, Q2, Q3: 3D orientation of the C-Probe in quaternion format. Heading, pitch, and roll can
  // - temp: temperature in units of 0.1°C.
  // - rh: relative humidity in units of 0.1%.
  // - batt: battery level from 0 to 100%.
  // - delta_press: differential pressure (dynamic - static) in units of 0.1 Pa.
  // - abs_press: absolute pressure in units of 1/400 Pa
  // - C: is transmitted only if the C-Probe is being charged. In this case, heat produced by the charging
  //    process is likely to affect the readings of the temperature and humidity sensors.

  char t_nmeaPcProbe[60] = "$PCPROBE,T,,,,,";

  char t_comma[2] = ",";

  //convert to sentence specs
  int caax = aax * 1000;
  int caay = aay * 1000;
  int caaz = aaz * 1000;

  int cTemp = temperature * 10;
  int cRh = humidity * 10;
  int cBat = batVolt * 1000;
  
  strcat(t_nmeaPcProbe, (String(caax,HEX)).c_str());
  strcat(t_nmeaPcProbe, t_comma);
 
  strcat(t_nmeaPcProbe, (String(caay,HEX)).c_str());
  strcat(t_nmeaPcProbe, t_comma);
  
  strcat(t_nmeaPcProbe, (String(caaz,HEX)).c_str());
  strcat(t_nmeaPcProbe, t_comma);

  strcat(t_nmeaPcProbe, (String(cTemp,HEX)).c_str());
  strcat(t_nmeaPcProbe, t_comma);

  strcat(t_nmeaPcProbe, (String(cRh,HEX)).c_str());
  strcat(t_nmeaPcProbe, t_comma);

  strcat(t_nmeaPcProbe, (String(cBat,HEX)).c_str());
  strcat(t_nmeaPcProbe, t_comma);

  strcat(t_nmeaPcProbe, t_comma); //delta pressure
  strcat(t_nmeaPcProbe, t_comma); //abs prssure
  strcat(t_nmeaPcProbe, t_comma);
  strcat(t_nmeaPcProbe, "*");

  getCRC(t_nmeaPcProbe);
  strcat(t_nmeaPcProbe, t_check);
  strcpy(nmeaPcProbe, t_nmeaPcProbe);


}


void NMEA::getCRC(char *buff) {
  // NMEA CRC: XOR each byte with previous for all chars between '$' and '*'
  char c;
  byte i;
  byte start_with = 0;
  byte end_with = 0;
  char CRC = 0;

  for (i = 0; i < 128; i++) {
    c = buff[i];
    if (c == 0x24) {
      start_with = i;
    }
    if (c == 0x2a) {
      end_with = i;
      break;
    }
  }
  if (end_with > start_with) {
    for (i = start_with + 1; i < end_with; i++) { // XOR every character between '$' and '*'
      CRC = CRC ^ buff[i] ;  // compute CRC

    }

  }

  //Single threaded, so this is allowed
  t_check[0] = hex[(CRC >> 4) & 0xF];
  t_check[1] = hex[(CRC) & 0xF];
  t_check[2] = '\0';

  //based on code by Elimeléc López - July-19th-2013
}


