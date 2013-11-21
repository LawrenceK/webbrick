#
# Class to handle Harmony HAPI API.
#
#    This code developed to interface a WebBrick (http://www.o2m8.com) into Harmony
#   Home automation server.
#
#   (C)o2m8 Ltd.
#

import mutex
import threading
import logging
import socket
import string
import time

#
# This is a place holder to capture the interface for delivering hanrony events.
# The passed vals are a sequence of the parameters in the harmony HAPI message. It has been
# separated at the commas.
# 
class harmonyEvents:
    def hRefreshProperties( self, param ):
        return
    def hCallMethod( self, param ):
        return
    def hIdent( self, param ):
        return
    def hDumpedData( self, param ):
        return
    def hConnected( self):
        return
    def hDisconnected( self):
        return

#
# This is the Python class that manages the Harmony API itself
#
#
# To use try implement the harmonyEvents target and pass to the creation of the Harmony Object
#
class Harmony(threading.Thread):
    DRIVER_NAME   = 'WebBrick'

    HARMONY_START   = '<HAPI>'
    HARMONY_START_LEN   = 6
    HARMONY_END     = '</HAPI>'
    HARMONY_END_LEN   = 7
    HARMONY_PORT    = 3131
    HARMONY_ADDRESS = '127.0.0.1'

    # Constructor
    def __init__(self, eventTgt):
        logging.debug( 'Harmony __INIT__ ' )
        self._eventTgt = eventTgt
        threading.Thread.__init__(self)
        self.harmonySkt = None
        self.bufferedData = ""
        self.opened = False
        self.running = True
        self.mut = mutex.mutex()

    # Start the Interface
    def start(self):
        logging.info( 'Harmony Start' )
        self.setDaemon( True )
        threading.Thread.start( self )

    # Terminate interface
    def shutdown(self):
        logging.info( 'Harmony Shutdown' )
        self.running = False
        self.harmonySkt.close()

    def connected(self):
        return self.opened

    def alive(self):
        return self.isAlive()

#
#   The following routines correspond to HAPI commands.
#
    # Register a Device
    def registerDevice( self, zone, devName, ipAdr=None, chn=None, type=None ):
        msg = 'Harmony registerDevice ' + zone + ' ' + devName
        if ( ipAdr != None ) :
            msg = msg + ' ' + ipAdr
        if ( chn != None ) :
            msg = msg + ' ' + chn
        logging.debug( msg )
        self.Send( "RegisterDevice", devName, "LED" )
        self.Send( "SetDeviceZone", devName, zone )
        if ( ipAdr != None ) and (chn != None ):
            self.Send( "SetDeviceAddress", devName, ipAdr + ":"+ chn )
        if ( type != None ):
            self.Send( "SetDeviceType", devName, type )

    # Register an event
    def registerEvent( self, devName, event ):
        logging.debug( 'Harmony registerEvent ' + devName + ' ' + event )
        self.Send( "RegisterEvent", devName, event )

    # Register a method
    def registerMethod( self, devName, method ):
        logging.debug( 'Harmony registerMethod ' + devName + ' ' + method )
        self.Send( "RegisterMethod", devName, method )

    # Register a discrete
    def registerDiscrete( self, devName, method ):
        logging.debug( 'Harmony RegisterDiscretes ' + devName + ' ' + method )
        self.Send( "RegisterDiscretes", devName, method )

    # Register a Property
    def registerProperty( self, devName, propName ):
        logging.debug( 'Harmony registerProperty ' + devName + ' ' + propName )
        self.Send( "RegisterProperty", devName, propName )

    # Set a Property value
    def setProperty( self, devName, propName, propValue ):
        logging.debug( 'Harmony registerProperty ' + devName + ' ' + propName + ' ' + propValue )
        self.Send( "PropertyValue", devName, propName+","+ propValue )

    # Raise a trigger event
    def sendTrigger( self, devName, eventName ):
        logging.debug( string.join( ('Harmony raise_Event',devName,eventName ), ' ' ) )
        self.Send( "raise_Event", devName, eventName )

    # update device state
    def sendState( self, devName, state ):
        logging.debug( string.join( ('Harmony StateChange',devName,state ), ' ' ) )
        self.Send( "StateChange", devName, state )

    # update device state
    def sendStateOn( self, devName ):
        self.sendState( devName, "On" )

    # update device state
    def sendStateOff( self, devName ):
        self.sendState( devName, "Off" )

    # update device state
    def sendStateLevel( self, devName, level ):
        self.sendState( devName, "Level"+str(level) )

    # Dump a database table
    def dumpDbTable( self, tableName ):
        logging.debug( 'Harmony dumpDBTable ' + tableName )
        self.Send( "dumpDB", tableName, None )

    # Create a new zone
    def createZone( self, zoneName ):
        logging.debug( 'Harmony createZone ' + zoneName )
        self.Send( "CreateZone", None, zoneName )

#
# Internal routines.
#
# Runs the Harmony API, we need to send periodic HEARTBEAT messages
    def run(self):
        sleepTime = 15
        logging.info( 'Enter Harmony run' )
        while self.running:
            if self.opened:
                # Do this on a timer and send heartbeat as required.
                try :
                    data = self.harmonySkt.recv(1024)
                    if data:
                        self.bufferedData = self.bufferedData + data
                        self.Process()
                    else:
                        self.Close()
                except socket.timeout:
                    logging.debug( 'Receive Timeout' )
                    self.Send( 'HeartBeat', None, None )
            else:
                self.Open()
                if not self.opened:
                    # go to sleep for a while
                    time.sleep( sleepTime )
        logging.info( 'Exit Harmony run' )

    # As this class is multithreaded we need to mutex the open and close.
    # Open the Harmony API
    def Close(self):
        logging.info( 'Harmony Close' )
        self.mut.lock( self.ClosePrim, None )

    # Open the Harmony API
    def Open(self):
        logging.info( 'Harmony Open' )
        self.mut.lock( self.OpenPrim, None )

    # Open the Harmony API
    def ClosePrim(self, dummy):
        if self.opened:
            # mark closed first
            self.opened = False
            self._eventTgt.hDisconnected()
            logging.debug( 'Harmony ClosePrim' )
            try:
                self.harmonySkt.close()
                self.harmonySkt = None
            except socket.error, err:
                logging.error( 'Harmony Close Error ' + str(err) )
        self.mut.unlock()

    # Open the Harmony API
    def OpenPrim(self, dummy):
        if not self.opened:
            logging.debug( 'Harmony OpenPrim' )
            # Create TCP socket and register WebBrick as name
            try:
                self.harmonySkt = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.harmonySkt.connect( (self.HARMONY_ADDRESS, self.HARMONY_PORT) )
                self.harmonySkt.settimeout( 30 )
                # disable nagle for speed and attempt to avoid Harmony bug.
                self.harmonySkt.setsockopt( socket.IPPROTO_TCP, socket.TCP_NODELAY, 1 )
                #self.harmonySkt.setsockopt( socket.IPPROTO_TCP, socket.TCP_PUSH, 1 )
                self.opened = True
                self.Send( "Register", None, "WebBrick interface V1.0,0" )
                self.Send( "RegisterNativeController", None, None )
                self._eventTgt.hConnected()
            except socket.error, err:
                logging.error( 'Harmony Open Error ' + str(err) )
        self.mut.unlock()

    # Process a message from Harmony
    def Process(self):
        logging.debug( 'Harmony Process' )
#        logging.debug( self.bufferedData )
        st = self.bufferedData.find( self.HARMONY_START )
        nd = self.bufferedData.find( self.HARMONY_END )
        #
        # There may be multiple messages in the received data or there may be an incomplete
        # HAPI message. REMEMBER TCP is a stream protocol and NOT a message passing protocol
        #
        while ( st != -1 ) and ( nd != -1 ) :
            if ( nd < st ):
                # strip up to st
                logging.debug( self.bufferedData )
                self.bufferedData = self.bufferedData[st:]
            else:
                # extract command and 
                self.ProcessCommand(self.bufferedData[st+self.HARMONY_START_LEN:nd] )
                self.bufferedData = self.bufferedData[nd+self.HARMONY_END_LEN:]
            st = self.bufferedData.find( self.HARMONY_START )
            nd = self.bufferedData.find( self.HARMONY_END )
        # handle contents of BufferedData
        # Locate <HAPI>
        # Locate </HAPI>
        # Extract and process.


    # Process a command from Harmony
    def ProcessCommand(self, cmdStr ):
        logging.debug( cmdStr )
        vals = string.split( cmdStr, ',', 2 )
#        for x in vals:
#            logging.debug( x )
        if ( len( vals ) >= 2 ):
            # vals[0] is common
            # vals[1] is command
            cmd = string.lower( vals[1] )
            if ( cmd == "heartbeat" ) :
                logging.debug( 'heartbeat' )
                self.Send( 'HeartBeat', None, None )
            elif ( cmd == "refreshproperties" ) :
                self._eventTgt.hRefreshProperties( vals[2] )
            elif ( cmd == "callmethod" ) :
                self._eventTgt.hCallMethod( vals[2] )
            elif ( cmd == "ident" ) :
                self._eventTgt.hIdent( vals[2] )
            elif ( cmd == "dumpeddata" ) :
                self._eventTgt.hDumpedData( vals[2] )
            else:
                logging.debug( 'unrecognised command' )
        else:
            logging.error( 'not enough parameters' )
            for x in vals:
                logging.error( x )
        
        
    # Send a message to Harmony.
    #
    #   Note if multiple parameters then include a comma between them in param
    #
    def Send(self, cmd, devName, param):
        fullMsg = self.HARMONY_START + self.DRIVER_NAME + ',' + cmd

        if ( devName != None ):
            fullMsg =  fullMsg + ',' + devName
        if ( param != None ):
            fullMsg =  fullMsg + ',' + param
        fullMsg = fullMsg + self.HARMONY_END
        logging.debug( 'Harmony Send ' + fullMsg )

        if not self.opened :
            self.Open()

        if self.opened :
            try:
                self.harmonySkt.send( fullMsg )
                time.sleep(0.1)
            except socket.error, err:
                logging.error( 'Harmony send error ' + str(err) )
                self.Close()
        else:
            # we failed to open Harmony
            logging.debug( 'Harmony not yet open' )
