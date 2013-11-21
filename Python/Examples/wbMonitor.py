import sys
sys.path.append('../API')

#
# Harmony to webbrick interface main code
#

import logging, time
import wbUdpEvents
import string

logging.basicConfig(level=logging.INFO)
#logging.basicConfig(level=logging.DEBUG)

#logging.basicConfig(level=logging.INFO,
#                    format='%(asctime)s %(levelname)s %(message)s',
#                    filename='./WebBrickMonitor.log',
#                    filemode='w')

class myTarget( wbUdpEvents.wbEvents ):
    def __init__(self):
        logging.info( 'WebBrick Monitor' )

    def source( self, event ):
        return string.join( ( event.ipAdr(), " Source (", event.udpType(), ")",str(event.fromNode()),":",str(event.srcChannel()) ), "" )

    def target( self, event ):
        if event.tgtType() == 0:
            return string.join( ( " Digital (", str(event.toNode()),":",str(event.tgtChannel()) ), "" )
        elif event.tgtType() == 1:
            return string.join( (" Scene (", str(event.toNode()),":",str(event.tgtChannel()) ), "" )
        else:
            # analogue
            return string.join( (" Analogue (", str(event.toNode()),":",str(event.tgtChannel())," SP", str(event.setPoint()) ), "" )

    def trigger( self, event ):
        if event.action() == 0:
            return "No Action"
        elif event.action() == 1:
            return "Off"
        elif event.action() == 2:
            return "On"
        elif event.action() == 3:
            return "Momentary"
        elif event.action() == 4:
            return "Toggle"
        elif event.action() == 5:
            return "Dwell " + str(event.dwell())
        elif event.action() == 6:
            return "DwellCan " + str(event.dwell())
        elif event.action() == 7:
            return "Next"
        elif event.action() == 8:
            return "Prev"
        return " Action " + str(event.action())

    def allValues( self, event ):
        return string.join( (self.source( event),self.target( event),self.trigger( event) ) )

    def wbNewNode( self, event ):
        mac = event.macAdr()
        macStr = string.join( (hex(mac[0]), hex(mac[1]), hex(mac[2]), hex(mac[3]), hex(mac[4]), hex(mac[5] )), ":" )
        logging.info( string.join(('Unconfigured WebBrick',event.ipAdr(),macStr ), " " ) )

    def wbStarted( self, event ):
        logging.info( string.join(('WebBrick started',self.source(event) ) ) )

    def wbAnLow( self, event ):
        logging.info( string.join( ('Analogue Under Threshold',self.allValues( event ) ) ) )

    def wbAnHi( self, event ):
        logging.info( string.join( ('Analogue Over Threshold',self.allValues( event ) ) ) )

    def wbTempLow( self, event ):
        logging.info( string.join( ('Temperature Under Threshold',self.allValues( event ) ) ) )

    def wbTempHi( self, event ):
        logging.info( string.join( ('Temperature Over Threshold',self.allValues( event ) ) ) )

    def wbTempIn( self, event ):
        logging.info( string.join( ('Temperature',self.source(event),"Value ",str(event.val()/16.0) ) ) )

    def wbDigIn( self, event ):
        logging.info( string.join( ('Digital In',self.allValues( event ),"Count ",str(event.val()) ) ) )

    def wbRemoteDigIn( self, event ):
        logging.info( string.join( ('Remote Digital In',self.allValues( event ) ) ) )

    def wbScheduled( self, event ):
        logging.info( string.join( ('Scheduled Event',self.allValues( event ) ) ) )

    def wbAnIn( self, event ):
        logging.info( string.join( ('Analogue In',self.source(event),"Value ",str(event.val()) ) ) )

    def wbAnOut( self, event ):
        logging.info( string.join( ('Analogue Out',self.source(event),"Value ",str(event.val()) ) ) )

    def wbDigOut( self, event ):
        logging.info( string.join( ('wbDigOut',self.source(event),str(event.action()) ) ) )

    def wbMonitor( self, event ):
        logging.info( string.join( ('wbMonitor',self.source(event) ) ) )

tgt = myTarget()
udp = wbUdpEvents.wbUdpEvents( tgt )
udp.startMonitor()
while ( udp.alive() ):
    time.sleep( 0.5 )

