#ifdef I2CEEUsed
//#include "Test_Auto.h"
#include <strings.h>
//#define DEBUG_I2C( s, i )  fnprintf(AddTx, s, i );
#define DEBUG_I2C( s, i )

#define WRITE_ADDRESS   (i2c_busAddress)
#define READ_ADDRESS    (i2c_busAddress | 0x01)
#define DO_READ         ( ( i2c_flags & I2C_READ_BIT ) == I2C_READ_BIT )
#define DO_WRITE        ( ( i2c_flags & I2C_READ_BIT ) != I2C_READ_BIT )
#define DO_BYTEADDRESS  ( ( i2c_flags & I2C_BYTEADDRESS_BIT ) == I2C_BYTEADDRESS_BIT )
#define DO_WORDADDRESS  ( ( i2c_flags & I2C_BYTEADDRESS_BIT ) != I2C_BYTEADDRESS_BIT )

unsigned char ee1;
unsigned char ee2;	//Temporary storage

void Check_ee_State()
{
	if(PIR1&(1<<SSPIF))
 		{
  		PIR1&= ~(1<<SSPIF);	//Clear the interrupt flag
  		process_ee_state();
		}
}
//
//THIS IS THE STATE PROCESSING UPON RECEIVING AN INTERRUPT.
//
//
//	(c) David Kerr April 2005. May be used and or modified provided this Copyright notice is displayed in the header
//
//	Revision History
//	April 2005			Original coding
//	5th August 2005		Modifications to become FED Element
//	6th August 2005		Addition of State 6 for Reads immediately following long Writes
//
//	State machine processing for hardware I2C external EEPROM support
//	for a 24LC256 EEProm (or equivalent) at address zero.
//	Note that addressing for multiple EEProms can be  introduced by
//	incorporating the device address into the read and write control words.
//	If real time addressing is desired, change the control words from constants to
//	variables and set them in the eeprom read/write routine.
//
//	Note that the bank write operation may not be complete until after the
//	status here says it is done. Thiis is because of the internal write cycle
//	in the EEProm. If another read/write occurs, the state processing does
//	"poll acknowledge" checking per the 24LC256 data sheet until the
//	device is ready.
//
void process_ee_state() 	
{ 	
 	ee2=i2c_state;
    DEBUG_I2C( "\n\rState %u", ee2 );
    DEBUG_I2C( " SSPSTAT %x", SSPSTAT );
    DEBUG_I2C( " SSPCON2 %x ", SSPCON2 );
 	i2c_state=98;	 //Failed is the default!
 	switch(ee2)
 	
 		{
 		case 1:
 			{
            DEBUG_I2C( "BusAdr %x", WRITE_ADDRESS );
 			SSPBUF=WRITE_ADDRESS;	//Send the control word
 			i2c_state=2;
            ee1 = 0;    // reset byte counter
 			break;
 			}
 			
 		case 2:
 			{
 			if((SSPCON2&(1<<ACKSTAT))==0)
 				{
                if ( DO_BYTEADDRESS )
                    {
                    DEBUG_I2C( "DevAdr low %x", i2c_addressl );
                    SSPBUF=i2c_addressl;	//Send the low address
                    if ( DO_READ )
                        {
                        i2c_state=10;
                        }
                    else 
                        {
                        i2c_state=4;
                        }
                    }
                else
                    {
                    DEBUG_I2C( "DevAdr High %x", i2c_addressh );
                    SSPBUF=i2c_addressh;	//Send the high address
                    i2c_state=3;
                    }
                }
 			else 	//We are still busy from a previous write
 				{	//So we must perform acknowledge polling
                DEBUG_I2C( "Busy", 0 );
 				SSPCON2|=1<<SEN;	//Re-assert the start condition
 				SSPBUF=WRITE_ADDRESS;	//Resend the control word
 				i2c_state=6;
 				}	
 			break;	
 			}
 			
 		case 6:
 			{
            DEBUG_I2C( "Res BusAdr %x", WRITE_ADDRESS );
 			SSPBUF=WRITE_ADDRESS;	//Resend the control word
 			i2c_state=2;	//Then back to "normal" state processing
 			break;
 			}
 				
 			
 		case 3:
 			{
 			if((SSPCON2&(1<<ACKSTAT))==0)
 				{
                DEBUG_I2C( "DevAdr low %x", i2c_addressl );
 				SSPBUF=i2c_addressl;	//Send the low address
 						//At this point, read and write processing diverges
                if ( DO_READ )
                    {
                    i2c_state=10;
                    }
                else 
                    {
                    i2c_state=4;
                    }
 				}
 			else 
            {
                SSPCON2|=1<<PEN; //Assert the Stop condition
            }
 			break;
 			}
 			
 		case 4:
 			{				//Page Write
 			if((SSPCON2&(1<<ACKSTAT))==0)
 				{
 				if(ee1>=i2c_bytes)
 					{
 					SSPCON2|=1<<PEN; //At the end- stop
 					i2c_state=5;
 					break;
 					}
 				
                DEBUG_I2C( "Wr Byte %x", *(i2c_pointer+ee1) );
 				SSPBUF=*(i2c_pointer+ee1++); //Send the next byte
 				i2c_state=4;	//Override the default
 				}
 			else 
            {
                SSPCON2|=1<<PEN; //Assert the Stop condition
            }
 			break;
 			}
 			
 		case 5:
 			{
			//Set "all finished" flag
            DEBUG_I2C( "Write Complete", 0 );
#ifdef EEWriteCompleteUsed
			EEWriteCompleteFlag|=(1<<EEWriteCompleteFlagBit);
#endif
   			i2c_state=0;
 			break;
 			}
 			
 		case 10:
 			{
 			if((SSPCON2&(1<<ACKSTAT))==0)
            {
                DEBUG_I2C( "DoStop", 0 );
                i2c_state=11;
 						//Do a stop here
 						//because 24LC256 does not follow
 						//the data sheet exactly- Start here is no good!
 						//Stop forces the address load okay
 				
// 						else is default i2c_state=98;	 //Failed!
            }
 				
 			SSPCON2|=1<<PEN; //Assert the Stop condition
 			break;
 			}
 
 		case 11:
 			{
            DEBUG_I2C( "DoStart", 0 );
 			SSPCON2|=1<<SEN; //Assert the start condition
 			i2c_state=12;
 			break;
 			}	
 		
 		case 12:
 			{
            DEBUG_I2C( "BusAdr", READ_ADDRESS );
 			SSPBUF=READ_ADDRESS;	//Send the control word with read set
 			i2c_state=13;
 			break;
 			}
 			
 		case 13:
 			{
 			if((SSPCON2&(1<<ACKSTAT))==0)
 				{
                DEBUG_I2C( "RecEn", 0 );
 				i2c_state=14;		//Have ACK- await read
 				SSPCON2|=1<<RCEN;	//Set Receive enable
 				}
 			else SSPCON2|=1<<PEN; //Assert the Stop condition
 			break;
 			}
 			
 		
 		case 14:
 			{
 			if((SSPSTAT&1)!=0)
 				{		//We have data!
                DEBUG_I2C( "ByteRx", SSPBUF );
 				*(i2c_pointer+ee1)=SSPBUF; //Get the next byte
 				if(++ee1 == i2c_bytes)
 					{
 					SSPCON2|=1<<PEN; //At the end- stop with no ACK
 					i2c_state=16;
 					break;
 					}
 				SSPCON2&=~(1<<ACKDT);	   //Send ACK & and remain in the same state
 				SSPCON2|=1<<ACKEN;
 				i2c_state=15;
 				}
 			else SSPCON2|=1<<PEN; //Error. No Data. Assert the Stop condition	
 			break;
 			}
 		
 		case 15:		//ACK cycle completed
 			{
            DEBUG_I2C( "RecEn", 0 );
 			i2c_state=14;
 			SSPCON2|=1<<RCEN;	//Set Receive enable for next byte
 			break;
 			}
 			
 		case 16:
 			{
			//Set "all finished" flag
            DEBUG_I2C( "Read Complete", 0 );
#ifdef EEReadCompleteUsed
			EEReadCompleteFlag|=(1<<EEReadCompleteFlagBit);
#endif
   			i2c_state=0;
 			break;
 			}
 			
 		case 98:
 			{
 				//Stopped EEProm and we finished with an error
#ifdef EEReadFailedUsed
            if ( DO_READ )
            {
 			    EEReadFailedFlag|=(1<<EEReadFailedFlagBit);
            }
#endif
#ifdef EEWriteFailedUsed
            if ( DO_WRITE )
            {
 			    EEWriteFailedFlag|=(1<<EEWriteFailedFlagBit);
            }
#endif
   			i2c_state=0;
 			break;
 			}
 			
 			
 		}
 	
 }

#endif


