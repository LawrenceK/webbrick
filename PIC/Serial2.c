//
//  Copyright L.P.Klyne 2013
//
//  Filename: Serial2.c
//
//  Description:    Contains all the code handling the interface to the 2nd serial port.
//
//  Notes:  At present this control is very basic. The driver code is a duplicated copy of the FED
//          serial element called SerInt2. This is configured at compile time.
//
//
// Include files
#include "Webrick_Auto.h"

// Local Defines
// approx 8 millisconds
#define MAX_WAIT_TIME 10
#define MAX_READ_BUFFER MAX_EVENT_PAYLOAD

// Local Declarations
static unsigned char waitTime = 0;
static unsigned char byteCount = 0;
static unsigned char byteBuf[MAX_READ_BUFFER];
static unsigned char smode = 0;

// all using high baud rate generator (16 bit)
// padded to 16 entries so bad baud index defaults.
const unsigned baudRates[] = { (APROCFREQ/(300*4) ) - 1,
            (APROCFREQ/(600*4) ) - 1,
            (APROCFREQ/(1200*4) ) - 1,
            (APROCFREQ/(2400*4) ) - 1,
            (APROCFREQ/(4800*4) ) - 1,
            (APROCFREQ/(9600*4) ) - 1,
            (APROCFREQ/(19200*4) ) - 1,
            (APROCFREQ/(38400*4) ) - 1,
            (APROCFREQ/(57600*4) ) - 1,
            (APROCFREQ/(115200*4) ) - 1,
            (APROCFREQ/(250000*4) ) - 1,
            (APROCFREQ/(9600*4) ) - 1,
            (APROCFREQ/(9600*4) ) - 1,
            (APROCFREQ/(9600*4) ) - 1,
            (APROCFREQ/(9600*4) ) - 1,
            (APROCFREQ/(9600*4) ) - 1
};

// Local forwards

// Local functions
static void sendData()
{
    UDPPkt_b.len = 4 + byteCount;
    UDPPkt_b.udpType = UDP_Packet_SerialRx;
    memcpy( UDPPkt_b.data, byteBuf, byteCount );
    SendUDP();
    byteCount = 0;
}

// Global functions
void CheckSerial()
{
    // Store any bytes available ready to create UDP packet
    // Longer term hold in buffer until can fill UDP directly.
    if ( ( smode == SM_Rs232Data ) || ( smode == SM_Rs485Data ) )
    {
        if ( GetRxSize2() > 0 )
        {
            byteBuf[byteCount] = WaitRx2();
            ++byteCount;
            waitTime = MAX_WAIT_TIME;
            if ( byteCount == MAX_READ_BUFFER )
            {
                sendData();
            }
        }
    }
    else
    if ( smode == SM_Dmx )
    {
        DmxCheck();
    }
}

void SerialReceivetimeout()
{
    // do nothing for now
    if ( waitTime > 0 )
    {
        --waitTime;
        if ( (byteCount > 0) && (waitTime == 0) )
        {
            sendData();
        }
    }
}

void SerialSendByte(unsigned char b)
{
    if ( ( smode == SM_Rs232Data ) || ( smode == SM_Rs485Data ) )
    {
        AddTx2(b);
    }
}

// Global functions
void SerialInit()
{
    smode = EEReadByte(SerialModeLoc);
    DmxStop();
    Serial2Disable();
    setRs485TxOff();    // always for now

    SetBaud2(baudRates[SerialSpeed(smode)]);
    smode = SerialMode(smode);

    if ( smode == SM_Rs232Data )
    {
        setRs232();
        Serial2Enable();
    }
    else
    if ( smode == SM_Rs485Data )
    {
        setRs485();
        Serial2Enable();
    }
    else
    if ( smode == SM_Dmx )
    {
        // DMX does some bit twiddling to get the long break etc.
        setRs485();
//        SetBaud2(baudRates[BAUD_250000]);   // force speed
        setRs485TxOn();    // always on
        DmxStart();
    }
    else
    {
        // unrecognised
        setRs232();
        Serial2Enable();
    }
}
