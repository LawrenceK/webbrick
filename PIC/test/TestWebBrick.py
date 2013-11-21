# $Id: TestWebBrick.py 533 2006-01-23 21:44:46Z graham $
#
# Integration testing for WebBrick hardware unit
# See http://pyunit.sourceforge.net/pyunit.html
#
# NOTE: this is not strictly a unit test, in that it requires a WebBrick to be
# available at the specified IP address

import sys, time, logging
# import os
import unittest

sys.path.append("../../Python/API")
import wb6, wb6Config, wb6Status

from TestUtils import *
from TestCfg import *

class TestWebBrick(unittest.TestCase):

    def setUp(self):
        return

    def tearDown(self):
        return

    def verifyDiConfiguration(self, idx, cfg):
        di = cfg.getDigInTrigger(idx)
        self.assertEqual( di["name"], "Sw-"+str(idx) )
        if ( idx < 8 ):
            self.assertEqual( di["actionNr"], 4)
            self.assertEqual( di["action"], "Toggle")
            self.assertEqual( di["pairChn"], idx)
            self.assertEqual( di["options"], 2)
            self.assertEqual( di["UDPRem"], "General")
            self.assertEqual( di["UDPRemNr"], 1)
        else:
            self.assertEqual( di["actionNr"], 0)
            self.assertEqual( di["action"], "None")
            self.assertEqual( di["pairChn"], 0)
            self.assertEqual( di["options"], 3)
            self.assertEqual( di["UDPRem"], "None")
            self.assertEqual( di["UDPRemNr"], 0)
        self.assertEqual( di["dwell"], 0, di["name"])
        self.assertEqual( di["typeNr"], 0)
        self.assertEqual( di["type"], "Digital")
        self.assertEqual( di["setPoint"], 0)
        self.assertEqual( di["RemNode"], 0)

        return
        
    def verifyAiConfiguration(self, idx, cfg):
        ai = cfg.getAnalogueTriggerLow(idx)
        self.assertEqual( ai["name"], "AnIn-"+str(idx) )
        self.assertEqual( ai["threshold"], 0, ai["name"] )
        self.assertEqual( ai["actionNr"], 0, ai["name"] )
        self.assertEqual( ai["action"], "None", ai["name"] )
        self.assertEqual( ai["pairChn"], 0, ai["name"] )
        self.assertEqual( ai["dwell"], 0, ai["name"] )
        self.assertEqual( ai["typeNr"], 0, ai["name"] )
        self.assertEqual( ai["type"], "Digital", ai["name"] )
        self.assertEqual( ai["setPoint"], 0, ai["name"] )
        self.assertEqual( ai["UDPRem"], "None", ai["name"] )
        self.assertEqual( ai["UDPRemNr"], 0, ai["name"] )
        self.assertEqual( ai["RemNode"], 0, ai["name"] )
        
        ai = cfg.getAnalogueTriggerHigh(idx)
        self.assertEqual( ai["name"], "AnIn-"+str(idx) )
        self.assertEqual( ai["threshold"], 100, ai["name"] )
        self.assertEqual( ai["actionNr"], 0, ai["name"] )
        self.assertEqual( ai["action"], "None", ai["name"] )
        self.assertEqual( ai["pairChn"], 0, ai["name"] )
        self.assertEqual( ai["dwell"], 0, ai["name"] )
        self.assertEqual( ai["typeNr"], 0, ai["name"] )
        self.assertEqual( ai["type"], "Digital", ai["name"] )
        self.assertEqual( ai["setPoint"], 0, ai["name"] )
        self.assertEqual( ai["UDPRem"], "None", ai["name"] )
        self.assertEqual( ai["UDPRemNr"], 0, ai["name"] )
        self.assertEqual( ai["RemNode"], 0, ai["name"] )
        return
        
    def verifyTempConfiguration(self, idx, cfg):
        ti = cfg.getTempTriggerLow(idx)
        self.assertEqual( ti["name"], "Temp-"+str(idx) )
        self.assertEqual( ti["threshold"], -50 )
        self.assertEqual( ti["actionNr"], 0 )
        self.assertEqual( ti["action"], "None" )
        self.assertEqual( ti["pairChn"], 0 )
        self.assertEqual( ti["dwell"], 0 )
        self.assertEqual( ti["typeNr"], 0 )
        self.assertEqual( ti["type"], "Digital" )
        self.assertEqual( ti["setPoint"], 0 )
        self.assertEqual( ti["UDPRem"], "None" )
        self.assertEqual( ti["UDPRemNr"], 0 )
        self.assertEqual( ti["RemNode"], 0 )
        
        ti = cfg.getTempTriggerHigh(idx)
        self.assertEqual( ti["name"], "Temp-"+str(idx) )
        self.assertEqual( ti["threshold"], 100 )
        self.assertEqual( ti["actionNr"], 0 )
        self.assertEqual( ti["action"], "None" )
        self.assertEqual( ti["pairChn"], 0 )
        self.assertEqual( ti["dwell"], 0 )
        self.assertEqual( ti["typeNr"], 0 )
        self.assertEqual( ti["type"], "Digital" )
        self.assertEqual( ti["setPoint"], 0 )
        self.assertEqual( ti["UDPRem"], "None" )
        self.assertEqual( ti["UDPRemNr"], 0 )
        self.assertEqual( ti["RemNode"], 0 )
        return

    def verifyScheduleConfiguration(self, idx, cfg):
        sc = cfg.getScheduledEvent(idx)
        self.assertEqual( sc["days"], 0 )
        self.assertEqual( sc["hours"], 0 )
        self.assertEqual( sc["mins"], 0 )
        self.assertEqual( sc["actionNr"], 0 )
        self.assertEqual( sc["action"], "None" )
        self.assertEqual( sc["pairChn"], 0 )
        self.assertEqual( sc["dwell"], 0 )
        self.assertEqual( sc["typeNr"], 0 )
        self.assertEqual( sc["type"], "Digital" )
        self.assertEqual( sc["setPoint"], 0 )
        self.assertEqual( sc["UDPRemNr"], 0 )
        self.assertEqual( sc["UDPRem"], "None" )
        self.assertEqual( sc["RemNode"], 0 )

    def verifySceneConfiguration(self, idx, cfg):
        sc = cfg.getScene(idx)
        self.assertEqual( sc["Digital0"], "Ignore" )
        self.assertEqual( sc["Digital1"], "Ignore" )
        self.assertEqual( sc["Digital2"], "Ignore" )
        self.assertEqual( sc["Digital3"], "Ignore" )
        self.assertEqual( sc["Digital4"], "Ignore" )
        self.assertEqual( sc["Digital5"], "Ignore" )
        self.assertEqual( sc["Digital6"], "Ignore" )
        self.assertEqual( sc["Digital7"], "Ignore" )
        self.assertEqual( sc["Analogue0"], "Ignore" )
        self.assertEqual( sc["Analogue1"], "Ignore" )
        self.assertEqual( sc["Analogue2"], "Ignore" )
        self.assertEqual( sc["Analogue3"], "Ignore" )

    # Actual tests follow

    def verifyName(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        self.assertEqual( cfg.getNodeName(),"UnNamed" )
        return
        
    def verifyIp(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        return
        
    def verifyNodeNumber(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        self.assertEqual( cfg.getNodeNumber(),0 )
        return
        
    def verifyRotary(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        self.assertEqual( cfg.getRotary(0 ),64)
        self.assertEqual( cfg.getRotary(1 ),64)
        return
        
    def verifyFadeRate(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        self.assertEqual( cfg.getFadeRate(),8)
        return
        
    def verifyDigitalIn(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        for idx in range(0,TestWbConfig._diCount):
            self.verifyDiConfiguration(idx, cfg)

    def verifyAnalogueIn(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        for idx in range(0,TestWbConfig._aiCount):
            self.verifyAiConfiguration(idx, cfg)

    def verifyTemperatures(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        for idx in range(0,TestWbConfig._tempCount):
            self.verifyTempConfiguration(idx, cfg)
        
    def verifySchedules(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        for idx in range(0,TestWbConfig._schedCount):
            self.verifyScheduleConfiguration(idx, cfg)
        
    def verifyScenes(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        for idx in range(0,TestWbConfig._sceneCount):
            self.verifySceneConfiguration(idx, cfg)
        
    def verifyDwellConfiguration(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        self.assertEqual( cfg.getDwell(0),30)
        self.assertEqual( cfg.getDwell(1),2)
        self.assertEqual( cfg.getDwell(2),60)
        self.assertEqual( cfg.getDwell(3),3600)
        #self.assertEqual( cfg.getDwell(4),300)
        #self.assertEqual( cfg.getDwell(5),600)
        #self.assertEqual( cfg.getDwell(6),900)
        #self.assertEqual( cfg.getDwell(7),1200)
        
    def verifySetPointConfiguration(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        self.assertEqual( cfg.getSetPoint(0), 0)
        self.assertEqual( cfg.getSetPoint(1), 14)
        self.assertEqual( cfg.getSetPoint(2), 28)
        self.assertEqual( cfg.getSetPoint(3),42)
        #self.assertEqual( cfg.getSetPoint(4), 57)
        #self.assertEqual( cfg.getSetPoint(5), 71)
        #self.assertEqual( cfg.getSetPoint(6), 85)
        #self.assertEqual( cfg.getSetPoint(7), 100)
        
    def verifyDoNameConfiguration(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        for idx in range(0,TestWbConfig._doCount):
            self.assertEqual( cfg.getDigOutName(idx),"DigOut-"+str(idx) )
    
    def verifyAoNameConfiguration(self):
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        for idx in range(0,TestWbConfig._aoCount):
            self.assertEqual( cfg.getAnalogueOutName(idx), "AnOut-"+str(idx))
    
    def verifyFactoryConfiguration(self):
        self.verifyName()
        self.verifyIp()
        self.verifyNodeNumber()
        self.verifyRotary()
        self.verifyFadeRate()
        self.verifyDigitalIn()
        self.verifyAnalogueIn()
        self.verifyTemperatures()
        self.verifySchedules()
        self.verifyScenes()
        self.verifyDwellConfiguration()
        self.verifySetPointConfiguration()
        self.verifyDoNameConfiguration()
        self.verifyAoNameConfiguration()
        return

    def testConfigureDigitalIn(self):
        doLogin(TestWbConfig._wbAddress)
        # send CD command
        # D1, Target A1, action On, sp 4, dw 2, udp 1, udpnode 100
        for cmd in range(wb6.AT_NONE, wb6.AT_SPARE):
            for sp in range(0, TestWbConfig._spCount):
                if ( cmd == wb6.AT_DWELL ) or ( cmd == wb6.AT_DWELLCAN ):
                    showWorking()
                    for dwell in range(0, TestWbConfig._dwellCount):
                        idStr = "ConfigDigIn, 1, wb6.TT_ANALOGUE, 1, cmd:" + str(cmd) + " sp:"+ str(sp) +" dw:"+str(dwell) + ",1,100"
                        wb6.ConfigDigIn(TestWbConfig._wbAddress, 1,wb6.TT_ANALOGUE,1,cmd,sp,dwell,1,100 )
                        time.sleep(1.0)
                        # read all the configuration and verify entries.
                        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
                        for idx in range(0,TestWbConfig._diCount):
                            if ( idx != 1):
                                self.verifyDiConfiguration(idx, cfg)
                            else:
                                di = cfg.getDigInTrigger(idx)
                                self.assertEqual( di["name"], "Sw-"+str(idx))
                                self.assertEqual( di["action"], wb6.ActionStrs[cmd] )
                                self.assertEqual( di["actionNr"], cmd)
                                self.assertEqual( di["pairChn"], 1)
                                self.assertEqual( di["options"], 2)
                                self.assertEqual( di["dwell"], dwell)
                                self.assertEqual( di["typeNr"], 2)
                                self.assertEqual( di["type"], "Analogue")
                                self.assertEqual( di["setPoint"], sp)
                                self.assertEqual( di["UDPRem"], "General")
                                self.assertEqual( di["UDPRemNr"], 1)
                                self.assertEqual( di["RemNode"], 100)
                else:
                    showWorking()
                    dwell = 0
                    idStr = "ConfigDigIn, 1, wb6.TT_ANALOGUE, 1, cmd:" + str(cmd) + " sp:"+ str(sp) +" dw:"+str(dwell) + ",1,100"
                    wb6.ConfigDigIn(TestWbConfig._wbAddress, 1,wb6.TT_ANALOGUE,1,cmd,sp,dwell,1,100 )
                    time.sleep(1.0)
                    # read all the configuration and verify entries.
                    cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
                    for idx in range(0,TestWbConfig._diCount):
                        if ( idx != 1):
                            self.verifyDiConfiguration(idx, cfg)
                        else:
                            di = cfg.getDigInTrigger(idx)
                            self.assertEqual( di["name"], "Sw-"+str(idx))
                            self.assertEqual( di["actionNr"], cmd)
                            self.assertEqual( di["action"], wb6.ActionStrs[cmd])
                            self.assertEqual( di["pairChn"], 1)
                            self.assertEqual( di["options"], 2)
                            self.assertEqual( di["dwell"], dwell)
                            self.assertEqual( di["typeNr"], 2)
                            self.assertEqual( di["type"], "Analogue")
                            self.assertEqual( di["setPoint"], sp)
                            self.assertEqual( di["UDPRem"], "General")
                            self.assertEqual( di["UDPRemNr"], 1)
                            self.assertEqual( di["RemNode"], 100)
        
    def testConfigureAnThreshold(self):
        doLogin(TestWbConfig._wbAddress)
        # send CD command
        # D1, Target A1, action On, sp 4, dw 2, udp 1, udpnode 100
        wb6.ConfigAnIn(TestWbConfig._wbAddress, 1, "L", 17, wb6.TT_ANALOGUE, 2, wb6.AT_ON, wb6.SP_4, wb6.DW_2, wb6.UDPT_GENERAL, 99 )
        wb6.ConfigAnIn(TestWbConfig._wbAddress, 1, "H", 87, wb6.TT_DIGITAL, 2, wb6.AT_ON, wb6.SP_0, wb6.DW_2, wb6.UDPT_GENERAL, 99 )
        idStr = "ConfigAnIn, 1"

        time.sleep(0.5)
        # read all the configuration and verify entries.
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        for idx in range(0,TestWbConfig._aiCount):
            if ( idx != 1):
                self.verifyAiConfiguration(idx, cfg)
            else:
                ai = cfg.getAnalogueTriggerLow(idx)
                self.assertEqual( ai["name"] , "AnIn-"+str(idx))
                self.assertEqual( ai["threshold"] , 17)
                self.assertEqual( ai["actionNr"] , 2)
                self.assertEqual( ai["action"] , "On")
                self.assertEqual( ai["pairChn"] , 2)
                self.assertEqual( ai["dwell"] , 2)
                self.assertEqual( ai["typeNr"] , 2)
                self.assertEqual( ai["type"] , "Analogue")
                self.assertEqual( ai["setPoint"] , 4)
                self.assertEqual( ai["UDPRem"] , "General")
                self.assertEqual( ai["UDPRemNr"] , 1)
                self.assertEqual( ai["RemNode"] , 99)
        
                ai = cfg.getAnalogueTriggerHigh(idx)
                self.assertEqual( ai["name"] , "AnIn-"+str(idx), idStr)
                self.assertEqual( ai["threshold"] , 87)
                self.assertEqual( ai["actionNr"] , 2)
                self.assertEqual( ai["action"] , "On")
                self.assertEqual( ai["pairChn"] , 2)
                self.assertEqual( ai["dwell"] , 2)
                self.assertEqual( ai["typeNr"] , 0)
                self.assertEqual( ai["type"] , "Digital")
                self.assertEqual( ai["setPoint"] , 0)
                self.assertEqual( ai["UDPRem"] , "General" )
                self.assertEqual( ai["UDPRemNr"] , 1)
                self.assertEqual( ai["RemNode"] , 99)
        return

    def testConfigureTempThreshold(self):
        doLogin(TestWbConfig._wbAddress)
        # send CD command
        wb6.ConfigTemp(TestWbConfig._wbAddress, 1, "L", -14, wb6.TT_ANALOGUE, 2, wb6.AT_ON, wb6.SP_4, wb6.DW_2, wb6.UDPT_GENERAL, 99 )
        wb6.ConfigTemp(TestWbConfig._wbAddress, 1, "H", 57, wb6.TT_DIGITAL, 2, wb6.AT_ON, wb6.SP_0, wb6.DW_2, wb6.UDPT_GENERAL, 99 )
        idStr = "ConfigTemp, 1"

        time.sleep(0.5)
        # read all the configuration and verify entries.
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        for idx in range(0,TestWbConfig._tempCount):
            if ( idx != 1):
                self.verifyTempConfiguration(idx, cfg)
            else:
                ti = cfg.getTempTriggerLow(idx)
                self.assertEqual( ti["name"] , "Temp-"+str(idx), idStr)
                self.assertEqual( ti["threshold"] , -14)
                self.assertEqual( ti["actionNr"] , 2)
                self.assertEqual( ti["action"] , "On")
                self.assertEqual( ti["pairChn"] , 2)
                self.assertEqual( ti["dwell"] , 2)
                self.assertEqual( ti["typeNr"] , 2)
                self.assertEqual( ti["type"] , "Analogue")
                self.assertEqual( ti["setPoint"] , 4)
                self.assertEqual( ti["UDPRem"] , "General")
                self.assertEqual( ti["UDPRemNr"] , 1)
                self.assertEqual( ti["RemNode"] , 99)
        
                ti = cfg.getTempTriggerHigh(idx)
                self.assertEqual( ti["name"] , "Temp-"+str(idx), idStr)
                self.assertEqual( ti["threshold"] , 57)
                self.assertEqual( ti["actionNr"] , 2)
                self.assertEqual( ti["action"] , "On")
                self.assertEqual( ti["pairChn"] , 2)
                self.assertEqual( ti["dwell"] , 2)
                self.assertEqual( ti["typeNr"] , 0)
                self.assertEqual( ti["type"] , "Digital")
                self.assertEqual( ti["setPoint"] , 0)
                self.assertEqual( ti["UDPRem"] , "General")
                self.assertEqual( ti["UDPRemNr"] , 1)
                self.assertEqual( ti["RemNode"] , 99)
        return

    def doSetTime( self, d, h, m ):
        doLogin(TestWbConfig._wbAddress)
        wb6.SetTime( TestWbConfig._wbAddress, d, h, m )
        time.sleep(5.0) # the schdule catch up could take a few seconds so lets make sure we wait to see whats happened

    def testSetClock(self):
        self.doSetTime( 1, 1, 1 )
        sts = wb6Status.wb6Status( TestWbConfig._wbAddress )
        self.assertEqual( sts.getError(), 0 )
        ts = sts.getTime()
#        print ts
        self.failUnless( (ts >= "01:01:00") and (ts <= "01:01:59") )
        self.assertEqual( sts.getDay(), 1)

    def testScheduledEvent(self):
        self.doSetTime( 1, 1, 1 )
        # Create a scheduled event
        wb6.ConfigScheduled( TestWbConfig._wbAddress, 1, "1", 1, 2, wb6.TT_DIGITAL, 3, wb6.AT_TOGGLE, wb6.SP_0, wb6.DW_0, wb6.UDPT_GENERAL, 98 )
        time.sleep(0.5)
        sts = wb6Status.wb6Status( TestWbConfig._wbAddress )
        self.assertEqual( sts.getError(), 0 )
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        for idx in range(0,TestWbConfig._schedCount):
            if ( idx == 1 ):
                sc = cfg.getScheduledEvent(idx)
                self.assertEqual( sc["days"] , 2)
                self.assertEqual( sc["hours"] , 1)
                self.assertEqual( sc["mins"] , 2)
                self.assertEqual( sc["actionNr"] , 4)
                self.assertEqual( sc["action"] , "Toggle")
                self.assertEqual( sc["pairChn"] , 3)
                self.assertEqual( sc["dwell"] , 0)
                self.assertEqual( sc["typeNr"] , 0)
                self.assertEqual( sc["type"] , "Digital")
                self.assertEqual( sc["setPoint"] , 0)
                self.assertEqual( sc["UDPRemNr"] , 1)
                self.assertEqual( sc["UDPRem"] , "General")
                self.assertEqual( sc["RemNode"] , 98)
            else:
                self.verifyScheduleConfiguration(idx, cfg)
        cur = sts.getDigOut(3);
        # wait for it to run?
        while ( sts.getTime() < "01:02:00" ):
            time.sleep(1.0)
            showWait()
            sts = wb6Status.wb6Status( TestWbConfig._wbAddress )
        # ensure it has a chance to run
        time.sleep(1.0)
        sts = wb6Status.wb6Status( TestWbConfig._wbAddress )
        self.assertNotEqual(cur,sts.getDigOut(3))

    def checkCmdStatus(self):
        sts = wb6Status.wb6Status( TestWbConfig._wbAddress )
        self.assertEqual( sts.getError(), 0 )

    def testScheduledRestart(self):
        factoryReset(TestWbConfig._wbAddress) # from known state
        self.doSetTime( 1, 1, 0 ) # before scheduled event
        # Create a scheduled event
        wb6.ConfigScheduled( TestWbConfig._wbAddress, 2, "1", 1, 2, wb6.TT_DIGITAL, 3, wb6.AT_TOGGLE, wb6.SP_0, wb6.DW_0, wb6.UDPT_GENERAL, 98 )
        time.sleep(0.5)
        self.checkCmdStatus()

        # reboot, let system settle and then move clock forward
        doReboot(TestWbConfig._wbAddress)
        self.doSetTime( 1, 1, 3 ) # after scheduled event
        # now see what has happened
        sts = wb6Status.wb6Status( TestWbConfig._wbAddress )
        self.assertEqual( sts.getDigOut(3), True)

    def testaScene(self):
        # configure a scene and verify it
        factoryReset(TestWbConfig._wbAddress)
        doLogin(TestWbConfig._wbAddress)
        wb6.ConfigScene(TestWbConfig._wbAddress, 1, ["N","F","I","N","F","I","N","F"], [("S",1),"I",("S",2),"I"] )
        time.sleep(1.0)
        cfg = wb6Config.wb6Config( TestWbConfig._wbAddress )
        for idx in range(0,TestWbConfig._sceneCount):
            if idx == 1 :
                sc = cfg.getScene(idx)
                self.assertEqual( sc["Digital0"], "On")
                self.assertEqual( sc["Digital1"], "Off")
                self.assertEqual( sc["Digital2"], "Ignore")
                self.assertEqual( sc["Digital3"], "On")
                self.assertEqual( sc["Digital4"], "Off")
                self.assertEqual( sc["Digital5"], "Ignore")
                self.assertEqual( sc["Digital6"], "On")
                self.assertEqual( sc["Digital7"], "Off")
                self.assertEqual( sc["Analogue0"], "SetPoint1")
                self.assertEqual( sc["Analogue1"], "Ignore")
                self.assertEqual( sc["Analogue2"], "SetPoint2")
                self.assertEqual( sc["Analogue3"], "Ignore")
            else:
                self.verifySceneConfiguration(idx, cfg)


    # did the scene set the outputs as expected.
    def verifySceneAction(self):
        time.sleep(1.0) # let analogues be read
        sts = wb6Status.wb6Status( TestWbConfig._wbAddress )
        # verify digital output state
        self.assertEqual( sts.getDigOut(0), True )
        self.assertEqual( sts.getDigOut(1), False )
        self.assertEqual( sts.getDigOut(2), False )     # not touched

        self.assertEqual( sts.getDigOut(3), True )
        self.assertEqual( sts.getDigOut(4), False )
        self.assertEqual( sts.getDigOut(5), False )     # not touched
        self.assertEqual( sts.getDigOut(6), True )
        self.assertEqual( sts.getDigOut(7), False )

        # verify analogue output state
        self.assertEqual( sts.getAnOut(0), 14)
        self.assertEqual( sts.getAnOut(1), 0)
        self.assertEqual( sts.getAnOut(2), 28)
        self.assertEqual( sts.getAnOut(3), 0)

    def testaSceneAndSet(self):
        # configure a scene and verify it
        self.testaScene()
        # Then set scene and verify outputs
        wb6.SetScene( TestWbConfig._wbAddress, 1 )
        # let it process and update Siteplayer
        self.verifySceneAction()

    def testaSceneAndTrigger(self):
        # configure a scene and verify it
        self.testaScene()
        # configure a digital input to trigger a scene.
        wb6.ConfigDigIn(TestWbConfig._wbAddress, 1,wb6.TT_SCENE, 1, wb6.AT_TOGGLE, wb6.SP_0, wb6.DW_0, wb6.UDPT_GENERAL, 100 )
        # Then trigger scene and verify outputs
        wb6.DigTrigger(TestWbConfig._wbAddress, 1)
        # let it process and update Siteplayer
        time.sleep(1.0)
        sts = wb6Status.wb6Status( TestWbConfig._wbAddress )
        self.verifySceneAction()

    def testFactoryReset(self):
        factoryReset( TestWbConfig._wbAddress )

# Code to run unit tests directly from command line.
# Constructing the suite manually allows control over the order of tests.
def getTestSuite():
    suite = unittest.TestSuite()
    suite.addTest(TestWebBrick("testFactoryReset"))
    suite.addTest(TestWebBrick("verifyFactoryConfiguration"))
    suite.addTest(TestWebBrick("verifyName"))
    suite.addTest(TestWebBrick("verifyIp"))
    suite.addTest(TestWebBrick("verifyNodeNumber"))
    suite.addTest(TestWebBrick("verifyRotary"))
    suite.addTest(TestWebBrick("verifyFadeRate"))
    suite.addTest(TestWebBrick("verifyDigitalIn"))
    suite.addTest(TestWebBrick("verifyAnalogueIn"))
    suite.addTest(TestWebBrick("verifyTemperatures"))
    suite.addTest(TestWebBrick("verifySchedules"))
    suite.addTest(TestWebBrick("verifyScenes"))
    suite.addTest(TestWebBrick("verifyDwellConfiguration"))
    suite.addTest(TestWebBrick("verifySetPointConfiguration"))
    suite.addTest(TestWebBrick("verifyDoNameConfiguration"))
    suite.addTest(TestWebBrick("verifyAoNameConfiguration"))
    suite.addTest(TestWebBrick("testConfigureDigitalIn"))
    suite.addTest(TestWebBrick("testConfigureAnThreshold"))
    suite.addTest(TestWebBrick("testConfigureTempThreshold"))
    suite.addTest(TestWebBrick("testSetClock"))
    suite.addTest(TestWebBrick("testScheduledEvent"))
    suite.addTest(TestWebBrick("testScheduledRestart"))
    suite.addTest(TestWebBrick("testaScene"))
    suite.addTest(TestWebBrick("testaSceneAndSet"))
    suite.addTest(TestWebBrick("testaSceneAndTrigger"))
    return suite

if __name__ == "__main__":

    if len(sys.argv) > 1:
        logging.basicConfig(level=logging.DEBUG)
        tests = TestWebBrick( sys.argv[1] )
    else:
        logging.basicConfig(level=logging.INFO)
        tests = getTestSuite()

    runner = unittest.TextTestRunner(verbosity=2)
    runner.run(tests)
