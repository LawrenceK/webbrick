//
//   
//  Dallas 1 wire - multiple temperatir sensors.
//   
//  Lawrence Klyne. Jun 2005
// 
//	These routines are to manage the Dallas 1 wire interface.
//	The fed 1 wire code provides simple send and receive byte code
//	only, this layers on top of these to do discovery etc.
//

#define MAX_SENSORS	5
// The family code for an 1820 sensor
#define DS1W_1820_FAMILY	0x28

#define INVALID_TEMP	0x7FFF
// make sure first 5 bits are clear.
#define NO_TEMP_SENSORS	0x80

// bus status
// less than 0, error state, no sensors.
unsigned char OWStatus();

// read the status and value of a temp sensor.
// These index are from 0 to MAX_SENSORS - 1
int OWTmp( unsigned idx );	// degrees*16 + decimal

// read all sensors and initiate a new temperature conversion.
void OWReadSensors( unsigned char chn );
void OWReadSensorsOnly( unsigned char chn );
void OWStartTempConversion( unsigned char chn );
void OWStartTempConversionAll();
// Do a address search.
void OWIdentifySensors();
// These two are for when only a single DS1820 on the bus.
int OWReadAddress( char *buf );	// helper reads a single address, must only be a single sensor on line.
int OWReadSingle();			// read a single DS1820.
void OWDeTag(unsigned char chn);		// erase the tags in sensor 'chn'
void OWDeTagAll();			// erase the tags in all attached sensors

#ifdef DEBUG_OW
extern unsigned char OWRdData[9];	// working buffer
#endif

