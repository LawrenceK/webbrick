#
# Harmony to webbrick interface main code
#

import threading, logging, time, string

import Harmony, wbUdpEvents, wb6, HarmonyLogging

# ipAdr = '193.123.195.209'

#logging.basicConfig(level=logging.INFO,
#                    filename='./HarmonyMain.log',
#                    filemode='w',
#                    format='%(asctime)s %(levelname)s %(message)s')

#logging.basicConfig(level=logging.DEBUG,
#                    filename='./HarmonyMain.log',
#                    filemode='w',
#                    format='%(asctime)s %(levelname)s %(message)s',

class Zone:
    def __init__(self):
        self.name = None
        self.id = None
        self.zonetype = None

class Device:
    def __init__(self):
        self.name = None
        self.id = None
        self.address = None

class HarmonyMain( Harmony.harmonyEvents ):

    UNCONFIG_ZONE   = "Unconfigured WebBricks"

    def __init__(self):
        self.harmonyApi = Harmony.Harmony( self )
        self.wbUdpEvents = wbUdpEvents.wbUdpEvents( self )
        self.ZonesByName = {}
        self.ZonesById = {}
        self.DevicesByName = {}
        self.DevicesById = {}
    
    def alive(self):
        # are my deamon threads still running
        return self.harmonyApi.alive() and self.wbUdpEvents.alive()

    def start(self):
        self.harmonyApi.start()
        self.wbUdpEvents.startMonitor()
    
    def stop(self):
        self.harmonyApi.shutdown()
        self.wbUdpEvents.shutdown()
    
    def Connected(self):
        return self.harmonyApi.connected()
    
    def CreateZone(self,zoneName):
        if zoneName not in self.ZonesByName:
            self.ZonesByName[zoneName] = Zone()
            self.harmonyApi.createZone( zoneName )

    # Register a new Un configured webbrick.
    def NewUnconfiguredWebbrick( self, ipAdr ):
        wbname = "wb_"+ipAdr
        logging.info( 'Harmony Unconfigured Webbrick ' + wbname )
        self.CreateZone( self.UNCONFIG_ZONE )
        if wbname not in self.DevicesByName:
            self.DevicesByName[wbname] = Device()
            self.harmonyApi.registerDevice( self.UNCONFIG_ZONE, wbname )


    # Register a new configured webbrick.
    def NewWebbrick( self, zoneName, wName, ipAdr ):
        logging.info( string.join( ('Harmony NewWebbrick',zoneName,wName,ipAdr), ' ' ) )
        self.CreateZone( zoneName )

        # digital outputs
        for i in range(8):
            name = wName + "_Digital Out "+str(i+1)
            self.harmonyApi.registerDevice( zoneName, name, ipAdr, "DO"+str(i), "Appliance Module (wired)" )
            self.harmonyApi.registerMethod( name, "Toggle" )
            self.harmonyApi.registerMethod( name, "Dwell" )
            self.harmonyApi.registerMethod( name, "Dwell-Cancel" )
            self.harmonyApi.registerMethod( name, "Momentary" )
            self.harmonyApi.registerDiscrete( name, "On|Off|Toggle|Dwell|Dwell-Cancel|Momentary" )

        # analogue outputs
        for i in range(4):
            name = wName + "_Analogue Out "+str(i+1)
            self.harmonyApi.registerDevice( zoneName, name, ipAdr, "AA"+str(i), "Din Rail Dimmer" )
            disc = ""
            for j in range(8):
                self.harmonyApi.registerMethod( name, "preSet"+str(j) )
                disc = disc + "preSet"+str(j) + "|"
            disc = disc[:len(disc)-1]
            self.harmonyApi.registerDiscrete( name, disc )

        # digital inputs
        for i in range(8):
            name = wName + "_Switch"+str(i+1)
            self.harmonyApi.registerDevice( zoneName, name, ipAdr, "DI"+str(i), "Component" )
            self.harmonyApi.registerEvent( name, "Pushed" )
            self.harmonyApi.registerEvent( name, "Released" )

        # temperature sensors
        for i in range(5):
            name = wName + "_Temperature "+str(i+1)
            self.harmonyApi.registerDevice( zoneName, name, ipAdr, "TT"+str(i), "Component" )
            self.harmonyApi.registerEvent( name, "tooHot" )
            self.harmonyApi.registerEvent( name, "tooCold" )
            self.harmonyApi.registerProperty( name, "Temperature" )

        # analogue inputs
        for i in range(4):
            name = wName + "_Analogue In "+str(i+1)
            self.harmonyApi.registerDevice( zoneName, name, ipAdr, "AI"+str(i), "Component" )
            self.harmonyApi.registerEvent( name, "tooHigh" )
            self.harmonyApi.registerEvent( name, "tooLow" )
            self.harmonyApi.registerProperty( name, "Value" )

        # monitor inputs
        for i in range(4):
            name = wName + "_Monitor "+str(i+1)
            self.harmonyApi.registerDevice( zoneName, name, ipAdr, "MM"+str(i), "Component" )
            self.harmonyApi.registerEvent( name, "Alarmed" )
            self.harmonyApi.registerEvent( name, "Cleared" )

        #name = wName + "_Rotary "+str(i+1)
        #self.harmonyApi.registerDevice( zoneName, name, ipAdr, "RR"+str(i), "Component" )
        #self.harmonyApi.registerEvent( name, "Up" )
        #self.harmonyApi.registerEvent( name, "Down" )

    # get a harmony configuration table
    def GetDbTable( self, tableName ):
        self.harmonyApi.dumpDbTable( tableName )

    # 
    def LoadZones(self):
        self.ZonesByName = {}
        self.ZonesById = {}
        self.GetDbTable( 'Zones' )
        return

    def UpdateZone(self, fName, rowNr, value ):
        # Zones have a Name, ZoneType,ID,DefaultAddress
        if rowNr not in self.ZonesById:
            self.ZonesById[rowNr] = Zone()
            self.ZonesById[rowNr].id = rowNr

        if ( fName == "Name" ):
            logging.info( string.join( ('new Zone',rowNr,value ), ' ' ) )
            self.ZonesById[rowNr].name = value
            self.ZonesByName[value] = self.ZonesById[rowNr]
        elif ( fName == "ZoneType" ):
            self.ZonesById[rowNr].zonetype = value

        return

    def LoadDevices(self):
        self.DevicesByName = {}
        self.DevicesById = {}
        self.GetDbTable( 'Devices' )
        return

    def UpdateDevice(self, fName, rowNr, value ):
        # Devices have a ID, Name, DeviceType,DimLevel,IsOn,AllowManualUse,
        #       Zone,Address,Component,Discretes,Icon,Active
        if rowNr not in self.DevicesById:
            self.DevicesById[rowNr] = Device()
            self.DevicesById[rowNr].id = rowNr

        if ( fName == "Name" ):
            logging.info( string.join( ('new Device',rowNr,value ), ' ' ) )
            self.DevicesById[rowNr].name = value
            self.DevicesByName[value] = self.DevicesById[rowNr]
        elif ( fName == "Address" ):
            self.DevicesById[rowNr].address = value

        return

    # Handle Harmony message events.
    def hRefreshProperties( self, param ):
        logging.debug( 'RefreshProperties' )
        logging.debug( param )
        return

    def DigitalOutCall( self, cmd, adr, chn, level ):
        if cmd == "TurnOn":
            wb.DigOn( adr, chn )
        elif cmd == "TurnOff":
            wb.DigOff( adr, chn )
        elif cmd == "FadeTo":
            if ( level >= 50 ) :
                wb.DigOn( adr, chn )
            else:
                wb.DigOff( adr, chn )
        elif cmd == "On":
            wb.DigOn( adr, chn )
        elif cmd == "Off":
            wb.DigOff( adr, chn )
        elif cmd == "Toggle":
            wb.DigToggle( adr, chn )
        elif cmd == "Dwell":
            wb.DigDwell( adr, chn, 3 )
#        elif cmd == "Dwell-Cancel":
#            wb.DigToggle( adr, chn )
#        elif cmd == "Momentary":
#            wb.DigToggle( adr, chn )
        else:
            logging.error( string.join(("Unrecognised digital command", cmd, adr, chn, level) ) )

    def AnalogueOutCall( self, cmd, adr, chn, level ):
        if cmd == "TurnOn":
            wb.AnOutPercent(adr, chn, 100)
        elif cmd == "TurnOff":
            wb.AnOutPercent(adr, chn, 0)
        elif cmd == "FadeTo":
            wb.AnOutPercent(adr, chn, level)
        elif cmd == "On":
            wb.AnOutPercent(adr, chn, 100)
        elif cmd == "Off":
            wb.AnOutPercent(adr, chn, 0)
        elif cmd == "preSet0":
            wb.AnOutSp(adr, chn, 0)
        elif cmd == "preSet1":
            wb.AnOutSp(adr, chn, 1)
        elif cmd == "preSet2":
            wb.AnOutSp(adr, chn, 2)
        elif cmd == "preSet3":
            wb.AnOutSp(adr, chn, 3)
        elif cmd == "preSet4":
            wb.AnOutSp(adr, chn, 4)
        elif cmd == "preSet5":
            wb.AnOutSp(adr, chn, 5)
        elif cmd == "preSet6":
            wb.AnOutSp(adr, chn, 6)
        elif cmd == "preSet7":
            wb.AnOutSp(adr, chn, 7)
        else:
            logging.error( string.join(("Unrecognised analogue command", cmd, adr, chn, level), "," ) )
        
    def hCallMethod( self, param ):
        logging.info( 'CallMethod '+ param )
        vals = string.split( param, ',' )
        # vals[0] - name
        # vals[1] - command
        # vals[2] - name+Level
        # vals[3] - address
        if ( len(vals) == 4 ):
            level = vals[2][len(vals[0]):]
            adr = string.split( vals[3], ':' )
            # adr[0] - ip addr
            # adr[1] - channel id
            if ( len(adr) == 2 ):
                devType = adr[1][0:2]
                chn = adr[1][2:]
                if ( devType == "DO" ):
                    self.DigitalOutCall( vals[1], adr[0], chn, level )
                elif ( devType == "AA" ):
                    self.AnalogueOutCall( vals[1], adr[0], chn, level )
                elif ( devType == "AI" ):
                    logging.error( string.join( ('CallMethod device input only', devType, chn ), ' ' ) )
                elif ( devType == "DI" ):
                    logging.error( string.join( ('CallMethod device input only', devType, chn ), ' ' ) )
                elif ( devType == "MM" ):
                    logging.error( string.join( ('CallMethod device input only', devType, chn ), ' ' ) )
                elif ( devType == "TT" ):
                    logging.error( string.join( ('CallMethod device input only', devType, chn ), ' ' ) )
                else:
                    logging.error( string.join( ('CallMethod unrecognised device type', devType, chn ), ' ' ) )
                    for x in vals:
                        logging.error( x )
                    for x in adr:
                        logging.error( x )
            else:
                logging.error( 'CallMethod address field invalid '+ vals[3] )
                for x in adr:
                    logging.error( x )
                for x in vals:
                    logging.error( x )
        else:
            logging.error( 'CallMethod params invalid '+ param )
            for x in vals:
                logging.error( x )
        return

    def hIdent( self, param ):
        logging.info( 'ident '+param )
        return

    def hDumpedData( self, param ):
        # format is table name, field name, record Nr, Value. 
        # Note Value has a <CR><LF> at end of it. Hence we first split records by \r\n
        logging.debug( 'dumped data' )
        records = string.split( param, '\r\n' )
        for record in records:
            logging.debug( record )
            if len(record) > 0 :
                cols = string.split( record, ',' )
                if len(cols) > 3:
                    tname,fname,r,v = cols
                    if ( tname == "Zones" ) :
                        self.UpdateZone( fname, r, v )
                    elif ( tname == "Devices" ) :
                        self.UpdateDevice( fname, r, v )
        return

    def hConnected( self):
        logging.info( 'Connected' )
        self.LoadZones()
        self.LoadDevices()
        return

    def hDisconnected( self):
        logging.info( 'Disconnected' )
        return

    # handle WebBrick Udp Events
    def wbNewNode( self, event ):
        if self.Connected():
            self.NewUnconfiguredWebbrick( event.ipAdr() )
        return

    def wbStarted( self, event ):
        # Get the Xml
        # Set the time
        # Replace by nodeName from the Xml
        if self.Connected():
            zonename = "wb"+str(event.fromNode())
            wname = zonename
            if zonename not in self.ZonesByName:
                # not set up yet.
                self.NewWebbrick( zonename, wname, event.ipAdr() )
            # Get Harmony configuration and download what we can.
        return

    def wbReconfigured( self, event ):
        # get the Xml and verify configuration
        # should update names etc.
        return

    def wbAnLow( self, event ):
        if self.Connected():
            name = "wb"+str(event.fromNode()) + "_Analogue In "+str(event.srcChannel()+1)
            self.harmonyApi.sendTrigger( name, "tooLow" )
        return

    def wbAnHi( self, event ):
        if self.Connected():
            name = "wb"+str(event.fromNode()) + "_Analogue In "+str(event.srcChannel()+1)
            self.harmonyApi.sendTrigger( name, "tooHigh" )
        return

    def wbTempLow( self, event ):
        if self.Connected():
            name = "wb"+str(event.fromNode()) + "_Temperature "+str(event.srcChannel()+1)
            self.harmonyApi.sendTrigger( name, "tooCold" )
        return

    def wbTempHi( self, event ):
        if self.Connected():
            name = "wb"+str(event.fromNode()) + "_Temperature "+str(event.srcChannel()+1)
            self.harmonyApi.sendTrigger( name, "tooHot" )
        return

    def wbDigIn( self, event ):
        if self.Connected():
            name = "wb"+str(event.fromNode()) + "_Temperature "+str(event.srcChannel()+1)
            if ( event.action() == 2 ):
                logging.info( string.join( (name,'On'), ' ' ) )
                self.harmonyApi.sendTrigger( name, "Pushed" )
            else:
                logging.info( string.join( (name,'Off'), ' ' ) )
                self.harmonyApi.sendTrigger( name, "Released" )
        return

    def wbRemoteDigIn( self, event ):
        # this will be a dig trigger sent remotly so we ignore.
        return

    def wbScheduled( self, event ):
        # this will be a scheduled event trigger so we will not add to Harmony yet.
        return

    def wbAnIn( self, event ):
        # update property
        if self.Connected():
            name = "wb"+str(event.fromNode()) + "_Analogue In "+str(event.srcChannel()+1)
            self.harmonyApi.setProperty( name, "Value", str(event.val()) )
        return

    def wbTempIn( self, event ):
        # update property
        if self.Connected():
            name = "wb"+str(event.fromNode()) + "_Temperature "+str(event.srcChannel()+1)
            self.harmonyApi.setProperty( name, "Temperature", str(event.val()) )
        return

    def wbAnOut( self, event ):
        if self.Connected():
            name = "wb"+str(event.fromNode()) + "_Analogue Out "+str(event.srcChannel()+1)
            self.harmonyApi.sendStateLevel( name, event.val() )
        return

    def wbDigOut( self, event ):
        if self.Connected():
            name = "wb"+str(event.fromNode()) + "_Digital Out "+str(event.srcChannel()+1)
            if ( event.action() == 2 ):
                logging.info( string.join( (name,'On'), ' ' ) )
                self.harmonyApi.sendStateOn( name )
            else:
                logging.info( string.join( (name,'Off'), ' ' ) )
                self.harmonyApi.sendStateOff( name )
        return

    def wbMonitor( self, event ):
        if self.Connected():
            name = "wb"+str(event.fromNode()) + "_Monitor "+str(event.srcChannel()+1)
            if ( event.action() == 2 ):
                logging.info( string.join( (name,'Alarmed'), ' ' ) )
                self.harmonyApi.sendTrigger( name, "Alarmed" )
            else:
                logging.info( string.join( (name,'Cleared'), ' ' ) )
                self.harmonyApi.sendTrigger( name, "Cleared" )
        return

# ------- --------- --------- --------- --------- ---------
# Invoke main program
# ------- --------- --------- --------- --------- ---------

if __name__ == "__main__":
    HarmonyLogging.configure()
    Main = HarmonyMain()
    Main.start()
    while ( Main.alive() ):
        time.sleep( 1 )
