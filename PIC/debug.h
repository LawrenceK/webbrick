// enabling this does a variety of things
// mainly rescans temp bus every second.
// disbales reconfiguring text strings and hard sets the webbrick name.
//#define DEBUG_TEMP_SENSOR_TEST    1
//
//#define DO_DEBUG_TRACE   1
//
//  Copyright L.P.Klyne 2013
//
//  Filename: Template.h
//
//  Description:
//
//  Notes:
//
//
// Include files

//#include "template.h"

// Defines

// now housekeeping
#define DEBUG_SOMETHING  1

// Some debug defines that can be nulled easily and directed to different locations easily.

#ifdef DEBUG_SOMETHING
void Debug1(unsigned char x);
void Debug2(unsigned char x,unsigned char x2);
void Debug3(unsigned char x,unsigned char x2,unsigned char x3);
void Debug4(unsigned char x,unsigned char x2,unsigned char x3,unsigned char x4);
void Debug5(unsigned char x,unsigned char x2,unsigned char x3,unsigned char x4,unsigned char x5);
void DebugB( unsigned char id, unsigned char *data, unsigned char len );
#endif

#ifdef DO_DEBUG_TRACE
#define DEBUGB(id,bf,ln)  {DebugB(id,bf,ln);}
#define DEBUG1(x)  {DebugB(x, 0, 0);}
#define DEBUG2(x,x2)  {Debug2(x,x2);}
#define DEBUG3(x,x2,x3)  {Debug3(x,x2,x3);}
#define DEBUG4(x,x2,x3,x4)  {Debug4(x,x2,x3,x4);}
#define DEBUG5(x,x2,x3,x4,x5)  {Debug5(x,x2,x3,x4,x5);}

#define DEBUGN(x)  {DebugB(x, 0, 0);}
#define DEBUGI(x)  {DebugB(x, 0, 0);}

#else
#define DEBUGB(id,bf,ln)
#define DEBUG1(x)
#define DEBUG2(x,x2)
#define DEBUG3(x,x2,x3)
#define DEBUG4(x,x2,x3,x4)
#define DEBUG5(x,x2,x3,x4,x5)
#define DEBUGN(x)
#define DEBUGI(x)

#endif

// Exported functions declarations
//
// 0x0n UserLoop()
// 0x1n UserLoop()
// 0x2n Siteplayer
// 0x3n Analogue
// 0x4n digital
// 0x5n clock
// 0x6n 
// 0x7n 
// 0x8n Parsing & IR
// 0x9n Parsing
// 0xAn 
// 0xBn 
// 0xCn 
// 0xDn 
// 0xEn 
// 0xFn 

