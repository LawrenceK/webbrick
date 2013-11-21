	list p=16f690
	#include <p16f690.inc>
	radix dec
; Begin
	R0L EQU 0x20
	R0H EQU 0x21
	R1L EQU 0x22
	R1H EQU 0x23
	R2L EQU 0x24
	R2H EQU 0x25
	R3L EQU 0x26
	R3H EQU 0x27
	R4L EQU 0x28
	R4H EQU 0x29
	R5L EQU 0x2A
	R5H EQU 0x2B
	SO_PORT EQU 0x2C
	SO_BIT EQU 0x2D
	SO_INTL EQU 0x2E
	SO_INTH EQU 0x2F
	ORG 0x0000
	BCF PCLATH,3
	BCF PCLATH,4
	GOTO L0007
	ORG 0x0004
	RETFIE
L0007:
; 1: Define CONF_WORD = 0x330c
; 2: 
; 3:  'HP-URPS Controller Firmware
; 4: 
; 5: Dim x As Byte
;       The address of 'x' is 0x33
	x EQU 0x33
; 6: Dim y As Byte
;       The address of 'y' is 0x34
	y EQU 0x34
; 7: Dim i As Byte
;       The address of 'i' is 0x35
	i EQU 0x35
; 8: Dim tim As Word
;       The address of 'tim' is 0x36
	tim EQU 0x36
; 9: Dim batvolts As Word
;       The address of 'batvolts' is 0x38
	batvolts EQU 0x38
; 10: Dim psu1volts As Word
;       The address of 'psu1volts' is 0x3A
	psu1volts EQU 0x3A
; 11: Dim psu2volts As Word
;       The address of 'psu2volts' is 0x3C
	psu2volts EQU 0x3C
; 12: Dim opvolts As Word
;       The address of 'opvolts' is 0x3E
	opvolts EQU 0x3E
; 13: Dim batov As Bit
;       The address of 'batov' is 0x30,0
; 14: Dim batuv As Bit
;       The address of 'batuv' is 0x30,1
; 15:  'Dim p1ov As Bit
; 16:  'Dim p1uv As Bit
; 17:  'Dim p2uv As Bit
; 18:  'Dim p2ov As Bit
; 19: Dim nobat As Bit
;       The address of 'nobat' is 0x30,2
; 20:  'Dim button As Bit
; 21: Dim charge As Bit
;       The address of 'charge' is 0x30,3
; 22: Dim discharge As Bit
;       The address of 'discharge' is 0x30,4
; 23: Dim trickle As Bit
;       The address of 'trickle' is 0x30,5
; 24: Dim top_green As Bit
;       The address of 'top_green' is 0x30,6
; 25: Dim top_red As Bit
;       The address of 'top_red' is 0x30,7
; 26: Dim ctr_green As Bit
;       The address of 'ctr_green' is 0x31,0
; 27: Dim ctr_red As Bit
;       The address of 'ctr_red' is 0x31,1
; 28: Dim bot_green As Bit
;       The address of 'bot_green' is 0x31,2
; 29: Dim bot_red As Bit
;       The address of 'bot_red' is 0x31,3
; 30: Dim leds_blanked As Bit
;       The address of 'leds_blanked' is 0x31,4
; 31:  'Dim alarm As Bit
; 32: Dim palarm1 As Bit 'psu alarm
;       The address of 'palarm1' is 0x31,5
; 33: Dim palarm2 As Bit 'psu alarm
;       The address of 'palarm2' is 0x31,6
; 34: Dim balarm As Bit 'Battery Alarm
;       The address of 'balarm' is 0x31,7
; 35: Dim alarm_on As Bit
;       The address of 'alarm_on' is 0x32,0
; 36: Dim al As Bit
;       The address of 'al' is 0x32,1
; 37:  'Dim pal As Bit
; 38:  'Dim tal As Bit
; 39: Dim alarm_byte As Byte
;       The address of 'alarm_byte' is 0x40
	alarm_byte EQU 0x40
; 40: Dim bov As Bit
;       The address of 'bov' is 0x32,2
; 41: Dim out_ctrl As Bit
;       The address of 'out_ctrl' is 0x32,3
; 42: Dim p1v As Word
;       The address of 'p1v' is 0x41
	p1v EQU 0x41
; 43: Dim p1vl As Byte
;       The address of 'p1vl' is 0x43
	p1vl EQU 0x43
; 44: Dim p2v As Word
;       The address of 'p2v' is 0x44
	p2v EQU 0x44
; 45: Dim p2vl As Byte
;       The address of 'p2vl' is 0x46
	p2vl EQU 0x46
; 46: Dim ov As Word
;       The address of 'ov' is 0x47
	ov EQU 0x47
; 47: Dim ovl As Byte
;       The address of 'ovl' is 0x49
	ovl EQU 0x49
; 48: Dim bv As Word
;       The address of 'bv' is 0x4A
	bv EQU 0x4A
; 49: Dim bvl As Byte
;       The address of 'bvl' is 0x4C
	bvl EQU 0x4C
; 50:  'Dim button_pressed1 As Bit
; 51:  'Dim button_pressed2 As Bit
; 52: Dim uv_psuv As Word
;       The address of 'uv_psuv' is 0x4D
	uv_psuv EQU 0x4D
; 53: 
; 54: 
; 55: 
; 56:  'With Schottky Diodes --- Voltages x 68.33
; 57: Const ov_psu = 963
;       The value of 'ov_psu' is 963
; 58: Const uv_psu = 950
;       The value of 'uv_psu' is 950
; 59: Const uv_psuch = 854 '12.5v
;       The value of 'uv_psuch' is 854
; 60: 
; 61: Const batt_ov = 965 '14.0V
;       The value of 'batt_ov' is 965
; 62: Const sw_out_off = 806 '11.8V
;       The value of 'sw_out_off' is 806
; 63: Const sw_out_on = 854 '12.5V
;       The value of 'sw_out_on' is 854
; 64: Const warn_batt = 750 '11.0V
;       The value of 'warn_batt' is 750
; 65: Const uv_batt = 683 '10.0V
;       The value of 'uv_batt' is 683
; 66: Const dead_batt = 341 '5.0V
;       The value of 'dead_batt' is 341
; 67: 
; 68: ADCON0 = 0
	CLRF 0x1F
; 69: ADCON1 = %01000000
	BSF STATUS,RP0
	MOVLW 0x40
	MOVWF 0x1F
; 70: OSCCON = %01000001 'Internal Osc 1MHz No external signal ***Ensure compiler set to 1MHz
	MOVLW 0x41
	MOVWF 0x0F
; 71: ANSEL = %10001001
	BSF STATUS,RP1
	BCF STATUS,RP0
	MOVLW 0x89
	MOVWF 0x1E
; 72: ANSELH = %00001000
	MOVLW 0x08
	MOVWF 0x1F
; 73: ADCON0 = %11011000
	BCF STATUS,RP1
	MOVLW 0xD8
	MOVWF 0x1F
; 74: 
; 75: TMR0 = 0
	CLRF 0x01
; 76: ASM:        clrwdt
	       clrwdt
; 77: OPTION_REG = %10001110
	BSF STATUS,RP0
	MOVLW 0x8E
	MOVWF 0x01
; 78: 
; 79: x = 0
	BCF STATUS,RP0
	CLRF 0x33
; 80: y = 0
	CLRF 0x34
; 81: Define ADC_CLOCK = 3
; 82:  'Define ADC_SAMPLEUS = 250
; 83: 
; 84:  'Define ports
; 85: TRISA.0 = 1 'Battery Voltage
	BSF STATUS,RP0
	BSF 0x05,0
; 86: TRISA.1 = 1 'ADC Reference input
	BSF 0x05,1
; 87: TRISA.2 = 0 'Switched output control
	BCF 0x05,2
; 88: TRISA.3 = 1 'Push button
	BSF 0x05,3
; 89: TRISA.4 = 1 'PSU1 Voltage
	BSF 0x05,4
; 90: TRISA.5 = 0 'LED1
	BCF 0x05,5
; 91: 
; 92: TRISB.4 = 0 'LED3
	BCF 0x06,4
; 93: TRISB.5 = 1 'Output voltage
	BSF 0x06,5
; 94: TRISB.6 = 0 'LED2
	BCF 0x06,6
; 95: TRISB.7 = 0 'Serial out
	BCF 0x06,7
; 96: 
; 97: TRISC.0 = 0 'LED3
	BCF 0x07,0
; 98: TRISC.1 = 0 'Buzzer control
	BCF 0x07,1
; 99: TRISC.2 = 0 'LED2
	BCF 0x07,2
; 100: TRISC.3 = 1 'PSU2 Voltage
	BSF 0x07,3
; 101: TRISC.4 = 0 'LED1
	BCF 0x07,4
; 102: TRISC.5 = 0 'Under-voltage control
	BCF 0x07,5
; 103: TRISC.6 = 0 'Trickle charge control
	BCF 0x07,6
; 104: TRISC.7 = 0 'Over-voltage switch control
	BCF 0x07,7
; 105: 
; 106:  'Initialise ports
; 107: PORTA.5 = 0
	BCF STATUS,RP0
	BCF 0x05,5
; 108: PORTA.5 = 0
	BCF 0x05,5
; 109: PORTB.4 = 0
	BCF 0x06,4
; 110: PORTB.6 = 0
	BCF 0x06,6
; 111: PORTB.7 = 0
	BCF 0x06,7
; 112: PORTC.0 = 0
	BCF 0x07,0
; 113: PORTC.1 = 0
	BCF 0x07,1
; 114: PORTC.2 = 0
	BCF 0x07,2
; 115: PORTC.4 = 0
	BCF 0x07,4
; 116: PORTC.5 = 0
	BCF 0x07,5
; 117: PORTC.6 = 0
	BCF 0x07,6
; 118: PORTC.7 = 0
	BCF 0x07,7
; 119: 
; 120:  'Initialise variables
; 121: charge = False
	BCF 0x30,3
; 122: discharge = False
	BCF 0x30,4
; 123: top_green = False
	BCF 0x30,6
; 124: top_red = False
	BCF 0x30,7
; 125: ctr_green = False
	BCF 0x31,0
; 126: ctr_red = False
	BCF 0x31,1
; 127: bot_green = False
	BCF 0x31,2
; 128: bot_red = False
	BCF 0x31,3
; 129: al = False
	BCF 0x32,1
; 130: alarm_on = True
	BSF 0x32,0
; 131: bov = False
	BCF 0x32,2
; 132: trickle = True
	BSF 0x30,5
; 133: out_ctrl = True
	BSF 0x32,3
; 134: tim = 1
	MOVLW 0x01
	MOVWF 0x36
	CLRF 0x37
; 135: balarm = False
	BCF 0x31,7
; 136: palarm1 = False
	BCF 0x31,5
; 137: palarm2 = False
	BCF 0x31,6
; 138: 
; 139: Gosub led_test
	CALL L0006
; 140:  '__________________________________________________________
; 141: Define SEROUT_DELAYUS = 2000
; 142: 
; 143:  '**********************************************************
; 144:  '__________________________________________________________
; 145: start:  'START OF MAIN LOOP
L0001:
; 146: 
; 147: Gosub control
	CALL L0004
; 148: ASM:        clrwdt
	       clrwdt
; 149:  'flash heart-beat LED every 64 times round loop
; 150: If leds_blanked = True And al = False And x = 0 Then
	BTFSS 0x31,4
	GOTO L0008
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSC 0x32,1
	GOTO L0008
	MOVLW 0x1F
	ANDWF STATUS,F
	MOVF 0x33,W
	SUBLW 0x00
	BTFSS STATUS,Z
	GOTO L0008
; 151: PORTB.6 = 1
	BSF 0x06,6
; 152: WaitMs 10
	MOVLW 0x0A
	MOVWF R0L
	CLRF R0H
	CALL W001
; 153: PORTB.6 = 0
	BCF 0x06,6
; 154: Else
	GOTO L0009
L0008:	MOVLW 0x1F
	ANDWF STATUS,F
; 155: If leds_blanked = True And al = True And x = 0 Then
	BTFSS 0x31,4
	GOTO L0010
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSS 0x32,1
	GOTO L0010
	MOVLW 0x1F
	ANDWF STATUS,F
	MOVF 0x33,W
	SUBLW 0x00
	BTFSS STATUS,Z
	GOTO L0010
; 156: PORTC.2 = 1
	BSF 0x07,2
; 157: WaitMs 10
	MOVLW 0x0A
	MOVWF R0L
	CLRF R0H
	CALL W001
; 158: PORTC.2 = 0
	BCF 0x07,2
; 159: Endif
L0010:	MOVLW 0x1F
	ANDWF STATUS,F
; 160: Endif
L0009:	MOVLW 0x1F
	ANDWF STATUS,F
; 161: 
; 162:  'Create Serial data bytes and send them every 64 times around the loop
; 163: If x = 0 Then
	MOVF 0x33,W
	SUBLW 0x00
	BTFSS STATUS,Z
	GOTO L0011
; 164: p1v = psu1volts * 8
	MOVF 0x3A,W
	MOVWF R3L
	MOVF 0x3B,W
	MOVWF R3H
	MOVLW 0x08
	MOVWF R1L
	CLRF R1H
	CALL M001
	MOVWF 0x41
	MOVF R2H,W
	MOVWF 0x42
; 165: p1vl = p1v.LB / 8
	MOVF 0x41,W
	MOVWF R0L
	CLRF R0H
	MOVLW 0x08
	MOVWF R1L
	CLRF R1H
	CALL D001
	MOVWF 0x43
; 166: 
; 167: p2v = psu2volts * 8
	MOVF 0x3C,W
	MOVWF R3L
	MOVF 0x3D,W
	MOVWF R3H
	MOVLW 0x08
	MOVWF R1L
	CLRF R1H
	CALL M001
	MOVWF 0x44
	MOVF R2H,W
	MOVWF 0x45
; 168: p2vl = p2v.LB / 8
	MOVF 0x44,W
	MOVWF R0L
	CLRF R0H
	MOVLW 0x08
	MOVWF R1L
	CLRF R1H
	CALL D001
	MOVWF 0x46
; 169: 
; 170: bv = batvolts * 8
	MOVF 0x38,W
	MOVWF R3L
	MOVF 0x39,W
	MOVWF R3H
	MOVLW 0x08
	MOVWF R1L
	CLRF R1H
	CALL M001
	MOVWF 0x4A
	MOVF R2H,W
	MOVWF 0x4B
; 171: bvl = bv.LB / 8
	MOVF 0x4A,W
	MOVWF R0L
	CLRF R0H
	MOVLW 0x08
	MOVWF R1L
	CLRF R1H
	CALL D001
	MOVWF 0x4C
; 172: 
; 173: ov = opvolts * 8
	MOVF 0x3E,W
	MOVWF R3L
	MOVF 0x3F,W
	MOVWF R3H
	MOVLW 0x08
	MOVWF R1L
	CLRF R1H
	CALL M001
	MOVWF 0x47
	MOVF R2H,W
	MOVWF 0x48
; 174: ovl = ov.LB / 8
	MOVF 0x47,W
	MOVWF R0L
	CLRF R0H
	MOVLW 0x08
	MOVWF R1L
	CLRF R1H
	CALL D001
	MOVWF 0x49
; 175: 
; 176:  'Start byte 0xFF
; 177:  'all data words shifted so high and low bytes each contain 5 bits both right justified
; 178: Serout PORTB.7, 2400, 0xff, p1v.HB, p1vl, p2v.HB, p2vl, bv.HB, bvl, ov.HB, ovl, alarm_byte
	BSF STATUS,RP0
	BCF TRISB,7
	BCF STATUS,RP0
	BSF PORTB,7
	MOVLW 0x06
	MOVWF SO_PORT
	MOVLW 0x80
	MOVWF SO_BIT
	MOVLW 0x93
	MOVWF SO_INTL
	MOVLW 0x01
	MOVWF SO_INTH
	MOVLW 0xFF
	CALL SO01
	MOVF 0x42,W
	CALL SO01
	MOVF 0x43,W
	CALL SO01
	MOVF 0x45,W
	CALL SO01
	MOVF 0x46,W
	CALL SO01
	MOVF 0x4B,W
	CALL SO01
	MOVF 0x4C,W
	CALL SO01
	MOVF 0x48,W
	CALL SO01
	MOVF 0x49,W
	CALL SO01
	MOVF 0x40,W
	CALL SO01
; 179: 
; 180: Endif
L0011:	MOVLW 0x1F
	ANDWF STATUS,F
; 181: 
; 182: If x < 63 Then
	MOVLW 0x3F
	SUBWF 0x33,W
	BTFSC STATUS,C
	GOTO L0012
; 183: x = x + 1
	MOVF 0x33,W
	ADDLW 0x01
	MOVWF 0x33
; 184: Else
	GOTO L0013
L0012:	MOVLW 0x1F
	ANDWF STATUS,F
; 185: x = 0
	CLRF 0x33
; 186: Endif
L0013:	MOVLW 0x1F
	ANDWF STATUS,F
; 187: 
; 188: 
; 189: For i = 0 To 9
	CLRF 0x35
L0014:
	MOVF 0x35,W
	SUBLW 0x09
	BTFSS STATUS,C
	GOTO L0015
; 190: 
; 191: Gosub time_on
	CALL L0005
; 192: ASM:        clrwdt
	       clrwdt
; 193:  'Adcin 0, batvolts
; 194: Adcin 3, psu2volts
	BSF ADCON0,ADFM
	BSF STATUS,RP0
	MOVLW 0x30
	MOVWF ADCON1
	MOVLW 0x03
	BCF STATUS,RP0
	MOVWF R0L
	CALL A001
	BSF STATUS,RP0
	MOVF ADRESL,W
	BCF STATUS,RP0
	MOVWF 0x3C
	MOVF ADRESH,W
	MOVWF 0x3D
; 195: Adcin 7, psu1volts
	BSF ADCON0,ADFM
	BSF STATUS,RP0
	MOVLW 0x30
	MOVWF ADCON1
	MOVLW 0x07
	BCF STATUS,RP0
	MOVWF R0L
	CALL A001
	BSF STATUS,RP0
	MOVF ADRESL,W
	BCF STATUS,RP0
	MOVWF 0x3A
	MOVF ADRESH,W
	MOVWF 0x3B
; 196: Adcin 2, opvolts
	BSF ADCON0,ADFM
	BSF STATUS,RP0
	MOVLW 0x30
	MOVWF ADCON1
	MOVLW 0x02
	BCF STATUS,RP0
	MOVWF R0L
	CALL A001
	BSF STATUS,RP0
	MOVF ADRESL,W
	BCF STATUS,RP0
	MOVWF 0x3E
	MOVF ADRESH,W
	MOVWF 0x3F
; 197: 
; 198:  'If charge = True Then  'Lower alarm threshold condition when charge is true
; 199: uv_psuv = uv_psuch
	MOVLW 0x56
	MOVWF 0x4D
	MOVLW 0x03
	MOVWF 0x4E
; 200:  'Else
; 201:  'uv_psuv = uv_psu
; 202:  'Endif
; 203: 
; 204:  '------------------------------------------
; 205:  'Mains PSU monitor  - 1
; 206: If psu1volts > ov_psu Then 'Over Voltage
	MOVF 0x3A,W
	MOVWF R0L
	MOVF 0x3B,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xC3
	CALL C003
	BTFSS STATUS,Z
	GOTO L0016
; 207: top_green = True
	BSF 0x30,6
; 208: top_red = True
	BSF 0x30,7
; 209: Endif
L0016:	MOVLW 0x1F
	ANDWF STATUS,F
; 210: 
; 211: If psu1volts < ov_psu And psu1volts > uv_psuv Then 'Voltage OK
	MOVF 0x3A,W
	MOVWF R0L
	MOVF 0x3B,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xC3
	CALL C004
	BTFSS STATUS,Z
	GOTO L0017
	MOVLW 0x1F
	ANDWF STATUS,F
	MOVF 0x3A,W
	MOVWF R0L
	MOVF 0x3B,W
	MOVWF R0H
	MOVF 0x4E,W
	MOVWF R1H
	MOVF 0x4D,W
	CALL C003
	BTFSS STATUS,Z
	GOTO L0017
; 212: top_green = True
	BSF 0x30,6
; 213: top_red = False
	BCF 0x30,7
; 214: If palarm1 = True Then alarm_on = True
	BTFSS 0x31,5
	GOTO L0018
	BSF 0x32,0
L0018:	MOVLW 0x1F
	ANDWF STATUS,F
; 215: palarm1 = False
	BCF 0x31,5
; 216: Endif
L0017:	MOVLW 0x1F
	ANDWF STATUS,F
; 217: 
; 218: If psu1volts < uv_psuv Then 'Under voltage
	MOVF 0x3A,W
	MOVWF R0L
	MOVF 0x3B,W
	MOVWF R0H
	MOVF 0x4E,W
	MOVWF R1H
	MOVF 0x4D,W
	CALL C004
	BTFSS STATUS,Z
	GOTO L0019
; 219: top_green = False
	BCF 0x30,6
; 220: top_red = True
	BSF 0x30,7
; 221: palarm1 = True
	BSF 0x31,5
; 222: Endif
L0019:	MOVLW 0x1F
	ANDWF STATUS,F
; 223: 
; 224:  '------------------------------------------
; 225:  'Mains PSU monitor  - 2
; 226: If psu2volts > ov_psu Then 'Over Voltage
	MOVF 0x3C,W
	MOVWF R0L
	MOVF 0x3D,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xC3
	CALL C003
	BTFSS STATUS,Z
	GOTO L0020
; 227: ctr_green = True
	BSF 0x31,0
; 228: ctr_red = True
	BSF 0x31,1
; 229: Endif
L0020:	MOVLW 0x1F
	ANDWF STATUS,F
; 230: 
; 231: If psu2volts < ov_psu And psu2volts > uv_psuv Then 'Voltage OK
	MOVF 0x3C,W
	MOVWF R0L
	MOVF 0x3D,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xC3
	CALL C004
	BTFSS STATUS,Z
	GOTO L0021
	MOVLW 0x1F
	ANDWF STATUS,F
	MOVF 0x3C,W
	MOVWF R0L
	MOVF 0x3D,W
	MOVWF R0H
	MOVF 0x4E,W
	MOVWF R1H
	MOVF 0x4D,W
	CALL C003
	BTFSS STATUS,Z
	GOTO L0021
; 232: ctr_green = True
	BSF 0x31,0
; 233: ctr_red = False
	BCF 0x31,1
; 234: If palarm2 = True Then alarm_on = True
	BTFSS 0x31,6
	GOTO L0022
	BSF 0x32,0
L0022:	MOVLW 0x1F
	ANDWF STATUS,F
; 235: palarm2 = False
	BCF 0x31,6
; 236: Endif
L0021:	MOVLW 0x1F
	ANDWF STATUS,F
; 237: 
; 238: If psu2volts < uv_psuv Then 'Under voltage
	MOVF 0x3C,W
	MOVWF R0L
	MOVF 0x3D,W
	MOVWF R0H
	MOVF 0x4E,W
	MOVWF R1H
	MOVF 0x4D,W
	CALL C004
	BTFSS STATUS,Z
	GOTO L0023
; 239: ctr_green = False
	BCF 0x31,0
; 240: ctr_red = True
	BSF 0x31,1
; 241: palarm2 = True
	BSF 0x31,6
; 242: Endif
L0023:	MOVLW 0x1F
	ANDWF STATUS,F
; 243: 
; 244: Next i
	MOVLW 0x01
	ADDWF 0x35,F
	BTFSS STATUS,C
	GOTO L0014
L0015:	MOVLW 0x1F
	ANDWF STATUS,F
; 245: 
; 246:  '---------------
; 247:  'Battery monitor
; 248: 
; 249: 
; 250: Gosub check_bat_present
	CALL L0002
; 251: If nobat = True Then
	BTFSS 0x30,2
	GOTO L0024
; 252: balarm = True
	BSF 0x31,7
; 253: Goto start
	GOTO L0001
; 254: Endif
L0024:	MOVLW 0x1F
	ANDWF STATUS,F
; 255: 
; 256: For i = 0 To 9
	CLRF 0x35
L0025:
	MOVF 0x35,W
	SUBLW 0x09
	BTFSS STATUS,C
	GOTO L0026
; 257: 
; 258: ASM:        clrwdt
	       clrwdt
; 259: Gosub time_on
	CALL L0005
; 260: 
; 261: If batvolts > ov_psu Then 'Over Voltage > at battery
	MOVF 0x38,W
	MOVWF R0L
	MOVF 0x39,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xC3
	CALL C003
	BTFSS STATUS,Z
	GOTO L0027
; 262: bot_green = True
	BSF 0x31,2
; 263: bot_red = True
	BSF 0x31,3
; 264: charge = False
	BCF 0x30,3
; 265: trickle = False
	BCF 0x30,5
; 266: Endif
L0027:	MOVLW 0x1F
	ANDWF STATUS,F
; 267: 
; 268: If batvolts < ov_psu And batvolts > warn_batt Then 'Voltage OK between 11V and 14V
	MOVF 0x38,W
	MOVWF R0L
	MOVF 0x39,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xC3
	CALL C004
	BTFSS STATUS,Z
	GOTO L0028
	MOVLW 0x1F
	ANDWF STATUS,F
	MOVF 0x38,W
	MOVWF R0L
	MOVF 0x39,W
	MOVWF R0H
	MOVLW 0x02
	MOVWF R1H
	MOVLW 0xEE
	CALL C003
	BTFSS STATUS,Z
	GOTO L0028
; 269: bot_green = True
	BSF 0x31,2
; 270: bot_red = False
	BCF 0x31,3
; 271: discharge = True
	BSF 0x30,4
; 272: out_ctrl = True
	BSF 0x32,3
; 273: If balarm = True Then alarm_on = True
	BTFSS 0x31,7
	GOTO L0029
	BSF 0x32,0
L0029:	MOVLW 0x1F
	ANDWF STATUS,F
; 274: balarm = False
	BCF 0x31,7
; 275: Endif
L0028:	MOVLW 0x1F
	ANDWF STATUS,F
; 276: 
; 277: If batvolts < ov_psu And batvolts > uv_batt Then
	MOVF 0x38,W
	MOVWF R0L
	MOVF 0x39,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xC3
	CALL C004
	BTFSS STATUS,Z
	GOTO L0030
	MOVLW 0x1F
	ANDWF STATUS,F
	MOVF 0x38,W
	MOVWF R0L
	MOVF 0x39,W
	MOVWF R0H
	MOVLW 0x02
	MOVWF R1H
	MOVLW 0xAB
	CALL C003
	BTFSS STATUS,Z
	GOTO L0030
; 278: trickle = True
	BSF 0x30,5
; 279: charge = True
	BSF 0x30,3
; 280: Endif
L0030:	MOVLW 0x1F
	ANDWF STATUS,F
; 281: 
; 282: If batvolts < sw_out_off Then 'Under voltage <11.8V
	MOVF 0x38,W
	MOVWF R0L
	MOVF 0x39,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0x26
	CALL C004
	BTFSS STATUS,Z
	GOTO L0031
; 283: out_ctrl = False
	BCF 0x32,3
; 284: Endif
L0031:	MOVLW 0x1F
	ANDWF STATUS,F
; 285: 
; 286: If ctr_green = True Or top_green = True Then 'Mains power restored from either PSU
	BTFSS 0x31,0
	GOTO L0034
	GOTO L0033
L0034:
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSS 0x30,6
	GOTO L0032
L0033:
; 287: out_ctrl = True
	BSF 0x32,3
; 288: Endif
L0032:	MOVLW 0x1F
	ANDWF STATUS,F
; 289: 
; 290: If batvolts < warn_batt Then 'Under voltage <11V
	MOVF 0x38,W
	MOVWF R0L
	MOVF 0x39,W
	MOVWF R0H
	MOVLW 0x02
	MOVWF R1H
	MOVLW 0xEE
	CALL C004
	BTFSS STATUS,Z
	GOTO L0035
; 291: balarm = True
	BSF 0x31,7
; 292: Endif
L0035:	MOVLW 0x1F
	ANDWF STATUS,F
; 293: 
; 294: If batvolts < uv_batt Then 'Under voltage <10V
	MOVF 0x38,W
	MOVWF R0L
	MOVF 0x39,W
	MOVWF R0H
	MOVLW 0x02
	MOVWF R1H
	MOVLW 0xAB
	CALL C004
	BTFSS STATUS,Z
	GOTO L0036
; 295: charge = False
	BCF 0x30,3
; 296: discharge = False
	BCF 0x30,4
; 297: bot_green = False
	BCF 0x31,2
; 298: bot_red = True
	BSF 0x31,3
; 299: trickle = True
	BSF 0x30,5
; 300: Endif
L0036:	MOVLW 0x1F
	ANDWF STATUS,F
; 301: 
; 302: Next i
	MOVLW 0x01
	ADDWF 0x35,F
	BTFSS STATUS,C
	GOTO L0025
L0026:	MOVLW 0x1F
	ANDWF STATUS,F
; 303: Goto start
	GOTO L0001
; 304: End
L0037:	GOTO L0037
; 305:  'Check if a battery is connected or is dead
; 306: 
; 307: check_bat_present: 
L0002:
; 308: 
; 309:  'Turn off charging circuit to isolate the battery input
; 310: PORTC.6 = 0 'Trickle off
	BCF 0x07,6
; 311: PORTC.7 = 0 'Charge off
	BCF 0x07,7
; 312: ASM:        clrwdt
	       clrwdt
; 313: WaitMs 3
	MOVLW 0x03
	MOVWF R0L
	CLRF R0H
	CALL W001
; 314: ASM:        clrwdt
	       clrwdt
; 315: 
; 316: Adcin 0, batvolts 'Measure battery voltage
	BSF ADCON0,ADFM
	BSF STATUS,RP0
	MOVLW 0x30
	MOVWF ADCON1
	MOVLW 0x00
	BCF STATUS,RP0
	MOVWF R0L
	CALL A001
	BSF STATUS,RP0
	MOVF ADRESL,W
	BCF STATUS,RP0
	MOVWF 0x38
	MOVF ADRESH,W
	MOVWF 0x39
; 317: If batvolts < dead_batt Then 'if battery <5V turn off the battery circuit
	MOVF 0x38,W
	MOVWF R0L
	MOVF 0x39,W
	MOVWF R0H
	MOVLW 0x01
	MOVWF R1H
	MOVLW 0x55
	CALL C004
	BTFSS STATUS,Z
	GOTO L0038
; 318: nobat = True
	BSF 0x30,2
; 319: charge = False
	BCF 0x30,3
; 320: trickle = False
	BCF 0x30,5
; 321: bot_green = False
	BCF 0x31,2
; 322: bot_red = True
	BSF 0x31,3
; 323: balarm = True
	BSF 0x31,7
; 324: Else
	GOTO L0039
L0038:	MOVLW 0x1F
	ANDWF STATUS,F
; 325: nobat = False
	BCF 0x30,2
; 326: Endif
L0039:	MOVLW 0x1F
	ANDWF STATUS,F
; 327: Gosub control
	CALL L0004
; 328: WaitUs 100
	MOVLW 0x64
	MOVWF R4L
	CALL X001
; 329: Return
	RETURN
; 330: 
; 331: display:  'Decide which LEDS are on
L0003:
; 332: 
; 333: If leds_blanked = True Then
	BTFSS 0x31,4
	GOTO L0040
; 334: PORTA.5 = 0
	BCF 0x05,5
; 335: PORTC.4 = 0
	BCF 0x07,4
; 336: PORTB.6 = 0
	BCF 0x06,6
; 337: PORTC.2 = 0
	BCF 0x07,2
; 338: PORTB.4 = 0
	BCF 0x06,4
; 339: PORTC.0 = 0
	BCF 0x07,0
; 340: Else
	GOTO L0041
L0040:	MOVLW 0x1F
	ANDWF STATUS,F
; 341: PORTB.6 = top_green
	BTFSC 0x30,6
	BSF 0x06,6
	BTFSS 0x30,6
	BCF 0x06,6
; 342: PORTC.2 = top_red
	BTFSC 0x30,7
	BSF 0x07,2
	BTFSS 0x30,7
	BCF 0x07,2
; 343: PORTC.4 = ctr_green
	BTFSC 0x31,0
	BSF 0x07,4
	BTFSS 0x31,0
	BCF 0x07,4
; 344: PORTA.5 = ctr_red
	BTFSC 0x31,1
	BSF 0x05,5
	BTFSS 0x31,1
	BCF 0x05,5
; 345: PORTC.0 = bot_green
	BTFSC 0x31,2
	BSF 0x07,0
	BTFSS 0x31,2
	BCF 0x07,0
; 346: PORTB.4 = bot_red
	BTFSC 0x31,3
	BSF 0x06,4
	BTFSS 0x31,3
	BCF 0x06,4
; 347: Endif
L0041:	MOVLW 0x1F
	ANDWF STATUS,F
; 348: Return
	RETURN
; 349: 
; 350: control:  'Output the controls to MOSFETs and Buzzer
L0004:
; 351: 
; 352: PORTC.7 = charge
	BTFSC 0x30,3
	BSF 0x07,7
	BTFSS 0x30,3
	BCF 0x07,7
; 353: PORTC.5 = discharge
	BTFSC 0x30,4
	BSF 0x07,5
	BTFSS 0x30,4
	BCF 0x07,5
; 354: PORTC.6 = trickle
	BTFSC 0x30,5
	BSF 0x07,6
	BTFSS 0x30,5
	BCF 0x07,6
; 355: PORTA.2 = out_ctrl
	BTFSC 0x32,3
	BSF 0x05,2
	BTFSS 0x32,3
	BCF 0x05,2
; 356: Gosub display
	CALL L0003
; 357: 
; 358: al = False
	BCF 0x32,1
; 359: al = palarm1 Or palarm2
	CLRW
	BTFSC 0x31,5
	ADDLW 0x01
	BTFSC 0x31,6
	IORLW 0x01
	BTFSC STATUS,Z
	BCF 0x32,1
	BTFSS STATUS,Z
	BSF 0x32,1
; 360: al = al Or balarm
	CLRW
	BTFSC 0x32,1
	ADDLW 0x01
	BTFSC 0x31,7
	IORLW 0x01
	BTFSC STATUS,Z
	BCF 0x32,1
	BTFSS STATUS,Z
	BSF 0x32,1
; 361: 
; 362:  'Pulse the buzzer
; 363: If alarm_on = True Then
	BTFSS 0x32,0
	GOTO L0042
; 364: If y >= 12 And al = True Then
	MOVLW 0x0C
	SUBWF 0x34,W
	BTFSS STATUS,C
	GOTO L0043
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSS 0x32,1
	GOTO L0043
; 365: PORTC.1 = 0
	BCF 0x07,1
; 366: Endif
L0043:	MOVLW 0x1F
	ANDWF STATUS,F
; 367: If y < 12 And al = True Then
	MOVLW 0x0C
	SUBWF 0x34,W
	BTFSC STATUS,C
	GOTO L0044
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSS 0x32,1
	GOTO L0044
; 368: PORTC.1 = 1
	BSF 0x07,1
; 369: Endif
L0044:	MOVLW 0x1F
	ANDWF STATUS,F
; 370: 
; 371: If y < 15 Then
	MOVLW 0x0F
	SUBWF 0x34,W
	BTFSC STATUS,C
	GOTO L0045
; 372: y = y + 1
	MOVF 0x34,W
	ADDLW 0x01
	MOVWF 0x34
; 373: Else
	GOTO L0046
L0045:	MOVLW 0x1F
	ANDWF STATUS,F
; 374: y = 0
	CLRF 0x34
; 375: Endif
L0046:	MOVLW 0x1F
	ANDWF STATUS,F
; 376: Endif
L0042:	MOVLW 0x1F
	ANDWF STATUS,F
; 377: If al = False Then PORTC.1 = 0 'ensure alarm always turns off
	BTFSC 0x32,1
	GOTO L0047
	BCF 0x07,1
L0047:	MOVLW 0x1F
	ANDWF STATUS,F
; 378: If alarm_on = False Then PORTC.1 = 0
	BTFSC 0x32,0
	GOTO L0048
	BCF 0x07,1
L0048:	MOVLW 0x1F
	ANDWF STATUS,F
; 379: 
; 380: 
; 381:  'Build alarm Byte
; 382: alarm_byte.0 = Not charge
	BTFSS 0x30,3
	BSF 0x40,0
	BTFSC 0x30,3
	BCF 0x40,0
; 383: alarm_byte.1 = Not discharge
	BTFSS 0x30,4
	BSF 0x40,1
	BTFSC 0x30,4
	BCF 0x40,1
; 384: alarm_byte.2 = Not trickle
	BTFSS 0x30,5
	BSF 0x40,2
	BTFSC 0x30,5
	BCF 0x40,2
; 385: alarm_byte.3 = Not out_ctrl 'battery < 11.8v
	BTFSS 0x32,3
	BSF 0x40,3
	BTFSC 0x32,3
	BCF 0x40,3
; 386: alarm_byte.4 = Not nobat
	BTFSS 0x30,2
	BSF 0x40,4
	BTFSC 0x30,2
	BCF 0x40,4
; 387: If top_red = 1 Or ctr_red = 1 Then alarm_byte.5 = 1 'psu failure
	BTFSS 0x30,7
	GOTO L0051
	GOTO L0050
L0051:
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSS 0x31,1
	GOTO L0049
L0050:
	BSF 0x40,5
L0049:	MOVLW 0x1F
	ANDWF STATUS,F
; 388: alarm_byte.6 = bot_red 'battery < 10V
	BTFSC 0x31,3
	BSF 0x40,6
	BTFSS 0x31,3
	BCF 0x40,6
; 389: alarm_byte.7 = 0 'must be zero to avoid confusion with start byte (hFF)
	BCF 0x40,7
; 390: Return
	RETURN
; 391: 
; 392: time_on:  'Timer for LED display
L0005:
; 393: 
; 394: If PORTA.3 = 0 Then 'Button pressed
	BTFSC 0x05,3
	GOTO L0052
; 395: tim = 65535 'Loop counter 2min
	MOVLW 0xFF
	MOVWF 0x36
	MOVLW 0xFF
	MOVWF 0x37
; 396: leds_blanked = False 'Disable LED display
	BCF 0x31,4
; 397: If al = True Then
	BTFSS 0x32,1
	GOTO L0053
; 398: alarm_on = False
	BCF 0x32,0
; 399: Endif
L0053:	MOVLW 0x1F
	ANDWF STATUS,F
; 400: Endif
L0052:	MOVLW 0x1F
	ANDWF STATUS,F
; 401: 
; 402: tim = tim - 1
	MOVLW 0x01
	SUBWF 0x36,W
	MOVWF 0x36
	MOVLW 0x00
	BTFSS STATUS,C
	ADDLW 0x01
	SUBWF 0x37,W
	MOVWF 0x37
; 403: If tim = 0 Then
	MOVF 0x36,W
	MOVWF R0L
	MOVF 0x37,W
	MOVWF R0H
	CLRF R1H
	MOVLW 0x00
	CALL C001
	BTFSS STATUS,Z
	GOTO L0054
; 404: leds_blanked = True
	BSF 0x31,4
; 405:  'alarm_on = True
; 406: Endif
L0054:	MOVLW 0x1F
	ANDWF STATUS,F
; 407: Return
	RETURN
; 408: 
; 409: 
; 410: 
; 411: led_test:  'Start-up Display
L0006:
; 412: ASM:        clrwdt
	       clrwdt
; 413: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x35
L0055:
	MOVF 0x35,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0056
; 414: PORTA.5 = 1 'Red
	BSF 0x05,5
; 415: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 416: PORTA.5 = 0
	BCF 0x05,5
; 417: Next i
	MOVLW 0x01
	ADDWF 0x35,F
	BTFSS STATUS,C
	GOTO L0055
L0056:	MOVLW 0x1F
	ANDWF STATUS,F
; 418: ASM:        clrwdt
	       clrwdt
; 419: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x35
L0057:
	MOVF 0x35,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0058
; 420: PORTC.4 = 1 'Green
	BSF 0x07,4
; 421: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 422: PORTC.4 = 0
	BCF 0x07,4
; 423: Next i
	MOVLW 0x01
	ADDWF 0x35,F
	BTFSS STATUS,C
	GOTO L0057
L0058:	MOVLW 0x1F
	ANDWF STATUS,F
; 424: ASM:        clrwdt
	       clrwdt
; 425: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x35
L0059:
	MOVF 0x35,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0060
; 426: PORTB.6 = 1 'Green
	BSF 0x06,6
; 427: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 428: PORTB.6 = 0
	BCF 0x06,6
; 429: Next i
	MOVLW 0x01
	ADDWF 0x35,F
	BTFSS STATUS,C
	GOTO L0059
L0060:	MOVLW 0x1F
	ANDWF STATUS,F
; 430:  'ASM:        clrwdt
; 431: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x35
L0061:
	MOVF 0x35,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0062
; 432: PORTC.2 = 1 'Red
	BSF 0x07,2
; 433: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 434: PORTC.2 = 0
	BCF 0x07,2
; 435: Next i
	MOVLW 0x01
	ADDWF 0x35,F
	BTFSS STATUS,C
	GOTO L0061
L0062:	MOVLW 0x1F
	ANDWF STATUS,F
; 436: ASM:        clrwdt
	       clrwdt
; 437: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x35
L0063:
	MOVF 0x35,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0064
; 438: PORTB.4 = 1 'Red
	BSF 0x06,4
; 439: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 440: PORTB.4 = 0
	BCF 0x06,4
; 441: Next i
	MOVLW 0x01
	ADDWF 0x35,F
	BTFSS STATUS,C
	GOTO L0063
L0064:	MOVLW 0x1F
	ANDWF STATUS,F
; 442: ASM:        clrwdt
	       clrwdt
; 443: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x35
L0065:
	MOVF 0x35,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0066
; 444: PORTC.0 = 1 'Green
	BSF 0x07,0
; 445: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 446: PORTC.0 = 0
	BCF 0x07,0
; 447: Next i
	MOVLW 0x01
	ADDWF 0x35,F
	BTFSS STATUS,C
	GOTO L0065
L0066:	MOVLW 0x1F
	ANDWF STATUS,F
; 448: ASM:        clrwdt
	       clrwdt
; 449: For i = 1 To 2
	MOVLW 0x01
	MOVWF 0x35
L0067:
	MOVF 0x35,W
	SUBLW 0x02
	BTFSS STATUS,C
	GOTO L0068
; 450: PORTA.5 = 1
	BSF 0x05,5
; 451: PORTC.4 = 1
	BSF 0x07,4
; 452: PORTB.6 = 1
	BSF 0x06,6
; 453: PORTC.2 = 1
	BSF 0x07,2
; 454: PORTB.4 = 1
	BSF 0x06,4
; 455: PORTC.0 = 1
	BSF 0x07,0
; 456: PORTC.1 = 1
	BSF 0x07,1
; 457: WaitMs 200
	MOVLW 0xC8
	MOVWF R0L
	CLRF R0H
	CALL W001
; 458: ASM:        clrwdt
	       clrwdt
; 459: PORTA.5 = 0
	BCF 0x05,5
; 460: PORTC.4 = 0
	BCF 0x07,4
; 461: PORTB.6 = 0
	BCF 0x06,6
; 462: PORTC.2 = 0
	BCF 0x07,2
; 463: PORTB.4 = 0
	BCF 0x06,4
; 464: PORTC.0 = 0
	BCF 0x07,0
; 465: PORTC.1 = 0
	BCF 0x07,1
; 466: WaitMs 200
	MOVLW 0xC8
	MOVWF R0L
	CLRF R0H
	CALL W001
; 467: ASM:        clrwdt
	       clrwdt
; 468: Next i
	MOVLW 0x01
	ADDWF 0x35,F
	BTFSS STATUS,C
	GOTO L0067
L0068:	MOVLW 0x1F
	ANDWF STATUS,F
; 469: ASM:        clrwdt
	       clrwdt
; 470: WaitMs 200
	MOVLW 0xC8
	MOVWF R0L
	CLRF R0H
	CALL W001
; 471: ASM:        clrwdt
	       clrwdt
; 472: Return
	RETURN
; 473: 
; 474: 
; 475: 
; End of program
L0069:	GOTO L0069
; Multiplication Routine
M001:	MOVLW 0x10
	MOVWF R4L
	CLRF R0H
	CLRF R0L
M002:	RRF R3H,F
	RRF R3L,F
	BTFSS STATUS,C
	GOTO M003
	MOVF R1L,W
	ADDWF R0L,F
	MOVF R1H,W
	BTFSC STATUS,C
	INCFSZ R1H,W
	ADDWF R0H,F
M003:	RRF R0H,F
	RRF R0L,F
	RRF R2H,F
	RRF R2L,F
	DECFSZ R4L,F
	GOTO M002
	MOVF R2L,W
	RETURN
; Division Routine
D001:	MOVLW 0x10
	MOVWF R3L
	CLRF R2H
	CLRF R2L
D002:	RLF R0H,W
	RLF R2L,F
	RLF R2H,F
	MOVF R1L,W
	SUBWF R2L,F
	MOVF R1H,W
	BTFSS STATUS,C
	INCFSZ R1H,W
	SUBWF R2H,F
	BTFSC STATUS,C
	GOTO D003
	MOVF R1L,W
	ADDWF R2L,F
	MOVF R1H,W
	BTFSC STATUS,C
	INCFSZ R1H,W
	ADDWF R2H,F
	BCF STATUS,C
D003:	RLF R0L,F
	RLF R0H,F
	DECFSZ R3L,F
	GOTO D002
	MOVF R0L,W
	RETURN
; Comparison Routine
C001:	MOVWF R1L
	MOVLW 0x05
	GOTO C007
C002:	MOVWF R1L
	MOVLW 0x02
	GOTO C007
C003:	MOVWF R1L
	MOVLW 0x06
	GOTO C007
C004:	MOVWF R1L
	MOVLW 0x03
	GOTO C007
C005:	MOVWF R1L
	MOVLW 0x04
	GOTO C007
C006:	MOVWF R1L
	MOVLW 0x01
	GOTO C007
C007:	MOVWF R4L
	MOVF R1H,W
	SUBWF R0H,W
	BTFSS STATUS,Z
	GOTO C008
	MOVF R1L,W
	SUBWF R0L,W
C008:	MOVLW 0x04
	BTFSC STATUS,C
	MOVLW 0x01
	BTFSC STATUS,Z
	MOVLW 0x02
	ANDWF R4L,W
	BTFSS STATUS,Z
	MOVLW 0xFF
	RETURN
; Waitms Routine
W001:	MOVF R0L,F
	BTFSC STATUS,Z
	GOTO W002
	CALL W003
	DECF R0L,F
	NOP
	NOP
	NOP
	NOP
	NOP
	GOTO W001
W002:	MOVF R0H,F
	BTFSC STATUS,Z
	RETURN
	CALL W003
	DECF R0H,F
	DECF R0L,F
	GOTO W001
W003:	MOVLW 0x0C
	MOVWF R2H
W004:	DECFSZ R2H,F
	GOTO W004
	NOP
	NOP
	MOVLW 0x12
	MOVWF R1L
W005:	DECFSZ R1L,F
	GOTO W006
	CALL W007
	CALL W007
	NOP
	NOP
	RETURN
W006:	CALL W007
	GOTO W005
W007:	MOVLW 0x0D
	MOVWF R2L
W008:	DECFSZ R2L,F
	GOTO W008
	NOP
	RETURN
; Waitus Routine - Byte Argument
X001:	MOVLW 0x0A
	SUBWF R4L,F
	BTFSS STATUS,C
	RETURN
	GOTO X002
X002:	MOVLW 0x06
	SUBWF R4L,F
	BTFSS STATUS,C
	RETURN
	GOTO X002
; Waitus Routine - Word Argument
Y001:	MOVLW 0x10
	SUBWF R4L,F
	CLRW
	BTFSS STATUS,C
	ADDLW 0x01
	SUBWF R4H,F
	BTFSS STATUS,C
	RETURN
	GOTO Y002
Y002:	MOVLW 0x0A
	SUBWF R4L,F
	CLRW
	BTFSS STATUS,C
	ADDLW 0x01
	SUBWF R4H,F
	BTFSS STATUS,C
	RETURN
	GOTO Y002
; Adcin Routine
A001:	RLF R0L,F
	RLF R0L,F
	MOVLW 0x3C
	ANDWF R0L,F
	MOVLW 0xC0
	ANDWF ADCON0,F
	MOVF R0L,W
	IORWF ADCON0,F
	BSF ADCON0,ADON
	MOVLW 0x14
	MOVWF R4L
	CALL X001
	BSF ADCON0,GO
A002:	BTFSC ADCON0,GO
	GOTO A002
	BCF PIR1,ADIF
	BCF ADCON0,ADON
	RETURN
; Serout Routine
SO01:	MOVWF R5L
	MOVLW 0xD0
	MOVWF R4L
	MOVLW 0x07
	MOVWF R4H
	CALL Y001
	MOVF SO_PORT,W
	MOVWF FSR
	MOVLW 0x09
	MOVWF R5H
	BCF STATUS,C
SO02:	CALL SO03
	RRF R5L,F
	DECFSZ R5H,F
	GOTO SO02
	BSF STATUS,C
	CALL SO03
	RETURN
SO03:	MOVF INDF,W
	IORWF SO_BIT,W
	BTFSS STATUS,C
	XORWF SO_BIT,W
	MOVWF INDF
	MOVF SO_INTL,W
	MOVWF R4L
	MOVF SO_INTH,W
	MOVWF R4H
	CALL Y001
	RETURN
; Configuration word settings
	ORG 0x2007
	DW 0x330C
; End of listing
	END
