import sys
sys.path.append('../API')
#
# Harmony to webbrick interface main code
#

import logging
import wbUdpEvents, wbXmlEvent
import string

logging.basicConfig(level=logging.INFO)

#logging.basicConfig(level=logging.INFO,
#                    format='%(asctime)s %(levelname)s %(message)s',
#                    filename='./WebBrickMonitor.log',
#                    filemode='w')

class myTarget( wbXmlEvent.wbXmlTarget ):
    def __init__(self):
        logging.info( 'WebBrick Xml Events' )

    def XmlEvent( self, Xml ):
        logging.info( Xml )
        return
		
udp = wbUdpEvents.wbUdpEvents( wbXmlEvent.wbXmlEvent( myTarget() ) )
udp.startMonitor()

