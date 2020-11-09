#include <IO_Expander-16.h>
#include <SPI.h>

const int IO_CS = A5;
const int IO_RST = 9;
const unsigned long EEPROM_START = 10;

// Instantiate I/O expander object
IO_Expander io(IO_CS, IO_RST, 0, 0, 0, EEPROM_START, false);

void setup()
{
  // Start SPI and Serial
  SPI.begin();
  Serial.begin(9600);

  // ATMEGA328p's SS line
  pinMode(10, OUTPUT);

  // Set initial configuration
  io.setConfig(0x6E6E);

  // Set pins as outputs, make latches active, and pull GPIOs low
  io.setDirections(0x0000);
  io.setLatches(0xFFFF);
  io.setIOs(0x0000);

  Serial.println("Setup complete!");
}

void loop()
{
  if(Serial.available() > 0)
  {
    unsigned char input = Serial.read();
    handleCommand(input);
    while(Serial.available() > 0) Serial.read();
  }
  delay(100);
}

void handleCommand(unsigned char in)
{
  switch(in)
  {
    case 'a':
    {
      // All pins high
      io.setIOs(0xFFFF);
      break;
    }
    case 's':
    {
      // High-half high
      io.setIOs(0xFF00);
      break;
    }
    case 'd':
    {
      // Low-half high
      io.setIOs(0x00FF);
      break;
    }
    case 'f':
    {
      // "Binary" counter
      for (unsigned int i = 0; i < 0xFFFF; i++)
      {
        io.setIOs(i);
        delay(100);
      }
      break;
    }
    case 'q':
    {
      // All low
      io.setIOs(0x0000);
    }
    default:
    {
      break;
    }
  }
}
