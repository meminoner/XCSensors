/*
  XCsensors by Marco van Zoest

  www.primaldev.nl
  www.primalcode.nl

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/


#include "config.h"
#include "Conf.h"
#include "XCSensors.h"
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "SubFunctions.h"

float f = 0.00f;
int eeAddress = 0;

void getConfig() { //load default config values from EEPROM

  getConfigFromEEPROM();
  if (!conf.hasSavedtoEEPROM) {
    conf.hasSavedtoEEPROM = true;
    setDefaultConfig();
    getConfigFromEEPROM();
    
  }

  getConfigVars();

}

void setDefaultConfig() {

  for (int i = 0 ; i < EEPROM.length() ; i++) { //clear the EEPROM
    EEPROM.write(i, 0);
  }
  getDefaultConfig();
  saveConfigToEEPROM();


}

void getDefaultConfig() {
  conf.GPSChannel = 1;
  conf.varioChannel = 2;
  conf.humidChannel = 3;
  conf.magChannel = 1;
  conf.AcclChannel = 4;
//  strcpy(conf.ssid, "XCSensors");
//  strcpy(conf.password, "thereisnospoon");
  conf.magDeclination = 0; //in Radians. Find your declination here: http://www.magnetic-declination.com/
  conf.qnePressure = 101325; //QNH value to calculate vario Altitude
  conf.varioDeadBand = 100; // X 1000 levels lower than this = 0. (ignored by ptas1)
  conf.wifiMultiPort = true; //use different ports for NMEA sentences, or only the gps port
  conf.bluetoothOnly = true; //only send data to BT. reset needed to start WIFI
  conf.ptas1 = true; // send ptas1 nmea, uses the gps channel (once every 155ms)
  conf.lxnav = true; //send vario lxnav sentence
  conf.accloffset = 0; //manual offset for accl x 1000
  conf.variosmooth = 17; //low pass filter, the higher the number the slower the raw vario reading changes.
  conf.buzzer = true; // turn vario audio on or off
  conf.varioAudioDeadBand = 200; //X 1000
  conf.varioAudioSinkDeadBand = 300;  //X 1000 and absolute value
  conf.advTriggerTime = 1500; // if vario level goes lower than advLowTrigger in this time, it will cause a trigger and increase conf.variosmooth.
  conf.advRelaxTime= 30000;  // if no trigger occurs in this time frame, conf.variosmooth is reduced by 1,
  conf.advMinSmooth=10; // lowest level for conf.variosmooth
  conf.advMaxSmooth=35;// highest level for conf.variosmooth
}



#if defined(HUMANCONFIG)


void printaf(int n) {
  switch (n) {

    case 1: SERIAL_MAIN.print(F("1) Bluetooth Only: ")); break;
    case 2: SERIAL_MAIN.print(F("2) Wifi Multiport: ")); break;
    case 3: SERIAL_MAIN.print(F("3) GPS Channel: ")); break;
    case 4: SERIAL_MAIN.print(F("4) Vario Channel: ")); break;
    case 5: SERIAL_MAIN.print(F("5) Humid Sensor Channel: ")); break;
    case 6: SERIAL_MAIN.print(F("6) Mag track Channel: ")); break;
    case 7: SERIAL_MAIN.print(F("7) Accl Channel: ")); break;
 //   case 8: SERIAL_MAIN.print(F("8) Wifi SSID: ")); break;
//    case 9: SERIAL_MAIN.print(F("9) Wifi Password: ")); break;
    case 10: SERIAL_MAIN.print(F("10) Magentic Declination: ")); break;
    case 11: SERIAL_MAIN.print(F("11) QNE Pressure: ")); break;
    case 12: SERIAL_MAIN.print(F("12) Vario deadband (x1000): ")); break;
    case 14: SERIAL_MAIN.print(F("14) Send PTAS: ")); break;
    case 16: SERIAL_MAIN.print(F("16) Send LXNAV: ")); break;
    case 17: SERIAL_MAIN.print(F("17) Accl manual offset x1000: ")); break;
    case 18: SERIAL_MAIN.print(F("18) Vario smoothness level: ")); break;
    case 20: SERIAL_MAIN.print(F("20) Enable Vario Audio: ")); break;
    case 21: SERIAL_MAIN.print(F("21) Vario Audio asc deadband x1000: ")); break;
    case 22: SERIAL_MAIN.print(F("22) Vario audio sink deadband x1000: ")); break;
    case 24: SERIAL_MAIN.print(F("24) Adaptive vario trigger time: ")); break;
    case 25: SERIAL_MAIN.print(F("25) Adaptive vario relax time: ")); break;
    case 26: SERIAL_MAIN.print(F("26) Adaptive vario minimum smooth level: ")); break;
    case 27: SERIAL_MAIN.print(F("27) Adaptive vario maximum smooth level: ")); break;


  }

}

void printtf() {
  SERIAL_MAIN.println();

}

void printhd() {
  SERIAL_MAIN.println();
  SERIAL_MAIN.println(F("XCSensors Config options:"));
  SERIAL_MAIN.println(F("200 - Stop Feed"));
  SERIAL_MAIN.println(F("201 - Start Feed"));
  SERIAL_MAIN.println(F("0 - Default values"));
  SERIAL_MAIN.println(F("101 - This menu"));
  SERIAL_MAIN.println(F("100 - Save to EEPROM"));
  SERIAL_MAIN.println(F("106 - Calibrate Accelerometer"));
  SERIAL_MAIN.println(F("Use 1 for on and 0 for off"));
  SERIAL_MAIN.println();
  SERIAL_MAIN.println(F("XCSensors Configuration:"));
  SERIAL_MAIN.println();
}

#else
void printaf(int n) {

  SERIAL_MAIN.print(F("<"));
  SERIAL_MAIN.print(n);
  SERIAL_MAIN.print(F("="));
}
void printtf() {
  SERIAL_MAIN.println(F(">"));
}

void printhd() {

}

#endif



void getConfigVars() { // order is not that important
  printhd();
  printaf(1);
  SERIAL_MAIN.print( getStringFromBool(conf.bluetoothOnly));
  printtf();
  printaf(2);
  SERIAL_MAIN.print(getStringFromBool(conf.wifiMultiPort));
  printtf();
  printaf(3);
  SERIAL_MAIN.print(conf.GPSChannel);
  printtf();
  printaf(4);
  SERIAL_MAIN.print(conf.varioChannel);
  printtf();
  printaf(5);
  SERIAL_MAIN.print(conf.humidChannel);
  printtf();
  printaf(6);
  SERIAL_MAIN.print(conf.magChannel);
  printtf();
  printaf(7);
  SERIAL_MAIN.print(conf.AcclChannel);
  printtf();
//  printaf(8);
//  SERIAL_MAIN.print(conf.ssid);
//  printtf();
//  printaf(9);
//  SERIAL_MAIN.print(conf.password);
 // printtf();
  printaf(10);
  SERIAL_MAIN.print(conf.magDeclination);
  printtf();
  printaf(11);
  SERIAL_MAIN.print(String(conf.qnePressure));
  printtf();
  printaf(12);
  SERIAL_MAIN.print(conf.varioDeadBand);
  printtf();
  printaf(14);
  SERIAL_MAIN.print(conf.ptas1);
  printtf();
  printaf(16);
  SERIAL_MAIN.print(conf.lxnav);
  printtf();
  printaf(17);
  SERIAL_MAIN.print(conf.accloffset);
  printtf();
  printaf(18);
  SERIAL_MAIN.print(conf.variosmooth);
  printtf();
  printaf(20);
  SERIAL_MAIN.print(conf.buzzer);
  printtf();
  printaf(21);
  SERIAL_MAIN.print(conf.varioAudioDeadBand);
  printtf();
  printaf(22);
  SERIAL_MAIN.print(conf.varioAudioSinkDeadBand);
  printtf();
  printaf(24);
  SERIAL_MAIN.print(conf.advTriggerTime);
  printtf();
  printaf(25);
  SERIAL_MAIN.print( conf.advRelaxTime);
  printtf();
  printaf(26);
  SERIAL_MAIN.print( conf.advMinSmooth);
  printtf();
  printaf(27);
  SERIAL_MAIN.print(conf.advMaxSmooth);
  printtf();
  
   if (conf.bluetoothOnly) {
    digitalWrite(WIFIEN_PIN, LOW);
  } else {
    digitalWrite(WIFIEN_PIN, HIGH);
  }

}


String getStringFromBool(bool bval) { //TODO: process Boolean values
  if (bval) {
    return "On";
  } else {
    return "Off";
  }
}

void getConfigFromEEPROM() {
 EEPROM.get(eeAddress, conf);
  
}

void saveConfigToEEPROM() {
  EEPROM.put(eeAddress, conf);

}

bool getBoolFromVal(char *sval) { //TODO: process Boolean values
  if (atoi(sval) == 1) {
    return true;
  } else {
    return false;
  }
}


void setConf(int varname, char *value) {

  switch (varname) {
    case 0: setDefaultConfig(); break;//load defaults
    case 1: conf.bluetoothOnly = getBoolFromVal(value); break;
    case 2: conf.wifiMultiPort = getBoolFromVal(value); break;
    case 3: conf.GPSChannel = atoi(value); break;
    case 4: conf.varioChannel = atoi(value); break;
    case 5: conf.humidChannel = atoi(value); break;
    case 6: conf.magChannel = atoi(value); break;
    case 7: conf.AcclChannel = atoi(value); break;
    //case 8: strcpy(conf.ssid, value); break;
    //case 9: strcpy(conf.password, value); break;
    case 10: conf.magDeclination = atoi(value); break;
    case 11: conf.qnePressure = atoi(value); break;
    case 12: conf.varioDeadBand = atoi(value); break;
    case 14: conf.ptas1 = getBoolFromVal(value); break;
    case 16: conf.lxnav = getBoolFromVal(value); break;
    case 17: conf.accloffset = atoi(value); break;
    case 18: conf.variosmooth = atoi(value); break;
    case 20: conf.buzzer = getBoolFromVal(value); break;
    case 21: conf.varioAudioDeadBand = atoi(value); break;
    case 22: conf.varioAudioSinkDeadBand = atoi(value); break;
    case 24: conf.advTriggerTime = atoi(value); break;
    case 25: conf.advRelaxTime = atoi(value); break;
    case 26: conf.advMinSmooth = atoi(value); break;
    case 27: conf.advMaxSmooth = atoi(value); break;
    case 100: saveConfigToEEPROM(); break; //save to eeprom
    case 106: resetACCLcompVal(); // quick set the ACCL to 0
    case 101: getConfigVars(); break; // get config for app
    case 200: runloop = false; break; //stop
    case 201: runloop = true; break; //start
    default:

      getConfigVars();
      break;
  }
  getConfigVars();
}



