//
//
//	(c) David Kerr April 2005. May be used and or modified provided this Copyright notice is displayed in the header
//
//	Revision History
//	April 2005			Original coding
//	5th August 2005		Modifications to become FED Element
//	6th August 2005		Addition of State 6 for Reads immediately following long Writes
//
//Read and Write routine for external EEPROM 24LC256
//At address zero using the MSSP hardware.
//Other addresses may be used provided
//the read and write control words in the State Processing
//are changed from constants to variables
//and merged with the bit-shifted device addresses.
//
//This uses Page Write Mode and sequential read mode
//The user must ensure that in Write mode, no more than 64 bytes are written and
//that page boundaries [at 64byte offset] are respected. If more than 64 bytes
//are written or you attempt to write across a page boundary, the
//bytes will overwrite bytes earlier in the page.
//
//Read may be any length from 1 byte to the maximum of 32767
//
//Users can invoke the software by setting up the variables shown
//below in their own functions.
//
#ifndef _I2C_H
#define _I2C_H

#define I2C_BYTEADDRESS_BIT 0x01
#define I2C_BYTEADDRESS I2C_BYTEADDRESS_BIT
#define I2C_WORDADDRESS 0x00
#define I2C_READ_BIT    0x02
#define I2C_READ        I2C_READ_BIT
#define I2C_WRITE       0x00

extern void I2CStart(unsigned char busAddress, unsigned char *dataBuffer, unsigned int dataAddress, unsigned int dataLen, unsigned char flags );

#define i2c_read_ba( busAddress, data, address, len ) I2CStart( busAddress, data, (unsigned int)address, len, (I2C_READ | I2C_BYTEADDRESS) )
#define i2c_write_ba( busAddress, data, address, len ) I2CStart( busAddress, data, (unsigned int)address, len, (I2C_WRITE | I2C_BYTEADDRESS) )
#define i2c_read_wa( busAddress, data, address, len ) I2CStart( busAddress, data, address, len, (I2C_READ | I2C_WORDADDRESS) )
#define i2c_write_wa( busAddress, data, address, len ) I2CStart( busAddress, data, address, len, (I2C_WRITE | I2C_WORDADDRESS) )

extern unsigned char i2c_state;		//State for the EEPROM FSM
extern unsigned char i2c_busAddress;
extern unsigned char *i2c_pointer;
extern unsigned int i2c_bytes;			//Byte limit for Int routine
extern unsigned char i2c_addressl, i2c_addressh;	//Address in EEPROM
extern unsigned char i2c_flags;		//Determines read or write

void process_ee_state();

#endif
