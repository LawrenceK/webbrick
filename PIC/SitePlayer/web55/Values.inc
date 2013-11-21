// Misc
OpState = 1;
LSt = 2;
CmdError = 0;
//CmdError = 499;

// Time
year = 2005;
month = 01;
date = 01;
day = 01;
hour = 12;
minute = 59;
second = 30;

// This is stuff that is configured and infrequently changes.
RotStep = 4;
FadeRate = 4;
NodeNumber = 253;
NodeName = "TestData";

// Infra Red
infrared = 0x4F;

// Serial 
SerialFlags = 0x86;

// Mimics 
MimicFlags = 0x80;

// Monitors
DIs = 0xAAA;
// DigOuts
DOs = 0x55;

// Temperatures/OneWire
OWSts = 0x0F;   // 4 sensors
TmV = new Array ( (21.3125*16), (22.5*16), (23.5*16), -(24.5*16), (25.5*16) ) ;

// Analogue Outs
AOv = new Array (25, 50, 0, 0 ) ;
// Analogue In
AIv = new Array ( 75, 0, 0, 0 );
// Dwells
DwV = new Array(2, 60, 300, 3600, 300, 600, 900, 1200 );

// SetPoints
SpV = new Array ( 0, 14, 30, 45, 60, 75, 90, 100, 0, 15, 30, 45, 60, 75, 90, 100 ) ;

// Trigger Configurations
// 4 Dwells, 
DiB1 = new Array( 0x80, 0x89, 0xF2, 0x1B, 0x81, 0x82, 0x83, 0x84, 0xE5, 0xEE, 0x87, 0xFF ); 
DiB2 = new Array( 0, 1, 0x41, 0x42, 0x91, 0x92, 0xA3, 0xA4, 0, 0, 0, 0 );
DiB3 = new Array( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ); 
DiB4 = new Array( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ); 
DiB5 = new Array( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ); 

// Names
//MNm = new Array("Burglar", "Fire", "Salt", "Flood" ) ;
AONm = new Array ("Fan", "Dimmer", "Lghts Up", "Lghts Dn") ;
AINm = new Array ("WaterLevel", "Salt", "Wind Speed", "Wind Dir") ;
DINm = new Array ("Light1", "Light2", "Light3", "Floor", "Heating", "Front", "Stairs", "Garage", "Burglar", "Fire", "Salt", "Flood", "ROTARY_A", "ROTARY_B") ;
DONm = new Array ("Boiler", "Floor", "Lights", "Door", "GDoor", "Stereo", "Patio", "Spare" ) ;
TNm = new Array ("Zone1", "Zone2", "Zone3", "External", "Attic") ;

//  Stuff that is used for advanced diagnostics.
SwVersion = "TestWeb";
SerialNr="010203040506";
// My IP Address
IP0=255;
IP1=255;
IP2=255;
IP3=255;
// UDP Details
UDPIP=255;
UDPIP2=255;
UDPIP3=255;
UDPIP4=255;
UDPPORT=255;
UDPADDR=255;
UDPCOUNT=16;
UDPRcvr=1;
