#ifndef _SERINT2_H
#define _SERINT2_H

#pragma wizcpp uselib "$(FEDPATH)\libs\SerInt2.c"

#define TXBUFSZ2    32
#define RXBUFSZ2    32
#pragma asmline _TXBUFSZ2=32
#pragma asmline _RXBUFSZ2=32

extern unsigned char TxTab2[TXBUFSZ2];
extern unsigned char RxTab2[RXBUFSZ2];

unsigned char GetRxSize2(void);
unsigned char GetTxSize2(void);
void AddTx2(unsigned char TxChar);
void FlushTx2(void);
//void FlushRx2(void);
void ResendTx2(void);
unsigned char WaitRx2(void);
void SerIntHandler2(void);
void SerIntInit2(void);
void SetBaud2(unsigned baud);
//void SetTxByte2(unsigned char idx, unsigned char val);
#define S2_DmxSetLevel(idx, val)   TxTab2[idx] = val
#define S2_DmxGetLevel(idx) TxTab2[idx]
//void S2_DmxSetLevel(unsigned char idx, unsigned char val);
//void S2_DmxGetLevel(unsigned char idx);
void DmxBreak(void);
void DmxMAB(void);
void DmxB(void);
void DmxMBB(void);

void Serial2Enable();
void Serial2Disable();

#pragma asmfunc GetRxSize2
#pragma asmfunc GetTxSize2
#pragma asmfunc AddTx2
#pragma asmfunc ResendTx2
#pragma asmfunc FlushTx2
#pragma asmfunc WaitRx2
#pragma asmfunc SerIntHandler2
#pragma asmfunc SerIntInit2
#pragma asmfunc Serial2Enable
#pragma asmfunc Serial2Disable
#pragma asmfunc DmxBreak
#pragma asmfunc DmxMBB
#pragma asmfunc DmxMAB

#pragma forcequick SerIntHandler2

#endif
