// Temperatures/OneWire
TCLo = new Array( -1.5*16, 2.5*16, 3.5*16, 4.5*16, 5.5*16 );
TDLo = new Array( -1.0*16, 2.5*16, 3.5*16, 4.5*16, 5.5*16 );
TLoB1 = new Array( 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
TLoB2 = new Array( 0, 1, 2, 3, 4 );
TLoB3 = new Array( 0, 0, 0, 0, 0 ); 
TLoB4 = new Array( 0, 0, 0, 0, 0 ); 
TCHi = new Array( 81.5*16, 82.5*16, 83.5*16, 84.5*16, 85.5*16 );
TDHi = new Array( 81.0*16, 82.5*16, 83.5*16, 84.5*16, 85.5*16 );
THiB1 = new Array( 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
THiB2 = new Array( 0, 1, 2, 3, 4 );
THiB3 = new Array( 0, 0, 0, 0, 0 ); 
THiB4 = new Array( 0, 0, 0, 0, 0 ); 

//Analogue In
AICLo = new Array( 10, 20, 30, 40 );
AIDLo = new Array( 15, 20, 30, 45 );
AILoB1 = new Array( 0xA1, 0xA2, 0xA3, 0xA4 );
AILoB2 = new Array( 1, 2, 3, 4 );
AILoB3 = new Array( 0, 0, 0, 0 );
AILoB4 = new Array( 0, 0, 0, 0 );
AICHi = new Array( 70, 80, 90, 100 );
AIDHi = new Array( 70, 85, 90, 90 );
AIHiB1 = new Array( 0xA1, 0xA2, 0xA3, 0xA4 );
AIHiB2 = new Array( 1, 2, 3, 4 );
AIHiB3 = new Array( 0, 0, 0, 0 );
AIHiB4 = new Array( 0, 0, 0, 0 );

// Schedules
SchD = new Array( 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x41, 0x3E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0 );
SchH = new Array( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
SchM = new Array( 00, 10, 20, 30, 40, 50, 00, 10, 20, 30, 40, 50, 00, 10, 20, 30 );
SchB1 = new Array( 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41 ); 
SchB2 = new Array( 0, 1, 2, 3, 4, 5, 6, 7, 0x80, 0x81, 0x82, 0x83, 0x44, 0x45, 0x46, 0x47 );
SchB3 = new Array( 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7 );
SchB4 = new Array( 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7 );

//Scenes
ScDM = new Array( 0x0F, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0 );
ScDS = new Array( 0x05, 0xA0, 0x05, 0xA0, 0x05, 0xA0, 0x05, 0xA0, 0x05, 0xA0, 0x05, 0xA0 );
ScAM = new Array( 0x05, 0x0A, 0x05, 0x0A, 0x05, 0x0A, 0x05, 0x0A, 0x05, 0x0A, 0x05, 0x0A );
ScAS = new Array( 0x0307, 0x1050, 0x0307, 0x1050, 0x0307, 0x1050, 0x0307, 0x1050, 0x0307, 0x1050, 0x0307, 0x1050 );
