import wb6
import xml.dom
import xml.dom.minidom

#from Logging import Trace, Warn, Error

class wb6Status:
    def __init__ (self, adrs):
        xmlstr = wb6.getStatusXml( adrs )
        # Trace("getStatusXml:\n"+xmlstr+"\n")
        self.dom = xml.dom.minidom.parseString( xmlstr )
        
    def getFirstTextNodeValue(self, elem ):
        for node in elem.childNodes:
            if ( node.nodeType == node.TEXT_NODE ):
                return node.nodeValue
        return ""
        
    def getError(self):
        nodeList = self.dom.getElementsByTagName("Error")
        return int(self.getFirstTextNodeValue(nodeList[0]))

    def getOpState(self):
        nodeList = self.dom.getElementsByTagName("Context")
        return int(self.getFirstTextNodeValue(nodeList[0]))

    def getLoginState(self):
        nodeList = self.dom.getElementsByTagName("LoginState")
        return int(self.getFirstTextNodeValue(nodeList[0]))

    def getOneWireBus(self):
        nodeList = self.dom.getElementsByTagName("OWBus")
        return int(self.getFirstTextNodeValue(nodeList[0]))

    def getTemp(self, idx):
        nodeList = self.dom.getElementsByTagName("Tmp")
        curTemp = self.getFirstTextNodeValue(nodeList[idx])
        curTemp = float(curTemp) / 16.0
        if ( curTemp > 127.0 ):
            curTemp -= 256.0
        return curTemp
        
    def getAnOut(self, idx):
        nodeList = self.dom.getElementsByTagName("AO")
        val = float(self.getFirstTextNodeValue(nodeList[idx]))
        return val
    
    def getAnIn(self, idx):
        nodeList = self.dom.getElementsByTagName("AI")
        val = float(self.getFirstTextNodeValue(nodeList[idx]))
        return val
        
    def getDigIn(self, idx):
        nodeList = self.dom.getElementsByTagName("DI")
        val = int(self.getFirstTextNodeValue(nodeList[0]))
        if ( val & ( 0x01 << idx ) ) != 0 :
            return True
        return False
        
    def getMonitor(self, idx):
        return self.getDigIn(idx+8)
        
    def getDigOut(self, idx):
        nodeList = self.dom.getElementsByTagName("DO")
        val = int(self.getFirstTextNodeValue(nodeList[0]))
        if ( val & ( 0x01 << idx ) ) != 0 :
            return True
        return False
    
    def getDate(self):
        nodeList = self.dom.getElementsByTagName("Date")
        return self.getFirstTextNodeValue(nodeList[0])
    
    def getTime(self):
        nodeList = self.dom.getElementsByTagName("Time")
        ts = self.getFirstTextNodeValue(nodeList[0])
        # may be better to parse and recreate.
        if ( ts[1] == ':' ):
            # pad hour.
            ts = '0' +ts
        if ( ts[4] == ':' ):
            # pad minute
            ts = ts[0:3]+ '0' +ts[3:]
        if ( len(ts) == 7 ):
            # pad second
            ts = ts[0:6]+ '0' +ts[6:]
        return ts
        
    def getDay(self):
        nodeList = self.dom.getElementsByTagName("Day")
        return int(self.getFirstTextNodeValue(nodeList[0]))
        
    def getVersion(self):
        nodeList = self.dom.getElementsByTagName("WebbrickStatus")
        return str(nodeList[0].attributes["Ver"].value)
        
    def getOperationalState(self):
        nodeList = self.dom.getElementsByTagName("Context")
        return int(self.getFirstTextNodeValue(nodeList[0]))
        
    def getLoginState(self):
        nodeList = self.dom.getElementsByTagName("LoginState")
        return int(self.getFirstTextNodeValue(nodeList[0]))
        
    def getCmdError(self):
        nodeList = self.dom.getElementsByTagName("Error")
        return int(self.getFirstTextNodeValue(nodeList[0]))
