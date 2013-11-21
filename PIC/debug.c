// Include files
#include "Webrick_Auto.h"

#ifdef DEBUG_SOMETHING
unsigned char dbBuff[4];

void DebugB( unsigned char id, unsigned char *data, unsigned char len )
{
    UDPPkt_b.len = 4 + 1 + len;
    UDPPkt_b.udpType = UDP_Packet_DebugTrace;
    UDPPkt_b.data[0] = id;
    memcpy( (void*)(UDPPkt_b.data+1), (void*)data, len );
    SendUDP();
}

void Debug2(unsigned char x,unsigned char x2)
{
    dbBuff[0] = x2;
    DebugB( x, dbBuff, 1 );
}

void Debug3(unsigned char x,unsigned char x2,unsigned char x3)
{
    dbBuff[0] = x2;
    dbBuff[1] = x3;
    DebugB( x, dbBuff, 2 );
}

void Debug4(unsigned char x,unsigned char x2,unsigned char x3,unsigned char x4)
{
    dbBuff[0] = x2;
    dbBuff[1] = x3;
    dbBuff[2] = x4;
    DebugB( x, dbBuff, 3 );
}

void Debug5(unsigned char x,unsigned char x2,unsigned char x3,unsigned char x4,unsigned char x5)
{
    dbBuff[0] = x2;
    dbBuff[1] = x3;
    dbBuff[2] = x4;
    dbBuff[3] = x5;
    DebugB( x, dbBuff, 4 );
}

#endif
