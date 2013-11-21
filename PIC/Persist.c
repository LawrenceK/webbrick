//
//  Copyright L.P.Klyne 2013
//
//  Filename: Persist.c
//
//  Description:    This module contains the persitant storage functions, i.e. Read and Write EEPROM
//
//  Notes:
//
//
// Include files
#include "Webrick_Auto.h"

// Local Defines

// Local forwards

// Static data (File scope)

// Global functions
void EEWriteStr (unsigned location, unsigned char *theStr, unsigned char maxLen )
{
    //
    //  Write a string into EEPROM with a maximum length of 16
    //  to prevent run away over writing
    //
    unsigned idx = 0;
    do {
        EEWriteByte( location+idx, theStr[idx] ) ;
        idx++;  // now this is count of bytes written
    }
    while (    ( idx < maxLen )
            && ( theStr[idx] != 0 ) );
    if ( idx < maxLen )
    {
        // we skip from loop before writing the null byte.
        EEWriteByte( location+idx, 0 );
    }
}

void EEReadStr( unsigned location, unsigned char *theStr, unsigned char bufLen)
{
    //  Return a String from EEProm location
    unsigned idx = 0;
    do {
        theStr[idx] = EEReadByte( location+idx ) ;
        ++idx;
    }
    while (    ( idx < bufLen )
            && ( theStr[idx] != 0 ) );
}

void EEWriteBuffer(unsigned location, unsigned char *buf, unsigned char len)
{
    //
    //  Write a string into EEPROM with a maximum length of 16
    //  to prevent run away over writing
    //
    unsigned idx = 0;
    do {
        EEWriteByte( location+idx, buf[idx] ) ;
        ++idx;
    }
    while ( idx < len );
}

void EEReadBuffer(unsigned location, unsigned char *buf, unsigned char len)
{
    //  Return a String from EEProm location
    unsigned idx = 0;
    do {
        buf[idx] = EEReadByte( location+idx ) ;
        ++idx;
    }
    while ( idx < len );
}

void EEWriteInt( unsigned location, int theInt )
{
    //
    //  Write an integer into the EEPROM
    //
    EEWriteByte(location, (unsigned char)(theInt & 0xFF) ); 	// write lower byte
    EEWriteByte(location+1, (unsigned char)(theInt >> 8) );	// write upper byte
}

int EEReadInt( unsigned location )
{
//    unsigned char lower = EEReadByte(location);
//    upper = EEReadByte(location+1) ; 
    return ( ( (int)EEReadByte(location+1) ) << 8) | ( (int)EEReadByte(location) );
}

// These two are for othoganality and so we can change persistance later if we wish.
// Write byte to EEProm
//#define EEWriteByte(location, theByte) WriteEEData( location, theByte )

// Read byte from EEProm
//#define EEReadByte(location) (ReadEEData( location ))
#ifndef EEWriteByte
void EEWriteByte( unsigned location, unsigned char theByte)
{
    WriteEEData( location, theByte );
//    Wait(5);
}
#endif

// Read byte from EEProm
#ifndef EEReadByte
unsigned char EEReadByte(unsigned location)
{
    return ReadEEData( location );
}
#endif

void ResetConfig()     // reset configuration data only
{
    unsigned idx;
    // This area is the configuration data
    for (idx=PersistStart; idx < SPEnd; ++idx) 
    {
	EEWriteByte(idx,DF_PERSIST_ERASE);
        HARTBEAT;
    }
}

void FactoryReset()
{
    /*
    *	Unconditional reset to factory defaults
    *	Implemented by wiping the memory !
    *
    */
    ResetConfig();

    // this will be followed by reset and call to VerifyPersistantCfg
    // that will set up defaults for all this as password starts FF
    EEWriteByte( Password1Loc, DF_PERSIST_ERASE );

    if ( ( Options[OPT_SecurityFlags] & 0x01 ) == 0 )
    {
        unsigned idx;
        for ( idx = IPAddrLoc; idx < Password1Loc; ++idx )
        {
	    EEWriteByte( idx, DF_PERSIST_ERASE );
        }
    }
}
