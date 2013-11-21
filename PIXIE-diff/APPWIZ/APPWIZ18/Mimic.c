#ifdef MimicUsed
#include "mimic.h"
unsigned char mimicTable[MAX_MIMIC_LEVEL];
static unsigned char mimicLevel[MimicCount];
static unsigned char mimicTarget[MimicCount];
// This maps mimic numbers to there position in port E.
#if RotateMimics == 1
const unsigned char mimicMap[8] = {0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x01, 0x02};
#define MIMICBIT(idx)   (mimicMap[idx])
#else
#define MIMICBIT(idx)   (0x01 << idx)
#endif

// **********
// * MimicSetPwmTable( unsigned char chnBit, unsigned char newLevel )
// * 
// **********
static void MimicSetPwmTable( unsigned char chnBit, unsigned char newLevel )
{
    // no mimic if used for debug
    unsigned char idx;
    unsigned char val = 0;
    unsigned char notchnBit;
    notchnBit = ~chnBit;
    for ( idx = 0; idx < MAX_MIMIC_LEVEL; idx++ )
    {
        val += newLevel;
        // the higher newLevel is the quicker we excede MAX_MIMIC_LEVEL
        // and therefore more often set the bit in the map.
        if ( val >= MAX_MIMIC_LEVEL )
        {
            mimicTable[idx] |= chnBit;
            val -= MAX_MIMIC_LEVEL;
        }
        else
        {
            mimicTable[idx] &= notchnBit;
        }
    }
}

// called to handle fading a mimic towards its desired level.
void FadeMimics()
{
    unsigned char idx;
    for ( idx = 0; idx < MimicCount; ++idx)
    {
        if ( mimicLevel[idx] != mimicTarget[idx] )
        {
            if ( mimicLevel[idx] > mimicTarget[idx] )
            {
                --mimicLevel[idx];
            }
            else
            {
                ++mimicLevel[idx];
            }
            MimicSetPwmTable( MIMICBIT(idx), mimicLevel[idx] );
//            UpdateMimic( 0x01 << idx, mimicLevel[idx] );
        }
    }
}

// Called to set the target level for a mimic when faded to.
void SetMimicTargetLevel(unsigned char chn, unsigned char targetLevel )
{
    if ( targetLevel > MAX_MIMIC_LEVEL )
    {
        targetLevel = MAX_MIMIC_LEVEL;
    }
    mimicTarget[chn] = targetLevel;
}

// called to set an immediate level for a mimic.
void SetMimicLevelNow(unsigned char chn, unsigned char targetLevel )
{
    if ( targetLevel > MAX_MIMIC_LEVEL )
    {
        targetLevel = MAX_MIMIC_LEVEL;
    }
    mimicTarget[chn] = targetLevel;
    mimicLevel[chn] = targetLevel;
    MimicSetPwmTable( MIMICBIT(chn), mimicLevel[chn] );
}

#if 0
//#pragma asm
module "_PwmMimicsInt"
_PwmMimicsInt::
        btfss PIR3,CCP5IF
        goto MimicIntEnd
        bcf PIR3,CCP5IF

        MOVFF   FSR0L,ISaveFSR0L
        MOVFF   FSR0H,ISaveFSR0H
        MOVLB   mimicIndex >> 8
        LFSR    FSR0, mimicTable
        MOVF    mimicIndex, w, BANKED ; get current index
        MOVFF   PLUSW0, LATE        ; update output
        INCF    mimicIndex, f, BANKED ; update current index
        BTFSC   mimicIndex, 6, BANKED ; have we reached 64
        CLRF    mimicIndex, BANKED    ; reset

        MOVFF   ISaveFSR0L,FSR0L
        MOVFF   ISaveFSR0H,FSR0H

MimicIntEnd:
        return 0

void PwmMimics()
{
//#asm
//      MOVLB   mimicIndex >> 8
//      LFSR    FSR0, mimicTable
//      MOVF    mimicIndex, w, BANKED ; get current index
//      MOVFF   PLUSW0, LATE        ; update output
//      INCF    mimicIndex, f, BANKED ; update current index
//      BTFSC   mimicIndex, 6, BANKED ; have we reached 64
//      CLRF    mimicIndex, BANKED    ; reset
//#asmend
    if ( mimicIndex > MAX_MIMIC_LEVEL )
    {
        mimicIndex = 0;
    }
    LATE = mimicTable[mimicIndex];
    mimicIndex++;
}
#endif


#endif