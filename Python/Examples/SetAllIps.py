import sys
sys.path.append('../API')

import socket, time, logging, string, copy
import wbUdp, wbUdpEvents

password = "password"
baseAddr = [10,100,100,200]
broadcastAddr = "255.255.255.255"
timeout = 30

class setIp( wbUdpEvents.wbEvents ):

    def __init__ (self):
        self.timeVal = timeout
        self.wbCount = 0
        self.macList = dict()
        logging.info( 'SetAllIpAddresses' )

    def wbNewNode( self, event ):
        mac = event.macAdr()
        macStr = string.join( (hex(mac[0]), hex(mac[1]), hex(mac[2]), hex(mac[3]), hex(mac[4]), hex(mac[5] )), ":" )
        if macStr in self.macList:
            newIp = self.macList[macStr]
        else:
            newIp = copy.deepcopy(baseAddr)
            newIp[3] = newIp[3] + self.wbCount
            self.macList[macStr] = newIp
        
        ipStr = string.join( (str(newIp[0]), str(newIp[1]), str(newIp[2]), str(newIp[3]) ), "." )
        if ( newIp[3] < 255 ):
            logging.info( string.join(('Setting Ip address for', macStr, event.ipAdr(), 'To', ipStr ), " " ) )
            self.login()
            self.setIp( mac, newIp )
            wbUdp.sendFrame( ipStr, ':SN;'+str(newIp[3])+':')
            wbUdp.sendFrame( ipStr, ':NN;WB_'+str(newIp[3])+':')
            self.wbCount = self.wbCount + 1
            self.timeVal = timeout    # reset
        
    def login(self):
        wbUdp.sendFrame( broadcastAddr, ':LG;'+password+':')
        

    def setIp(self, macAddr, ipAddr):
        cmd = ':SA'
        for b in macAddr:
            cmd = cmd + ';' + str(b)
        for b in ipAddr:
            cmd = cmd + ';' + str(b)
        cmd = cmd + ':'
        wbUdp.sendFrame( broadcastAddr, cmd )

def SetAllIpAddresses( args ):
    # WbCfg(sys.argv[1:])
    logging.info( "Starting" )
    setter = setIp()
    events = wbUdpEvents.wbUdpEvents( setter )
    events.startMonitor()
    while ( setter.timeVal > 0 ):
        logging.info( string.join(('running ', str(setter.timeVal) ) ) )
        setter.timeVal = setter.timeVal - 1
        time.sleep(1)
    events.shutdown()

# ------- --------- --------- --------- --------- ---------
# Invoke main program
# ------- --------- --------- --------- --------- ---------

if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)
    SetAllIpAddresses(sys.argv[1:])
