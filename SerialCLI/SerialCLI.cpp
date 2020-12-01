#include <avr/pgmspace.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include "HardwareSerial.h"

#include "SerialCLI.h"

int SerialCLI::message[] = {0};
int* SerialCLI::messagePointer = NULL;

const char banner[] PROGMEM = "-------------- CLI --------------";
const char prompt[] PROGMEM = "COMMAND: ";
const char unrecognized[] PROGMEM = "Error: Command not recognized.";

SerialCLI::SerialCLI()
{
  head = NULL;
  tail = NULL;
}

void SerialCLI::addCommand(char *cmd, void (*func)(int argc, char **argv))
{
  current = (node*)malloc(sizeof(node));
  char *command_name = (char*)malloc(strlen(cmd) + 1);

  strcpy(command_name, cmd);
  command_name[strlen(cmd)] = '\0';

  current->command = command_name;
  current->function = func;

  if(head == NULL)
  {
    head = current;
    head->next = NULL;
    tail = NULL;
    current = NULL;
  }
  else if(head != NULL && tail == NULL)
  {
    head->next = current;
    tail = current;
    tail->next = NULL;
    current = NULL;
  }
  else
  {
    tail->next = current;
    tail = current;
    tail->next = NULL;
    current = NULL;
  }
}

void SerialCLI::displayCLI()
{
  char buffer[MAX_MESSAGE_SIZE];
  Serial.println();

  strcpy_P(buffer, banner);
  Serial.println(buffer);

  strcpy_P(buffer, prompt);
  Serial.println(buffer);
}

void SerialCLI::parse(char *cmd)
{
  int argc, i = 0;
  char *argv[MAX_ARG_SIZE];
  char buffer[MAX_MESSAGE_SIZE];

  fflush(stdout);

  argv[i] = strtok(cmd, " ");
  do
  {
    argv[++i] = strtok(NULL, " ");
  }while((i < MAX_ARG_SIZE) && (argv[i] != NULL));

  argc = i - 1;

  Serial.print(F("Input was: "));
  Serial.println(argv[0]);

  for(current = head; current != NULL; current = current->next)
  {
    if(!strcmp(argv[0], current->command))
    {
      current->function(argc, argv);
      displayCLI();
      return;
    }
  }

  strcpy_P(buffer, unrecognized);
  Serial.println(buffer);
  displayCLI();
  Serial.flush();
}

void SerialCLI::handleCommand()
{
  char c = Serial.read();

  switch(c)
  {
    case '\r':
      *messagePointer = '\0';
      Serial.print("\r\n");
      parse((char *)message);
      messagePointer = message;
      break;

    case '\b':
      Serial.print(c);
      if(messagePointer > message)
      {
        messagePointer--;
      }
      break;

    default:
      Serial.print(c);
      *messagePointer = c;
      messagePointer++;
      break;
  }
}

void SerialCLI::pollCLI()
{
  while(Serial.available())
  {
    handleCommand();
  }
}

void SerialCLI::initCLI(long speed)
{
  messagePointer = message;
  Serial.begin(speed);
}

long SerialCLI::string2Long(char *input)
{
  long number = 0;

  for(int i = 0; i < strlen(input); i++)
  {
    if(input[i] >= '0' && input[i] <= '9')
    {
      number *= 10;
      number += (long)input[i];
    }
    else
    {
      return number;
    }
  }
  return number;
}

float SerialCLI::string2Float(char *input)
{
  float number = 0.0;
  float decimal = 0.0;
  bool after_decimal = false;

  for(int i = 0; i < strlen(input); i++)
  {
    if(input[i] == '.')
    {
      after_decimal = true;
      continue;
    }
    else if(input[i] >= '0' && input[i] <= '9' && !after_decimal)
    {
      number *= 10;
      number += (float)input[i];
    }
    else if(input[i] >= '0' && input[i] <= '9' && after_decimal)
    {
      decimal += (float)input[i];
      decimal /= 10;
    }
    else
    {
      return (number + decimal);
    }
  }
  return (number + decimal);
}
