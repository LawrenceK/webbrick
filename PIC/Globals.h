//
//  Copyright L.P.Klyne 2013
//
//  Filename: Template.h
//
//  Description:    System wide Global data.
//
//  Notes:
//
//
// Include files

#include "SpLocations.h"

// Defines

// Static data (Global scope)

// Operational State
// FROZEN - no changes made to outputs at all, nothing sent to SP, SP monitored for change
// This is mainly an engineering state
#define OPER_STATE_FROZEN   0
// STARTUP inputs ignored
#define OPER_STATE_HOLIDAY  1
// All the rest do monitor inputs.
#define OPER_STATE_NORMAL   2
//
//#define OPER_STATE_QUIESCENT   3
//
// Thoughts
// bit 7
// bit 6
// bit 0-3  // Time Of Day.
extern unsigned char OperState;			// Start up in frozen state

#define OPER_STATE_IS_FROZEN  ( OperState == OPER_STATE_FROZEN )
#define OPER_STATE_IS_NOT_FROZEN  ( OperState != OPER_STATE_FROZEN )
#define OPER_STATE_DO_INPUTS  ( OperState > OPER_STATE_HOLIDAY )

//
//
// Bit 1 do not allow IP address change.
#define OPT_SecurityFlags       0
// Options below this are private options.
#define MinOptions              1
#define OPT_UDPOptions          1
#define OPT_UDPSendTempChange   1
#define OPT_UDPSendAnInChange   2
#define OPT_UDPSendAnOutChange  4
#define OPT_UDPSendInfraRed     8
#define OPT_UDPSendRtc          16
#define OPT_UDPSendDOutChange   32

// Suppress AnIn by default.
#define DF_UDPOption (OPT_UDPSendTempChange+OPT_UDPSendAnOutChange+OPT_UDPSendInfraRed+OPT_UDPSendDOutChange)
//#define DF_UDPOption (OPT_UDPSendTempChange+OPT_UDPSendAnInChange+OPT_UDPSendAnOutChange+OPT_UDPSendInfraRed+OPT_UDPSendDOutChange)

// Options that affect analogue inputs
#define OPT_AnInOptions     2
#define OPT_AnInReCheckThr       1
#define DF_AnInOption (0)
//#define OPT_AIReadContinuous          1

// These reserve a byte each for the other channel types.
#define OPT_AnOutOptions     3
#define OPT_AnOutDoDmx       2
#define DF_AnOutOption (OPT_AnOutDoDmx)

#define OPT_DigInOptions     4
#define DF_DigInOption (0)

#define OPT_DigOutOptions    5
#define DF_DigOutOption (0)

#define OPT_TempOptions      6
#define OPT_TempReCheckThr       1
#define DF_TempOption (OPT_TempReCheckThr)

#define OPT_SceneOptions     7
// Clear this option and scenes do not target analgue outputs but DMX channels direct
// The DMX channels can be access indirectly through the analague outputs by setting the option for AnOut to include OPT_AnOutDoDmx
#define OPT_SceneDoAnalogue  1
//#define OPT_SceneDoDmx       2
#define DF_SceneOption (OPT_SceneDoAnalogue)

// defined in SPLocations
//#define MaxOptions  3

extern unsigned char Options[MaxOptions];

#define OptionEnabled(option)   ((Options[option]) > 0 )
#define OptionValue(option)     Options[option]
#define UDP_SEND(option)        ((Options[OPT_UDPOptions] & option) != 0 )
#define ANIN_OPTION(option)        ((Options[OPT_AnInOptions] & option) != 0 )
#define ANOUT_OPTION(option)        ((Options[OPT_AnOutOptions] & option) != 0 )
#define SCENE_OPTION(option)        ((Options[OPT_SceneOptions] & option) != 0 )
#define TEMP_OPTION(option)        ((Options[OPT_TempOptions] & option) != 0 )

#define RAND_DO_NOTHING 0
#define RAND_ALL_OFF    100
#define RAND_ALL_ON     101
#define RAND_6RANDOM    102
#define RAND_3RANDOM    103
extern unsigned char RBehaviour;					// char that controls random behaviour

// Current settings for One wire temperature settings
//#define TEMP_ACTION_CHANNEL 0x07
//#define TEMP_ACTION_TODO 0x08
//#define TEMP_ACTION_DIR 0x10
//#define TEMP_ACTION_DIRABOVE 0x10
//#define TEMP_ACTION_DIRBELOW 0
//#define TEMP_ACTION_LOWER 0x20
//#define TEMP_ACTION_UPPER 0x40
//#define TEMP_ACTION_FAN 0x80
extern unsigned char RotStep;					// Rotary Encoder step per indent
extern unsigned SetPoints[SPCount];

extern unsigned char MyNodeNr;					// My NodeNumber

extern unsigned char FadeRate;					// used to conrol fade rates
extern unsigned char MimicFadeRate;					// used to conrol fade rates

extern unsigned DwellTimes[DwellCount];			// Array of Dwell times

extern unsigned LoggedInTimer;
extern unsigned char LoggedInState;

extern unsigned char myMac[6];

// temperature thresholds
extern int TempLowThreshold[TempCount];
extern int TempHighThreshold[TempCount];

// analogue thresholds
extern unsigned char AnLowThreshold[AnOutEntryCount];
extern unsigned char AnHighThreshold[AnOutEntryCount];

// used in decoding/performing triggers
extern unsigned char chnType;
extern unsigned char tgtChn;
extern unsigned char action;
//extern unsigned char dwellNr;
extern unsigned dwellValue;    // A value in seconds between 1 and 32767
extern unsigned char setPointNr;      // a setpoint or a value between 0 and 100.
extern unsigned char udpDo;
extern char assocValue1;
extern unsigned char assocValue2;
extern unsigned char b1Work;
extern unsigned char b2Work;
