//#define DO_DEBUG_TRACE  1
//
//  Copyright L.P.Klyne 2013
//
//  Filename: Analogue.c
//
//  Description:
//
//  Notes:
//
//
// Include files

#include "Webrick_Auto.h"

#define fadeStep    4
// for now limit max analogue out to be 1023.
#define MAX_AN_OUT  1023

// Local Defines

// Local forwards

// Static data (File scope)
static unsigned char chn;
static unsigned char idx;
static unsigned curVal;
static unsigned targetVal;

static unsigned AnOutCurrent[AnOutEntryCount] = {0,0,0,0};	// analogue out, actual and target
unsigned AnOutTarget[AnOutEntryCount] = {0,0,0,0};	// analogue out, actual and target
static unsigned AnDwells[AnOutEntryCount] = { 0,0,0,0};	        // array of dwells
unsigned char AnInVals[AnInEntryCount] = { 0,0,0,0 };	// analogue in, 0-100%
static unsigned char AnOutCurrentSetPoint[AnOutEntryCount] = { 0,0,0,0 };	// keep track of current set point.

// Read 8 samples from the AD for each channel and average them.
#define AnInAccCounts    8
#define AnInAccLimit    7
//static unsigned char adChn;
static unsigned char AnInAccCount = AnInAccLimit; // count number of accumulated entries.
static unsigned AnInAccumulate[AnInEntryCount] = { 0,0,0,0 };	// sum of a number of analogue reads

#ifdef USE_MIMICS
unsigned char mimicForAnOut[(AnOutEntryCount+1)/2] = { 0xFF, 0xFF }; // default no mimics
#endif

// Local functions

//
//  Here is the code that operates Analogue outputs as a result of rotary encoder changes.
//
static void updateAnMimic( unsigned char chn )
{
#ifdef USE_MIMICS
    idx = mimicForAnOut [chn/2];
    if ( chn & 0x01 )
    {
        idx = idx >> 4;
    }
    else
    {
        idx = idx & 0x0F;
    }
    if ( idx < MimicCount )
    {
        SetMimicState( idx, ( ( AnOutTarget[chn] > 0 ) || ( AnOutCurrent[chn] > 0 ) ) );
    }
#endif
}

#ifdef USE_MIMICS
void AnalogueMimicInit()
{
    for ( idx = 0 ; idx < AnOutEntryCount; ++idx )
    {
        updateAnMimic( idx );
    }
}
#endif

static void AnalogueDec(unsigned char chn, unsigned char step)
{
    if ( chn < AnOutEntryCount )
    {
    }
}

// Global functions

void AnalogueInit()    // initialise data
{
    unsigned baseC = AnInCfgBaseLoc;
    unsigned baseV = AnInValBase;
    for ( chn = 0; chn < AnInEntryCount; ++chn )
    {
        AnLowThreshold[chn] = EEReadByte(baseC + AnInCfgLowOs );
        AnHighThreshold[chn] = EEReadByte(baseC + AnInCfgHiOs );
        SPWriteChr( baseV + AnInValLowOs, AnLowThreshold[chn] );  // inform outside world
        SPWriteChr( baseV + AnInValHiOs, AnHighThreshold[chn] );  // inform outside world
        baseC += AnInCfgSize;
        baseV += AnInValSize;
    }
#ifdef USE_MIMICS
    AnalogueMimicInit();
#endif
}

void AnaAdjustThreshold(unsigned char chn, unsigned char action, char value)
{
    // Tell wide world.
    unsigned baseV = AnInValBase + (AnInValSize *chn);
    switch ( action )
    {
        case DIN_GROUP1_SET_LOW_THRESHOLD:
            AnLowThreshold[chn] = value;
            SPWriteChr( baseV + AnInValLowOs, AnLowThreshold[chn] );  // inform outside world
            break;
        case DIN_GROUP1_SET_HIGH_THRESHOLD:
            AnHighThreshold[chn] = value;
            SPWriteChr( baseV + AnInValHiOs, AnHighThreshold[chn] );  // inform outside world
            break;
        case DIN_GROUP1_ADJ_LOW_THRESHOLD:
            AnLowThreshold[chn] += value;
            SPWriteChr( baseV + AnInValLowOs, AnLowThreshold[chn] );  // inform outside world
            break;
        case DIN_GROUP1_ADJ_HIGH_THRESHOLD:
            AnHighThreshold[chn] += value;
            SPWriteChr( baseV + AnInValHiOs, AnHighThreshold[chn] );  // inform outside world
            break;
    }
}

void AnaFade()
{
//
//  Here we execute any required fades
//
    for ( chn = 0; chn < AnOutEntryCount; ++chn )
    {
        curVal = AnOutCurrent[chn];
        targetVal = AnOutTarget[chn];
	if ( curVal != targetVal )
        {
            // fade a little faster, this gives a default with faderate at 8 of 2 seconds
            // the fatest fade with faderate at 1 is around 0.02 second
            // the slowest fade with faderate at 255 is around 50 seconds
	    if ( curVal > targetVal)
	    {
	        if ( ( curVal - targetVal ) < fadeStep )
                {
                    curVal = targetVal;
                }
                else
                {
	            curVal -= fadeStep;    // fade down
                }
	    }
	    else
	    {
	        if ( ( targetVal - curVal ) < fadeStep )
                {
                    curVal = targetVal;
                }
                else
                {
	            curVal += fadeStep;    // fade up
                }
	    }
            AnOutCurrent[chn] = curVal;

            // because these functions are not indexed!
            switch( chn )
            {
                case 0:
	            SetPWM1Volts(curVal);
                    break;
                case 1:
	            SetPWM2Volts(curVal);
                    break;
#ifdef PWM3Used
                case 2:
	            SetPWM3Volts(curVal);
                    break;
#endif
#ifdef PWM4Used
                case 3:
	            SetPWM4Volts(curVal);
                    break;
#endif
            }
            if (ANOUT_OPTION(OPT_AnOutDoDmx))
            {
                // analogues are 0-1023, Dmx is 0-255
                DmxSetLevel( chn+1, (char)(curVal>>2) );
            }
	    if ( curVal == targetVal )
            {
                curVal = ANVALTOPERCENT( curVal );
                SPWriteChr( AnOutBLoc+(unsigned int)chn, curVal );  // inform outside world
                updateAnMimic( chn );
                // Send UDP packet with new value.
                if UDP_SEND(OPT_UDPSendAnOutChange)
                {
                    UDPPkt.source[0] = 'A';
                    UDPPkt.source[1] = 'O';
                    UDPPkt.srcChannel = chn;   // source channel.
                    UDPPkt.curValH = 0;
                    UDPPkt.curValL = curVal;
                    UDPPkt.udpType = 'G';
                    SendUDP();
                }
            }
        }
    }
}

void AnaCommand(unsigned char chn, unsigned char action, unsigned char spNum )
{
    //
    //  Analogue commands, extra implemented from version 5.0B5
    //
    if (   OPER_STATE_IS_NOT_FROZEN
        && ( chn < AnOutEntryCount ) )
    {
        curVal = AnOutCurrent[chn];
        targetVal = AnOutTarget[chn];
        switch ( action )
        {
            case DIN_GROUP1_ON:
                AnOutTarget[chn] = SetPoints[spNum]; // simple analogue 'ON' command
                AnOutCurrentSetPoint[chn] = spNum;
                break;
            case DIN_GROUP1_OFF:
                AnOutTarget[chn] = 0; // simple analogue 'OFF' command
                AnOutCurrentSetPoint[chn] = 0;
                break;
            case DIN_GROUP1_TOGGLE:
                if ( curVal > 0 )
                {
                    AnOutTarget[chn] = 0;
                    AnOutCurrentSetPoint[chn] = 0;
                }
                else
                {
                    AnOutTarget[chn] = SetPoints[spNum];
                    AnOutCurrentSetPoint[chn] = spNum;
                }
                break;

            case DIN_DWELL_ON:
                // currently off (switch on) or in a dwell (extend)
                if ( ( AnOutTarget[chn] == 0 ) || ( AnDwells[chn] > 0 ) )
                {
                    AnOutTarget[chn] = SetPoints[spNum];
  	            AnDwells[chn] = dwellValue;
                }
  	        break ;

            case DIN_DWELL_OFF:
                // currently on
                if ( AnOutTarget[chn] > 0 )
                {
  	            AnDwells[chn] = dwellValue;
                    AnOutCurrentSetPoint[chn] = 0;  // so we go off when dwell expires
                }
  	        break ;

            case DIN_DWELL_CANCEL:
                if ( AnDwells[chn] == 0 )
                {
                    AnOutTarget[chn] = SetPoints[spNum];
  	            AnDwells[chn] = dwellValue;
                }
                else
                {
  	            AnDwells[chn] = 0;
                    AnOutTarget[chn] = SetPoints[AnOutCurrentSetPoint[chn]]; // simple analogue 'ON' command
                }
  	        break;

            case DIN_DWELL_ALWAYS:
                AnOutTarget[chn] = SetPoints[spNum];
  	        AnDwells[chn] = dwellValue;
  	        break ;

            case DIN_GROUP1_MOMENTARY:
                curVal = SetPoints[spNum];
                AnOutTarget[chn] = 0;
//                AnOutCurrentSetPoint[chn] = 0;
  	        break;
            case DIN_GROUP1_NEXT:
                spNum = AnOutCurrentSetPoint[chn]+1;
                if ( spNum >= SPCount )
                {
                    spNum = (SPCount-1);    // restrict range
                }
                AnOutCurrentSetPoint[chn] = spNum;
                AnOutTarget[chn] = SetPoints[spNum];
                break;
            case DIN_GROUP1_PREV:
                spNum = AnOutCurrentSetPoint[chn];
                if ( spNum > 0 )
                {
                    --spNum;
                }
                AnOutCurrentSetPoint[chn] = spNum;
                AnOutTarget[chn] = SetPoints[spNum];
                break;
            case DIN_GROUP1_DOWN:
                if (targetVal < RotStep) 
                {
                    targetVal = 0; // limit check
                }
                else
                {
                    targetVal -= RotStep;   // decrement by step
                }
                AnOutTarget[chn] = targetVal;
//                updateAnMimic( chn );
                break;
            case DIN_GROUP1_UP:
                targetVal += RotStep;
                if (targetVal > MAX_AN_OUT) 
                {
                    targetVal = MAX_AN_OUT; // limit check
                }
                AnOutTarget[chn] = targetVal;
//                updateAnMimic( chn );
                break;
	    case DIN_GROUP1_NONE:
	        break;
        }   // end switch
        if ( AnOutTarget[chn] > MAX_AN_OUT )
        {
            AnOutTarget[chn] = MAX_AN_OUT;
        }
        SPWriteChr( AnOutBLoc + (unsigned int)chn, ANVALTOPERCENT(AnOutTarget[chn]) );  // inform outside world
        updateAnMimic( chn );
    } // endif OperState
} // of function

void AnaOut( unsigned char chn, unsigned value )
{
    //
    //  update Set Values of analogue outputs, done this way in case there are slave PICs in the future
    //
    if ( chn < AnOutEntryCount )
    {
        AnOutTarget[chn] = value ;
        updateAnMimic( chn );
    }
}

// FED Occurence
void GetAnIn()
{
    // Check that the conversion has completed. Had some funnys that appeared to be getting previous channel value
    // in data.
    if (OPER_STATE_IS_NOT_FROZEN && AN_FREE )   // ensure finished first
    {
        //
        //  Get result of analogue input
        //
//        curVal = ADRES;
//        curVal = ( ((unsigned)ADRESH) << 8 ) | (unsigned)ADRESL;
        chn = ((ADCON0 >> 2) & 0x03);   // get current channel number
//        if ( adChn != chn )
//        {
            // data corruption
//            DEBUG3( DebugTrace_AnInGetAnIn, chn, adChn );
//        }
//        DEBUG5( DebugTrace_AnInGetAnIn, AnInAccCount, chn, ADRESH, ADRESL );
        AnInAccumulate[chn] = AnInAccumulate[chn] + ((int)ADRESH << 8) + ((int)ADRESL);
//        AnInAccumulate[chn] += curVal;
//        DEBUG5( DebugTrace_AnInGetAnIn, AnInAccCount, chn, (AnInAccumulate[chn]>>8), (AnInAccumulate[chn]&0xFF) );
        if (AnInAccCount == 0)
        {
            // accumulated a number of AnIn values now calc average.
            unsigned char AnInP;
            AnInP = ANVALTOPERCENT( (AnInAccumulate[chn]/AnInAccCounts) );
//            DEBUG5( DebugTrace_AnInGetAnIn, chn, AnInP, (AnInAccumulate[chn]>>8), (AnInAccumulate[chn]&0xFF) );
            if ( AnInVals[chn] != AnInP )
	    {
                unsigned char thr;

//                DEBUG5( DebugTrace_AnInGetAnIn, chn, AnInP, (AnInAccumulate[chn]>>8), (AnInAccumulate[chn]&0xFF) );
                unsigned baseC = AnInCfgBaseLoc + ( ((unsigned int)chn)*AnInCfgSize);
                unsigned baseV = AnInValBase + ( ((unsigned int)chn)*AnInValSize);

                SPWriteChr( baseV + AnInValOs, AnInP );

                if UDP_SEND(OPT_UDPSendAnInChange)
                {
                    // Send UDP packet with new value.
                    UDPPkt.source[0] = 'A';
                    UDPPkt.source[1] = 'I';
                    UDPPkt.srcChannel = chn;   // source channel.
                    UDPPkt.curValH = 0;
                    UDPPkt.curValL = AnInP;
                    UDPPkt.udpType = 'G';
                    SendUDP();
                }

    // Get thresholds and do triggers
                // later treat as general triggers.
                thr = AnLowThreshold[chn];
                if ( ( AnInP < thr ) && ( AnInVals[chn] >= thr ) )
    //            if ( AnInP < thr )
                {
                    PerformTrigger( 'a', chn, baseC + AnInCfgLowActionOs ); 
                }
                thr =  AnHighThreshold[chn];
                if ( ( AnInP > thr ) && ( AnInVals[chn] <= thr ) )
    //            if ( AnInP > thr )
                {
                    PerformTrigger( 'A', chn, baseC + AnInCfgHiActionOs );
                }
	        AnInVals[chn] = AnInP;   // Save
	    }
	    AnInAccumulate[chn] = 0;
        }
#ifdef __18F4525
        // legacy
        if (AnInAccCount == 0 )
        {
            AnInAccCount = AnInAccLimit;
        }
        else
        {
            --AnInAccCount; // next set of values
        }
#else
//        if ( adChn != chn )
//        {
            // data corruption
//            DEBUG3( DebugTrace_AnInGetAnIn, chn, adChn );
//        }
        // change AD channel.
        ++chn;
        if ( chn >= AnInEntryCount )
        {
            chn = 0;
            if (AnInAccCount == 0)
            {
                AnInAccCount = AnInAccLimit;
            }
            else
            {
                --AnInAccCount; // next set of values
            }
        }
//        adChn = chn;
        ADCON0 = ( ADCON0 & 0x03 ) | ( chn << 2 );    // set next channel ready for next conversion.
#endif
    }
}

// reissue threshold triggers
void AnalogueCheckThresholds()
{
    for (chn = 0 ; chn < AnOutEntryCount; ++chn)
    {
    }
}

// dwell processing.
void AnaloguePeriodic()
{
    unsigned baseC = AnInCfgBaseLoc;
    for (chn = 0 ; chn < AnOutEntryCount; ++chn)
    {
	if (AnDwells[chn] > 0) 
	{
	    --AnDwells[chn];			// decrement dwell
	    if (AnDwells[chn] == 0) 
            {
                AnaCommand(chn, DIN_GROUP1_ON, AnOutCurrentSetPoint[chn] ); // return to last set point
            }
        }
        if ( ANIN_OPTION(OPT_AnInReCheckThr) && ( Secs == 25 ) )
        {
            if ( AnInVals[chn] < AnLowThreshold[chn] )
            {
                PerformTrigger( 'a', chn, baseC + AnInCfgLowActionOs ); 
            }
            if ( AnInVals[chn] > AnHighThreshold[chn] )
            {
                PerformTrigger( 'A', chn, baseC + AnInCfgHiActionOs );
            }
        }
        baseC += AnInCfgSize;
    }
}



