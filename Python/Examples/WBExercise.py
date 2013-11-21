import sys
sys.path.append('../API')
#
# WBExercise
#
#  Andy Harris
# 
#  Beat the living daylights out of a WebBrick till it gives up
#


import wb6, time

#targetIP = 'mail.thirtover.com:84'
targetIP = '10.100.100.100'

while 1:
    for i in range(8):
        wb6.DigToggle(targetIP,str(i))
        time.sleep(0.1)
#    for i in range(8):
#        wb.DigToggle(targetIP,str(8-i))
#        time.sleep(0.1)

