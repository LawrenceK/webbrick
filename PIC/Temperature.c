// #define DO_DEBUG_TRACE  1
//
//  Copyright L.P.Klyne 2013
//
//  Filename: Temperature.c
//
//  Description:
//
//  Notes:
//
//
// Include files

#include "Webrick_Auto.h"

static unsigned int lastTemp[TempCount];    // last temp reads

void TempInit()    // initialise data
{
    unsigned char chn;
    unsigned baseC = TempCfgBase;
    unsigned baseV = TempValBase;
    for ( chn = 0; chn < TempCount; ++chn )
    {
        TempLowThreshold[chn] = EEReadInt( baseC+TempCfgLowOs );
        TempHighThreshold[chn] = EEReadInt( baseC+TempCfgHiOs );
        SPWriteInt( baseV + TempLowOs, TempLowThreshold[chn] );
        SPWriteInt( baseV + TempHiOs, TempHighThreshold[chn] );
        baseC += TempCfgSize;
        baseV += TempValSize;
    }
}

void TempAdjustThreshold(unsigned char chn, unsigned char action, int value)
{
    unsigned baseV = TempValBase + (TempValSize*chn);
    switch ( action )
    {
        case DIN_GROUP1_SET_LOW_THRESHOLD:
            TempLowThreshold[chn] = value;
            SPWriteInt( baseV + TempLowOs, value );  // inform outside world
            break;
        case DIN_GROUP1_SET_HIGH_THRESHOLD:
            TempHighThreshold[chn] = value;
            SPWriteInt( baseV + TempHiOs, value );  // inform outside world
            break;
        case DIN_GROUP1_ADJ_LOW_THRESHOLD:
            TempLowThreshold[chn] += value;
            SPWriteInt( baseV + TempLowOs, TempLowThreshold[chn] );  // inform outside world
            break;
        case DIN_GROUP1_ADJ_HIGH_THRESHOLD:
            TempHighThreshold[chn] += value;
            SPWriteInt( baseV + TempHiOs, TempHighThreshold[chn] );  // inform outside world
            break;
    }
    lastTemp[chn] = -255;   // force refresh and check of thresholds.
}

//void TempFindSensors()
//{
//    OWIdentifySensors();
//}

void TempReadSensors( unsigned char chn )
{
    int deg;
    unsigned char Ows;
    unsigned baseV = TempValBase + (TempValSize*chn);
    unsigned baseC = TempCfgBase + (TempCfgSize*chn);

    // This updates two bits of data.
    OWReadSensors( chn );

    // Check status after read of sensor in case read fails.
    Ows = OWStatus();   // status 
    deg = OWTmp(chn);   // and value

    // bus ok but may be invalid sensor still
    // data always written, could be invalid temp (0x7FFF)
    if ( ( Ows & (0x01 << chn ) ) == 0 )
    {
        // offline so update status first
        SPWriteChr( OneWStsLoc, Ows );
        SPWriteInt( baseV+TempValOs, deg ); // always update, if -1 written then bad data
        lastTemp[chn] = deg;    // ensure has invalid value
    }
    else
    {
        int thr;

        // online so update data first
        SPWriteInt( baseV+TempValOs, deg ); // always update, if -1 written then bad data
        SPWriteChr( OneWStsLoc, Ows );

        thr = lastTemp[chn] - deg;  // calculate diff in temp
        if (thr < 0) thr = -thr;
	if ( thr > 1 )  // greater than 1 bit change
        {
            // Send UDP packet with new value.
            if UDP_SEND(OPT_UDPSendTempChange)
            {
                UDPPkt.source[0] = 'C';
                UDPPkt.source[1] = 'T';
                UDPPkt.srcChannel = chn;   // source channel.
                UDPPkt.curValH = deg >> 8;
                UDPPkt.curValL = deg & 0xFF;
                UDPPkt.udpType = 'G';
                SendUDP();
            }

            thr = TempLowThreshold[chn];
            if ( ( deg < thr ) && ( lastTemp[chn] >= thr ) )
//            if ( deg < thr )
            {
                PerformTrigger( 't', chn, baseC+TempCfgLowActionOs );
            }
            thr = TempHighThreshold[chn];
            if ( ( deg > thr ) && ( lastTemp[chn] <= thr ) )
//            if ( deg > thr )
            {
                PerformTrigger( 'T', chn, baseC+TempCfgHiActionOs );
            }
            lastTemp[chn] = deg;
        }
    }
}

void TemperaturePeriodic()
{
    unsigned char chn;
    if ( TEMP_OPTION(OPT_TempReCheckThr) && ( Secs == 15 ) )
    {
    	unsigned baseC = TempCfgBase;
        for ( chn = 0; chn < TempCount; ++chn )
        {
            if ( lastTemp[chn] < TempLowThreshold[chn] )
            {
                PerformTrigger( 't', chn, baseC+TempCfgLowActionOs );
            }
            if ( lastTemp[chn] > TempHighThreshold[chn] )
            {
                PerformTrigger( 'T', chn, baseC+TempCfgHiActionOs );
            }
            baseC += TempCfgSize;
        }
    }
}
