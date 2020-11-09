/***************************************************
* A library for the MCP23S17 16-bit I/O Expander
* with Serial interface.
*
* Each R/W register (except config) has 2 overloaded write functions:
*   - set(pin, state):    set given pin (or bit) to state
*   - set(states):        set pins at once, with states being the byte
*
* In addition, there are 2 overloaded read functions:
*   - read(pin):          read individual bit
*   - read():             read whole byte from register
*
* There are also functions to access both registers A & B,
* written as:
*   - sets(port, pin, state)
*   - sets(states):       states is a 2-byte integer
*   - reads(port, pin)
*   - reads()
*
* Created by: Ryan French
* Date: 04-04-2018
****************************************************/


#ifndef IO_EXPANDER_16_H
#define IO_EXPANDER_16_H

#include "Arduino.h"
#include <SPI.h>
#include <EEPROM.h>

class IO_Expander
{
public:

  IO_Expander(int cs_pin, int rst_pin, unsigned int a0, unsigned int a1, unsigned int a2, unsigned int eeprom_start, bool bankEnable);
  IO_Expander(int cs_pin, int rst_pin, unsigned int a0, unsigned int a1, unsigned int a3, bool bankEnable);
  // Reset the IO expander IC
  void resetChip();

  // Reset all registers to defaults
  void resetRegisters();

  // Save registers to EEPROM, starting at certain address
  void saveRegisters();

  // Initialize registers from EEPROM, given the starting
  // address in EEPROM
  void loadRegisters();

  // IO Direction Register A
  void resetDirectionA();
  void setDirectionA(unsigned int pin, unsigned int direction);
  void setDirectionA(unsigned int directions);
  unsigned int readDirectionA(unsigned int pin);
  unsigned int readDirectionA();

  // IO Direction Register B
  void resetDirectionB();
  void setDirectionB(unsigned int pin, unsigned int direction);
  void setDirectionB(unsigned int directions);
  unsigned int readDirectionB(unsigned int pin);
  unsigned int readDirectionB();

  // IO Direction Registers, combined
  void resetDirections();
  void setDirections(char port, unsigned int pin, unsigned int direction);
  void setDirections(unsigned int directions);
  unsigned int readDirections(char port, unsigned int pin);
  unsigned int readDirections();

  // IO Polarity Register A
  void resetPolarityA();
  void setPolarityA(unsigned int pin, unsigned int polarity);
  void setPolarityA(unsigned int polarities);
  unsigned int readPolarityA(unsigned int pin);
  unsigned int readPolarityA();

  // IO Polarity Register B
  void resetPolarityB();
  void setPolarityB(unsigned int pin, unsigned int polarity);
  void setPolarityB(unsigned int polarities);
  unsigned int readPolarityB(unsigned int pin);
  unsigned int readPolarityB();

  // IO Polarity Registers, combined
  void resetPolarities();
  void setPolarities(char port, unsigned int pin, unsigned int polarity);
  void setPolarities(unsigned int polarities);
  unsigned int readPolarities(char port, unsigned int pin);
  unsigned int readPolarities();

  // Interrupt-On-Change Register A
  void resetIOCA();
  void setIOCA(unsigned int pin, unsigned int state);
  void setIOCA(unsigned int states);
  unsigned int readIOCA(unsigned int pin);
  unsigned int readIOCA();

  // Interrupt-On-Change Register B
  void resetIOCB();
  void setIOCB(unsigned int pin, unsigned int state);
  void setIOCB(unsigned int states);
  unsigned int readIOCB(unsigned int pin);
  unsigned int readIOCB();

  // Interrupt-On-Change Registers, combined
  void resetIOCs();
  void setIOCs(char port, unsigned int pin, unsigned int state);
  void setIOCs(unsigned int states);
  unsigned int readIOCs(char port, unsigned int pin);
  unsigned int readIOCs();

  // Default Compare Register A
  void resetDefCompA();
  void setDefCompA(unsigned int pin, unsigned int level);
  void setDefCompA(unsigned int levels);
  unsigned int readDefCompA(unsigned int pin);
  unsigned int readDefCompA();

  // Default Compare Register B
  void resetDefCompB();
  void setDefCompB(unsigned int pin, unsigned int level);
  void setDefCompB(unsigned int levels);
  unsigned int readDefCompB(unsigned int pin);
  unsigned int readDefCompB();

  // Default Compare Registers, combined
  void resetDefComps();
  void setDefComps(char port, unsigned int pin, unsigned int level);
  void setDefComps(unsigned int levels);
  unsigned int readDefComps(char port, unsigned int pin);
  unsigned int readDefComps();

  // Interrupt Control Register A
  void resetIntContA();
  void setIntContA(unsigned int pin, unsigned int ref);
  void setIntContA(unsigned int refs);
  unsigned int readIntContA(unsigned int pin);
  unsigned int readIntContA();

  // Interrupt Control Register B
  void resetIntContB();
  void setIntContB(unsigned int pin, unsigned int ref);
  void setIntContB(unsigned int refs);
  unsigned int readIntContB(unsigned int pin);
  unsigned int readIntContB();

  // Interrupt Control Registers, combined
  void resetIntConts();
  void setIntConts(char port, unsigned int pin, unsigned int ref);
  void setIntConts(unsigned int refs);
  unsigned int readIntConts(char port, unsigned int pin);
  unsigned int readIntConts();

  // Configuration Registers
  void resetConfig();
  void setConfig(unsigned int bits);
  unsigned int readConfig();

  // Pull-up Resistor Register A
  void resetPullupA();
  void setPullupA(unsigned int pin, unsigned int enable);
  void setPullupA(unsigned int enables);
  unsigned int readPullupA(unsigned int pin);
  unsigned int readPullupA();

  // Pull-up Resistor Register B
  void resetPullupB();
  void setPullupB(unsigned int pin, unsigned int enable);
  void setPullupB(unsigned int enables);
  unsigned int readPullupB(unsigned int pin);
  unsigned int readPullupB();

  // Pull-up Resistor Registers, combined
  void resetPullups();
  void setPullups(char port, unsigned int pin, unsigned int enable);
  void setPullups(unsigned int enables);
  unsigned int readPullups(char port, unsigned int pin);
  unsigned int readPullups();

  // Interrupt Flag Register A (Read-Only)
  unsigned int readINTFA(unsigned int pin);
  unsigned int readINTFA();

  // Interrupt Flag Register B (Read-Only)
  unsigned int readINTFB(unsigned int pin);
  unsigned int readINTFB();

  // Interrupt Flag Registers, combined (Read-Only)
  unsigned int readINTFs(char port, unsigned int pin);
  unsigned int readINTFs();

  // Interrupt Capture Register A (Read-Only)
  unsigned int readINTCAPA(unsigned int pin);
  unsigned int readINTCAPA();

  // Interrupt Capture Register B (Read-Only)
  unsigned int readINTCAPB(unsigned int pin);
  unsigned int readINTCAPB();

  // Interrupt Capture Registers (Read-Only)
  unsigned int readINTCAPs(char port, unsigned int pin);
  unsigned int readINTCAPs();

  // GPIO Register A
  void resetIOA();
  void setIOA(unsigned int pin, unsigned int logic);
  void setIOA(unsigned int logics);
  unsigned int readIOA(unsigned int pin);
  unsigned int readIOA();

  // GPIO Register B
  void resetIOB();
  void setIOB(unsigned int pin, unsigned int logic);
  void setIOB(unsigned int logics);
  unsigned int readIOB(unsigned int pin);
  unsigned int readIOB();

  // GPIO Registers, combined
  void resetIOs();
  void setIOs(char port, unsigned int pin, unsigned int logic);
  void setIOs(unsigned int logics);
  unsigned int readIOs(char port, unsigned int pin);
  unsigned int readIOs();

  // Output Latch Register A
  void resetLatchA();
  void setLatchA(unsigned int pin, unsigned int logic);
  void setLatchA(unsigned int logics);
  unsigned int readLatchA(unsigned int pin);
  unsigned int readLatchA();

  // Output Latch Register B
  void resetLatchB();
  void setLatchB(unsigned int pin, unsigned int logic);
  void setLatchB(unsigned int logics);
  unsigned int readLatchB(unsigned int pin);
  unsigned int readLatchB();

  // Output Latch Registers, combined
  void resetLatches();
  void setLatches(char port, unsigned int pin, unsigned int logic);
  void setLatches(unsigned int logics);
  unsigned int readLatches(char port, unsigned int pin);
  unsigned int readLatches();


private:
  byte WRITE;
  byte WRITE_INIT;
  byte READ;
  byte READ_INIT;

  unsigned int _EEPROM_START;
  bool BANK;

  // Registers:
  byte IODIRA;
  byte IODIRB;
  byte IPOLA;
  byte IPOLB;
  byte GPINTENA ;
  byte GPINTENB;
  byte DEFVALA;
  byte DEFVALB;
  byte INTCONA;
  byte INTCONB;
  byte IOCONA;
  byte IOCONB;
  byte GPPUA;
  byte GPPUB;
  byte INTFA;
  byte INTFB;
  byte INTCAPA;
  byte INTCAPB;
  byte GPIOA;
  byte GPIOB;
  byte OLATA;
  byte OLATB;


  int _CS;
  int _RST;
  unsigned int _A0 = 0;
  unsigned int _A1 = 0;
  unsigned int _A2 = 0;

  void send(byte addr, unsigned int bytes);
  void send16(byte addr1, byte addr2, unsigned int bytes);
  void setBit(byte addr, unsigned int pin, unsigned int logic);
  unsigned int receive(byte addr);
  unsigned int readBit(byte addr, unsigned int pin);

  void setAddress();
  void determineAddresses();

};

#endif // IO_EXPANDER_H
