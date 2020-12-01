#ifndef SERIALCLI_H_
#define SERIALCLI_H_
#include <stdint.h>

#define MAX_MESSAGE_SIZE 30
#define MAX_ARG_SIZE 30

struct node
{
  char *command;
  void (*function)(int argc, char **argv);
  struct node *next;
};

class SerialCLI
{
public:
  SerialCLI();
  void addCommand(char *cmd, void (*func)(int argc, char **argv));
  void displayCLI();
  void handleCommand();
  void pollCLI();
  void initCLI(long speed);
  long string2Long(char *input);
  float string2Float(char *input);
  static int message[MAX_MESSAGE_SIZE];
  static int *messagePointer;

private:
  node *head, *tail, *current;
  void parse(char *cmd);
};


#endif // SERIALCLI_H_
