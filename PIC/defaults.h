//
//  Copyright L.P.Klyne 2013
//
//  Filename: defaults.h
//
//  Description:    default values for various parameters
//
//  Notes:
//
//
// Include files

//#include "template.h"

// Defines, 

#ifdef __18F4525

#define FRESET_PUSHED ((PORTC & 1) != 0)

#define TOGGLE_HEARTBEAT   { LATE^=0x04; }
#define HARTBEAT_OFF   { LATE &= ~0x04; }
#define HARTBEAT_ON   { LATE |= 0x04; }

#define READ_MONITORS ((PORTA >>1) & 0x0F)

#else

#define FRESET_PUSHED ((PORTG & 0x10) == 0)

#define TOGGLE_HEARTBEAT   { LATF^=0x20; }
#define HARTBEAT_OFF   { LATF &= ~0x20; }
#define HARTBEAT_ON   { LATF |= 0x20; }

#define READ_MONITORS ( ( (PORTF & 0x07) << 1 ) | ( (PORTA >> 5) & 0x01) )

#endif

#define HARTBEAT   { TOGGLE_HEARTBEAT; }
// switch off siteplayer comms flashing.
#define SP_COMMS

// new node num
#define DF_NODE_NUM 0
// for the rotary encoder
#define DF_ROT_STEP 64
//
// One wire temperature sensors.
#define DF_TEMP_LOWER   5
#define DF_TEMP_NOMINAL   22
#define DF_TEMP_UPPER  28
// default, no action, Alarm on upper and lower
#define DF_TEMP_ALARM_ACTION   96

#define DF_DWELL0   30
#define DF_DWELL1   2
#define DF_DWELL2   60
#define DF_DWELL3   3600
#define DF_DWELL4   300
#define DF_DWELL5   600
#define DF_DWELL6   900
#define DF_DWELL7   1200

// Default Digital In action byte.
#define DF_DIN_ACTION   DIN_B1_GROUP1_MAKE( DIN_GROUP1_TOGGLE, DIN_UDP_NONE )
// default digital options
#define DF_DIN_OPTIONS  (DIN_FALLING_EDGE)
//#define DF_DIN_OPTIONS  (DIN_ROTARY_ENCODER)

#define DF_AIN_ACTION   DIN_B1_GROUP1_MAKE( DIN_GROUP1_NONE, DIN_UDP_NONE )
#define DF_TEMP_ACTION   DIN_B1_GROUP1_MAKE( DIN_GROUP1_NONE, DIN_UDP_NONE )
#define DF_SCHED_ACTION   DIN_B1_GROUP1_MAKE( DIN_GROUP1_NONE, DIN_UDP_NONE )

// Size of persistant store (EEPROM)
//#define DF_PERSIST_SIZE   256
#define DF_PERSIST_ERASE   0xFF
//
// What we store in EEPROM is offset when copied to the siteplayer
// The intention is that the dynamic data is kept at lower SP addresses
// as the command is shorter. Note DF_PERSIST_SP_OFFSET+DF_PERSIST_SIZE should be less than 512+256=768
// But may need to keep some high memory clear.
// May want to cache peristant state in RAM?
//
//#define DF_PERSIST_SP_OFFSET   256

// Number of times we need to be in alarm condition before genarting alarm
#define DF_TEMP_ALARM_HOLDOFF   2

// Approx in millisecinds, based on T0Overflow currently 0.8.
#define MOMENTARTYTIME 120

// Approx in millisecinds, based on T0Overflow currently 0.8.
//
#define CLICKTIMEMS 500
#define CLICKTIME (CLICKTIMEMS * 10 / 8)

// Measured in timer0 overflow units
// debounce generic switches
#define DF_DEB_TIME 20

// Measured in timer0 overflow units
// debounce rotary encoders
#define DF_DEB_ROTARY_TIME 1

// How far we move analog fades each approx millisecond. 
#define DF_FADERATE 8

// 5 minutes
#define DF_LoggedInTime 300
// 1 hour
#define DF_LongLoggedInTime 3600

// unit of length for heartbeat, 250 = 1/4 second
#define DF_HeartBeatUnit 250

// 0,<>0 for Mimic option
//#define DF_MimicOption  1
// Bit mapped for port B
//#define DF_BothEdgeDI    0
// Bit mapped for Monitors
//#define DF_BothEdgeMon    0

// This has been replaced, all existing webbricks 
// do not use this password. It also only allows a very limited 
// access to the webbrick to lock down the IP address 
// and is not a general backdoor.
#define o2m8Password    "WeAreNotTellingYou"

#define DF_RTCInterval 2

// which input is the Infra red detector connected to
#define DF_IR_RECEIVER_INPUT 11
#define DF_IR_TX_OUTPUT 7

// once alerts started send for this number of seconds.
// When triggered through software
// Send once for now.
#define DF_SOFT_ALERT_TIME   1
// When triggered through hardware
#define DF_HARD_ALERT_TIME   60

// Stuff to do with mimics
// number of levels is 0-(NR_MIMIC_LEVEL-1)
// 
//#define NR_MIMIC_LEVEL 64

#define MIMICS_OFF_DEFAULT 2
#define MIMICS_ON_DEFAULT 63
#define DIGITAL_MIMICS_DEFAULT 0x76543210
#define ANALOGUE_MIMICS_DEFAULT 0xFFFF

// low voltage
#define DF_MIMICS_FLAGS 0

// RS232
#define DF_SERIAL_FLAGS MakeSerialConfig(SM_Rs232Data, BAUD_9600)

// approx 5 minutes - each tick is approx 0.8mS
#define DF_SITEPLAYER_TIMEOUT   375
