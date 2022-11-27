/*
 VDP Graphic screen 3 (basic screen4) page concept
 Using FUSION-C 1.2

 2022 F. Dumarey
 */

#include <stdio.h>
#include "fusion-c/header/vdp_graph2.h"
#include "fusion-c/header/msx_fusion.h"

void main (void)
{
  // Prepare vdp mode
  Screen(4);
  SetColors(15,4,4);
  Cls();

  // Don't use this in Graphic screen 3!
  // it changes R#2 bit 5 for pages concept, but R#2 is used for defining the base address of the name table!
  //SetDisplayPage(0);
  //SetActivePage(0);

  //Put a box on the screen
  FillVram(0x0000,0xFF,8); // pattern 0 = black box 8x8
  FillVram(0x2000,0xFF,8); // color table = white on white
  FillVram(0x1800,0,10); // name table = 10 times pattern 0
  WaitKey();

  // Test HMMM
  // tried to convert 0x1800 / 256 (pixel based) and copy pixels to memory, but does not work!
  HMMM(0,0,0,10,8,8);
  HMMM(0,24,0,25,1,1);
  WaitKey();

  Screen(0);
  Exit(0);

}
