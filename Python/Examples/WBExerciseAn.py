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
def doPercent():
    for i in range(98,101):
        print "Percent %s" % (i)
        wb6.AnOutPercent(targetIP,2, i)
        time.sleep(10.0)
        
def doNumericLvl( val ):
    print "Level %s" % (val)
    cmd = "AV0;%i" % val
    wb6.SendHTTP(targetIP, cmd)

def doNumeric():
    for i in range(0,1024,8):
        doNumericLvl(i)
        time.sleep(0.2)
    doNumericLvl(1023)
        
def doSet():
    for i in range(4):
        wb6.AnOutSp(targetIP,str(i),3)
        time.sleep(0.1)
        
def doScan():
    while 1:
        print "Setpoint %s" % (7)
        for i in range(4):
            wb6.AnOutSp(targetIP,str(i),7)
            time.sleep(0.1)
        print "Setpoint %s" % (0)
        for i in range(4):
            wb6.AnOutSp(targetIP,str(i),0)
            time.sleep(0.1)

doNumeric()

