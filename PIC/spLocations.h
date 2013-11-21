//
// Define the locations in siteplayer memory to store values
// from the PIC
//
// LPK Added upper area.
//
// We want to move any others we find into here as well
//
#ifndef _SPLOCATIONS_H
#define _SPLOCATIONS_H	1

#include ".\SitePlayer\WB55Lib.h"

//
// Locations within site player and EEPROM data within PIC chip
//
// We use lowe addresses for dyanmic stuff as shorter address when
// sending to siteplayer.
//
#define UDPLoc udpstring
#define LoginStateLoc loginstate
#define OperStateLoc sptod
#define DInLoc digin
#define CmdErrorLoc cmderror
//#define DIn2Loc digin2
#define DOutStsLoc spdigoutstatus
#define MimicStsLoc (spdigoutstatus+1)
#define OneWStsLoc onewsts

// Clock values, not all yet implemented.
#define TimeYearLoc spyear
#define TimeMonthLoc spmonth
#define TimeDateLoc spdate
#define TimeDayLoc spday
#define TimeHourLoc sphour
#define TimeMinuteLoc spminute
#define TimeSecondLoc spsecond

// Base of current temperatures
#define TempValBase sptemp0
#define TempCount 5
#define TempValSize (sptemp1-sptemp0)
#define TempValOs 0
#define TempLowOs (sptemp0low-sptemp0)
#define TempHiOs (sptemp0hi-sptemp0)

// 1 bytes for value
#define AnOutBLoc anout0value
#define AnOutEntryCount 4

// 1 bytes for value
#define AnInValBase anin0value
#define AnInEntryCount 4
#define AnInValSize (anin1value-anin0value)
#define AnInValOs 0
#define AnInValLowOs (spanin0low-anin0value)
#define AnInValHiOs (spanin0hi-anin0value)

#define PersistStart nodename

#define NNameLoc nodename
#define NNameLen (nodenumber-nodename)
#define NNumLoc nodenumber

// If bit 7 is set then invalid data so reset to factory values
#define InfraRedLoc infrared
#define InfraRedAdr(b) (b & 0x1F)
#define InfraRedTx(b) ( (b&0x20) != 0 )
#define InfraRedRx(b) ( (b&0x40) != 0 )
#define SetInfraRedTx(b) (b|0x20)
#define SetInfraRedRx(b) (b|0x40)
#define ClearInfraRedTx(b) (b&0xDF)
#define ClearInfraRedRx(b) (b&0xBF)
#define SetInfraRedAdr(b,adr) ( (b & 0xE0) | adr )

#define RotStepLoc rot0step

#define FadeRateLoc faderate

#define SerialModeLoc serialflags
#define SM_Rs232Data     0x02
#define SM_Rs485Data     0x04
#define SM_Dmx           0x03
#define MakeSerialConfig(m,b)    ( ( m<<4) | b )
#define SerialMode(b)    (b>>4)
#define SerialSpeed(b)   (b&0x0F)

// 
//#define isSerialModeRs485(b) ( (b & SerialModeBit) != 0 )
//#define isSerialModeRs485(b) ( (b & SerialModeBit) != 0 )
//#define isSerialModeRs232(b) ( (b & SerialModeBit) == 0 )

#define MimicFlagsLoc mimicflags
#define MimicVoltsBit 0x80
#define isMimicLowVolts(b) ( (b & MimicVoltsBit) == 0 )
#define isMimicHighVolts(b) ( (b & MimicVoltsBit) != 0 )

// control byte 1
// control byte 2
// control byte 3
// control byte 4
// control byte 5
//
//  Digital Input Command Structure
//
//  FIRST byte
//	Bits 0-3 - Action
//	Bits 4-5 - Dwell Choice
//	Bits 6-7 - UDP or Remote
// 
// Webbrick 6.5 encoding
// bit 7, set to send UDP
// bits 5-6 main group
// command group
//  0(0x20) - Group 1
//  1 (0x40) - 
//  2 (0x60) - 
//  3 (0x00) - Dwell Group
//
// Dwell group
// bits 0-2 Is a dwell number
// bits 3-4 dwell type
//  0 (0x00) - Dwell On, ignored if channel already on
//  1 (0x08) - Dwell Off, always switches off after dwell, NOP if already off
//  2 (0x10) - Dwell Cancel, on for dwell if off, otherwise off now
//  3 (0x18) - Dwell Always, channel on and off after dwell. override existing on
//
// Group 1
// bits 0-4 encode existing simple commands
//
//  SECOND Byte
//	Bit 7  -  0 Digital/Scene/Temp  1 Analogue
//
//      Digital/Scene
//	Bits 6 - 0 Digital, 1 Scene
//	Bits 0-4 - Chn to operate
//
//      Analogue
//	Bits 4-6 - Channel
//	Bits 0-3 - Set Point
//
//  THIRD Byte
//    Associated value 1
//
//  Fourth Byte
//    Associated value, currently using remote Node only, i.e. associated value is a remote bode number when UDP type is R
//
//  Fifth Byte
//    Additional options.
//
// NOTE the same formating is used to encode triggers from thershold values
// and scheduled events.
//
// Note changes to this will result in changes to
//  parse.c
//  lib.js
//  possibly performTrigger in digital.c
//
#define DInBloc dig0cfgb1
#define DInCfgSize (dig1cfgb1-dig0cfgb1)

#define DInEntryCount 12

#define DInBlocEnd ( dig0cfgb1 + ( DInCfgSize * DInEntryCount ) )
#define DInActionOs 0

#define DIN_UDP_NONE  0x00
#define DIN_UDP_SENDUDP  0x80

#define DIN_B1_DWELL_MAKE( act, dwell, udp ) ((unsigned char)( act | dwell | udp ) )
#define DIN_B1_GROUP1_MAKE( act, udp ) ((unsigned char)( act | udp ) )

#define DIN_COMMAND_MASK  0x7F
// update if adding more commands
#define DIN_MAX_COMMAND  DIN_GROUP1_SEND_IR

#define DIN_GROUP_MASK  0x60
#define DIN_GROUP_1  0x00
#define DIN_GROUP_2  0x20
#define DIN_GROUP_3  0x40
#define DIN_GROUP_DWELL  0x60
#define DIN_B1_COMMAND_GROUP(b1)  (b1&DIN_GROUP_MASK)

//#define DIN_B1_ACTION_MASK  0x0F
//#define DIN_B1_ACTION(b1)  ( b1 & DIN_B1_ACTION_MASK )
#define DIN_COMMAND(b1)  (b1 & DIN_COMMAND_MASK)

#define DIN_SUBGROUP_MASK  0x1F

// special handling Dwell combines dwell command and dwell number
#define DIN_DWELL_MASK  (DIN_GROUP_DWELL | 0x18)
#define DIN_B1_DWELL_COMMAND(b1)  (b1&DIN_DWELL_MASK)
#define DIN_B1_DWELL_NR(b1)  (b1 & 0x07)
#define DIN_DWELL_ON  (DIN_GROUP_DWELL | 0x00)
#define DIN_DWELL_OFF  (DIN_GROUP_DWELL | 0x08)
#define DIN_DWELL_CANCEL  (DIN_GROUP_DWELL | 0x10)
#define DIN_DWELL_ALWAYS  (DIN_GROUP_DWELL | 0x18)

#define DIN_B1_GROUP1_COMMAND(b1)  (b1 & DIN_SUBGROUP_MASK)
#define DIN_GROUP1_NONE  (DIN_GROUP_1 | 0x00)
#define DIN_GROUP1_OFF  (DIN_GROUP_1 | 0x01)
#define DIN_GROUP1_ON  (DIN_GROUP_1 | 0x02)
#define DIN_GROUP1_MOMENTARY  (DIN_GROUP_1 | 0x03)
#define DIN_GROUP1_TOGGLE  (DIN_GROUP_1 | 0x04)
#define DIN_GROUP1_NEXT  (DIN_GROUP_1 | 0x07)
#define DIN_GROUP1_PREV  (DIN_GROUP_1 | 0x08)
#define DIN_GROUP1_SET_LOW_THRESHOLD  (DIN_GROUP_1 | 0x09)
#define DIN_GROUP1_SET_HIGH_THRESHOLD  (DIN_GROUP_1 | 0x0A)
#define DIN_GROUP1_ADJ_LOW_THRESHOLD  (DIN_GROUP_1 | 0x0B)
#define DIN_GROUP1_ADJ_HIGH_THRESHOLD  (DIN_GROUP_1 | 0x0C)
// should this be its own group?
#define DIN_GROUP1_SEND_IR  (DIN_GROUP_1 | 0x0D)
#define DIN_GROUP1_UP  (DIN_GROUP_1 | 0x0E)
#define DIN_GROUP1_DOWN  (DIN_GROUP_1 | 0x0F)
#define DIN_GROUP1_SET_DMX  (DIN_GROUP_1 | 0x10)
#define DIN_GROUP1_COUNT  (DIN_GROUP_1 | 0x11)

// These are the command codes that are delivered from the UI and externally
// The aim was tp not change any existing codes when the new ones where added and
// the encoding in Byte1 changed. This is alaso why the dwells are in command group 4 (>=0x60)
// also why the Group1 codes currently have a gap in them.
#define CC_NONE  0
#define CC_OFF  1
#define CC_ON  2
#define CC_MOMENTARY  3
#define CC_TOGGLE  4
#define CC_DWELL_ALWAYS 5
#define CC_DWELL_CANCEL 6
#define CC_NEXT  7
#define CC_PREV  8
#define CC_SET_LOW_THRESHOLD  9
#define CC_SET_HIGH_THRESHOLD  10
#define CC_ADJ_LOW_THRESHOLD  11
#define CC_ADJ_HIGH_THRESHOLD  12
#define CC_SEND_IR  13
#define CC_UP  14
#define CC_DOWN  15
#define CC_SET_DMX  16
#define CC_COUNT  17
#define CC_DWELL_ON 18
#define CC_DWELL_OFF 19

#define DIN_B1_SEND_UDP(b1)  ((b1&DIN_UDP_SENDUDP) != 0)

#define DInChanOs (dig0cfgb2-dig0cfgb1)
#define DIN_B2_MAKE( chn, type ) ((unsigned char)( type | chn ) )
#define DIN_B2_AN_MAKE( chn, sp ) ((unsigned char)( DIN_ACTYPE_ANALOGUE | (chn << 4) | sp ) )

#define DIN_ACTYPE_ANALOGUE  0x80
#define DIN_ACTYPE_DIGITAL  0x00
// when setting thresholds cannot be digital
#define DIN_ACTYPE_TEMPERATURE  0x00
#define DIN_ACTYPE_SCENE  0x40
#define DIN_ACTYPE_INFRARED  0x00
#define DIN_ACTYPE_DMX  0x00

#define DIN_B2_IS_ANALOGUE(b1)  ((b1&DIN_ACTYPE_ANALOGUE)==DIN_ACTYPE_ANALOGUE)
#define DIN_B2_IS_DIGITAL(b1)  ((b1&0xC0)==DIN_ACTYPE_DIGITAL)
#define DIN_B2_IS_SCENE(b1)  ((b1&0xC0)==DIN_ACTYPE_SCENE)

#define DIN_B2_ANCHN(b1)  ((b1>>4)&0x7)
#define DIN_B2_DIGCHN(b1)  (b1& 0x3F)
//#define DIN_B2_SCENE(b1)  (b1& 0x1F)
//#define DIN_B2_TEMPCHN(b1)  (b1& 0x1F)
#define DIN_B2_CHN(b1)  (b1&0x80)!=0?DIN_B2_ANCHN(b1):DIN_B2_DIGCHN(b1)

#define DIN_B2_ANSETPOINT(b1)  (b1 & 0x0F)
#define DIN_B2_ACTION_TYPE(b1)  (b1&0x80)!=0?0x80:b1 & 0xC0

#define DInValue1Os (dig0cfgb3-dig0cfgb1)
#define DInValue2Os (dig0cfgb4-dig0cfgb1)
#define DInOptionsOs (dig0cfgb5-dig0cfgb1)
// defined options and tests on them
#define DIN_RISING_EDGE 1
#define DIN_FALLING_EDGE 2
#define DIN_ROTARY_ENCODER 4
#define DIN_B5_IS_RisingEdge(b1)  ((b1&DIN_RISING_EDGE)==DIN_RISING_EDGE)
#define DIN_B5_IS_FallingEdge(b1)  ((b1&DIN_FALLING_EDGE)==DIN_FALLING_EDGE)
#define DIN_B5_IS_RotaryEncoder(b1)  ((b1&DIN_ROTARY_ENCODER)==DIN_ROTARY_ENCODER)

#define DwellBaseLoc dw0
#define DwellEntryLen (dw1-dw0)
#define DwellCount 8

#define SetPBLoc sp0
#define SPCount 8
#define SPEntryLen (sp1-sp0)

//  Temperature configuration.
#define TempCfgBase temp0low
#define TempCfgSize (temp1low-temp0low)
#define TempCfgLowOs (temp0low-temp0low)
#define TempCfgLowActionOs (temp0lowb1-temp0low)
#define TempCfgLowTypeOs (temp0lowb2-temp0low)
#define TempCfgLowValue1Os (temp0lowb3-temp0low)
#define TempCfgLowValue2Os (temp0lowb4-temp0low)
#define TempCfgHiOs (temp0hi-temp0low)
#define TempCfgHiActionOs (temp0hib1-temp0low)
#define TempCfgHiTypeOs (temp0hib2-temp0low)
#define TempCfgHiValue1Os (temp0hib3-temp0low)
#define TempCfgHiValue2Os (temp0hib4-temp0low)

//  Analog Input configuration.
#define AnInCfgBaseLoc anin0low
#define AnInCfgSize (anin1low-anin0low)
#define AnInCfgLowOs (anin0low-anin0low)
#define AnInCfgLowActionOs (anin0lowb1-anin0low)
#define AnInCfgLowTypeOs (anin0lowb2-anin0low)
#define AnInCfgLowValue1Os (anin0lowb3-anin0low)
#define AnInCfgLowValue2Os (anin0lowb4-anin0low)
#define AnInCfgHiOs (anin0hi-anin0low)
#define AnInCfgHiActionOs (anin0hib1-anin0low)
#define AnInCfgHiTypeOs (anin0hib2-anin0low)
#define AnInCfgHiValue1Os (anin0hib3-anin0low)
#define AnInCfgHiValue2Os (anin0hib4-anin0low)

//
//   Note scehduling will use the same format for encoding its control bytes.
#define SchedBaseLoc sch0days
#define SchedEntryLen (sch1days-sch0days)
#define SchedEntryCount 16
#define SchedDaysOs 0
#define SchedHoursOs (sch0hours-sch0days)
#define SchedMinsOs (sch0mins-sch0days)
#define SchedActionOs (sch0cfgb1-sch0days)
#define SchedTypeOs (sch0cfgb2-sch0days)
#define SchedValue1Os (sch0cfgb3-sch0days)
#define SchedValue2Os (sch0cfgb4-sch0days)

// Digital In Names
#define DInNameBase dig0name
#define DInNameLen (dig1name-dig0name)
// There can be fewer name strings than digital in trigger defs.
#define DInNameCount ((digout0name-dig0name)/DInNameLen)

// Digital Out Names
#define DOutNameBase digout0name
#define DOutNameLen (digout1name-digout0name)
// Preparation. There are 8 primary digital outputs.
// They are named and have Mimics and can be the target of a scene.
#define DOutPrimaryCount 8
// There are 8 extended digital outputs
// The second 8 are unnamed and cannot be part of Scenes
#define DOutExtendedCount 8
#define DOutEntryCount (DOutPrimaryCount+DOutExtendedCount)

// Monitor Names
//#define MonNameBase monin0
//#define MonNameLen (monin1-monin0)
//#define MonEntryCount 4

// Analogue input names
#define AnInNameBase anin0name
#define AnInNameLen (anin1name-anin0name)

// Analogue output names
#define AnOutNameBase anout0name
#define AnOutNameLen (anout1name-anout0name)

// Temperature names
#define TempNameBase temp0name
#define TempNameLen (temp1name-temp0name)

#define VerMajorLoc swvermajor
#define VerMinorLoc swverminor
#define VerBuildLoc swverbuild

// Scenes
#define SceneCfgBase scene0digmask
#define SceneCfgSize (scene1digmask-scene0digmask)
#define SceneGroup1EntryStart 0
#define SceneGroup1EntryBase 0
#define SceneGroup1EntryCount 8
#define SceneGroup1EntryLimit (SceneGroup1EntryCount - 1)
#define SceneGroup2EntryBase SceneGroup1EntryCount
#define SceneGroup2EntryCount 4
#define SceneGroup2EntryLimit (SceneGroup2EntryBase + SceneGroup2EntryCount - 1)
#define SceneEntryCount (SceneGroup1EntryCount + SceneGroup2EntryCount)
#define SceneDigMaskOs 0
#define SceneDigStateOs (scene0digstate-scene0digmask)
#define SceneAnMaskOs (scene0anmask-scene0digmask)
#define SceneAnStateOs  (scene0anstate-scene0digmask)

#define MimicsForDigOutLoc  mimicsdigout
#define MimicsForAnOutLoc  mimicsanout
#define MimicsLevelLowLoc  mimicslow
#define MimicsLevelHighLoc  mimicshigh
#define MimicFadeRateLoc  mimicfaderate

#define SPEnd spend
#define SPPersist2 dw0
#define SPEnd2 0x300    // end of siteplayer memory
//#define SPEnd2 sp15+1

// End of configuration/status data.

// ===============================================================================
// WARNING, This is in low PIC EEPROM but does not go to the siteplayer
//
//  This is because UDPs only go to the SitePlayer
//  and the IP address will be going to upper locations  in the sp 
//  memory, therefore in the PIC the IP address is stored in a 
//  lower location
//
// ===============================================================================
// location of IP Addr in PIC EEPROM
#define IPAddrLoc 0x00
// view configuration password
#define Password1Loc 0x04
// location of password in peristant memory
#define PasswordLen 12
// location of password in peristant memory
#define Password2Loc 0x10
// location of password in peristant memory
#define Password3Loc 0x1C
// location of password in peristant memory
#define OptionsLoc 0x28
#define MaxOptions (PersistStart-OptionsLoc)
// to 0x003D in DATA EEPROM is available.
// And from spend to 0x3FF

// Start of MAC address in sp
#define SPMacAddr spmac0
// Start of IP address in sp
#define SPIPAddr spip0
// Start of UDP command packet
#define SPUdpDataPkt udpmac
// Start of UDP command packet
#define SPUdpCmdPkt spudprcvr

#if spend > udpmac
#pragma error( ".... ERROR Too many siteplayer objects, they overwite the udp command buffer" )
#endif

#endif
