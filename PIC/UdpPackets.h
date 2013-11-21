//
//  Copyright L.P.Klyne 2013
//
//  Filename: UdpPackets.h
//
//  Description: Documents UDP packlet formats
//
//  Notes:
//
//
// Include files

// #include "template.h"

// Defines
// 
#define MAX_EVENT_PAYLOAD   12

struct tag_Trigger
{
    unsigned char srcChannel;   // identifies source channel includes setpoint Nr.
    unsigned char tgtChannel;   // identifies source channel includes setpoint Nr.
    unsigned char action;       // to be performed includes Dwell value
    unsigned char fromNodeNr;   // always self
    unsigned char toNodeNr;     // non zero if R command
    unsigned char setPointNr;   // only
    unsigned char curValH;      // Can be temperature or analog value
    unsigned char curValL;      // Low part of value
};

struct tag_Attention
{
    unsigned char macAddr[6];
};

struct tag_Alive
{
    unsigned long upTime[4];
};

struct tag_Time
{
    unsigned char hours;    // srcChannel
    unsigned char mins;     // tgtChannel
    unsigned char resetCause;   // action
    unsigned char fromNodeNr;   // always self
    unsigned char secs;     // toNodeNr
    unsigned char day;      // setPointNr
    unsigned upTimeMins;    // curValH, curValL
};

//
// Hoisted on own petard
// Abuse this for a couple of uses, sending MAC address and debug. 
// Handled time better.
//
// IF SENDING NEW PACKETS TRY TO AVOID fromNodeNr so always valid.
struct tag_UDP_Packet
{
    unsigned char len;          // placeholder so I can try some webrick to webrick transfers
    unsigned char udpType;      // G, A, R. Can be a small binary number
    unsigned char source[2];    // Ta, low Trigger from analogue
                                // TA, high Trigger from analogue
                                // Td, Trigger from remote DI command
                                // TD, Trigger from local digital input
                                // Tt  low Trigger from temperature
                                // TT  high Trigger from temperature
                                // TS  Trigger from scheduled event
                                // AI, new input value
                                // AO, new output value
                                // DO, new digital output state
                                // NN, unconfigured node.
                                // SS, node starting.
                                // MM, monitor state change, will become obsolete
    unsigned char srcChannel;   // identifies source channel includes setpoint Nr.
    unsigned char tgtChannel;   // identifies source channel includes setpoint Nr.
    unsigned char action;       // to be performed includes Dwell value
    unsigned char fromNodeNr;   // always self
    unsigned char toNodeNr;     // non zero if R command
    unsigned char setPointNr;   // only
    unsigned char curValH;      // Can be temperature or analog value
    unsigned char curValL;      // Low part of value
    unsigned char seqNr;        // webbrick sequence number, can be used to detect duplicates.
//    unsigned char pad[MAX_EVENT_PAYLOAD-13];      // binary pad to 32 bytes
};

struct tag_UDP_Packet_Bin
{
    unsigned char len;          // placeholder so I can try some webrick to webrick transfers
    unsigned char udpType;      // small binary number
    unsigned char fromNodeNr;   // always self
    unsigned char seqNr;        // webbrick sequence number, can be used to detect duplicates.
    unsigned char data[MAX_EVENT_PAYLOAD];     // binary payload
};

#define UDP_Packet_General  'G'
#define UDP_Packet_Alarm    'A'
#define UDP_Packet_Remote   'R'

// Received serial data
#define UDP_Packet_SerialRx  1

// DebugTrace, first data byte is a type code.
#define UDP_Packet_DebugTrace  2
#define DebugTrace_RTCInit  1
#define DebugTrace_RTCSetTime  2
#define DebugTrace_RTCSetDate  3
#define DebugTrace_RTCRead  4

#define DebugTrace_1WireRead  5
#define DebugTrace_1WireAdr  6
#define DebugTrace_1WireStart  7
#define DebugTrace_1WireTag  8
#define DebugTrace_1WireSearch  9
#define DebugTrace_1WireDeTag  10

#define DebugTrace_AnInGetAnIn  11

#define UDP_Packet_NEXTBINARY  3

