/*
    MSX Fusion-C source code
    ------------------------

    Purpose: Screen 4 specific library for tiles and 128KB VRAM usage
    Author: F. Dumarey
    Date: November 2022
    Version: 0.2

*/

// include headers
#include "fusion-c/header/msx_fusion.h"
#include "fusion-c/header/vdp_sprites.h"
#include "fusion-c/header/vdp_graph1.h"
#include <stdio.h>

// function prototypes
void SC4SetDisplayPage (unsigned char page);
void SC4SetActivePage (unsigned char page);
void SC4Vpoke (unsigned int address, unsigned char value);
unsigned char SC4Vpeek (unsigned int address);
void SC4SetTilePattern (unsigned char block, unsigned char tilenumber, char *p_pattern);
void SC4SetTileColor (unsigned char block, unsigned char tilenumber, char *p_color);
void SC4PutTile (unsigned char block, unsigned char tilenumber, unsigned char x, unsigned char y);
void SC4CopyRamToVram (char *RamAddress, unsigned int VramAddress, unsigned char count);
void SC4CopyVramToRam (unsigned int VramAddress, char *RamAddress, unsigned char count);


// global variables

// sprite example from tinysprite
const unsigned char sprite1[] = {
0x00,0x7F,0x7F,0x7F,0x7F,0x7F,0x77,0x73,
0x71,0x70,0x70,0x70,0x70,0x00,0x00,0x00,
0x00,0xF8,0xF8,0xF8,0x00,0x80,0x80,0xE0,
0xE0,0xF0,0x78,0x38,0x18,0x00,0x00,0x00
};

const unsigned char color1[] = {
0x00,0x02,0x02,0x02,0x09,0x09,0x09,0x04,
0x04,0x04,0x0D,0x0D,0x0D,0x00,0x00,0x00
};

// globals for the SC4 library
const unsigned int SC4BasePatternAddress = 0x0000;
const unsigned int SC4BaseColorAddress = 0x2000;
const unsigned int SC4BaseNameAddress = 0x1800;
static unsigned char SC4Page = 0;

// main function
int main(char *argv[], int argc)
{
  // local variables
  unsigned char key,i;

  // screen mode
    Screen(4);
    SpriteReset();
    Sprite16();
  // code
    SC2Rect(0,0,48,48,15,NO_FILL); //screen 2 VDP functions also work for screen 4!
    SetSpritePattern(0, sprite1, 32);
    //SetSpriteColors(1, color1);
    for(i=0; i<16;i++)
    {
      Vpoke(0x1C00+i,color1[i]); //no sprite color support for screen 4, so doing manually
    }
    PutSprite(0,0,10,10,15);

    key = WaitKey();
    SC4SetDisplayPage(1);
    key = WaitKey();
    SC4SetDisplayPage(0);
    key = WaitKey();
    Screen(0);
  // return or exit code
  return(0);
}

// functions

void SC4SetDisplayPage(unsigned char page)
{
  unsigned char bit16;
  unsigned int PatternAddress;
  unsigned int ColorAddress;
  unsigned int NameAddress;
  unsigned char Register4;
  unsigned char Register3;
  unsigned char Register10;
  unsigned char Register2;
  
  if (page>7) return; //invalid page number

  bit16 = (page>3) ? 1 : 0; //are we above 64KB - 16bits?
  page %= 4; //make 16-bit adress
  PatternAddress = SC4BasePatternAddress + page * 0x4000; //calculate new adresses
  ColorAddress = SC4BaseColorAddress + page * 0x4000;
  NameAddress = SC4BaseNameAddress + page * 0x4000;
  Register4 = ((unsigned char)(PatternAddress >> 11)) & 0x1C | 0x03 | (bit16 << 5); //calculate VDP register values for base adresses
  Register3 = ((unsigned char)(ColorAddress >> 6)) & 0x80 | 0x7F;
  Register10 = ((unsigned char)(ColorAddress >> 14)) & 0x03 | (bit16 << 2);
  Register2 = ((unsigned char)(NameAddress >> 10)) & 0x3F | (bit16 << 6);

  DisableInterrupt();
  VDPwriteNi(4,Register4); //set VDP registers in order to realign base adresses
  VDPwriteNi(3,Register3);
  VDPwriteNi(10,Register10);
  VDPwriteNi(2,Register2);
  EnableInterrupt();
}

void SC4SetActivePage(unsigned char page)
{
  if (page>7) return; //invalid page number
  SC4Page = page;
}

void SC4Vpoke(unsigned int address, unsigned char value)
{
  unsigned char bit16;
  unsigned char page;
  unsigned int Address;
  
  bit16 = (SC4Page>3) ? 1 : 0; //are we above 64KB - 16bits?
  page = SC4Page % 4; //make 16-bit adress
  Address = address + page * 0x4000; //calculate new adresses
  
  DisableInterrupt();
  VDPwriteNi(14,(unsigned char)(Address >> 14) & 0x07 | (bit16 << 2));
  OutPort(0x99,(unsigned char)(Address));
  OutPort(0x99,(unsigned char)(Address >> 8) & 0x3F | 0x40);
  OutPort(0x98,value);
  EnableInterrupt();
}

unsigned char SC4Vpeek(unsigned int address)
{
  unsigned char bit16;
  unsigned char page;
  unsigned int Address;
  
  bit16 = (SC4Page>3) ? 1 : 0; //are we above 64KB - 16bits?
  page = SC4Page % 4; //make 16-bit adress
  Address = address + page * 0x4000; //calculate new adresses
  
  DisableInterrupt();
  VDPwriteNi(14,(unsigned char)(Address >> 14) & 0x07 | (bit16 << 2));
  OutPort(0x99,(unsigned char)(Address));
  OutPort(0x99,(unsigned char)(Address >> 8) & 0x3F);
  EnableInterrupt();
  return InPort(0x98);
}

void SC4SetTilePattern(unsigned char block, unsigned char tilenumber, char *p_pattern)
{
  unsigned int Address;
  
  if (block>3) return; //illegal block number

  Address = SC4BasePatternAddress + block * 256 + tilenumber * 8;
  SC4Vpoke(Address, p_pattern[0]);
  OutPorts(0x98, ++p_pattern, 7);
}

void SC4SetTileColor(unsigned char block, unsigned char tilenumber, char *p_color)
{
  unsigned int Address;
  
  if (block>3) return; //illegal block number

  Address = SC4BaseColorAddress + block * 256 + tilenumber * 8;
  SC4Vpoke(Address, p_color[0]);
  OutPorts(0x98, ++p_color, 7);
}

void SC4PutTile(unsigned char block, unsigned char tilenumber, unsigned char x, unsigned char y)
{
  unsigned int Address;

  if (block>3 || x>31 || y>7) return; //illegal parameter

  Address = SC4BaseNameAddress + block * 256 + y * 32 + x;
  SC4Vpoke(Address, tilenumber);
}

void SC4CopyRamToVram(char *RamAddress, unsigned int VramAddress, unsigned char count)
{
  if (VramAddress > 0x3FFF || count <2) return; //illegal address (>16KB)

  SC4Vpoke(VramAddress, RamAddress[0]);
  OutPorts(0x98, ++RamAddress, count-1);
}

void SC4CopyVramToRam(unsigned int VramAddress, char *RamAddress, unsigned char count)
{
  if (VramAddress > 0x3FFF || count <2) return; //illegal address (>16KB)

  RamAddress[0] = SC4Vpeek(VramAddress);

  //make inports function
  __asm
  ld iy,#2
  add iy,sp ;bypass the return address of the function call
  ld c,#0x98 ;set in port to port 0x98
  ld l,2(iy) ;bypass vramaddress, get ramaddress lsb
  ld h,3(iy) ;get ramaddress msb
  inc hl ;increase ramaddress by one since vpeek done
  ld b,4(iy) ;get count
  dec b ;decrease count by one since vpeek done
  di ;dissable interrupt
  inir ;in instruction increase and repeat
  ei ;enable interrupt
  __endasm;
}
