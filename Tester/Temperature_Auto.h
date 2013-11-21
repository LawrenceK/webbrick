#include <P16F88.h>
#include <P16F88_bits.h>
#include <PortBits.h>
#include <Appwiz.h>
#include <datalib.h>
#include <delays.h>

#define APROCFREQ 8000000

#define BITSIZE 14
#define BOOTADDRESS 0
#define FIRSTRAM 0x20
#define LASTRAM 0x1FF
#define HASOSCCAL 0
#define nPAGESRAM 4
#define nPAGESROM 2
//
// Header file - should be included in all C files used within the program
// This file is created automatically and should not be changed
//

//
// Functions
//
void UserInitialise();			// Your initialisation code
void UserInterrupt();			// Your interrupt code
void UserLoop();			// your Main loop code


#define SimpButnCopy 1
#pragma asmline _SimpButnCopy=1
#define PortOutnCopy 7
// **********
//
// Module 1 - PWM D/A
//
// **********
#define PWM1Used 1
#define vPR2 255
#define VInit1 512
#define PWM1Port PORTB
#define PWM1Bit 0
// **********
//
// A/D converter for 16F88 device
//
// **********
#define AD16F88Used 1
#define ADEn 1
#define InChan 0
#define ADClock OscDiv64
#define VRefCfg vc_Vdd
#define ADRtJust 1
#define AnaAN0 1
#define AnaAN1 0
#define AnaAN2 0
#define AnaAN3 0
#define AnaAN4 0
#define AnaAN5 0
#define AnaAN6 0
#define A0Port PORTA
#define A0Bit 0
#define A1Port PORTA
#define A1Bit 1
#define A2_VRefMPort PORTA
#define A2_VRefMBit 2
#define A3_VRefPPort PORTA
#define A3_VRefPBit 3
#define A4Port PORTA
#define A4Bit 4
#define A5Port PORTB
#define A5Bit 6
#define A6Port PORTB
#define A6Bit 7
// **********
//
// Port B Driver
//
// **********
#define PBDriveUsed 1
#pragma asmline _PBDriveUsed=1
#define UsePU 1
#pragma asmline _UsePU=1
#define BIntEdge 0
#pragma asmline _BIntEdge=0
#define RB0Port PORTB
#define RB0Bit 0
#pragma asmline _RB0Port=PORTB
#pragma asmline _RB0Bit=0
#define RB1Port PORTB
#define RB1Bit 1
#pragma asmline _RB1Port=PORTB
#pragma asmline _RB1Bit=1
#define RB2Port PORTB
#define RB2Bit 2
#pragma asmline _RB2Port=PORTB
#pragma asmline _RB2Bit=2
#define RB3Port PORTB
#define RB3Bit 3
#pragma asmline _RB3Port=PORTB
#pragma asmline _RB3Bit=3
#define RB4Port PORTB
#define RB4Bit 4
#pragma asmline _RB4Port=PORTB
#pragma asmline _RB4Bit=4
#define RB5Port PORTB
#define RB5Bit 5
#pragma asmline _RB5Port=PORTB
#pragma asmline _RB5Bit=5
#define RB6Port PORTB
#define RB6Bit 6
#pragma asmline _RB6Port=PORTB
#pragma asmline _RB6Bit=6
#define RB7Port PORTB
#define RB7Bit 7
#pragma asmline _RB7Port=PORTB
#pragma asmline _RB7Bit=7
// **********
//
// WatchDog
//
// **********
#define WatchDogUsed 1
#pragma asmline _WatchDogUsed=1
#define WDUsePS 0
#pragma asmline _WDUsePS=0
#define WDPSD 1
#pragma asmline _WDPSD=1
// **********
//
// Port Driver
//
// **********
#define PortOutUsed 1
#define PortOut6Used 1
#define InitialValue6 0
#define Out6Port PORTB
#define Out6Bit 3
// **********
//
// Hours, Minutes and Seconds
//
// **********
#define HMSUsed 1
#pragma asmline _HMSUsed=1
#pragma asmline _SecPassUsed=1
#pragma asmline _SecPassFlag=Flag1
#pragma asmline _SecPassFlagBit=0
#define SecPassUsed 1
#define SecPassFlag Flag1
#define SecPassFlagBit 0
void SecOcc();
// **********
//
// Wait for W milli-seconds
//
// **********
#define WaitWmUsed 1
// **********
//
// Timer 0
//
// **********
#define Tmr0Used 1
void SBT0Of();
void HMSTick();
void MSTick();
#define UsePS 1
#define PSD 4
#define CRE 1
#define UseOsc 1
// **********
//
// Simple Switch
//
// **********
#define SimpButUsed 1
#pragma asmline _SimpButUsed=1
#define SimpBut0Used 1
#pragma asmline _SimpBut0Used=1
#define InitialDelay0 500
#pragma asmline _InitialDelay0=500
#define RepDelay0 250
#pragma asmline _RepDelay0=250
#define Debounce0 60
#pragma asmline _Debounce0=60
#define EnableRepeat0 0
#pragma asmline _EnableRepeat0=0
#define Invert0 0
#pragma asmline _Invert0=0
#define SBIn0Port PORTB
#define SBIn0Bit 4
#pragma asmline _SBIn0Port=PORTB
#pragma asmline _SBIn0Bit=4
// **********
//
// Port Driver
//
// **********
#define PortOut5Used 1
#define InitialValue5 0
#define Out5Port PORTA
#define Out5Bit 1
// **********
//
// Timer 2
//
// **********
#define Tmr2Used 1
#define T2_Post 1
#define T2_Pre 1
#define T2_En 1
// **********
//
// Port Driver
//
// **********
#define PortOut4Used 1
#define InitialValue4 0
#define Out4Port PORTA
#define Out4Bit 2
// **********
//
// Port Driver
//
// **********
#define PortOut3Used 1
#define InitialValue3 0
#define Out3Port PORTA
#define Out3Bit 3
// **********
//
// Port Driver
//
// **********
#define PortOut2Used 1
#define InitialValue2 0
#define Out2Port PORTA
#define Out2Bit 4
// **********
//
// Port Driver
//
// **********
#define PortOut1Used 1
#define InitialValue1 0
#define Out1Port PORTA
#define Out1Bit 6
// **********
//
// Port Driver
//
// **********
#define PortOut0Used 1
#define InitialValue0 0
#define Out0Port PORTA
#define Out0Bit 7
// **********
//
// Dallas - 1 Wire interface
//
// **********
#define Dallas1WUsed 1
#pragma asmline _Dallas1WUsed=1
#define TxRx1WirePort PORTB
#define TxRx1WireBit 1
#pragma asmline _TxRx1WirePort=PORTB
#pragma asmline _TxRx1WireBit=1

// **********
//
// Application Designer Variables
//
// **********
extern unsigned char register Flag1;
extern unsigned char register Temp1;
extern unsigned char register Temp2;
extern unsigned char register Temp3;


// Defines specified by 1 or more elements

