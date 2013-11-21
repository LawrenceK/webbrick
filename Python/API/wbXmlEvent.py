#
# Harmony to webbrick interface main code
#

import wbUdpEvents
import string

#logging.basicConfig(level=logging.INFO,
#                    format='%(asctime)s %(levelname)s %(message)s',
#                    filename='./WebBrickMonitor.log',
#                    filemode='w')

class wbXmlTarget:
	def XmlEvent( self, Xml ):
		return

class wbXmlEvent( wbUdpEvents.wbEvents ):
    def __init__(self, target):
        self.xmlTarget = target

    def targetType( self, event ):
        if event.tgtType() == 0:
            return 'Digital'
        elif event.tgtType() == 1:
            return 'Scene'
        else:
            return 'Analogue'

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
        return str(event.action())

    def allFields( self, event ):
		return string.join( 
				( '<WbEvent IpAddr="',event.ipAdr(), 
				'" UdpType="', event.udpType(),
				'" EventType="', event.pktType(),
				'" From="',str(event.fromNode()),
				'" To="',str(event.fromNode()),
				'" Source="',str(event.srcChannel()),
				'" Target="',str(event.tgtChannel()),
				'" TargetType="',self.targetType(event),
				'" Action="',self.trigger(event),
				'" Dwell="',str(event.dwell()),
				'" SetPoint="',str(event.setPoint()),
				'" Value="',str(event.val()),
				'"/>'
				), ""
			)
            
    def wbNewNode( self, event ):
		self.xmlTarget.XmlEvent( string.join( 
				( '<WbEvent IpAddr="',event.ipAdr(), 
				'" UdpType="', event.udpType(),
				'" EventType="', event.pktType(),
				'" From="',str(event.fromNode()),
				'" To="',str(event.fromNode()),
				'" Source="',str(event.srcChannel()),
				'" Target="',str(event.tgtChannel()),
				'" TargetType="',self.targetType(event),
				'" Action="',self.trigger(event),
				'" Dwell="',str(event.dwell()),
				'" SetPoint="',str(event.setPoint()),
				'" Value="',str(event.val()),
				'"/>'
				), ""
			) )

    def wbStarted( self, event ):
		self.xmlTarget.XmlEvent( string.join( 
				( '<WbEvent IpAddr="',event.ipAdr(), 
				'" UdpType="', event.udpType(),
				'" EventType="', event.pktType(),
				'" From="',str(event.fromNode()),
				'" To="',str(event.fromNode()),
				'" Source="',str(event.srcChannel()),
				'" Target="',str(event.tgtChannel()),
				'" TargetType="',self.targetType(event),
				'" Action="',self.trigger(event),
				'" Dwell="',str(event.dwell()),
				'" SetPoint="',str(event.setPoint()),
				'" Value="',str(event.val()),
				'"/>'
				), ""
			) )

    def wbAnLow( self, event ):
		self.xmlTarget.XmlEvent( self.allFields( event ) )

    def wbAnHi( self, event ):
		self.xmlTarget.XmlEvent( self.allFields( event ) )

    def wbTempLow( self, event ):
		self.xmlTarget.XmlEvent( self.allFields( event ) )

    def wbTempHi( self, event ):
		self.xmlTarget.XmlEvent( self.allFields( event ) )

    def wbDigIn( self, event ):
		self.xmlTarget.XmlEvent( self.allFields( event ) )

    def wbRemoteDigIn( self, event ):
		self.xmlTarget.XmlEvent( self.allFields( event ) )

    def wbScheduled( self, event ):
		self.xmlTarget.XmlEvent( self.allFields( event ) )

    def wbAnIn( self, event ):
		self.xmlTarget.XmlEvent( self.allFields( event ) )

    def wbAnOut( self, event ):
		self.xmlTarget.XmlEvent( self.allFields( event ) )

    def wbDigOut( self, event ):
		self.xmlTarget.XmlEvent( self.allFields( event ) )

    def wbMonitor( self, event ):
		self.xmlTarget.XmlEvent( self.allFields( event ) )
