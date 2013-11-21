#
#  Class to handle UDP
#
#  Lawrence Klyne
#
#
import socket, logging, threading
#import sys, string, xml.dom.minidom, urllib
#from socket import socket, AF_INET, SOCK_DGRAM
#from time import time, ctime, sleep

# This feels as if this needs further generalization and the event attributes should be dictionaried.


#
# A template event handler for inbound webbrick packets
# This is just a place holder, the event parameter will implement the wbEvent class that follows this.
#
class wbEvents:
    def wbNewNode( self, event ):
        return

    def wbStarted( self, event ):
        return

    def wbAnLow( self, event ):
        return

    def wbAnHi( self, event ):
        return

    def wbTempLow( self, event ):
        return

    def wbTempHi( self, event ):
        return

    def wbDigIn( self, event ):
        return

    def wbRemoteDigIn( self, event ):
        return

    def wbScheduled( self, event ):
        return

    def wbAnIn( self, event ):
        return

    def wbTempIn( self, event ):
        return

    def wbAnOut( self, event ):
        return

    def wbDigOut( self, event ):
        return

    def wbMonitor( self, event ):
        return

    def wbReconfigured( self, event ):
        return

# Generic event type from UDP event source
#
# Different event sources may implement this differently, but this is the interface that will be exposed.
#
class wbEvent:
    def ipAdr(self):
        return

    def macAdr(self):
        return

    def udpType(self):
        return

    def pktType(self):
        return

    def srcChannel(self):
        return

    def tgtChannel(self):
        return

    def tgtType(self):
        return

    def action(self):
        return

    def dwell(self):
        return

    def fromNode(self):
        return

    def toNode(self):
        return

    def setPoint(self):
        return

    def val(self):
        return 
        
# accessor to UDP data from webbrick 6.
class udp6Packet(wbEvent):
    def __init__ (self, adr, data):
        self._ipAdr = str(adr[0])
        self._udpType = data[1]
        self._pktType = data[2:4]
        self._macAdr = str(adr[0])
        if ( self._pktType == "NN" ):
            self._srcChannel = 0
            self._tgtType = 0
            self._tgtChannel = 0
            self._action = 0
            self._dwell = 0
            self._fromNode = 0
            self._toNode = 0
            self._setPoint = 0
            self._val = 0
            self._macAdr = [ord(data[4]), ord(data[5]),ord(data[6]),ord(data[7]),ord(data[8]),ord(data[9]) ]
        else:
            self._srcChannel = ord(data[4])
            self._tgtType = ord(data[5]) >> 6
            self._tgtChannel = ord(data[5]) & 0x1F
            self._action = ord(data[6]) & 0xF
            self._dwell = ord(data[6]) >> 4
            self._fromNode = ord(data[7])
            self._toNode = ord(data[8])
            self._setPoint = ord(data[9])
            self._val = (ord(data[10]) * 256) + ord(data[11])
            self._macAdr = []
        logging.debug( 'udpPacket' )
        logging.debug( self._ipAdr )
        logging.debug( self._macAdr )
        logging.debug( self._udpType )
        logging.debug( self._pktType )
        logging.debug( self._srcChannel )
        logging.debug( self._tgtChannel )
        logging.debug( self._action)
        logging.debug( self._dwell)
        logging.debug( self._fromNode )
        logging.debug( self._toNode )
        logging.debug( self._setPoint )
        logging.debug( self._val )

    def ipAdr(self):
        return self._ipAdr

    def macAdr(self):
        return self._macAdr

    def udpType(self):
        return self._udpType

    def pktType(self):
        return self._pktType

    def srcChannel(self):
        return self._srcChannel

    def tgtChannel(self):
        return self._tgtChannel

    def tgtType(self):
        return self._tgtType

    def action(self):
        return self._action

    def dwell(self):
        return self._dwell

    def fromNode(self):
        return self._fromNode

    def toNode(self):
        return self._toNode

    def setPoint(self):
        return self._setPoint

    def val(self):
        return self._val
        
# accessor to UDP data from webbrick 5
class udp5Packet(wbEvent):
    def __init__ (self, adr, data):
        self._ipAdr = str(adr[0])
        self._udpType = "G"
        self._srcChannel = 0
        self._tgtType = 0
        self._pktType = ""
        self._tgtChannel = 0
        self._action = 0
        self._dwell = 0
        self._fromNode = 0
        self._toNode = 0
        self._setPoint = 0
        self._val = 0
        self._macAdr = []
        
        logging.debug( "data " + data )
        
        if ( data[0] == "R" ):
            self._pktType = "TD"
            self._udpType = "R"
            self._toNode = ord(data[1])
            self._tgtChannel = ord(data[3])
            if ( data[2] == "D" ):
                self._tgtType = 0
            else:
                # analogue
                self._tgtType = 2
                self._setPoint = ord(data[5])
        elif ( data[0:2] == "NN" ):
            self._pktType = "NN"
            # thats it
        elif ( data[0:2] == "LT" ):
            self._udpType = "A"
            self._pktType = "Tt"
            self._fromNode = ord(data[2])
            self._val = ord(data[3]) * 16   # into 1/16ths to match DS18B20/wb6
            # high or low? use temperature and limits to decide.
            if ( ord(data[3]) > ord(data[6]) ) :
                self._pktType = "TT"
        elif ( data[0:2] == "LA" ):
            self._udpType = "A"
            self._pktType = "Ta"
            self._fromNode = ord(data[3])
            if ( ord(data[2]) == "H" ) :
                self._pktType = "TA"
            self._val = ord(data[4]) + ( ord(data[5]) << 8)
        elif ( data[0:2] == "DI" ):
            self._pktType = "TD"
            self._fromNode = ord(data[2])
            self._srcChannel = ord(data[3])
            # UDPString[4] = operand ;	// Ignore
        elif ( data[0:2] == "LI" ):
            # digital in generated alarm packet
            self._udpType = "A"
            self._pktType = "TD"
            self._fromNode = ord(data[2])
            self._srcChannel = ord(data[3])
        elif ( data[0:2] == "St" ):
            self._pktType = "SS"
            self._fromNode = ord(data[5])
        else:
            self._pktType = data[0:2]
        
        logging.debug( 'WebBrick5 udpPacket' )
        logging.debug( "ip " + self._ipAdr )
#        logging.debug( "mac " + self._macAdr )
        logging.debug( "udpType " + self._udpType )
        logging.debug( "pktType " + self._pktType )
        logging.debug( "srcChannel " + str(self._srcChannel) )
        logging.debug( "tgtChannel " + str(self._tgtChannel) )
        logging.debug( "action " + str(self._action) )
        logging.debug( "dwell " + str(self._dwell) )
        logging.debug( "fromNode " + str(self._fromNode) )
        logging.debug( "toNode " + str(self._toNode) )
        logging.debug( "sp " + str(self._setPoint) )
        logging.debug( "val " + str(self._val) )

    def ipAdr(self):
        return self._ipAdr

    def macAdr(self):
        return self._macAdr

    def udpType(self):
        return self._udpType

    def pktType(self):
        return self._pktType

    def srcChannel(self):
        return self._srcChannel

    def tgtChannel(self):
        return self._tgtChannel

    def tgtType(self):
        return self._tgtType

    def action(self):
        return self._action

    def dwell(self):
        return self._dwell

    def fromNode(self):
        return self._fromNode

    def toNode(self):
        return self._toNode

    def setPoint(self):
        return self._setPoint

    def val(self):
        return self._val

#
# WebBrick UDP event packets
#
class wbUdpEvents(threading.Thread):

    LOCALHOST = ''
    WBPORT = 2552

    def __init__ (self, eventTgt ):
        self.wbSkt = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.eventTgt = eventTgt
        self.running = True
        threading.Thread.__init__(self)

    # Terminate interface
    def shutdown(self):
        logging.info( 'WebBrick UDP monitor shutdown' )
        self.running = False
        self.wbSkt.close()

    def alive(self):
        return self.isAlive()

    def startMonitor(self):
        # bind socket
        self.wbSkt.bind((self.LOCALHOST, self.WBPORT))
        self.setDaemon( True )
        threading.Thread.start(self)

    def run(self):
        # stay in loop reading packets while socket open.
        logging.debug( 'wbUdpEvents run' )
        while ( self.running ):
            # read packet
            try:
                data = self.wbSkt.recvfrom(32)
                if data:
                    # create wbUdpEvents
                    # wb 6 UDP packets have a length byte in the first position, use to decide on 5 or 6.
                    if ( ord(data[0][0]) < 32 ):
                        udp = udp6Packet( data[1], data[0] )
                    else:
                        udp = udp5Packet( data[1], data[0] )

                    # call eventTgt method.
                    if udp.pktType() == "Ta" :
                        self.eventTgt.wbAnLow( udp )
                    elif udp.pktType() == "TA" :
                        self.eventTgt.wbAnHi( udp )
                    elif udp.pktType() == "Tt" :
                        self.eventTgt.wbTempLow( udp )
                    elif udp.pktType() == "TT" :
                        self.eventTgt.wbTempHi( udp )
                    elif udp.pktType() == "Td" :
                        self.eventTgt.wbRemoteDigIn( udp )
                    elif udp.pktType() == "TD" :
                        self.eventTgt.wbDigIn( udp )
                    elif udp.pktType() == "TS" :
                        self.eventTgt.wbScheduled( udp )
                    elif udp.pktType() == "AI" :
                        self.eventTgt.wbAnIn( udp )
                    elif udp.pktType() == "CT" :
                        self.eventTgt.wbTempIn( udp )
                    elif udp.pktType() == "AO" :
                        self.eventTgt.wbAnOut( udp )
                    elif udp.pktType() == "DO" :
                        self.eventTgt.wbDigOut( udp )
                    elif udp.pktType() == "MM" :
                        self.eventTgt.wbMonitor( udp )
                    elif udp.pktType() == "NS" :
                        self.eventTgt.wbNewNode( udp )
                    elif udp.pktType() == "NN" :
                        self.eventTgt.wbNewNode( udp )
                    elif udp.pktType() == "SN" :
                        self.eventTgt.wbNewNode( udp )
                    elif udp.pktType() == "SS" :
                        self.eventTgt.wbStarted( udp )
                    elif udp.pktType() == "CC" :
                        self.eventTgt.wbReconfigured( udp )
                    # end if pktType
                # end if data
            except socket.error, err:
                logging.error( err )
        logging.debug( 'wbUdpEvents stopping' )
                
    