//
//  Copyright L.P.Klyne 2013
//
//  Filename: spComms.h
//
//  Description:    Siteplayer interface declarations.
//
//  Notes:
//
//
// Include files
#include "UdpPackets.h"

// Defines

// Static data (Global scope)
extern struct tag_UDP_Packet_Bin   UDPPkt_b;
#define UDPPkt    ((tag_UDP_Packet)UDPPkt_b)
//extern struct tag_UDP_Packet   UDPPkt;
//#define UDPPkt_b    ((tag_UDP_Packet_Bin)UDPPkt)

// ensure Siteplayer ready.
void SPClearComms();
void SitePlayerReset();

// Exported functions declarations
// Write a 2 byte integer to the site Player
// location can be anywhere within sitePlayer memory.
void SPWriteInt ( unsigned location, int theint ) ;				// Write Int to SP
// Write a single character to the siteplayer
void SPWriteChr ( unsigned location, unsigned char thechar ) ;		        // Write Char to SP
// Write a null terminated string to the site player
// location can be anywhere in siteplayer memory.
// maxLen limits the number of characters sent
// length also limited to 15 characters excluding a null.
void SPWriteStr ( unsigned location, unsigned char *theStr, unsigned maxLen ) ;	// Write String to SP

// Write a sequence of bytes
void SPWriteBuffer( unsigned location, unsigned char *buf, unsigned len ) ;	// Write Buffer to SP

// Read a sequence of bytes
// beware of inbound messages, best done at quiet time.
void SPReadBuffer( unsigned location, unsigned char *buf, unsigned len ) ;	// Read Buffer from SP

//void SendTrigger( unsigned char type, unsigned char chn, );

void SendUDP() ;							        // Send a UDP Packet
//void UDPEnable() ;  						                // sets UPD receive enabled

void enableSiteplayerWatchdog( unsigned tmr );    // called to arm/rearm software watchdog of siteplayer
void periodicSiteplayer();    // called to check software watchdog of siteplayer

#define UDPEnable() SPWriteChr( SPUdpCmdPkt, 1 );
