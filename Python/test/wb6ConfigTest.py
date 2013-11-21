import sys
sys.path.append('../API')

import wb6Config

test_ip = "10.100.100.100";

wb6Cfg = wb6Config.wb6Config( test_ip );

print "Version = " + wb6Cfg.getVersion()
print "NodeName = " + wb6Cfg.getNodeName()
print "NodeNumber = " + str(wb6Cfg.getNodeNumber())

print "FadeRate = " + str(wb6Cfg.getFadeRate())
print "IpAddress = " + str(wb6Cfg.getIpAddress())
print "MAC Address = " + str(wb6Cfg.getMacAddress())
print "Rotary 0 = " + str(wb6Cfg.getRotary(0))
print "Dwell 0 = " + str(wb6Cfg.getDwell(0))
print "Dwell 0 = " + wb6Cfg.getDwellStr(0)
print "Dwell 1 = " + str(wb6Cfg.getDwell(1))
print "Dwell 1 = " + wb6Cfg.getDwellStr(1)
print "Dwell 2 = " + str(wb6Cfg.getDwell(2))
print "Dwell 2 = " + wb6Cfg.getDwellStr(2)
print "Dwell 3 = " + str(wb6Cfg.getDwell(3))
print "Dwell 3 = " + wb6Cfg.getDwellStr(3)
#print "Dwell 4 = " + str(wb6Cfg.getDwell(4))
#print "Dwell 4 = " + wb6Cfg.getDwellStr(4)

print "Set Point 0 = " + str(wb6Cfg.getSetPoint(0))
print "Dig Out Name 0 = " + str(wb6Cfg.getDigOutName(0))
print "Analogue Out Name 0 = " + str(wb6Cfg.getAnalogueOutName(0))

print "Dig In 0 = " + str(wb6Cfg.getDigInTrigger(0))
print "Dig In 7 = " + str(wb6Cfg.getDigInTrigger(0))

print "Temp Low 0 = " + str(wb6Cfg.getTempTriggerLow(0))
print "Temp High 0 = " + str(wb6Cfg.getTempTriggerHigh(0))
print "Temp Low 4 = " + str(wb6Cfg.getTempTriggerLow(4))
print "Temp High 4 = " + str(wb6Cfg.getTempTriggerHigh(4))

print "Analogue Low 0 = " + str(wb6Cfg.getAnalogueTriggerLow(0))
print "Analogue High 0 = " + str(wb6Cfg.getAnalogueTriggerHigh(0))
print "Analogue Low 3 = " + str(wb6Cfg.getAnalogueTriggerLow(3))
print "Analogue High 3 = " + str(wb6Cfg.getAnalogueTriggerHigh(3))

print "Schedule 0 = " + str(wb6Cfg.getScheduledEvent(0))
print "Schedule 1 = " + str(wb6Cfg.getScheduledEvent(1))
print "Schedule 15 = " + str(wb6Cfg.getScheduledEvent(15))

print "Scene 0 = " + str(wb6Cfg.getScene(0))
print "Scene 1 = " + str(wb6Cfg.getScene(1))
print "Scene 2 = " + str(wb6Cfg.getScene(2))
