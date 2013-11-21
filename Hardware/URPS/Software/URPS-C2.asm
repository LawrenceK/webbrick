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
	ORG 0x0000
	BCF PCLATH,3
	BCF PCLATH,4
	GOTO L0007
	ORG 0x0004
	RETFIE
L0007:
; 1:  'URPC Controller Firmware
; 2: 
; 3: Define CONF_WORD = 0x33bc
; 4: 
; 5: 
; 6: Dim x As Byte
;       The address of 'x' is 0x2F
	x EQU 0x2F
; 7: Dim y As Byte
;       The address of 'y' is 0x30
	y EQU 0x30
; 8: Dim yy As Byte
;       The address of 'yy' is 0x31
	yy EQU 0x31
; 9: Dim i As Byte
;       The address of 'i' is 0x32
	i EQU 0x32
; 10: Dim tim As Word
;       The address of 'tim' is 0x33
	tim EQU 0x33
; 11: Dim batvolts As Word
;       The address of 'batvolts' is 0x35
	batvolts EQU 0x35
; 12: Dim psu1volts As Word
;       The address of 'psu1volts' is 0x37
	psu1volts EQU 0x37
; 13: Dim psu2volts As Word
;       The address of 'psu2volts' is 0x39
	psu2volts EQU 0x39
; 14: Dim opvolts As Word
;       The address of 'opvolts' is 0x3B
	opvolts EQU 0x3B
; 15: Dim batt_ov As Word
;       The address of 'batt_ov' is 0x3D
	batt_ov EQU 0x3D
; 16: Dim batov As Bit
;       The address of 'batov' is 0x2C,0
; 17: Dim batuv As Bit
;       The address of 'batuv' is 0x2C,1
; 18: Dim p1ov As Bit
;       The address of 'p1ov' is 0x2C,2
; 19: Dim p1uv As Bit
;       The address of 'p1uv' is 0x2C,3
; 20: Dim p2uv As Bit
;       The address of 'p2uv' is 0x2C,4
; 21: Dim p2ov As Bit
;       The address of 'p2ov' is 0x2C,5
; 22: Dim nobat As Bit
;       The address of 'nobat' is 0x2C,6
; 23: Dim pa7 As Bit
;       The address of 'pa7' is 0x2C,7
; 24: Dim button As Bit
;       The address of 'button' is 0x2D,0
; 25: Dim charge As Bit
;       The address of 'charge' is 0x2D,1
; 26: Dim discharge As Bit
;       The address of 'discharge' is 0x2D,2
; 27: Dim trickle As Bit
;       The address of 'trickle' is 0x2D,3
; 28: Dim top_green As Bit
;       The address of 'top_green' is 0x2D,4
; 29: Dim top_red As Bit
;       The address of 'top_red' is 0x2D,5
; 30: Dim ctr_green As Bit
;       The address of 'ctr_green' is 0x2D,6
; 31: Dim ctr_red As Bit
;       The address of 'ctr_red' is 0x2D,7
; 32: Dim bot_green As Bit
;       The address of 'bot_green' is 0x2E,0
; 33: Dim bot_red As Bit
;       The address of 'bot_red' is 0x2E,1
; 34: Dim leds_blanked As Bit
;       The address of 'leds_blanked' is 0x2E,2
; 35: Dim alarm As Bit
;       The address of 'alarm' is 0x2E,3
; 36: Dim alarm_on As Bit
;       The address of 'alarm_on' is 0x2E,4
; 37: Dim al As Bit
;       The address of 'al' is 0x2E,5
; 38:  'Dim bov As Bit
; 39: Dim warn As Bit
;       The address of 'warn' is 0x2E,6
; 40: 
; 41:  'volts/6/2.5*1024
; 42: Const ov_psu = 997 '14.6V at PSU -0.6 Diode Drop = 14.0V at Battery
;       The value of 'ov_psu' is 997
; 43: Const uv_psu = 983 '14.4V at PSU -0.6 Diode Drop = 13.8V at Battery
;       The value of 'uv_psu' is 983
; 44: Const ov_batt = 955 '14.0V
;       The value of 'ov_batt' is 955
; 45: Const warn_batt = 750 '11V
;       The value of 'warn_batt' is 750
; 46: Const uv_batt = 683 '10V
;       The value of 'uv_batt' is 683
; 47: Const dead_batt = 341 '5V
;       The value of 'dead_batt' is 341
; 48: 
; 49: ADCON0 = 0
	CLRF 0x1F
; 50: ADCON1 = %01000000
	BSF STATUS,RP0
	MOVLW 0x40
	MOVWF 0x1F
; 51: OSCCON = %01000001 'Internal Osc 1MHz No external signal
	MOVLW 0x41
	MOVWF 0x0F
; 52: ANSEL = %00001111
	BSF STATUS,RP1
	BCF STATUS,RP0
	MOVLW 0x0F
	MOVWF 0x1E
; 53: ANSELH = %00000000
	CLRF 0x1F
; 54: ADCON0 = %11011000
	BCF STATUS,RP1
	MOVLW 0xD8
	MOVWF 0x1F
; 55: 
; 56: TMR0 = 0
	CLRF 0x01
; 57: ASM:        clrwdt
	       clrwdt
; 58: OPTION_REG = %10001100
	BSF STATUS,RP0
	MOVLW 0x8C
	MOVWF 0x01
; 59: 
; 60: x = 0
	BCF STATUS,RP0
	CLRF 0x2F
; 61: y = 0
	CLRF 0x30
; 62: Define ADC_CLOCK = 3
; 63:  'Define ADC_SAMPLEUS = 250
; 64: 
; 65: TRISA.0 = 1 'Battery Voltage
	BSF STATUS,RP0
	BSF 0x05,0
; 66: TRISA.1 = 1 'ADC Reference input
	BSF 0x05,1
; 67: TRISA.2 = 1 'Output voltage
	BSF 0x05,2
; 68: TRISA.4 = 1 'PSU1 Voltage
	BSF 0x05,4
; 69: TRISA.5 = 0 'LED1
	BCF 0x05,5
; 70: 
; 71: TRISB.4 = 0 'LED3
	BCF 0x06,4
; 72: TRISB.5 = 1 'Push button
	BSF 0x06,5
; 73: TRISB.6 = 0 'LED2
	BCF 0x06,6
; 74: TRISB.7 = 0 'Serial out
	BCF 0x06,7
; 75: 
; 76: TRISC.0 = 0 'LED3
	BCF 0x07,0
; 77: TRISC.1 = 0 'Buzzer control
	BCF 0x07,1
; 78: TRISC.2 = 0 'LED2
	BCF 0x07,2
; 79: TRISC.3 = 1 'PSU2 Voltage
	BSF 0x07,3
; 80: TRISC.4 = 0 'LED1
	BCF 0x07,4
; 81: TRISC.5 = 0 'Under Voltage control
	BCF 0x07,5
; 82: TRISC.6 = 0 'Trickle charge control
	BCF 0x07,6
; 83: TRISC.7 = 0 'Over voltage switch control
	BCF 0x07,7
; 84: 
; 85: PORTA.5 = 0
	BCF STATUS,RP0
	BCF 0x05,5
; 86: PORTB.4 = 0
	BCF 0x06,4
; 87: PORTB.6 = 0
	BCF 0x06,6
; 88: PORTB.7 = 0
	BCF 0x06,7
; 89: PORTC.0 = 0
	BCF 0x07,0
; 90: PORTC.1 = 1
	BSF 0x07,1
; 91: PORTC.2 = 0
	BCF 0x07,2
; 92: PORTC.4 = 0
	BCF 0x07,4
; 93: PORTC.5 = 0
	BCF 0x07,5
; 94: PORTC.6 = 0
	BCF 0x07,6
; 95: PORTC.7 = 0
	BCF 0x07,7
; 96: 
; 97: charge = False
	BCF 0x2D,1
; 98: discharge = False
	BCF 0x2D,2
; 99: top_green = False
	BCF 0x2D,4
; 100: top_red = False
	BCF 0x2D,5
; 101: ctr_green = False
	BCF 0x2D,6
; 102: ctr_red = False
	BCF 0x2D,7
; 103: bot_green = False
	BCF 0x2E,0
; 104: bot_red = True
	BSF 0x2E,1
; 105: alarm = False
	BCF 0x2E,3
; 106: alarm_on = True
	BSF 0x2E,4
; 107:  'bov = False
; 108: 
; 109: 
; 110: Gosub led_test
	CALL L0006
; 111:  '___
; 112:  'RS232 Test routine_______________________________________________________
; 113:  'Define SEROUT_DELAYUS = 5000
; 114:  'Serout PORTB.7, 1200, "Hello world!", CrLf
; 115: 
; 116: 
; 117:  '**********************************************************
; 118:  '__________________________________________________________
; 119: start:  'START OF MAIN LOOP
L0001:
; 120: 
; 121: Gosub control
	CALL L0004
; 122: 
; 123:  'Flash heart-beat LED every 64 times round loop
; 124: 
; 125: If alarm = True Then
	BTFSS 0x2E,3
	GOTO L0008
; 126: warn = True
	BSF 0x2E,6
; 127: Endif
L0008:	MOVLW 0x1F
	ANDWF STATUS,F
; 128: 
; 129: If leds_blanked = True And warn = False And x = 0 Then
	BTFSS 0x2E,2
	GOTO L0009
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSC 0x2E,6
	GOTO L0009
	MOVLW 0x1F
	ANDWF STATUS,F
	MOVF 0x2F,W
	SUBLW 0x00
	BTFSS STATUS,Z
	GOTO L0009
; 130: PORTB.6 = 1
	BSF 0x06,6
; 131: WaitMs 10
	MOVLW 0x0A
	MOVWF R0L
	CLRF R0H
	CALL W001
; 132: PORTB.6 = 0
	BCF 0x06,6
; 133: Else
	GOTO L0010
L0009:	MOVLW 0x1F
	ANDWF STATUS,F
; 134: If leds_blanked = True And warn = True And x = 0 Then
	BTFSS 0x2E,2
	GOTO L0011
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSS 0x2E,6
	GOTO L0011
	MOVLW 0x1F
	ANDWF STATUS,F
	MOVF 0x2F,W
	SUBLW 0x00
	BTFSS STATUS,Z
	GOTO L0011
; 135: PORTC.2 = 1
	BSF 0x07,2
; 136: WaitMs 10
	MOVLW 0x0A
	MOVWF R0L
	CLRF R0H
	CALL W001
; 137: PORTC.2 = 0
	BCF 0x07,2
; 138: Endif
L0011:	MOVLW 0x1F
	ANDWF STATUS,F
; 139: Endif
L0010:	MOVLW 0x1F
	ANDWF STATUS,F
; 140: If x < 63 Then
	MOVLW 0x3F
	SUBWF 0x2F,W
	BTFSC STATUS,C
	GOTO L0012
; 141: x = x + 1
	MOVF 0x2F,W
	ADDLW 0x01
	MOVWF 0x2F
; 142: Else
	GOTO L0013
L0012:	MOVLW 0x1F
	ANDWF STATUS,F
; 143: x = 0
	CLRF 0x2F
; 144: Endif
L0013:	MOVLW 0x1F
	ANDWF STATUS,F
; 145: 
; 146: 
; 147: For i = 0 To 9
	CLRF 0x32
L0014:
	MOVF 0x32,W
	SUBLW 0x09
	BTFSS STATUS,C
	GOTO L0015
; 148: 
; 149: Gosub time_on
	CALL L0005
; 150: ASM:        clrwdt
	       clrwdt
; 151: Adcin 0, batvolts
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
	MOVWF 0x35
	MOVF ADRESH,W
	MOVWF 0x36
; 152: Adcin 3, psu2volts
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
	MOVWF 0x39
	MOVF ADRESH,W
	MOVWF 0x3A
; 153: Adcin 7, psu1volts
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
	MOVWF 0x37
	MOVF ADRESH,W
	MOVWF 0x38
; 154: Adcin 2, opvolts
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
	MOVWF 0x3B
	MOVF ADRESH,W
	MOVWF 0x3C
; 155:  '------------------------------------------
; 156: 
; 157: If psu1volts > ov_psu Then 'Over Voltage
	MOVF 0x37,W
	MOVWF R0L
	MOVF 0x38,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xE5
	CALL C003
	BTFSS STATUS,Z
	GOTO L0016
; 158: top_green = True
	BSF 0x2D,4
; 159: top_red = True
	BSF 0x2D,5
; 160: warn = True
	BSF 0x2E,6
; 161: Endif
L0016:	MOVLW 0x1F
	ANDWF STATUS,F
; 162: 
; 163: If psu1volts < ov_psu And psu1volts > uv_psu Then 'Voltage OK
	MOVF 0x37,W
	MOVWF R0L
	MOVF 0x38,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xE5
	CALL C004
	BTFSS STATUS,Z
	GOTO L0017
	MOVLW 0x1F
	ANDWF STATUS,F
	MOVF 0x37,W
	MOVWF R0L
	MOVF 0x38,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xD7
	CALL C003
	BTFSS STATUS,Z
	GOTO L0017
; 164: top_green = True
	BSF 0x2D,4
; 165: top_red = False
	BCF 0x2D,5
; 166: warn = False
	BCF 0x2E,6
; 167: Endif
L0017:	MOVLW 0x1F
	ANDWF STATUS,F
; 168: 
; 169: If psu1volts < uv_psu Then 'Under voltage
	MOVF 0x37,W
	MOVWF R0L
	MOVF 0x38,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xD7
	CALL C004
	BTFSS STATUS,Z
	GOTO L0018
; 170: top_green = False
	BCF 0x2D,4
; 171: top_red = True
	BSF 0x2D,5
; 172: warn = True
	BSF 0x2E,6
; 173: Endif
L0018:	MOVLW 0x1F
	ANDWF STATUS,F
; 174:  '------------------------------------------
; 175: 
; 176: If psu2volts > ov_psu Then 'Over Voltage >14.6
	MOVF 0x39,W
	MOVWF R0L
	MOVF 0x3A,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xE5
	CALL C003
	BTFSS STATUS,Z
	GOTO L0019
; 177: ctr_green = True
	BSF 0x2D,6
; 178: ctr_red = True
	BSF 0x2D,7
; 179: warn = True
	BSF 0x2E,6
; 180: Endif
L0019:	MOVLW 0x1F
	ANDWF STATUS,F
; 181: 
; 182: If psu2volts < ov_psu And psu2volts > uv_psu Then 'Voltage OK between 14.2V and 14.6
	MOVF 0x39,W
	MOVWF R0L
	MOVF 0x3A,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xE5
	CALL C004
	BTFSS STATUS,Z
	GOTO L0020
	MOVLW 0x1F
	ANDWF STATUS,F
	MOVF 0x39,W
	MOVWF R0L
	MOVF 0x3A,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xD7
	CALL C003
	BTFSS STATUS,Z
	GOTO L0020
; 183: ctr_green = True
	BSF 0x2D,6
; 184: ctr_red = False
	BCF 0x2D,7
; 185: warn = False
	BCF 0x2E,6
; 186: Endif
L0020:	MOVLW 0x1F
	ANDWF STATUS,F
; 187: 
; 188: If psu2volts < uv_psu Then 'Under voltage <14.2V
	MOVF 0x39,W
	MOVWF R0L
	MOVF 0x3A,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xD7
	CALL C004
	BTFSS STATUS,Z
	GOTO L0021
; 189: ctr_green = False
	BCF 0x2D,6
; 190: ctr_red = True
	BSF 0x2D,7
; 191: warn = True
	BSF 0x2E,6
; 192: Endif
L0021:	MOVLW 0x1F
	ANDWF STATUS,F
; 193: 
; 194: Next i
	MOVLW 0x01
	ADDWF 0x32,F
	BTFSS STATUS,C
	GOTO L0014
L0015:	MOVLW 0x1F
	ANDWF STATUS,F
; 195:  '------------------------------------------
; 196: 
; 197: Gosub check_bat_present
	CALL L0002
; 198: If nobat = True Then
	BTFSS 0x2C,6
	GOTO L0022
; 199: alarm = True
	BSF 0x2E,3
; 200: Goto start
	GOTO L0001
; 201: Endif
L0022:	MOVLW 0x1F
	ANDWF STATUS,F
; 202:  '------------------------------------------
; 203: 
; 204: For i = 0 To 9
	CLRF 0x32
L0023:
	MOVF 0x32,W
	SUBLW 0x09
	BTFSS STATUS,C
	GOTO L0024
; 205: ASM:        clrwdt
	       clrwdt
; 206: Gosub time_on
	CALL L0005
; 207: 
; 208: If batvolts > ov_psu Then 'Over Voltage >13.9 at battery
	MOVF 0x35,W
	MOVWF R0L
	MOVF 0x36,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xE5
	CALL C003
	BTFSS STATUS,Z
	GOTO L0025
; 209: bot_green = True
	BSF 0x2E,0
; 210: bot_red = True
	BSF 0x2E,1
; 211: charge = False
	BCF 0x2D,1
; 212: alarm = True
	BSF 0x2E,3
; 213: Endif
L0025:	MOVLW 0x1F
	ANDWF STATUS,F
; 214: 
; 215: If batvolts < ov_batt And batvolts > uv_batt Then 'Voltage OK between 10V and 14V
	MOVF 0x35,W
	MOVWF R0L
	MOVF 0x36,W
	MOVWF R0H
	MOVLW 0x03
	MOVWF R1H
	MOVLW 0xBB
	CALL C004
	BTFSS STATUS,Z
	GOTO L0026
	MOVLW 0x1F
	ANDWF STATUS,F
	MOVF 0x35,W
	MOVWF R0L
	MOVF 0x36,W
	MOVWF R0H
	MOVLW 0x02
	MOVWF R1H
	MOVLW 0xAB
	CALL C003
	BTFSS STATUS,Z
	GOTO L0026
; 216: bot_green = True
	BSF 0x2E,0
; 217: bot_red = False
	BCF 0x2E,1
; 218: charge = True
	BSF 0x2D,1
; 219: discharge = True
	BSF 0x2D,2
; 220: alarm = False
	BCF 0x2E,3
; 221: alarm_on = True
	BSF 0x2E,4
; 222: Endif
L0026:	MOVLW 0x1F
	ANDWF STATUS,F
; 223: 
; 224: If batvolts < warn_batt Then 'Under voltage <11V
	MOVF 0x35,W
	MOVWF R0L
	MOVF 0x36,W
	MOVWF R0H
	MOVLW 0x02
	MOVWF R1H
	MOVLW 0xEE
	CALL C004
	BTFSS STATUS,Z
	GOTO L0027
; 225: alarm = True
	BSF 0x2E,3
; 226: Endif
L0027:	MOVLW 0x1F
	ANDWF STATUS,F
; 227: 
; 228: If batvolts < uv_batt Then 'Under voltage <10V
	MOVF 0x35,W
	MOVWF R0L
	MOVF 0x36,W
	MOVWF R0H
	MOVLW 0x02
	MOVWF R1H
	MOVLW 0xAB
	CALL C004
	BTFSS STATUS,Z
	GOTO L0028
; 229: discharge = False
	BCF 0x2D,2
; 230: bot_green = False
	BCF 0x2E,0
; 231: bot_red = True
	BSF 0x2E,1
; 232: trickle = True
	BSF 0x2D,3
; 233: Endif
L0028:	MOVLW 0x1F
	ANDWF STATUS,F
; 234: 
; 235: Next i
	MOVLW 0x01
	ADDWF 0x32,F
	BTFSS STATUS,C
	GOTO L0023
L0024:	MOVLW 0x1F
	ANDWF STATUS,F
; 236:  '------------------------------------------
; 237: 
; 238: Goto start
	GOTO L0001
; 239: End
L0029:	GOTO L0029
; 240: check_bat_present: 
L0002:
; 241: 
; 242: PORTC.6 = 0 'Charge off
	BCF 0x07,6
; 243: PORTC.7 = 0 'Trickle off
	BCF 0x07,7
; 244: WaitMs 1
	MOVLW 0x01
	MOVWF R0L
	CLRF R0H
	CALL W001
; 245: Adcin 0, batvolts
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
	MOVWF 0x35
	MOVF ADRESH,W
	MOVWF 0x36
; 246: If batvolts < dead_batt Then '<5V
	MOVF 0x35,W
	MOVWF R0L
	MOVF 0x36,W
	MOVWF R0H
	MOVLW 0x01
	MOVWF R1H
	MOVLW 0x55
	CALL C004
	BTFSS STATUS,Z
	GOTO L0030
; 247: nobat = True
	BSF 0x2C,6
; 248: charge = False
	BCF 0x2D,1
; 249: trickle = False
	BCF 0x2D,3
; 250: bot_green = False
	BCF 0x2E,0
; 251: bot_red = True
	BSF 0x2E,1
; 252: Else
	GOTO L0031
L0030:	MOVLW 0x1F
	ANDWF STATUS,F
; 253: nobat = False
	BCF 0x2C,6
; 254: Endif
L0031:	MOVLW 0x1F
	ANDWF STATUS,F
; 255: Gosub control
	CALL L0004
; 256: WaitUs 100
	MOVLW 0x64
	MOVWF R4L
	CALL X001
; 257: Return
	RETURN
; 258: display: 
L0003:
; 259: If leds_blanked = True Then
	BTFSS 0x2E,2
	GOTO L0032
; 260: PORTA.5 = 0
	BCF 0x05,5
; 261: PORTC.4 = 0
	BCF 0x07,4
; 262: PORTB.6 = 0
	BCF 0x06,6
; 263: PORTC.2 = 0
	BCF 0x07,2
; 264: PORTB.4 = 0
	BCF 0x06,4
; 265: PORTC.0 = 0
	BCF 0x07,0
; 266: Else
	GOTO L0033
L0032:	MOVLW 0x1F
	ANDWF STATUS,F
; 267: PORTB.6 = top_green
	BTFSC 0x2D,4
	BSF 0x06,6
	BTFSS 0x2D,4
	BCF 0x06,6
; 268: PORTC.2 = top_red
	BTFSC 0x2D,5
	BSF 0x07,2
	BTFSS 0x2D,5
	BCF 0x07,2
; 269: PORTC.4 = ctr_green
	BTFSC 0x2D,6
	BSF 0x07,4
	BTFSS 0x2D,6
	BCF 0x07,4
; 270: PORTA.5 = ctr_red
	BTFSC 0x2D,7
	BSF 0x05,5
	BTFSS 0x2D,7
	BCF 0x05,5
; 271: PORTC.0 = bot_green
	BTFSC 0x2E,0
	BSF 0x07,0
	BTFSS 0x2E,0
	BCF 0x07,0
; 272: PORTB.4 = bot_red
	BTFSC 0x2E,1
	BSF 0x06,4
	BTFSS 0x2E,1
	BCF 0x06,4
; 273: 
; 274:  'Flash the bottom LED if battery below 5V
; 275: If yy >= 7 And nobat = True Then
	MOVLW 0x07
	SUBWF 0x31,W
	BTFSS STATUS,C
	GOTO L0034
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSS 0x2C,6
	GOTO L0034
; 276: PORTB.4 = 1
	BSF 0x06,4
; 277: Endif
L0034:	MOVLW 0x1F
	ANDWF STATUS,F
; 278: If yy < 7 And nobat = True Then
	MOVLW 0x07
	SUBWF 0x31,W
	BTFSC STATUS,C
	GOTO L0035
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSS 0x2C,6
	GOTO L0035
; 279: PORTB.4 = 0
	BCF 0x06,4
; 280: Endif
L0035:	MOVLW 0x1F
	ANDWF STATUS,F
; 281: 
; 282: If yy < 15 Then
	MOVLW 0x0F
	SUBWF 0x31,W
	BTFSC STATUS,C
	GOTO L0036
; 283: yy = yy + 1
	MOVF 0x31,W
	ADDLW 0x01
	MOVWF 0x31
; 284: Else
	GOTO L0037
L0036:	MOVLW 0x1F
	ANDWF STATUS,F
; 285: yy = 0
	CLRF 0x31
; 286: Endif
L0037:	MOVLW 0x1F
	ANDWF STATUS,F
; 287: 
; 288: 
; 289: 
; 290: Endif
L0033:	MOVLW 0x1F
	ANDWF STATUS,F
; 291: Return
	RETURN
; 292: 
; 293: control: 
L0004:
; 294: PORTC.7 = charge
	BTFSC 0x2D,1
	BSF 0x07,7
	BTFSS 0x2D,1
	BCF 0x07,7
; 295: PORTC.5 = discharge
	BTFSC 0x2D,2
	BSF 0x07,5
	BTFSS 0x2D,2
	BCF 0x07,5
; 296: PORTC.6 = trickle
	BTFSC 0x2D,3
	BSF 0x07,6
	BTFSS 0x2D,3
	BCF 0x07,6
; 297: Gosub display
	CALL L0003
; 298: button = Not PORTB.5
	BTFSS 0x06,5
	BSF 0x2D,0
	BTFSC 0x06,5
	BCF 0x2D,0
; 299: 
; 300: al = alarm_on And alarm
	CLRW
	BTFSS 0x2E,4
	ADDLW 0x01
	BTFSS 0x2E,3
	ADDLW 0x01
	BTFSC STATUS,Z
	BSF 0x2E,5
	BTFSS STATUS,Z
	BCF 0x2E,5
; 301: 
; 302:  'Pulse the buzzer
; 303: If y >= 12 And al = True Then
	MOVLW 0x0C
	SUBWF 0x30,W
	BTFSS STATUS,C
	GOTO L0038
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSS 0x2E,5
	GOTO L0038
; 304: PORTC.1 = 1
	BSF 0x07,1
; 305: Endif
L0038:	MOVLW 0x1F
	ANDWF STATUS,F
; 306: If y < 12 And al = True Then
	MOVLW 0x0C
	SUBWF 0x30,W
	BTFSC STATUS,C
	GOTO L0039
	MOVLW 0x1F
	ANDWF STATUS,F
	BTFSS 0x2E,5
	GOTO L0039
; 307: PORTC.1 = 0
	BCF 0x07,1
; 308: Endif
L0039:	MOVLW 0x1F
	ANDWF STATUS,F
; 309: 
; 310: If y < 15 Then
	MOVLW 0x0F
	SUBWF 0x30,W
	BTFSC STATUS,C
	GOTO L0040
; 311: y = y + 1
	MOVF 0x30,W
	ADDLW 0x01
	MOVWF 0x30
; 312: Else
	GOTO L0041
L0040:	MOVLW 0x1F
	ANDWF STATUS,F
; 313: y = 0
	CLRF 0x30
; 314: Endif
L0041:	MOVLW 0x1F
	ANDWF STATUS,F
; 315: 
; 316: If al = False Then PORTC.1 = 1
	BTFSC 0x2E,5
	GOTO L0042
	BSF 0x07,1
L0042:	MOVLW 0x1F
	ANDWF STATUS,F
; 317: 
; 318: 
; 319: Return
	RETURN
; 320: 
; 321: time_on: 
L0005:
; 322: If PORTB.5 = 0 Then
	BTFSC 0x06,5
	GOTO L0043
; 323: tim = 65535
	MOVLW 0xFF
	MOVWF 0x33
	MOVLW 0xFF
	MOVWF 0x34
; 324: leds_blanked = False
	BCF 0x2E,2
; 325: alarm_on = False
	BCF 0x2E,4
; 326: Endif
L0043:	MOVLW 0x1F
	ANDWF STATUS,F
; 327: tim = tim - 1
	MOVLW 0x01
	SUBWF 0x33,W
	MOVWF 0x33
	MOVLW 0x00
	BTFSS STATUS,C
	ADDLW 0x01
	SUBWF 0x34,W
	MOVWF 0x34
; 328: If tim = 0 Then
	MOVF 0x33,W
	MOVWF R0L
	MOVF 0x34,W
	MOVWF R0H
	CLRF R1H
	MOVLW 0x00
	CALL C001
	BTFSS STATUS,Z
	GOTO L0044
; 329: leds_blanked = True
	BSF 0x2E,2
; 330:  'alarm_on = True
; 331: Endif
L0044:	MOVLW 0x1F
	ANDWF STATUS,F
; 332: Return
	RETURN
; 333: 
; 334: led_test: 
L0006:
; 335: ASM:        clrwdt
	       clrwdt
; 336: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x32
L0045:
	MOVF 0x32,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0046
; 337: PORTA.5 = 1 'Red
	BSF 0x05,5
; 338: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 339: PORTA.5 = 0
	BCF 0x05,5
; 340: Next i
	MOVLW 0x01
	ADDWF 0x32,F
	BTFSS STATUS,C
	GOTO L0045
L0046:	MOVLW 0x1F
	ANDWF STATUS,F
; 341: ASM:        clrwdt
	       clrwdt
; 342: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x32
L0047:
	MOVF 0x32,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0048
; 343: PORTC.4 = 1 'Green
	BSF 0x07,4
; 344: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 345: PORTC.4 = 0
	BCF 0x07,4
; 346: Next i
	MOVLW 0x01
	ADDWF 0x32,F
	BTFSS STATUS,C
	GOTO L0047
L0048:	MOVLW 0x1F
	ANDWF STATUS,F
; 347: ASM:        clrwdt
	       clrwdt
; 348: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x32
L0049:
	MOVF 0x32,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0050
; 349: PORTB.6 = 1 'Green
	BSF 0x06,6
; 350: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 351: PORTB.6 = 0
	BCF 0x06,6
; 352: Next i
	MOVLW 0x01
	ADDWF 0x32,F
	BTFSS STATUS,C
	GOTO L0049
L0050:	MOVLW 0x1F
	ANDWF STATUS,F
; 353: ASM:        clrwdt
	       clrwdt
; 354: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x32
L0051:
	MOVF 0x32,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0052
; 355: PORTC.2 = 1 'Red
	BSF 0x07,2
; 356: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 357: PORTC.2 = 0
	BCF 0x07,2
; 358: Next i
	MOVLW 0x01
	ADDWF 0x32,F
	BTFSS STATUS,C
	GOTO L0051
L0052:	MOVLW 0x1F
	ANDWF STATUS,F
; 359: ASM:        clrwdt
	       clrwdt
; 360: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x32
L0053:
	MOVF 0x32,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0054
; 361: PORTB.4 = 1 'Red
	BSF 0x06,4
; 362: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 363: PORTB.4 = 0
	BCF 0x06,4
; 364: Next i
	MOVLW 0x01
	ADDWF 0x32,F
	BTFSS STATUS,C
	GOTO L0053
L0054:	MOVLW 0x1F
	ANDWF STATUS,F
; 365: ASM:        clrwdt
	       clrwdt
; 366: For i = 1 To 10
	MOVLW 0x01
	MOVWF 0x32
L0055:
	MOVF 0x32,W
	SUBLW 0x0A
	BTFSS STATUS,C
	GOTO L0056
; 367: PORTC.0 = 1 'Green
	BSF 0x07,0
; 368: WaitMs 20
	MOVLW 0x14
	MOVWF R0L
	CLRF R0H
	CALL W001
; 369: PORTC.0 = 0
	BCF 0x07,0
; 370: Next i
	MOVLW 0x01
	ADDWF 0x32,F
	BTFSS STATUS,C
	GOTO L0055
L0056:	MOVLW 0x1F
	ANDWF STATUS,F
; 371: ASM:        clrwdt
	       clrwdt
; 372: For i = 1 To 2
	MOVLW 0x01
	MOVWF 0x32
L0057:
	MOVF 0x32,W
	SUBLW 0x02
	BTFSS STATUS,C
	GOTO L0058
; 373: PORTA.5 = 1
	BSF 0x05,5
; 374: PORTC.4 = 1
	BSF 0x07,4
; 375: PORTB.6 = 1
	BSF 0x06,6
; 376: PORTC.2 = 1
	BSF 0x07,2
; 377: PORTB.4 = 1
	BSF 0x06,4
; 378: PORTC.0 = 1
	BSF 0x07,0
; 379: PORTC.1 = 0
	BCF 0x07,1
; 380: WaitMs 200
	MOVLW 0xC8
	MOVWF R0L
	CLRF R0H
	CALL W001
; 381: ASM:        clrwdt
	       clrwdt
; 382: PORTA.5 = 0
	BCF 0x05,5
; 383: PORTC.4 = 0
	BCF 0x07,4
; 384: PORTB.6 = 0
	BCF 0x06,6
; 385: PORTC.2 = 0
	BCF 0x07,2
; 386: PORTB.4 = 0
	BCF 0x06,4
; 387: PORTC.0 = 0
	BCF 0x07,0
; 388: PORTC.1 = 1
	BSF 0x07,1
; 389: WaitMs 200
	MOVLW 0xC8
	MOVWF R0L
	CLRF R0H
	CALL W001
; 390: ASM:        clrwdt
	       clrwdt
; 391: Next i
	MOVLW 0x01
	ADDWF 0x32,F
	BTFSS STATUS,C
	GOTO L0057
L0058:	MOVLW 0x1F
	ANDWF STATUS,F
; 392: ASM:        clrwdt
	       clrwdt
; 393: WaitMs 200
	MOVLW 0xC8
	MOVWF R0L
	CLRF R0H
	CALL W001
; 394: ASM:        clrwdt
	       clrwdt
; 395: Return
	RETURN
; 396: 
; 397: 
; 398: 
; End of program
L0059:	GOTO L0059
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
	MOVLW 0x03
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
X001:	MOVLW 0x25
	SUBWF R4L,F
	BTFSS STATUS,C
	RETURN
	GOTO X002
X002:	MOVLW 0x18
	SUBWF R4L,F
	BTFSS STATUS,C
	RETURN
	GOTO X002
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
; Configuration word settings
	ORG 0x2007
	DW 0x33BC
; End of listing
	END
