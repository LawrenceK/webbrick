//#define DO_DEBUG_TRACE  1

// generate multiple pulses per cycle of Rotary encoder, i.e. one trigger on each input change.
//#define DO_MP_ROTENC  1
//
//  Copyright L.P.Klyne 2013
//
//  Filename: Digital.c
//
//  Description:     Contains the code for digital inputs and outputs
//
//  Notes:
//
//
// Include files

#include "Webrick_Auto.h"

// Local Defines

// Local forwards

//char dinIgnore1 = 0;    // set bit to ignore the inputs on port D
//char dinIgnore2 = 0;    // set bit to ignore the other digital inputs mixed ports
// clear bit to disable output.
static char doutMask1 = 0xFF;

// Static data (File scope)
static unsigned char DigCounts[DInEntryCount];
static unsigned ClickTimers[DInEntryCount];   // array of timers used to control multiple click counts
                    
// Active dwells have non zero values in this array.
static unsigned DigDwells[DOutEntryCount];	            // array of dwells
static unsigned char ShrtDwells[DOutEntryCount];   // array of short dwells, used for momentary

struct _tagDigPort DigitalWork[2];

//unsigned char DigInState = 0xFF;        // pulled high
//unsigned char DigInPending = 0xFF;
//unsigned char DigInLockout = 0xFF;      // bit cleared for 20mS to lockout channel.
//unsigned char DigInState2 = 0xFF;	// previous state of monitors, next 4 inputs.
//unsigned char DigInPending2 = 0xFF;

static unsigned char DebounceTimers[DInEntryCount];
static unsigned LastGroup1Scene = 0x00;
static unsigned LastGroup2Scene = 0x08; // init to scene 8.

#ifdef USE_MIMICS
unsigned char mimicSts = 0;
unsigned char mimicOff;
unsigned char mimicOn;
unsigned char mimicForDig[(DOutPrimaryCount+1)/2] = { 0x10, 0x32, 0x54, 0x76 };
#endif

// More work data, carefull not reused whilst in use.

/*

There is an 8 way table for a rotary encoder, R,F,0,1 are the inputs rising, falling, 0, 1

a | b | result
--------------
0 | R | down
0 | F | up
1 | R | up
1 | F | down
R | 0 | up
F | 0 | down
R | 1 | down
F | 1 | up

For this test we are going to simplify this and only pick one transition, a falling edge on b.
We could define a lookup table, only needs single bit? 16 combinations, some illegal/impossible.

e.g.

a | b | aEdge | bEdge | result
------------------------------
0 | 0 | R     | N     | up
0 | 1 | R     | N     | down
1 | 0 | F     | N     | down
1 | 1 | F     | N     | up
0 | 0 | N     | R     | down
0 | 1 | N     | F     | up
1 | 0 | N     | R     | up
1 | 1 | N     | F     | down

note: a&b cannot both change state at the same time. hence 8 entries, implementation may include 
the illegals of both changing state, 16 entry lookup table.

*/
// this state table is as follows, indexed by 2 bit old state to get an entry
// and then get bit number using new state, obviously some entries never get used..
// if the bit is 1 go up, else go down.
#ifdef DO_MP_ROTENC

static unsigned char rotaryMpStateTable[4] = { 
    0x02,   // was 00, new state 00 and 11 are invalid, 01 is up and 10 is down
    0x08,   // was 01, new state 10 and 01 are invalid, 11 is up and 00 is down
    0x01,   // was 10, new state 10 and 01 are invalid, 11 is down and 00 is up
    0x04    // was 11, new state 00 and 11 are invalid, 01 is down and 10 is up
    };

#else

// alternate rotary encoder, index by input number, masks out bits that are irrelevant to it.
// for each input number gives mask for the two bits of input.
//static unsigned char rotaryBitPairMasks[DInEntryCount] = { 0x03, 0x03, 0x0C, 0x0C, 0x30, 0x30, 0xC0, 0xC0, 0x03, 0x03, 0x0C, 0x0C };
// for each input masks out just the up bit
//static unsigned char rotaryBitUpMasks[DInEntryCount] = { 0x01, 0x01, 0x04, 0x04, 0x10, 0x10, 0x40, 0x40, 0x01, 0x01, 0x04, 0x04 };

#endif

// these two used to lockout an input when being used fro a second purpose.
// When the bit is cleared then the input is not read, 
// [ Note: the bit can be temporarily cleared for debounce]
void reserveInput( unsigned char chn )
{
    DigitalWork[(chn>>3) & 1].lockout &= ~(0x01 << (chn&0x07) );
}

void unReserveInput( unsigned char chn )
{
    DigitalWork[(chn>>3) & 1].lockout |= (0x01 << (chn&0x07) );
}

// these two used to lockout an output when being used fro a second purpose.
void reserveOutput( unsigned char chn )
{
    doutMask1 &= ~(0x01<<chn);
}

void unReserveOutput( unsigned char chn )
{
    doutMask1 |= (0x01<<chn);
}

// Global functions
// Action a trigger that we have already decoded
void ActionTrigger( unsigned char ttype, unsigned char srcChn )
{
    // First notify the trigger
    if ( udpDo )
    {
        UDPPkt.source[0] = 'T';
        UDPPkt.source[1] = ttype;
        UDPPkt.srcChannel = srcChn;   // source channel.

        UDPPkt.tgtChannel = chnType | tgtChn;   // target channel & type.

        UDPPkt.action = action;
        UDPPkt.setPointNr = setPointNr;
        if ( ttype == 'D' )
        {
            UDPPkt.curValH = 0;
            UDPPkt.curValL = DigCounts[srcChn];
        }
        UDPPkt.udpType = 'G';
        UDPPkt.toNodeNr = 0;
        SendUDP();
    }   // end switch

    if (   ( action >= DIN_GROUP1_SET_LOW_THRESHOLD ) 
        && ( action <= DIN_GROUP1_ADJ_HIGH_THRESHOLD ) )
    {
        // can only be analogue or temperature.
        if ( chnType == DIN_ACTYPE_ANALOGUE )
        {
            AnaAdjustThreshold( tgtChn, action, assocValue1 );
        }
        else
        {
            TempAdjustThreshold( tgtChn, action, (((int)assocValue1)*16) );
        }
    }
    else
    if ( action == DIN_GROUP1_SEND_IR )
    {
        sendIr( assocValue1, tgtChn );
    }
    else
    if ( action == DIN_GROUP1_SET_DMX )
    {
        // trigger encodes 0.. n for channels 1..n+1
        DmxSetLevel( tgtChn+1, assocValue1 );
    }
    else
    if ( chnType == DIN_ACTYPE_DIGITAL )
    {
	//Digital command
	DigitalOut(tgtChn, action);
    }
    else
    if ( chnType == DIN_ACTYPE_SCENE )
    {
        // Scene trigger
        DoScene( tgtChn, action );
    }
    else
    {
	// Analogue command
	AnaCommand(tgtChn, action, setPointNr );
    }
}

void PerformTrigger( unsigned char ttype, unsigned char srcChn, unsigned baseLoc )
{
    //
    //  Generic trigger handler, passed trigger location.
    //
    b1Work = EEReadByte(baseLoc + DInActionOs);
    b2Work = EEReadByte(baseLoc + DInChanOs);
    assocValue1 = EEReadByte(baseLoc + DInValue1Os);
    assocValue2 = EEReadByte(baseLoc + DInValue2Os);

    chnType = DIN_B2_ACTION_TYPE(b2Work);
    tgtChn = DIN_B2_CHN(b2Work);
    setPointNr = DIN_B2_ANSETPOINT(b2Work);

    udpDo = DIN_B1_SEND_UDP(b1Work);
//    dwellNr = DIN_B1_DWELL_CHN(b1Work);
    // not always needed
    if ( DIN_B1_COMMAND_GROUP(b1Work) == DIN_GROUP_DWELL )
    {
        action = DIN_B1_DWELL_COMMAND(b1Work);
        dwellValue = DwellTimes[DIN_B1_DWELL_NR(b1Work)];
    }
    else
    {
        action = DIN_COMMAND(b1Work);
    }

    ActionTrigger( ttype, srcChn );
}

// updated to limit scene range using the scene numbr in the trigger. As scenes no longer
// wrap from begin to end (&VV). May also want to detect unconfigured scene as well and skip it.
// would need to read scene as doing calc.
void DoScene( unsigned snum, unsigned char action )
{
    unsigned char idx;
    unsigned char Mask;
    unsigned State;
    unsigned eloc;  // EEPROM Location

    // map Up/Down to next/prev. as Up?Down only have specific actions on Analogue
    if ( action == DIN_GROUP1_UP )
    {
        action = DIN_GROUP1_NEXT;
    }
    else
    if ( action == DIN_GROUP1_DOWN )
    {
        action = DIN_GROUP1_PREV;
    }

    // change to limit NEXt to not going above tgtChn and PREV not going below tgtChn
    if ( snum <= SceneGroup1EntryLimit )
    {
        if ( action == DIN_GROUP1_NEXT )
        {
            if ( ( LastGroup1Scene < SceneGroup1EntryLimit ) && ( LastGroup1Scene < snum ) )
            {
                ++LastGroup1Scene;
                action = DIN_GROUP1_ON;
            }
            else
            {
                action = DIN_GROUP1_NONE;
            }
        }
        else
        if ( action == DIN_GROUP1_PREV )
        {
            if ( ( LastGroup1Scene > SceneGroup1EntryBase ) && ( LastGroup1Scene > snum ) )
            {
                --LastGroup1Scene;
                action = DIN_GROUP1_ON;
            }
            else
            {
                action = DIN_GROUP1_NONE;
            }
        }
        else
        {
            LastGroup1Scene = snum;
        }
        eloc = SceneCfgBase + (LastGroup1Scene*SceneCfgSize);
    }
    else
    {
        if ( action == DIN_GROUP1_NEXT )
        {
            if ( ( LastGroup2Scene < SceneGroup2EntryLimit ) && ( LastGroup2Scene < snum ) )
            {
                ++LastGroup2Scene;
                action = DIN_GROUP1_ON;
            }
            else
            {
                action = DIN_GROUP1_NONE;
            }
        }
        else
        if ( action == DIN_GROUP1_PREV )
        {
            if ( ( LastGroup2Scene > SceneGroup2EntryBase ) && ( LastGroup2Scene > snum ) )
            {
                --LastGroup2Scene;
                action = DIN_GROUP1_ON;
            }
            else
            {
                action = DIN_GROUP1_NONE;
            }
        }
        else
        {
            LastGroup2Scene = snum;
        }
        eloc = SceneCfgBase + (LastGroup2Scene*SceneCfgSize);
    }

    // we may reset the operand if no action to be performed.
    if ( action != DIN_GROUP1_NONE )
    {
        // Read the scene.
        Mask = EEReadByte(eloc+SceneDigMaskOs );
        State = EEReadByte(eloc+SceneDigStateOs );

        idx = 0;
        while ( idx < DOutPrimaryCount )
        {
            if ( ( Mask & 1 ) != 0 )
            {
                if ( ( State & 1 ) != 0 )
                {
	            DigitalOut( idx, action );
                }
                else
                {
	            DigitalOut( idx, DIN_GROUP1_OFF );
                }
            }
            Mask = Mask >> 1;
            State = State >> 1; // tried >>= 1 and had compile error.
            ++idx;
        }

        Mask = EEReadByte(eloc+SceneAnMaskOs );
        State = EEReadInt(eloc+SceneAnStateOs );
        idx = 0;
        while ( idx < AnOutEntryCount )
    //            while ( AnMask > 0 )
        {
            if ( ( Mask & 1 ) != 0 )
            {
                if (SCENE_OPTION(OPT_SceneDoAnalogue))
                {
	            AnaCommand( idx, action, State & 0x07 );
                }
                else
                {
                    // Setpoints are in the range 0-100%
                    int lvl = SetPoints[State & 0x07];
                    lvl = lvl *255/100;
                    DmxSetLevel( idx+1, lvl );
                }
            }
            Mask = Mask >> 1;
            State = State >> 4;
            ++idx;
        }
    }
}

//void UpdateMimic( unsigned char chn, unsigned char targetLevel )
void SetMimicState( unsigned char chn, unsigned char isOn )
{
    unsigned char newMimicSts;
    if (isOn)
    {
        newMimicSts = mimicSts | (0x01<<chn);
        SetMimicTargetLevel( chn, mimicOn );
    }
    else
    {
        newMimicSts = mimicSts & ~(0x01<<chn);
        SetMimicTargetLevel( chn, mimicOff );
    }
    if ( mimicSts != newMimicSts )
    {
        // only update siteplayer if the state changes.
        mimicSts = newMimicSts;
        SPWriteChr(MimicStsLoc, mimicSts);
    }
}

void DigitalOut (unsigned char chn, unsigned char action )
{
    //
    //  Deal with request to control output
    //
    //  Need to deal with Precedence
    //
    // TODO Handle channel 8 or over. (i.e. Mimics)
    //
    unsigned char chnBit = (1<<chn);
    unsigned char idx;
    if (   OPER_STATE_IS_NOT_FROZEN
        && ( (chnBit & doutMask1)  != 0 ) ) // has this channel ben reserved?
    {
        unsigned char oldDigSts = LATD;
        UDPPkt.action = action;   // may get changed in switch stetement
        UDPPkt.curValH = dwellValue >> 8;
        UDPPkt.curValL = dwellValue & 0xFF;
        switch (action)
	{
	    case DIN_GROUP1_ON:
	    case DIN_GROUP1_UP:
	    case DIN_GROUP1_NEXT:
	        LATD|=chnBit;		// turn on channel
	        DigDwells[chn] = 0 ;			// cancel any existing dwell
	        break ;

	    case DIN_GROUP1_OFF:
	    case DIN_GROUP1_DOWN:
	    case DIN_GROUP1_PREV:
	        LATD&=~chnBit;		// turn off channel
	        DigDwells[chn] = 0 ;			// cancel any existing dwell
	        break ;

	    case DIN_GROUP1_TOGGLE:
	        LATD^=chnBit;		// Toggle
	        DigDwells[chn] = 0 ;			// cancel any existing dwell
                if ( (LATD & chnBit ) != 0 )  // Now On
                {
                    UDPPkt.action = DIN_GROUP1_ON;
                }
                else
                {
                    UDPPkt.action = DIN_GROUP1_OFF;
                }
	        break ;

	    case DIN_DWELL_ON:
                // currently off or in a dwell
                if ( ( ( LATD & chnBit ) == 0 ) || ( DigDwells[chn] > 0 ) )
                {
		    LATD|=chnBit;		// turn on channel
		    DigDwells[chn] = dwellValue;	// set Dwell
                    UDPPkt.action = DIN_GROUP1_ON;
                }
	        break ;

	    case DIN_DWELL_OFF:
                // if channel on switch off after a dwell.
                if ( ( LATD & chnBit ) != 0 )
                {
		    DigDwells[chn] = dwellValue;	// set Dwell
                }
	        break ;

	    case DIN_DWELL_CANCEL:
                if ( (LATD & chnBit ) != 0 )  // currently On
                {
                    // cancel dwell
		    DigDwells[chn] = 0 ;			// Zero Dwell
		    LATD&=~chnBit ;			// turn off channel
                    UDPPkt.action = DIN_GROUP1_OFF;
                }
                else
		{
		    DigDwells[chn] = dwellValue;	// set Dwell
		    LATD|=chnBit;			// turn on channel
                    UDPPkt.action = DIN_GROUP1_ON;
		}
	        break ;


	    case DIN_DWELL_ALWAYS:
                // we do not care whether channel already on it is going on NOW!
                // if already on we extend dwell
		LATD|=chnBit;		// turn on channel
		DigDwells[chn] = dwellValue;	// set Dwell
                UDPPkt.action = DIN_GROUP1_ON;
	        break ;

	    case DIN_GROUP1_MOMENTARY:
	        LATD|=chnBit;		    // turn on channel
	        ShrtDwells[chn] = MOMENTARTYTIME;   // set dwell to low value using Mw
                UDPPkt.action = DIN_GROUP1_ON;
	        break ;
	
	    case DIN_GROUP1_NONE:
	        break;

	}   // end switch

        if ( oldDigSts != LATD )
        {
	        // reduce siteplayer traffic.
	        SPWriteChr(DOutStsLoc, LATD);
	
	        if ( ( action != DIN_GROUP1_NONE ) && UDP_SEND(OPT_UDPSendDOutChange) )
	        {
	            // Send UDP packet with new value.
	            UDPPkt.udpType = 'G';
	            UDPPkt.source[0] = 'D';
	            UDPPkt.source[1] = 'O';
	            UDPPkt.srcChannel = chn;   // source channel.
	
	            SendUDP();
	        }
	}
	
#ifdef MimicUsed
        idx = mimicForDig[chn/2];
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
            SetMimicState( idx, ( LATD & chnBit ) );
        }
#endif
    }
}

// called at 1 second interval, handles dwells for digital outputs
void DigitalPeriodic()
{
    unsigned char idx;
    for (idx = 0 ; idx < DOutEntryCount; ++idx)
    {
	if (DigDwells[idx] > 0) 
	{
	    --DigDwells[idx];			// decrement dwell
	    if (DigDwells[idx] == 1) 
            {
                DigitalOut( idx, DIN_GROUP1_OFF ) ; 	// switch off channel
            }
        }
    }
}

#ifdef MimicUsed
// there are only 8 digital outputs.
void DigitalMimicInit()
{
    unsigned char idx;
    unsigned char idx2;
    for ( idx2 = 0 ; idx2 < DOutPrimaryCount; ++idx2 )
    {
        idx = mimicForDig[idx2/2];
        if ( idx2 & 0x01 )
        {
            idx = idx >> 4;
        }
        else
        {
            idx = idx & 0x0F;
        }
        if ( idx < MimicCount )
        {
            SetMimicState( idx, ( LATD & (1<<idx2) ) );
//            if ( ( LATD & (1<<idx2) ) == 0 )
//            {
//                UpdateMimic( idx, mimicOff );
//            }
//            else
//            {
//                UpdateMimic( idx, mimicOn );
//            }
        }
    }
    if ( isMimicHighVolts(EEReadByte(MimicFlagsLoc)) )
    {
        Mimic12V();
    }
    else
    {
        Mimic5V();
    }
}
#endif

void DigitalInit()
{
    DigitalWork[0].pending = DigitalWork[0].state = PORTB;
    DigitalWork[1].pending = DigitalWork[1].state = READ_MONITORS;
    DigitalWork[0].lockout = DigitalWork[1].lockout = 0xFF;
    LATD = 0;
    SPWriteChr(DOutStsLoc, LATD);
    SPWriteChr(DInLoc, DigitalWork[0].state);
    SPWriteChr(DInLoc+1, DigitalWork[1].state);
}

// we have found a valid trigger now do it.
void DigitalAction( unsigned char idx )
{
    DigCounts[idx]++;  // count trigger
    // start counter
    ClickTimers[idx] = CLICKTIME;
    PerformTrigger( 'D', idx, (DInBloc + (idx*DInCfgSize)) );
}

//
// Handles up to 8 inputs on a single port.
//
// Expected to be called at approx 1 millisecond intervals
//  adjust DF_DEB_TIME for a reasonable debounce time.
//  Call could be at a lower interval if required.
//
// extended to handle a rotary encoder on pairs of digital inputs, 
// these need to be an event numbered input (0,2,4,6,8,10) and the next input. i.e. (0,1) , (8,9)
//
// contemplation:
// require an input change to be at l;east 1mS but then lock out for 20mS
//
static unsigned char newState;
static unsigned char newPending;
static unsigned char oldPending;
static unsigned char bt;
static unsigned char changes;
static unsigned char optByte;

void ProcessDigitalInPort( unsigned char dSet, unsigned char curState, 
                                            unsigned char firstNr, unsigned char lastNr )
{
    unsigned char idx;
    unsigned char idx2;
    newState = DigitalWork[dSet].state;
    newPending = 0;
    bt = 0x01;    // The current bit we are manipulating

    //unsigned char changes = oldState ^ curState;
    changes = (newState ^ curState) & DigitalWork[dSet].lockout;

    if ( changes != 0 )
    {
        // A change do we need 
        oldPending = DigitalWork[dSet].pending;
        for ( idx = firstNr; idx < lastNr; ++idx )  // this does not run 0 to 7, currently called with 0,7 and 8,11
        {
            if ( ( changes & bt ) != 0 )   // has this bit changed
            {
                if ( ( oldPending & bt ) == 0 )   // did we see it last time
                {
                    // no
                    newPending |= bt; // mark pending
                }
                else
                {
                    optByte = EEReadByte( (DInBloc + (idx*DInCfgSize)) + DInOptionsOs);   // cache ??
                    DebounceTimers[idx] = DF_DEB_TIME;  // actually lockout time.

                    newState ^= bt; // toggle the bit to store change
                    DigitalWork[dSet].lockout ^= bt; // toggle the bit to lockout channel

                    if ( DIN_B5_IS_RotaryEncoder(optByte) )
                    {
#ifdef DO_MP_ROTENC
// this variant produces multiple pulses per cycle, i.e. 4 * 24
                        unsigned char oldRe;
                        unsigned char newRe;
                        // detect direction and call correct action.
                        idx2 = idx & 0x6;   // get bits only
                        oldRe = (DigitalWork[dSet].state >> idx2) & 0x03;// now have 0-3
                        newRe = (newState >> idx2) & 0x03;// now have 0-3

                        if ( ( rotaryMpStateTable[oldRe] & ( 0x01 << newRe ) ) == 0 )
                        {
                            DigitalAction( idx&~0x01 ); // rotary down, mask bottom bit.
                        }
                        else
                        {
                            DigitalAction( idx|0x01 );   // rotary up.
                        }
#else
                        idx2 = idx & 0x6;   // get an even number that can isolate the bits we want in the byte
                        if ( ( newState & ( 0x03 << idx2 ) ) == 0 ) // both inputs now low, i.e. mid cycle.
//                        if ( ( newState & rotaryBitPairMasks[idx] ) == 0 ) // both inputs now low, i.e. mid cycle.
                        {
                            // was input a high before, i.e. a leads b.
                            if ( ( DigitalWork[dSet].state & ( 0x01 << idx2 ) ) != 0 )
//                            if ( ( DigitalWork[dSet].state & rotaryBitUpMasks[idx] ) != 0 )
                            {
                                DigitalAction( idx|0x01 );   // rotary up.
                            }
                            else
                            {
                                DigitalAction( idx&~0x01 ); // rotary down, mask bottom bit.
                            }
                        }
#endif
                    }
                    else
                    if (   ( ( ( curState & bt ) != 0 ) && (DIN_B5_IS_RisingEdge(optByte) ) )
                        || ( ( ( curState & bt ) == 0 ) && (DIN_B5_IS_FallingEdge(optByte) ) ) )
                    {
                        DigitalAction( idx );
                    }
                }
            }

            bt <<= 1;  // shift bit left.
        }
        SPWriteChr(DInLoc + (firstNr >> 3), newState);	// tell the outside world
        DigitalWork[dSet].state = newState;
    }
    DigitalWork[dSet].pending = newPending;

    bt = 0x01;
    for ( idx = firstNr; idx < lastNr; ++idx )  // this does not run 0 to 7, currently called with 0,7 and 8,11
    {
        // reset lockout timers
        if ( DebounceTimers[idx] > 0 )
        {
	    --DebounceTimers[idx];			// decrement dwell
	    if (DebounceTimers[idx] == 0) 
            {
                DigitalWork[dSet].lockout ^= bt; // toggle the bit to un-lockout channel (should)
            }
        }
        bt <<= 1;  // shift bit left.

        // reset click counts.
        if ( ClickTimers[idx] > 0 )
        {
	    --ClickTimers[idx];			// decrement dwell
	    if (ClickTimers[idx] == 0) 
            {
                DigCounts[idx] = 0;
            }
        }
    }
}


//
// Expected to be called at approx 1 millisecond intervals
//  adjust DF_DEB_TIME for a reasonable debounce time.
//
// digital inputs could also be rotary encoders.
//
void CheckDigital()
{
    unsigned char idx;
    if ( OPER_STATE_DO_INPUTS )
    {
        ProcessDigitalInPort( 0, PORTB, 0, 8 );
        ProcessDigitalInPort( 1, (READ_MONITORS), 8, 12 );
    }

    for (idx = 0 ; idx < DOutEntryCount; ++idx)
    {
	if (ShrtDwells[idx] > 0) 
	{
	    --ShrtDwells[idx];			// decrement dwell
	    if (ShrtDwells[idx] == 1) 
            {
                DigitalOut( idx, DIN_GROUP1_OFF ) ; 	// switch off channel
            }
        }
    }
}

