
#include "Tone_Volume.h"

#include "Arduino.h"
#include <HardwareTimer.h>

#define BUZZPIN PB0  //board pin

#define PinTimer(pin) (PIN_MAP[pin].timer_device->clk_id - RCC_TIMER1 + 1)
#define PinChannel(pin) (PIN_MAP[pin].timer_channel)

uint8_t tone_channel = 3;            // timer channel used to generate frequency
uint8_t tone_ntimer = 3;             // timer used to generate frequency

HardwareTimer TTimer1(1), TTimer2(2), TTimer3(3), TTimer4(4);
HardwareTimer *TTimer[4] =  { &TTimer1,&TTimer2,&TTimer3,&TTimer4 };
HardwareTimer *tone_timer;


void Tone_Volume::setup(uint8_t tonePin){

    Buzz_Pin = tonePin;

    tone_channel = PinChannel(Buzz_Pin);
    tone_ntimer = PinTimer(Buzz_Pin);
    tone_timer = TTimer[tone_ntimer-1];

    is_playing = false;
}


void Tone_Volume::play(uint16_t frequency, uint32_t duration, int volume){ // volume 1023 max
    
    pinMode(Buzz_Pin, PWM);
    
    int overflow = 72000000 / frequency;
    int compare = ( volume * overflow * 0.5f ) / 1024;

    tone_timer->pause();
    tone_timer->setPrescaleFactor(1);                 // 1
    tone_timer->setOverflow(overflow);               // 72MHZ / freq = overflow
    tone_timer->setCompare(tone_channel, compare);   // 50% duty cycle on volume == 1023
    tone_timer->refresh();
    tone_timer->resume();

    tone_started = millis();
    tone_duration = duration;
    is_playing = true;
}

void Tone_Volume::stop(){
    tone_timer->pause();
    pinMode(Buzz_Pin, INPUT);
    is_playing = false;
}

void Tone_Volume::loop(){
    if( is_playing && millis() > tone_started + tone_duration){
        stop();
    }
}



