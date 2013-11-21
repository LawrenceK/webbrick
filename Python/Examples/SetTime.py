import sys
sys.path.append('../API')

import socket, time, logging, string, copy, datetime
import wb6, wbUdpEvents

password = "password"
timeout = 90

class setTime( wbUdpEvents.wbEvents ):

    def __init__ (self):
        self.timeVal = timeout
        self.wbCount = 0
        self.lastWebBrick = ""
        logging.info( 'SetTime' )

    def wbStarted( self, event ):
        if ( self.lastWebBrick != event.ipAdr() ):
            self.lastWebBrick = event.ipAdr()
            now = datetime.datetime.now()
            # synchronize to a minute.
            logging.info( string.join(('Setting Time for ', event.ipAdr(), ' Waiting for start of minute' ) ) )
            while ( now.second <> 0 ):
                time.sleep( 0.5 )
                now = datetime.datetime.now()
            # datetime starts on monday
            day = now.weekday() + 1
            if ( day == 7 ):
                day = 0
        
            logging.info( string.join(('Setting Time for ', event.ipAdr(), ' To ', str(day), " ", str(now.hour), ":", str(now.minute) ) ) )
            wb6.Login( event.ipAdr(), password );
            wb6.SetTime( event.ipAdr(), day, now.hour, now.minute )
    
            self.timeVal = timeout    # reset
        else:
            logging.info( string.join(('Duplicate ', event.ipAdr() ) ) )

def SetTime( args ):
    # WbCfg(sys.argv[1:])
    logging.info( "Starting" )
    setter = setTime()
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
    SetTime(sys.argv[1:])
