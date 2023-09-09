
#ifndef _Tone_Volume_h
#define _Tone_Volume_h

#include <stdint.h>



class Tone_Volume
{
  public:
    void setup(uint8_t tonePin);
    void play(uint16_t frequency, uint32_t duration = 300, int volume=1023);
    void stop();
    void loop();

  private:
    uint8_t Buzz_Pin;
    unsigned long tone_started;
    unsigned long tone_duration;
    bool is_playing;
};

#endif

