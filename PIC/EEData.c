//
//  Copyright L.P.Klyne 2013
//
//  Filename: dmx.h
//
//  Description:    DMX protocol functionality
//
//  Notes:
//
//
// Include files

// Generated to test EEPROM DAta memory will set to zero.
// NOTE bug in WIZASM de assumes even number of bytes.

#asm
module "EEData" forced absolute 0xf00000
; 0-127
; IP address
 de 255,255,255,255
; 0x04 View password up to 12 characters
 de 0,0,0,0,0,0,0,0,0,0,0,0
; 0x10 Admin password up to 12 characters
 de 'p','a','s','s'
 de 'w','o','r','d', 0, 0, 0, 0
; 0x1C Extra password up to 12 characters
 de 'i','n','s','t','a','l','l','e','r',0,0,0

; Options
; address 0x28 Private option 0 Z0
; UDP options default all on
 de 0, 0xFF
; OPT_AnInOptions
; OPT_AnOutOptions     3
de 0x1, 0x0
; OPT_DigInOptions     4
; OPT_DigOutOptions    5
de 0x0, 0x0
; OPT_TempOptions      6
; spare options
de 0x0, 0x0
; marker
de 0xFF, 0xFF
; address 0x30 
; available up to 0x5E, extended UDP buffer
#asmend
/*
; Rest is peristant user configuration.
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
; 128-255
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
; 256
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
; 
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
; 512
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
; 
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
; 768
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
; 
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
 de 0,0,0,0,0,0,0,0
endmodule
*/

