# $Id:$
#
import sys, time, logging

sys.path.append("../../Python/API")
import wb6

def showWorking():
    sys.stdout.write(".")

def showWait():
    sys.stdout.write("w")


def doLogin(wbAddress):
    logging.debug( "doLogin" )
    wb6.Login( wbAddress, "installer" )
    return

def doReboot(wbAddress):
    logging.debug( "doReboot" )
    # takes a while.
    wb6.Send(wbAddress, "RB" )
    time.sleep(5.0)
    return

def factoryReset(wbAddress):
    logging.debug( "factoryReset" )
    doLogin(wbAddress)
    # send factory reset, full reset
    wb6.Send(wbAddress, "FR1" )
    time.sleep(8.0 )# takes a while.
    return
