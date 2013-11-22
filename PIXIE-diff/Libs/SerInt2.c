#include <pic.h>
#include <datalib.h>
#include <SerInt2.h>

unsigned char TxTab2[TXBUFSZ2];
unsigned char RxTab2[RXBUFSZ2];

#ifdef _SerIntInit2
#asmdefine _GetSPOtoFSR0
//********************************************************************
//
// Asynchronous TX and RX handlers
//
//********************************************************************

#ifdef _AddTx2
 #define _WaitRx2
#endif

#ifdef _WaitRx2
 #define _GetRxSize2
#endif

 const int SerVarSpace2=7;	  // SPace used for serial variables
 unsigned char register SerVars2[SerVarSpace2]; // Vars in common page

// TODO use common isfsrsave across all interrupt handlers
// Fix location of buffers to simplify buffer manipulation/address calculation see mimic interrupt handler.
 #pragma asm
 module "SerVarAsm" global
   isfsrsaveL2 = SerVars2+0 ; // Saves FSR
   isfsrsaveH2 = SerVars2+1 ; // Saves FSR
   sertemp2 = SerVars2+2;   // Temporary variable, may be used elsewhere
   rxhead2 = SerVars2+3;    // Pointer to head of receive buffer
   rxtail2 = SerVars2+4;    // Pointer to tail of receive buffer
   txhead2 = SerVars2+5;    // Pointer to head of transmit buffer
   txtail2 = SerVars2+6;    // Pointer to head of transmit buffer
 endmodule
 #pragma asmend
#endif

#ifdef _GetRxSize2
#pragma asm
module "_GetRxSize2"
; Return 0 size of receive buffer 
; Set zero flag if buffer is empty (i.e. no characters)
; Return 0 size of receive buffer in W

GetRxSize2::
		movlb rxhead2>>8
		movf rxhead2,w,BANKED	; Calculate size of rx buffer
                subwf rxtail2,w,BANKED
                clrf WREG
                btfsc STATUS,C
                movlw _RXBUFSZ2
                addwf rxhead2,w,BANKED
                movwf sertemp2,BANKED
                movf rxtail2,w,BANKED
                subwf sertemp2,w,BANKED
                andlw _RXBUFSZ2-1
                movwf ACC
                MRET 0
endmodule
#pragma asmend
#endif

#ifdef _GetTxSize2
#pragma asm
module "_GetTxSize2"

		
; Return 0 size of transmit buffer 
; Set zero flag if buffer is empty (i.e. no characters)
; Return 0 size of receive buffer in W

GetTxSize2::
		movlb txhead2>>8
		movf txhead2,w,BANKED
                subwf txtail2,w,BANKED
                clrf WREG
                btfsc STATUS,C
                movlw _TXBUFSZ2
                addwf txhead2,w,BANKED
                movwf sertemp2,BANKED
                movf txtail2,w,BANKED
                subwf sertemp2,w,BANKED
                andlw _TXBUFSZ2-1
                movwf ACC
		MRET 0
endmodule
#pragma asmend
#endif

#ifdef _AddTx2

#pragma asm
module "_AddTx2"

;
; Add byte in W to the transmit list, if no space in the transmit buffer 
; then wait for space to become available as characters are transmitted
;

AddTx2::
		
	if _QUICKCALL==1
		MGETFSRSPO 1		; FSR points to Count
	else
		MGETFSRSPO 3		; FSR points to Count
	endif
		movf INDF0,w
		movlb rxhead2>>8
		movwf sertemp2,BANKED
waitsp2          incf txhead2,w,BANKED   ; Wait for space to be available
                andlw _TXBUFSZ2-1
                subwf txtail2,w,BANKED
                btfsc STATUS,Z
                bra waitsp2
                
                movlw TxTab2>>8
                movwf FSR0H
                movf txhead2,w,BANKED    ;  Find the address of the head
                addlw TxTab2
                movwf FSR0L
                btfsc STATUS,C
                incf FSR0H
                movf sertemp2,w,BANKED
                movwf INDF0
                incf txhead2,f,BANKED
                movlw _TXBUFSZ2-1
                andwf txhead2,f,BANKED
                bsf PIE3,TX2IE  ; enable interrupts
                MRET 0 
endmodule
#pragma asmend
#endif

#ifdef _ResendTx2

#pragma asm
module "_ResendTx2"
; Note this makes assumptions, these are that the data to be resent starts at the 
; begining of the Tx buffer and that txTail is the end of the data.
; Purpose is to make DMX work better by being able to just resend buffer contents.
;
ResendTx2::
                bcf PIE3,TX2IE  ; enable interrupts
		movlb txhead2>>8
		clrf txtail2,BANKED   ;  clear pointer
                incf txtail2,f,BANKED   ; move on

                ; get first byte and send.
                LFSR FSR0, TxTab2
                movf INDF0,w
                movwf TXREG2
                
                bsf PIE3,TX2IE  ; enable interrupts
                MRET 0 
endmodule
#pragma asmend
#endif

#ifdef _FlushTx2

#pragma asm
module "_FlushTx2"
; Note this makes assumptions, these are that the data to be resent starts at the 
; begining of the Tx buffer and that txTail is the end of the data.
; Purpose is to make DMX work better by being able to just resend buffer contents.
;

FlushTx2::
		movlb txhead2>>8
		clrf txhead2,BANKED   ;  clear pointer
		clrf txtail2,BANKED
		MRET 0 
endmodule
#pragma asmend
#endif

#ifdef _WaitRx2
#pragma asm
module "_WaitRx2"
;
; Get next byte from receive buffer, wait if none there yet
; return 0 the result in the W register
;

WaitRx2::
                MCALL GetRxSize2
		movf ACC,w
		btfsc STATUS,Z
                bra WaitRx2
                
		movlb rxhead2>>8
		movlw RxTab2>>8
		movwf FSR0H
                movf rxtail2,w,BANKED
                addlw RxTab2     	; Move address of byte to FSR
                movwf FSR0L
                btfsc STATUS,C
                incf FSR0H
                incf rxtail2,f,BANKED
                movlw _RXBUFSZ2-1
                andwf rxtail2,f,BANKED
                movlw _RXBUFSZ2/8
                subwf ACC
		movlb rxhead2>>8
                movf INDF0,w
                movwf ACC
                MRET 0

endmodule
#pragma asmend
#endif

#ifdef _SerIntHandler2
#pragma asm
module "_SerIntHandler2"
;********************************************************************
;
; Interrupt handler
;
; Add to this code for other interrupt sources - note can only be called
; from interrupts
;
;********************************************************************

SerIntHandler2::
                btfsc PIR3,RC2IF         ; Check rx flag
                bra asrx2
                btfsc PIR3,TX2IF
                bra astx2
                bra SIRR2
bufempt2        bcf PIE3,TX2IE
		ifdef _TxFree2Used
		    bsf _TxFree2Flag,_TxFree2FlagBit		; Set flag
		endif
SIRet2          movlb txhead2>>8
		movf isfsrsaveL2,w,BANKED
		movwf FSR0L
		movf isfsrsaveH2,w,BANKED
		movwf FSR0H
SIRR2		return 0
;
; Transmit interrupt handler
;
astx2
		movlb txhead2>>8
		movf FSR0L,w
		movwf isfsrsaveL2,BANKED
		movf FSR0H,w
		movwf isfsrsaveH2,BANKED

                movf txhead2,w,BANKED
                subwf txtail2,w,BANKED          ; Test if buffer empty
                btfsc STATUS,Z
                bra bufempt2
                movlw TxTab2>>8
                movwf FSR0H
                movf txtail2,w,BANKED            ; Now move in tail of buffer
                addlw TxTab2
                movwf FSR0L
                btfsc STATUS,C
                incf FSR0H
                incf txtail2,f,BANKED
                movlw _TXBUFSZ2-1         ; Chop off the upper bits
                andwf txtail2,f,BANKED
                movf INDF0,w                 ; Send this byte now
                movwf TXREG2
	 	bra SIRet2
;
; Receive Interrupt Handler
;
asrx2           movlb txhead2>>8
		movf FSR0L,w
		movwf isfsrsaveL2,BANKED
		movf FSR0H,w
		movwf isfsrsaveH2,BANKED
		movlw RxTab2>>8
		movwf FSR0H
		movf rxhead2,w,BANKED
                addlw RxTab2
                movwf FSR0L            ; FSR holds address to put incoming byte
                btfsc STATUS,C
                incf FSR0H
                movf RCREG2,w             ; Get and save the incoming byte
                movwf INDF0

		ifdef _RxByte2Used
    		    bsf _RxByte2Flag,_RxByte2FlagBit
		endif

		incf rxhead2,w,BANKED           ; Check there is room in rxbuf
                andlw _RXBUFSZ2-1
                subwf rxtail2,w,BANKED
                btfsc STATUS,Z
                bra SIRet2
                incf rxhead2,f,BANKED             ; Move up head
                movlw _RXBUFSZ2-1
                andwf rxhead2,f,BANKED
                movf rxhead2,w,BANKED
                subwf rxtail2,w,BANKED
                movlw _RXBUFSZ2           ; If head<tail then add buffer size to head
                btfss STATUS,C
                clrf WREG
                addwf rxhead2,w,BANKED    ; Now get size of buffer to w
       		bra SIRet2

endmodule
#pragma asmend
#endif
#ifdef _SerIntInit2
#pragma asm
module "_SerIntInit2"

SPHIGH2                 equ ((_APROCFREQ*.1)/(_SERINTRATE2*.4))-1

SerIntInit2::
;	bcf INTCON,GIE
	movlb txhead2>>8
        clrf txhead2,BANKED		; Clear all buffers
        clrf txtail2,BANKED
        clrf rxhead2,BANKED
        clrf rxtail2,BANKED

; Now set the ports and tris registers up
	bsf LATG,1		; Start driving serial bit high
        bcf TRISG,1
        bsf TRISG,2

; Now the peripheral registers
	
	bsf INTCON,PEIE
        movlw (1<<SPEN)+(1<<SREN)+(1<<CREN) ; Enable Receive port
        movwf RCSTA2
        bsf PIE3,RC2IE
        bsf PIE3,TX2IE
        movlw SPHIGH2>>8          	 ; Set baud rate high byte
        movwf SPBRGH2
        movlw SPHIGH2          	 ; Set baud rate low byte
        movwf SPBRG2
;        bsf TXEN,TXSTA2 ; Enable TX
        movlw (1<<TXEN)+(1<<BRGH)+(1<<TX9)+(1<<TX9D)      ; Enable TX and High baud select bit, 2 stop bits
        movwf TXSTA2

;
; Final action before starting is to enable interrupts
;
;    if _USEAD==0
;        bsf INTCON,GIE          ; Enable interrupts
;    endif
        MRET 0
endmodule
#pragma asmend
#endif

#ifdef _Serial2Enable
#pragma asm
module "_Serial2Enable"
Serial2Enable::
        movlw (1<<SPEN)+(1<<SREN)+(1<<CREN) ; Enable Receive port
        movwf RCSTA2
        movlw (1<<TXEN)+(1<<BRGH)+(1<<TX9)+(1<<TX9D)      ; Enable TX and High baud select bit, 2 stop bits
        movwf TXSTA2
	MRET 0 
endmodule
#pragma asmend
#endif

#ifdef _Serial2Disable
#pragma asm
module "_Serial2Disable"
Serial2Disable::
        bcf RCSTA2,SPEN
	MRET 0 
endmodule
#pragma asmend
#endif

#ifdef _DmxMBB
#pragma asm
module "_DmxMBB"
DmxMBB::
        bcf RCSTA2,SPEN ; disable EUSART
        bsf LATG, 1     ; set high
        DELAY 100    ; 10 uS at 40Mhz.
	MRET 0 
endmodule
#pragma asmend
#endif

#ifdef _DmxMAB
#pragma asm
module "_DmxMAB"
DmxMAB::
        bsf LATG, 1     ; set high
        DELAY 100    ; 10 uS at 40Mhz.
        movlw (1<<SPEN)+(1<<SREN)+(1<<CREN) ; Enable Receive port
        movwf RCSTA2
        movlw (1<<TXEN)+(1<<BRGH)+(1<<TX9)+(1<<TX9D)      ; Enable TX and High baud select bit, 2 stop bits
        movwf TXSTA2
	MRET 0 
endmodule
#pragma asmend
#endif

#ifdef _DmxB
#pragma asm
module "_DmxB"
DmxB::
        bcf LATG, 1     ; set low
	MRET 0 
endmodule
#pragma asmend
#endif

#ifdef _DmxBreak
#pragma asm
module "_DmxBreak"
DmxBreak::
        bcf RCSTA2,SPEN ; disable EUSART
        bcf LATG, 1     ; set low
        DELAY 1000    ; 100 uS at 40Mhz.
        bsf LATG, 1     ; set high
        DELAY 100    ; 10 uS at 40Mhz.
        movlw (1<<SPEN)+(1<<SREN)+(1<<CREN) ; Enable Receive port
        movwf RCSTA2
        movlw (1<<TXEN)+(1<<BRGH)+(1<<TX9)+(1<<TX9D)      ; Enable TX and High baud select bit, 2 stop bits
        movwf TXSTA2
	MRET 0 
endmodule
#pragma asmend
#endif

#ifdef _SetBaud2
void SetBaud2(unsigned baud)
{
    SPBRGH2 = baud >> 8;
    SPBRG2 = baud;
}
#endif

//#ifdef _SetTxByte2
//void SetTxByte2(unsigned char idx, unsigned char val)
//{
//    TxTab2[idx] = val;
//}
//#endif
