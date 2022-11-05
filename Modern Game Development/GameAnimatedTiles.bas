10 COLOR 15,1,1
20 SCREEN 4: DEFINT A-Z: CO=0: REM color sweep

120 REM Load patterns and colors
130 FOR I=0 TO &H1F
140 READ R
150 VPOKE I,R : REM load tile pattern
160 VPOKE &H2000+I, &H20: REM load color fore/back ground = green on black
170 NEXT I

200 REM populate the screen
210 FOR I=&H1800 TO &H18FF
220 VPOKE I,I MOD 2: REM set name table
230 NEXT I

300 REM Main loop
310 IF CF=1 THEN CF=0 ELSE CF=1
320 S=16+CF*8
330 FOR I=0 TO 7
340 VPOKE 8+I, VPEEK(S+I)
350 VPOKE &H2000+8+I, VPEEK(&H2000+S+I)
360 NEXT I
361 CO=(CO+1) MOD 8
362 COLOR=(2,0,CO,0)
370 IF TIME<20 THEN 370 ELSE TIME=0: REM sync cpu & gpu
380 GOTO 300

1000 DATA &H00,&H00,&H00,&H00,&H00,&H00,&H00,&H00: REM black tile
1010 DATA &H18,&H3C,&H7E,&HDB,&HFF,&H24,&H5A,&HA5: REM base tile
1020 DATA &H18,&H3C,&H7E,&HDB,&HFF,&H24,&H5A,&HA5: REM animation 1
1030 DATA &H18,&H3C,&H7E,&HDB,&HFF,&H5A,&H81,&H42: REM animation 2
