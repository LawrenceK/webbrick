#include "D:\\o2m8\\svn\\WebBrick\\Trunk\\Tester\\Temperature_Auto.h"


// **********
//
// Application Designer Variables
//
// **********
unsigned char register Flag1;
unsigned char register Temp1;
unsigned char register Temp2;
unsigned char register Temp3;
//
// This file is automatically generated. Any changes will be overwritten when
// the application is regenerated
//
void LoadOscCal();
// Conditional Defines
const int OscDiv2=0; // A/D converter for 16F88 device
const int OscDiv8=0x40;
const int OscDiv32=0x80;
const int ADIntRC=0xc0;
const int OscDiv4=0x100;
const int OscDiv16=0x140;
const int OscDiv64=0x180;
const int vc_Vdd=0;
const int vc_Vdd_VRef=0x10;
const int vc_VRef_Vss=0x20;
const int vc_VRefP_VRefM=0x30;

#if WDPSD==1 // WatchDog
 #define vWDPSD 0
#endif
#if WDPSD==2
 #define vWDPSD 1
#endif
#if WDPSD==4
 #define vWDPSD 2
#endif
#if WDPSD==8
 #define vWDPSD 3
#endif
#if WDPSD==16
 #define vWDPSD 4
#endif
#if WDPSD==32
 #define vWDPSD 5
#endif
#if WDPSD==64
 #define vWDPSD 6
#endif
#if WDPSD==128
 #define vWDPSD 7
#endif
#ifdef HourPassUsed // Hours, Minutes and Seconds
 unsigned char register HrCount;
 unsigned char register HrCount1;
 unsigned char register HrCount2;
 unsigned char register HrCount3;
#endif
#ifdef MinPassUsed
 unsigned char register MinCount;
 unsigned char register MinCount1;
 unsigned char register MinCount2;
#endif
#ifdef SecPassUsed
 unsigned char register SecCount;
 unsigned char register SecCount1;
#endif

#if PSD==2 // Timer 0
 #define T0PSD 0
#endif
#if PSD==4
 #define T0PSD 1
#endif
#if PSD==8
 #define T0PSD 2
#endif
#if PSD==16
 #define T0PSD 3
#endif
#if PSD==32
 #define T0PSD 4
#endif
#if PSD==64
 #define T0PSD 5
#endif
#if PSD==128
 #define T0PSD 6
#endif
#if PSD==256
 #define T0PSD 7
#endif
#if (T2_Pre==1) // Timer 2
 #define vT2CON ((T2_Post-1)<<3)|(T2_En<<2)|(0)
#endif
#if (T2_Pre==4)
 #define vT2CON ((T2_Post-1)<<3)|(T2_En<<2)|(1)
#endif
#if (T2_Pre==16)
 #define vT2CON ((T2_Post-1)<<3)|(T2_En<<2)|(2)
#endif

// *** Input File - D:\o2m8\svn\WebBrick\Trunk\PIXIE\APPWIZ\MiscInit.c

// APPWIZ version

#ifdef SimpButUsed	// Init Code for Simp But - once only
const int T0PS=UsePS*PSD+!UsePS;		// T0 prescale
const int SBCT=(1000000000/APROCFREQ)*4;	// Cycle time in nS
const int SBTmr0OF=(SBCT*T0PS*256)/1000;	// Overflow time in uS

const int SBPSC=20000/SBTmr0OF;	                // Prescalar overflows every 20mS
const int SBPSCNZ=(SBPSC) ? SBPSC : 1;
const int SBPreScalarCnt=(SBPSCNZ>255) ? 255 : SBPSC;

const int SBIC=InitialDelay0/20;		// Delay before 1st repeat
const int SBInitCnt=(SBIC>127) ? 127 : SBIC;

const int SBRC=RepDelay0/20;			// Delay between repeats
const int SBRepCnt=(SBRC>127) ? 127 : SBRC;

const int SBDC=Debounce0/20;			// Debounce time after rising
const int SBDebounceCnt=(SBDC>127) ? 127 : SBDC;

unsigned char register SBPreScalar=SBPreScalarCnt;  // Pre-scalar is common to all buttons
#endif
#ifdef SimpButUsed	// %% Shows duplication for each instance
unsigned char register SBStatus0; 	// Each button has its own status byte
#endif
                                                        


//
// Main function
//

void main()
{
  OPTION_REG&=0x7f;		// Pull ups on PORTB by default
 #if HASOSCCAL==1
  LoadOscCal();
 #endif
 
 // Initialisation Code
 PR2=vPR2;
 CCPR1L=VInit1>>2;
 CCP1CON=(((VInit1<<4)&0x30))|0x0c);
 ADCON0=(ADClock&0xc0)|((InChan&7)<<3)|ADEn;
 ADCON1=(ADRtJust<<7)|((ADClock>>2)&0x40)|VRefCfg;
 ANSEL=(AnaAN6<<6)|(AnaAN5<<5)|(AnaAN4<<4)|(AnaAN3<<3)|(AnaAN2<<2)|(AnaAN1<<1)|AnaAN0;
 OPTION_REG&=~((1<<NOT_RBPU)|(1<<INTEDG));
 OPTION_REG|=((!UsePU)<<NOT_RBPU)|(BIntEdge<<INTEDG);
 #pragma asmline clrwdt
 #if WDUsePS
  OPTION_REG&=~((1<<PSA)|7);
  OPTION_REG|=(WDUsePS<<PSA)|vWDPSD;
 #else
  OPTION_REG&=~(1<<PSA);
  OPTION_REG|=(WDUsePS<<PSA);
 #endif
 
 #if UsePS==1
  OPTION_REG&=~((1<<PSA)|(1<<T0CS)|(1<<T0SE)|7);
  OPTION_REG|=((!UsePS)<<PSA)|((!UseOsc)<<T0CS)|((!CRE)<<T0SE)|T0PSD;
 #else
  OPTION_REG&=~((1<<PSA)|(1<<T0CS)|(1<<T0SE));
  OPTION_REG|=((!UsePS)<<PSA)|((!UseOsc)<<T0CS)|((!CRE)<<T0SE);
 #endif
 
 TMR0=0;
 T2CON=vT2CON;
 TMR2=0;
 Flag1=0;
 

 // End Initialisation Code
 PORTA|=0xff;
 PORTB|=0xff;
 INTCON&=0xf8;
 TRISB&=0xf7;
 TRISA&=0x21;
#if !InitialValue6
 Out6Port&=~(1<<Out6Bit); // Port Driver
#endif // Port Driver
 // Port Driver
 HMSInit();
#if !InitialValue5
 Out5Port&=~(1<<Out5Bit); // Port Driver
#endif // Port Driver
 // Port Driver
#if !InitialValue4
 Out4Port&=~(1<<Out4Bit); // Port Driver
#endif // Port Driver
 // Port Driver
#if !InitialValue3
 Out3Port&=~(1<<Out3Bit); // Port Driver
#endif // Port Driver
 // Port Driver
#if !InitialValue2
 Out2Port&=~(1<<Out2Bit); // Port Driver
#endif // Port Driver
 // Port Driver
#if !InitialValue1
 Out1Port&=~(1<<Out1Bit); // Port Driver
#endif // Port Driver
 // Port Driver
#if !InitialValue0
 Out0Port&=~(1<<Out0Bit); // Port Driver
#endif // Port Driver
 // Port Driver
 
 UserInitialise();
 
 // Finally enable interrupts
  
 INTCON|=(1<<GIE);
 
 // Main Loop
 
 while(1)				
 {					// Loop forever
  if (SecPassFlag&(1<<SecPassFlagBit))		// Occurs once every second
  {
   SecPassFlag&=~(1<<SecPassFlagBit);
   SecOcc();
  }
  if (INTCON&(1<<T0IF))		// Timer 0 overflow
  {
   INTCON&=~(1<<T0IF);
   SBT0Of();
   HMSTick();
   MSTick();
  }
  UserLoop();
  #ifdef WatchDogUsed			
   #pragma asmline clrwdt		; Clear watchdog timer if used
  #endif
 }
}

//
// Interrupts
//
const int QuickInt=1;

void Interrupt()
{
 // Priority Interrupts first
 // Other Interrupts
 #pragma asm
  SETPCLATH UserInterrupt,-1 	; Set PCLATH
  goto UserInterrupt	    	; goto user interrupt
 UserIntReturn::	    	; Return to here after user routine
  clrf STATUS		; Clear RP0/RP1
  SETPCLATH IntRet,-1		; and set PCLATH back to return address 
 #pragma asmend
}

//
// Load oscillator calibration value on reset for 14 bit processors
//
#if HASOSCCAL==1
	#pragma asm
		module "LoadOscCal"
LoadOscCal::
		SETPCLATH OVRet,LoadOscCal
                call OVRet
                bsf STATUS,RP0
                movwf OSCCAL
                bcf STATUS,RP0
                MRET 0
                
                endmodule
                
		module "LoadOscCal" forced absolute _TOPROM+1
		 OVRet::
		 	retlw 0xff
		endmodule
	#pragma asmend
#endif


