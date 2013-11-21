import sys
sys.path.append('../API')

import wb6Status

test_ip = "10.100.100.100";

wb6Sts = wb6Status.wb6Status( test_ip );

print "Version = " + wb6Sts.getVersion()
print "OWBus = " + wb6Sts.getOneWireBus()

print "Temp 0 = " + str(wb6Sts.getTemp( 0))
print "Temp 1 = " + str(wb6Sts.getTemp( 1))
print "Temp 2 = " + str(wb6Sts.getTemp( 2))
print "Temp 3 = " + str(wb6Sts.getTemp( 3))
print "Temp 4 = " + str(wb6Sts.getTemp( 4))

print "An Out 0 = " + str(wb6Sts.getAnOut( 0))
print "An Out 1 = " + str(wb6Sts.getAnOut( 1))
print "An Out 2 = " + str(wb6Sts.getAnOut( 2))
print "An Out 3 = " + str(wb6Sts.getAnOut( 3))

print "An In 0 = " + str(wb6Sts.getAnIn( 0))
print "An In 1 = " + str(wb6Sts.getAnIn( 1))
print "An In 2 = " + str(wb6Sts.getAnIn( 2))
print "An In 3 = " + str(wb6Sts.getAnIn( 3))

print "Dig In 0 = " + str(wb6Sts.getDigIn( 0))
print "Dig In 1 = " + str(wb6Sts.getDigIn( 1))
print "Dig In 2 = " + str(wb6Sts.getDigIn( 2))
print "Dig In 3 = " + str(wb6Sts.getDigIn( 3))
print "Dig In 4 = " + str(wb6Sts.getDigIn( 4))
print "Dig In 5 = " + str(wb6Sts.getDigIn( 5))
print "Dig In 6 = " + str(wb6Sts.getDigIn( 6))
print "Dig In 7 = " + str(wb6Sts.getDigIn( 7))

print "Monitor 0 = " + str(wb6Sts.getMonitor( 0))
print "Monitor 1 = " + str(wb6Sts.getMonitor( 1))
print "Monitor 2 = " + str(wb6Sts.getMonitor( 2))
print "Monitor 3 = " + str(wb6Sts.getMonitor( 3))

print "Dig Out 0 = " + str(wb6Sts.getDigOut( 0))
print "Dig Out 1 = " + str(wb6Sts.getDigOut( 1))
print "Dig Out 2 = " + str(wb6Sts.getDigOut( 2))
print "Dig Out 3 = " + str(wb6Sts.getDigOut( 3))
print "Dig Out 4 = " + str(wb6Sts.getDigOut( 4))
print "Dig Out 5 = " + str(wb6Sts.getDigOut( 5))
print "Dig Out 6 = " + str(wb6Sts.getDigOut( 6))
print "Dig Out 7 = " + str(wb6Sts.getDigOut( 7))

print "Last cmd error = " + str(wb6Sts.getCmdError())
print "Operational State = " + str(wb6Sts.getOperationalState())
print "LoginState = " + str(wb6Sts.getLoginState())
print "Date = " + str(wb6Sts.getDate())
print "Time = " + str(wb6Sts.getTime())
print "Day = " + str(wb6Sts.getDay())
