// $Id:$
//#define UDP_TRACE   1
//
//  Copyright L.P.Klyne 2013
//
//  Filename: Template.c
//
//  Description:
//
//  Notes:
//
//
// Include files
#include "Webrick_Auto.h"

// Local Defines

// Local forwards

// Static data (File scope)
static unsigned char idx;
int clockSet = false;

#ifdef USE_RTC
int rtcSet = false;
//static unsigned int rtc_timer = 5;
static unsigned char rtc_buf[16];    // data from rtc chip
#endif

#ifdef UDP_TRACE
static void RtcTrace( unsigned char id, unsigned char len )
{
    UDPPkt_b.len = 4 + 1 + len;
    UDPPkt_b.udpType = UDP_Packet_DebugTrace;
    UDPPkt_b.data[0] = id;
    memcpy( (void*)(UDPPkt_b.data+1), rtc_buf, len );
    SendUDP();
}
#else
#define RtcTrace( id, len )
#endif

// Global functions

// Called at system start
void SendClockTime()
{
    SPWriteChr( TimeHourLoc, Hours );
    SPWriteChr( TimeMinuteLoc, Mins );
    SPWriteChr( TimeSecondLoc, Secs );
    SPWriteChr( TimeDayLoc, Day );
}

#ifdef USE_RTC
void RTC_Init()
{
    rtc_buf[0] = 0x80; // !AE, Minute Alarm
    rtc_buf[1] = 0x80; // !AE, Hour Alarm
    rtc_buf[2] = 0x80; // !AE, Day Alarm
    rtc_buf[3] = 0x80; // !AE, Day of week alarm
    rtc_buf[4] = 0x80; // CLKOUT enabled, 32K
    rtc_buf[5] = 0; // Timer disabled
    rtc_buf[6] = 0; // Timer
    i2c_write_ba( 0xA2, rtc_buf, 9, 7 );

    RtcTrace( DebugTrace_RTCInit, 7 );
}
#endif

//#define BcdToBinary( v ) (((v>>4)*10) + (v&0x0F))
unsigned char BcdToBinary( unsigned char v) 
{
    return (((v>>4)*10) + (v&0x0F));
}

//unsigned char BinaryToBcd( v ) ( ((v/10) << 4 ) | (v -( (v/10)*10)) )
unsigned char BinaryToBcd( unsigned char v )
{
    unsigned char t = v/10; 
    return (t << 4 ) | (v -(t*10));
}

#ifdef USE_RTC
void RTC_SetTime()
{
    // We need to convert binary to BCD to set the clock.
    rtc_buf[0] = BinaryToBcd(Secs);
    rtc_buf[1] = BinaryToBcd(Mins);
    rtc_buf[2] = BinaryToBcd(Hours);
    rtc_buf[3] = 0; // date
    rtc_buf[4] = Day;
    i2c_write_ba( 0xA2, rtc_buf, 2, 5 );
    RtcTrace( DebugTrace_RTCSetTime, 5 );
}
#endif

void SetDate( unsigned char Yr, unsigned char Mn, unsigned char Dt )
{
#ifdef USE_RTC
    rtc_buf[0] = Dt;
    rtc_buf[1] = Day;
    rtc_buf[2] = Mn;
    rtc_buf[3] = Yr;
    i2c_write_ba( 0xA2, rtc_buf, 5, 4 );
    RtcTrace( DebugTrace_RTCSetDate, 4 );
#endif
}

void checkScheduledEvents( unsigned char day, unsigned char hr, unsigned char min )
{
    unsigned base = SchedBaseLoc;
    unsigned char dayMask = 1 << day;

    // Go through the scheduled events
    for ( idx = 0; idx < SchedEntryCount; ++idx )
    {
        if (   ( ( EEReadByte(base + SchedDaysOs) & dayMask ) != 0 )
            && ( EEReadByte(base + SchedHoursOs) == hr )
            && ( EEReadByte(base + SchedMinsOs) == min ) )
        {
            PerformTrigger( 'S', idx, base+SchedActionOs );
        }
        base += SchedEntryLen;
    }
}

#define MINS_PER_HOUR 60
#define MINS_PER_DAY 1440
#define MINS_PER_WEEK 10080
// minutes
#define NOADJUST_THRESHOLD  10

// Called by parser, updates Clock element
void SetClockElement( unsigned char Dy, unsigned char Hr, unsigned char Mn, unsigned char Sc )
{
    // TODO add a check and do not process if only setting to a short time in the past
    // As all steps will of recently been performed.
    bool doEvents;
    int mNow = (Day*MINS_PER_DAY)+(Hours*MINS_PER_HOUR)+Mins;
    int mNxt = (Dy*MINS_PER_DAY)+(Hr*MINS_PER_HOUR)+Mn;

    doEvents = true;
    // The purpose of this is to work out whether we are adjusting the clokc by a small amount backwards
    // In which case we do not rerun all the scheduled events as if going forward nearly a whole week.
    // It is assumed that i) anything that is on is already on and ii) a small aadjustment is from the gateway
    // adjusting the clock.
    //
    // we have 
    // clock forward without cross end of week
    //      mNow < mNxt
    // clock forward across end of week
    //      mNow > mNxt
    // clock back without cross end of week
    //      mNow > mNxt
    // clock back across end of week
    //      mNow < mNxt
    if (mNow >= mNxt )  // catches same time as well.
    {
        // clock forward across end of week
        // clock back without cross end of week
        if ( (mNow-mNxt) < NOADJUST_THRESHOLD )  // back small amount
        {
            doEvents = false;
        }
    }
    else
    {
        // clock forward without cross end of week
        // clock back across end of week
        if ( (mNow+MINS_PER_WEEK-mNxt) < NOADJUST_THRESHOLD )    // across end of week
        {
            doEvents = false;
        }
    }

    if (doEvents)
    {
        // large clock set, need to verify the scheduled events.
        // process Scheduled events
        // go through every minute for 7 days.
        // basically wrap a 7 day clock from now until we reach now again.
        do {
            ++Mins;
            if ( Mins >= 60 ) 
            {
                Mins = 0;
                ++Hours;
                if ( Hours >= 24 ) 
                {
                    Hours = 0;
                    ++Day;
                    if ( Day >= 7 ) Day = 0;
                }
            }
            checkScheduledEvents( Day, Hours, Mins );
        } while ((Day != Dy) || (Hours != Hr) || (Mins != Mn));
    }
    else
    {
        // small backward adjustment.
        Hours = Hr;
        Mins = Mn;
        Day = Dy;
    }

    Secs = Sc;
    SendClockTime();    // tell the world
    clockSet = true;
}

void SetClock( unsigned char Dy, unsigned char Hr, unsigned char Mn, unsigned char Sc )
{
    SetClockElement( Dy, Hr, Mn, Sc );
#ifdef USE_RTC
    RTC_SetTime();
#endif    
}

void ckSecOcc()
{
    DEBUGN( 0x50 );
    SPWriteChr( TimeSecondLoc, Secs );
#ifdef USE_RTC
    if ( Secs == 5 )    // do sooner after startup
    {
        // done mid minute so RTC changes do not affect scheduled events at minute markers.
//        tryRtcRead();
        i2c_read_ba( 0xA2, rtc_buf, 0, 9 );
    }
#endif    
}

//void tryRtcRead()
//{
//    i2c_read_ba( 0xA2, rtc_buf, 0, 16 );
//    i2c_read_ba( 0xA2, rtc_buf, 0, 9 );
//}

void ckMinuteOcc()
{
    DEBUGN( 0x51 );
    SPWriteChr( TimeMinuteLoc, Mins );
    checkScheduledEvents( Day, Hours, Mins );
}

void ckHourOcc()
{
    DEBUGN( 0x52 );
    SPWriteChr( TimeHourLoc, Hours );
}

void ckDayOcc()
{
    DEBUGN( 0x53 );
    SPWriteChr( TimeDayLoc, Day );
}

#ifdef USE_RTC
void i2c_ReadComplete()
{
    DEBUGN( 0x54 );
//rtc_buf
    rtcSet = ( ( rtc_buf[2] & 0x80 ) == 0 ); // is the Rtc valid
    if ( ( rtc_buf[2] & 0x80 ) != 0 )
    {
        // initialise RTC alarms etc. Not the clock counters these are handled by standard clock and date setting.
        RTC_Init();
    }
    else
    {
        // rtc_buf[0], rtc_buf[1] is status bytes.
        // Convert BCD to decimal.
        rtc_buf[2] = BcdToBinary(rtc_buf[2]&0x7F); // seconds
        rtc_buf[3] = BcdToBinary(rtc_buf[3]&0x7F); // minutes
        rtc_buf[4] = BcdToBinary(rtc_buf[4]&0x3F); // hours
        rtc_buf[5] = BcdToBinary(rtc_buf[5]&0x3F); // date
        rtc_buf[6] &= 0x07; // day of week,
        rtc_buf[7] = BcdToBinary(rtc_buf[7]&0x1F);; // month
        rtc_buf[8] = BcdToBinary(rtc_buf[2]);; // years

        RtcTrace( DebugTrace_RTCRead, 9 );
        
        // We need to do something to detect a clock that is not running.
        
//        if (   !clockSet
//            || (   ( rtc_buf[3] == Mins ) 
//                && ( rtc_buf[4] == Hours ) ) )
        {
            SetClockElement( rtc_buf[6], rtc_buf[4], rtc_buf[3], rtc_buf[2] );
        }
    }
    memset( rtc_buf, 0, 16 );
//    rtc_timer = DF_RTCInterval;
}

void i2c_WriteComplete()
{
    DEBUGN( 0x55 );
//    rtc_timer = DF_RTCInterval;
}

void i2c_ReadFailed()
{
    DEBUGN( 0x56 );
//    rtc_timer = DF_RTCInterval;
}

void i2c_WriteFailed()
{
    DEBUGN( 0x57 );
//    rtc_timer = DF_RTCInterval;
}
#endif
