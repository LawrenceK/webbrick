#include "D:\\o2m8\\svn\\WebBrick\\Trunk\\Tester\\Temperature_Auto.h"
#include "Temperature.h"
//
// This file includes all user definable routines. It may be changed at will as
// it will not be regenerated once the application has been generated for the
// first time.
//

//*******************************************************************************
//
// Insert your interrupt handling code if required here. 
// Note quick interrupts are used so code must be simple
// See the manual for details of quick interrupts.
//

void UserInterrupt()
{
	// Insert your code here

 #asmline SETPCLATH UserIntReturn,-1  ; SETPCLATH for interrupt routine
 #asmline goto UserIntReturn	      ; Assembler - go back to interrupt routine
}
const static unsigned char LedMap[] = { 0x02, 0x04, 0x08, 0x80, 0x40, 0x10};
//const static unsigned char LedMap[] = { 0x80, 0x40, 0x02, 0x04, 0x08, 0x10};
static unsigned char idx = 0;
static unsigned char bt = 0;
static unsigned char bts = 0;

//*******************************************************************************
//
// Insert your initialisation code if required here. 
// Note that when this routine is called Interrupts will not be enabled - the 
// Application Designer will enable them before the main loop
//

void UserInitialise()
{
	PORTA = 0;
	PORTB = 0;
//	TRISB &= ~2;
	ResetCheck1W();
	OWDeTagAll();
}

void MSTick()
{
	// Test helper
//	OwTx1WireABit(0);
//	OwTx1WireABit(1);
}

void SecOcc()
{
	unsigned char owsts;
	
	PORTB ^= 0x08;

	OWIdentifySensors();
	for (idx =0; idx<MAX_SENSORS;idx++)
	{
		OWReadSensors( idx );
	}
	
	owsts = OWStatus();
//	PORTA = (owsts<<1);
	bts = 0;
	for (idx =0; idx<MAX_SENSORS;idx++)
	{
		if ( owsts & (0x01<<idx))
		{
			bts |= LedMap[idx];
		}
	}
	PORTA = bts;
}

//*******************************************************************************
//
// Insert your main loop code if required here. This routine will be called
// as part of the main loop code
//

void UserLoop()
{
}

//
// User occurrence code
//
