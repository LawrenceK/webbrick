#
#  Class to do factory acceptance testing of a web brick.
#
#  Lawrence Klyne
#
# Missing test of rotary encoder
# Test of temp sensors
# test of mains triacs.
# Mains/temp should be one test, use sensors to detect mains light on.
#
# Added retry on DIgIn reading of status.
#
#   Refactored to use unit test framework to manage the individual tests.
#
import logging, os, sys, time, datetime, string, unittest

sys.path.append("../API")

# From API
import wb6
import wbUdpEvents2
from wb6Status import wb6Status
from wb6Config import wb6Config

# The IP address we expect the webbrick under test to be on.
underTestAddress  = "10.100.100.100"

doTestWb7  = False

#LpkTestEnv = True
LpkTestEnv = False

FirstTest = True
# Numbering in pyserial is from 0 = COM1

useSerialForRs485  = False  # Test rig use False here
# The IP address for the control webbrick
controlWbAddress  = "10.100.100.250"

underTestPassword  = "installer"
controlPassword  = "installer"

# switches relay over
CONTROL_SERIAL_RELAY    = 0

# event target
# add filter ability.
#class myTarget:
class myTarget( wbUdpEvents2.wbEventTarget ):
    def __init__(self, filter=None):
        logging.info( 'myTarget for UDP events' )
        self._testWb = list()
        self._controlWb = list()
        self._filter = filter

    def handleEvent( self, event ):
        # filter out what we need
        if self._filter == None or self._filter == event.type() :

            if event.other_data()["ipAdr"] == underTestAddress:
                if not self._testWb or self._testWb[-1].other_data()["seqNr"] != event.other_data()["seqNr"]:
                    logging.debug( 'From Under Test %s ' % (event) )
                    self._testWb.append( event )
                else:
                    logging.debug( 'Duplicate %s ' % (event) )
            elif event.other_data()["ipAdr"] == controlWbAddress:
                if not self._controlWb or self._controlWb[-1].other_data()["seqNr"] != event.other_data()["seqNr"]:
                    logging.debug( 'From Control %s ' % (event) )
                    self._controlWb.append( event )
                else:
                    logging.debug( 'Duplicate %s ' % (event) )
            else:
                logging.debug( 'Not webbricks involved in test %s ' % (event) )
        else:
            logging.debug( 'filtered Out %s ' % (event) )

#
class wbFactoryTest(unittest.TestCase):

    def setUp(self):
        # Called before every test
        self.testStartAt = time.time()
        self.ErrorCount = 0
        self.ErrorMsgs = list()
        self.MimicErrorCount = 0
        self.DigitalErrorCount = 0
        self.testName = ""
        self.newTest = False
        # flush ARP cache, this does iot every test
        os.system( "arp -d %s" % underTestAddress )
#        os.system( "arp -d *" )

        self.doLogin(underTestAddress)
        self.doLogin(controlWbAddress)
        time.sleep( 0.1 )
        wb6.Send(underTestAddress,'SO1;1')   # disable UDP events for analigue etc
#        wb6.Send(underTestAddress,'SO2;1')   # Read continuous on analogue inputs.
        wb6.Send(controlWbAddress,'SO1;1')   # disable UDP events for analigue etc
        wb6.Send(controlWbAddress,'SO2;1')   # Read continuous on analogue inputs.

        # ensure relay connected for RS232
        wb6.DigOff( controlWbAddress, CONTROL_SERIAL_RELAY )
        
        wb6.SendHTTP( underTestAddress,"CR2" ) # RS232
        wb6.SendHTTP( controlWbAddress,"CR2" ) # RS232
        
        # we only need to reset all actions on first test of run.
        global FirstTest
        if FirstTest:
            wb6.Send(underTestAddress,'RT')   # rescan temp bus
            self.disableWebBrickActions(underTestAddress)
            self.disableWebBrickActions(controlWbAddress)
            FirstTest = False

        self.tgt = None
        self.udp = None
#        print( "Test Setup %f seconds" % (time.time() - self.testStartAt) )

        return

    def tearDown(self):
        # Called after every test.
        for msg in self.ErrorMsgs:
            logging.error( msg )
            
        if self.udp:
            self.udp.shutdown()
            self.udp = None
            self.tgt = None
            time.sleep(2)
        print( "Test Elapsed %f seconds" % (time.time() - self.testStartAt) )
        return

    def disableWebBrickActions(self, ipAdr):
        for idx in range(0, 12 ):
            wb6.ConfigDigIn(ipAdr, idx, "D", 0, 0, 0, 0, 0, 0 )
            time.sleep(0.02)
#        wb6.ConfigFadeRate(ipAdr, 1 )
#        for idx in range(0,8):
#            wb6.DigOff( ipAdr, idx )

    def showWorking(self):
        sys.stdout.write( "." )
        
    def doLogin(self, ipAdr = None ):
        if ipAdr:
            wb6.Login( ipAdr, "installer" );
        else:
            wb6.Login( underTestAddress, "installer" );
        time.sleep( 0.1 )
        
    def setTest(self, name):
        self.testName = name
        logging.info( self.testName )
        self.newTest = False
        
    def error(self, msg):
        if self.newTest:
#            logging.error( self.testName )
            self.ErrorMsgs.append(self.testName)
            self.newTest = False
#        logging.error( msg )
        self.ErrorMsgs.append( msg )
        self.ErrorCount +=1

    def loadWbStatus( self, addr ):
        sts = wb6Status(addr)
        # check site player not reset
        self.assertNotEqual( sts.getOpState(), 255 )
        return sts
            
    # Digital In and Digital outs are connected together so testing can be simple.
    def testADigitalChannel( self, chn ):
        # switch all off
        for idx in range(0,8):
            if ( idx == chn ):
                wb6.DigOn( underTestAddress, chn )
            else:
                wb6.DigOff( underTestAddress, idx )

        self.setTest( 'On test Digital channel ' + str(chn) + ' ip address ' + underTestAddress )

        time.sleep(0.1)
        # first read the status XMl until we get what we expect
        # both the control and under test webbrick
        repeat = 0
        while repeat < 3:
            sts = self.loadWbStatus(underTestAddress)
            sts2 = self.loadWbStatus(controlWbAddress)
            if sts2.getDigIn(chn):
                # mimic correct
                if ( chn <= 3 ) and not sts.getDigIn(idx):
                    # OC correct
                    break
                if ( chn > 3 ) and sts.getDigIn(idx):
                    # DigOut correct
                    break
            repeat = repeat + 1
            time.sleep(0.5)

        # verify only channel under test on
        for idx in range(0,8):
            # if the same
            if ( idx > 3 ):
                if ( idx == chn ):
                    if not sts.getDigIn(idx):
                        self.error( 'channel ' + str(chn) + ' Should be On' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
                else:
                    if sts.getDigIn(idx):
                        self.error( 'channel ' + str(idx) + ' Should be Off' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
            else:
                if ( idx == chn ):
                    if sts.getDigIn(idx):
                        self.error( 'channel ' + str(chn) + ' Should be On' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
                else:
                    if not sts.getDigIn(idx):
                        self.error( 'channel ' + str(idx) + ' Should be Off' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1

            if ( idx == chn ):
                if not sts2.getDigIn(idx):
                    self.error( 'channel ' + str(chn) + ' Mimic Should be On' )
                    self.MimicErrorCount = self.MimicErrorCount + 1
            else:
                if sts2.getDigIn(idx):
                    self.error( 'channel ' + str(idx) + ' Mimic Should be Off' )
                    self.MimicErrorCount = self.MimicErrorCount + 1

        # verify relays
        firstRelay = 8
        if doTestWb7:
            firstRelay = 9
        for idx in range(firstRelay,12):
            if (idx/2) == chn :
                if not sts.getDigIn(idx):
                    self.error( 'test Relay channel ' + str(chn) + 'Monitor ' + str(idx) + ' High' )
                    self.DigitalErrorCount = self.DigitalErrorCount + 1
            else:
                if sts.getDigIn(idx):
                    self.error( 'test Relay channel ' + str(chn) + 'Monitor ' + str(idx) +  ' Low' )
                    self.DigitalErrorCount = self.DigitalErrorCount + 1

        # switch all on
        self.setTest( 'Off test Digital channel ' + str(chn) + ' ip address ' + underTestAddress )

        for idx in range(0,8):
            if ( idx == chn ):
                wb6.DigOff( underTestAddress, chn )
            else:
                wb6.DigOn( underTestAddress, idx )
        time.sleep(0.1)

        # read the status XMl until we get what we expect
        # both the control and under test webbrick
        repeat = 0
        while repeat < 3:
            sts = self.loadWbStatus(underTestAddress)
            sts2 = self.loadWbStatus(controlWbAddress)
            if not sts2.getDigIn(chn):
                # mimic correct
                if ( chn <= 3 ) and sts.getDigIn(idx):
                    # OC correct
                    break
                if ( chn > 3 ) and not sts.getDigIn(idx):
                    # DigOut correct
                    break
            repeat = repeat + 1
            time.sleep(0.5)

        # verify only channel under test off
        for idx in range(0,8):
            # if the same
            if ( idx > 3 ):
                if ( idx == chn ):
                    if sts.getDigIn(idx):
                        self.error( 'channel ' + str(chn) +' Should be Off' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
                else:
                    if not sts.getDigIn(idx):
                        self.error( 'channel ' + str(idx) + ' Should be On' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
            else:
                if ( idx == chn ):
                    if not sts.getDigIn(idx):
                        self.error( 'channel ' + str(chn) +' Should be Off' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
                else:
                    if sts.getDigIn(idx):
                        self.error( 'channel ' + str(idx) + ' Should be On' )
                        self.MimicErrorCount = self.DigitalErrorCount + 1
            if ( idx == chn ):
                if sts2.getDigIn(idx):
                    self.error( 'channel ' + str(chn) + ' Mimic Should be Off' )
                    self.MimicErrorCount = self.MimicErrorCount + 1
            else:
                if not sts2.getDigIn(idx):
                    self.error( 'channel ' + str(idx) + ' Mimic Should be On' )
                    self.MimicErrorCount = self.MimicErrorCount + 1

        # verify relays again
        for idx in range(firstRelay,12):
            if (idx/2) == chn :
                if sts.getDigIn(idx):
                    self.error( 'test Relay channel ' + str(chn) + 'Monitor ' + str(idx) + ' High' )
                    self.DigitalErrorCount = self.DigitalErrorCount + 1
            else:
                if not sts.getDigIn(idx):
                    self.error( 'test Relay channel ' + str(chn) + 'Monitor ' + str(idx) +  ' Low' )
                    self.DigitalErrorCount = self.DigitalErrorCount + 1
                    
    def testAn( self, chn ):
        self.setTest( 'test Analogue channel ' + str(chn) + ' ip address ' + underTestAddress )
        # for analogue level from 0 to 100% in 10% steps.

        # We are going to stop at 80 % due to power supply problems limiting an out 
        for i in range(0,81,20):
            # set all channels to this level.
            for j in range(0, 4):
                if j != chn:
                    wb6.AnOutPercent( underTestAddress, j, i )
                else:
                    wb6.AnOutPercent( underTestAddress, j, 0 )
            time.sleep(0.4) # let settle, possibly long Fade time
            # for channel under analogue level from 0 to 100% in 10% steps.
            for k in range(0, 81, 20):
                self.showWorking()
                wb6.AnOutPercent( underTestAddress, chn, k )
                time.sleep(0.2) # Fade time
                sts = self.loadWbStatus(underTestAddress)
                for j in range(0, 4):
                    val = sts.getAnIn(j)
                    if j == chn :
                        # should be k
                        if ( val < ( k - 5 ) ) or ( val > ( k + 5 ) ) :
                            self.error( 'test Analogue channel %i expecting %s got %s' % (chn, k, val) )
                    else:
                        # should be i
                        if ( val < ( i - 5 ) ) or ( val > ( i + 5 ) ) :
                            self.error( 'test Analogue channel %i verify %i expecting %s got %s' % (chn, j, i, val) )
                    
    def testAnFast( self, chn ):
        self.setTest( 'test Analogue channel ' + str(chn) + ' ip address ' + underTestAddress )
        # for analogue level from 0 to 100% in 10% steps.

        # set all channels to 50 %
        wb6.AnOutPercent( underTestAddress, 0, 0 )
        for j in range(0, 4):
            if j != chn:
                wb6.AnOutPercent( underTestAddress, j, 50 )
            
        #time.sleep(0.4) # let settle, possibly long Fade time
        # for channel under analogue level from 0 to 100% in 10% steps.
        for k in range(0, 81, 20):
            self.showWorking()
            wb6.AnOutPercent( underTestAddress, chn, k )
#            time.sleep(0.2) # Fade time
            time.sleep(0.1) # Fade time
            sts = self.loadWbStatus(underTestAddress)
            for j in range(0, 4):
                val = sts.getAnIn(j)
                if j == chn :
                    # should be k
                    if ( val < ( k - 5 ) ) or ( val > ( k + 5 ) ) :
                        self.error( 'test Analogue channel %i expecting %s got %s' % (chn, k, val) )
                else:
                    # should be i
                    if ( val < ( 45 ) ) or ( val > ( 55 ) ) :
                        self.error( 'test Analogue channel %i verify %i expecting %s got %s' % (chn, j, 50, val) )

    # NOW THE TESTS

    def factoryReset(self):
        logging.info( "factoryReset" )
        # send factory reset, full reset
        self.doLogin(underTestAddress)
        wb6.Send(underTestAddress, "FR1" )
        for idx in range(0,8):
            self.showWorking()
            time.sleep(1.0 )# takes a while.
#        wb6.Send(underTestAddress, "RB" )
#        time.sleep(5.0)
        self.assertEqual( self.ErrorCount, 0 )
    
    def setConfiguration(self):
        logging.info( "setConfiguration" )
        wb6.SendHTTP( underTestAddress,"SM0;63;1;0" )    # make sure Mimics full off or full on and fade fast
        self.disableWebBrickActions(underTestAddress)

        wb6.ConfigFadeRate(underTestAddress, 1 )
        for idx in range(0,8):
            wb6.DigOff( underTestAddress, idx )
        # force a change that Wb will see.
        wb6.DigOn( underTestAddress, 1 )
        wb6.DigOff( underTestAddress, 1 )
        self.assertEqual( self.ErrorCount, 0 )
        
    def doSetClock(self):
        # synchronize to a minute.
        logging.info( string.join(('Setting Time for ', underTestAddress, ' Waiting for start of minute' ) ) )
        now = datetime.datetime.now()
        while ( now.second <> 0 ):
            self.showWorking()
            time.sleep( 0.5 )
            now = datetime.datetime.now()
            
        # datetime starts on monday
        day = now.weekday() + 1
        if ( day == 7 ):
            day = 0
        logging.info( 'Setting Time for %s To %u, %02u:%02u:00' % (underTestAddress, day, now.hour,now.minute) )
#        logging.info( string.join(('Setting Time for ', underTestAddress, ' To ', str(day), " ", str(now.hour), ":", str(now.minute) ) ) )
        wb6.SetTime( underTestAddress, day, now.hour, now.minute )
        self.assertEqual( self.ErrorCount, 0 )

    def expectMimicVolts(self, lo, hi):
        # retries Mimic read until in range
        rdLimit = 5
        val = 0

        while rdLimit > 0:
            sts2 = self.loadWbStatus(controlWbAddress)
            val = sts2.getAnIn(0)
            if ( val >= lo ) and ( val <= hi ) :
                break
            time.sleep(1.0)
            rdLimit = rdLimit - 1

        if ( val < lo ) or ( val > hi ) :
            self.error( 'test Mimic volts got %s expecting between %s and %s' % (val, lo, hi) )

    def testMimicVolts(self):
        # default should be five volts
        wb6.SendHTTP( underTestAddress,"SM0;63;1;0" )    # make sure Mimics full off or full on and fade fast & 5V

        wb6.DigOn( underTestAddress, 0 )
        self.expectMimicVolts(20, 40)
        
        wb6.SendHTTP( underTestAddress,"SM0;63;1;1" )    # make sure Mimics full off or full on and fade fast & 12V
        self.expectMimicVolts(55, 90)   # 55 in case running off 12V instead of 13.8V

        wb6.SendHTTP( underTestAddress,"SM0;63;1;0" )    # make sure Mimics full off or full on and fade fast & 5V
        wb6.DigOff( underTestAddress, 0 )

        self.assertEqual( self.ErrorCount, 0 )

    def testDigitals(self):
        # Combine digital and mimic tests
        # include triac tests
        
        # get current temps  for main lamps.
        triacMap = (1,2,3,4)    # maps triac index to temp index.
        
        savedTemps = list()
        sts2 = self.loadWbStatus(controlWbAddress)
        owSts = sts2.getOneWireBus()
        for idx in range(0,4):
            tidx = triacMap[idx]
            if ( owSts & (0x01 << tidx )) == 0 :
                self.error( 'Not seeing temperature Triac/Lamp: %i (%i)' % (idx, tidx) )
                savedTemps.append( 0 )
            else:
                savedTemps.append( sts2.getTemp( tidx ) )

        wb6.SendHTTP( underTestAddress,"SM0;63;1;0" )    # make sure Mimics full off or full on and fade fast
        # tests digital in 0-7 and digital out 0-7/mimics 0-7. The later depends on the conenction to the inputs
        for idx in range(0,8):
            self.testADigitalChannel( idx )

        # leave all on for soak test
        for idx in range(0,8):
            wb6.DigOn( underTestAddress, idx )
            
        
        # get current temps.
        # see whether mains lamps switched on during this test run.
        # they should of generated enough of a temperature rise to be detected.
        sts2 = self.loadWbStatus(controlWbAddress)
        owSts = sts2.getOneWireBus()
        
        for idx in range(0,4):
            tidx = triacMap[idx]
            if ( owSts & (0x01 << tidx )) != 0 :
                # check we had a sensor.
                tv = sts2.getTemp( tidx )
                if ( savedTemps[idx] + 3 ) > tv:
                    self.error( 'Not seeing temperature rise for Triac/Lamp: %i Old Temp %s new Temp %s' % (idx, savedTemps[idx],tv) )
                    
        self.assertEqual( self.DigitalErrorCount, 0 )
        self.assertEqual( self.MimicErrorCount, 0 )
        self.assertEqual( self.ErrorCount, 0 )

    def testTemperature(self):
        # test analogue out and inputs
        sts = self.loadWbStatus(underTestAddress)
        if ( sts.getOneWireBus() != 1 ):
            # rescan bus
            wb6.Send(underTestAddress,'RT')   # rescan temp bus
        time.sleep(1.0)
        sts = self.loadWbStatus(underTestAddress)
        if ( sts.getOneWireBus() != 1 ):
            self.error( 'invalid temp sensor count : ' + str(sts.getOneWireBus()) )
        else:
            idx = 0
            tv = sts.getTemp( idx )
            if ( tv > 125.0 ) or ( tv < -55.0 ) :
                self.error( 'invalid temp reading : ' + str(tv) )
        self.assertEqual( self.ErrorCount, 0 )
            
    def testTriacs(self):
        # need better test rig for this. 
        # e.g. temp sensors on mains lamps.

        # first read temp sensors
        savedTemps = list()
        sts = self.loadWbStatus(underTestAddress)
        sts2 = self.loadWbStatus(controlWbAddress)
       
        for idx in range(0,5):
            savedTemps.append( sts2.getTemp( idx ) )

        for idx in range(0,4):
            wb6.DigOn( underTestAddress, idx )
            time.sleep(10)
            #sts = self.loadWbStatus(underTestAddress)
            sts2 = self.loadWbStatus(controlWbAddress)
            tv = sts2.getTemp( idx+1 )
            if ( savedTemps[idx+1] + 3 ) > tv:
                time.sleep(10)
                sts2 = self.loadWbStatus(controlWbAddress)
                tv = sts2.getTemp( idx+1 )
                if ( savedTemps[idx+1] + 3 ) > tv:
                    self.error( 'Not seeing temperature rise for Triac/Lamp: %i Old Temp %s new Temp %s' % (idx, savedTemps[idx],tv) )
                
            wb6.DigOff( underTestAddress, idx )

        self.assertEqual( self.ErrorCount, 0 )

    def captureEvents(self, filter="binary/1"):
        try:
            time.sleep(1.0)
            self.tgt = myTarget( filter )   # serial rx
            self.udp = wbUdpEvents2.wbUdpEvents2( self.tgt )
            self.udp.startMonitor()
        except Exception,ex :
            self.error( "failed to start event monitor up %s" % (ex) )

    def clearEvents(self):
        self.tgt._testWb = list()
        self.tgt._controlWb = list()
    
    def getSerialData(self, lst):
        # should be 26 characters
        s = ""
        idx = 0
        limit = 10
        while len(s) < 26 and limit > 0:
            time.sleep(0.2)
            while idx < len(lst):
                s = s + lst[idx].other_data()["data"]
                idx = idx + 1
            limit = limit - 1
        return s
    
    def getSerialControlWb(self):
        return self.getSerialData(self.tgt._controlWb)

    def getSerialUnderTestWb(self):
        return self.getSerialData(self.tgt._testWb)

    def sendSerialWb(self, addr):
        time.sleep(0.5)
        for i in range( ord('a'), ord('z')+1-6, 6 ):
            wb6.SendHTTP(addr,"RD%s;%s;%s;%s;%s;%s" % (i, i+1, i+2, i+3, i+4, i+5)  )
            time.sleep(0.5)
        i = i + 6
        wb6.SendHTTP(addr,"RD%s;%s" % (i, i+1)  )
        time.sleep(0.1)
        
    def verifySerialPrim(self, addr, rx):

        self.sendSerialWb(addr)

        # should be 26 characters
        s = rx()
        if len(s) != 26:
            self.error( 'Received data length wrong (%u) : %s' % (len(s), s) )
        if s != "abcdefghijklmnopqrstuvwxyz":
            self.error( 'Received data wrong (%u) : %s' % (len(s), s) )

    def verifySerial(self, addr, rx):
        for idx in range(0,2):
            self.ErrorCount = 0
            # clear the list of events
            self.clearEvents()
            self.verifySerialPrim(addr, rx)
            if self.ErrorCount == 0:
                break
        
    def testSerial(self):
        self.captureEvents()
        # RS232
        self.verifySerial(underTestAddress, self.getSerialControlWb)
        self.verifySerial(controlWbAddress, self.getSerialUnderTestWb)
        # RS485
        wb6.DigOn( controlWbAddress, CONTROL_SERIAL_RELAY )
        wb6.SendHTTP( underTestAddress,"CR4" ) # RS485
        wb6.SendHTTP( controlWbAddress,"CR4" ) # RS485
        wb6.SendHTTP( underTestAddress,"RI" ) # RS485 input
        wb6.SendHTTP( controlWbAddress,"RI" ) # RS485 input
        
        wb6.SendHTTP( underTestAddress,"RO" ) # RS485 output
        self.verifySerial(underTestAddress, self.getSerialControlWb)
        wb6.SendHTTP( underTestAddress,"RI" ) # RS485 input
        
        wb6.SendHTTP( controlWbAddress,"RO" ) # RS485 output
        self.verifySerial(controlWbAddress, self.getSerialUnderTestWb)
        wb6.SendHTTP( controlWbAddress,"RI" ) # RS485 input
        
        wb6.DigOff( controlWbAddress, CONTROL_SERIAL_RELAY )
        self.assertEqual( self.ErrorCount, 0 )
    
    def testSerial232OutOfWebBrick(self):
        self.captureEvents()
        
        self.verifySerial(underTestAddress, self.getSerialControlWb)

        self.assertEqual( self.ErrorCount, 0 )
            
    def testSerial232InToWebBrick(self):
        self.captureEvents()

        self.verifySerial(controlWbAddress, self.getSerialUnderTestWb)

        self.assertEqual( self.ErrorCount, 0 )
            
    def testSerial485OutOfWebBrick(self):
        wb6.DigOn( controlWbAddress, CONTROL_SERIAL_RELAY )
        wb6.SendHTTP( underTestAddress,"CR4" ) # RS485
        wb6.SendHTTP( controlWbAddress,"CR4" ) # RS485
        wb6.SendHTTP( controlWbAddress,"RI" ) # RS485 input
        wb6.SendHTTP( underTestAddress,"RO" ) # RS485 output

        self.captureEvents()

        self.verifySerial(underTestAddress, self.getSerialControlWb)

        wb6.SendHTTP( underTestAddress,"RI" ) # RS485 input
        wb6.DigOff( controlWbAddress, CONTROL_SERIAL_RELAY )

        self.assertEqual( self.ErrorCount, 0 )
            
    def testSerial485InToWebBrick(self):
        # ensure relay connected for RS232
        wb6.DigOn( controlWbAddress, CONTROL_SERIAL_RELAY )
        
        wb6.SendHTTP( underTestAddress,"CR4" ) # RS485
        wb6.SendHTTP( controlWbAddress,"CR4" ) # RS485
        wb6.SendHTTP( underTestAddress,"RI" ) # RS485 input
        wb6.SendHTTP( controlWbAddress,"RO" ) # RS485 output

        self.captureEvents()

        self.verifySerial(controlWbAddress, self.getSerialUnderTestWb)

        wb6.SendHTTP( controlWbAddress,"RI" ) # RS485 output
        wb6.DigOff( controlWbAddress, CONTROL_SERIAL_RELAY )

        self.assertEqual( self.ErrorCount, 0 )
 
    def testAnalogue(self):
        # test analogue out and inputs
        wb6.Send(underTestAddress,'SO2;1')   # Read continuous on analogue inputs.
        wb6.Send(underTestAddress,'SF1')   # fast fade rate for analogue outputs
        for idx in range(0,4):
            self.testAnFast( idx )
        self.assertEqual( self.ErrorCount, 0 )
 
    def testAnalogueSlow(self):
        # test analogue out and inputs
        wb6.Send(underTestAddress,'SO2;1')   # Read continuous on analogue inputs.
        wb6.Send(underTestAddress,'SF1')   # fast fade rate for analogue outputs
        for idx in range(0,4):
            self.testAn( idx )
        self.assertEqual( self.ErrorCount, 0 )
 
    def checkTimePrim(self ):
        result = list()
        now = datetime.datetime.now()
        
        # Hold off at minute boundary.
        while ( now.second < 2 ):
            self.showWorking()
            time.sleep(1)
            now = datetime.datetime.now()
        # verify.
        sts = self.loadWbStatus(underTestAddress)
        ts = sts.getTime()
        ms = '%02u:%02u:00' % (now.hour,now.minute)
        me = '%02u:%02u:59' % (now.hour,now.minute)
        if (ms > ts) or (ts > me):
            result.append( "checkTime wbts: %s ms: %s me: %s" % (ts,ms,me) )
        # webbrick counts sunday as 0, python monday as 0 (sun=6)
        day = sts.getDay()
        if ( day == 0 ):
            day = 6
        else:
            day = day - 1
        if day != now.weekday():
            result.append( 'checkTime  wbday: (%u)%u local %u' % (sts.getDay(), day, now.weekday())  )
        return result

    def checkTime(self ):
        self.setTest( 'checkTime ' )
        idx = 0
        result = self.checkTimePrim()
        while idx < 10 and len(result) > 0:
            idx = idx + 1
            self.showWorking()
            time.sleep(1)# allow WB to read RTC on 5 sec boundary after boot.
            result = self.checkTimePrim()
        for s in result:
            self.error(s)
        self.assertEqual( self.ErrorCount, 0 )

    def showMacAddress(self ):
        cfg = wb6Config(underTestAddress)
        print cfg.getMacAddress()
        
def getTestSuiteFull():
    suite = unittest.TestSuite()
# Thesse first two get us in a known state
# The digital inputs are set to no actions.
    suite.addTest(wbFactoryTest("factoryReset"))
    suite.addTest(wbFactoryTest("setConfiguration"))
    suite.addTest(wbFactoryTest("doSetClock"))
    suite.addTest(wbFactoryTest("testMimicVolts"))  # before leaving digitals fully on.
    suite.addTest(wbFactoryTest("testDigitals"))
    suite.addTest(wbFactoryTest("testTemperature"))
    suite.addTest(wbFactoryTest("testAnalogue"))    # analogue out connected to analogue in.
    suite.addTest(wbFactoryTest("testSerial"))
#    suite.addTest(wbFactoryTest("testSerial232OutOfWebBrick"))
#    suite.addTest(wbFactoryTest("testSerial232InToWebBrick"))
#    suite.addTest(wbFactoryTest("testSerial485OutOfWebBrick"))
#    suite.addTest(wbFactoryTest("testSerial485InToWebBrick"))
    suite.addTest(wbFactoryTest("factoryReset"))    # leave in known state
    suite.addTest(wbFactoryTest("checkTime"))    # RTC reloaded after reset
    suite.addTest(wbFactoryTest("showMacAddress"))
    return suite
            
if __name__ == "__main__":
    startAt = time.time()

    if len(sys.argv) > 1:
#        logging.basicConfig(level=logging.DEBUG)
        logging.basicConfig(level=logging.INFO)
        tests = wbFactoryTest( sys.argv[1] )
    else:
#        logging.basicConfig(level=logging.DEBUG)
        logging.basicConfig(level=logging.ERROR)
        tests = getTestSuiteFull()

    runner = unittest.TextTestRunner(verbosity=2)
    runner.run(tests)
    endAt = time.time()
    logging.info( "Elapsed %f seconds" % (endAt - startAt) )
