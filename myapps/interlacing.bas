1000 '***************************************************
1010 '  List 4.5  interlace mode
1020 '***************************************************
1030 '
1040 COLOR 15,0,0 : SCREEN 5,,,,,0 : DEFINT A-Z    'noninterlace mode
1050 '
1060 SET PAGE 0,0 : CLS
1070 LINE (32,0)-(64,120),15,BF
1080 SET PAGE 1,1 : CLS
1090 LINE (192,91)-(224,211),15,BF
1100 '
1110 VDP(10)=VDP(10) OR &B00001100     'interlace mode!!!
1120 '
1130 FOR I=32 TO 192
1140   SET PAGE 1,0
1150   LINE (I,0)-STEP(0,120),0
1160   LINE (I + 33,0)-STEP(0,120),15
1170   SET PAGE 1,1
1180   LINE (256 - I,91)-STEP(0,120),0
1190   LINE (221 - I,91)-STEP(0,120),15
1200 NEXT I
1210 '
1220 VDP(10)=VDP(10) AND &B11110011    'interlace off
1230 GOTO 1230