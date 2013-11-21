//
//  Copyright L.P.Klyne 2013
//
//  Filename: Template.h
//
//  Description:    System wide Global data.
//
//  Notes:
//
//
// Include files

#include "Webrick_Auto.h"

// Defines

// Static data (Global scope)
//#define wb_version(maj,min,build) const char *wb_version = "Firmware version" #maj "." #min "." #build
//wb_version(DF_SW_VER_MAJOR,DF_SW_VER_MINOR,DF_SW_VER_BUILD);
//#define wb_version() const char *wb_version = "Firmware version" #DF_SW_VER_MAJOR "." #DF_SW_VER_MINOR "." #DF_SW_VER_BUILD
//wb_version();

// Operational State
unsigned char OperState = 0;				// Start up in frozen state
unsigned char RBehaviour = 0 ;				// char that controls random behaviour

unsigned char RotStep;					// Rotary Encoder step per indent
unsigned SetPoints[SPCount];// = {0,128,256,512,768,1023,0,0} ;		// SetPoints for Analogue

unsigned char MyNodeNr;					// My NodeNumber

unsigned char FadeRate = DF_FADERATE;			// used to conrol fade rates, means Fade Every FadeRate millisconds
unsigned char MimicFadeRate = DF_FADERATE;

unsigned DwellTimes[DwellCount];		// Array of Dwell times

unsigned char LoggedInState = 0;
unsigned int LoggedInTimer = 0;    // seconds.

unsigned char Options[MaxOptions] = { 0, DF_UDPOption };

unsigned char myMac[6];

int TempLowThreshold[TempCount];
int TempHighThreshold[TempCount];

unsigned char AnLowThreshold[AnOutEntryCount];	// analogue low thresholds
unsigned char AnHighThreshold[AnOutEntryCount];

// used in decoding triggers
unsigned char chnType;
unsigned char tgtChn;
unsigned char action;
//unsigned char dwellNr;
unsigned dwellValue;    // A value in seconds between 1 and 32767
unsigned char setPointNr;
unsigned char udpDo;
char assocValue1;
unsigned char assocValue2;
unsigned char b1Work;
unsigned char b2Work;
