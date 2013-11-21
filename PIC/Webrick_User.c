//
//  Custom Functionality defines
//
// #define AH_Lampost

//
// This file includes all user definable routines. It may be changed at will as
// it will not be regenerated once the application has been generated for the
// first time.
//
#ifdef __18F452
//...............Error should be building for 18f452.
#pragma __config _CONFIG1H,_OSCS_OFF_1H & _HS_OSC_1H
#pragma __config _CONFIG2H,_WDT_OFF_2H
#pragma __config _CONFIG4L,_STVR_ON_4L & _LVP_OFF_4L
#endif

#ifdef __18F4525
#define NOWATCHDOG  1
#pragma __config _CONFIG1H,_OSC_HS_1H & _IESO_OFF_1H & _FCMEN_OFF_1H
//#pragma __config _CONFIG2L,_PWRT_ON_2L & _BOREN_OFF_2L
#pragma __config _CONFIG2L,_PWRT_ON_2L & _BOREN_ON_2L & _BORV_46_2L
#ifdef NOWATCHDOG
#pragma __config _CONFIG2H,_WDT_OFF_2H & _WDTPS_32768_2H
#else
#pragma __config _CONFIG2H,_WDT_ON_2H & _WDTPS_8192_2H
#endif
#pragma __config _CONFIG3H,_MCLRE_ON_3H & _LPT1OSC_OFF_3H & _PBADEN_OFF_3H & _CCP2MX_PORTC_3H
#pragma __config _CONFIG4L,_STVREN_ON_4L & _LVP_OFF_4L & _XINST_OFF_4L & _DEBUG_OFF_4L
#pragma __config _CONFIG5L,_CP0_OFF_5L & _CP1_OFF_5L & _CP2_OFF_5L & _CP3_OFF_5L
#pragma __config _CONFIG5H,_CPB_OFF_5H & _CPD_OFF_5H
#pragma __config _CONFIG6L,_WRT0_OFF_6L & _WRT1_OFF_6L & _WRT2_OFF_6L & _WRT3_OFF_6L
#pragma __config _CONFIG6H,_WRTB_OFF_6H & _WRTC_OFF_6H & _WRTD_OFF_6H
#pragma __config _CONFIG7L,_EBTR0_OFF_7L & _EBTR1_OFF_7L & _EBTR2_OFF_7L & _EBTR3_OFF_7L
#pragma __config _CONFIG7H,_EBTRB_OFF_7H

#else

#if APROCFREQ == 40000000
#pragma __config _CONFIG1H,_OSC_HSPLL_1H & _IESO_OFF_1H & _FCMEN_OFF_1H
#else
#pragma __config _CONFIG1H,_OSC_HS_1H & _IESO_OFF_1H & _FCMEN_OFF_1H
#endif

//#pragma __config _CONFIG2L,_PWRT_ON_2L & _BORV_43_2L & _BOREN_OFF_2L
#pragma __config _CONFIG2L,_PWRT_ON_2L & _BOREN_ON_2L & _BORV_46_2L
//#pragma __config _CONFIG2L,_PWRT_ON_2L & _BORV_46_2L & _BOREN_ON_2L
#ifdef NOWATCHDOG
#pragma __config _CONFIG2H,_WDT_OFF_2H & _WDTPS_32768_2H
#else
#pragma __config _CONFIG2H,_WDT_ON_2H & _WDTPS_8192_2H
#endif
#pragma __config _CONFIG3H,_MCLRE_ON_3H & _LPT1OSC_OFF_3H & _CCP2MX_PORTC_3H
#pragma __config _CONFIG4L,_STVREN_ON_4L & _LVP_OFF_4L & _BBSIZ_BB2K_4L & _XINST_OFF_4L & _DEBUG_OFF_4L

#ifdef __18F6622
// 64K memory
#pragma __config _CONFIG5L,_CP0_ON_5L & _CP1_ON_5L & _CP2_ON_5L & _CP3_ON_5L
#pragma __config _CONFIG6L,_WRT0_ON_6L & _WRT1_ON_6L & _WRT2_ON_6L & _WRT3_ON_6L
#pragma __config _CONFIG7L,_EBTR0_ON_7L & _EBTR1_ON_7L & _EBTR2_ON_7L & _EBTR3_ON_7L
#else
#pragma __config _CONFIG5L,_CP0_ON_5L & _CP1_ON_5L & _CP2_ON_5L & _CP3_ON_5L & _CP4_ON_5L & _CP5_ON_5L & _CP6_ON_5L & _CP7_ON_5L
#pragma __config _CONFIG6L,_WRT0_ON_6L & _WRT1_ON_6L & _WRT2_ON_6L & _WRT3_ON_6L & _WRT4_ON_6L & _WRT5_ON_6L & _WRT6_ON_6L & _WRT7_ON_6L
#pragma __config _CONFIG7L,_EBTR0_ON_7L & _EBTR1_ON_7L & _EBTR2_ON_7L & _EBTR3_ON_7L & _EBTR4_ON_7L & _EBTR5_ON_7L & _EBTR6_ON_7L & _EBTR7_ON_7L
#endif
#pragma __config _CONFIG5H,_CPB_ON_5H & _CPD_OFF_5H
#pragma __config _CONFIG6H,_WRTB_ON_6H & _WRTC_ON_6H & _WRTD_OFF_6H
#pragma __config _CONFIG7H,_EBTRB_ON_7H
#endif
//
//   Library include sections
//
//

#include "Webrick_Auto.h"

//
static int lastMsApprox = 0;            // Monitor the timer overflow count used by dig clock.
static int lastSecond = 0xFF;           // Monitor second timer
static unsigned int heartBeatTmr = 100;            // 
static unsigned char fadeTick = 2;	// rate of fade tick

#ifdef MimicUsed
static unsigned char mimicFadeTick = 2;	// rate of fade tick
#endif

static unsigned char ResetCause = 0;
static unsigned InitStage = 3000;       // approx 3 seconds.
#ifdef AH_Lampost
static unsigned char FadeCount[8];      // fade count
static unsigned char FadeTarget[8];     // fade target
#endif
// This needs to moce to a pixie library file.
extern unsigned char T0_OfCount;

static unsigned upTimeMins = 0;                // how many minutes we been going

//*******************************************************************************
//
// Insert your interrupt handling code if required here. 
// Note quick interrupts are used so code must be simple
// See the manual for details of quick interrupts.
//

void UserInterrupt()
{
	// Insert your code here
 #asmline goto UserIntReturn	; PIC Assembler - go back to interrupt routine
}

//*******************************************************************************
//
// Insert your initialisation code if required here. 
// Note that when this routine is called Interrupts will not be enabled - the 
// Application Designer will enable them before the main loop
//
void UserInitialise()
{
    OperState = OPER_STATE_HOLIDAY;

    INTCON2&=~(1<<7) ; 	// clear bit 7 to enable pullups on PORTB
//    ADCON1 = 142 ;		// channel A/D right justified output
//    InitStage = 30000;  	// zero when initialise has finished, we count down from here.


    ResetCause = RCON;
    RCON = 0x1F;	// set all reset bits.
    // bug in Wizc uses Port to initialise outputs.
    LATD = 0;
    LATE = 0;
    LATF &= 0x3F;
    LATC &= 0xDF;
}

// called from HMS element - minute occurence.
void sendTime()
{
    ++upTimeMins;
    UDPPkt.udpType = 'G';

    UDPPkt.source[0] = 'S';
    UDPPkt.source[1] = 'T';
    UDPPkt.srcChannel = Hours;
    UDPPkt.tgtChannel = Mins;
    UDPPkt.toNodeNr = Secs;
    UDPPkt.setPointNr = Day;
    UDPPkt.curValH = upTimeMins >> 8;
    UDPPkt.curValL = upTimeMins;
    UDPPkt.action = ResetCause;

    SendUDP() ;
}


void sendSPResetReason()
{
    //
	//  This is a re-use of the ST packet with '0' as the action, because this cannot occur normally in RCON
	//  Also note that we do not update the uptime
	//
    UDPPkt.udpType = 'G';

    UDPPkt.source[0] = 'S';
    UDPPkt.source[1] = 'T';
    UDPPkt.srcChannel = Hours;
    UDPPkt.tgtChannel = Mins;
    UDPPkt.toNodeNr = Secs;
    UDPPkt.setPointNr = Day;
    UDPPkt.curValH = upTimeMins >> 8;
    UDPPkt.curValL = upTimeMins;
    UDPPkt.action = 0;

    SendUDP() ;
}




#ifdef AH_Lampost
void DoLampPost()
{
    unsigned idx = 0;
    if (RBehaviour == RAND_3RANDOM)
    {
        idx = 3;
    }
    else
    if (RBehaviour == RAND_6RANDOM)
    {
        idx = 6;
    }
    while ( idx > 0 )
    {
        --idx;
	// handle fades
        if (   (FadeCount[idx] != 0) 
            && (msTick == 1) )
        {
            LATD|=(1<<idx) ;	        // cct ON if non-zero count
        }

        if (FadeCount[idx] == msTick) 
        {
            LATD&=~(1<<idx) ;  		// ccts to OFF	
        }
        if (msTick == FadeRateMax)		// index fade
        {	
            if (FadeCount[idx] > FadeTarget[idx]) 
            {
                --FadeCount[idx];
            }
            if (FadeCount[idx] < FadeTarget[idx]) 
            {
                ++FadeCount[idx];
            }
        }
    } 
}

void RandomActivity()
{
    int rnum ;		// the random number
    unsigned char idx;
    //
    //  Brute force approach to twinkling effect
    //
    //  Treat the first 6 ccts as if they are part of the random selection
    //   Should really undate this with a config setting later
    //
    switch (RBehaviour)
    {
	case RAND_ALL_OFF:
	    //
	    //  All ccts off
	    //
	    for (idx = 0 ; idx < 6 ; ++idx)
	    {
		PORTD&=~(1<<idx) ;  // ccts to OFF
	    } 
	break;
	
        case RAND_ALL_ON:
	    //
	    //  All ccts on
	    //
	    for (idx = 0 ; idx < 6 ; ++idx)
	    {
		PORTD|=(1<<idx) ;  // ccts to ON
	    } 
	    break ;

        case RAND_6RANDOM:
        case RAND_3RANDOM:
	    //
	    //  ccts to random state
	    //
	    rnum = rand() ;
	    for (idx = 0 ; idx < 6 ; ++idx)
	    {
	        if (rnum & (1<<idx))
		{
		    FadeTarget[idx] = FadeRateMax;
		}
	        else
		{
		    FadeTarget[idx] = 0;
		}
	    } 
	    break ;
	
	default:
	    break;
    } // end switch
}
#endif

//*******************************************************************************
//
// Insert your main loop code if required here. This routine will be called
// as part of the main loop code
//
void UserLoop()
{
#ifdef Tmr0IntUsed
    if ( T0_OfCount > 0 )
    {
        --T0_OfCount;
        DigClkTick();
    }
#endif    
    if ( InitStage > 0 )
    {
        if ( lastMsApprox != SubSecs )
        {
            lastMsApprox = SubSecs;
            // this is to keep each time round user loop short, mainly as simulator does not handle long watchdog timeouts.
            if ( InitStage == 40 )
            {
                SPWriteChr( CmdErrorLoc, ERR_Startup );
                if (FRESET_PUSHED)
                {
                    FactoryReset();  // factory reset
                }
            }
            else
            if ( InitStage == 31 )
            {
                // read in IP address
                SPReadBuffer( SPIPAddr, myMac, 4 );
                // if IP address is 192.168...
                // Then set to 10.100.100.252
                // go tight loop
                // clear watchdog.
                // allow siteplayer to be programmed
            }
            else
            if ( InitStage == 30 )
            {
                // read in MAC address
                SPReadBuffer( SPMacAddr, myMac, 6 );
            }
            else
            if ( InitStage == 25 )
            {
                VerifyPersistantCfg();
            }
            else
            if ( InitStage == 24 )
            {
                SPStartup();    // Ensure peristant data valid.
            }
            else
            if ( InitStage == 23 )
            {
#ifdef AH_Lampost
                srand(MyNodeNr);
#endif
            }
            else
            if ( InitStage == 22 )
            {
                DigitalInit();
                AnalogueInit();
                TempInit();
                SerialInit();
            }
            else
            if ( InitStage == 21 )
            {
                SendClockTime();                // to siteplayer
            }
            else
            if ( InitStage == 20 )
            {
                UDPEnable();		    // Allow inbound UDP
            }
            else
            if ( InitStage == 19 )
            {
                OperState = OPER_STATE_NORMAL;  // otherwise we cannot write to siteplayer.
                SPWriteChr(OperStateLoc, OperState);
                if ( EEReadByte(Password1Loc) == 0 )
                {
                    LoggedInState = 1;  // allow controls/view configuration.
                }
                SPWriteChr( LoginStateLoc, LoggedInState );
            }
            else
            if ( InitStage == 18 )
            {
#ifdef MimicUsed
                DigitalMimicInit();
                AnalogueMimicInit();
#endif
            }
            else
            if ( InitStage == 17 )
            {
                initIr();
            }
            else
            if ( InitStage == 16 )
            {
                TempFindSensors();
            }
            else
            if ( ( InitStage >= 10 ) && ( InitStage <= 14 ) )
            {
                TempReadSensors( InitStage-10 );
            }
            else
            if ( InitStage == 1 )
            {
                FlushCmds();
//                tryRtcRead();   // start time stuff off properly.
                SPWriteChr( CmdErrorLoc, ERR_NoCommand );
            }
            --InitStage;
        }
//        Wait( 1 );
    }
    else
    {
//        if ( OPER_STATE_IS_NOT_FROZEN )
        {
            // Monitor the timer overflow count used by dig clock.
            // nominally changed every milliscond.
            // But we may miss some if a previous operation takes too long, e.g. OneWire access
            // will take longer.
            if ( lastMsApprox != SubSecs )    
            {
                lastMsApprox = SubSecs;
                if (heartBeatTmr > 0)
                {
                    --heartBeatTmr;
                    if (heartBeatTmr == 0)
                    {
                        HARTBEAT_OFF;
                    }
                }
                CheckDigital();         // for new inputs and momentary expiry
	        --fadeTick;				// increment global rate tick
	        if (fadeTick == 0) 
	        {
                    fadeTick = FadeRate ;  // reset
	            AnaFade();
	        }
#ifdef MimicUsed
	        --mimicFadeTick;
	        if (mimicFadeTick == 0) 
	        {
                    mimicFadeTick = MimicFadeRate ;  // reset
	            FadeMimics();
	        }
#endif
                SerialReceivetimeout();    // check for characters waiting to send onwards

                // channel number updated as part of completion/occurence process.
                // Currently setup to read one channel every 0.8mS
                // need to play with accqusisition timer if we want to go faster simply.
                ANSTARTCONVERT();   // always trigger.
                CheckSerial();
            }

	    //
	    // Various tick increments
	    //  Rework to use DigClock Element and at second intervals.
	    if ( lastSecond != Secs )
            {
                // every two seconds switch on.
                if ( ( Secs & 0x01 ) == 0 )
                {
                    HARTBEAT_ON;
                    // logged in state is 0 - controls locked, 1 - normal, 2 configure , 3 installer
                    // this gives multipliers of 1,2,4,8
                    // at default setting of DF_HeartBeatUnit this is 1/4, 1/2, 1, 2 seconds
                    heartBeatTmr = (unsigned int)DF_HeartBeatUnit << (unsigned int)LoggedInState;
                }
                if (FRESET_PUSHED)
                {
                    doAlert = DF_HARD_ALERT_TIME;
                }
                lastSecond = Secs;		// reset count
                Do1SecondActivities();
                periodicSiteplayer();
#ifdef AH_lampost
                RandomActivity();
#endif
            }

        }
        CheckForCmds();
#ifdef AH_Lampost
        DoLampPost();
#endif		
    } // end if
}

//
// User occurrence code
//
