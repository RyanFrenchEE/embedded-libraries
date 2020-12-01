#include <LTC6903.h>

const float frequency = 1000000.0;

/* Instantiate object with initial frequency */
LTC6903 freq_chip(frequency);


void setup()
{
  // Set up chip
}

void loop()
{
  /* While there are no errors in calculating OCT and DAC */
  while(!freq_chip.get_oct_error() && ! freq_chip.get_dac_error())
  {
    /* Gets float input */
    while(Serial.available())
    {
      /* Change frequency of chip */
      float freq = Serial.read();
      changeFrequency(freq);
    }
  }
}

void changeFrequency(float newFreq)
{
  freq_chip.set_frequency(newFreq);
}

