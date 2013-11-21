import sys
sys.path.append('../API')

#
# WBExercise
#
#  Andy Harris
# 
#  Beat the living daylights out of a WebBrick till it gives up
#


import wbUdp, time

targetIP = '10.100.100.100'

while 1:
    for i in range(8):
        wbUdp.sendFrame(targetIP,"DO" + str(i) + ";T:" )
        time.sleep(0.03)
#    for i in range(8):
#        wbUdp.sendFrame(targetIP,"DO" + str(i) + ";T:" )
#        time.sleep(0.05)

