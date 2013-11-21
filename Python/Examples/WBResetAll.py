import sys, socket
sys.path.append('../API')

#
# WBExercise
#
#  Andy Harris
# 
#  Beat the living daylights out of a WebBrick till it gives up
#


import wbUdp, time

#targetIP = '255.255.255.255'
targetIP = '10.100.100.100'

def reset_sps():
    wbUdp.sendFrame(targetIP,"LGpassword:RS:" )

def reset_webbrick():
    wbUdp.sendFrame(targetIP,"LGpassword:RB:" )
    
if len(sys.argv) > 0:
    targetIP = sys.argv[1]

import logging
logging.basicConfig(level=logging.DEBUG)

reset_sps()
#reset_webbrick()
