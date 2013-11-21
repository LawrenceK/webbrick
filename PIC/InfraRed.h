//
//  Copyright L.P.Klyne. 2006
//
//  Filename: InfraRed.h
//
//  Description:
//
//  Notes:
//
//
// Include files

//#include "Clock.h"

// Defines

#define maxRC5Address   0x1F

void configureIr(;
void initIr();

// Exported functions declarations
void doEnableIrRx();
void doDisableIrRx();
void doEnableIrTx();
void doDisableIrTx();
void doSetIrAdr(unsigned char adr );

void sendIr( unsigned char adr, unsigned char chn );

// Fed Occurences
void haveRC5();
// We have seen a falling edge on RC5 receiver, but may not be in use.
void checkRC5();
