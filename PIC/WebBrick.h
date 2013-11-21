//
//  Copyright L.P.Klyne 2013
//
//  Filename: Webrick.h
//
//  Description: Web Brick functions.
//
//  Notes:
//
//
// Include files

//#include "template.h"

#ifndef _WEBBRICK_H
#define _WEBBRICK_H

extern unsigned char doAlert;
// Global functions
void Do1SecondActivities();                                     // what to do each near second

void SendStUDP() ;						// send startup UDP  packet
void UpdateDSts() ;						// update SP value of PORTD

#endif
