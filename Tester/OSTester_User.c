#include "D:\\o2m8\\newsvn\\WebBrick\\Trunk\\Tester\\OSTester_Auto.h"

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


//*******************************************************************************
//
// Insert your initialisation code if required here. 
// Note that when this routine is called Interrupts will not be enabled - the 
// Application Designer will enable them before the main loop
//

void UserInitialise()
{
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
