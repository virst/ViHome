#ifndef UTILS
#define UTILS

#include "OneButton.h"

#define PIN_COUNT 8

void(* resetFunc) (void) = 0;//объявляем функцию reset с адресом 0

struct Parameter {   // Declare PERSON struct type
  int did;
  int pid;
  byte direct;
  byte type;  
  char additionalCodes[PIN_COUNT];
  byte additionalInformation[PIN_COUNT];
  long period;
  unsigned long nextTime;  
  OneButton **buttons;
};

int PidCount(Parameter p)
{
  int r = 0;
  for (int i = 0; i < PIN_COUNT; i++)
    if (p.additionalCodes[i] != 0) r++;
  return r;
}

#endif
