#ifdef MimicUsed

#define MAX_MIMIC_LEVEL  64
#define MimicCount 8
extern unsigned char mimicTable[MAX_MIMIC_LEVEL];

//void MimicSetPwmTable( unsigned char chnBit, unsigned char newLevel );

// Call periodically to handle mimic fades
void FadeMimics();

// Call to update target Mimic level.
void SetMimicTargetLevel( unsigned char chn, unsigned char targetLevel );
// Call to set an immediate value for the mimic.
void SetMimicLevelNow(unsigned char chn, unsigned char targetLevel );

#endif
