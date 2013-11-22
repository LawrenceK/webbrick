#include <pic.h>
#include <I2C.h>
//#include <Stdio.h>
//#include <Strings.h>

#define DEBUG_I2C( s, i )
//#define DEBUG_I2C( s, i )  fnprintf(AddTx, s, i );

void I2CStart( unsigned char busAddress, unsigned char *dataBuffer, unsigned int dataAddress, unsigned int dataLen, unsigned char flags )
{
    i2c_busAddress = busAddress;
	i2c_pointer=dataBuffer;	//Data pointer for the state processing routines
	i2c_bytes=dataLen;	//Number of bytes to move
	i2c_addressh=dataAddress>>8;	//High address bits follow control word
	i2c_addressl=dataAddress&0xff;	//then the low bits
	i2c_state=1;		//Set start state for interrupt code
	i2c_flags=flags;	//Set to 0 for write and 1 for read
    DEBUG_I2C( "\n\r BusAddress %x", i2c_busAddress );
    DEBUG_I2C( " DevAddress %x", dataAddress );
    DEBUG_I2C( " flags %x", i2c_flags );
	SSPCON2&=~(1<<RCEN);	//Disable Receive enable initially
	SSPCON2|=1<<SEN;	//Assert the start condition
}
