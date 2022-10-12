1000 '*********************************************************
1010 '  LIST 4.1  BLINK SAMPLE
1020 '*********************************************************
1030 '
1040 SCREEN 0 : WIDTH 80 : DEFINT A-Z		'TEXT 2 mode
1050 ADR=BASE(1)			'TAKE COLOR TABLE ADDRESS
1060 '
1070 FOR I=0 TO 2048/8
1080   VPOKE ADR+I,0			'reset blink mode
1090 NEXT
1100 '
1110 VDP(7) =&HF1			'text color=15, back color=1
1120 VDP(13)=&H1F			'text color=1,  back color=15
1130 VDP(14)=&H22			'set interval and start blink
1140 '
1150 PRINT "Input any character : ";
1160 '
1170 K$=INPUT$(1)
1180 IF K$<CHR$(28) THEN 1230
1190 IF K$>" " THEN GOSUB 1280
1200 PRINT K$;
1210 GOTO 1170
1220 '
1230 VDP(14)=0				'stop blink
1240 END
1250 '
1260 '----- set blink mode -----
1270 '
1280 X=POS(0) : Y=CSRLIN
1290 A=(Y*80+X)\8
1300 B=X MOD 8
1310 M=VAL("&B"+MID$("000000010000000",8-B,8))
1320 VPOKE ADR + A, VPEEK(ADR + A) XOR M
1330 RETURN