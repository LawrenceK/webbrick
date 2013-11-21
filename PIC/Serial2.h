//
//  Copyright L.P.Klyne 2013
//
//  Filename: Serial2.h
//
//  Description:    Serial interface controller
//
//  Notes:
//
//

#define setRs485()        bRF6 = 0
#define setRs232()        bRF6 = 1
#define setRs485TxOn()    bRF7 = 1
#define setRs485TxOff()   bRF7 = 0
#define setSerial2TxHigh()  bRG1 = 1
#define setSerial2TxLow()   bRG1 = 0

#define BAUD_300    0
#define BAUD_600    1
#define BAUD_1200    2
#define BAUD_2400    3
#define BAUD_4800    4
#define BAUD_9600    5
#define BAUD_19200    6
#define BAUD_38400    7
#define BAUD_57600    8
#define BAUD_115200    9
#define BAUD_250000    10
#define BAUD_MAX    BAUD_250000

//#define SerialSendByte(b) TXREG2 = b
void SerialSendByte(unsigned char b);

// Called on user loop to see what data has been received
void CheckSerial();
// called circa millisecond to timeout buffered data.
void SerialReceivetimeout();
void SerialInit();
