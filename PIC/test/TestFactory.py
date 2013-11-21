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
import logging, sys, time, datetime,string

sys.path.append("../../Python/API")

import unittest

import wb6
from wb6Status import wb6Status

from TestUtils import *
from TestCfg import *

# This feels as if this needs further generalization and the event attributes should be dictionaried.

#
# A template event handler for inbound webbrick packets
# This is just a place holder, the event parameter will implement the wbEvent class that follows this.
#
class TestFactory(unittest.TestCase):

    def setUp(self):
        self.ErrorCount = 0
        self.testName = ""
        self.newTest = False
        return

    def tearDown(self):
        return

    def checkCmdStatus(self):
        sts = wb6Status( TestWbConfig._wbAddress )
        self.assertEqual( sts.getError(), 0 )

    def testFactoryReset(self):
        factoryReset(TestWbConfig._wbAddress)
        return
    
    def testSetConfiguration(self):
        doLogin( TestWbConfig._wbAddress )
        for idx in range(0, 11 ):
            wb6.ConfigDigIn(TestWbConfig._wbAddress, idx, "D", 0, 0, 0, 0, 0, 0 )
            time.sleep(0.02)
            self.checkCmdStatus()
        wb6.ConfigFadeRate(TestWbConfig._wbAddress, 1 )
        self.checkCmdStatus()
        for idx in range(0,8):
            wb6.DigOff( TestWbConfig._wbAddress, idx )
            self.checkCmdStatus()
        # force a change that Wb will see.
        wb6.DigOn( TestWbConfig._wbAddress, 1 )
        wb6.DigOff( TestWbConfig._wbAddress, 1 )

    def testSetClock(self):
        # synchronize to a minute.
        logging.debug( string.join(('Setting Time for ', TestWbConfig._wbAddress, ' Waiting for start of minute' ) ) )
        now = datetime.datetime.now()
        while ( now.second <> 0 ):
            showWorking()
            time.sleep( 0.5 )
            now = datetime.datetime.now()
            
        # datetime starts on monday
        day = now.weekday() + 1
        if ( day == 7 ):
            day = 0
        doLogin( TestWbConfig._wbAddress )
        logging.debug( 'Setting Time for %s To %u, %02u:%02u:00' % (TestWbConfig._wbAddress, day, now.hour,now.minute) )
#        logging.debug( string.join(('Setting Time for ', TestWbConfig._wbAddress, ' To ', str(day), " ", str(now.hour), ":", str(now.minute) ) ) )
        wb6.SetTime( TestWbConfig._wbAddress, day, now.hour, now.minute )
        self.checkCmdStatus()
        
    def setTest(self, name):
        self.testName = name
        self.newTest = False
        
    def error(self, msg):
        if self.newTest:
            logging.error( self.testName )
            self.newTest = False
        logging.error( msg )
        self.ErrorCount +=1
#        Py_Exit(1)

    # Digital In and Digital outs are connected together so testing can be simple.
    def doTestOneDin( self, chn ):
        # switch all off
        isMixed = 2
        isOc = False

        while ( isMixed > 0 ):
            for idx in range(0,8):
                wb6.DigOff( TestWbConfig._wbAddress, idx )
                time.sleep(1.0)
            
            # See whether connected to OC outputs which invert.
            sts = wb6Status(TestWbConfig._wbAddress)

            if (sts.getDigIn(0) or sts.getDigIn(1) or sts.getDigIn(2) or sts.getDigIn(3)):
                # Some high
                isOc = True
                logging.debug( "Testing OC outputs" )
                if (sts.getDigIn(0) and sts.getDigIn(1) and sts.getDigIn(2) and sts.getDigIn(3)):
                    isMixed = 0
                else:
                    self.error( 'test Digital channel ' + str(chn) + ' D0-D3 mixed results' )
                    isMixed = isMixed - 1
            else:
                logging.debug( "Testing MIMICS" )
                isMixed = 0
            # print sts.getDigIn(0), sts.getDigIn(1), sts.getDigIn(2), sts.getDigIn(3), isOc
         
        self.setTest( 'On test Digital channel ' + str(chn) + ' ip address ' + TestWbConfig._wbAddress )
        # switch on under test channel
        wb6.DigOn( TestWbConfig._wbAddress, chn )
        time.sleep(1.0)
        sts = wb6Status(TestWbConfig._wbAddress)
        # verify only channel under test on
        for idx in range(0,8):
            # if the same
            if ( idx > 3 ) or not isOc:
                if ( idx == chn ):
                    if not sts.getDigIn(idx):
                        sts = wb6Status(TestWbConfig._wbAddress)
                        if not sts.getDigIn(idx):
                            self.error( 'channel ' + str(chn) + ' stuck low' )
                else:
                    if sts.getDigIn(idx):
                        sts = wb6Status(TestWbConfig._wbAddress)
                        if sts.getDigIn(idx):
                            self.error( 'channel ' + str(idx) + ' unexpectedly high' )
            else:
                if ( idx == chn ):
                    if sts.getDigIn(idx):
                        sts = wb6Status(TestWbConfig._wbAddress)
                        if sts.getDigIn(idx):
                            self.error( 'channel ' + str(chn) + ' stuck high' )
                else:
                    if not sts.getDigIn(idx):
                        sts = wb6Status(TestWbConfig._wbAddress)
                        if not sts.getDigIn(idx):
                            self.error( 'channel ' + str(idx) + ' unexpectedly low' )

        # switch all on
        self.setTest( 'Off test Digital channel ' + str(chn) + ' ip address ' + TestWbConfig._wbAddress )
        for idx in range(0,8):
            wb6.DigOn( TestWbConfig._wbAddress, idx )
            time.sleep(1)
        # switch off under test channel
        wb6.DigOff( TestWbConfig._wbAddress, chn )
        time.sleep(1)
        sts = wb6Status(TestWbConfig._wbAddress)
        # verify only channel under test off
        for idx in range(0,8):
            # if the same
            if ( idx > 3 ) or not isOc:
                if ( idx == chn ):
                    if sts.getDigIn(idx):
                        sts = wb6Status(TestWbConfig._wbAddress)
                        if sts.getDigIn(idx):
                            self.error( 'channel ' + str(chn) +' stuck high' )
                else:
                    if not sts.getDigIn(idx):
                        sts = wb6Status(TestWbConfig._wbAddress)
                        if not sts.getDigIn(idx):
                            self.error( 'channel ' + str(idx) + ' unexpectedly low' )
            else:
                if ( idx == chn ):
                    if not sts.getDigIn(idx):
                        sts = wb6Status(TestWbConfig._wbAddress)
                        if not sts.getDigIn(idx):
                            self.error( 'channel ' + str(chn) +' stuck low' )
                else:
                    if sts.getDigIn(idx):
                        sts = wb6Status(TestWbConfig._wbAddress)
                        if sts.getDigIn(idx):
                            self.error( 'channel ' + str(idx) + ' unexpectedly high' )
                    
    def doTestOneAn( self, chn ):
        self.setTest( 'test Analogue channel ' + str(chn) + ' ip address ' + TestWbConfig._wbAddress )
        # for analogue level from 0 to 100% in 10% steps.

        # We are going to stop at 80 % due to power supply problems limiting an out 
        for i in range(0,81,20):
            # set all channels to this level.
            for j in range(0, 4):
                wb6.AnOutPercent( TestWbConfig._wbAddress, j, i )
            time.sleep(0.5)
            # for channel under analogue level from 0 to 100% in 10% steps.
            for k in range(0, 81, 20):
                showWorking()
                wb6.AnOutPercent( TestWbConfig._wbAddress, chn, k )
                # could be a step of 100% which is approx 1 second. Could also 
                # get caught by the A/D timer which reads a channel every second so 4 seconds for the lot.
                time.sleep(0.5)
                sts = wb6Status(TestWbConfig._wbAddress)
                for j in range(0, 4):
                    val = sts.getAnIn(j)
                    if j == chn :
                        # should be k
                        if ( val < ( k - 5 ) ) or ( val > ( k + 5 ) ) :
                            self.error( 'test Analogue channel ' + str(chn) + ' expecting ' + str(k) + ' got ' + str(val) )
                    else:
                        # should be i
                        if ( val < ( i - 5 ) ) or ( val > ( i + 5 ) ) :
                            self.error( 'test Analogue channel ' + str(chn) +  '('+str(k)+') verify channel ' + str(j) + ' expecting ' + str(i) + ' got ' + str(val) )
                            
    def testAllDigital(self):
        wb6.Send(TestWbConfig._wbAddress,'SM0;63;8')   # make mimic full off or full on..
        # tests digital in 0-7 and digital out 0-7/mimics 0-7. The later depends on the conenction to the inputs
        for idx in range(0,8):
            self.doTestOneDin( idx )
        self.assertEqual( self.ErrorCount, 0 )
 
    def testAllAnalogue(self):
        # test analogue out and inputs
        wb6.Send(TestWbConfig._wbAddress,'SO2;1')   # Read continuous on analogue inputs.
        for idx in range(0,4):
            self.doTestOneAn( idx )
        self.assertEqual( self.ErrorCount, 0 )
            
    def testRelays(self):
        # NOTE a digital input is active LOW, hence
        # relay on takes input high and the dig in state is false.
        #
        # test the relay contacts and digital in 8-11 (AKA Monitor inputs).
        for r in range(4,6):
            self.setTest( 'test Relays channel ' + str(r) + ' ip address ' + TestWbConfig._wbAddress )
            for idx in range(0,8):
                wb6.DigOff( TestWbConfig._wbAddress, idx )
                time.sleep(0.1)
            # should currently all be low.
            sts = wb6Status(TestWbConfig._wbAddress)
            for idx in range(8,12):
                if sts.getDigIn(idx):
                    sts = wb6Status(TestWbConfig._wbAddress)
                    if sts.getDigIn(idx):
                        self.error( 'Relay channel ' + str(r) + ' Initialise all Low ' + str(idx) + ' High' )

            wb6.DigOn( TestWbConfig._wbAddress, r )
            time.sleep(1)
            # now 2 monitors high and two low. Note relay 4 is inputs 8 and 9.
            sts = wb6Status(TestWbConfig._wbAddress)
            for idx in range(8,12):
                if (idx/2) == r :
                    if not sts.getDigIn(idx):
                        logging.debug( 'test Relay channel ' + str(idx/2) )
                        sts = wb6Status(TestWbConfig._wbAddress)
                        if not sts.getDigIn(idx):
                            self.error( 'test Relay channel ' + str(r) + 'Monitor ' + str(idx) + ' High' )
                else:
                    if sts.getDigIn(idx):
                        logging.debug( 'test Relay channel ' + str(idx/2) )
                        sts = wb6Status(TestWbConfig._wbAddress)
                        if sts.getDigIn(idx):
                            self.error( 'test Relay channel ' + str(r) + 'Monitor ' + str(idx) +  ' Low' )
        self.assertEqual( self.ErrorCount, 0 )

    def testTemperature(self):
        # test analogue out and inputs
        sts = wb6Status(TestWbConfig._wbAddress)
        if ( sts.getOneWireBus() != 7 ):
            # let system catch up.
            for idx in range(0,21):
                showWorking()
                time.sleep(1)
                
        sts = wb6Status(TestWbConfig._wbAddress)
        if ( sts.getOneWireBus() != 7 ):
            self.error( 'invalid temp sensor count : ' + str(sts.getOneWireBus()) )
        
        for idx in range(0,5):
            tv = sts.getTemp( idx )
            if ( tv > 125.0 ) or ( tv < -55.0 ) :
                self.error( 'invalid temp reading : ' + str(tv) )
        self.assertEqual( self.ErrorCount, 0 )
            
    def testTriacs(self):
        # need better test rig for this. 
        # e.g. temp sensors on mains lamps.
        showWorking()
        self.assertEqual( self.ErrorCount, 0 )
 
    def testCheckClock(self ):
        self.setTest( 'testCheckClock' )
        for idx in range(0,40):
            showWorking()
            time.sleep(1)# allow WB to read RTC on 30 sec boundary after boot.
        now = datetime.datetime.now()
        
        # Hold off at minute boundary.
        while ( now.second < 2 ):
            showWorking()
            time.sleep(1)
            now = datetime.datetime.now()
        # verify.
        sts = wb6Status(TestWbConfig._wbAddress)
        ts = sts.getTime()
        ms = '%02u:%02u:00' % (now.hour,now.minute)
        me = '%02u:%02u:59' % (now.hour,now.minute)
        if (ms > ts) or (ts > me):
            self.error( 'checkTime ' + "wbts:" + ts + " ms:" + ms + " me:" + me )
        # webbrick counts sunday as 0, python monday as 0 (sun=6)
        day = sts.getDay()
        if ( day == 0 ):
            day = 6
        else:
            day = day - 1
        if day != now.weekday():
            self.error( 'checkTime  wbday: (%u)%u local %u' % (sts.getDay(), day, now.weekday())  )
        self.assertEqual( self.ErrorCount, 0 )

# Code to run unit tests directly from command line.
# Constructing the suite manually allows control over the order of tests.
def getTestSuite():
    suite = unittest.TestSuite()
    suite.addTest(TestFactory("testFactoryReset"))
    suite.addTest(TestFactory("testSetConfiguration"))
    suite.addTest(TestFactory("testSetClock"))
    suite.addTest(TestFactory("testCheckClock"))
    suite.addTest(TestFactory("testAllDigital"))
    suite.addTest(TestFactory("testRelays"))
    suite.addTest(TestFactory("testTemperature"))
    suite.addTest(TestFactory("testTriacs"))
    suite.addTest(TestFactory("testAllAnalogue"))
    suite.addTest(TestFactory("testFactoryReset"))
    return suite

if __name__ == "__main__":

    if len(sys.argv) > 1:
        logging.basicConfig(level=logging.DEBUG)
        tests = TestFactory( sys.argv[1] )
    else:
        logging.basicConfig(level=logging.INFO)
        tests = getTestSuite()

    runner = unittest.TextTestRunner(verbosity=2)
    runner.run(tests)

#    doAnalogue = True
#    doClock = True
#    doDigital = True
#    doTemperature = True
#    doMains = True
#     args = sys.argv[1:]
#    if args:
#        for arg in args:
#            if arg == "fast":
#                doAnalogue = False
#                doClock = False
#            elif arg == "noanalogue":
#                doAnalogue = False
#            elif arg == "clockonly":
#                doAnalogue = False
#                doClock = True
#                doDigital = False
#                doTemperature = False
#                doMains = False
