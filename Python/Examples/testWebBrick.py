# $Id: testWebBrick.py 533 2006-01-23 21:44:46Z graham $
#
# Integration testing for WebBrick hardware unit
# See http://pyunit.sourceforge.net/pyunit.html
#
# NOTE: this is not strictly a unit test, in that it requires a WebBrick to be
# available at the specified IP address

import sys, time, logging
# import os
import unittest

sys.path.append("../API")
#from WbAccess import *
import wb6, wb6Config, wb6Status

#logging.basicConfig(level=logging.DEBUG)

def showWorking():
    sys.stdout.write(".")

def showWait():
    sys.stdout.write("w")

class testWebBrick(unittest.TestCase):

    _wbAddress = "10.100.100.100"

    # these should match what the webBrick has.
    _dwellCount = 4
    _spCount = 8
    _diCount = 12
    _aiCount = 4
    _doCount = 8
    _aoCount = 4
    _tempCount = 5
    _schedCount = 16
    _sceneCount = 8

    def doLogin(self):
        wb6.Login( self._wbAddress, "installer" )
        return

    def doReboot(self):
        # takes a while.
        wb6.Send(self._wbAddress, "RB" )
        time.sleep(5.0)
        return

    def factoryReset(self):
        self.doLogin()
        # send factory reset, full reset
        wb6.Send(self._wbAddress, "FR1" )
        time.sleep(3.0 )# takes a while.
        self.doReboot()
        return
        
    def setUp(self):
#        self.factoryReset()
        return

    def tearDown(self):
#        self.factoryReset()
        return

    def verifyDiConfiguration(self, idx, cfg):
        di = cfg.getDigInTrigger(idx)
        self.assertEqual( di["name"], "Sw-"+str(idx+1) )
        if ( idx < 8 ):
            self.assertEqual( di["actionNr"], 4)
            self.assertEqual( di["action"], "Toggle")
            self.assertEqual( di["pairChn"], idx)
            self.assertEqual( di["options"], 2)
        else:
            self.assertEqual( di["actionNr"], 0)
            self.assertEqual( di["action"], "None")
            self.assertEqual( di["pairChn"], 0)
            self.assertEqual( di["options"], 3)
        self.assertEqual( di["dwell"], 0)
        self.assertEqual( di["typeNr"], 0)
        self.assertEqual( di["type"], "Digital")
        self.assertEqual( di["setPoint"], 0)
        self.assertEqual( di["UDPRem"], "None" )
        self.assertEqual( di["UDPRemNr"], 0)
        self.assertEqual( di["RemNode"], 0)

        return
        
    def verifyAiConfiguration(self, idx, cfg):
        ai = cfg.getAnalogueTriggerLow(idx)
        self.assertEqual( ai["name"], "AnIn-"+str(idx+1) )
        self.assertEqual( ai["threshold"], 0 )
        self.assertEqual( ai["actionNr"], 0 )
        self.assertEqual( ai["action"], "None" )
        self.assertEqual( ai["pairChn"], 0 )
        self.assertEqual( ai["dwell"], 0 )
        self.assertEqual( ai["typeNr"], 0 )
        self.assertEqual( ai["type"], "Digital" )
        self.assertEqual( ai["setPoint"], 0 )
        self.assertEqual( ai["UDPRem"], "None" )
        self.assertEqual( ai["UDPRemNr"], 0 )
        self.assertEqual( ai["RemNode"], 0 )
        
        ai = cfg.getAnalogueTriggerHigh(idx)
        self.assertEqual( ai["name"], "AnIn-"+str(idx+1) )
        self.assertEqual( ai["threshold"], 100 )
        self.assertEqual( ai["actionNr"], 0 )
        self.assertEqual( ai["action"], "None" )
        self.assertEqual( ai["pairChn"], 0 )
        self.assertEqual( ai["dwell"], 0 )
        self.assertEqual( ai["typeNr"], 0 )
        self.assertEqual( ai["type"], "Digital" )
        self.assertEqual( ai["setPoint"], 0 )
        self.assertEqual( ai["UDPRem"], "None" )
        self.assertEqual( ai["UDPRemNr"], 0 )
        self.assertEqual( ai["RemNode"], 0 )
        return
        
    def verifyTempConfiguration(self, idx, cfg):
        ti = cfg.getTempTriggerLow(idx)
        self.assertEqual( ti["name"], "Temp-"+str(idx+1) )
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
        self.assertEqual( ti["name"], "Temp-"+str(idx+1) )
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
        cfg = wb6Config.wb6Config( self._wbAddress )
        assert cfg.getNodeName() == "UnNamed", cfg.getNodeName()
        return
        
    def verifyIp(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        return
        
    def verifyNodeNumber(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        assert cfg.getNodeNumber() == 0, str(cfg.getNodeNumber())
        return
        
    def verifyRotary(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        assert cfg.getRotary(0 ) == 8, str(cfg.getRotary(0))
        assert cfg.getRotary(1 ) == 8, str(cfg.getRotary(1))
        return
        
    def verifyFadeRate(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        assert cfg.getFadeRate() == 8, str(cfg.getFadeRate())
        return
        
    def verifyDigitalIn(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        for idx in range(0,self._diCount):
            self.verifyDiConfiguration(idx, cfg)

    def verifyAnalogueIn(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        for idx in range(0,self._aiCount):
            self.verifyAiConfiguration(idx, cfg)

    def verifyTemperatures(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        for idx in range(0,self._tempCount):
            self.verifyTempConfiguration(idx, cfg)
        
    def verifySchedules(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        for idx in range(0,self._schedCount):
            self.verifyScheduleConfiguration(idx, cfg)
        
    def verifyScenes(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        for idx in range(0,self._sceneCount):
            self.verifySceneConfiguration(idx, cfg)
        
    def verifyDwellConfiguration(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        assert cfg.getDwell(0) == 30, str(cfg.getDwell(0))
        assert cfg.getDwell(1) == 2, str(cfg.getDwell(1))
        assert cfg.getDwell(2) == 60, str(cfg.getDwell(2))
        assert cfg.getDwell(3) == 3600, str(cfg.getDwell(3))
        assert cfg.getDwell(4) == 300, str(cfg.getDwell(4))
        assert cfg.getDwell(5) == 600, str(cfg.getDwell(5))
        assert cfg.getDwell(6) == 900, str(cfg.getDwell(6))
        assert cfg.getDwell(7) == 1200, str(cfg.getDwell(7))
        
    def verifySetPointConfiguration(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        assert cfg.getSetPoint(0) == 0, str(cfg.getSetPoint(0))
        assert cfg.getSetPoint(1) == 14, str(cfg.getSetPoint(1))
        assert cfg.getSetPoint(2) == 28, str(cfg.getSetPoint(2))
        assert cfg.getSetPoint(3) ==42, str(cfg.getSetPoint(3))
        assert cfg.getSetPoint(4) == 57, str(cfg.getSetPoint(4))
        assert cfg.getSetPoint(5) == 71, str(cfg.getSetPoint(5))
        assert cfg.getSetPoint(6) == 85, str(cfg.getSetPoint(6))
        assert cfg.getSetPoint(7) == 100, str(cfg.getSetPoint(7))
        
    def verifyDoNameConfiguration(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        for idx in range(0,self._doCount):
            assert cfg.getDigOutName(idx) == "DigOut-"+str(idx+1), cfg.getDigOutName(idx)
    
    def verifyAoNameConfiguration(self):
        cfg = wb6Config.wb6Config( self._wbAddress )
        for idx in range(0,self._aoCount):
            assert cfg.getAnalogueOutName(idx) == "AnOut-"+str(idx+1), cfg.getAnalogueOutName(idx)
    
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
        self.doLogin()
        # send CD command
        # D1, Target A1, action On, sp 4, dw 2, udp 1, udpnode 100
        for cmd in range(wb6.AT_NONE, wb6.AT_SPARE):
            for sp in range(0, self._spCount):
                if ( cmd == wb6.AT_DWELL ) or ( cmd == wb6.AT_DWELLCAN ):
                    showWorking()
                    for dwell in range(0, self._dwellCount):
                        idStr = "ConfigDigIn, 1, wb6.TT_ANALOGUE, 1, cmd:" + str(cmd) + " sp:"+ str(sp) +" dw:"+str(dwell) + ",1,100"
                        wb6.ConfigDigIn(self._wbAddress, 1,wb6.TT_ANALOGUE,1,cmd,sp,dwell,1,100 )
                        time.sleep(1.0)
                        # read all the configuration and verify entries.
                        cfg = wb6Config.wb6Config( self._wbAddress )
                        for idx in range(0,self._diCount):
                            if ( idx != 1):
                                self.verifyDiConfiguration(idx, cfg)
                            else:
                                di = cfg.getDigInTrigger(idx)
                                assert di["name"] == "Sw-"+str(idx+1), idStr + " di[name]:" + str(di["name"])
                                assert di["action"] == wb6.ActionStrs[cmd], idStr + " di[action]:" + di["action"]
                                assert di["actionNr"] == cmd, idStr + " di[actionNr]:" + str(di["actionNr"])
                                assert di["pairChn"] == 1, idStr + " di[pairChn]:" + str(di["pairChn"])
                                assert di["options"] == 2, idStr + " di[options]:" + str(di["options"])
                                assert di["dwell"] == dwell, idStr + " di[dwell]:" + str(di["dwell"])
                                assert di["typeNr"] == 2, idStr + " di[typeNr]:" + str(di["typeNr"])
                                assert di["type"] == "Analogue", idStr + " di[type]:" + str(di["type"])
                                assert di["setPoint"] == sp, idStr + " di[setPoint]:" + str(di["setPoint"])
                                assert di["UDPRem"] == "General", idStr + " di[UDPRem]:" + str(di["UDPRem"])
                                assert di["UDPRemNr"] == 1, idStr + " di[UDPRemNr]:" + str(di["UDPRemNr"])
                                assert di["RemNode"] == 100, idStr + " di[RemNode]:" + str(di["RemNode"])
                else:
                    showWorking()
                    dwell = 0
                    idStr = "ConfigDigIn, 1, wb6.TT_ANALOGUE, 1, cmd:" + str(cmd) + " sp:"+ str(sp) +" dw:"+str(dwell) + ",1,100"
                    wb6.ConfigDigIn(self._wbAddress, 1,wb6.TT_ANALOGUE,1,cmd,sp,dwell,1,100 )
                    time.sleep(1.0)
                    # read all the configuration and verify entries.
                    cfg = wb6Config.wb6Config( self._wbAddress )
                    for idx in range(0,self._diCount):
                        if ( idx != 1):
                            self.verifyDiConfiguration(idx, cfg)
                        else:
                            di = cfg.getDigInTrigger(idx)
                            assert di["name"] == "Sw-"+str(idx+1), idStr + " di[name]:" + str(di["name"])
                            assert di["actionNr"] == cmd, idStr + " di[actionNr]:" + str(di["actionNr"])
                            assert di["action"] == wb6.ActionStrs[cmd], idStr + " di[action]:" + di["action"]
                            assert di["pairChn"] == 1, idStr + " di[pairChn]:" + str(di["pairChn"])
                            assert di["options"] == 2, idStr + " di[options]:" + str(di["options"])
                            assert di["dwell"] == dwell, idStr + " di[dwell]:" + str(di["dwell"])
                            assert di["typeNr"] == 2, idStr + " di[typeNr]:" + str(di["typeNr"])
                            assert di["type"] == "Analogue", idStr + " di[type]:" + str(di["type"])
                            assert di["setPoint"] == sp, idStr + " di[setPoint]:" + str(di["setPoint"])
                            assert di["UDPRem"] == "General", idStr + " di[UDPRem]:" + str(di["UDPRem"])
                            assert di["UDPRemNr"] == 1, idStr + " di[UDPRemNr]:" + str(di["UDPRemNr"])
                            assert di["RemNode"] == 100, idStr + " di[RemNode]:" + str(di["RemNode"])
        
    def testConfigureAnThreshold(self):
        self.doLogin()
        # send CD command
        # D1, Target A1, action On, sp 4, dw 2, udp 1, udpnode 100
        wb6.ConfigAnIn(self._wbAddress, 1, "L", 17, wb6.TT_ANALOGUE, 2, wb6.AT_ON, wb6.SP_4, wb6.DW_2, wb6.UDPT_GENERAL, 99 )
        wb6.ConfigAnIn(self._wbAddress, 1, "H", 87, wb6.TT_DIGITAL, 2, wb6.AT_ON, wb6.SP_0, wb6.DW_2, wb6.UDPT_GENERAL, 99 )
        idStr = "ConfigAnIn, 1"

        time.sleep(0.5)
        # read all the configuration and verify entries.
        cfg = wb6Config.wb6Config( self._wbAddress )
        for idx in range(0,self._aiCount):
            if ( idx != 1):
                self.verifyAiConfiguration(idx, cfg)
            else:
                ai = cfg.getAnalogueTriggerLow(idx)
                assert ai["name"] == "AnIn-"+str(idx+1), idStr + " ai[name]:" + str(ai["name"])
                assert ai["threshold"] == 17, str(ai["threshold"])
                assert ai["actionNr"] == 2, str(ai["actionNr"])
                assert ai["action"] == "On", ai["action"]
                assert ai["pairChn"] == 2, str(ai["pairChn"])
                assert ai["dwell"] == 2, str(ai["dwell"])
                assert ai["typeNr"] == 2, str(ai["typeNr"])
                assert ai["type"] == "Analogue", ai["type"]
                assert ai["setPoint"] == 4, str(ai["setPoint"])
                assert ai["UDPRem"] == "General", ai["UDPRem"]
                assert ai["UDPRemNr"] == 1, str(ai["UDPRemNr"])
                assert ai["RemNode"] == 99, str(ai["RemNode"])
        
                ai = cfg.getAnalogueTriggerHigh(idx)
                assert ai["name"] == "AnIn-"+str(idx+1), idStr + " ai[name]:" + str(ai["name"])
                assert ai["threshold"] == 87, str(ai["threshold"])
                assert ai["actionNr"] == 2, str(ai["actionNr"])
                assert ai["action"] == "On", ai["action"]
                assert ai["pairChn"] == 2, str(ai["pairChn"])
                assert ai["dwell"] == 2, str(ai["dwell"])
                assert ai["typeNr"] == 0, str(ai["typeNr"])
                assert ai["type"] == "Digital", ai["type"]
                assert ai["setPoint"] == 0, str(ai["setPoint"])  # not relevant to digital
                assert ai["UDPRem"] == "General", ai["UDPRem"]
                assert ai["UDPRemNr"] == 1, str(ai["UDPRemNr"])
                assert ai["RemNode"] == 99, str(ai["RemNode"])
        return

    def testConfigureTempThreshold(self):
        self.doLogin()
        # send CD command
        wb6.ConfigTemp(self._wbAddress, 1, "L", -14, wb6.TT_ANALOGUE, 2, wb6.AT_ON, wb6.SP_4, wb6.DW_2, wb6.UDPT_GENERAL, 99 )
        wb6.ConfigTemp(self._wbAddress, 1, "H", 57, wb6.TT_DIGITAL, 2, wb6.AT_ON, wb6.SP_0, wb6.DW_2, wb6.UDPT_GENERAL, 99 )
        idStr = "ConfigTemp, 1"

        time.sleep(0.5)
        # read all the configuration and verify entries.
        cfg = wb6Config.wb6Config( self._wbAddress )
        for idx in range(0,self._tempCount):
            if ( idx != 1):
                self.verifyTempConfiguration(idx, cfg)
            else:
                ti = cfg.getTempTriggerLow(idx)
                assert ti["name"] == "Temp-"+str(idx+1), idStr + " ti[name]:" + str(ti["name"])
                assert ti["threshold"] == -14, str(ti["threshold"])
                assert ti["actionNr"] == 2, str(ti["actionNr"])
                assert ti["action"] == "On", ti["action"]
                assert ti["pairChn"] == 2, str(ti["pairChn"])
                assert ti["dwell"] == 2, str(ti["dwell"])
                assert ti["typeNr"] == 2, str(ti["typeNr"])
                assert ti["type"] == "Analogue", ti["type"]
                assert ti["setPoint"] == 4, str(ti["setPoint"])
                assert ti["UDPRem"] == "General", ti["UDPRem"]
                assert ti["UDPRemNr"] == 1, str(ti["UDPRemNr"])
                assert ti["RemNode"] == 99, str(ti["RemNode"])
        
                ti = cfg.getTempTriggerHigh(idx)
                assert ti["name"] == "Temp-"+str(idx+1), idStr + " ti[name]:" + str(ti["name"])
                assert ti["threshold"] == 57, str(ti["threshold"])
                assert ti["actionNr"] == 2, str(ti["actionNr"])
                assert ti["action"] == "On", ti["action"]
                assert ti["pairChn"] == 2, str(ti["pairChn"])
                assert ti["dwell"] == 2, str(ti["dwell"])
                assert ti["typeNr"] == 0, str(ti["typeNr"])
                assert ti["type"] == "Digital", ti["type"]
                assert ti["setPoint"] == 0, str(ti["setPoint"]) # not relevant to digital
                assert ti["UDPRem"] == "General", ti["UDPRem"]
                assert ti["UDPRemNr"] == 1, str(ti["UDPRemNr"])
                assert ti["RemNode"] == 99, str(ti["RemNode"])
        return

    def doSetTime( self, d, h, m ):
        self.doLogin()
        wb6.SetTime( self._wbAddress, d, h, m )
        time.sleep(5.0) # the schdule catch up could take a few seconds so lets make sure we wait to see whats happened

    def testSetClock(self):
        self.doSetTime( 1, 1, 1 )
        sts = wb6Status.wb6Status( self._wbAddress )
        assert sts.getError() == 0
        ts = sts.getTime()
#        print ts
        assert (ts >= "01:01:00") and (ts <= "01:01:59"), ts
        assert (sts.getDay() == 1), str(sts.getDay())

    def testScheduledEvent(self):
        self.doSetTime( 1, 1, 1 )
        # Create a scheduled event
        wb6.ConfigScheduled( self._wbAddress, 1, "1", 1, 2, wb6.TT_DIGITAL, 3, wb6.AT_TOGGLE, wb6.SP_0, wb6.DW_0, wb6.UDPT_GENERAL, 98 )
        time.sleep(0.5)
        sts = wb6Status.wb6Status( self._wbAddress )
        assert sts.getError() == 0
        cfg = wb6Config.wb6Config( self._wbAddress )
        for idx in range(0,self._schedCount):
            if ( idx == 1 ):
                sc = cfg.getScheduledEvent(idx)
                assert sc["days"] == 2, str(sc["days"])
                assert sc["hours"] == 1, str(sc["hours "])
                assert sc["mins"] == 2, str(sc["mins"])
                assert sc["actionNr"] == 4, str(sc["actionNr"])
                assert sc["action"] == "Toggle", sc["action"]
                assert sc["pairChn"] == 3, str(sc["pairChn"])
                assert sc["dwell"] == 0, str(sc["dwell"])
                assert sc["typeNr"] == 0, str(sc["typeNr"])
                assert sc["type"] == "Digital", sc["type"]
                assert sc["setPoint"] == 0, str(sc["setPoint"])
                assert sc["UDPRemNr"] == 1, str(sc["UDPRemNr"])
                assert sc["UDPRem"] == "General", sc["UDPRem"]
                assert sc["RemNode"] == 98, str(sc["RemNode"])
            else:
                self.verifyScheduleConfiguration(idx, cfg)
        cur = sts.getDigOut(3);
        # wait for it to run?
        while ( sts.getTime() < "01:02:00" ):
            time.sleep(1.0)
            showWait()
            sts = wb6Status.wb6Status( self._wbAddress )
        # ensure it has a chance to run
        time.sleep(1.0)
        sts = wb6Status.wb6Status( self._wbAddress )
        assert (cur <> sts.getDigOut(3)),str(sts.getDigOut(3))    # did it toggle

    def testScheduledRestart(self):
        self.factoryReset() # from known state
        self.doSetTime( 1, 1, 0 ) # before scheduled event
        # Create a scheduled event
        wb6.ConfigScheduled( self._wbAddress, 2, "1", 1, 2, wb6.TT_DIGITAL, 3, wb6.AT_TOGGLE, wb6.SP_0, wb6.DW_0, wb6.UDPT_GENERAL, 98 )
        time.sleep(0.5)
        sts = wb6Status.wb6Status( self._wbAddress )
        assert sts.getError() == 0, str(sts.getError())

        # reboot, let system settle and then move clock forward
        self.doReboot()
        self.doSetTime( 1, 1, 3 ) # after scheduled event
        # now see what has happened
        sts = wb6Status.wb6Status( self._wbAddress )
        assert sts.getDigOut(3), sts.getTime() + " " + str(sts.getDigOut(3))

    def testaScene(self):
        # configure a scene and verify it
        self.factoryReset()
        self.doLogin()
        wb6.ConfigScene(self._wbAddress, 1, ["N","F","I","N","F","I","N","F"], [("S",1),"I",("S",2),"I"] )
        time.sleep(1.0)
        cfg = wb6Config.wb6Config( self._wbAddress )
        for idx in range(0,self._sceneCount):
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
        time.sleep(1.0)
        sts = wb6Status.wb6Status( self._wbAddress )
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
        an = sts.getAnOut(0)
        assert  an == 14 , "Analogue 0 not at expected level " + str(an)
        an = sts.getAnOut(1)
        assert  an == 0 , "Analogue 1 not at expected level " + str(an)
        an = sts.getAnOut(2)
        assert  an == 28 , "Analogue 2 not at expected level " + str(an)
        an = sts.getAnOut(3)
        assert  an == 0 , "Analogue 3 not at expected level " + str(an)

    def testaSceneAndSet(self):
        # configure a scene and verify it
        self.testaScene()
        # Then set scene and verify outputs
        wb6.SetScene( self._wbAddress, 1 )
        # let it process and update Siteplayer
        self.verifySceneAction()

    def testaSceneAndTrigger(self):
        # configure a scene and verify it
        self.testaScene()
        # configure a digital input to trigger a scene.
        wb6.ConfigDigIn(self._wbAddress, 1,wb6.TT_SCENE, 1, wb6.AT_TOGGLE, wb6.SP_0, wb6.DW_0, wb6.UDPT_GENERAL, 100 )
        # Then trigger scene and verify outputs
        wb6.DigTrigger(self._wbAddress, 1)
        # let it process and update Siteplayer
        time.sleep(1.0)
        sts = wb6Status.wb6Status( self._wbAddress )
        self.verifySceneAction()

# Code to run unit tests directly from command line.
# Constructing the suite manually allows control over the order of tests.
def getTestSuite():
    suite = unittest.TestSuite()
    suite.addTest(testWebBrick("factoryReset"))
    suite.addTest(testWebBrick("verifyFactoryConfiguration"))
    suite.addTest(testWebBrick("verifyName"))
    suite.addTest(testWebBrick("verifyIp"))
    suite.addTest(testWebBrick("verifyNodeNumber"))
    suite.addTest(testWebBrick("verifyRotary"))
    suite.addTest(testWebBrick("verifyFadeRate"))
    suite.addTest(testWebBrick("verifyDigitalIn"))
    suite.addTest(testWebBrick("verifyAnalogueIn"))
    suite.addTest(testWebBrick("verifyTemperatures"))
    suite.addTest(testWebBrick("verifySchedules"))
    suite.addTest(testWebBrick("verifyScenes"))
    suite.addTest(testWebBrick("verifyDwellConfiguration"))
    suite.addTest(testWebBrick("verifySetPointConfiguration"))
    suite.addTest(testWebBrick("verifyDoNameConfiguration"))
    suite.addTest(testWebBrick("verifyAoNameConfiguration"))
    suite.addTest(testWebBrick("testConfigureDigitalIn"))
    suite.addTest(testWebBrick("testConfigureAnThreshold"))
    suite.addTest(testWebBrick("testConfigureTempThreshold"))
    suite.addTest(testWebBrick("testSetClock"))
    suite.addTest(testWebBrick("testScheduledEvent"))
    suite.addTest(testWebBrick("testScheduledRestart"))
    suite.addTest(testWebBrick("testaScene"))
    suite.addTest(testWebBrick("testaSceneAndSet"))
    suite.addTest(testWebBrick("testaSceneAndTrigger"))
    return suite

if __name__ == "__main__":
    # If there is a command line argument, override the default IP address
    args = sys.argv[1:]
    if args and args[0]:
        testWebBrick._wbAddress = args[0]
    runner = unittest.TextTestRunner(verbosity=2)
    runner.run(getTestSuite())
    