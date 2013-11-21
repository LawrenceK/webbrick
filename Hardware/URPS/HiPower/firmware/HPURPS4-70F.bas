Define CONF_WORD = 0x330c

'HP-URPS Controller Firmware
Dim x As Byte
Dim y As Byte
Dim i As Byte
Dim tim As Word
Dim batvolts As Word
Dim psu1volts As Word
Dim psu2volts As Word
Dim opvolts As Word
Dim batov As Bit
Dim batuv As Bit
Dim nobat As Bit
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
Dim palarm1 As Bit  'psu alarm
Dim palarm2 As Bit  'psu alarm
Dim balarm As Bit  'Battery Alarm
Dim alarm_on As Bit
Dim al As Bit
Dim alarm_byte As Byte
Dim bov As Bit
Dim out_ctrl As Bit
Dim p1v As Word
Dim p1vl As Byte
Dim p2v As Word
Dim p2vl As Byte
Dim ov As Word
Dim ovl As Byte
Dim bv As Word
Dim bvl As Byte
Dim uv_psuv As Word
Dim opalarm As Bit
Dim opv_ok As Bit
Dim volts As Word
Dim ignore As Bit
Dim ae As Byte

'Voltage Constants x 68.33
Const ov_psu = 963  '14.1
Const uv_psuadj = 950  '13.9
Const uv_psurun = 915  '13.4
Const batt_ov = 965  '14.0V
Const sw_out_off = 806  '11.8V
Const sw_out_on = 854  '12.5V
Const warn_batt = 750  '11.0V
Const uv_batt = 683  '10.0V
Const dead_batt = 341  '5.0V

ADCON0 = %11011000
ADCON1 = %01000000
OSCCON = %01000001  'Internal Osc 1MHz No external signal ***Ensure compiler set to 1MHz
ANSEL = %10001001
ANSELH = %00001000
TMR0 = 0
OPTION_REG = %10001110
Define ADC_CLOCK = 3
Define SEROUT_DELAYUS = 2000
'Define ADC_SAMPLEUS = 250

ASM:        clrwdt

'Define ports
TRISA.0 = 1  'Battery Voltage
TRISA.1 = 1  'ADC Reference input
TRISA.2 = 0  'Switched output control
TRISA.3 = 1  'Push button
TRISA.4 = 1  'PSU1 Voltage
TRISA.5 = 0  'LED1

TRISB.4 = 0  'LED3
TRISB.5 = 1  'Output voltage
TRISB.6 = 0  'LED2
TRISB.7 = 0  'Serial out

TRISC.0 = 0  'LED3
TRISC.1 = 0  'Buzzer control
TRISC.2 = 0  'LED2
TRISC.3 = 1  'PSU2 Voltage
TRISC.4 = 0  'LED1
TRISC.5 = 0  'Under-voltage control
TRISC.6 = 0  'Trickle charge control
TRISC.7 = 0  'Over-voltage switch control

'Initialise ports
PORTA.5 = 0
PORTA.5 = 0
PORTB.4 = 0
PORTB.6 = 0
PORTB.7 = 0
PORTC.0 = 0
PORTC.1 = 0
PORTC.2 = 0
PORTC.4 = 0
PORTC.5 = 0
PORTC.6 = 0
PORTC.7 = 0

'Initialise variables
charge = False
discharge = False
top_green = False
top_red = False
ctr_green = False
ctr_red = False
bot_green = False
bot_red = False
al = False
alarm_on = True
bov = False
trickle = True
out_ctrl = True
tim = 1
balarm = False
palarm1 = False
palarm2 = False
x = 0
y = 0
ignore = False
ae = 0

Gosub led_test
'__________________________________________________________


'**********************************************************
'__________________________________________________________
start:  'START OF MAIN LOOP

Gosub control
ASM:        clrwdt
'flash heart-beat LED every 64 times round loop
If leds_blanked = True And al = False And x = 0 Then
	PORTB.6 = 1
	WaitMs 10
	PORTB.6 = 0
Else
	If leds_blanked = True And al = True And x = 0 Then
		PORTC.2 = 1
		WaitMs 10
		PORTC.2 = 0
	Endif
Endif

'Create Serial data bytes and send them every 64 times around the loop
If x = 0 Then
	p1v = psu1volts * 8
	p1vl = p1v.LB / 8

	p2v = psu2volts * 8
	p2vl = p2v.LB / 8

	bv = batvolts * 8
	bvl = bv.LB / 8

	ov = opvolts * 8
	ovl = ov.LB / 8

'Start byte 0xFF
'all data words shifted so high and low bytes each contain 5 bits both right justified
	Serout PORTB.7, 2400, 0xff, p1v.HB, p1vl, p2v.HB, p2vl, bv.HB, bvl, ov.HB, ovl, alarm_byte

Endif

If x < 63 Then
	x = x + 1
Else
	x = 0
Endif

	Gosub time_on
ASM:        clrwdt

volts = 0
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	psu2volts = volts / 8  'Take 8 readings and average them

volts = 0
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	psu1volts = volts / 8  'Take 8 readings and average them

	Adcin 2, opvolts

ASM:        clrwdt
'PSU adjustment routine
If PORTA.3 = 0 Then

	Gosub time_on

	uv_psuv = uv_psuadj

	adjustpsus:
volts = 0
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	WaitMs 1
	Adcin 3, psu2volts
	volts = volts + psu2volts
	psu2volts = volts / 8  'Take 8 readings and average them

volts = 0
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	WaitMs 1
	Adcin 7, psu1volts
	volts = volts + psu1volts
	psu1volts = volts / 8  'Take 8 readings and average them
	If psu1volts > ov_psu Then  'Over Voltage
		top_green = True
		top_red = True
	Endif
	If psu1volts < ov_psu And psu1volts > uv_psuv Then  'Voltage OK
		top_green = True
		top_red = False
	Endif
	If psu1volts < uv_psuv Then  'Under voltage
		top_green = False
		top_red = True
	Endif
	If psu2volts > ov_psu Then  'Over Voltage
		ctr_green = True
		ctr_red = True
	Endif
	If psu2volts < ov_psu And psu2volts > uv_psuv Then  'Voltage OK
		ctr_green = True
		ctr_red = False
	Endif
	If psu2volts < uv_psuv Then  'Under voltage
		ctr_green = False
		ctr_red = True
	Endif
	Gosub display
ASM:        clrwdt
	If PORTA.3 = 0 Then
		Goto adjustpsus
	Else
		uv_psuv = uv_psurun
		WaitMs 50
	Endif
Endif
'------------------------------------------
'Mains PSU monitor  - 1
	If psu1volts > ov_psu Then  'Over Voltage
		top_green = True
		top_red = True
	Endif

	If psu1volts < ov_psu And psu1volts > uv_psuv Then  'Voltage OK
		top_green = True
		top_red = False
	If palarm1 = True Then alarm_on = True
		palarm1 = False
	Endif

	If psu1volts < uv_psuv Then  'Under voltage
		top_green = False
		top_red = True
		palarm1 = True
	Endif

'------------------------------------------
'Mains PSU monitor  - 2
	If psu2volts > ov_psu Then  'Over Voltage
		ctr_green = True
		ctr_red = True
	Endif

	If psu2volts < ov_psu And psu2volts > uv_psuv Then  'Voltage OK
		ctr_green = True
		ctr_red = False
		If palarm2 = True Then alarm_on = True
		palarm2 = False
	Endif

	If psu2volts < uv_psuv Then  'Under voltage
		ctr_green = False
		ctr_red = True
		palarm2 = True
	Endif

'---------------
'Battery monitor

Gosub check_bat_present
If nobat = True Then
	balarm = True
	Goto start
Endif

For i = 0 To 9

ASM:        clrwdt
	Gosub time_on

	If batvolts > ov_psu Then  'Over Voltage > at battery
		bot_green = True
		bot_red = True
		charge = False
		trickle = False
	Endif

	If batvolts < ov_psu And batvolts > warn_batt Then  'Voltage OK between 11V and 14V
		bot_green = True
		bot_red = False
		discharge = True
		out_ctrl = True
		If balarm = True Then
			alarm_on = True
			balarm = False
		Endif
	Endif

	If batvolts < ov_psu And batvolts > uv_batt Then
		trickle = True
		charge = True
	Endif

	If batvolts < sw_out_off Then  'Under voltage <11.8V
		out_ctrl = False
	Endif

	If ctr_green = True Or top_green = True Then  'Mains power restored from either PSU
		out_ctrl = True
	Endif

	If batvolts < warn_batt Then  'Under voltage <11V
		balarm = True
	Endif

	If batvolts < uv_batt Then  'Under voltage <10V
		charge = False
		discharge = False
		bot_green = False
		bot_red = True
		trickle = True
	Endif

Next i
Goto start
End                                               
'Check if a battery is connected or is dead

check_bat_present:

'Turn off charging circuit to isolate the battery input
PORTC.6 = 0  'Trickle off
PORTC.7 = 0  'Charge off
ASM:        clrwdt
WaitMs 3
ASM:        clrwdt

Adcin 0, batvolts  'Measure battery voltage
If batvolts < dead_batt Then  'if battery <5V turn off the battery circuit
	nobat = True
	charge = False
	trickle = False
	bot_green = False
	bot_red = True
	balarm = True
Else
	nobat = False
Endif
Gosub control
WaitUs 100
Return                                            

display:  'Decide which LEDS are on

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
Endif
Return                                            

control:  'Output the controls to MOSFETs and Buzzer

PORTC.7 = charge
PORTC.5 = discharge
PORTC.6 = trickle
PORTA.2 = out_ctrl
Gosub display

al = False
al = palarm1 Or palarm2
al = al Or balarm

'Pulse the buzzer
If alarm_on = True Then
	If y >= 12 And al = True Then
		PORTC.1 = 0
	Endif
	If y < 12 And al = True Then
		PORTC.1 = 1
	Endif
	
	If y < 15 Then
		y = y + 1
	Else
		y = 0
	Endif
Endif
If al = False Then PORTC.1 = 0  'ensure alarm always turns off
If alarm_on = False Then PORTC.1 = 0


'Build alarm Byte
alarm_byte.0 = Not charge
alarm_byte.1 = Not discharge
alarm_byte.2 = Not trickle
alarm_byte.3 = Not out_ctrl  'battery < 11.8v
alarm_byte.4 = Not nobat
If top_red = 1 Or ctr_red = 1 Then alarm_byte.5 = 1  'psu failure
alarm_byte.6 = bot_red  'battery < 10V
alarm_byte.7 = 0  'must be zero to avoid confusion with start byte (hFF)
Return                                            

time_on:  'Timer for LED display

If PORTA.3 = 0 Then  'Button pressed
	tim = 65535  'Loop counter 2min
	leds_blanked = False  'Disable LED display
	If al = True Then
		alarm_on = False
	Endif

Endif

tim = tim - 1
If tim = 0 Then
	leds_blanked = True
Endif

Return                                            



led_test:  'Start-up Display
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
'ASM:        clrwdt
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
	PORTC.1 = 1
	WaitMs 200
ASM:        clrwdt
	PORTA.5 = 0
	PORTC.4 = 0
	PORTB.6 = 0
	PORTC.2 = 0
	PORTB.4 = 0
	PORTC.0 = 0
	PORTC.1 = 0
	WaitMs 200
ASM:        clrwdt
Next i
ASM:        clrwdt
WaitMs 200
ASM:        clrwdt
Return                                            



