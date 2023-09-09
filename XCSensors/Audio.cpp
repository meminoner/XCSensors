/*
  XCSensors http://XCSensors.org

  Copyright (c), PrimalCode (http://www.primalcode.org)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  any later version. see <http://www.gnu.org/licenses/>
*/

//Audio related stuff

#include <Arduino.h>
#include <HardwareTimer.h>
#include "Audio.h"
#include "config.h"
#include "Conf.h"
#include "XCSensors.h"


#if defined(BUZZER)
bool climbing = false;
int tm;
int rm;
int stime;
byte toneOn = false;
byte muted = false;
float variof;
int pause=0;
byte tcount=0;

#endif

#if defined(TESTBUZZER)
unsigned long btime = 0;
float testvario = -6;
#endif


#if defined(BUZZER)

#define BASEPULSE 200
#define TOPPULSE  1000

// TONE VOLUME ----------------------------------------

unsigned long tone_started;
unsigned long tone_duration;
bool is_playing;

#define BUZZPIN PB0  //board pin

#define PinTimer(pin) (PIN_MAP[pin].timer_device->clk_id - RCC_TIMER1 + 1)
#define PinChannel(pin) (PIN_MAP[pin].timer_channel)

uint8_t tone_channel = 3;            // timer channel used to generate frequency
uint8_t tone_ntimer = 3;             // timer used to generate frequency

HardwareTimer TTimer1(1), TTimer2(2), TTimer3(3), TTimer4(4);
HardwareTimer *TTimer[4] =  { &TTimer1,&TTimer2,&TTimer3,&TTimer4 };
HardwareTimer *tone_timer;

void Tone_Loop();

void Tone_Setup(){
    tone_channel = PinChannel(BUZZPIN);
    tone_ntimer = PinTimer(BUZZPIN);
    tone_timer = TTimer[tone_ntimer-1];
    tone_timer->attachCompare1Interrupt(Tone_Loop);
    is_playing = false;
}


void Tone_Play(uint16_t frequency, uint32_t duration){

  int volume_val;

  switch (conf.speakerVolume){
  case 0:
    volume_val = 256;
    break;
  case 1:
    volume_val = 256;
    break;
  case 2:
    volume_val = 36;
    break;
  case 3:
    volume_val = 2;
    break;
  default:
    volume_val = 256;
    break;
  }
    
    pinMode(BUZZPIN, PWM);

    int overflow = 256*1100/frequency;
    int compare = overflow/volume_val;

    tone_timer->pause();
    tone_timer->setPrescaleFactor(256);              // 256
    tone_timer->setOverflow(overflow);               // 256*1100/frequency
    tone_timer->setCompare(tone_channel, compare);   // duty cycle
    tone_timer->refresh();
    tone_timer->resume();
    
    tone_started = millis();
    tone_duration = duration;
    is_playing = true;
}

void Tone_Stop(){
    tone_timer->pause();
    pinMode(BUZZPIN, INPUT);
    is_playing = false;
}


void Tone_Loop(){
    if( is_playing && millis() > tone_started + tone_duration){
      Tone_Stop();
    }
}


// --------------------------------------------------------



// Non-Blocking beep blob beep
// void playTwoToneInterval(int freq,int freq2, int period, int intervald) {
 
  
//   if (toneOn) {
//     int wait = period + tm;
    
    
//     if ( wait < millis()) {
//       toneOn = false;
//       noTone(BUZZPIN);
//       tone(BUZZPIN, freq2, intervald);
//       rm = millis();
//     }

//   } else {
//     int ndwait = intervald + rm;
     
//     if(ndwait < millis()) {
    
//     tone(BUZZPIN, freq, period);
//     toneOn = true;
//     tm = millis();
//     }
//   }

// }


// Non-Blocking beep beep beep
void playToneInterval(int freq, int period, int tinterval) {

  if (toneOn) {
    int wait = period + tinterval + tm;

    if ( wait < millis()) {
      toneOn = false;
      // noTone(BUZZPIN);
      Tone_Stop();
      tcount++;            // count the amount of beeps for playTonePause
      if (tcount > 1000) { // prevent overflow
        tcount = 0;
      }
    }

  } else {
    // tone(BUZZPIN, freq, period);
    Tone_Play(freq, period);
    toneOn = true;
    tm = millis();
  }

}

void playToneContinuous(int freq) {

  int period = 50;

  if (toneOn) {
    int wait = period + tm;
    if ( wait <= millis()) {
      toneOn = false;
      // tone(BUZZPIN, freq, 300);
      Tone_Play(freq, 300);
    }

  } else {
    // tone(BUZZPIN, freq, 300);
    Tone_Play(freq, 300);
    toneOn = true;
    tm = millis();
  }

}


// nth beeps then pauze x

void playTonePause(int freq, int nbeeps, int tpause) {

   if (pause < millis()) {
      
      if (tcount < nbeeps) {
        playToneInterval(freq, 500, 200);
        
      }else{
        pause = millis() + tpause;
        tcount=0;

      }
      
    
   }

  
}


void makeVarioAudio(float vario) {
  int pulse;
  float varioorg = vario;
 
#if defined(TESTBUZZER)
  vario = testvario;
  int tpassed = millis() - btime;
  if (tpassed > 1000) {
    testvario = testvario + 0.2;
    btime = millis();
  }

  if (testvario > 6 ) {
    testvario = -3;
  }

  
#endif

  if (vario > 9) {
    vario = 9;

  }

#if defined(SOARDETECTION) && !defined(TESTBUZZER)

  if (varioorg > -0.2 && varioorg < 0.2) {
    int diffe = millis() - stime;
    if (diffe >  int(conf.SoarDeadBandTime)) {
      muted = true;
    }
  } else {
    stime = millis();
    muted = false;
  }

#endif

    // if (vario <= 0 && vario >= BUZZERZEROCLIMB) {
    //   if (!muted) {
    //      playToneInterval(variof, 50, 400);
    //   }

    // }
    
  //  if (vario <= double(conf.sinkAlarmLevel)/1000 ) { //sink alarm
  //     if (!muted) {
  //        playTwoToneInterval(1400, 1800, 100, 100);
  //     }
  //  }

#if defined(BUZZSINKALERT) 
    if (vario <=  BUZZSINKALERT /* && vario >= double(conf.sinkAlarmLevel)/1000 */ ) {
      // playTonePause(300, abs(vario), BUZZSINKALERTPAUSE);
      
      // ***************

      float vario_fix = vario < -6 ? -6 : vario;
      vario_fix += 6;

      float variofa = (vario_fix * 20 ) + 150;
      playToneContinuous(variofa);
      
      // ***************

    }
  
#endif
   

 // float variofa = (float(fabs(vario)) * 200 ) + 800;
 float variofa = (float(fabs(vario)) * 200 ) + 900;
  variof = (10 * variof + variofa) / 11 ;

  if (vario > 0) {
    pulse = TOPPULSE / (vario * 10) + 200;
    if (!muted) {
      playToneInterval(variof, pulse, pulse / 2);
    }
    climbing = true;
  } else {
    if (climbing ) { //dropped out of the thermal
      // tone(BUZZPIN, 100, OUTOFTHERMALBUZZT);
      climbing = false;
    }
  }

}


#endif


