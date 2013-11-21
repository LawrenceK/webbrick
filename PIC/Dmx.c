//
//  Copyright L.P.Klyne 2013
//
//  Filename: dmx.h
//
//  Description:    DMX protocol functionality
//
//  Notes:
//
//
// Include files

#include "Webrick_Auto.h"

// Local static data
// target values for each DMX channel
static unsigned char DMX_Target[MAX_DMX_CHANNELS];

#ifdef _DMXSetScene
/*
*  --------------------------------------------------------
*    DMXSetScene, the contents of OpStr define a new scene
*  --------------------------------------------------------
*/
void DMXSetScene()
{
    unsigned char lc ;		//local count
    /*
    *  Very crude copy, might change to check that OpStr[3] is ':' later
    */

    //for (lc = 0 ; lc <= 3 ; lc++) DMXSet[lc] = OpStr[lc] ; 

    /*
    *  We do all fades on Scene 1
    */

    /*
    *    Note that this routine is getting Hijacked at the moment to test some fades
    *
    *
    */

    //SerialOut(&PORTE,1,0x03);  // DMX goto Scene
    //SerialOut(&PORTE,1,0x01);  // Scene 1
    //DMXdoFade = 3 ; // Assume there was a change so start the fade to new scene

//    RBehaviour = RAND_3RANDOM;	// not really a random behaviour but a fade test
//    for (lc = 0 ; lc <= 2 ; lc++) Ft[lc] = OpStr[lc] ;   //Ft is the target 
}

#endif

#ifdef _DMXFade
/*
*   ------------------------------------------------------
*	DMXFade - Handle changing DMX 'scene' gradually
*   ------------------------------------------------------
*/
void DMXFade()
{
    if (DMXdoFade) 
    {
        unsigned char lc ;		// local count
        unsigned char doit ;		// if set do something

        doit = false;

        for (lc = (DMXdoFade-1)*4 ; lc <= ((DMXdoFade-1)*4)+3 ; lc++)
        {
	    if (DMXAct[lc] != DMXSet[lc])
	    {
	        doit = true;
	        if (DMXAct[lc] > DMXSet[lc])
	        {
    	            DMXAct[lc]-- ;	// decrement
	        }
	        else
                {
		    DMXAct[lc]++ ;  // increment
	        }
	    }
        }

        if (doit)
        {
	    /*
	    *     Changes were made so prepare a string to go
	    */
	    SerialOut(&PORTE,1,0x01);  	// Start Loading data
	    SerialOut(&PORTE,1,0x00);  	// HighByte start chan
	    SerialOut(&PORTE,1,DMXdoFade);  // Low Byte start chan
	    /*
	    *  Now load the data
	    */
	    for (lc = (DMXdoFade-1)*4 ; lc <= ((DMXdoFade-1)*4)+3 ; lc++)
            {
	        SerialOut(&PORTE,1,DMXAct[lc]);  // send char at position
	    }
        }
        else
        {
	    --DMXdoFade;			// decrement
        }
    }
}
#endif

void DMXFade()
{
    unsigned char chn;
    for (chn = 0; chn<MAX_DMX_CHANNELS; ++chn)
    {
    	if ( S2_DmxGetLevel(chn) < DMX_Target[chn] )
    	{
	     S2_DmxSetLevel(chn, S2_DmxGetLevel(chn)+1);
	}
    	else if ( S2_DmxGetLevel(chn) > DMX_Target[chn] )
    	{
	     S2_DmxSetLevel(chn, S2_DmxGetLevel(chn)-1);
	}
    }
}

void DmxSetLevel( unsigned char chn, unsigned char level )
{
    // 0 is control byte, we support 1 to n (8?)
    if ( ( chn > 0 ) && ( chn <= MAX_DMX_CHANNELS ) )
    {
        // locate Tx2 buffer and directly update
        S2_DmxSetLevel(chn, level);
        //SetTxByte2(chn, level);
        // TODO set to fast refresh
    }
}

// state 0 idle
// state 1 mark before
// state 2 space
// state 3 mark after
// state 4..n sending channels.
#define dmx_SlowRefresh    200
#define dmx_FastRefresh    20
static unsigned char dmx_Refresh = dmx_FastRefresh;
static unsigned char dmx_State = 255;

// call every millisecond'ish
void DmxCheck( )
{
    ++dmx_State;
// alternate ways of getting the long break before DMX transmission.
#if 0
    if ( dmx_State == 1 )
    {
        DmxBreak();
        ResendTx2();
    }
#else
    if ( dmx_State == 1 )
    {
        // take over Tx pin and set low for very long break.
        DmxMBB();   // may not be needed. approx 10uS
        DmxB();
    }
    else
    if ( dmx_State == 2 )
    {
        DmxMAB(); // approx 10uS
        // start data send.
        ResendTx2();
    }
    else
/*
    if ( dmx_State == 1 )
    {
        // take over Tx pin and set low for very long break.
        Serial2Disable();
        setSerial2TxLow();
    }
    else
    if ( dmx_State == 2 )
    {
        setSerial2TxHigh();
//        #asmline DELAY 70    ; 7 uS at 40Mhz.
        // return Tx pin to EUSART
        Serial2Enable();
        // start data send.
        ResendTx2();
    }
    else
*/
#endif
    if ( dmx_State > dmx_FastRefresh )
    {
        // approx 20 mS.
//        DmxBreak();
//        ResendTx2();
        dmx_State = 0;  // restart
        // TODO set to slow refresh
    }
}

// called to enable the Dmx interface
// flush Tx
// flush Rx
// set baud
// set RX485
// preload Tx Buffer
void DmxStart( )
{
    FlushTx2();
    //preload Dmx Buffer
    unsigned idx;
    AddTx2(0);	// control byte
    for (idx = 0; idx < MAX_DMX_CHANNELS;++idx )
    {
        DMX_Target[idx] = 0;
        AddTx2(0);  // all zeroed
    }

    dmx_State = 0;  // start processing.
}

void DmxStop()
{
    dmx_State = 255;  // stop processing.
}
