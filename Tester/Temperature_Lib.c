#include "D:\\o2m8\\svn\\WebBrick\\Trunk\\Tester\\Temperature_Auto.h"


// *** Input File - D:\o2m8\svn\WebBrick\Trunk\PIXIE\APPWIZ\misc.c

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
#ifdef HMSUsed
#ifdef UsePS
 const int HMS_T0OF=256*PSD;		// Overflow time for timer 0
#else 
 const int HMS_T0OF=D'256';
#endif
#pragma asm
		module "HMSInit"
;
; Hours,Minutes & Seconds
;

		SECCOUNTS=(_APROCFREQ/4)/_HMS_T0OF;
		MINCOUNTS=(_APROCFREQ*D'15')/_HMS_T0OF;
		HOURCOUNTS=((_APROCFREQ*D'30')/_HMS_T0OF)*D'30';
		
		; MFORCEPAGE D'100'
HMSInit::
		#ifdef HourPassUsed
		 call HourInit
		#endif
		#ifdef MinPassUsed
		 call MinInit
		#endif
		#ifdef SecPassUsed
		 call SecInit
		#endif
		MRET 0
		
ifdef _HourPassUsed
HourInit:	clrf HrCount
		clrf HrCount1
		clrf HrCount2
		clrf HrCount3
		return
endif
ifdef _MinPassUsed
MinInit:	clrf MinCount
		clrf MinCount1
		clrf MinCount2
		return
endif
ifdef _SecPassUsed
SecInit:	clrf SecCount
		clrf SecCount1
		return
endif
;
; Called every timer 0 overflow
;
HMSTick::
 ifdef _SecPassUsed
 		incf SecCount1
 		incfsz SecCount
 		decf SecCount1
HMS_NIS:
		movlw SECCOUNTS>>0
		subwf SecCount,w
		skpz
		goto HMS_NoOccurS
		movlw SECCOUNTS>>8
		subwf SecCount1,w
		skpz
		goto HMS_NoOccurS
HMS_SecPassed:	bsf _SecPassFlag,_SecPassFlagBit
		call SecInit
HMS_NoOccurS:
 endif
 ifdef _MinPassUsed
 		incfsz MinCount
 		goto HMS_NIM
 		incfsz MinCount1
 		goto HMS_NIM
 		incf MinCount+2
HMS_NIM:
		movlw MINCOUNTS>>0
		subwf MinCount,w
		skpz
		goto HMS_NoOccurM
		movlw MINCOUNTS>>8
		subwf MinCount1,w
		skpz
		goto HMS_NoOccurM
		movlw MINCOUNTS>>D'16'
		subwf MinCount2,w
		skpz
		goto HMS_NoOccurM
		bsf _MinPassFlag,_MinPassFlagBit
		call MinInit
	ifdef SecPassUsed
		call SecInit
	endif
HMS_NoOccurM:
 endif
 ifdef _HourPassUsed
 		incfsz HrCount
 		goto HMS_NIH
 		incfsz HrCount1
 		goto HMS_NIH
 		incfsz HrCount2
 		goto HMS_NIH
 		incf HrCount3
HMS_NIH:
		movlw HOURCOUNTS>>0
		subwf HrCount,w
		skpz
		goto HMS_NoOccurH
		movlw HOURCOUNTS>>8
		subwf HrCount1,w
		skpz
		goto HMS_NoOccurH
		movlw HOURCOUNTS>>D'16'
		subwf HrCount2,w
		skpz
		goto HMS_NoOccurH
		movlw HOURCOUNTS>>D'24'
		subwf HrCount3,w
		skpz
		goto HMS_NoOccurH
		bsf _HourPassFlag,_HourPassFlagBit
		goto HMSInit
HMS_NoOccurH:
 endif
HMS_End:
		MRET 0
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

		








                                                        
