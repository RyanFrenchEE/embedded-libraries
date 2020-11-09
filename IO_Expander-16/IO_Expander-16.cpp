#include "IO_Expander-16.h"

IO_Expander::IO_Expander(int cs_pin, int rst_pin, unsigned int a0, unsigned int a1, unsigned int a2, unsigned int eeprom_start, bool bankEnable = false)
{
  /* Instantiate IO_Expander object */
  _CS = cs_pin;
  _RST = rst_pin;
  _A0 = a0;
  _A1 = a1;
  _A2 = a2;
  _EEPROM_START = eeprom_start;
  BANK = bankEnable;
  determineAddresses();

  pinMode(_CS, OUTPUT);
  pinMode(_RST, OUTPUT);
  digitalWrite(_CS, HIGH);
  digitalWrite(_RST, HIGH);

  WRITE = 64;

  if(_A2 == 1)
  {
    WRITE += 8;
  }
  if(_A1 == 1)
  {
    WRITE += 4;
  }

  if(_A0 == 1)
  {
    WRITE += 2;
  }

  READ = WRITE + 1;
  WRITE_INIT = WRITE;
  READ_INIT = READ;
}

IO_Expander::IO_Expander(int cs_pin, int rst_pin, unsigned int a0, unsigned int a1, unsigned int a2, bool bankEnable = false)
{
  /* Instantiate IO_Expander object */
  _CS = cs_pin;
  _RST = rst_pin;
  _A0 = a0;
  _A1 = a1;
  _A2 = a2;
  BANK = bankEnable;
  determineAddresses();

  pinMode(_CS, OUTPUT);
  pinMode(_RST, OUTPUT);
  digitalWrite(_CS, HIGH);
  digitalWrite(_RST, HIGH);

  WRITE = 64;

  if(_A2 == 1)
  {
    WRITE += 8;
  }
  if(_A1 == 1)
  {
    WRITE += 4;
  }

  if(_A0 == 1)
  {
    WRITE += 2;
  }

  READ = WRITE + 1;
  WRITE_INIT = WRITE;
  READ_INIT = READ;
}

void IO_Expander::resetChip()
{
  digitalWrite(_RST, LOW);
  delay(1000);
  digitalWrite(_RST, HIGH);
  BANK = false;
  setAddress();
  determineAddresses();
}

void IO_Expander::resetRegisters()
{
  /* Resets all registers to defaults */
  resetDirections();
  resetPolarities();
  resetIOCs();
  resetDefComps();
  resetIntConts();
  resetConfig();
  resetPullups();
  resetIOs();
  resetLatches();

  setAddress();
  BANK = false;
  determineAddresses();
}

void IO_Expander::saveRegisters()
{
  /* Save the registers to EEPROM (17 bytes)*/
  unsigned int dirA = readDirectionA();
  unsigned int dirB = readDirectionB();
  unsigned int polA = readPolarityA();
  unsigned int polB = readPolarityB();
  unsigned int iocA = readIOCA();
  unsigned int iocB = readIOCB();
  unsigned int def_compA = readDefCompA();
  unsigned int def_compB = readDefCompB();
  unsigned int int_contA = readIntContA();
  unsigned int int_contB = readIntContB();
  unsigned int config = readConfig();
  unsigned int pu_resA = readPullupA();
  unsigned int pu_resB = readPullupB();
  unsigned int gpioA = readIOA();
  unsigned int gpioB = readIOB();
  unsigned int latchA = readLatchA();
  unsigned int latchB = readLatchB();

  EEPROM.write(_EEPROM_START, dirA);
  EEPROM.write((_EEPROM_START+1), dirB);
  EEPROM.write((_EEPROM_START+2), polA);
  EEPROM.write((_EEPROM_START+3), polB);
  EEPROM.write((_EEPROM_START+4), iocA);
  EEPROM.write((_EEPROM_START+5), iocB);
  EEPROM.write((_EEPROM_START+6), def_compA);
  EEPROM.write((_EEPROM_START+7), def_compB);
  EEPROM.write((_EEPROM_START+8), int_contA);
  EEPROM.write((_EEPROM_START+9), int_contB);
  EEPROM.write((_EEPROM_START+10), config);
  EEPROM.write((_EEPROM_START+11), pu_resA);
  EEPROM.write((_EEPROM_START+12), pu_resB);
  EEPROM.write((_EEPROM_START+13), gpioA);
  EEPROM.write((_EEPROM_START+14), gpioB);
  EEPROM.write((_EEPROM_START+15), latchA);
  EEPROM.write((_EEPROM_START+16), latchB);
}

void IO_Expander::loadRegisters()
{
  /* Write values to registers from EEPROM */
  unsigned int dirA = (unsigned int)EEPROM.read(_EEPROM_START);
  unsigned int dirB = (unsigned int)EEPROM.read(_EEPROM_START + 1);
  unsigned int polA = (unsigned int)EEPROM.read(_EEPROM_START + 2);
  unsigned int polB = (unsigned int)EEPROM.read(_EEPROM_START + 3);
  unsigned int iocA = (unsigned int)EEPROM.read(_EEPROM_START + 4);
  unsigned int iocB = (unsigned int)EEPROM.read(_EEPROM_START + 5);
  unsigned int def_compA = (unsigned int)EEPROM.read(_EEPROM_START + 6);
  unsigned int def_compB = (unsigned int)EEPROM.read(_EEPROM_START + 7);
  unsigned int int_contA = (unsigned int)EEPROM.read(_EEPROM_START + 8);
  unsigned int int_contB = (unsigned int)EEPROM.read(_EEPROM_START + 9);
  unsigned int configA = (unsigned int)EEPROM.read(_EEPROM_START + 10);
  unsigned int pu_resA = (unsigned int)EEPROM.read(_EEPROM_START + 11);
  unsigned int pu_resB = (unsigned int)EEPROM.read(_EEPROM_START + 12);
  unsigned int gpioA = (unsigned int)EEPROM.read(_EEPROM_START + 13);
  unsigned int gpioB = (unsigned int)EEPROM.read(_EEPROM_START + 14);
  unsigned int latchA = (unsigned int)EEPROM.read(_EEPROM_START + 15);
  unsigned int latchB = (unsigned int)EEPROM.read(_EEPROM_START + 16);

  unsigned int dir = (dirA << 8) | dirB;
  unsigned int pol = (polA << 8) | polB;
  unsigned int ioc = (iocA << 8) | iocB;
  unsigned int def_comp = (def_compA << 8) | def_compB;
  unsigned int int_cont = (int_contA << 8) | int_contB;
  unsigned int config = (configA << 8) | configA;
  unsigned int pu_res = (pu_resA << 8) | pu_resB;
  unsigned int gpio = (gpioA << 8) | gpioB;
  unsigned int latch = (latchA << 8) | latchB;

  SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV32, MSBFIRST, SPI_MODE0));
  digitalWrite(_CS, LOW);

  setConfig(config);
  setDirections(dir);
  setPolarities(pol);
  setIOCs(ioc);
  setDefComps(def_comp);
  setIntConts(int_cont);
  setPullups(pu_res);
  setIOs(gpio);
  setLatches(latch);

  digitalWrite(_CS, HIGH);
  SPI.endTransaction();
}

/*--------------------------------------------------------------------------*/
/*--------------------------------DIRECTION---------------------------------*/
/*--------------------------------------------------------------------------*/
void IO_Expander::resetDirectionA()
{
  send(IODIRA, 0xFF);
}

void IO_Expander::setDirectionA(unsigned int pin, unsigned int direction)
{
  setBit(IODIRA, pin, direction);
}

void IO_Expander::setDirectionA(unsigned int directions)
{
  send(IODIRA, directions);
}

unsigned int IO_Expander::readDirectionA(unsigned int pin)
{
  readBit(IODIRA, pin);
}

unsigned int IO_Expander::readDirectionA()
{
  receive(IODIRA);
}

void IO_Expander::resetDirectionB()
{
  send(IODIRB, 0xFF);
}

void IO_Expander::setDirectionB(unsigned int pin, unsigned int direction)
{
  setBit(IODIRB, pin, direction);
}

void IO_Expander::setDirectionB(unsigned int directions)
{
  send(IODIRB, directions);
}

unsigned int IO_Expander::readDirectionB(unsigned int pin)
{
  readBit(IODIRB, pin);
}

unsigned int IO_Expander::readDirectionB()
{
  receive(IODIRB);
}

void IO_Expander::resetDirections()
{
  send16(IODIRA, IODIRB, 0xFFFF);
}

void IO_Expander::setDirections(unsigned int directions)
{
  send16(IODIRA, IODIRB, directions);
}

void IO_Expander::setDirections(char port, unsigned int pin, unsigned int direction)
{
  if (port == 'A' || port == 'a')
  {
  setBit(IODIRA, pin, direction);
  }
  else if (port == 'B' || port == 'b')
  {
    setBit(IODIRB, pin, direction);
  }
}

unsigned int IO_Expander::readDirections(char port, unsigned int pin)
{
  if (port == 'A' || port == 'a')
  {
    readBit(IODIRA, pin);
  }
  else if (port == 'B' || port == 'b')
  {
    readBit(IODIRB, pin);
  }
}

unsigned int IO_Expander::readDirections()
{
  unsigned int byte1 = receive(IODIRA);
  unsigned int byte2 = receive(IODIRB);

  return ((byte1 << 8) | byte2);
}

/*--------------------------------------------------------------------------*/
/*---------------------------------POLARITY---------------------------------*/
/*--------------------------------------------------------------------------*/
void IO_Expander::resetPolarityA()
{
  send(IPOLA, 0x00);
}

void IO_Expander::setPolarityA(unsigned int pin, unsigned int polarity)
{
  setBit(IPOLA, pin, polarity);
}

void IO_Expander::setPolarityA(unsigned int polarities)
{
  send(IPOLA, polarities);
}

unsigned int IO_Expander::readPolarityA(unsigned int pin)
{
  readBit(IPOLA, pin);
}

unsigned int IO_Expander::readPolarityA()
{
  receive(IPOLA);
}

void IO_Expander::resetPolarityB()
{
  send(IPOLB, 0x00);
}

void IO_Expander::setPolarityB(unsigned int pin, unsigned int polarity)
{
  setBit(IPOLB, pin, polarity);
}

void IO_Expander::setPolarityB(unsigned int polarities)
{
  send(IPOLB, polarities);
}

unsigned int IO_Expander::readPolarityB(unsigned int pin)
{
  readBit(IPOLB, pin);
}

unsigned int IO_Expander::readPolarityB()
{
  receive(IPOLB);
}

void IO_Expander::resetPolarities()
{
  send16(IPOLA, IPOLB, 0x0000);
}

void IO_Expander::setPolarities(unsigned int polarities)
{
  send16(IPOLA, IPOLB, polarities);
}

void IO_Expander::setPolarities(char port, unsigned int pin, unsigned int polarity)
{
  if (port == 'A' || port == 'a')
  {
  setBit(IPOLA, pin, polarity);
  }
  else if (port == 'B' || port == 'b')
  {
    setBit(IPOLB, pin, polarity);
  }
}

unsigned int IO_Expander::readPolarities(char port, unsigned int pin)
{
  if (port == 'A' || port == 'a')
  {
    readBit(IPOLA, pin);
  }
  else if (port == 'B' || port == 'b')
  {
    readBit(IPOLB, pin);
  }
}

unsigned int IO_Expander::readPolarities()
{
  unsigned int byte1 = receive(IPOLA);
  unsigned int byte2 = receive(IPOLB);

  return ((byte1 << 8) | byte2);
}

/*--------------------------------------------------------------------------*/
/*-----------------------------------IOC------------------------------------*/
/*--------------------------------------------------------------------------*/
void IO_Expander::resetIOCA()
{
  send(GPINTENA, 0x00);
}

void IO_Expander::setIOCA(unsigned int pin, unsigned int state)
{
  setBit(GPINTENA, pin, state);
}

void IO_Expander::setIOCA(unsigned int states)
{
  send(GPINTENA, states);
}

unsigned int IO_Expander::readIOCA(unsigned int pin)
{
  readBit(GPINTENA, pin);
}

unsigned int IO_Expander::readIOCA()
{
  receive(GPINTENA);
}

void IO_Expander::resetIOCB()
{
  send(GPINTENB, 0x00);
}

void IO_Expander::setIOCB(unsigned int pin, unsigned int state)
{
  setBit(GPINTENB, pin, state);
}

void IO_Expander::setIOCB(unsigned int states)
{
  send(GPINTENB, states);
}

unsigned int IO_Expander::readIOCB(unsigned int pin)
{
  readBit(GPINTENB, pin);
}

unsigned int IO_Expander::readIOCB()
{
  receive(GPINTENB);
}

void IO_Expander::resetIOCs()
{
  send16(GPINTENA, GPINTENB, 0x0000);
}

void IO_Expander::setIOCs(unsigned int states)
{
  send16(GPINTENA, GPINTENB, states);
}

void IO_Expander::setIOCs(char port, unsigned int pin, unsigned int state)
{
  if (port == 'A' || port == 'a')
  {
  setBit(GPINTENA, pin, state);
  }
  else if (port == 'B' || port == 'b')
  {
    setBit(GPINTENB, pin, state);
  }
}

unsigned int IO_Expander::readIOCs(char port, unsigned int pin)
{
  if (port == 'A' || port == 'a')
  {
    readBit(GPINTENA, pin);
  }
  else if (port == 'B' || port == 'b')
  {
    readBit(GPINTENB, pin);
  }
}

unsigned int IO_Expander::readIOCs()
{
  unsigned int byte1 = receive(GPINTENA);
  unsigned int byte2 = receive(GPINTENB);

  return ((byte1 << 8) | byte2);
}

/*--------------------------------------------------------------------------*/
/*---------------------------------DEFCOMP----------------------------------*/
/*--------------------------------------------------------------------------*/
void IO_Expander::resetDefCompA()
{
  send(DEFVALA, 0x00);
}

void IO_Expander::setDefCompA(unsigned int pin, unsigned int level)
{
  setBit(DEFVALA, pin, level);
}

void IO_Expander::setDefCompA(unsigned int levels)
{
  send(DEFVALA, levels);
}

unsigned int IO_Expander::readDefCompA(unsigned int pin)
{
  readBit(DEFVALA, pin);
}

unsigned int IO_Expander::readDefCompA()
{
  receive(DEFVALA);
}

void IO_Expander::resetDefCompB()
{
  send(DEFVALB, 0x00);
}

void IO_Expander::setDefCompB(unsigned int pin, unsigned int level)
{
  setBit(DEFVALB, pin, level);
}

void IO_Expander::setDefCompB(unsigned int levels)
{
  send(DEFVALB, levels);
}

unsigned int IO_Expander::readDefCompB(unsigned int pin)
{
  readBit(DEFVALB, pin);
}

unsigned int IO_Expander::readDefCompB()
{
  receive(DEFVALB);
}

void IO_Expander::resetDefComps()
{
  send16(DEFVALA, DEFVALB, 0x0000);
}

void IO_Expander::setDefComps(unsigned int levels)
{
  send16(DEFVALA, DEFVALB, levels);
}

void IO_Expander::setDefComps(char port, unsigned int pin, unsigned int level)
{
  if (port == 'A' || port == 'a')
  {
  setBit(DEFVALA, pin, level);
  }
  else if (port == 'B' || port == 'b')
  {
    setBit(DEFVALB, pin, level);
  }
}

unsigned int IO_Expander::readDefComps(char port, unsigned int pin)
{
  if (port == 'A' || port == 'a')
  {
    readBit(DEFVALA, pin);
  }
  else if (port == 'B' || port == 'b')
  {
    readBit(DEFVALB, pin);
  }
}

unsigned int IO_Expander::readDefComps()
{
  unsigned int byte1 = receive(DEFVALA);
  unsigned int byte2 = receive(DEFVALB);

  return ((byte1 << 8) | byte2);
}

/*--------------------------------------------------------------------------*/
/*----------------------------------INTCON----------------------------------*/
/*--------------------------------------------------------------------------*/
void IO_Expander::resetIntContA()
{
  send(INTCONA, 0x00);
}

void IO_Expander::setIntContA(unsigned int pin, unsigned int ref)
{
  setBit(INTCONA, pin, ref);
}

void IO_Expander::setIntContA(unsigned int refs)
{
  send(INTCONA, refs);
}

unsigned int IO_Expander::readIntContA(unsigned int pin)
{
  readBit(INTCONA, pin);
}

unsigned int IO_Expander::readIntContA()
{
  receive(INTCONA);
}

void IO_Expander::resetIntContB()
{
  send(INTCONB, 0x00);
}

void IO_Expander::setIntContB(unsigned int pin, unsigned int ref)
{
  setBit(INTCONB, pin, ref);
}

void IO_Expander::setIntContB(unsigned int refs)
{
  send(INTCONB, refs);
}

unsigned int IO_Expander::readIntContB(unsigned int pin)
{
  readBit(INTCONB, pin);
}

unsigned int IO_Expander::readIntContB()
{
  receive(INTCONB);
}

void IO_Expander::resetIntConts()
{
  send16(INTCONA, INTCONB, 0x0000);
}

void IO_Expander::setIntConts(unsigned int refs)
{
  send16(INTCONA, INTCONB, refs);
}

void IO_Expander::setIntConts(char port, unsigned int pin, unsigned int ref)
{
  if (port == 'A' || port == 'a')
  {
    setBit(INTCONA, pin, ref);
  }
  else if (port == 'B' || port == 'b')
  {
    setBit(INTCONB, pin, ref);
  }
}

unsigned int IO_Expander::readIntConts(char port, unsigned int pin)
{
  if (port == 'A' || port == 'a')
  {
    readBit(INTCONA, pin);
  }
  else if (port == 'B' || port == 'b')
  {
    readBit(INTCONB, pin);
  }
}

unsigned int IO_Expander::readIntConts()
{
  unsigned int byte1 = receive(INTCONA);
  unsigned int byte2 = receive(INTCONB);

  return ((byte1 << 8) | byte2);
}

/*--------------------------------------------------------------------------*/
/*---------------------------------CONFIG-----------------------------------*/
/*--------------------------------------------------------------------------*/
void IO_Expander::resetConfig()
{
  send16(IOCONA, IOCONB, 0x0000);
  BANK = false;
  setAddress();
  determineAddresses();
}

void IO_Expander::setConfig(unsigned int bits)
{
  send16(IOCONA, IOCONB, bits);

  if (readBit(IOCONA, 7) == 0)
  {
    BANK = false;
    determineAddresses();
  }
  else if (readBit(IOCONA, 7) == 0)
  {
    BANK = true;
    determineAddresses();
  }

  setAddress();
}

unsigned int IO_Expander::readConfig()
{
  receive(IOCONA);
}

/*--------------------------------------------------------------------------*/
/*----------------------------------PULLUP----------------------------------*/
/*--------------------------------------------------------------------------*/
void IO_Expander::resetPullupA()
{
  send(GPPUA, 0x00);
}

void IO_Expander::setPullupA(unsigned int pin, unsigned int enable)
{
  setBit(GPPUA, pin, enable);
}

void IO_Expander::setPullupA(unsigned int enables)
{
  send(GPPUA, enables);
}

unsigned int IO_Expander::readPullupA(unsigned int pin)
{
  readBit(GPPUA, pin);
}

unsigned int IO_Expander::readPullupA()
{
  receive(GPPUA);
}

void IO_Expander::resetPullupB()
{
  send(GPPUB, 0x00);
}

void IO_Expander::setPullupB(unsigned int pin, unsigned int enable)
{
  setBit(GPPUB, pin, enable);
}

void IO_Expander::setPullupB(unsigned int enables)
{
  send(GPPUB, enables);
}

unsigned int IO_Expander::readPullupB(unsigned int pin)
{
  readBit(GPPUB, pin);
}

unsigned int IO_Expander::readPullupB()
{
  receive(GPPUB);
}

void IO_Expander::resetPullups()
{
  send16(GPPUA, GPPUB, 0x0000);
}

void IO_Expander::setPullups(unsigned int enables)
{
  send16(GPPUA, GPPUB, enables);
}

void IO_Expander::setPullups(char port, unsigned int pin, unsigned int enable)
{
  if (port == 'A' || port == 'a')
  {
  setBit(GPPUA, pin, enable);
  }
  else if (port == 'B' || port == 'b')
  {
    setBit(GPPUB, pin, enable);
  }
}

unsigned int IO_Expander::readPullups(char port, unsigned int pin)
{
  if (port == 'A' || port == 'a')
  {
    readBit(GPPUA, pin);
  }
  else if (port == 'B' || port == 'b')
  {
    readBit(GPPUB, pin);
  }
}

unsigned int IO_Expander::readPullups()
{
  unsigned int byte1 = receive(GPPUA);
  unsigned int byte2 = receive(GPPUB);

  return ((byte1 << 8) | byte2);
}

/*--------------------------------------------------------------------------*/
/*----------------------------------INTF------------------------------------*/
/*--------------------------------------------------------------------------*/
unsigned int IO_Expander::readINTFA(unsigned int pin)
{
  readBit(INTFA, pin);
}

unsigned int IO_Expander::readINTFA()
{
  receive(INTFA);
}

unsigned int IO_Expander::readINTFB(unsigned int pin)
{
  readBit(INTFB, pin);
}

unsigned int IO_Expander::readINTFB()
{
  receive(INTFB);
}

unsigned int IO_Expander::readINTFs(char port, unsigned int pin)
{
  if (port == 'A' || port == 'a')
  {
    readBit(INTFA, pin);
  }
  else if (port == 'B' || port == 'b')
  {
    readBit(INTFB, pin);
  }
}

unsigned int IO_Expander::readINTFs()
{
  unsigned int byte1 = receive(INTFA);
  unsigned int byte2 = receive(INTFB);

  return ((byte1 << 8) | byte2);
}

/*--------------------------------------------------------------------------*/
/*---------------------------------INTCAP-----------------------------------*/
/*--------------------------------------------------------------------------*/
unsigned int IO_Expander::readINTCAPA(unsigned int pin)
{
  readBit(INTCAPA, pin);
}

unsigned int IO_Expander::readINTCAPA()
{
  receive(INTCAPA);
}

unsigned int IO_Expander::readINTCAPB(unsigned int pin)
{
  readBit(INTCAPB, pin);
}

unsigned int IO_Expander::readINTCAPB()
{
  receive(INTCAPB);
}

unsigned int IO_Expander::readINTCAPs(char port, unsigned int pin)
{
  if (port == 'A' || port == 'a')
  {
    readBit(INTCAPA, pin);
  }
  else if (port == 'B' || port == 'b')
  {
    readBit(INTCAPB, pin);
  }
}

unsigned int IO_Expander::readINTCAPs()
{
  unsigned int byte1 = receive(INTCAPA);
  unsigned int byte2 = receive(INTCAPB);

  return ((byte1 << 8) | byte2);
}

/*--------------------------------------------------------------------------*/
/*-----------------------------------GPIO-----------------------------------*/
/*--------------------------------------------------------------------------*/
void IO_Expander::resetIOA()
{
  send(GPIOA, 0x00);
}

void IO_Expander::setIOA(unsigned int pin, unsigned int logic)
{
  setBit(GPIOA, pin, logic);
}

void IO_Expander::setIOA(unsigned int logics)
{
  send(GPIOA, logics);
}

unsigned int IO_Expander::readIOA(unsigned int pin)
{
  readBit(GPIOA, pin);
}

unsigned int IO_Expander::readIOA()
{
  receive(GPIOA);
}

void IO_Expander::resetIOB()
{
  send(GPIOB, 0x00);
}

void IO_Expander::setIOB(unsigned int pin, unsigned int logic)
{
  setBit(GPIOB, pin, logic);
}

void IO_Expander::setIOB(unsigned int logics)
{
  send(GPIOB, logics);
}

unsigned int IO_Expander::readIOB(unsigned int pin)
{
  readBit(GPIOB, pin);
}

unsigned int IO_Expander::readIOB()
{
  receive(GPIOB);
}

void IO_Expander::resetIOs()
{
  send16(GPIOA, GPIOB, 0x0000);
}

void IO_Expander::setIOs(unsigned int logics)
{
  send16(GPIOA, GPIOB, logics);
}

void IO_Expander::setIOs(char port, unsigned int pin, unsigned int logic)
{
  if (port == 'A' || port == 'a')
  {
  setBit(GPIOA, pin, logic);
  }
  else if (port == 'B' || port == 'b')
  {
    setBit(GPIOB, pin, logic);
  }
}

unsigned int IO_Expander::readIOs(char port, unsigned int pin)
{
  if (port == 'A' || port == 'a')
  {
    readBit(GPIOA, pin);
  }
  else if (port == 'B' || port == 'b')
  {
    readBit(GPIOB, pin);
  }
}

unsigned int IO_Expander::readIOs()
{
  unsigned int byte1 = receive(GPIOA);
  unsigned int byte2 = receive(GPIOB);

  return ((byte1 << 8) | byte2);
}

/*--------------------------------------------------------------------------*/
/*----------------------------------LATCH-----------------------------------*/
/*--------------------------------------------------------------------------*/
void IO_Expander::resetLatchA()
{
  send(OLATA, 0x00);
}

void IO_Expander::setLatchA(unsigned int pin, unsigned int logic)
{
  setBit(OLATA, pin, logic);
}

void IO_Expander::setLatchA(unsigned int logics)
{
  send(OLATA, logics);
}

unsigned int IO_Expander::readLatchA(unsigned int pin)
{
  readBit(OLATA, pin);
}

unsigned int IO_Expander::readLatchA()
{
  receive(OLATA);
}

void IO_Expander::resetLatchB()
{
  send(OLATB, 0x00);
}

void IO_Expander::setLatchB(unsigned int pin, unsigned int logic)
{
  setBit(OLATB, pin, logic);
}

void IO_Expander::setLatchB(unsigned int logics)
{
  send(OLATB, logics);
}

unsigned int IO_Expander::readLatchB(unsigned int pin)
{
  readBit(OLATB, pin);
}

unsigned int IO_Expander::readLatchB()
{
  receive(OLATB);
}

void IO_Expander::resetLatches()
{
  send16(OLATA, OLATB, 0x0000);
}

void IO_Expander::setLatches(unsigned int logics)
{
  send16(OLATA, OLATB, logics);
}

void IO_Expander::setLatches(char port, unsigned int pin, unsigned int logic)
{
  if (port == 'A' || port == 'a')
  {
  setBit(OLATA, pin, logic);
  }
  else if (port == 'B' || port == 'b')
  {
    setBit(OLATB, pin, logic);
  }
}

unsigned int IO_Expander::readLatches(char port, unsigned int pin)
{
  if (port == 'A' || port == 'a')
  {
    readBit(OLATA, pin);
  }
  else if (port == 'B' || port == 'b')
  {
    readBit(OLATB, pin);
  }
}

unsigned int IO_Expander::readLatches()
{
  unsigned int byte1 = receive(OLATA);
  unsigned int byte2 = receive(OLATB);

  return ((byte1 << 8) | byte2);
}




/************************************************************************/
/************************************************************************/
/************************************************************************/
/************************************************************************/

void IO_Expander::send(byte addr, unsigned int bytes)
{
  byte byte_to_send = bytes & 0xFF;
  SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV32, MSBFIRST, SPI_MODE0));
  digitalWrite(_CS, LOW);
  SPI.transfer(WRITE);
  SPI.transfer(addr);
  SPI.transfer(byte_to_send);
  digitalWrite(_CS, HIGH);
  SPI.endTransaction();
}

void IO_Expander::send16(byte addr1, byte addr2, unsigned int bytes)
{
  SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV32, MSBFIRST, SPI_MODE0));
  digitalWrite(_CS, LOW);
  SPI.transfer(WRITE);
  SPI.transfer(addr1);
  SPI.transfer(((bytes >> 8) & 0xFF));
  digitalWrite(_CS, HIGH);
  SPI.endTransaction();

  SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV32, MSBFIRST, SPI_MODE0));
  digitalWrite(_CS, LOW);
  SPI.transfer(WRITE);
  SPI.transfer(addr2);
  SPI.transfer((bytes & 0xFF));
  digitalWrite(_CS, HIGH);
  SPI.endTransaction();
}

void IO_Expander::setBit(byte addr, unsigned int pin, unsigned int logic)
{
  /* Set direction of an individual pin */
  SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV32, MSBFIRST, SPI_MODE0));
  digitalWrite(_CS, LOW);
  SPI.transfer(READ);
  SPI.transfer(addr);
  byte byte1 = (byte)SPI.transfer(0x00);
  digitalWrite(_CS, HIGH);
  delay(2);

  // Use 2's complement method of changing specific bit
  byte1 ^= ((-logic ^ byte1) & (1 << pin));

  digitalWrite(_CS, LOW);
  SPI.transfer(WRITE);
  SPI.transfer(addr);
  SPI.transfer(byte1);

  digitalWrite(_CS, HIGH);
  SPI.endTransaction();
}

unsigned int IO_Expander::receive(byte addr)
{
  SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV32, MSBFIRST, SPI_MODE0));
  digitalWrite(_CS, LOW);
  SPI.transfer(READ);
  SPI.transfer(addr);
  unsigned int return_byte = (unsigned int)SPI.transfer(0x00);
  digitalWrite(_CS, HIGH);
  SPI.endTransaction();

  return return_byte;
}

unsigned int IO_Expander::readBit(byte addr, unsigned int pin)
{
  SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV32, MSBFIRST, SPI_MODE0));
  digitalWrite(_CS, LOW);
  SPI.transfer(READ);
  SPI.transfer(addr);
  unsigned int return_byte = (unsigned int)SPI.transfer(0x00);
  digitalWrite(_CS, HIGH);
  SPI.endTransaction();

  return (unsigned int) bitRead(return_byte, pin);
}

void IO_Expander::setAddress()
{
  if (readBit(IOCONA, 3) == 0)
  {
    WRITE = 64;
    READ = 65;
  }
  else if (readBit(IOCONA, 3) == 1)
  {
    WRITE = WRITE_INIT;
    READ = READ_INIT;
  }
}

void IO_Expander::determineAddresses()
{
  if(BANK == false)
  {
    IODIRA = 0x00;
    IODIRB = 0x01;
    IPOLA = 0x02;
    IPOLB = 0x03;
    GPINTENA = 0x04;
    GPINTENB = 0x05;
    DEFVALA = 0x06;
    DEFVALB = 0x07;
    INTCONA = 0x08;
    INTCONB = 0x09;
    IOCONA = 0x0A;
    IOCONB = 0x0B;
    GPPUA = 0x0C;
    GPPUA = 0x0D;
    INTFA = 0x0E;
    INTFB = 0x0F;
    INTCAPA = 0x10;
    INTCAPB = 0x11;
    GPIOA = 0x12;
    GPIOB = 0x13;
    OLATA = 0x14;
    OLATB = 0x15;
  }
  else if(BANK == true)
  {
    IODIRA = 0x00;
    IODIRB = 0x10;
    IPOLA = 0x01;
    IPOLB = 0x11;
    GPINTENA = 0x02;
    GPINTENB = 0x12;
    DEFVALA = 0x03;
    DEFVALB = 0x13;
    INTCONA = 0x04;
    INTCONB = 0x14;
    IOCONA = 0x05;
    IOCONB = 0x15;
    GPPUA = 0x06;
    GPPUA = 0x16;
    INTFA = 0x07;
    INTFB = 0x17;
    INTCAPA = 0x08;
    INTCAPB = 0x18;
    GPIOA = 0x09;
    GPIOB = 0x19;
    OLATA = 0x0A;
    OLATB = 0x1A;
  }
}
