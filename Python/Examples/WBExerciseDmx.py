import sys
from random import randrange



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

# this is for the LED DMX lamp.

targetIP = '10.100.100.100'
stepDelay = 0.05
defChannel = 2
start = 0
stop = 255
step = 16
maxBright = 127 # above this 
doInvert = False
initDone = False

def doDmxCmd(chn,level):
    if doInvert:
        cmd = "DX%i;%i" % (chn,(255-level))
    else:
        cmd = "DX%i;%i" % (chn,level)
    print cmd
    wb6.SendHTTP(targetIP,cmd )

def allOff():
    init()
    for idx in range(1,9,1):
        doDmxCmd(idx,0)
        
def allOn():
    init()
    for idx in range(1,9,1):
        doDmxCmd(idx,255)
        
def init():
    global initDone
    if not initDone:
        wb6.Login( targetIP,"installer" )
        wb6.SendHTTP(targetIP,"CR3" ) # DMX
        for idx in range(1,9,1):
            doDmxCmd(idx,0)
        initDone = True

def doDmx():
    init()
    doDmxCmd( 1, randrange( start, maxBright ) )
    doDmxCmd( 2, start)
    doDmxCmd( 3, stop)
    doDmxCmd( 4, start)
    
    # 2 down 3 up, 4 down
    # so down with 3 and up with 2
    for idx in range(start,stop,step):
        doDmxCmd( 2, idx)
        doDmxCmd( 3, stop-idx)
        time.sleep(stepDelay)
    doDmxCmd( 3, start)

    # 2 up 3 down, 4 down
    # so down with 2 and up with 4
    for idx in range(start,stop,step):
        doDmxCmd( 4, idx)
        doDmxCmd( 2, stop-idx)
        time.sleep(stepDelay)
    doDmxCmd( 2, start)

    for idx in range(start,stop,step):
        doDmxCmd( 3, idx)
        doDmxCmd( 4, stop-idx)
        time.sleep(stepDelay)
    doDmxCmd( 4, start)

def doDmxNightRider():
    init()
    for cnt in range(30):
        for idx in range(1,9,1):
            for idx2 in range(1,9,1):
                if idx == idx2:
                    doDmxCmd( idx2, 255)
                else:
                    doDmxCmd( idx2, 0)
            time.sleep(0.05)
        for idx in range(8):
            for idx2 in range(1,9,1):
                if idx == idx2:
                    doDmxCmd( 9-idx2, 255)
                else:
                    doDmxCmd( 9-idx2, 0)
            time.sleep(0.05)
            
def doDmxSequenceUp( limit ):
    for idx in range(1, limit+1, 1):
        if idx == 1:
            doDmxCmd( limit, 0)
        else:
            doDmxCmd( idx-1, 0)
        doDmxCmd( idx, 255)
        time.sleep(0.05)
    doDmxCmd( limit, 0)

def doDmxSequenceDown( limit ):
    for idx in range(limit,0,-1):
        if idx == limit:
            doDmxCmd( 1, 0)
        else:
            doDmxCmd( idx+1, 0)
        doDmxCmd( idx, 255)
        time.sleep(0.05)
    doDmxCmd( 1, 0)

def doDmxSequence( limit ):
    init()
    for cnt in range(6):
        doDmxSequenceUp(limit)
        doDmxSequenceDown(limit)

def doFlash( chn, int = 0.5 ):
    print "flash ", chn
    init()
    for idx in range(4):
        doDmxCmd( chn, 0)
        time.sleep(int)
        doDmxCmd( chn, 255)
        time.sleep(int)
        
def doFlashAll2( int = 0.5):
    for chn in range(1,9,1):
        doFlash( chn, int )

def doFlashAll( int = 0.5):
    init()
    for idx in range(8):
        for chn in range(1,9,1):
            doDmxCmd( chn, 0)
        time.sleep(int)
        for chn in range(1,9,1):
            doDmxCmd( chn, 255)
        time.sleep(int)

def doFade( int = 0.2):
    init()
    for idx in range(256):
        doDmxCmd( defChannel, idx)
        time.sleep(int)
    for idx in range(256):
        doDmxCmd( defChannel, 255-idx)
        time.sleep(int)

#doDmx()
#doDmxNightRider()
#doFlashAll()
#doFlashAll2()
#doDmxSequence(6)
#allOff()
#allOn()
#doFlash( 6 )
doFade()
