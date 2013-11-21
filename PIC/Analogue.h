//
//  Copyright L.P.Klyne 2013
//
//  Filename: Analogue.h
//
//  Description:
//
//  Notes:
//
//
// Include files

//#include "template.h"

// Defines
// Convert analog values between percent (1 byte) and 10 bit binary.
// +255 to round percent to nearest
#define ANVALTOPERCENT( val ) ( (unsigned char)( ( (val*50)+255)/511 ))
#define ANPERCENTTOVAL( val ) ( ( ((unsigned)val*511) + 25 )/50 )


#define ANSTARTCONVERT() {bGO_DONE = 1;}
//#define ANSTARTCONVERT() {ADCON0|=(1<<GO);}
#define AN_BUSY (bGO_DONE == 1)
#define AN_FREE (bGO_DONE == 0)

// Static data (Global scope)
extern unsigned char mimicForAnOut[(AnOutEntryCount+1)/2];
extern unsigned char AnInVals[AnInEntryCount];
extern unsigned AnOutTarget[AnOutEntryCount];

// Exported functions declarations

void AnalogueInit();    // initialise data
void AnaAdjustThreshold(unsigned char chn, unsigned char action, char value);
void AnalogueInc(unsigned char chn, unsigned char step);    // action for rotary encoder
void AnalogueDec(unsigned char chn, unsigned char step);    // action for rotary encoder
void AnaOut(unsigned char chn, unsigned value) ;	    // controls target values of analogue channels
void AnaFade() ;					    // fades analogue channels
void AnaloguePeriodic();
void AnaCommand(unsigned char chn, unsigned char action, unsigned char spNum );

#ifdef USE_MIMICS
void AnalogueMimicInit();
#endif

// FED occurence
void GetAnIn();							// Get Analogue Input
