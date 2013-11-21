#include "D:\\o2m8\\newsvn\\WebBrick\\Trunk\\Tester\\OSTester_Auto.h"


// *** Input File - D:\o2m8\newsvn\WebBrick\Trunk\PIXIE\APPWIZ\Misc.c

#ifdef SimpButUsed	// Common code for all buttons
#pragma asm
	; MFORCEPAGE D'64'
	
SBMacro	MACRO
	 if _SimpButnCopy>0
	  SETPCLATH SBCount0,SBT0Of
	  call SBCount0
	 endif
	 if _SimpButnCopy>1
	  SETPCLATH SBCount1,SBCount0
	  call SBCount1
	 endif
	 if _SimpButnCopy>2
	  SETPCLATH SBCount2,SBCount1
	  call SBCount2
	 endif
	 if _SimpButnCopy>3
	  SETPCLATH SBCount3,SBCount2
	  call SBCount3
	 endif
	 if _SimpButnCopy>4
	  SETPCLATH SBCount4,SBCount3
	  call SBCount4
	 endif
	 if _SimpButnCopy>5
	  SETPCLATH SBCount5,SBCount4
	  call SBCount5
	 endif
	 if _SimpButnCopy>6
	  SETPCLATH SBCount6,SBCount5
	  call SBCount6
	 endif
	 if _SimpButnCopy>7
	  SETPCLATH SBCount7,SBCount6
	  call SBCount7
	 endif
	 if _SimpButnCopy>8
	  SETPCLATH SBCount8,SBCount7
	  call SBCount8
	 endif
	 if _SimpButnCopy>9
	  SETPCLATH SBCount9,SBCount8
	  call SBCount9
	 endif
	 if _SimpButnCopy>D'10'
	  SETPCLATH SBCount10,SBCount9
	  call SBCount10
	 endif
	 if _SimpButnCopy>D'11'
	  SETPCLATH SBCount11,SBCount10
	  call SBCount11
	 endif
	 if _SimpButnCopy>D'12'
	  SETPCLATH SBCount12,SBCount11
	  call SBCount12
	 endif
	 if _SimpButnCopy>D'13'
	  SETPCLATH SBCount13,SBCount12
	  call SBCount13
	 endif
	 if _SimpButnCopy>D'14'
	  SETPCLATH SBCount14,SBCount13
	  call SBCount14
	 endif
	 if _SimpButnCopy>D'15'
	  SETPCLATH SBCount15,SBCount14
	  call SBCount15
	 endif
	endm
	
	module "SBT0Of"
SBT0Of::
	decfsz SBPreScalar
	goto RetSBT0
	movlw _SBPreScalarCnt
	movwf SBPreScalar
	SBMacro
RetSBT0	MRET 0
	endmodule
#pragma asmend
#endif
#ifdef SimpButUsed	// %% Shows duplication for each instance
#pragma asm
	module "SBCount0"
	; MFORCEPAGE D'40'
;
; Come here every time pre-scalar times out 
;
SBCount0::
	tstf SBStatus0		; If zero then switch currently open
	skpz
	goto SBTO0
	#if Invert0==0
	 btfsc _SBIn0Port,_SBIn0Bit	; Is it low ?
	#else
	 btfss _SBIn0Port,_SBIn0Bit	; Is it high ?
	#endif
	return
	#if EnableRepeat0==1
	 movlw (_SBInitCnt|H'80')		; Show we are in initial delay
	#else
	 movlw H'80'
	#endif
	movwf SBStatus0
	goto SBRetOc0
;
; Switch pressed last time
;
SBTO0:	
   	btfsc SBStatus0,7
   	goto SBChkHI0
	decf SBStatus0			; Debounce counter from switch Hi
	return
SBChkHI0:
	#if Invert0==0
	 btfss _SBIn0Port,_SBIn0Bit	; Closed ?
	#else
	 btfss _SBIn0Port,_SBIn0Bit	; Closed ?
	#endif
	goto SBTimes0
	movlw _SBDebounceCnt		; Switch now high - so debounce
	movwf SBStatus0
	ifdef _SBRelease0Used
	 bsf _SBRelease0Flag,_SBRelease0FlagBit
	endif
	return
SBTimes0:
	#if EnableRepeat0==1
	decf SBStatus0,w
	andlw H'7f'
	movwf SBStatus0
	bsf SBStatus0,7
	skpz
	#endif
	return
	#if EnableRepeat0==1
	movlw (_SBRepCnt|H'80')			; Show we are in initial delay
	movwf SBStatus0
	#endif
SBRetOc0:
	ifdef _SBPress0Used
	 bsf _SBPress0Flag,_SBPress0FlagBit	; Occurrence
	endif
	return
	endmodule
#pragma asmend
#endif
#ifdef SimpButUsed	// %% Shows duplication for each instance
#pragma asm
	module "SBCount1"
	; MFORCEPAGE D'40'
;
; Come here every time pre-scalar times out 
;
SBCount1::
	tstf SBStatus1		; If zero then switch currently open
	skpz
	goto SBTO1
	#if Invert1==0
	 btfsc _SBIn1Port,_SBIn1Bit	; Is it low ?
	#else
	 btfss _SBIn1Port,_SBIn1Bit	; Is it high ?
	#endif
	return
	#if EnableRepeat1==1
	 movlw (_SBInitCnt|H'80')		; Show we are in initial delay
	#else
	 movlw H'80'
	#endif
	movwf SBStatus1
	goto SBRetOc1
;
; Switch pressed last time
;
SBTO1:	
   	btfsc SBStatus1,7
   	goto SBChkHI1
	decf SBStatus1			; Debounce counter from switch Hi
	return
SBChkHI1:
	#if Invert1==0
	 btfss _SBIn1Port,_SBIn1Bit	; Closed ?
	#else
	 btfss _SBIn1Port,_SBIn1Bit	; Closed ?
	#endif
	goto SBTimes1
	movlw _SBDebounceCnt		; Switch now high - so debounce
	movwf SBStatus1
	ifdef _SBRelease1Used
	 bsf _SBRelease1Flag,_SBRelease1FlagBit
	endif
	return
SBTimes1:
	#if EnableRepeat1==1
	decf SBStatus1,w
	andlw H'7f'
	movwf SBStatus1
	bsf SBStatus1,7
	skpz
	#endif
	return
	#if EnableRepeat1==1
	movlw (_SBRepCnt|H'80')			; Show we are in initial delay
	movwf SBStatus1
	#endif
SBRetOc1:
	ifdef _SBPress1Used
	 bsf _SBPress1Flag,_SBPress1FlagBit	; Occurrence
	endif
	return
	endmodule
#pragma asmend
#endif
#ifdef SimpButUsed	// %% Shows duplication for each instance
#pragma asm
	module "SBCount2"
	; MFORCEPAGE D'40'
;
; Come here every time pre-scalar times out 
;
SBCount2::
	tstf SBStatus2		; If zero then switch currently open
	skpz
	goto SBTO2
	#if Invert2==0
	 btfsc _SBIn2Port,_SBIn2Bit	; Is it low ?
	#else
	 btfss _SBIn2Port,_SBIn2Bit	; Is it high ?
	#endif
	return
	#if EnableRepeat2==1
	 movlw (_SBInitCnt|H'80')		; Show we are in initial delay
	#else
	 movlw H'80'
	#endif
	movwf SBStatus2
	goto SBRetOc2
;
; Switch pressed last time
;
SBTO2:	
   	btfsc SBStatus2,7
   	goto SBChkHI2
	decf SBStatus2			; Debounce counter from switch Hi
	return
SBChkHI2:
	#if Invert2==0
	 btfss _SBIn2Port,_SBIn2Bit	; Closed ?
	#else
	 btfss _SBIn2Port,_SBIn2Bit	; Closed ?
	#endif
	goto SBTimes2
	movlw _SBDebounceCnt		; Switch now high - so debounce
	movwf SBStatus2
	ifdef _SBRelease2Used
	 bsf _SBRelease2Flag,_SBRelease2FlagBit
	endif
	return
SBTimes2:
	#if EnableRepeat2==1
	decf SBStatus2,w
	andlw H'7f'
	movwf SBStatus2
	bsf SBStatus2,7
	skpz
	#endif
	return
	#if EnableRepeat2==1
	movlw (_SBRepCnt|H'80')			; Show we are in initial delay
	movwf SBStatus2
	#endif
SBRetOc2:
	ifdef _SBPress2Used
	 bsf _SBPress2Flag,_SBPress2FlagBit	; Occurrence
	endif
	return
	endmodule
#pragma asmend
#endif
#ifdef SimpButUsed	// %% Shows duplication for each instance
#pragma asm
	module "SBCount3"
	; MFORCEPAGE D'40'
;
; Come here every time pre-scalar times out 
;
SBCount3::
	tstf SBStatus3		; If zero then switch currently open
	skpz
	goto SBTO3
	#if Invert3==0
	 btfsc _SBIn3Port,_SBIn3Bit	; Is it low ?
	#else
	 btfss _SBIn3Port,_SBIn3Bit	; Is it high ?
	#endif
	return
	#if EnableRepeat3==1
	 movlw (_SBInitCnt|H'80')		; Show we are in initial delay
	#else
	 movlw H'80'
	#endif
	movwf SBStatus3
	goto SBRetOc3
;
; Switch pressed last time
;
SBTO3:	
   	btfsc SBStatus3,7
   	goto SBChkHI3
	decf SBStatus3			; Debounce counter from switch Hi
	return
SBChkHI3:
	#if Invert3==0
	 btfss _SBIn3Port,_SBIn3Bit	; Closed ?
	#else
	 btfss _SBIn3Port,_SBIn3Bit	; Closed ?
	#endif
	goto SBTimes3
	movlw _SBDebounceCnt		; Switch now high - so debounce
	movwf SBStatus3
	ifdef _SBRelease3Used
	 bsf _SBRelease3Flag,_SBRelease3FlagBit
	endif
	return
SBTimes3:
	#if EnableRepeat3==1
	decf SBStatus3,w
	andlw H'7f'
	movwf SBStatus3
	bsf SBStatus3,7
	skpz
	#endif
	return
	#if EnableRepeat3==1
	movlw (_SBRepCnt|H'80')			; Show we are in initial delay
	movwf SBStatus3
	#endif
SBRetOc3:
	ifdef _SBPress3Used
	 bsf _SBPress3Flag,_SBPress3FlagBit	; Occurrence
	endif
	return
	endmodule
#pragma asmend
#endif
#ifdef SvSegUsed
///////////////////////////////////////////////////////////////////////////////
//
// 7 Segment display driver
//
///////////////////////////////////////////////////////////////////////////////
#pragma asm
		module "SevSeg"
		; MFORCEPAGE 100
SvSegInit::
		movlw Pat7
		movwf FSR
		MSETIRP Pat7
		movlw _nDigit
		movwf Temp
ClrDigLop	clrf 0
		incf FSR
		decfsz Temp
		goto ClrDigLop
		call SvSegAllOff
		movlw Pat7
		movwf FSR

		MSETRP TRISA
		if _CommonDrive==1
		 call SvSegSetBars  ; Set all o/p's to drive as _Segments all 0
		else
		 bcf _SegAPort,_SegABit
		 bcf _SegBPort,_SegBBit
		 bcf _SegCPort,_SegCBit
		 bcf _SegDPort,_SegDBit
		 bcf _SegEPort,_SegEBit
		 bcf _SegFPort,_SegFBit
		 bcf _SegGPort,_SegGBit
		 ifdef _SegDPPort
		  bcf _SegDPPort,_SegDPBit
		 endif
		endif
		if _nDigit>0
		 bcf _Digit1Port,_Digit1Bit
		endif
		if _nDigit>1
		 bcf _Digit2Port,_Digit2Bit
		endif
		if _nDigit>2
		 bcf _Digit3Port,_Digit3Bit
		endif
		if _nDigit>3
		 bcf _Digit4Port,_Digit4Bit
		endif
		if _nDigit>4
		 bcf _Digit5Port,_Digit5Bit
		endif
		if _nDigit>5
		 bcf _Digit6Port,_Digit6Bit
		endif
		if _nDigit>6
		 bcf _Digit7Port,_Digit7Bit
		endif
		if _nDigit>7
		 bcf _Digit8Port,_Digit8Bit
		endif
		MCLEARRP TRISA

		clrf cDigit
SetPS7		movlw _Prescale7Seg
		movwf SvSegPs
SvSegRet	MRET 0

;
; Overflow timer 0 - call here
;

SvSegTime::
		decfsz SvSegPs
		goto SvSegRet
		ifdef _ChgDigUsed
		 bsf _ChgDigFlag,_ChgDigFlagBit
		endif
		incf cDigit
		movlw _nDigit
		subwf cDigit,w
		skpnz
		clrf cDigit
		ifdef _ChgFrameUsed
		 skpnz
		 bsf _ChgFrameFlag,_ChgFrameFlagBit
		endif
		movlw Pat7
		addwf cDigit,w
		movwf FSR		; FSR holds current _Digit
		MSETIRP Pat7
		clrf Temp
		incf cDigit,w
		movwf Flags
		setc
gbl		rlf Temp		; Get bit for _Digit in Temp
		decfsz Flags
		goto gbl
		call SvSegAllOff
		call SvSegSetBars
		call SvSegSetDig
		goto SetPS7
		
SvSegAllOff:
	       if _CommonDrive==1
		if _nDigit>0
		 bcf _Digit1Port,_Digit1Bit
		endif
		if _nDigit>1
		 bcf _Digit2Port,_Digit2Bit
		endif
		if _nDigit>2
		 bcf _Digit3Port,_Digit3Bit
		endif
		if _nDigit>3
		 bcf _Digit4Port,_Digit4Bit
		endif
		if _nDigit>4
		 bcf _Digit5Port,_Digit5Bit
		endif
		if _nDigit>5
		 bcf _Digit6Port,_Digit6Bit
		endif
		if _nDigit>6
		 bcf _Digit7Port,_Digit7Bit
		endif
		if _nDigit>7
		 bcf _Digit8Port,_Digit8Bit
		endif
	      else
		if _nDigit>0
		 bsf _Digit1Port,_Digit1Bit
		endif
		if _nDigit>1
		 bsf _Digit2Port,_Digit2Bit
		endif
		if _nDigit>2
		 bsf _Digit3Port,_Digit3Bit
		endif
		if _nDigit>3
		 bsf _Digit4Port,_Digit4Bit
		endif
		if _nDigit>4
		 bsf _Digit5Port,_Digit5Bit
		endif
		if _nDigit>5
		 bsf _Digit6Port,_Digit6Bit
		endif
		if _nDigit>6
		 bsf _Digit7Port,_Digit7Bit
		endif
		if _nDigit>7
		 bsf _Digit8Port,_Digit8Bit
		endif
	      endif
		return

SvSegSetBars:
	       if _CommonDrive==1
		bcf _SegAPort,_SegABit
		bcf _SegBPort,_SegBBit
		bcf _SegCPort,_SegCBit
		bcf _SegDPort,_SegDBit
		bcf _SegEPort,_SegEBit
		bcf _SegFPort,_SegFBit
		bcf _SegGPort,_SegGBit
		ifdef _SegDPPort
		 bcf _SegDPPort,_SegDPBit
		endif
		btfsc 0,0
		bsf _SegAPort,_SegABit
		btfsc 0,1
		bsf _SegBPort,_SegBBit
		btfsc 0,2
		bsf _SegCPort,_SegCBit
		btfsc 0,3
		bsf _SegDPort,_SegDBit
		btfsc 0,4
		bsf _SegEPort,_SegEBit
		btfsc 0,5
		bsf _SegFPort,_SegFBit
		btfsc 0,6
		bsf _SegGPort,_SegGBit
		ifdef _SegDPPort
		 btfsc 0,7
		 bsf _SegDPPort,_SegDPBit
		endif
	       else
		bsf _SegAPort,_SegABit
		bsf _SegBPort,_SegBBit
		bsf _SegCPort,_SegCBit
		bsf _SegDPort,_SegDBit
		bsf _SegEPort,_SegEBit
		bsf _SegFPort,_SegFBit
		bsf _SegGPort,_SegGBit
		ifdef _SegDPPort
		 bsf _SegDPPort,_SegDPBit
		endif
		btfsc 0,0
		bcf _SegAPort,_SegABit
		btfsc 0,1
		bcf _SegBPort,_SegBBit
		btfsc 0,2
		bcf _SegCPort,_SegCBit
		btfsc 0,3
		bcf _SegDPort,_SegDBit
		btfsc 0,4
		bcf _SegEPort,_SegEBit
		btfsc 0,5
		bcf _SegFPort,_SegFBit
		btfsc 0,6
		bcf _SegGPort,_SegGBit
		ifdef _SegDPPort
		 btfsc 0,7
		 bcf _SegDPPort,_SegDPBit
		endif
	       endif
	        return

SvSegSetDig:
	       if _CommonDrive==1
		if _nDigit>0
		 btfsc Temp,0
		 bsf _Digit1Port,_Digit1Bit
		endif
		if _nDigit>1
		 btfsc Temp,1
		 bsf _Digit2Port,_Digit2Bit
		endif
		if _nDigit>2
		 btfsc Temp,2
		 bsf _Digit3Port,_Digit3Bit
		endif
		if _nDigit>3
		 btfsc Temp,3
		 bsf _Digit4Port,_Digit4Bit
		endif
		if _nDigit>4
		 btfsc Temp,4
		 bsf _Digit5Port,_Digit5Bit
		endif
		if _nDigit>5
		 btfsc Temp,5
		 bsf _Digit6Port,_Digit6Bit
		endif
		if _nDigit>6
		 btfsc Temp,6
		 bsf _Digit7Port,_Digit7Bit
		endif
		if _nDigit>7
		 btfsc Temp,7
		 bsf _Digit8Port,_Digit8Bit
		endif
	      else		; Common anode
		if _nDigit>0
		 btfsc Temp,0
		 bcf _Digit1Port,_Digit1Bit
		endif
		if _nDigit>1
		 btfsc Temp,1
		 bcf _Digit2Port,_Digit2Bit
		endif
		if _nDigit>2
		 btfsc Temp,2
		 bcf _Digit3Port,_Digit3Bit
		endif
		if _nDigit>3
		 btfsc Temp,3
		 bcf _Digit4Port,_Digit4Bit
		endif
		if _nDigit>4
		 btfsc Temp,4
		 bcf _Digit5Port,_Digit5Bit
		endif
		if _nDigit>5
		 btfsc Temp,5
		 bcf _Digit6Port,_Digit6Bit
		endif
		if _nDigit>6
		 btfsc Temp,6
		 bcf _Digit7Port,_Digit7Bit
		endif
		if _nDigit>7
		 btfsc Temp,7
		 bcf _Digit8Port,_Digit8Bit
		endif
	      endif
	        return
	       endmodule
#pragma asmend
#endif
#ifdef SvSegPatUsed
#pragma asm
		module "SevSeg"
;
; Put a number in 7 segment form to a digit in the Pat7 array
GetPat7::

		SETPCLATH 0,GetPat7
	if _QUICKCALL==1
		MGETFSRSPO 1		; FSR points to Value
	else
		MGETFSRSPO 3		; FSR points to Value
	endif
		movfw 0
		movwf Temp		; Value to Flags
		
		incf FSR
		movfw 0
		addlw Pat7
		movwf FSR		; Digit number to FSR
		clrf 0			; Default value is to clear
		movlw D'10'		; Check Digit between 0 and 9
		subwf Temp,w
		MSETIRP Pat7
		SETPCLATH Get7Call,0
		skpc
		call Get7Call
		skpc
		movwf 0
		SETPCLATH 0,GetPat7
		MRET 0
		
	
Get7Call	movlw Pats7>>8
		movwf PCLATH
		movlw Pats7
		addwf Temp,w
		skpnc
		incf PCLATH
		movwf PCL
;
;			GFEDCBA
Pats7		retlw b'0111111'	; 0
		retlw b'0000110'	; 1
		retlw b'1011011'	; 2
		retlw b'1001111'	; 3
		retlw b'1100110'	; 4
		retlw b'1101101'	; 5
		retlw b'1111101'	; 6
		retlw b'0000111'	; 7
		retlw b'1111111'	; 8
		retlw b'1101111'	; 9
		endmodule
#pragma asmend
#endif
#ifdef PWM1Used
#pragma asm
		module "PWM1"
;
; FSR points to a 2 byte variable in low/high form which is 10 bits in length
; this value is written to set the voltage for PWM1
;
SetPWM1Volts::
		SETPCLATH 0,SetPWM1Volts
	if _QUICKCALL==1
		MGETFSRSPO 1		; FSR points to Value
	else
		MGETFSRSPO 3		; FSR points to Value
	endif
		bcf CCP1CON,4
		bcf CCP1CON,5
		btfsc 0,0
		bsf CCP1CON,4
		btfsc 0,1
		bsf CCP1CON,5
		movfw 0
		movwf Temp
		incf FSR
		movfw 0
		movwf Flags
		rrf Flags
		rrf Temp
		rrf Flags
		rrf Temp,w
		movwf CCPR1L
		MRET 0
		endmodule
#pragma asmend
#endif

		








                                                        
