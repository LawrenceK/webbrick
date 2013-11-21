#
#  Class to handle Commands to a webbrick and retrieve data from a webbrick
#
#   The aim is for this to provide low level webbrick access, this version uses
#   HTTP exclusively, where as the original used UDP packets.
#
#  o2m8.com
#
#
import logging
import httplib

# Action types.
AT_NONE = 0
AT_OFF = 1
AT_ON = 2
AT_MOMENTARY = 3
AT_TOGGLE = 4
AT_DWELL = 5
AT_DWELLCAN = 6
AT_NEXT = 7
AT_PREV = 8
AT_SETLOW = 9
AT_SETHIGH = 10
AT_ADJUSTLOW = 11
AT_ADJUSTHIGH = 12
AT_SPARE = 13

ActionStrs =  [ "None", "Off", "On", "Momentary", "Toggle", "Dwell", "Dwell-Can", "Next", "Prev","SetLowThreshold","SetHighThreshold","AdjustLowThreshold","AdjustHighThreshold" ]

# UDP types.
UDPT_NONE = 0
UDPT_GENERAL = 1
UDPT_ALARM = 2
UDPT_REMOTE = 3
UDPRemStrs = ["None", "General", "Remote", "Alarm"]

# target types.
TT_DIGITAL = "D"
TT_ANALOGUE = "A"
TT_SCENE = "S"
TT_TEMPERATURE = "T"
ChannelTypeStrs = ["Digital", "Scene", "Analogue", "Temperature" ]

# Set points
SP_0 = 0
SP_1 = 1
SP_2 = 2
SP_3 = 3
SP_4 = 4
SP_5 = 5
SP_6 = 6
SP_7 = 7
SP_8 = 8
SP_9 = 9
SP_10 = 10
SP_11 = 11
SP_12 = 12
SP_13 = 13
SP_14 = 14
SP_15 = 15

# Dwells
DW_0 = 0
DW_1 = 1
DW_2 = 2
DW_3 = 3
DW_4 = 4
DW_5 = 5
DW_6 = 6
DW_7 = 7

DWELLCOUNT = 4
SPCOUNT = 8
DICOUNT = 12
AICOUNT = 4
DOCOUNT = 8
AOCOUNT = 4
TEMPCOUNT = 5
SCHEDCOUNT = 16
SCENECOUNT = 8


#
# WebBrick Http handling. Send and retreve stuff from webbrick.
#
def getStatusXml(adrs):
    return GetXml(adrs, "wbStatus.xml")

def getConfigXml(adrs):
    return GetXml(adrs, "wbCfg.xml")

def DigTrigger(adrs, chn):
    SendHTTP(adrs,"DI" + str(chn) )

def DigOn(adrs, chn):
    SendHTTP(adrs,"DO" + str(chn) + ";N" )

def DigOff(adrs, chn):
    SendHTTP(adrs,"DO" + str(chn) + ";F" )

def DigToggle(adrs, chn):
    SendHTTP(adrs,"DO" + str(chn) + ";T" )

def DigDwell(adrs, chn, DwellNr):
    SendHTTP(adrs,"DO" + str(chn) + ";D;" + str(DwellNr) )

def AnOutSp(adrs, chn, sp):
    SendHTTP(adrs,"AA" + str(chn) + ";S" + str(sp) )

def AnOutPercent(adrs, chn, val):
    SendHTTP(adrs,"AA" + str(chn) + ";" + str(val) )

def SetScene(adrs, sc):
    SendHTTP(adrs,"SC" + str(sc) )

# day 0-6, hour 0-23, minute 0-59    
def Login(adrs, password):
    SendHTTP(adrs,"LG;" + password )

# day 0-6, hour 0-23, minute 0-59    
def SetTime(adrs, day, hr, min):
    assert ( ( day >= 0 ) and ( day < 7 ) and ( hr >= 0 ) and (hr < 24 ) and ( min >= 0 ) and (min < 60 ) ), "Bad Parameter(s)"
    SendHTTP(adrs,"ST" + str(day) + ";" + str(hr) + ";" + str(min) )
        
def ConfigDigIn(adrs, iChn, tgtType, tgtIdx, action, sp, dwell, udpType, nodeNr ):
    # CD<chn>;<ADS><tgtChn>;<sp>;<actionType>;<dwell>;<udpType>;<nodeNr>:
    assert ( ( iChn >= 0 ) and ( iChn < DICOUNT ) and (tgtIdx >= 0 ) and ( tgtIdx <= 15 ) and ( action >= AT_NONE ) and (action < AT_SPARE ) ), "Bad Parameter(s)"
    SendHTTP(adrs,"CD" + str(iChn) + ";" + tgtType + str(tgtIdx) + ";" + str(sp) + ";" + str(action) + ";" + str(dwell) + ";" + str(udpType) + ";" + str(nodeNr) +":" )

def ConfigDigInEx(adrs, iChn, tgtType, tgtIdx, action, sp, dwell, udpType, nodeNr, reserved, opt ):
    # CD<chn>;<ADS><tgtChn>;<sp>;<actionType>;<dwell>;<udpType>;<nodeNr>:
    assert ( ( iChn >= 0 ) and ( iChn < DICOUNT ) and (tgtIdx >= 0 ) and ( tgtIdx <= 15 ) and ( action >= AT_NONE ) and (action < AT_SPARE ) ), "Bad Parameter(s)"
    SendHTTP(adrs,"CD" + str(iChn) + ";" + tgtType + str(tgtIdx) + ";" + str(sp) + ";" + str(action) + ";" + str(dwell) + ";" + str(udpType) + ";" + str(nodeNr) + ";" + str(reserved) + ";" + str(opt) +":" )

def ConfigFadeRate(adrs, rate ):
    # CD<chn>;<ADS><tgtChn>;<sp>;<actionType>;<dwell>;<udpType>;<nodeNr>:
    assert ( rate> 0 ), "Bad Parameter(s)"
    SendHTTP(adrs,"SF" + str(rate) +":" )

def ConfigAnIn(adrs, iChn, thType, thVal, tgtType, tgtIdx, action, sp, dwell, udpType, nodeNr ):
    # CI<chn>;<L|H>;<ADS><tgtChn>;<sp>;<actionType>;<dwell>;<udpType>;<nodeNr>;<val>:
    assert ( ( iChn >= 0 ) and ( iChn < AICOUNT ) and (tgtIdx >= 0 ) and ( tgtIdx <= 15 ) and ( action >= AT_NONE ) and (action < AT_SPARE ) ), "Bad Parameter(s)"
    SendHTTP(adrs,"CI" + str(iChn) + ";" + thType + str(thVal) +";" + tgtType + str(tgtIdx) + ";" + str(sp) + ";" + str(action) + ";" + str(dwell) + ";" + str(udpType) + ";" + str(nodeNr) +":" )

def ConfigTemp(adrs, iChn, thType, thVal, tgtType, tgtIdx, action, sp, dwell, udpType, nodeNr ):
    # CT<chn>;<L|H>;<ADS><tgtChn>;<sp>;<actionType>;<dwell>;<udpType>;<nodeNr>;<val>:
    assert ( ( iChn >= 0 ) and ( iChn < TEMPCOUNT ) and (tgtIdx >= 0 ) and ( tgtIdx <= 15 ) and ( action >= AT_NONE ) and (action < AT_SPARE ) ), "Bad Parameter(s)"
    SendHTTP(adrs,"CT" + str(iChn) + ";" + thType + str(thVal) +";" + tgtType + str(tgtIdx) + ";" + str(sp) + ";" + str(action) + ";" + str(dwell) + ";" + str(udpType) + ";" + str(nodeNr) +":" )

def ConfigScheduled(adrs, iEv, days, hour, min, tgtType, tgtIdx, action, sp, dwell, udpType, nodeNr ):
    # CE<num>;<Days>;<Hours>;<Mins>;<ADS><tgtChn>;<sp>;<actionType>;<dwell>;<udpType>;<nodeNr>;<val>:
    assert ( ( iEv >= 0 ) and ( iEv <= SCHEDCOUNT ) and (tgtIdx >= 0 ) and ( tgtIdx <= 15 ) and ( action >= AT_NONE ) and (action < AT_SPARE ) ), "Bad Parameter(s)"
    SendHTTP(adrs,"CE" + str(iEv) + ";" + days + ";" + str(hour) + ";" + str(min) +";" + tgtType + str(tgtIdx) + ";" + str(sp) + ";" + str(action) + ";" + str(dwell) + ";" + str(udpType) + ";" + str(nodeNr) +":" )

def ConfigScene(adrs, iScene, digOut, anOut ):
    # CC\param{nr};[NFI][NFI][NFI][NFI][NFI][NFI][NFI][NFI];[I|S\param{nn}];[I|S\param{nn}];[I|S\param{nn}];[I|S\param{nn}]:
    assert ( ( iScene >= 0 ) and ( iScene <= SCENECOUNT ) and ( len(digOut) == DOCOUNT ) and ( len(anOut) == AOCOUNT ) ), "Bad Parameter(s)"
    cmd = "CC" + str(iScene) + ";"
    for d in digOut:
        assert d in ["N","F","I"], "Bad digital parameter"
        cmd = cmd + d
    for a in anOut:
        cmd = cmd + ";"
        if ( a == "I" ):
            cmd = cmd + a
        elif (a[0] == "S"):
            assert a[1] >= 0 and a[1] < SPCOUNT, "Bad scene number"
            cmd = cmd + "S" + str(a[1])
        else:
            assert False, "Bad analogue parameter"
    cmd = cmd + ":"
    SendHTTP(adrs,cmd )

def Send(adrs,cmd):
    # include colon in case caller misses them out.
    # the first colon is to flush any crap in the receive buffer.
    SendHTTP(adrs,":"+cmd+":" )

def GetXml(adrs,xmlName):
    result = ""
    lines = GetHTTPlines(adrs,"/"+xmlName)
    for line in lines:
        result += str(line)
    return result
    
# ------- --------- --------- --------- --------- ---------
# Logic for sending commands to a WebBrick
# ------- --------- --------- --------- --------- ---------

def isUriUnreserved(c):
    return ( c.isalpha() or c.isdigit() or (c in "-_.~") )

def isUriReserved(c):
    return (c in (":/?#[]@"+"!$&'()*+,;="))     # gendelims + subdelims

def MakeHex(char, length):
    # From: http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/82748
    # hex representation of character/integer
    char = ord(char)
    x    = hex(char)[2:]
    while len(x) < length:
        x = '0' + x
    return x

def SendHTTP(adrs,cmd):
    """
    Send a command to a WebBrick via an HTTP transaction.
    The command is preceded and followed by a colon ):), and encoded
    in a URI, with %-encoding applied as necessary.
    """
    h   = httplib.HTTP(adrs)
    uri = "/hid.spi?com=%3A"
    for c in cmd:
        if isUriUnreserved(c):
            uri = uri + c
        else:
            uri = uri + "%" + MakeHex(c,2)
    uri = uri + "%3A"
    logging.debug("HTTP GET "+uri)
    h.putrequest( "GET", uri )
    h.endheaders()
    errcode, errmsg, headers = h.getreply()
    if (errcode != 200) and (errcode != 302):
        print("HTTP error "+str(errcode)+": "+errmsg+"\n")
        return None
    return h

def GetHTTPlines(adrs,uri):
    """
    Return content of an HTTP resource as a sequence lines (including 
    terminating newlines), or None if the resource cannot be read.
    """
    h = httplib.HTTP(adrs)
    logging.debug("HTTP GET "+uri)
    h.putrequest( "GET", uri )
    h.endheaders()
    errcode, errmsg, headers = h.getreply()
    if (errcode != 200):
        print("HTTP error "+str(errcode)+": "+errmsg+"\n")
        return None
    f = h.getfile()
    respdata = f.readlines()
    f.close()
    return respdata
