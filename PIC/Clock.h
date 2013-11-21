//
//  Copyright L.P.Klyne 2013
//
//  Filename: Clock.h
//
//  Description:
//
//  Notes:
//
//
// Include files

//#include "Clock.h"

// Defines

// Static data (Global scope)
extern int clockSet;

// Exported functions declarations
void SetClock( unsigned char Dy, unsigned char Hr, unsigned char Mn, unsigned char Sc );
void SetDate( unsigned char Yr, unsigned char Mn, unsigned char Dt );

void SendClockTime();   // to siteplayer.

// Fed Occurences
void ckSecOcc();
void ckMinuteOcc();
void ckHourOcc();
void ckDayOcc();

//void tryRtcRead();

