//
//  Copyright L.P.Klyne 2013
//
//  Filename: spComms.c
//
//  Description:    Contains all the code handling the interface to the siteplayer
//
//  Notes:
//
//
// Include files
#include "Webrick_Auto.h"

// Local Defines

// Local forwards

void sendSPResetReason();

// Static data (File scope)
static unsigned char udpSeq = 0;
struct tag_UDP_Packet_Bin   UDPPkt_b;
static unsigned spWatchdog = 0;
static unsigned spWatchdog_tmr = 0;
static unsigned spRefresh = 0;  // when above 0 the siteplayer needs refresh in n seconds
//struct tag_UDP_Packet   UDPPkt;

// Global fucntions
#ifdef _SPWriteInt 
void SPWriteInt ( unsigned location, int theint )
{
    if ( OperState > 0 )
    {
        SP_COMMS;
        //
        //  This routine will write an integer into SP memory using the
        //  Write method, therefore not useful for extended addresses  
        //
        if ( location > 255 )
        {
            AddTx( 0x91 );  	    // Write two bytes
            AddTx( location & 0xFF );   // low address
            AddTx( location >> 8 );     // high address
        }
        else
        {
            AddTx( 0x81 );  	// Write two bytes
            AddTx( location );	// location to write to
        }
        AddTx (theint & 0xFF) ; // lower byte
        AddTx (theint >> 8) ;	// upper byte
    }
}
#endif

#ifdef _SPWriteChr
void SPWriteChr(unsigned location, unsigned char thechar)
{
//
//  This routine will write a character into SP memory using the
//  Write method, therefore not useful for extended addresses  
//
    if ( OperState > 0 )
    {
        SP_COMMS;
        if ( location > 255 )
        {
            AddTx (0x90);  	// Write one byte
            AddTx( location & 0xFF );   // low address
            AddTx( location >> 8 );     // high address
        }
        else
        {
            AddTx (0x80);  	// Write one byte
            AddTx (location) ;	// location to write to
        }
        AddTx (thechar);	// the character to write
    }
}
#endif


#ifdef _SPWriteStr
void SPWriteStr( unsigned location, unsigned char *theStr, unsigned maxLen )
{
    //
    //   Write String to SP
    //
    if (   ( OperState > 0 )
        && ( maxLen <= 16 ) )   // we do not support strings over 16 bytes long.
    {
        unsigned char i;
        unsigned char len;
        SP_COMMS;

        len = 0;
        // need to work out length first.
        while ( ( len < maxLen ) // limit to user provided length, excludes null
             && ( theStr[len] != 0 ) )
        {
            ++len;
        }
        if ( len < maxLen )   // then we need to also send the null byte.
        {
            ++len; // will now include the null byte in the buffer
        }

        //
        //  Now set up Write to SP
        //
        i = len - 1 + 0x80;    // Convert to sp command Write, Note length nibble for SP is 0-15 for 1-16 bytes
        if ( location > 255 )
        {
            AddTx( i + 0x10 );  	    // Convert to WriteX
            AddTx( location & 0xFF );   // low address
            AddTx( location >> 8 );     // high address
        }
        else
        {
            AddTx( i );  	// Write two bytes
            AddTx( location );	// location to write to
        }

        for ( i = 0 ; i < len ; ++i )
        {
            if ( theStr[i] >= 0x80 )
            {
                AddTx('.');
            }
            else
            {
                AddTx(theStr[i]);
            }
        }
    }
}
#endif

#ifdef _SPWriteBuffer
void SPWriteBuffer( unsigned location, unsigned char *buf, unsigned len )	// Write Buffer to SP
{
    if ( OperState > 0 )
    {
        unsigned char i;
        SP_COMMS;
        if ( location > 255 )
        {
            AddTx( 0x90 + len - 1 );  	    // Convert to WriteX
            AddTx( location & 0xFF );   // low address
            AddTx( location >> 8 );     // high address
        }
        else
        {
            AddTx( 0x80 + len - 1 );  	    // Convert to WriteX
            AddTx( location );	// location to write to
        }

        for ( i = 0 ; i < len ; ++i )
        {
            AddTx(buf[i]) ;
        }
    }
}
#endif


// Read a sequence of bytes
// beware of inbound messages, best done at quiet time.
#ifdef _SPReadBuffer

// in milliSeconds, total for whole buffer
#define MAX_SP_WAIT    1000
#define SP_WAIT_MILLIS    2
void SPReadBuffer( unsigned location, unsigned char *buf, unsigned len )
{
    unsigned char i;
    unsigned w;
    SP_COMMS;

    // flush buffer
    while ( GetRxSize() > 0 )
    {
        WaitRx();
    }

    if ( location > 255 )
    {
        AddTx( 0xD0 + len - 1 );  	    // ReadX
        AddTx( location & 0xFF );   // low address
        AddTx( location >> 8 );     // high address
    }
    else
    {
        AddTx( 0xC0 + len - 1 );  	    // Read
        AddTx( location );	// location to write to
    }

    w = (MAX_SP_WAIT/SP_WAIT_MILLIS);
    for ( i = 0 ; i < len ; ++i )
    {
        buf[i] = 0;
        while ( ( GetRxSize() == 0 ) && ( w > 0 ) )
        {
            // this is so we timeout lack of siteplayer.
            --w;
            Wait( SP_WAIT_MILLIS );
            // simulator help
            #pragma asmline CLRWDT
        }
        if ( GetRxSize() > 0 )
        {
            // read
            buf[i] = WaitRx();
        }
    }
}
#endif


void SPClearComms()
{
    unsigned idx;
    for ( idx = 0; idx < 20; ++idx )
    {
        AddTx( 0 ); // ensure ready for commands.
    }
}

void SitePlayerReset()
{
    spRefresh = 7;   // we start at 7, at 5 the bit is cleared
    bitset(PORTC,0); // on 7 series cut power to SP, on 6.4 series operate SP reset line
}

#ifdef _SendUDP
void SendUDP()
{
    //
    //  Construct the string and send the packet
    //
    if ( OperState > 0 )
    {
        if ( UDPPkt.udpType >= 'A' )
        {
            // old format
            UDPPkt.seqNr = udpSeq++;    // lets give a chance of spotting duplicates
            UDPPkt.len = sizeof(tag_UDP_Packet);    // old fixed length was 13
            UDPPkt.fromNodeNr = MyNodeNr;
        }
        else
        {
            // new format
            UDPPkt_b.seqNr = udpSeq++;    // lets give a chance of spotting duplicates
            UDPPkt_b.fromNodeNr = MyNodeNr;
        }
        unsigned char i;
        SP_COMMS;
// LPK need to handle packet over 16 bytes and break up write command
// Also the address is in low mem so do not need WriteX

        // assert UDPLoc == 0

        if (UDPPkt.len > 16)
        {
            // allow for up to 32 bytes of UDP packet.
            AddTx( 0x8F );
            AddTx( 0 );   // address
            for ( i = 0 ; i < 16 ; ++i )
            {
                AddTx(((unsigned char*)&UDPPkt)[i]);
            }
            // remainder
            AddTx( 0x80 + UDPPkt.len - 17 );    
            AddTx( 16 );   // address
            for ( i = 16 ; i < UDPPkt.len ; ++i )
            {
                AddTx(((unsigned char*)&UDPPkt)[i]);
            }
        }
        else
        {
            // buffer in low object space - at 0 so use Write and not WriteX
            AddTx( 0x80 + UDPPkt.len - 1 );
            AddTx( 0 );   // low address
            for ( i = 0 ; i < UDPPkt.len ; ++i )
            {
                AddTx(((unsigned char*)&UDPPkt)[i]);
            }
        }

// update udpcount ??
        // Then the sent
        AddTx('P') ;  		// actually send the UDP Packet
    }
}
#endif

void enableSiteplayerWatchdog( unsigned tmr )    // called to arm/rearm software watchdog of siteplayer
{
    spWatchdog = tmr;
    spWatchdog_tmr = spWatchdog;    // start it going, Stays armed until user issues 0 timeout.
}

void periodicSiteplayer()    // called to check software watchdog of siteplayer
{
    if ( spRefresh > 0 )
    {
        if ( spRefresh == 5 )
        {
            bitclear(PORTC,0);  // high for at least 1 second at most 2 seconds, returns power on 7, release SP reset on 6.4
        }
        --spRefresh;
        if ( spRefresh == 0 )
        {
            SPStartup();
            RefreshDynamicData();
            //
            //  Now log that we have restart
            //
	    sendSPResetReason();
        }
    }
    if ( spWatchdog_tmr > 0 )
    {
        --spWatchdog_tmr;
        if ( spWatchdog_tmr == 0 )
        {
            // Disable, wait for gateway to rearm
            spWatchdog_tmr = spWatchdog;
            SitePlayerReset();
        }
    }
}

// Local functions

