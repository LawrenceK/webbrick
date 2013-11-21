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

#targetIP = 'mail.thirtover.com:84'
targetIP = '10.100.100.100'

wb6.Login( targetIP,"password" )
wb6.SendHTTP(targetIP,"ITN" )
wb6.SendHTTP(targetIP,"IRN" )
while 1:
    # address
    for i in range(32):
        # channel
        for j in range(64):
            wb6.SendHTTP(targetIP,"IS"+str(i)+";" + str(j)  )
            time.sleep(0.1)
