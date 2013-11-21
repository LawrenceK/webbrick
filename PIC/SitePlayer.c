//#define DO_DEBUG_TRACE  1
//
//   SitePlayer Libfrary
//
//   Andy Harris, Extend Technologies, February 2003
//
//  This is a library of utilities to drive the siteplayer over the
//  standard serial port on a PIC chip 
//
//

#include "Webrick_Auto.h"

static const unsigned char defIpAddress[] = {10,100,100,100};

// Local Data
static const unsigned default_dwells[] = {
    DF_DWELL0,
    DF_DWELL1,
    DF_DWELL2,
    DF_DWELL3,
    DF_DWELL4,
    DF_DWELL5,
    DF_DWELL6,
    DF_DWELL7
};
// can this be shared elsewhere?
static unsigned char buf[16];  // send 16 byte lumps.

void clearWatchDog()
{
#pragma asm
    clrwdt	; Clear the watchdog timer
#pragma asmend
}

// load all perisitant config into RAM structures as required
void ReadPersistantCfg()
{
    unsigned char idx;
    unsigned baseC = SetPBLoc;

    MyNodeNr = EEReadByte(NNumLoc) ;

#ifdef USE_MIMICS
    EEReadBuffer(MimicsForDigOutLoc, mimicForDig, sizeof(mimicForDig));
    EEReadBuffer(MimicsForAnOutLoc, mimicForAnOut, sizeof(mimicForAnOut));
    mimicOff = EEReadByte( MimicsLevelLowLoc );
    mimicOn = EEReadByte( MimicsLevelHighLoc );
#endif
    for (idx = 0; idx < SPCount; ++idx)
    {
	SetPoints[idx] = EEReadByte(baseC);
        // now up date to be 0-1023
        SetPoints[idx] = ANPERCENTTOVAL(SetPoints[idx]);  // convert to 0-1023 
        baseC += SPEntryLen;
    }

    RotStep = EEReadByte(RotStepLoc) ;

    baseC = DwellBaseLoc;
    for (idx = 0; idx < DwellCount; ++idx)
    {
        DwellTimes[idx] = EEReadInt(baseC);
        baseC += DwellEntryLen;
    }

    EEReadBuffer( OptionsLoc, Options, MaxOptions );
    FadeRate = EEReadByte( FadeRateLoc );
    MimicFadeRate = EEReadByte( MimicFadeRateLoc );

}

// write factory defaults to all parts of the persistant config.
void WriteDefaultPersistantCfg()
{
    unsigned char idx;
    unsigned baseC = DInBloc;
    unsigned baseN = DInNameBase;

    EEWriteByte( VerMajorLoc, DF_SW_VER_MAJOR );
    EEWriteByte( VerMinorLoc, DF_SW_VER_MINOR );
    EEWriteInt( VerBuildLoc, DF_SW_VER_BUILD );
#ifdef DEBUG_TEMP_SENSOR_TEST
    EEWriteStr( NNameLoc, "TEST_ONLY", NNameLen );	// write name to EEProm
    EEWriteByte( NNumLoc,254 ) ;  // Write to EEPROM
#else
    EEWriteStr( NNameLoc, "UnNamed", NNameLen );	// write name to EEProm
    EEWriteByte( NNumLoc,DF_NODE_NUM ) ;  // Write to EEPROM
#endif

    // digital inputs.
    for ( idx = 0; idx < DInEntryCount; ++idx )
    {
	// Channel is unnamed so write defaults.
        sprintf( buf, "Sw-%u", idx );
	EEWriteStr(baseN, buf,DInNameLen);
        if ( idx > 7 )
        {
            // originally monitor inputs
	    EEWriteByte(baseC+DInActionOs, DIN_B1_GROUP1_MAKE( DIN_GROUP1_NONE, DIN_UDP_NONE) );
	    EEWriteByte(baseC+DInChanOs, 0 );
	    EEWriteByte(baseC+DInOptionsOs, DIN_RISING_EDGE+DIN_FALLING_EDGE );
        }
        else
        {
	    EEWriteByte(baseC+DInActionOs, DIN_B1_GROUP1_MAKE( DIN_GROUP1_TOGGLE, DIN_UDP_NONE) ); 	// default to toggle
	    EEWriteByte(baseC+DInChanOs, DIN_B2_MAKE( idx, DIN_ACTYPE_DIGITAL ) ); 	// default to same output channel
	    EEWriteByte(baseC+DInOptionsOs, DF_DIN_OPTIONS);
        }
	EEWriteByte(baseC+DInValue1Os, 0 ); 	// default to local command 
	EEWriteByte(baseC+DInValue2Os, 0 );
        baseC += DInCfgSize;
        baseN += DInNameLen;
    }
#ifdef USE_MIMICS
    EEWriteBuffer(MimicsForDigOutLoc, mimicForDig, sizeof(mimicForDig));
    EEWriteBuffer(MimicsForAnOutLoc, mimicForAnOut, sizeof(mimicForAnOut));

    mimicOff = MIMICS_OFF_DEFAULT;
    mimicOn = MIMICS_ON_DEFAULT;

    EEWriteByte( MimicsLevelLowLoc, mimicOff );
    EEWriteByte( MimicsLevelHighLoc, mimicOn );
    EEWriteByte(MimicFlagsLoc, DF_MIMICS_FLAGS );

#endif
    baseN = DOutNameBase;
    for (idx = 0 ; idx < DOutEntryCount ; ++idx )
    {
#ifdef DEBUG_TEMP_SENSOR_TEST
	EEWriteStr(baseN, "TestOnly", DOutNameLen) ;
#else
	EEWriteStr(baseN, "DigOut-0", DOutNameLen) ;
#endif
	EEWriteByte(baseN+7, idx+48 ) ;// update number to get to ascii 
        baseN += DOutNameLen;
    }

    baseN = AnInNameBase;
    baseC = AnInCfgBaseLoc;
    for ( idx = 0; idx < AnInEntryCount; ++idx )
    {
        // DInBloc

	EEWriteStr(baseN, "AnIn-0",AnInNameLen);
	EEWriteByte(baseN+5, idx+48); 	// update number to get to ascii 

	EEWriteByte(baseC+AnInCfgLowOs, 0 );
	EEWriteByte(baseC+AnInCfgLowActionOs, DF_AIN_ACTION );
	EEWriteByte(baseC+AnInCfgLowTypeOs, DIN_B2_MAKE( 0, DIN_ACTYPE_DIGITAL ) );
	EEWriteByte(baseC+AnInCfgLowValue1Os, 0);
	EEWriteByte(baseC+AnInCfgLowValue2Os, 0); 	// default to local command 
	EEWriteByte(baseC+AnInCfgHiOs, 100 );
	EEWriteByte(baseC+AnInCfgHiActionOs, DF_AIN_ACTION );
	EEWriteByte(baseC+AnInCfgHiTypeOs, DIN_B2_MAKE( 0, DIN_ACTYPE_DIGITAL ) );
	EEWriteByte(baseC+AnInCfgHiValue1Os, 0);
	EEWriteByte(baseC+AnInCfgHiValue2Os, 0); 	// default to local command 
        baseC += AnInCfgSize;
        baseN += AnInNameLen;
    }
    baseN = AnOutNameBase;
    for (idx = 0 ; idx < AnOutEntryCount; ++idx)
    {
	EEWriteStr(baseN, "AnOut-0", AnOutNameLen) ;
	EEWriteByte(baseN+6, idx+48 ) ; 	// update number to get to ascii 
        baseN += AnOutNameLen;
    }

    baseC = SetPBLoc;
    for (idx = 0; idx < SPCount; ++idx)
    {
        // calculate as percent
        SetPoints[idx] = ( ((unsigned)idx) * 100) / (SPCount-1); // seven non zero points
        EEWriteByte(baseC,(unsigned char)(SetPoints[idx]));
        baseC += SPEntryLen;
    }

    baseN = TempNameBase;
    baseC = TempCfgBase;
    for ( idx = 0; idx < TempCount; ++idx )
    {
	EEWriteStr(baseN, "Temp-0",TempNameLen);
	EEWriteByte(baseN+5, idx+48); 	// update number to get to ascii 

	EEWriteInt(baseC+TempCfgLowOs, -50*16 );
	EEWriteByte(baseC+TempCfgLowActionOs, DF_TEMP_ACTION );
	EEWriteByte(baseC+TempCfgLowTypeOs, DIN_B2_MAKE( 0, DIN_ACTYPE_DIGITAL ));
	EEWriteByte(baseC+TempCfgLowValue1Os, 0);
	EEWriteByte(baseC+TempCfgLowValue2Os, 0);
	EEWriteInt(baseC+TempCfgHiOs, 100*16 );
	EEWriteByte(baseC+TempCfgHiActionOs, DF_TEMP_ACTION );
	EEWriteByte(baseC+TempCfgHiTypeOs, DIN_B2_MAKE( 0, DIN_ACTYPE_DIGITAL ));
	EEWriteByte(baseC+TempCfgHiValue1Os, 0);
	EEWriteByte(baseC+TempCfgHiValue2Os, 0);
        baseN += TempNameLen;
        baseC += TempCfgSize;
    }

    RotStep = DF_ROT_STEP;
    EEWriteByte(RotStepLoc,RotStep) ;  // Write to EEPROM

    baseC = DwellBaseLoc;
    for (idx = 0; idx < DwellCount; ++idx)
    {
	DwellTimes[idx] = default_dwells[idx];			// default Dwells
	EEWriteInt(baseC,DwellTimes[idx]);
        baseC += DwellEntryLen;
    }

    baseC = SchedBaseLoc;
    for ( idx = 0; idx < SchedEntryCount; ++idx )
    {
	EEWriteByte(baseC+SchedDaysOs, 0);
	EEWriteByte(baseC+SchedHoursOs, 0);
	EEWriteByte(baseC+SchedMinsOs, 0);
	EEWriteByte(baseC+SchedActionOs, DF_SCHED_ACTION );
	EEWriteByte(baseC+SchedTypeOs, DIN_B2_MAKE( 0, DIN_ACTYPE_DIGITAL ) );
	EEWriteByte(baseC+SchedValue1Os, 0);
	EEWriteByte(baseC+SchedValue2Os, 0);
        baseC += SchedEntryLen;
    }

    baseC = SceneCfgBase;
    for ( idx = 0; idx < SceneEntryCount; ++idx )
    {
	EEWriteByte(baseC+SceneDigMaskOs, 0);
	EEWriteByte(baseC+SceneDigStateOs, 0);
	EEWriteByte(baseC+SceneAnMaskOs, 0);
	EEWriteInt(baseC+SceneAnStateOs, 0);
        baseC += SceneCfgSize;
    }

    EEWriteBuffer( OptionsLoc, Options, MaxOptions );

    FadeRate = DF_FADERATE;
    EEWriteByte( FadeRateLoc, FadeRate );
    MimicFadeRate = DF_FADERATE;
    EEWriteByte( MimicFadeRateLoc, MimicFadeRate );
    EEWriteByte(SerialModeLoc, DF_SERIAL_FLAGS );

    EEWriteStr( Password1Loc, "", PasswordLen );
    EEWriteStr( Password2Loc, "password", PasswordLen );
    EEWriteStr( Password3Loc, "installer", PasswordLen );

    // Erase options, but skip SecurityOptions flag
    for ( idx = OptionsLoc+MinOptions; idx < PersistStart; ++idx )
    {
        EEWriteByte( idx, 0 );
    }
    // set default UDP options
    EEWriteByte( OptionsLoc+OPT_UDPOptions, DF_UDPOption );
    EEWriteByte( OptionsLoc+OPT_AnOutOptions, DF_AnOutOption );
    EEWriteByte( OptionsLoc+OPT_SceneOptions, DF_SceneOption );
    EEWriteByte( OptionsLoc+OPT_AnInOptions, DF_AnInOption );
    EEWriteByte( OptionsLoc+OPT_TempOptions, DF_TempOption );
    EEWriteByte( OptionsLoc+OPT_DigInOptions, DF_DigInOption );
    EEWriteByte( OptionsLoc+OPT_DigOutOptions, DF_DigOutOption );
}

void VerifyPersistantCfg()
{
    SPClearComms();

    if (EEReadByte(IPAddrLoc) == 0xFF)
    {
        EEWriteStr(IPAddrLoc, defIpAddress, 4); // factory reset.
    }
    
    if (   (EEReadByte(VerMajorLoc) == 0xFF )
        || (EEReadByte(NNameLoc) == 0xFF) )
        {
        WriteDefaultPersistantCfg();
        }
    ReadPersistantCfg();

    initIr(); // verfifies config

}

void RefreshDynamicData()
{
    unsigned baseV;
    unsigned char idx;
    SPWriteChr( LoginStateLoc, LoggedInState );
    SPWriteChr( OperStateLoc, OperState );
    SPWriteChr( DOutStsLoc, LATD );
    SPWriteChr( DInLoc, DigitalWork[0].state );
    SPWriteChr( DInLoc+1, DigitalWork[1].state );
    SPWriteChr( OneWStsLoc, 0x80 );
    SendClockTime();
    // temperatures
    baseV = TempValBase;
    for ( idx = 0; idx < TempCount; ++idx )
    {
        // should send but will catch up very soon.
//        SPWriteInt( baseV + TempValOs, temp[idx] );
        SPWriteInt( baseV + TempLowOs, TempLowThreshold[idx] );
        SPWriteInt( baseV + TempHiOs, TempHighThreshold[idx] );
        baseV += TempValSize;
    }
    for ( idx = 0; idx < AnOutEntryCount; ++idx )
    {
        SPWriteChr( AnOutBLoc + idx, ANVALTOPERCENT(AnOutTarget[idx]) );  // inform outside world
    }
    baseV = AnInValBase;
    for ( idx = 0; idx < AnInEntryCount; ++idx )
    {
        SPWriteChr( baseV + AnInValOs, AnInVals[idx] );
        SPWriteChr( baseV + AnInValLowOs, AnLowThreshold[idx] );
        SPWriteChr( baseV + AnInValHiOs, AnHighThreshold[idx] );
        baseV += AnInValSize;
    }
}

void SPStartup()
{
    unsigned len;
    unsigned idx;
    
    DEBUGN( 0x2D);

    // from 0 -> PersistStart
    // This area is for dynamic data in the siteplayer
    for ( idx = 0; idx < PersistStart; ++idx )
    {
	SPWriteChr( idx, 0 );
    }
    SPWriteChr( LoginStateLoc, LoggedInState );

    // from PersistStart -> end of SP objects.
    for ( idx = PersistStart; idx < SPEnd; idx += 16 )
    {
        len = SPEnd-idx;
        if ( len > 16 ) len = 16;
        EEReadBuffer( idx, buf, len );
        SPWriteBuffer( idx, buf, len );   // Max 20 characters.
        HARTBEAT;
        Wait(5);    // slow down to not overwhelm SP.
    }
    // Second area of peristant memory
    // UDP space is between them.
    for ( idx = SPPersist2; idx < SPEnd2; idx += 16 )
    {
        len = SPEnd-idx;
        if ( len > 16 ) len = 16;
        EEReadBuffer( idx, buf, len );
        SPWriteBuffer( idx, buf, len );   // Max 20 characters.
        HARTBEAT;
        Wait(5);    // slow down to not overwhelm SP.
    }
    DEBUGN( 0x2E);
    
    /* ========================================================================
    *     Write IP adrdress to SitePlayer, only if its configured in EEPROM
    *  =========================================================================
    */
    buf[0] = EEReadByte(IPAddrLoc);
    if (buf[0] != 0xFF)
    {
        EEReadBuffer( IPAddrLoc, buf, 4 );
        SPWriteBuffer( SPIPAddr, buf, 4 );
    }
    DEBUGN( 0x2F);
}

