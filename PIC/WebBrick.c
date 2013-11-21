//
//   WebBrick Library
//
//   Andy Harris, Extend Technologies, April 2003
//
//  This is a library of utilities to drive the siteplayer over the
//  standard serial port on a 18F452 (previously 16F877)
//
//
#include "Webrick_Auto.h"

// count down for period readings, set to 1 to provoke early reading
static unsigned char ltick = 1;                // long tick
unsigned char doAlert = 0;   // when greater than zero send alert packets every second?

// Local functions.

// Functions.
static void sendAlert()
{
    UDPPkt.udpType = 'G';
    UDPPkt.source[0] = 'A';
    UDPPkt.source[1] = 'A';
    if (MyNodeNr == DF_NODE_NUM)
    {
        UDPPkt.source[0] = 'N';
        UDPPkt.source[1] = 'N';
    }
    else 
    if ( doAlert > DF_SOFT_ALERT_TIME )
    {
        UDPPkt.source[0] = 'A';
        UDPPkt.source[1] = 'T';
    }

    UDPPkt.source[2] = myMac[0];
    UDPPkt.source[3] = myMac[1];
    UDPPkt.source[4] = myMac[2];
    UDPPkt.source[5] = myMac[3];
    UDPPkt.source[6] = myMac[4];
    UDPPkt.source[7] = myMac[5];
    SendUDP() ;
}

/*
void FanC(unsigned char temperature)
{
    //
    //  Analogue control of fan
    //
    //
    //  Use TempNominal as base and Analogue out 0 as appropriate  
    //
    //
    unsigned char fanpoint ; // used to control set point
    if (temperature >= TempNominal)
    {
	fanpoint = temperature - TempNominal ;
	if (fanpoint > 7) 
        {
            fanpoint = 7 ;	// upper bound
        }
//        else
//	if (fanpoint < 0) 
//        {
//            fanpoint = 0 ;	// lower bound
//        }
    }
    else
    {
	fanpoint = 0 ;   // cooler than set point
    }
    AnaCommand(0, DIN_B1_ACTION_DWELL, fanpoint, 30) ;   // dwell chan 0 at fanpoint for 30 seconds
}
*/

static void verifySitePlayerOpState()
{
    // read oper state 
    // This takes time as read is blocking at present, otherwise hard to separate 
    // Network inputs and this request.
    // NOT sure want to retain this.
    unsigned char res = 255;
    SPReadBuffer( OperStateLoc, &res, 1 );
    if ( res != OperState )
    {
        // error reload siteplayer
        SPStartup();
        RefreshDynamicData();
    }
}

void Do1SecondActivities()
{
    unsigned char idx;

    //
    //  Period actions
    //
    if ( LoggedInTimer > 0 )
    {
        --LoggedInTimer;
        if ( LoggedInTimer == 0 )
        {
            if ( EEReadByte(Password1Loc) == 0 )
            {
                LoggedInState = 1;  // allow controls/view configuration.
            }
            else
            {
                LoggedInState = 0;
            }
            SPWriteChr( LoginStateLoc, LoggedInState );
            if ( OPER_STATE_IS_FROZEN )
            {
                // Auto exit this state
                OperState = OPER_STATE_NORMAL;  // otherwise we cannot write to siteplayer.
            }
        }
    }

    DigitalPeriodic();
    AnaloguePeriodic();
    TemperaturePeriodic();

    if ( doAlert > 0 )
    {
        sendAlert();
        --doAlert;
    }

    if (--ltick == 0)
    {
        // 20 second interval stuff
        if ( OPER_STATE_IS_NOT_FROZEN )
        {
// Boot only or manual command.
//            TempFindSensors();
            if (MyNodeNr == DF_NODE_NUM)
            {
                sendAlert();
            }
//            checkNodeStatus();
// it is blocking better to just not worry.
// the aim was to detect the siteplayer rebooting. 
//            verifySitePlayerOpState();
        }
        ltick = 20 ;  // reset count
    }

#ifdef DEBUG_TEMP_SENSOR_TEST
    TempFindSensors();
    TempReadSensors( 0 );
    TempReadSensors( 1 );
    TempReadSensors( 2 );
    TempReadSensors( 3 );
    TempReadSensors( 4 );
    OWStartTempConversionAll();
#else
    // 1 sensor each time through
    TempReadSensors( ltick % MAX_SENSORS );
#endif    
}
