import sys
from socket import *
from struct import *

sys.path.append('../API')

#
# WBExercise
#
#  Andy Harris
# 
#  Beat the living daylights out of a WebBrick till it gives up
#


import wbUdp, time

#targetIP = '10.100.100.255'
targetIP = '255.255.255.255'

testEvents1 = [  
                # low Trigger from analogue
                "\012GTa\001\000\004\100\101\001\000\000\000\000\000\000",  
                # high Trigger from analogue
                "\012GTA\001\000\004\100\101\001\000\000\000\000\000\000",  
                # Trigger from remote DI command
                "\012GTd\001\000\004\100\101\001\000\000\000\000\000\000",  
                # Trigger from local digital input
                "\012GTD\001\000\004\100\101\001\000\000\000\000\000\000",  
                # low Trigger from temperature
                "\012GTt\001\000\004\100\101\001\000\000\000\000\000\000",  
                # hi Trigger from temperature
                "\012GTT\001\000\004\100\101\001\000\000\000\000\000\000",  
                # Trigger from scheduled event
                "\012GTS\001\000\004\100\101\001\000\000\000\000\000\000",  
                # new input value
                "\012GAI\001\000\004\100\101\001\000\000\000\000\000\000",  
                # new output value
                "\012GAO\001\000\004\100\101\001\000\000\000\000\000\000",  
                # new digital output state
                "\012GDO\001\000\004\100\101\001\000\000\000\000\000\000",  
                # unconfigured node.
                "\012GNN\001\000\004\100\101\001\000\000\000\000\000\000",  
                # node starting.
                "\012GSS\001\000\004\100\101\001\000\000\000\000\000\000",  
                # node sending attention
                "\012GAA\001\000\004\100\101\001\000\000\000\000\000\000",  
                # Current temperature
                "\012GCT\001\000\004\100\101\001\000\000\000\000\000\000",  
                # RTC clock data
                "\012GRR\001\000\004\100\101\001\000\000\000\000\000\000",  
                # infra red data
                "\012GIR\001\000\004\100\101\001\000\000\000\000\000\000"   
    ]


testEvents2 = [  
                # Trigger from local digital input
                # len,u,src,sCh,tCh,act,from,to,sp,valH/L
                "\x0CGTD\x00\x00\x04\x64\x65\x01\x00\x00",  
                "\x0CGTD\x01\x00\x04\x64\x65\x01\x00\x00",  
    ]
    
def _sendUdpEvents(events ):
    # open socket
    testSkt = socket(AF_INET, SOCK_DGRAM)
    testSkt.setsockopt(SOL_SOCKET, SO_BROADCAST, 1)
    # generate some UDP event packets
    for pkt in events:
        time.sleep(0.02)
        testSkt.sendto( pkt, (targetIP, 2552) )
    testSkt.close()

while 1:
    for i in range(100):
        print i
        _sendUdpEvents( testEvents1 )
        time.sleep(0.1)
