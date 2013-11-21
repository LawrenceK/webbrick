import sys
sys.path.append('../API')
#
# WBExercise
#
#  Andy Harris
# 
#  Beat the living daylights out of a WebBrick till it gives up
#


import wb6, wbUdp, time
import serial

targetIP = '10.100.100.100'
Rs232portName = 15
#Rs232portName = "COM16"
Rs485portName = "COM9"

BaudIdx = 5
BaudTable = [300,600,1200,2400,4800,9600,19200,38400,57600,115200,250000]

doTest = 2  # 0 - Out, 1 - In, 2 - both
doRs485 = False
pktSize = 40
serSize = 6

az = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"

# set up test data/commands
strings = list()
for i in range(0,len(az),pktSize):
    strings.append( az[i:i+pktSize] )

serCmd = list()
for i in range( ord('a'), ord('z')+1, serSize ):
    serCmd.append( "RD%i;%i;%i;%i;%i;%i" %(i,i+1,i+2,i+3,i+4,i+5) )

def OpenPort():
    print "baud %i"%BaudTable[BaudIdx]
    
    if doRs485:
        return serial.Serial( port=Rs485portName, baudrate=BaudTable[BaudIdx], timeout=1, stopbits=2 )
    return serial.Serial( port=Rs232portName, baudrate=BaudTable[BaudIdx], timeout=1, stopbits=2 )
        
def serialInAndOut():
    
    port = OpenPort()
        
    for idx in range(1000):
        # address
        wb6.SendHTTP(targetIP,serCmd[idx % len(serCmd)] )
        port.write( strings[idx % len(strings)] )
#        time.sleep(0.5)
    port.close()
    port = None

def serialOut():
    for idx in range(10):
        # address
        for s in serCmd:
            wb6.SendHTTP(targetIP,s  )
        time.sleep(1.0)

#strings = [ "abcdef", "ghijkl", "mnopqr", "stuvwx", "yz"]
#strings2 = [ "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", "abcdef"]

def serialIn():
    port = OpenPort()
    
    # send strings
    for idx in range(10):
        for s in strings:
            print s
            port.write( s )
            time.sleep(0.1)
        # address
        time.sleep(1.0)

    # close comm port
    port.close()
    port = None

# add if main here and parameter checking.
   
wb6.Login( targetIP,"installer" )
if doRs485:
    cmd = "CR4;%i"%(BaudIdx)
    wb6.SendHTTP(targetIP,cmd  ) # RS485
else:
    cmd = "CR2;%i"%(BaudIdx)
print "setup %s" % cmd    
wb6.SendHTTP(targetIP,cmd ) # RS232
    
if doRs485:
    if doSend:
        wb6.SendHTTP(targetIP,"RO" ) # RS485 output
    else:
        wb6.SendHTTP(targetIP,"RI" ) # RS485 input

    
if doTest == 0:
    serialOut()
elif doTest == 1:
    serialIn()
else:
    serialInAndOut()
