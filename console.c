/*
 Fusion-C
 Some console functions
 F. Dumarey
*/

#include "fusion-c/header/msx_fusion.h"
#include <stdio.h>

void main(void) 
{
  char i;
    
  Cls();
  Screen(0);
  Width(80);
  SetColors(2,1,1);
  FunctionKeys(0);
  Print("Testing some console \t functions \"ok\"\n");
  PrintNumber(100);
  i = InputChar();
  Locate(10,10);
  Print("At location 10,10\n");
  i = ReadMSXtype();
  printf("MSX type: %c\n", i);
  printf("Timer: %u\n", RealTimer());
  do
  {
    i = Inkey();
    Locate(0,20);
    printf("Timer: %u \t Key pressed: %c", RealTimer(), i);
  } while (i != 65);
  Beep();
}
