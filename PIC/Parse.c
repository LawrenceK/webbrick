//
//  Copyright L.P.Klyne 2013
//
//  Filename: Parse.c
//
//  Description: Contains all code for parsing incomming commands
//
//  Notes:
//
//  Thoughts
//  CI<0-1><N|L|H><A|0-7>;<Thresh>;:       (Configure Analogue triggers)
//      Analogue channel nr, Type (None, Low, High), (Action Alarm or Trigger)
// Include files
//
// --------------------------------------------------
//
//  New command structure
//
//  General:
//      D is dig input
//      O is dig output
//      I is analogue input
//      A is analogue output
//      T is temperature
//
//  ND<chn>;<nameStr>:  Name Dig Input
//  NO<chn>;<nameStr>:  Name Dig Output
//  NI<chn>;<nameStr>:  Name Analog Input
//  NA<chn>;<nameStr>:  Name Analog Output
//  NT<chn>;<nameStr>:  Name Temperature
//  NN<nameStr>:  Name node
//*  NS<nr>;<nameStr>:  Name Strings nr is 0 to max known strings. 0 is node Name.
//
//  A later option may allow multiple names 1 after the other, they name consecutive entitys
//  starting at <chn>
//  i.e. ND<chn>;<nameStr1>;<nameStr2>;<nameStr3>;<nameStr4>;<nameStr5>:
//
//  CD<chn>;<A|D|S><targetChn|scene>;<actionType>;<dwell>;<sp>;<udpType>;<nodeNr>;  Digital input
//  CI<chn>;<L|H>;<Threshold>;<A|D><targetChn>;<actionType>;<dwell>;<sp>;<udpType>;<nodeNr>;
//  CT<chn>;<L|H>;<Threshold>;<A|D><targetChn>;<actionType>;<dwell>;<sp>;<udpType>;<nodeNr>;
//  CS<chn>;<val>  Set point
//  CW<chn>;<val>  Dwell
//  CR<chn>;<anChn>  Configure rotary encoder.
//  CE<chn>;<Days>;<hours>;<Min>;<A|D><targetChn>;<actionType>;<dwell>;<sp>;<udpType>;<nodeNr>;  Digital input
//  CC<nr>;[NFI][NFI][NFI][NFI][NFI][NFI][NFI][NFI];[I|S<nn>];[I|S<nn>];[I|S<nn>];[I|S<nn>]:
//      Configure a scene. nr is the scene number.
//      Then a set of characters to set the digital states, oN or oFf or Ignore
//      Then the analogue channle setpoints, an I says no change otherwise it is a setpoint number
//      Any dig channel not given will be ignored they count from 0, same with analogue
//
//  SN<num>  Set node number
//  SF<rate>  Set Fade rate
//  ST<dd>;<hh>;<mm>  Set Time
//  SD<years>;<mon>;<date>  Set Date
//  SR<chn>;<val>  Set rotary encoder step
//  SI<ip1>;<ip2>;<ip3>;<ip4>  Set internet address
//  SS<state>  Set operational state
//  SO<nr>;[0|1]    Set option on (1) or Off(0)
//
//  DI<chn>  Trigger input
//  DO<chn>;<>  Set digital output.
//  DT<A|D|S><targetChn|scene>;<actionType>;<dwell>;<sp>;<udpType>;<nodeNr>;  Digital input
//
//  AA<chn>;[S<nn>|<nn>]  Set Analogue output setpoint or absolute
//
//  FR  Factory reset.
//  LG;<password>  Try to login.
//
//  RT  Rescan 1 wire bus.
//  RU  Refresh UI data
//  RB  Reboot PIC
//
//  X commands are private extensions.
//
#include "Webrick_Auto.h"

// index by CC code to get B1 code.
const unsigned char CC_TO_B1_MAP[] = {DIN_GROUP1_NONE, DIN_GROUP1_OFF, 
    DIN_GROUP1_ON, DIN_GROUP1_MOMENTARY,
    DIN_GROUP1_TOGGLE, 
    DIN_DWELL_ALWAYS, DIN_DWELL_CANCEL, 
    DIN_GROUP1_NEXT, DIN_GROUP1_PREV, 
    DIN_GROUP1_SET_LOW_THRESHOLD, DIN_GROUP1_SET_HIGH_THRESHOLD,
    DIN_GROUP1_ADJ_LOW_THRESHOLD, DIN_GROUP1_ADJ_HIGH_THRESHOLD,
    DIN_GROUP1_SEND_IR,
    DIN_GROUP1_UP, DIN_GROUP1_DOWN,
    DIN_GROUP1_SET_DMX, DIN_GROUP1_COUNT,
    DIN_DWELL_ON, DIN_DWELL_OFF };

// Static data (File scope)
// Buffer large as planning large name commands.
#define MAX_CMD_STR  64
static unsigned LogInTimeOutVal = DF_LoggedInTime;
static unsigned char OpStr[MAX_CMD_STR] ;                               // Operand String
static unsigned OpStr_Flushing = false; // flushing until we get good data.
static unsigned OpStrLen = 0;
static unsigned OpStrIdx = 0; // working index

// channel number
static unsigned cnum;   // not unsigned char due to wizc Bug.
static unsigned baseC;  // EEPROM Location
static unsigned baseV;  // EEPROM Location

// Now some private work data, we will go for global here to reduce stack usage.
static int ival = 0;
static unsigned uval = 0;
static int ineg = false;
static unsigned cmdError = 0;
//static unsigned char dwellNr;

//static unsigned char tValue;
//static unsigned char tOptions;
static unsigned char work;
static unsigned char idx;

// used in parsing date/time
static unsigned char days;
static unsigned char hours;
static unsigned char mins;
static unsigned char years;
static unsigned char month;
static unsigned char date;

#define MORE_INPUT  ( OpStrIdx < OpStrLen )
#define NO_MORE_INPUT  ( OpStrIdx >= OpStrLen )
#define END_INPUT   ( OpStrIdx >= OpStrLen )

//  In all the private parse routines OpStr contains the
//  Complete string, it may be modified if helpfull.
//  The colon will be nulled so a string is fully terminated.
//  Any trailing semi colon will be nulled as well.
//
static void GetANumber()
{
    ival = 0;
    ineg = false;
    if ( OpStr[OpStrIdx] == ';' )
    {
        ++OpStrIdx;
    }
    if ( OpStr[OpStrIdx] == '-' )
    {
        ineg = true;
        ++OpStrIdx;
    }
    while (    MORE_INPUT
            && ( OpStr[OpStrIdx] >= '0' )
            && ( OpStr[OpStrIdx] <= '9' ) )
    {
        ival = (ival*10) + (OpStr[OpStrIdx] - 48);
        ++OpStrIdx;
    }
    if (   MORE_INPUT
        && ( OpStr[OpStrIdx] == ';' ) )
    {
        ++OpStrIdx; // skip number terminator
    }
    if ( ineg )
    {
        ival = -ival;
    }
}

/*
    A temperature value is possibly negative and possibly fractional.
    with the decimal being in the lower 4 bits.
*/
static int GetTempVal()
{
    ival = 0;
    ineg = false;
    if ( OpStr[OpStrIdx] == ';' )
    {
        ++OpStrIdx;
    }
    if ( OpStr[OpStrIdx] == '-' )
    {
        ineg = true;
        ++OpStrIdx;
    }
    while (    MORE_INPUT
            && ( OpStr[OpStrIdx] >= '0' )
            && ( OpStr[OpStrIdx] <= '9' ) )
    {
        ival = (ival*10) + (OpStr[OpStrIdx] - 48);
        ++OpStrIdx;
    }

    if ( ival > 255 )
    {
        cmdError = ERR_BadParam;
        return 0; // bad input.
    }

    ival = ival << 4;  // move whole part into correct bits.

    if ( ( MORE_INPUT ) && ( OpStr[OpStrIdx] == '.' ) )
    {
        // fractional, single character
        int frac = 0;
        OpStrIdx++;
        // allow decimal number in format 52. with no fractional digit.
        if (   MORE_INPUT
            && ( OpStr[OpStrIdx] >= '0' )
            && ( OpStr[OpStrIdx] <= '9' ) )
            {
            frac = OpStr[OpStrIdx] - 0x30;  // single digit
            OpStrIdx++; // skip character
            if ( ( frac < 0 ) || ( frac > 9 ) )
            {
                cmdError = ERR_BadParam;
                return 0; // bad input.
            }
            frac = (frac << 4) / 10;  // convert to 16ths ((n/10)*16)
        }
        ival = ival + frac;
    }

    if ( ( MORE_INPUT ) && ( OpStr[OpStrIdx] == ';' ) )
    {
        ++OpStrIdx; // skip number terminator
    }

    if ( ineg )
    {
        ival = -ival;
    }
    return ival;
}

// Local functions
//
//  Called to handle any setting of a text name string
//  loc is location in peristant store
//  maxLen is maximum length
//  offset if location in OpStr to start from.
//  OpStr will be modified to replace semi colon by null.
//
unsigned char badChars[] = "<>& %?:;+'\"";
//#define BADCHARS = "<>& %?:;+"

static void SetAName( unsigned char maxLen )
{
#ifdef DEBUG_TEMP_SENSOR_TEST
    cmdError = ERR_BadParam;
#else
    unsigned offset = OpStrIdx;    // Save start of name.
    // locate end of name string, null terminate and
    // point to after name.
    while (    MORE_INPUT
            && ( OpStr[OpStrIdx] != 0 )
            && ( OpStr[OpStrIdx] != ';' ) )
    {
        if ( strchr( badChars, OpStr[OpStrIdx]  ) != 0 )
        {
            // invalid characters, these are XMl special and cause issues with downloading the XML formatted data.
            cmdError = ERR_BadParam;
        }
        ++OpStrIdx;
    }
    if ( cmdError == 0 )
    {
        if ( OpStrIdx <= OpStrLen )
        {
            OpStr[OpStrIdx] = 0;
            ++OpStrIdx;
        }

        EEWriteStr(baseC, OpStr+offset, maxLen);
        SPWriteStr(baseC, OpStr+offset, maxLen);

        LoggedInTimer = LogInTimeOutVal; // seconds.
    }
#endif    
}

// Local functions
//
//  Called to handle any setting of a byte value
//  loc is location in peristant store
//  offset if location in OpStr to start from.
//  OpStr will be modified to replace next semi colon by null.
//
unsigned char SetAByte( unsigned loc )
{
    if ( LoggedInState > 1 )
    {
        GetANumber(); // into ival
        if ( ( ival >= 0 ) && ( ival <= 255 ) )
        {
            EEWriteByte( loc, (unsigned char)ival );
            SPWriteChr( loc, (unsigned char)ival );
        }
        else
        {
            cmdError = ERR_BadParam;
        }
        LoggedInTimer = LogInTimeOutVal;    // seconds.
    }
    else
    {
        cmdError = ERR_NotLoggedIn;
    }
    return EEReadByte( loc );
}

// Local functions
//
//  Called to handle any setting of a byte value
//  loc is location in peristant store
//  offset if location in OpStr to start from.
//  OpStr will be modified to replace next semi colon by null.
//
/*
static void SetAnUnsigned( unsigned loc )
{
    if ( LoggedInState > 1 )
    {
        GetANumber(); // into ival
        if ( ( ival >= 0 ) && ( ival <= 32767 ) )
        {
            EEWriteInt(loc, ival);
            SPWriteInt(loc, ival);
        }
        LoggedInTimer = LogInTimeOutVal;    // seconds.
    }
}
*/
// Local functions
//
static char GetASmallNumber()
{
    GetANumber(); // into ival
    if ( ( ival < -128 ) || ( ival > 255 ) )
    {
        ival = 0;
        cmdError = ERR_BadParam;
    }
    return (char)ival;
}

static unsigned char GetAByte()
{
    GetANumber(); // into ival
    if ( ( ival < 0 ) || ( ival > 255 ) )
    {
        ival = 0;
        cmdError = ERR_BadParam;
    }
    return (unsigned char)ival;
}

static void GetAnUnsigned()
{
    uval = 0;
    GetANumber(); // into ival
    if ( ival >= 0 )
    {
        uval = ival;
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//
//  Decodes the received input into its trigger values
//
static int DecodeTrigger()
{
    if ( OpStr[OpStrIdx] == 'A' )
    {
        chnType = DIN_ACTYPE_ANALOGUE;
    }
    else
    if ( OpStr[OpStrIdx] == 'D' )
    {
        chnType = DIN_ACTYPE_DIGITAL;
    }
    else
    if ( OpStr[OpStrIdx] == 'S' )
    {
        chnType = DIN_ACTYPE_SCENE;
    }
    else
    if ( OpStr[OpStrIdx] == 'T' )
    {
        chnType = DIN_ACTYPE_TEMPERATURE;
    }
    else
    if ( OpStr[OpStrIdx] == 'I' )
    {
        chnType = DIN_ACTYPE_INFRARED;
    }
    else
    if ( OpStr[OpStrIdx] == 'X' )
    {
        chnType = DIN_ACTYPE_DMX;
    }
    else
    {
        cmdError = ERR_BadParam;
    }
    ++OpStrIdx;

    if ( cmdError == 0 )    // no data error yet.
    {
        // Target channel number
        tgtChn = GetAByte();
        setPointNr = GetAByte();
        action = GetAByte();
        // convert action to valid value to store.
        if ( action < sizeof(CC_TO_B1_MAP) )
        {
            action = CC_TO_B1_MAP[action];
        }
        else
        {
            // error
            cmdError = ERR_BadParam;
        }

//        dwellNr = GetAByte();
        GetAnUnsigned();
        dwellValue = uval;
        udpDo = GetAByte(); // 0 or 1
        if ( udpDo > 1 )
        {
            udpDo = 1;
        }
        assocValue1 = GetASmallNumber();
        assocValue2 = 0;
        if (MORE_INPUT)
        {
            // Get associated value byte
            assocValue2 = GetAByte();
        }

        // TODO verify action againast target channel type and
        // restrict channel numbers further.

        // Validate values
        if (   ( cmdError == 0 )    // no data error yet.
            && (   ( ( chnType == DIN_ACTYPE_ANALOGUE) && ( tgtChn < 16 ) )
                || ( ( chnType != DIN_ACTYPE_ANALOGUE) && ( tgtChn < 64 ) ) )  // room for 6 bits when not analogue
//            && ( dwellNr < DwellCount ) // could be larger as decodes a dotrigger command that has a dwell time not index.
            && ( setPointNr < SPCount )
            && ( udpDo < 2 ) )
        {
            LoggedInTimer = LogInTimeOutVal;    // seconds.
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
    return ( cmdError == 0 );
}

//  <Xxxx><A|D><targetChn>;<actionType>;<dwell>;<sp>;<udpType>;<nodeNr>;  Digital input
//  <CD3>;<A|D><targetChn>;<actionType>;<dwell>;<sp>;<udpType>;<nodeNr>;  Digital input
//
// Helper to Store a trigger in the EEPROM and to the siteplayer
//
//  We use a trigger configuration in many places, e.g. Digital inputs temperature alarms.
//
static int StoreTrigger( unsigned loc )
{
    if ( DecodeTrigger() )
    {
        if (   ( dwellValue < DwellCount )
            && ( setPointNr < SPCount ) )
        {
            if (udpDo)
            {
                udpDo = DIN_UDP_SENDUDP;
            }
            if ( DIN_B1_COMMAND_GROUP(action) == DIN_GROUP_DWELL)
            {
                b1Work = DIN_B1_DWELL_MAKE( action, dwellValue, udpDo );
            }
            else
            {
                b1Work = DIN_B1_GROUP1_MAKE( action, udpDo );
            }
            if ( chnType == DIN_ACTYPE_ANALOGUE )
            {
                b2Work = DIN_B2_AN_MAKE( tgtChn, setPointNr );
            }
            else
            {
                b2Work = DIN_B2_MAKE( tgtChn, chnType );
            }

            EEWriteByte(loc+DInActionOs, b1Work);
            EEWriteByte(loc+DInChanOs, b2Work);
            EEWriteByte(loc+DInValue1Os, assocValue1);
            EEWriteByte(loc+DInValue2Os, assocValue2);
            SPWriteChr(loc+DInActionOs, b1Work);
            SPWriteChr(loc+DInChanOs, b2Work);
            SPWriteChr(loc+DInValue1Os, assocValue1);
            SPWriteChr(loc+DInValue2Os, assocValue2);
            if (( loc >= DInBloc ) && ( loc < DInBlocEnd ))
            {
                if (MORE_INPUT)
                {
                    // Get options byte
                    work = GetAByte();
                    EEWriteByte(loc+DInOptionsOs, work);
                    SPWriteChr(loc+DInOptionsOs, work);
                }
            }
        }
        else
        {
            // decode allows for dwell to be passed for DoTrigger command
            cmdError = ERR_BadParam;
        }
    }
    return ( cmdError == 0 );
}

//  ST<dd>;<hh>;<mm>  Set Time
static void SetTime()
{
    if ( LoggedInState > 1 )
    {
        days = GetAByte();
        hours = GetAByte();
        mins = GetAByte();
        if ( ( days < 7 ) && ( hours < 24 ) && ( mins < 60 ) )
        {
            SetClock( days, hours, mins, 0 );
        }
        else
        {
            cmdError = ERR_BadParam;
        }
        LoggedInTimer = LogInTimeOutVal;    // seconds.
    }
    else
    {
        cmdError = ERR_NotLoggedIn;
    }
}

//  SD<years>;<mon>;<date>  Set Date
static void PSetDate()
{
    if ( LoggedInState > 1 )
    {
        years = GetAByte();
        month = GetAByte();
        date = GetAByte();
        if ( ( years < 99 ) && ( month < 12 )  && ( date < 31 ) )
        {
            SetDate( years, month, date );
        }
        else
        {
            cmdError = ERR_BadParam;
        }
        LoggedInTimer = LogInTimeOutVal;    // seconds.
    }
    else
    {
        cmdError = ERR_NotLoggedIn;
    }
}

// SPl;password
static void SetPassword()
{
    if ( LoggedInState > 1 )
    {
        unsigned char level;
        level = GetAByte();
        if ( level == 1 )
        {
            EEWriteStr(Password1Loc, OpStr+OpStrIdx, PasswordLen);
        }
        else
        if ( level == 2 )
        {
            EEWriteStr(Password2Loc, OpStr+OpStrIdx, PasswordLen);
        }
        else
        if ( level == 3 )
        {
            EEWriteStr(Password3Loc, OpStr+OpStrIdx, PasswordLen);
        }
        else
        {
            cmdError = ERR_BadParam;
        }
        LoggedInTimer = LogInTimeOutVal;    // seconds.
    }
    else
    {
        cmdError = ERR_NotLoggedIn;
    }
}

//  SI<ip1>;<ip2>;<ip3>;<ip4>  Set internet address
static void SetIpAddress()
{
    //
    //  Routine to configure the IP address, expect to see OpStr as:
    //
    //   IA byte ; byte ; byte ; byte
    //  IA<n>;<n>;<n>;<n>;: (n is 0-255)    (Set IP address)
    //
    if ( LoggedInState > 1 )
    {
        if ( ( Options[OPT_SecurityFlags] & 0x01 ) != 0 )
        {
            cmdError = ERR_AddressLocked;
        }
        else
        {
            unsigned char newIP[4];     // This will hold the new IP address if valid
            for ( idx = 0; idx < 4; ++idx )
            {
                newIP[idx] = GetAByte();
            }

            // We should have a good IP address with 4 elements
            // Nothing should be 0 or 255.
    //        if (   ( newIP[0] != 0 )
    //            && ( newIP[0] != 255 )
    //            && ( newIP[1] != 0 )
    //            && ( newIP[1] != 255 )
    //            && ( newIP[2] != 0 )
    //            && ( newIP[2] != 255 )
    //            && ( newIP[3] != 0 )
    //            && ( newIP[3] != 255 ) )
            if ( cmdError == 0 )    // all bytes between 0 and 255 inclusive.
            {
                EEWriteBuffer(IPAddrLoc, newIP, 4);
                SPWriteBuffer(SPIPAddr,newIP,4);
                LoggedInTimer = LogInTimeOutVal;    // seconds.
            }
    //        else
    //        {
    //            cmdError = ERR_BadParam;
    //        }
        }
    }
    else
    {
        cmdError = ERR_NotLoggedIn;
    }
}

// reads 10 bytes and verifies the MAC address against self.
static void SetAddressVerifyMac()
{
    if ( LoggedInState > 1 )
    {
        unsigned char newMac[6];        // This will hold the new IP address if valid
        for ( idx = 0; idx < 6; ++idx )
        {
            newMac[idx] = GetAByte();
        }
        idx = 0;
        while ( (idx < 6) && (newMac[idx] == myMac[idx]) )
        {
            ++idx;
        }
        if ( idx == 6 )
        {
            SetIpAddress();
        }
    }
}

//  CW<chn>;<val>  Dwell
static void CfgDwell()
{
    //
    //  First extract Dwell num
    //
    cnum = GetAByte();
    if ( cnum < DwellCount )
    {
        unsigned dwellLoc;
        GetAnUnsigned();
        dwellLoc = DwellBaseLoc + (cnum * DwellEntryLen);
        if ( uval < 2 )
        {
            uval = 2;    // minimum
        }

        DwellTimes[cnum] = uval;
        EEWriteInt(dwellLoc, uval);
        SPWriteInt(dwellLoc, uval);
        LoggedInTimer = LogInTimeOutVal;    // seconds.
    }
}

//  NA<0-1><name>;:                     (Configure analogue)
static void NameAnOut()
{
    cnum = GetAByte();
    if ( cnum < AnOutEntryCount )
    {
        baseC = AnOutNameBase + (cnum * AnOutNameLen);
        SetAName( AnOutNameLen );
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  NI<0-1><name>;:                     (Configure analogue)
static void NameAnIn()
{
    cnum = GetAByte();
    if ( cnum < AnInEntryCount )
    {
        baseC = AnInNameBase + (cnum * AnInNameLen);
        SetAName( AnInNameLen );
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  NT<0-5><name>;:                     (Configure analogue)
static void NameTemperature()
{
    cnum = GetAByte();
    if ( cnum < TempCount )
    {
        baseC = TempNameBase + (cnum * TempNameLen);
        SetAName( TempNameLen );
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  CO<0-7><Name>;:                     (Configure dig out)
static void NameDigOut()
{
    cnum = GetAByte();
    if ( cnum < DOutPrimaryCount )
    {
        baseC = DOutNameBase + (cnum * DOutNameLen);
        SetAName( DOutNameLen );
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  CO<0-7><Name>;:                     (Configure dig out)
static void NameDigIn()
{
    cnum = GetAByte();
    if ( cnum < DInEntryCount )
    {
        baseC = DInNameBase + (cnum * DInNameLen);
        SetAName( DInNameLen );
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  SR<val>  Set rotary encoder step
//*  SR<chn>;<val>  Set rotary encoder step
static void SetRotStep()
{
    cnum = GetAByte();
    if ( cnum == 0 )
    {
        RotStep = SetAByte( RotStepLoc );
    }
    else
    if ( cnum == 1 )
    {
        // do nothing, webbrick 6.3 fix
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

static void SetOption()
{
    if ( LoggedInState > 1 )
    {
        cnum = GetAByte();      // option number
        action = GetAByte();    // option value
        if (   ( cnum < MaxOptions )
            && ( cnum >= MinOptions ) )
        {
            Options[cnum] = action;
            EEWriteByte( OptionsLoc+cnum, Options[cnum] );
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
}

static void SetSecOption()
{
    cnum = GetAByte();      // option number
    action = GetAByte();    // option value
    if ( cnum < MaxOptions )
    {
        Options[cnum] = action;
        EEWriteByte( OptionsLoc+cnum, Options[cnum] );
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  CM<0-3><Name>;:                     (Configure monitor)
//static void NameMonitor()
//{
//    cnum = GetAByte();
//    if ( cnum < MonEntryCount )
//    {
//        baseC = MonNameBase + (cnum * MonNameLen);
//        SetAName( MonNameLen );
//    }
//    else
//    {
//        cmdError = ERR_BadParam;
//    }
//}

//  CD<chn>;<A|D><targetChn>;<actionType>;<dwell>;<sp>;<udpType>;<nodeNr>;  Digital input
static void CfgDigIn()
{
    cnum = GetAByte();
    if ( cnum < DInEntryCount )
    {
        StoreTrigger( DInBloc + (cnum*DInCfgSize) );
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

static void DoTrigger()
{
    // May consider letting a DoTrigger send arbitrary analogue value
    // but for now you can use the analogue out command.
    if ( DecodeTrigger() )
    {
        ActionTrigger( 'X', 0 );
    }
}

//  CE<chn>;<Days>;<hours>;<Min>;<A|D><targetChn>;<actionType>;<dwell>;<sp>;<udpType>;<nodeNr>;  Digital input
static void CfgSchedEvent()
{
    cnum = GetAByte();
    if ( cnum < SchedEntryCount )
    {
        days = 0;
        while (    ( OpStr[OpStrIdx] >= '0' )  // Sunday
                && ( OpStr[OpStrIdx] <= '6' ) )
        {
            days |= ( 1 << (OpStr[OpStrIdx]-'0') );
            ++OpStrIdx;
        }

        hours = GetAByte();
        mins = GetAByte();

        baseC = SchedBaseLoc + (cnum*SchedEntryLen);
        if (   ( days >= 0 )
            && ( days <= 0x7F )
            && ( hours >= 0 )
            && ( hours < 24 )
            && ( mins >= 0 )
            && ( mins < 60 )
            && StoreTrigger( baseC + SchedActionOs ) )
        {
            // Save time stamp as well.
            EEWriteByte(baseC+SchedDaysOs, days);
            EEWriteByte(baseC+SchedHoursOs, hours);
            EEWriteByte(baseC+SchedMinsOs, mins);
            SPWriteChr(baseC+SchedDaysOs, days);
            SPWriteChr(baseC+SchedHoursOs, hours);
            SPWriteChr(baseC+SchedMinsOs, mins);
            LoggedInTimer = LogInTimeOutVal;    // seconds.
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  CS<chn>;<val>  Set point
static void CfgSetPoint()
{
    cnum = GetAByte();
    if ( cnum < SPCount )
    {
        unsigned char val;
        baseC = SetPBLoc + (cnum * SPEntryLen);
        val = GetAByte();
        if ( val <= 100 )
        {
            // store in percent.
            EEWriteByte(baseC,val);
            SPWriteChr(baseC,val);
            SetPoints[cnum] = ANPERCENTTOVAL(val);
            LoggedInTimer = LogInTimeOutVal;    // seconds.
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  IC<N|F>[;<address>]   Infra Red
static void EnableInfraRedRx()
{
    if ( MORE_INPUT )
    {
        action = OpStr[OpStrIdx++];
        if ( action == 'N' )
        {
            doEnableIrRx();
        }
        else
        if ( action == 'F' )
        {
            doDisableIrRx();
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
}

//  IC<N|F>[;<address>]   Infra Red
static void EnableInfraRedTx()
{
    if ( MORE_INPUT )
    {
        action = OpStr[OpStrIdx++];
        if ( action == 'N' )
        {
            doEnableIrTx();
        }
        else
        if ( action == 'F' )
        {
            doDisableIrTx();
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
}

void CfgInfraRedAddress()
{
    cnum = GetAByte();
    if ( cmdError == 0 )
    {
        if ( cnum <= maxRC5Address )
        {
            doSetIrAdr( cnum );
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
}

// send a command
// IR.
void SendInfraRed()
{
    cnum = GetAByte() & 0x1F;
    if ( cmdError == 0 )
    {
        action = GetAByte() & 0x3F;
        if ( cmdError == 0 )
        {
            sendIr( cnum, action );
        }
    }
}

static void AdjustAnThres()
{
    cnum = GetAByte();
    if ( cnum < AnInEntryCount )
    {
        unsigned char type;
        unsigned char thres;
        type = OpStr[OpStrIdx++];
        thres = GetAByte();
        if (   ( ( type == 'L' ) || ( type == 'H' ) || ( type == 'B' )  )
            && ( thres <= 100 ) )
        {
            if ( type == 'L' )
            {
                AnaAdjustThreshold( cnum, DIN_GROUP1_SET_LOW_THRESHOLD, thres );
            }
            else
            if ( type == 'B' )
            {
                AnaAdjustThreshold( cnum, DIN_GROUP1_SET_LOW_THRESHOLD, thres );
                AnaAdjustThreshold( cnum, DIN_GROUP1_SET_HIGH_THRESHOLD, thres );
            }
            else
            {
                AnaAdjustThreshold( cnum, DIN_GROUP1_SET_HIGH_THRESHOLD, thres );
            }
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//
//
//
static void CfgAnIn()
{
    cnum = GetAByte();
    if ( cnum < AnInEntryCount )
    {
        unsigned char type;
        unsigned char thres;
        type = OpStr[OpStrIdx++];
        thres = GetAByte();
        if (   ( ( type == 'L' ) || ( type == 'H' ) )
            && ( thres <= 100 ) )
        {
            unsigned tloc;
            baseC = AnInCfgBaseLoc + (cnum*AnInCfgSize);
            baseV = AnInValBase + (cnum*AnInValSize);
            if ( type == 'H' )
            {
                tloc = baseC + AnInCfgHiActionOs;
            }
            else
            {
                tloc = baseC + AnInCfgLowActionOs;
            }
            if ( NO_MORE_INPUT || StoreTrigger( tloc ) )
            {
                if ( type == 'H' )
                {
                    EEWriteByte(baseC+AnInCfgHiOs, thres);
                    SPWriteChr(baseC+AnInCfgHiOs, thres);
                    AnHighThreshold[cnum] = thres;
                    SPWriteChr( baseV + AnInValHiOs, AnHighThreshold[cnum] );
                }
                else
                {
                    EEWriteByte(baseC+AnInCfgLowOs, thres);
                    SPWriteChr(baseC+AnInCfgLowOs, thres);
                    AnLowThreshold[cnum] = thres;
                    SPWriteChr( baseV + AnInValLowOs, AnLowThreshold[cnum] );
                }
                AnInVals[cnum] = 255;   // force threshold check on next analogue read.
                LoggedInTimer = LogInTimeOutVal;    // seconds.
            }
            else
            {
                cmdError = ERR_BadParam;
            }
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

static void AdjustTempThres()
{
    cnum = GetAByte();
    if ( cnum < TempCount )
    {
        int thres;
        unsigned char type;
        type = OpStr[OpStrIdx++];
        thres = GetTempVal();
        if (   ( ( type == 'L' ) || ( type == 'H' ) || ( type == 'B' ) )
            && ( cmdError == 0 ) )
        {
            if ( type == 'L' )
            {
                TempAdjustThreshold( cnum, DIN_GROUP1_SET_LOW_THRESHOLD, thres );
            }
            else
            if ( type == 'B' )
            {
                TempAdjustThreshold( cnum, DIN_GROUP1_SET_LOW_THRESHOLD, thres );
                TempAdjustThreshold( cnum, DIN_GROUP1_SET_HIGH_THRESHOLD, thres );
            }
            else
            {
                TempAdjustThreshold( cnum, DIN_GROUP1_SET_HIGH_THRESHOLD, thres );
            }
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  CT<chn>;<low>;<nom>;<hi>;<action>;  Temp input ???
static void CfgTempIn()
{
    cnum = GetAByte();
    if ( cnum < TempCount )
    {
        int thres;
        unsigned char type;
        type = OpStr[OpStrIdx++];
        thres = GetTempVal();
        if (   ( ( type == 'L' ) || ( type == 'H' ) )
            && ( cmdError == 0 ) )
        {
            unsigned tloc;
            baseC = TempCfgBase + (cnum*TempCfgSize);
            baseV = TempValBase + (cnum*TempValSize);
            if ( type == 'H' )
            {
                tloc = baseC + TempCfgHiActionOs;
            }
            else
            {
                tloc = baseC + TempCfgLowActionOs;
            }
            if ( NO_MORE_INPUT || StoreTrigger( tloc ) )
            {
                if ( ( type == 'H' ) || ( type == 'B' ) )
                {
                    EEWriteInt(baseC+TempCfgHiOs, thres);
                    SPWriteInt(baseC+TempCfgHiOs, thres);
                    TempHighThreshold[cnum] = thres;
                    SPWriteInt( baseV + TempHiOs, TempHighThreshold[cnum] );
                }
                if ( ( type == 'L' ) || ( type == 'B' ) )
                {
                    EEWriteInt(baseC+TempCfgLowOs, thres);
                    SPWriteInt(baseC+TempCfgLowOs, thres);
                    TempLowThreshold[cnum] = thres;
                    SPWriteInt( baseV + TempLowOs, TempLowThreshold[cnum] );
                }
                LoggedInTimer = LogInTimeOutVal;    // seconds.
            }
            else
            {
                cmdError = ERR_BadParam;
            }
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  CC<nr>;[NFI][NFI][NFI][NFI][NFI][NFI][NFI][NFI];[I|S<nn>];[I|S<nn>];[I|S<nn>];[I|S<nn>]:
static void CfgScene()
{
    cnum = GetAByte();
    if ( cnum < SceneEntryCount )
    {
        unsigned char DigMask = 0;
        unsigned char DigState = 0;
        unsigned char AnMask = 0;
        unsigned AnState = 0;
        idx = 0;
        while ( OpStr[OpStrIdx] != ';' )
        {
            if ( OpStr[OpStrIdx] != 'I' )
            {
                DigMask |= (0x01 << idx );
                if ( OpStr[OpStrIdx] == 'N' )
                {
                    DigState |= (0x01 << idx );
                }
            }
            ++idx;
            ++OpStrIdx;
        }
        ++OpStrIdx;

        // Now analogue setpoints
        idx = 0;
        while ( OpStrIdx < OpStrLen )
        {
            work = OpStr[OpStrIdx];
            ++OpStrIdx;
            if ( work != 'I' )
            {
                if ( work == 'S' )
                {
                    AnMask |= (0x01 << idx );
                    AnState |= ( ( ((unsigned)GetAByte()) & 0x000F ) << (4*idx) );
                }
                else
                {
                    cmdError = ERR_BadParam;
                }
            }
            else
            {
                // skip I
            }
            if ( OpStr[OpStrIdx] == ';' )
            {
                ++OpStrIdx;
            }
            ++idx;   // next setpoint store.
        }
        // Now store the scene.
        baseC = SceneCfgBase + (cnum*SceneCfgSize);
        EEWriteByte(baseC+SceneDigMaskOs, DigMask);
        EEWriteByte(baseC+SceneDigStateOs, DigState);
        EEWriteByte(baseC+SceneAnMaskOs, AnMask);
        EEWriteInt(baseC+SceneAnStateOs, AnState);
        // update siteplayer
        SPWriteChr(baseC+SceneDigMaskOs, DigMask);
        SPWriteChr(baseC+SceneDigStateOs, DigState);
        SPWriteChr(baseC+SceneAnMaskOs, AnMask);
        SPWriteInt(baseC+SceneAnStateOs, AnState);
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

static void SetScene()
{
    if ( LoggedInState > 0 )
    {
        cnum = GetAByte();
        if ( cnum < SceneEntryCount )
        {
            DoScene( cnum, DIN_GROUP1_ON );
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
    else
    {
        cmdError = ERR_NotLoggedIn;
    }
}

//  DO<0-7>NFTD[0-3]:                   (Trigger dig out)
static void DoDigOut()
{
    cnum = GetAByte();
    if ( ( cmdError == 0 ) && ( cnum < DOutEntryCount ) )
    {
        unsigned char action;
        action = OpStr[OpStrIdx++];
        switch (action)
        {
            case 'T' :
                // toggle
                DigitalOut( cnum, DIN_GROUP1_TOGGLE );
                break ;

            case 'D' :
                {
                    GetAnUnsigned();
                    if ( cmdError == 0 )
                    {
                        if ( uval < DwellCount )
                        {
                            dwellValue = DwellTimes[uval];
                        }
                        else
                        {
                            dwellValue = uval;
                        }
                        DigitalOut( cnum, DIN_DWELL_ON );
                    }
                }
                break ;

            case 'N' :
                // On
                DigitalOut( cnum, DIN_GROUP1_ON );
                break ;

            case 'F' :
                // toggle
                DigitalOut( cnum, DIN_GROUP1_OFF );
                break ;

            default:
                break;
        }
        if ( LoggedInState == 1 )
        {
            LoggedInTimer = LogInTimeOutVal;    // seconds.
        }
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  DI<0-7>:                   (Trigger dig in)
static void DoDigIn()
{
    cnum = GetAByte();
    if ( cnum < DInNameCount )
    {
        PerformTrigger( 'd', cnum, DInBloc + (cnum*DInCfgSize) );
        if ( LoggedInState == 1 )
        {
            LoggedInTimer = LogInTimeOutVal;    // seconds.
        }
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  DM<0-7>:<level>                   (Trigger dig in)
#ifdef USE_MIMICS
static void ParseDoMimic()
{
    while ( MORE_INPUT && ( cmdError == 0 ) )
    {
        cnum = GetAByte();
        work = GetAByte();
        if (   ( cnum < MimicCount )
            && ( work <= MAX_MIMIC_LEVEL ) )
        {
            SetMimicTargetLevel( cnum, work );
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
}

//  SM<off>;<on>;<mimic fade rate>
static void SetMimicParams()
{
    unsigned char hi;
    work = GetAByte();
    hi = GetAByte();
    if (   ( hi <= MAX_MIMIC_LEVEL )
        && ( work <= MAX_MIMIC_LEVEL ) )
    {
        mimicOff = work;
        mimicOn = hi;
        EEWriteByte( MimicsLevelLowLoc, mimicOff );
        EEWriteByte( MimicsLevelHighLoc, mimicOn );
        if ( MORE_INPUT )
        {
            MimicFadeRate = SetAByte(MimicFadeRateLoc);
        }
        if ( MORE_INPUT )
        {
            // should be 0 or 1
            // need to persist
            work = EEReadByte(MimicFlagsLoc);

            if ( GetAByte() == 0 )
            {
                Mimic5V();
                work &= ~MimicVoltsBit;
            }
            else
            {
                Mimic12V();
                work |= MimicVoltsBit;
            }
            EEWriteByte(MimicFlagsLoc, work);

        }
        DigitalMimicInit();
        AnalogueMimicInit();
    }
    else
    {
        cmdError = ERR_BadParam;
    }
}

//  CM<A|D>;<chn>;<mimic>*[;<A|D>;<chn>;<mimic>]:
// TODO change data storeage to 8 bytes
// bits 0-3 are source channel nr
// bits 4-7 are source type (None, digital, analogue, scene ???).
static void ConfigureMimic()
{
    while ( MORE_INPUT && ( cmdError == 0 ) )
    {
        chnType = OpStr[OpStrIdx++];    // either A or D
        cnum = GetAByte();              // 0-7 or 0-3
        work = GetAByte() & 0x0F;       // 0-7 or 15
        idx = cnum >> 1;
        if ( chnType == 'A' )
        {
            if ( cnum < AnOutEntryCount )
            {
                if ( cnum & 0x01 )
                {
                    mimicForAnOut[idx] = (mimicForAnOut[idx] & 0x0F) | (work << 4);
                }
                else
                {
                    mimicForAnOut[idx] = (mimicForAnOut[idx] & 0xF0) | work;
                }
                baseC = MimicsForAnOutLoc + (unsigned int)idx;
                // now tell the world
                EEWriteByte(baseC, mimicForAnOut[idx]);
                SPWriteChr(baseC, mimicForAnOut[idx]);
            }
            else
            {
                cmdError = ERR_BadParam;
            }
        }
        else
        if ( chnType == 'D' )
        {
            if ( cnum < DOutPrimaryCount )
            {
                if ( cnum & 0x01 )
                {
                    mimicForDig[idx] = (mimicForDig[idx] & 0x0F) | (work << 4);
                }
                else
                {
                    mimicForDig[idx] = (mimicForDig[idx] & 0xF0) | work;
                }
                baseC = MimicsForDigOutLoc + (unsigned int)idx;
                // now tell the world
                EEWriteByte(baseC, mimicForDig[idx]);
                SPWriteChr(baseC, mimicForDig[idx]);
            }
            else
            {
                cmdError = ERR_BadParam;
            }
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
    if ( cmdError == 0 )
    {
        DigitalMimicInit();
        AnalogueMimicInit();
    }
}
#endif

//  AA<chn>;[S<nn>|<nn>]  Set Analogue output setpoint or absolute
static void DoAnOut()
{
    if ( LoggedInState > 0 )
    {
        cnum = GetAByte();
        if ( cnum < AnOutEntryCount )
        {
            if ( OpStr[OpStrIdx] == 'S' )
            {
                unsigned spnum;
                ++OpStrIdx;
                spnum = GetAByte();
                if ( spnum < SPCount )
                {
                    AnaCommand( cnum, DIN_GROUP1_ON, spnum );
                    if ( LoggedInState == 1 )
                    {
                        LoggedInTimer = LogInTimeOutVal;    // seconds.
                    }
                }
                else
                {
                    cmdError = ERR_BadParam;
                }
            }
            else
            {
                // absolute
                GetAnUnsigned();
                if ( uval <= 100 )
                {
                    AnaOut( cnum, ANPERCENTTOVAL(uval) );
                    if ( LoggedInState == 1 )
                    {
                        LoggedInTimer = LogInTimeOutVal;    // seconds.
                    }
                }
                else
                {
                    cmdError = ERR_BadParam;
                }
            }
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
    else
    {
        cmdError = ERR_NotLoggedIn;
    }
}

static int Match( unsigned char *pass, unsigned loc )
{
    unsigned char ch;
    unsigned len;
    for ( len = 0; len < PasswordLen; ++len )
    {
        ch = EEReadByte(loc);
        if ( *pass != ch )
        {
            break;
        }
        if ( ch == 0 )
        {
            break; // password Ok.
        }
        ++loc;
        ++pass;
    }
    return ( *pass == ch );
}

static int MatchS( unsigned char *pass, unsigned char *cmp )
{
    unsigned char ch;
    unsigned len;
    while ( *cmp != 0 )
    {
        if ( *pass != *cmp )
        {
            return false;
        }
        ++pass;
        ++cmp;
    }
    return ( *pass == *cmp ); // check end of both
}

// RS
static void serialSend()
{
    while ( MORE_INPUT && ( cmdError == 0 ) )
    {
        cnum = GetAByte();  // byte count
        // verify enough
        SerialSendByte(cnum);
    }
}
static void SetSoftWatchdog()
{
    uval = DF_SITEPLAYER_TIMEOUT;
    if ( MORE_INPUT )
    {
        GetAnUnsigned();
    }
    // enable siteplayer watchdogging
    // This requires 
    if ( cmdError == 0 )
    {
        enableSiteplayerWatchdog(uval);
    }
}

static void doDmxLighting()
{
    if ( MORE_INPUT )
    {
        idx = GetAByte();
        if ( MORE_INPUT )
        {
            work = GetAByte();
            DmxSetLevel( idx, work );
        }
    }
}

// CR[2|4|<protocol>];[baudIndex|baudRate];[8|9];[N|O|E];[1|2]
// mode of 0 means change speed only.
static void serialConfigure()
{
    if ( MORE_INPUT )
    {
        work = GetAByte();
        if ( MORE_INPUT )
        {
            idx = GetAByte();
        }
        else
        {
            idx = BAUD_9600;
        }
        if (   ( cmdError == 0 )
            && ( work <= 15 ) 
            && ( idx <= BAUD_MAX ) )
        {
            if ( work == 0 )
            {
                // same mode, so can just change speed
                work = SerialMode(EEReadByte(SerialModeLoc));
            }
            if ( work == SM_Dmx )
            {
                idx = BAUD_250000;
            }
        
            work = MakeSerialConfig(work,idx);
            EEWriteByte( SerialModeLoc, work );
            SPWriteChr( SerialModeLoc, work );
            SerialInit();   // re-init
        }
    }
}

static void FRCommand()
{
    if ( LoggedInState > 1 )
    {
        // verify logged in state
        cnum = GetAByte();
        if ( cnum > 0 )
        {
            FactoryReset();
        }
        else
        {
            ResetConfig();
        }
        // Reset the CPU.
        #asmline RESET
    }
    else
    {
        cmdError = ERR_NotLoggedIn;
    }
}

static void resetTempBus()
{
    if ( MORE_INPUT )
    {
        unsigned char action;
        action = OpStr[OpStrIdx++];
        if ( action == 'D')
        {
            OWDeTagAll();
        }
        else
        {
            cmdError = ERR_BadParam;
        }
    }
    if (cmdError == 0 )
    {
        TempFindSensors();
    }
}

static void DoLogin()
{
    unsigned char level;
    unsigned char *pass;
    LogInTimeOutVal = DF_LoggedInTime;
    level = 0;
    pass = OpStr+OpStrIdx;
    if ( Match( pass, Password3Loc ) )
    {
        LogInTimeOutVal = DF_LongLoggedInTime;
        level = 3;
    }
    else if ( Match( pass, Password2Loc ) )
    {
        level = 2;
    }
    else if ( Match( pass, Password1Loc ) )
    {
        level = 1;
    }
    else if ( MatchS( pass, o2m8Password ) )
    {
        level = 4;
    }
    else
    {
        cmdError = ERR_BadParam;
        if ( EEReadByte(Password1Loc) == 0 )
        {
            level = 1;
        }
    }
    LoggedInState = level;
    if ( level > 0 )
    {
        LoggedInTimer = LogInTimeOutVal;    // seconds.
    }
    SPWriteChr( LoginStateLoc, LoggedInState );
}

static void setOperState(unsigned char newState)
{
    // Not in persistant memory
    // Need to read first so that we can tell Siteplayer before we lock out interface to SP.
//            if ( OperState != NewState )
    {
        if ( OPER_STATE_IS_FROZEN )
        {
            // Do local update first so we can send result to siteplayer.
            // as the SPComms functions lockout on state frozen
            OperState = newState;
        }
        SPWriteChr( OperStateLoc, newState );
        OperState = newState;
    }
}

// Global functions
static void Parse()
{
    cmdError = 0;

    OpStrIdx = 2;   // Skip command pair.
    if (OpStr[2] == ';' )
    {
        OpStrIdx = 3;   // Skip semicolon after command pair
    }

    if ( LoggedInState > 3 )
    {
        // private login.
        if ( OpStr[0] == 'Z' )
        {
            // private commands
            if ( OpStr[1] == 'O' )
            {
                SetSecOption();
            }
            else
            {
                cmdError = ERR_BadCommand;
            }
        }
        else
        {
            cmdError = ERR_BadCommand;
        }
    }
    else
    if ( OpStr[0] == 'F' )
    {
        if ( OpStr[1] == 'R' )
        {
            FRCommand();
        }
        else
        {
            cmdError = ERR_BadCommand;
        }
    }
    else
    if ( OpStr[0] == 'T' )
    {
        // thresholds
        if ( OpStr[1] == 'A' )
        {
            AdjustAnThres();
        }
        else
        if ( OpStr[1] == 'T' )
        {
            AdjustTempThres();
        }
        else
        {
            cmdError = ERR_BadCommand;
        }
    }
    else
    if ( OpStr[0] == 'N' )
    {
        if ( LoggedInState > 1 )
        {
            if ( OpStr[1] == 'D' )
            {
                NameDigIn();
            }
            else
            if ( OpStr[1] == 'O' )
            {
                NameDigOut();
            }
            else
            if ( OpStr[1] == 'A' )
            {
                NameAnOut();
            }
            else
            if ( OpStr[1] == 'I' )
            {
                NameAnIn();
            }
            else
            if ( OpStr[1] == 'T' )
            {
                NameTemperature();
            }
            else
            if ( OpStr[1] == 'N' )
            {
                baseC = NNameLoc;
                SetAName( NNameLen );
            }
            else
            {
                cmdError = ERR_BadCommand;
            }
        }
        else
        {
            cmdError = ERR_NotLoggedIn;
        }
    }
    else
    if ( OpStr[0] == 'C' )
    {
        if ( LoggedInState > 1 )
        {
            if ( OpStr[1] == 'E' )
            {
                CfgSchedEvent();
            }
            else
            if ( OpStr[1] == 'W' )
            {
                CfgDwell();
            }
            else
            if ( OpStr[1] == 'D' )
            {
                CfgDigIn();
            }
            else
            if ( OpStr[1] == 'I' )
            {
                CfgAnIn();
            }
            else
            if ( OpStr[1] == 'S' )
            {
                CfgSetPoint();
            }
            else
            if ( OpStr[1] == 'T' )
            {
                CfgTempIn();
            }
            else
            if ( OpStr[1] == 'C' )
            {
                CfgScene();
            }
            else
            if ( OpStr[1] == 'R' )
            {
                // configure serial interface
                serialConfigure();
            }
#ifdef USE_MIMICS
            else
            if ( OpStr[1] == 'M' )
            {
                ConfigureMimic();
            }
#endif
            else
            {
                cmdError = ERR_BadCommand;
            }
        }
        else
        {
            cmdError = ERR_NotLoggedIn;
        }
    }
    else
    if ( OpStr[0] == 'D' )
    {
        if ( OpStr[1] == 'A' )
        {
            doAlert = DF_SOFT_ALERT_TIME;
        }
        else
        if ( OpStr[1] == 'X' )
        {
            doDmxLighting();
        }
        else
        if ( LoggedInState > 0 )
        {
            if ( OpStr[1] == 'O' )
            {
                DoDigOut();
            }
            else
            if ( OpStr[1] == 'I' )
            {
                DoDigIn();
            }
            else
            if ( OpStr[1] == 'T' )
            {
                DoTrigger();
            }
#ifdef USE_MIMICS
            else
            if ( OpStr[1] == 'M' )
            {
                ParseDoMimic();
            }
#endif
            else
            {
                cmdError = ERR_BadCommand;
            }
        }
        else
        {
            cmdError = ERR_NotLoggedIn;
        }
    }
    else
    if ( OpStr[0] == 'A' )
    {
        if ( OpStr[1] == 'A' )
        {
            DoAnOut();
        }
        else
        if ( OpStr[1] == 'V' )
        {
            cnum = GetAByte();
            GetAnUnsigned();
            if ( ( cnum < AnOutEntryCount ) && ( uval < 1024 ) )
            {
                AnaOut( cnum, uval );
            }
        }
        else
        {
            cmdError = ERR_BadCommand;
        }
    }
    else
    if ( OpStr[0] == 'L' )
    {
        if ( OpStr[1] == 'G' )
        {
            DoLogin();
        }
        else
        {
            cmdError = ERR_BadCommand;
        }
    }
    else
    if ( OpStr[0] == 'S' )
    {
        if ( OpStr[1] == 'F' )
        {
            FadeRate = SetAByte( FadeRateLoc );
        }
        else
        if ( OpStr[1] == 'T' )
        {
            SetTime();
        }
        else
        if ( OpStr[1] == 'D' )
        {
            PSetDate();
        }
        else
        if ( OpStr[1] == 'I' )
        {
            SetIpAddress();
        }
        else
        if ( OpStr[1] == 'N' )
        {
            MyNodeNr = SetAByte( NNumLoc ); // reload into working memory.
        }
        else
        if ( OpStr[1] == 'R' )
        {
            SetRotStep();
        }
        else
        if ( OpStr[1] == 'C' )
        {
            SetScene();
        }
        else
        if ( OpStr[1] == 'S' )
        {
            // SS - Set operational state
            setOperState(GetAByte());
        }
        else
        if ( OpStr[1] == 'P' )
        {
            SetPassword();
        }
        else
        if ( OpStr[1] == 'O' )
        {
            SetOption();
        }
        else
        if ( OpStr[1] == 'A' )
        {
            SetAddressVerifyMac();
        }
#ifdef USE_MIMICS
        else
        if ( OpStr[1] == 'M' )
        {
            SetMimicParams();
        }
#endif
        else
        if ( OpStr[1] == 'W' )
        {
            SetSoftWatchdog();
        }
        else
        {
            cmdError = ERR_BadCommand;
        }
    }
    else
    if ( OpStr[0] == 'X' )
    {
        if ( OpStr[1] == 'r' )
        {
            RBehaviour = RAND_6RANDOM;  // Start random behaviour
        }
        else
        if ( OpStr[1] == 'n' )
        {
            RBehaviour = RAND_ALL_ON;  // Signal for ccts ON
        }
        else
        if ( OpStr[1] == 'f' )
        {
            RBehaviour = RAND_ALL_OFF;  // Signal for ccts OFF
        }
        else
        if ( OpStr[1] == 'c' )
        {
            RBehaviour = RAND_DO_NOTHING;
        }
        else
        if ( OpStr[1] == 'U' )
        {
            // ????
            SendUDP() ;
        }
        else
        {
            cmdError = ERR_BadCommand;
        }
    }
    else
    if ( OpStr[0] == 'R' )
    {
        if ( OpStr[1] == 'D' )
        {
            // send single character
            serialSend();
        }
        else
        if ( OpStr[1] == 'O' )
        {
            // RS485 transmitter enbled
            setRs485TxOn();
        }
        else
        if ( OpStr[1] == 'I' )
        {
            // RS485 transmitter disabled
            setRs485TxOff();
        }
        else
        if ( LoggedInState > 1 )
        {
            if ( OpStr[1] == 'T' )
            {
                resetTempBus();
            }
            else
            if ( OpStr[1] == 'U' )
            {
                SPStartup();
                RefreshDynamicData();
            }
            else
            if ( OpStr[1] == 'B' )
            {
                // reboot or reset the SitePlayer
                bitset(PORTC,0); // on 7 series cut power to SP, on 6.4 series operate SP reset line
                Wait(2000);      // we need at least a second to be sure of a reset  
                bitclear(PORTC,0); 
                
                // now reboot the PIC
                #asmline RESET 
            }
            else
            if ( OpStr[1] == 'S' )
            {
                if ( OPER_STATE_IS_FROZEN )
                {
                    setOperState( OPER_STATE_NORMAL );  // enable comms with SP
                }

                SitePlayerReset();
            }
            else
            {
                cmdError = ERR_BadCommand;
            }
        }
        else
        {
            cmdError = ERR_NotLoggedIn;
        }
    }
    else
    if ( OpStr[0] == 'I' )  // infra red commands
    {
        if ( OpStr[1] == 'R' )
        {
            if ( LoggedInState > 1 )
            {
                EnableInfraRedRx();
            }
            else
            {
                cmdError = ERR_NotLoggedIn;
            }
        }
        else
        if ( OpStr[1] == 'T' )
        {
            // do we need to enable disable IR send
            if ( LoggedInState > 1 )
            {
                EnableInfraRedTx();
            }
            else
            {
                cmdError = ERR_NotLoggedIn;
            }
        }
        else
        if ( OpStr[1] == 'A' )
        {
            if ( LoggedInState > 1 )
            {
                CfgInfraRedAddress();
            }
            else
            {
                cmdError = ERR_NotLoggedIn;
            }
        }
        else
        if ( OpStr[1] == 'S' )
        {
            if ( LoggedInState >= 1 )
            {
                SendInfraRed();
            }
            else
            {
                cmdError = ERR_NotLoggedIn;
            }
        }
        else
        {
            cmdError = ERR_BadCommand;
        }
    }
    else
    {
        cmdError = ERR_BadCommand;
    }
    SPWriteChr( CmdErrorLoc, cmdError );
}

static void ParseBinary()
{
}

// Looks for characters being available from the Siteplayer over serial connection
// reads and buffers into OpStr until it sees a colon at which point it parses the command
// and actions it if valid.
// Technically this function should be in spComms
//
void CheckForCmds()
{
    unsigned char c;
    while ( GetRxSize() > 0 )
    {
        c = WaitRx();
        if ( OpStr_Flushing )
        {
            if ( c == ':' )
            {
                OpStr_Flushing = false;
            }
        }
        else
        if ( c >= 32 )
        {
            OpStr[OpStrLen] = c;
            if ( c == ':' )
            {
                OpStr[OpStrLen] = 0;    // make terminated string
                if ( OpStr[OpStrLen-1] == ';' )
                {
                    --OpStrLen;
                    OpStr[OpStrLen] = 0;    // Loose trailing colon.
                }
                if ( OpStrLen >= 2 )    // minimal command length
                {
                    Parse();
                }
                OpStrLen = 0;   // reset for next command
                memset( OpStr, 0, MAX_CMD_STR );
            }
            else
            {
                // continuing text input.
                ++OpStrLen;
                if ( OpStrLen >= MAX_CMD_STR )
                {
                    // Error bin it, should also bin until next colon
                    OpStrLen = 0;   // reset for next command
                    OpStr_Flushing = true;
                    memset( OpStr, 0, MAX_CMD_STR );
//                    --OpStrLen; // so cheat and overwrite last char
                }
            }
        }
        else
        {
            // ignore control character.
/*
            if ( OpStrLen == OpStr[0] )
            {
                // binary input complete, binned for now.
                ParseBinary();
                OpStrLen = 0;   // reset for next command
            }
            else
            {
                // continuing binary input.
                ++OpStrLen;
            }
*/
        }
    }
}

void FlushCmds()
{
    OpStrLen = 0;
    while ( GetRxSize() > 0 )
    {
        OpStr[OpStrLen] = WaitRx();
    }
}
