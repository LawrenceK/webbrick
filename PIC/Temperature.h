//
//  Copyright L.P.Klyne 2013
//
//  Filename: Temperature.h
//
//  Description:
//
//  Notes:
//
//
// Include files

//#include "template.h"

void TempInit();    // initialise data

void TempAdjustThreshold(unsigned char chn, unsigned char action, int value);

//void TempFindSensors();
#define TempFindSensors() OWIdentifySensors()

void TempReadSensors( unsigned char chn );

void TemperaturePeriodic();
