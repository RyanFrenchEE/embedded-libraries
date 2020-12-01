/***************************************************************
 * LTC6903.cpp - A class that controls behavior of the LTC6903
 * 1kHz-68MHz Serial Port Programmable Oscillator by
 * Linear Technology.
 *
 * Created by Ryan French 02-26-2018
 *
 * This library is completely open-source and is subject to
 * no copyrights.
****************************************************************/

#include "LTC6903.h"

LTC6903::LTC6903(const int chipSelect, double init_frequency)
{
  /* Constructor. Sets the chip to an initial frequency. */
  _CS = chipSelect;
  set_frequency(init_frequency);
  set_chip();
}

void LTC6903::calculate_OCT()
{
  /* Calculates _OCT based on Table 1 (page 8) of the LTC6903
     specifications. */

  if(frequency < 68030000.0 && frequency >= 34050000.0)
  {
    _OCT = 15;
  }
  else if(frequency < 34010000.0 && frequency >= 17020000.0)
  {
    _OCT = 14;
  }
  else if(frequency < 17010000.0 && frequency >= 8511000.0)
  {
    _OCT = 13;
  }
  else if(frequency < 8503000.0 && frequency >= 4256000.0)
  {
    _OCT = 12;
  }
  else if(frequency < 4252000.0 && frequency >= 2128000.0)
  {
    _OCT = 11;
  }
  else if(frequency < 2126000.0 && frequency >= 1064000.0)
  {
    _OCT = 10;
  }
  else if(frequency < 1063000.0 && frequency >= 532000.0)
  {
    _OCT = 9;
  }
  else if(frequency < 531400.0 && frequency >= 266000.0)
  {
    _OCT = 8;
  }
  else if(frequency < 265700.0 && frequency >= 133000.0)
  {
    _OCT = 7;
  }
  else if(frequency < 132900.0 && frequency >= 66500.0)
  {
    _OCT = 6;
  }
  else if(frequency < 66430.0 && frequency >= 33250.0)
  {
    _OCT = 5;
  }
  else if(frequency < 33220.0 && frequency >= 16620.0)
  {
    _OCT = 4;
  }
  else if(frequency < 16610.0 && frequency >= 8312.0)
  {
    _OCT = 3;
  }
  else if(frequency < 8304.0 && frequency >= 4156.0)
  {
    _OCT = 2;
  }
  else if(frequency < 4152.0 && frequency >= 2078.0)
  {
    _OCT = 1;
  }
  else if(frequency < 2076.0 && frequency >= 1039.0)
  {
    _OCT = 0;
  }
  else
  {
    /* If there is an error, change _OCT_error to 1 */
    oct_error = 1;
  }
  oct_error = 0;
}

void LTC6903::calculate_DAC()
{
  /* Calculates _DAC as shown on page 8 of the specification */

  int exponent = 10 + _OCT;
  double base_freq = 2078.0;
  double two_exponent = (double) pow(2, exponent);

  double top = (two_exponent * base_freq);
  double right = top / frequency;

  double dac_double = round(2048.0 - right);

  _DAC = (int) dac_double;

  /* Check to see if _DAC is in-bounds. If not, set it to
     the edge case and flag the DAC_error */

  if(_DAC < 0)
  {
    _DAC = 0;
    dac_error = 1;
  }
  else if(_DAC > 1023)
  {
    _DAC = 1023;
    dac_error = 1;
  }
  dac_error = 0;
}

void LTC6903::set_chip()
{
  /* Calculates both _OCT and _DAC, then puts that information
     into 2 bytes, which is sent to the chip by SPI. */

  calculate_OCT();
  calculate_DAC();
  byte1 = _OCT << 4;
  byte1 |= (_DAC >> 6);

  byte2 = ((_DAC << 2) & 0xFF);
  byte2 = byte2 | config_bits;

  unsigned long wordToSend = (byte1 << 8) | (byte2);

  SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV32, MSBFIRST, SPI_MODE0));
  digitalWrite(_CS, LOW);
  delayMicroseconds(20);
  SPI.transfer16(wordToSend);
  delayMicroseconds(20);
  digitalWrite(_CS, HIGH);
  SPI.endTransaction();
}

void LTC6903::set_chip(int dac, int oct)
{
  /* Calculates both _OCT and _DAC, then puts that information
     into 2 bytes, which is sent to the chip by SPI. */

  _DAC = dac;
  _OCT = oct;
  byte1 = _OCT << 4;
  byte1 |= (_DAC >> 6);

  byte2 = ((_DAC << 2) & 0xFF);
  byte2 = byte2 | config_bits;

  unsigned long wordToSend = (byte1 << 8) | (byte2);

  SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV32, MSBFIRST, SPI_MODE0));
  digitalWrite(_CS, LOW);
  delayMicroseconds(20);
  SPI.transfer16(wordToSend);
  delayMicroseconds(20);
  digitalWrite(_CS, HIGH);
  SPI.endTransaction();
}

void LTC6903::set_frequency(double new_frequency)
{
  /* Sets the chip to output a new frequency */

  frequency = new_frequency;
  set_chip();
}

/**** Getter functions for private variables ****/
double LTC6903::get_frequency()
{
  return frequency;
}

int LTC6903::get_OCT()
{
  return _OCT;
}

void LTC6903::set_OCT(int new_oct)
{
  _OCT = new_oct;
}

int LTC6903::get_DAC()
{
  return _DAC;
}

void LTC6903::set_DAC(int new_dac)
{
  _DAC = new_dac;
}

int LTC6903::get_oct_error()
{
  return oct_error;
}

int LTC6903::get_dac_error()
{
  return dac_error;
}
