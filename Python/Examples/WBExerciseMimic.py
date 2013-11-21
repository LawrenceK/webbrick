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
pattern = ( 
    (63,32,0,0,0,0,0,32 ),
    (16,63,16,0,0,0,0,0 ),
    (0,16,63,16,0,0,0,0 ),
    (0,0,16,63,16,0,0,0 ),
    (0,0,0,16,63,16,0,0 ),
    (0,0,0,0,16,63,16,0 ),
    (0,0,0,0,0,16,63,16 ),
    (16,0,0,0,0,0,16,63 ),
    )

egg = ( 
    (63,0,0 ),
    (63,32,0 ),
    (63,63,0 ),
    (32,63,0 ),
    (0,63,0 ),
    (0,63,32 ),
    (0,63,63 ),
    (0,32,63 ),
    (0,0,63),
    (32,0,63 ),
    (63,0,63 ),
    (63,0,32 ),
    )

def makeCmd( pat ):
    cmd = "DM"
    for i in range(len(pat)):
        cmd = cmd + "%i;%i;" % (i,pat[i])
    return cmd[0:len(cmd)-1]

def sendHttp( pat ):
    while 1:
        for i in range(8):
            cmd = makeCmd( pat[i] )
            wb6.SendHTTP(targetIP, cmd)
            time.sleep(0.5)

def sendUdp( pat ):

    while 1:
        for i in range(8):
            cmd = makeCmd( pat[i] )
            wbUdp.sendFrame( targetIP, cmd)
            time.sleep(0.5)
            
if __name__ == "__main__":

    test = sendHttp
    pat = egg
    args = sys.argv[1:]
    for arg in args:
        if arg == "udp":
            test = sendUdp
        if arg == "http":
            test = sendHttp
        if arg == "egg":
            pat = egg

    test( pat )
