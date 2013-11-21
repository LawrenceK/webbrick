//
//  Copyright L.P.Klyne 2013
//
//  Filename: Persist.h
//
//  Description:This module contains the persitant storage functions, i.e. Read and Write EEPROM
//
//  Notes:
//
//
// Include files

//#include "Persist.h"

// Defines

// These two are for othoganality and so we can change persistance later if we wish.
// Write byte to EEProm
#define EEWriteByte( location, theByte ) WriteEEData( location, theByte )
//void EEWriteByte(unsigned location, unsigned char theByte);

// Read byte from EEProm
#define EEReadByte(location) (ReadEEData( location ))
//unsigned char EEReadByte(unsigned location);

// Static data (Global scope)

// Exported functions declarations

// Write Int to EEProm
void EEWriteInt (unsigned location, int theInt);

// Read Int from EEProm
int EEReadInt (unsigned location);

// Write String to EEProm
// location
// maxLen is maaximum number of bytes to write.
void EEWriteStr (unsigned location, unsigned char *theStr, unsigned char maxLen);

// Read String from EEProm
// bufLen is size of buffer at theStr.
void EEReadStr(unsigned location, unsigned char *theStr, unsigned char bufLen);	

// Write Buffer to EEProm
// location
// maxLen is maaximum number of bytes to write.
void EEWriteBuffer(unsigned location, unsigned char *buf, unsigned char len);

// Read Buffer from EEProm
// bufLen is size of buffer at theStr.
void EEReadBuffer(unsigned location, unsigned char *buf, unsigned char len);

void ResetConfig();     // reset configuration data only
void FactoryReset();    // resets everything
