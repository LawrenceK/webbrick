//#define DEBUG_OW	1
#define DO_DEBUG_TRACE	1
#define NULL_TAG	0x0F

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
// Include files
#include "Webrick_Auto.h"

//
#ifdef DEBUG_OW
#define OneWTrace( id, data, len )  DebugB( id, data, len )
#else
#define OneWTrace( id, data, len )
#endif

// allow MAX_READ_ERROR failed reads before marking sensor offline
#define MAX_READ_ERROR 5

//*****************************************************************
// Local Data
//
// array of received addresses, we will not store the device type and CRC as yet.
//unsigned char OWCount = MAX_SENSORS;	// Number of devices on one wire bus.
unsigned char OWAddresses[8*MAX_SENSORS]
#if 0
    = {
    #ifndef TEST	
	0x28,0x3E,0xC1,0x9B,0x0,0x0,0x0,0x7A	// tippex/one
	,0x28,0xAC,0xBA,0x9B,0x0,0x0,0x0,0xB8	// tippex/two
	,0x28,0x11,0x8C,0x9B,0x0,0x0,0x0,0xED	// tippex/three
	,0x28,0xA3,0xB2,0x9B,0x0,0x0,0x0,0xA2	// tippex/four
	,0x28,0xD,0x56,0x9B,0x0,0x0,0x0,0x28	// tippex/red
    #else	
	0x28,0xA2,0xB1,0x9B,0x0,0x0,0x0,0xDB // Red
	,0x28,0xDB,0xCA,0x9B,0x0,0x0,0x0,0xC8 // Orange
	,0x28,0xED,0x57,0x9B,0x0,0x0,0x0,0xCC	// Blue
	,0x28,0x7E,0xC5,0x9B,0x0,0x0,0x0,0x10	// tippex/blue
	,0x28,0x59,0xB9,0x9B,0x0,0x0,0x0,0x6F	// tippex/green
    #endif	
    }
#endif	
;	// end of OWAddresses

static unsigned char OWRdBuf[9];	// working buffer

//static int OWPres = -1;				// is anything present
static unsigned int OWSts = 0;   // No sensors
static int OWRes[MAX_SENSORS] = {INVALID_TEMP,INVALID_TEMP,INVALID_TEMP,INVALID_TEMP,INVALID_TEMP};
static int OWErrCnt[MAX_SENSORS] = {0,0,0,0,0};

//*****************************************************************
// Forward Defines.
//
int  OWFirst();
int  OWNext();
//int  OWVerify();
//void OWTargetSetup(unsigned char family_code);
//void OWFamilySkipSetup();
int  OWReset();
int  OWSearch();

void InitCrc8();
//void Initcrc8(unsigned char value);
unsigned char UpdateCrc8(unsigned char value);

int OWWriteAddress( unsigned char *adr );
int OWReadData( );
int OWReadSensor( unsigned char *adr );
int OWWriteTag( unsigned char *adr, unsigned char tag );

//*****************************************************************
//
// Private data, mainly for search
unsigned char ROM_NO[8];
static int LastDiscrepancy;
static int LastFamilyDiscrepancy;
static int LastDeviceFlag;
static unsigned char crc8 = 0;	// working value.
static int id_bit_number;
static int last_zero;
static int rom_byte_number;
static int search_result;
static unsigned char id_bit;
static unsigned char cmp_id_bit;
static unsigned char rom_byte_mask;
static unsigned char search_direction;


// TODO
// if OWPres error then sensor read error, so do not save just use locally?
//

//*****************************************************************
// Visible functions.
//
#ifdef _OWStatus
unsigned char OWStatus()
{
//    if (OWPres == 1)
//   {
        return OWSts;
//    }
//   return NO_TEMP_SENSORS;    // no sensors
}
#endif

#ifdef _OWTmp
int OWTmp( unsigned idx )
{
    if (   ( idx >= 0 ) 
        && ( idx < MAX_SENSORS )
        && ( ( OWSts & (0x01 << idx ) ) != 0 ) )
    {
	return OWRes[idx];
    }
    return INVALID_TEMP;
}
#endif

#ifdef _OWReadSensors
void OWReadSensors( unsigned char chn )
{
    OWReadSensorsOnly( chn );
    OWStartTempConversion( chn );
}
#endif

#ifdef _OWIdentifySensors
void OWIdentifySensors()
{
    // first see what we already have. means OwSts set for known sensors.
    // Updates OWPres.
    int idx;
    int OWPres;
    for ( idx = 0; idx < MAX_SENSORS; ++idx )
    {
        OWReadSensorsOnly( idx );    // do not trigger a new conversion.
    }
    
    OWPres = ResetCheck1W() ; // is anyone there ?
    if ( OWPres == 1 )
    {
    	int rslt, idx;
        unsigned char *anAdr;
	
    	rslt = OWFirst();
    	while (rslt)
    	{
	    OWReadSensor( ROM_NO );   // Reads data for this sensor to get "sensor tag"
	    
	    // do we already have this one in the address list
            anAdr = OWAddresses;
	    for ( idx = 0; idx < MAX_SENSORS; ++idx )
	    {
	        if ( memcmp( (void*)ROM_NO, (void*)anAdr, 8 ) == 0 )
	        {
	            //  found it.
	    	    if ( OWRdBuf[3] != idx )
	    	    {
	    		// mismatched chip tag
			OWWriteTag( ROM_NO, idx );
	    	    }
	    	    break;
	    	}
                anAdr += 8;
	    }
	    if (   ( idx >= MAX_SENSORS ) 
                && ( ROM_NO[0] == DS1W_1820_FAMILY ) )
	    {
		// Has this sensor been tagged and is the location spare.
	    	idx = OWRdBuf[3];
		if (   ( idx != OWRdBuf[2] ) 
		    || ( idx < 0 ) 
	    	    || ( idx >= MAX_SENSORS ) 
		    || ( ( OWSts & (0x01<<idx) ) != 0 ) )
		{
		    // no so search for another slot.
		    // NOTE if OWSts is greater than zero its already in use.
		    idx = 0;
		    while (    ( idx < MAX_SENSORS )
		    	&& ( ( OWSts & (0x01<<idx) ) != 0 ) )
		    {
		        ++idx;
		    }
		}
		else
		{
		}
		if ( idx < MAX_SENSORS )
		{
	    	    memcpy( (void*)(OWAddresses + (8*idx)), (void*)ROM_NO, 8 );
	    	    // Update values.
	    	    OWSts |= (0x01<<idx);
    	    	    OWRes[idx] = (((unsigned)OWRdBuf[1]) << 8) | ((unsigned)OWRdBuf[0]);
		    OWWriteTag( ROM_NO, idx );
		}
	    }
            rslt = OWNext( );
        }
    }
}
#endif

// Private Functions.

void OWReadSensorsOnly( unsigned char chn )
{
    // test for power up state of scratch pad when temp reading is 85C (0x50, 0x05)
    if ( OWReadSensor( OWAddresses + chn*8 ) 
            && !( (OWRdBuf[0] == 0x50) && (OWRdBuf[1] == 0x05) ) )
    {
        if ( (OWRdBuf[0] == 0x0) && (OWRdBuf[1] == 0x0) )
        {
            DebugB( DebugTrace_1WireRead, OWRdBuf, 9 );
        }
        OWErrCnt[chn] = 0;
    	OWRes[chn] = (((unsigned)OWRdBuf[1]) << 8) | ((unsigned)OWRdBuf[0]);
    	OWSts |= (0x01 << chn);
    }
    else
    if (OWErrCnt[chn] > MAX_READ_ERROR)
    {
        OWRes[chn] = INVALID_TEMP;
    	OWSts = OWSts & (~(0x01 << chn));
    }
    else
    {
        ++OWErrCnt[chn];
    }
}

void OWStartTempConversion( unsigned char chn )
{
    if ( OWWriteAddress( OWAddresses + chn*8 ) )
    {
        OneWTrace( DebugTrace_1WireStart, 0, 0 );
	Tx1Wire(0x44) ;	// Start temperature conversion
    }
/*
    int OWPres = ResetCheck1W() ;
    if (OWPres == 1)
    {
        OneWTrace( DebugTrace_1WireStart, 0, 0 );
	Tx1Wire(0xCC) ;	// Skip
	Tx1Wire(0x44) ;	// Start temperature conversion
    }
*/
}

void OWStartTempConversionAll()
{
    int OWPres = ResetCheck1W() ;
    if (OWPres == 1)
    {
        OneWTrace( DebugTrace_1WireStart, 0, 0 );
	Tx1Wire(0xCC) ;	// Skip
	Tx1Wire(0x44) ;	// Start temperature conversion
    }
}

#ifdef _OWReadSingle
// return true if read Ok.
int OWReadSingle()
{
    int OWPres = ResetCheck1W() ;				// is anyone there ?
    OWSts = 0;
    OWRes[0] = INVALID_TEMP;
    if (OWPres == 1)
    {
	Tx1Wire(0xCC) ;	// Skip
	if ( OWReadData( ) )
	{
    	    OWRes[0] = (OWRdBuf[1] << 8) | OWRdBuf[0];
	    OWSts |= 1;
	}

    	// restart temp conversion.
	OWPres = ResetCheck1W() ;
	if (OWPres == 1)
	{
	    Tx1Wire(0xCC) ;	// Skip
	    Tx1Wire(0x44) ;	// Start temperature conversion
	}
	
    }
    return ( (OWSts & 1) > 0 );	// may fail read.
}
#endif

#ifdef _OWReadAddress
// return true if read Ok.
int OWReadAddress( char *buf )
{
    int idx = 0;
    int OWPres = ResetCheck1W() ;				// is anyone there ?
    if (OWPres == 1)
    {
	InitCrc8();
	Tx1Wire(0x33) ;	// Read address
    	for ( idx = 0; idx < 8; ++idx )
    	{
    	    buf[idx] = Rx1Wire() ;
    	    // verify CRC.
    	    UpdateCrc8( buf[idx] );
	}
        OneWTrace( DebugTrace_1WireReadAdr, OWRdBuf, 8 );
	return (crc8 == 0);
    }
    return false;
}
#endif

//#ifdef _OWReadSensor
// return TRUE if read succeded
int OWReadSensor( unsigned char *adr )
{
    return OWWriteAddress( adr ) && OWReadData( );
}
//#endif

//#ifdef _OWReadData
int OWReadData( )
{
    // this is always called after someone else has checked the Bus.
    int idx = 0;
    InitCrc8();
    Tx1Wire(0xBE) ;	// Start read
    for ( idx = 0; idx < 9; ++idx )
    {
	OWRdBuf[idx] = Rx1Wire() ;
	UpdateCrc8(OWRdBuf[idx]);
    }
    OneWTrace( DebugTrace_1WireRead, data, 9 );
    return ( crc8 == 0 );
}
//#endif

//#ifdef _OWWriteAddress
// return TRUE if address written
int OWWriteAddress( unsigned char *adr )
{
    if ( *adr != 0 )
    {
        int idx = 0;
        int OWPres = ResetCheck1W() ;				// is anyone there ?
        if (OWPres == 1)
        {
	    Tx1Wire(0x55) ;	// Match
            OneWTrace( DebugTrace_1WireAdr, adr, 8 );
	    for ( idx = 0 ; idx < 8; ++idx )
	    {
	        Tx1Wire( adr[idx] );
	    }
        }
        return ( OWPres > 0 );
    }
    return false;
}
//#endif

//#ifdef _OWWriteTag
// return TRUE if tag set.
int OWWriteTag( unsigned char *adr, unsigned char tag )
{
    OneWTrace( DebugTrace_1WireTag, 0, 0 );
    if ( OWWriteAddress( adr ) )
    {
	Tx1Wire( 0x4E ) ;	// write scratchpad
    	Tx1Wire( tag );	// alarm High
    	Tx1Wire( tag );	// alarm Low
// This is different for 18s20.
    	Tx1Wire( 0x7F );	// 12 bit conversion
	if ( OWWriteAddress( adr ) )
    	{
	    Tx1Wire( 0x48 ) ;	// copy scratchpad to EEprom
	    return true;
	}
    }
    return false;
}

// erase the tags in sensor 'chn'
void OWDeTag(unsigned char chn)
{
    OWWriteTag( OWAddresses + (8*(int)chn), NULL_TAG ); // 
}

// erase the tags in all attached sensors
void OWDeTagAll()
{
    int OWPres;
// This could be faster by issuing ROM skip comman first and then write etc.
    OneWTrace( DebugTrace_1WireDeTag, 0, 0 );
#if 0
    unsigned char idx;
    for ( idx = 0; idx < MAX_SENSORS; ++idx )
    {
        OWDeTag( idx );
    }
#else
    OWPres = ResetCheck1W() ;				// is anyone there ?
    if (OWPres == 1)
    {
        Tx1Wire( 0xCC ); // Rom skip
        Tx1Wire( 0x4E );	// write scratchpad
        Tx1Wire( NULL_TAG );	// alarm High
        Tx1Wire( NULL_TAG );	// alarm Low
        Tx1Wire( 0x7F );	// 12 bit conversion
        OWPres = ResetCheck1W() ;				// is anyone there ?
        if (OWPres == 1)
        {
            Tx1Wire( 0xCC ); // Rom skip
	    Tx1Wire( 0x48 ) ;	// copy scratchpad to EEprom
        }
    }
#endif
}

//--------------------------------------------------------------------------
// Find the 'first' devices on the 1-Wire bus
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : no device present
//
int OWFirst()
{
//   memset( OWAddresses, 0, 8*MAX_SENSORS);
   // reset the search state
   LastDiscrepancy = 0;
   LastDeviceFlag = false;
   LastFamilyDiscrepancy = 0;

   return OWSearch();
}

//--------------------------------------------------------------------------
// Find the 'next' devices on the 1-Wire bus
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//
int OWNext()
{
   // leave the search state alone
   return OWSearch();
}

//--------------------------------------------------------------------------
// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return TRUE  : device found, ROM number in ROM_NO buffer
//        FALSE : device not found, end of search
//
int OWSearch()
{
    OneWTrace( DebugTrace_1WireSearch, 0, 0 );
   // initialize for search
   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = 0;
   InitCrc8();

   // if the last call was not the last one
   if (!LastDeviceFlag)
   {
      // 1-Wire reset
      if (!OWReset())
      {
         // reset the search
         LastDiscrepancy = 0;
         LastDeviceFlag = false;
         LastFamilyDiscrepancy = 0;
         return false;
      }

      // issue the search command 
      Tx1Wire(0xF0);  

      // loop to do the search
      do
      {
         // read a bit and its complement
         id_bit = OwRx1WireABit();
         cmp_id_bit = OwRx1WireABit();

         // check for no devices on 1-wire
         if ((id_bit == 1) && (cmp_id_bit == 1))
            break;
         else
         {
            // all devices coupled have 0 or 1
            if (id_bit != cmp_id_bit)
               search_direction = id_bit;  // bit write value for search
            else
            {
               // if this discrepancy if before the Last Discrepancy
               // on a previous next then pick the same as last time
               if (id_bit_number < LastDiscrepancy)
                  search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
               else
                  // if equal to last pick 1, if not then pick 0
                  search_direction = (id_bit_number == LastDiscrepancy);

               // if 0 was picked then record its position in LastZero
               if (search_direction == 0)
               {
                  last_zero = id_bit_number;

                  // check for Last discrepancy in family
                  if (last_zero < 9)
                     LastFamilyDiscrepancy = last_zero;
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if (search_direction == 1)
              ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
              ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            // serial number search direction write bit
            OwTx1WireABit(search_direction);

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0)
            {
                UpdateCrc8(ROM_NO[rom_byte_number]);  // accumulate the CRC
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      }
      while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if (! (   (id_bit_number < 65) 
             || (crc8 != 0) ) )
      {
         // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
         LastDiscrepancy = last_zero;

         // check for last device
         if (LastDiscrepancy == 0)
         {
            LastDeviceFlag = true;
         }
         
         // TODO DebugTrace?
         search_result = true;
      }
   }

   // if no device found then reset counters so next 'search' will be like a first
   if (!search_result || !ROM_NO[0])
   {
      LastDiscrepancy = 0;
      LastDeviceFlag = false;
      LastFamilyDiscrepancy = 0;
      search_result = false;
   }

   return search_result;
}

//--------------------------------------------------------------------------
// Verify the device with the ROM number in ROM_NO buffer is present.
// Return TRUE  : device verified present
//        FALSE : device not present
//
#ifdef _OWVerify
int OWVerify()
{
   unsigned char rom_backup[8];
   int i,rslt,ld_backup,ldf_backup,lfd_backup;

   // keep a backup copy of the current state
   for (i = 0; i < 8; i++)
      rom_backup[i] = ROM_NO[i];
   ld_backup = LastDiscrepancy;
   ldf_backup = LastDeviceFlag;
   lfd_backup = LastFamilyDiscrepancy;

   // set search to find the same device
   LastDiscrepancy = 64;
   LastDeviceFlag = false;

   if (OWSearch())
   {
      // check if same device found
      rslt = true;
      for (i = 0; i < 8; i++)
      {
         if (rom_backup[i] != ROM_NO[i])
         {
            rslt = false;
            break;
         }
      }
   }
   else
     rslt = FALSE;

   // restore the search state 
   for (i = 0; i < 8; i++)
      ROM_NO[i] = rom_backup[i];
   LastDiscrepancy = ld_backup;
   LastDeviceFlag = ldf_backup;
   LastFamilyDiscrepancy = lfd_backup;

   // return the result of the verify
   return rslt;
}
#endif

//--------------------------------------------------------------------------
// Setup the search to find the device type 'family_code' on the next call
// to OWNext() if it is present.
//
#ifdef _OWTargetSetup
void OWTargetSetup(unsigned char family_code)
{
   int i;

   // set the search state to find SearchFamily type devices
   ROM_NO[0] = family_code;
   for (i = 1; i < 8; i++)
      ROM_NO[i] = 0;
   LastDiscrepancy = 64;
   LastFamilyDiscrepancy = 0;
   LastDeviceFlag = false;
}
#endif

//--------------------------------------------------------------------------
// Setup the search to skip the current device type on the next call
// to OWNext().
//
#ifdef _OWFamilySkipSetup
void OWFamilySkipSetup()
{
   // set the Last discrepancy to last family discrepancy
   LastDiscrepancy = LastFamilyDiscrepancy;
   LastFamilyDiscrepancy = 0;

   // check for end of list
   if (LastDiscrepancy == 0)
      LastDeviceFlag = true;
}
#endif

//--------------------------------------------------------------------------
// Reset the 1-Wire bus and return the presence of any device
// Return TRUE  : device present
//        FALSE : no device present
//
int OWReset()
{
    return ResetCheck1W();
}

//--------------------------------------------------------------------------

// TEST BUILD
static const unsigned char dscrc_table[] = {
        0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
      157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
       35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
      190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
       70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
      219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
      101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
      248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
      140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
       17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
      175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
       50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
      202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
       87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
      233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
      116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};

void InitCrc8()
//void InitCrc8(unsigned char value)
{
    crc8 = 0;
}

//--------------------------------------------------------------------------
// Calculate the CRC8 of the byte value provided with the current 
// global 'crc8' value. 
// Returns current global crc8 value
//
unsigned char UpdateCrc8(unsigned char value)
{
   // See Application Note 27
   
   // TEST BUILD
   crc8 = dscrc_table[crc8 ^ value];
//   fnprintf( AddTx, "value %X, crc8 %X", value, crc8 );
   return crc8;
}
//#endif

