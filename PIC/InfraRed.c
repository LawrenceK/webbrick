//#define DO_DEBUG_TRACE  1
//
//  Copyright L.P.Klyne. 2006
//
//  Filename: InfraRed.c
//
//  Description:
//
//  Notes:
//
//
// Include files
#include "Webrick_Auto.h"

// Local Defines

static unsigned char rc5Address = 29;   // tlc-direct 8 button remote.

// Static data (File scope)
// So we ignore repeats
static unsigned int lastRC5 = 0;
static unsigned lastTog = 0;
static unsigned char ir = 0;

void configureIr()
{
    if ( ir == 0xFF )
    {
        ir = rc5Address; // factory reset value
        EEWriteByte(InfraRedLoc,ir);
    }
    rc5Address = InfraRedAdr(ir);
    if ( InfraRedTx(ir) )
    {
        reserveOutput( DF_IR_TX_OUTPUT );
    }
    else
    {
        unReserveOutput( DF_IR_TX_OUTPUT );
    }
    if ( InfraRedRx(ir) )
    {
        reserveInput( DF_IR_RECEIVER_INPUT );
    }
    else
    {
        unReserveInput( DF_IR_RECEIVER_INPUT );
    }
    DEBUGN( ir );
}

void initIr() 
{
    ir = EEReadByte(InfraRedLoc);
    configureIr();
}

static void updateIr()
{
    EEWriteByte(InfraRedLoc,ir);
    SPWriteChr(InfraRedLoc,ir);
    configureIr();
}

void doSetIrAdr( unsigned char adr )
{
    ir = EEReadByte(InfraRedLoc);
    adr &= 0x1F;
    ir = SetInfraRedAdr(ir,adr);
    updateIr();
}

void doEnableIrRx()
{
    ir = EEReadByte(InfraRedLoc);
    ir = SetInfraRedRx(ir);
    updateIr();
}

void doDisableIrRx()
{
    ir = EEReadByte(InfraRedLoc);
    ir = ClearInfraRedRx(ir);
    updateIr();
}

void doEnableIrTx()
{
    ir = EEReadByte(InfraRedLoc);
    ir = SetInfraRedTx(ir);
    updateIr();
}

void doDisableIrTx()
{
    ir = EEReadByte(InfraRedLoc);
    ir = ClearInfraRedTx(ir);
    updateIr();
}

// Fed occurence.
void haveRC5()
{
    DEBUGN( 0x80 );
    if ( (RC5Value != 0xFFFF) && ( lastRC5 != RC5Value ) )
    {
    	lastRC5 = RC5Value;
        unsigned char adr = ( ( lastRC5 >> 6 ) & 0x1F );
        unsigned char chn = ( lastRC5 & 0x3F );
        if (   ( adr == rc5Address ) 
            && ( chn < DInEntryCount ) )
        {
            // we take notice of this.
            // Digital trigger, from infra red
            PerformTrigger( 'R', chn, DInBloc + (chn*DInCfgSize) );
        }
        // send UDP packet always?
        if UDP_SEND(OPT_UDPSendInfraRed)
        {
            // Send UDP packet with new value.
            UDPPkt.udpType = 'G';
            UDPPkt.source[0] = 'I';
            UDPPkt.source[1] = 'R';
            UDPPkt.tgtChannel = chn;   // RC5 channel
            UDPPkt.toNodeNr = adr;   // RC5 address
//            UDPPkt.curValH = lastRC5 >> 8;
//            UDPPkt.curValL = lastRC5;

            SendUDP();
        }
    }
}

// We have seen a falling edge on RC5 receiver, but may not be in use.
void checkRC5()
{
    DEBUGN( 0x81 );
    if ( InfraRedRx(ir) )
    {
        DEBUGN( 0x82 );
        // in use for infra red.
        RC5Rx();    // attempt receive.
    }
}

void sendIr( unsigned char adr, unsigned char chn )
{
    DEBUGN( 0x91 );
    if ( InfraRedTx(ir) )
    {
        DEBUGN( 0x92 );
        TransmitRC5( ( ((unsigned int)adr) << 6) | chn | lastTog );
        lastTog ^= 0x800;   // toggle it.
    }
}
