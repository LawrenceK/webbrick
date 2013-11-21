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

#ifndef _DMX_H
#define _DMX_H

#define MAX_DMX_CHANNELS    8

//void DMXSetScene();     // loads OpStr into DMXSet
//void DMXFade();		// Do a DMX Fade
//void SendDMX( unsigned char *cmd, unsigned char cmdLen );   // send a DMX or generic serial blob

void DmxSetLevel( unsigned char chn, unsigned char level );
// call every millisecond
void DmxCheck( );
void DmxStart( );
void DmxStop( );

#endif
