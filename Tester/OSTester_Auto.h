#include <P16F886.h>
#include <P16F886_bits.h>
#include <PortBits.h>
#include <AppWiz.h>
#include <datalib.h>

#define APROCFREQ 8000000

#define BITSIZE 14
#define BOOTADDRESS 0
#define FIRSTRAM 0x00
#define LASTRAM 0x170
#define HASOSCCAL 0
#define nPAGESRAM 4
#define nPAGESROM 4
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


#define PortOutnCopy 3
#define SimpButnCopy 4
#pragma asmline _SimpButnCopy=4
// **********
//
// Seven Segment Pattern Element
//
// **********
#define SvSegPatUsed 1
#pragma asmline _SvSegPatUsed=1
// **********
//
// Dallas - 1 Wire interface
//
// **********
#define Dallas1WUsed 1
#pragma asmline _Dallas1WUsed=1
#define TxRx1WirePort PORTC
#define TxRx1WireBit 5
#pragma asmline _TxRx1WirePort=PORTC
#pragma asmline _TxRx1WireBit=5
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
// Module 1 - PWM D/A
//
// **********
#define PWM1Used 1
#define vPR2 255
#define VInit1 512
#define PWM1Port PORTC
#define PWM1Bit 2
// **********
//
// A/D converter for other devices
//
// **********
#define ADAllUsed 1
#define ADEn 1
#define InChan 0
#define ADClock OscDiv2
#define ADRtJust 1
#define AnaAN0 1
#define AnaAN1 0
#define AnaAN2 0
#define AnaAN3 0
#define AnaAN4 0
#define AnaAN8 0
#define AnaAN9 0
#define AnaAN10 0
#define AnaAN11 0
#define AnaAN12 0
#define AnaAN13 0
#define bVCFG1 VRefm_Vss
#define bVCFG0 VRefp_Vdd
#define A0Port PORTA
#define A0Bit 0
#define A1_VRefPort PORTA
#define A1_VRefBit 1
#define A2Port PORTA
#define A2Bit 2
#define A3Port PORTA
#define A3Bit 4
#define A4Port PORTA
#define A4Bit 5
#define A8Port PORTB
#define A8Bit 2
#define A9Port PORTB
#define A9Bit 3
#define A10Port PORTB
#define A10Bit 1
#define A11Port PORTB
#define A11Bit 4
#define A12Port PORTB
#define A12Bit 0
#define A13Port PORTB
#define A13Bit 5
// **********
//
// Port Driver
//
// **********
#define PortOutUsed 1
#define PortOut2Used 1
#define InitialValue2 0
#define Out2Port PORTC
#define Out2Bit 4
// **********
//
// Interrupt Driven Serial Interface
//
// **********
#define SerIntUsed 1
#pragma asmline _SerIntUsed=1
#define SerIntIntUsed 1
#pragma asmline _SerIntIntUsed=1
#define SERINTRATE 19200
#pragma asmline _SERINTRATE=19200
#define USEXON 1
#pragma asmline _USEXON=1
#define RXBUFSZ 32
#pragma asmline _RXBUFSZ=32
#define TXBUFSZ 8
#pragma asmline _TXBUFSZ=8
#define RxPort PORTC
#define RxBit 7
#pragma asmline _RxPort=PORTC
#pragma asmline _RxBit=7
#define TxPort PORTC
#define TxBit 6
#pragma asmline _TxPort=PORTC
#pragma asmline _TxBit=6
// **********
//
// Port Driver
//
// **********
#define PortOut1Used 1
#define InitialValue1 0
#define Out1Port PORTB
#define Out1Bit 5
// **********
//
// Port Driver
//
// **********
#define PortOut0Used 1
#define InitialValue0 0
#define Out0Port PORTB
#define Out0Bit 4
// **********
//
// Simple Switch
//
// **********
#define SimpButUsed 1
#pragma asmline _SimpButUsed=1
#define SimpBut3Used 1
#pragma asmline _SimpBut3Used=1
#define InitialDelay3 500
#pragma asmline _InitialDelay3=500
#define RepDelay3 250
#pragma asmline _RepDelay3=250
#define Debounce3 60
#pragma asmline _Debounce3=60
#define EnableRepeat3 0
#pragma asmline _EnableRepeat3=0
#define Invert3 0
#pragma asmline _Invert3=0
#define SBIn3Port PORTB
#define SBIn3Bit 3
#pragma asmline _SBIn3Port=PORTB
#pragma asmline _SBIn3Bit=3
// **********
//
// Simple Switch
//
// **********
#define SimpBut2Used 1
#pragma asmline _SimpBut2Used=1
#define InitialDelay2 500
#pragma asmline _InitialDelay2=500
#define RepDelay2 250
#pragma asmline _RepDelay2=250
#define Debounce2 60
#pragma asmline _Debounce2=60
#define EnableRepeat2 0
#pragma asmline _EnableRepeat2=0
#define Invert2 0
#pragma asmline _Invert2=0
#define SBIn2Port PORTB
#define SBIn2Bit 2
#pragma asmline _SBIn2Port=PORTB
#pragma asmline _SBIn2Bit=2
// **********
//
// Simple Switch
//
// **********
#define SimpBut1Used 1
#pragma asmline _SimpBut1Used=1
#define InitialDelay1 500
#pragma asmline _InitialDelay1=500
#define RepDelay1 250
#pragma asmline _RepDelay1=250
#define Debounce1 60
#pragma asmline _Debounce1=60
#define EnableRepeat1 0
#pragma asmline _EnableRepeat1=0
#define Invert1 0
#pragma asmline _Invert1=0
#define SBIn1Port PORTB
#define SBIn1Bit 1
#pragma asmline _SBIn1Port=PORTB
#pragma asmline _SBIn1Bit=1
// **********
//
// Simple Switch
//
// **********
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
#define SBIn0Bit 0
#pragma asmline _SBIn0Port=PORTB
#pragma asmline _SBIn0Bit=0
// **********
//
// Timer 0
//
// **********
#define Tmr0Used 1
void SvSegTime();
void SBT0Of();
#define UsePS 1
#define PSD 4
#define CRE 1
#define UseOsc 1
// **********
//
// 7 Segment Display Driver
//
// **********
#define SvSegUsed 1
#pragma asmline _SvSegUsed=1
#define nDigit 2
#pragma asmline _nDigit=2
#define CommonDrive CAnode
#pragma asmline _CommonDrive=CAnode
#define Prescale7Seg 5
#pragma asmline _Prescale7Seg=5
#define SegAPort PORTA
#define SegABit 1
#pragma asmline _SegAPort=PORTA
#pragma asmline _SegABit=1
#define SegBPort PORTA
#define SegBBit 2
#pragma asmline _SegBPort=PORTA
#pragma asmline _SegBBit=2
#define SegCPort PORTA
#define SegCBit 3
#pragma asmline _SegCPort=PORTA
#pragma asmline _SegCBit=3
#define SegDPort PORTA
#define SegDBit 4
#pragma asmline _SegDPort=PORTA
#pragma asmline _SegDBit=4
#define SegEPort PORTA
#define SegEBit 5
#pragma asmline _SegEPort=PORTA
#pragma asmline _SegEBit=5
#define SegFPort PORTA
#define SegFBit 6
#pragma asmline _SegFPort=PORTA
#pragma asmline _SegFBit=6
#define SegGPort PORTA
#define SegGBit 7
#pragma asmline _SegGPort=PORTA
#pragma asmline _SegGBit=7
#define Digit1Port PORTC
#define Digit1Bit 0
#pragma asmline _Digit1Port=PORTC
#pragma asmline _Digit1Bit=0
#define Digit2Port PORTC
#define Digit2Bit 3
#pragma asmline _Digit2Port=PORTC
#pragma asmline _Digit2Bit=3

// **********
//
// Application Designer Variables
//
// **********
extern unsigned char register Temp1;
extern unsigned char register Temp2;
extern unsigned char register Temp3;


// Defines specified by 1 or more elements

