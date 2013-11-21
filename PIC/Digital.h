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

struct _tagDigPort
{
    unsigned char state;
    unsigned char pending;
    unsigned char lockout;
};
// 1 entry per set of up to 8 inputs managed together.
extern struct _tagDigPort DigitalWork[2];

// Static data (Global scope)

// Exported functions declarations
// these two used to lockout an input when being used fro a second purpose.
extern void reserveInput( unsigned char chn );
extern void unReserveInput( unsigned char chn );
extern void reserveOutput( unsigned char chn );
extern void unReserveOutput( unsigned char chn );

// Decode and do the correct trigger.
// can be called by dig input, analogue threshold, temp threshold, scheduled event
void PerformTrigger( unsigned char ttype, unsigned char srcChn, unsigned baseLoc );
// Action atrigger that has already been decoded.
void ActionTrigger( unsigned char ttype, unsigned char srcChn );

void DigitalInit();

void DigitalOut (unsigned char chn, unsigned char operand );	// generic digital out handler

void DoScene( unsigned snum, unsigned char operand );

// Called at 1 second intervals.
void DigitalPeriodic();

void CheckDigital();

#ifdef MimicUsed

#define Mimic12V()    bRC5 = 1
#define Mimic5V()     bRC5 = 0

void SetMimicState( unsigned char chn, unsigned char isOn );
//void UpdateMimic( unsigned char chn, unsigned char targetLevel );

void DigitalMimicInit();

extern unsigned char mimicOff;
extern unsigned char mimicOn;
extern unsigned char mimicForDig[(DOutPrimaryCount+1)/2];

#endif
