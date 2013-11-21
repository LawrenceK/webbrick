import sys
sys.path.append('../API')

#
# Simple Python event monitor
#

import logging, time
import wbUdpEvents2

logging.basicConfig(level=logging.INFO)
#logging.basicConfig(level=logging.DEBUG)

#logging.basicConfig(level=logging.INFO,
#                    format='%(asctime)s %(levelname)s %(message)s',
#                    filename='./WebBrickMonitor.log',
#                    filemode='w')

class myTarget( wbUdpEvents2.wbEventTarget ):
    def __init__(self):
        logging.info( 'WebBrick Monitor' )

    def handleEvent( self, event ):
        logging.info( "%s:%s %s" % (event.type(), event.source(), event.other_data() )  )


tgt = myTarget()
udp = wbUdpEvents2.wbUdpEvents2( tgt )
udp.startMonitor()
while ( udp.alive() ):
    time.sleep( 0.5 )

