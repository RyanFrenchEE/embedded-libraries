/***************************************************************
 * LTC6903.h - A class that controls behavior of the LTC6903
 * 1kHz-68MHz Serial Port Programmable Oscillator by
 * Linear Technology.
 *
 * Created by Ryan French 02-26-2018
 *
 * This library is completely open-source and is subject to
 * no copyrights.
****************************************************************/

#ifndef LTC6903_H_INCLUDED
#define LTC6903_H_INCLUDED

#include <math.h>
#include "Arduino.h"
#include <SPI.h>

class LTC6903
{
public:
  LTC6903(const int chipSelect, double init_frequency);     // Constructor

  void set_chip();            // Sets the chip given the current frequency
  void set_chip(int dac, int oct);
  void set_frequency(double new_frequency);  // Sets the frequency
  double get_frequency();      // Gets the current frequency
  int get_OCT();              // Gets the OCT constant
  void set_OCT(int new_oct);
  int get_DAC();              // Gets the DAC constant
  void set_DAC(int new_dac);
  int get_oct_error();        // Gets the error in calculating OCT
  int get_dac_error();        // Gets the error in calculating DAC

private:
  int _CS;                    // Chip select pin
  int oct_error, dac_error;   // Stores error state. 0 = False, 1 = True
  double frequency;            // Current frequency
  int _OCT, _DAC;               // Constants used to send info to chip
  void calculate_OCT();        // Calculates OCT given the frequency
  void calculate_DAC();        // Calculates DAC given OCT and frequency
  int byte1, byte2;           // The two bytes to be sent to the chip
  const int config_bits = 2;  // Config bits. Sets CLK = ON and /CLK = OFF

};

#endif
