'URPC Controller Firmware

Define CONF_WORD = 0x33bc


Dim x As Byte
Dim y As Byte
Dim yy As Byte
Dim i As Byte
Dim tim As Word
Dim batvolts As Word
Dim psu1volts As Word
Dim psu2volts As Word
Dim opvolts As Word
Dim batt_ov As Word
Dim batov As Bit
Dim batuv As Bit
Dim p1ov As Bit
Dim p1uv As Bit
Dim p2uv As Bit
Dim p2ov As Bit
Dim nobat As Bit
Dim pa7 As Bit
Dim button As Bit
Dim charge As Bit
Dim discharge As Bit
Dim trickle As Bit
Dim top_green As Bit
Dim top_red As Bit
Dim ctr_green As Bit
Dim ctr_red As Bit
Dim bot_green As Bit
Dim bot_red As Bit
Dim leds_blanked As Bit
Dim alarm As Bit
Dim alarm_on As Bit
Dim al As Bit
'Dim bov As Bit
Dim warn As Bit

'volts/6/2.5*1024
Const ov_psu = 997  '14.6V at PSU -0.6 Diode Drop = 14.0V at Battery
Const uv_psu = 983  '14.4V at PSU -0.6 Diode Drop = 13.8V at Battery
Const ov_batt = 955  '14.0V
Const warn_batt = 750  '11V
Const uv_batt = 683  '10V
Const dead_batt = 341  '5V

ADCON0 = 0
ADCON1 = %01000000
OSCCON = %01000001  'Internal Osc 1MHz No external signal
ANSEL = %00001111
ANSELH = %00000000
ADCON0 = %11011000

TMR0 = 0
ASM:        clrwdt
OPTION_REG = %10001100

x = 0
y = 0
Define ADC_CLOCK = 3
'Define ADC_SAMPLEUS = 250

TRISA.0 = 1  'Battery Voltage
TRISA.1 = 1  'ADC Reference input
TRISA.2 = 1  'Output voltage
TRISA.4 = 1  'PSU1 Voltage
TRISA.5 = 0  'LED1

TRISB.4 = 0  'LED3
TRISB.5 = 1  'Push button
TRISB.6 = 0  'LED2
TRISB.7 = 0  'Serial out

TRISC.0 = 0  'LED3
TRISC.1 = 0  'Buzzer control
TRISC.2 = 0  'LED2
TRISC.3 = 1  'PSU2 Voltage
TRISC.4 = 0  'LED1
TRISC.5 = 0  'Under Voltage control
TRISC.6 = 0  'Trickle charge control
TRISC.7 = 0  'Over voltage switch control

PORTA.5 = 0
PORTB.4 = 0
PORTB.6 = 0
PORTB.7 = 0
PORTC.0 = 0
PORTC.1 = 1
PORTC.2 = 0
PORTC.4 = 0
PORTC.5 = 0
PORTC.6 = 0
PORTC.7 = 0

charge = False
discharge = False
top_green = False
top_red = False
ctr_green = False
ctr_red = False
bot_green = False
bot_red = True
alarm = False
alarm_on = True
'bov = False


Gosub led_test
'___
'RS232 Test routine_______________________________________________________
'Define SEROUT_DELAYUS = 5000
'Serout PORTB.7, 1200, "Hello world!", CrLf


'**********************************************************
'__________________________________________________________
start:  'START OF MAIN LOOP

Gosub control

'Flash heart-beat LED every 64 times round loop

If alarm = True Then
	warn = True
Endif

If leds_blanked = True And warn = False And x = 0 Then
	PORTB.6 = 1
	WaitMs 10
	PORTB.6 = 0
Else
	If leds_blanked = True And warn = True And x = 0 Then
		PORTC.2 = 1
		WaitMs 10
		PORTC.2 = 0
	Endif
Endif
If x < 63 Then
	x = x + 1
Else
	x = 0
Endif


For i = 0 To 9

	Gosub time_on
	ASM:        clrwdt
	Adcin 0, batvolts
	Adcin 3, psu2volts
	Adcin 7, psu1volts
	Adcin 2, opvolts
'------------------------------------------

	If psu1volts > ov_psu Then  'Over Voltage
		top_green = True
		top_red = True
		warn = True
	Endif

	If psu1volts < ov_psu And psu1volts > uv_psu Then  'Voltage OK
		top_green = True
		top_red = False
		warn = False
		Endif

	If psu1volts < uv_psu Then  'Under voltage
		top_green = False
		top_red = True
		warn = True
	Endif
'------------------------------------------

	If psu2volts > ov_psu Then  'Over Voltage >14.6
		ctr_green = True
		ctr_red = True
		warn = True
	Endif

	If psu2volts < ov_psu And psu2volts > uv_psu Then  'Voltage OK between 14.2V and 14.6
		ctr_green = True
		ctr_red = False
		warn = False
	Endif

	If psu2volts < uv_psu Then  'Under voltage <14.2V
		ctr_green = False
		ctr_red = True
		warn = True
	Endif

Next i
'------------------------------------------

Gosub check_bat_present
If nobat = True Then
	alarm = True
	Goto start
Endif
'------------------------------------------

For i = 0 To 9
ASM:        clrwdt
Gosub time_on

	If batvolts > ov_psu Then  'Over Voltage >13.9 at battery
		bot_green = True
		bot_red = True
		charge = False
		alarm = True
	Endif

	If batvolts < ov_batt And batvolts > uv_batt Then  'Voltage OK between 10V and 14V
		bot_green = True
		bot_red = False
		charge = True
		discharge = True
		alarm = False
		alarm_on = True
	Endif

	If batvolts < warn_batt Then  'Under voltage <11V
		alarm = True
	Endif

	If batvolts < uv_batt Then  'Under voltage <10V
		discharge = False
		bot_green = False
		bot_red = True
		trickle = True
	Endif

Next i
'------------------------------------------

Goto start
End                                               
check_bat_present:

PORTC.6 = 0  'Charge off
PORTC.7 = 0  'Trickle off
WaitMs 1
Adcin 0, batvolts
If batvolts < dead_batt Then  '<5V
	nobat = True
	charge = False
	trickle = False
	bot_green = False
	bot_red = True
Else
	nobat = False
Endif
Gosub control
WaitUs 100
Return                                            
display:
If leds_blanked = True Then
	PORTA.5 = 0
	PORTC.4 = 0
	PORTB.6 = 0
	PORTC.2 = 0
	PORTB.4 = 0
	PORTC.0 = 0
Else
	PORTB.6 = top_green
	PORTC.2 = top_red
	PORTC.4 = ctr_green
	PORTA.5 = ctr_red
	PORTC.0 = bot_green
	PORTB.4 = bot_red

'Flash the bottom LED if battery below 5V
If yy >= 7 And nobat = True Then
	PORTB.4 = 1
Endif
If yy < 7 And nobat = True Then
	PORTB.4 = 0
Endif

If yy < 15 Then
	yy = yy + 1
Else
	yy = 0
Endif



Endif
Return                                            

control:
PORTC.7 = charge
PORTC.5 = discharge
PORTC.6 = trickle
Gosub display
button = Not PORTB.5

al = alarm_on And alarm

'Pulse the buzzer
If y >= 12 And al = True Then
	PORTC.1 = 1
Endif
If y < 12 And al = True Then
	PORTC.1 = 0
Endif

If y < 15 Then
	y = y + 1
Else
	y = 0
Endif

If al = False Then PORTC.1 = 1


Return                                            

time_on:
If PORTB.5 = 0 Then
	tim = 65535
	leds_blanked = False
	alarm_on = False
Endif
tim = tim - 1
If tim = 0 Then
	leds_blanked = True
	'alarm_on = True
Endif
Return                                            

led_test:
ASM:        clrwdt
For i = 1 To 10
PORTA.5 = 1  'Red
WaitMs 20
PORTA.5 = 0
Next i
ASM:        clrwdt
For i = 1 To 10
PORTC.4 = 1  'Green
WaitMs 20
PORTC.4 = 0
Next i
ASM:        clrwdt
For i = 1 To 10
PORTB.6 = 1  'Green
WaitMs 20
PORTB.6 = 0
Next i
ASM:        clrwdt
For i = 1 To 10
PORTC.2 = 1  'Red
WaitMs 20
PORTC.2 = 0
Next i
ASM:        clrwdt
For i = 1 To 10
PORTB.4 = 1  'Red
WaitMs 20
PORTB.4 = 0
Next i
ASM:        clrwdt
For i = 1 To 10
PORTC.0 = 1  'Green
WaitMs 20
PORTC.0 = 0
Next i
ASM:        clrwdt
For i = 1 To 2
	PORTA.5 = 1
	PORTC.4 = 1
	PORTB.6 = 1
	PORTC.2 = 1
	PORTB.4 = 1
	PORTC.0 = 1
	PORTC.1 = 0
	WaitMs 200
ASM:        clrwdt
	PORTA.5 = 0
	PORTC.4 = 0
	PORTB.6 = 0
	PORTC.2 = 0
	PORTB.4 = 0
	PORTC.0 = 0
	PORTC.1 = 1
	WaitMs 200
ASM:        clrwdt
Next i
ASM:        clrwdt
WaitMs 200
ASM:        clrwdt
Return                                            



