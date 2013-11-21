import socket, logging, string

wbPort = 26482

def sendFrame(ipAddr, value):
    frameLen = len(value)
    logging.debug( 'ipAddr %s frameLen %i frame %s' % (ipAddr,frameLen,value) )
    
#    logging.debug( string.join(('frameLen', str(frameLen), "frame", value ), " " ) )
    FrameData = chr(frameLen) + chr(255-frameLen) + '\x19\xFF' + value + '\x00\x00'
    wbsocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    wbsocket.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, True)
    wbsocket.sendto(FrameData, (ipAddr, wbPort))
    return 1

