import wb6
import xml.dom
import xml.dom.minidom

class wb6Config:
    ToDStrs = ["Commands Disabled", "Startup", "Normal Operation", "Quiescent Operation"]
#    AStrs = ["Off", "Enabled"]
    OneWStrs = ["No Sensors Found", "Bus Good, Sensor Found", "Bus Good, Reading Good", "Bus Good, Software Error", "Bus Bad, Held Low"]

    def __init__ (self, adrs):
        str = wb6.getConfigXml( adrs )
        self.dom = xml.dom.minidom.parseString( str )
        
    def getFirstTextNodeValue(self, elem ):
        for node in elem.childNodes:
            if ( node.nodeType == node.TEXT_NODE ):
                return node.nodeValue
        return ""
        
    def getVersion(self):
        nodeList = self.dom.getElementsByTagName("WebbrickConfig")
        return str(nodeList[0].attributes["Ver"].value)
        
    def getNodeName(self):
        nodeList = self.dom.getElementsByTagName("NN")
        return self.getFirstTextNodeValue(nodeList[0])

    def getNodeNumber(self):
        nodeList = self.dom.getElementsByTagName("SN")
        return int(self.getFirstTextNodeValue(nodeList[0]))
        
    def getFadeRate(self):
        nodeList = self.dom.getElementsByTagName("SF")
        return int(self.getFirstTextNodeValue(nodeList[0]))
    
    def getIpAddress(self):
        nodeList = self.dom.getElementsByTagName("SI")
        return nodeList[0].attributes["ip"].value
        
    def getMacAddress(self):
        nodeList = self.dom.getElementsByTagName("SI")
        return nodeList[0].attributes["mac"].value
        
    def getRotary(self, idx):
        nodeList = self.dom.getElementsByTagName("SR")
        s = nodeList[idx].attributes["Value"].value
        return int(s)
    
    def getDwell(self, idx):
        nodeList = self.dom.getElementsByTagName("CW")
        return int(self.getFirstTextNodeValue(nodeList[idx]))
    
    def getDwellStr(self,idx):
        dw = self.getDwell(idx)
        if (dw<60):
            return ( str(dw) + ' Secs')
        if (dw<=3600):
            return ( str(round(dw/60)) + ' Mins')
        return (str(round(dw/360)/10) + ' Hours')
        
    def getSetPoint(self,idx):
        nodeList = self.dom.getElementsByTagName("CS")
        return int(self.getFirstTextNodeValue(nodeList[idx]))
        
    def getName(self, type, idx):
        nodeList = self.dom.getElementsByTagName(type)
        return nodeList[idx].attributes["Name"].value
        
    def getDigOutName(self, idx):
        return self.getName('NO', idx)
        
#    def getMonitorName(self, idx):
#        return self.getName('NM', idx)
        
    def getAnalogueOutName(self, idx):
        return self.getName('NA', idx)
        
    def decodeTrigger(self, elem):
        result = dict() # empty dictionary
        b1 = int(elem.attributes["B1"].value)
        b2 = int(elem.attributes["B2"].value)
        b3 = int(elem.attributes["B3"].value)
        
        
        result["actionNr"] = (b1 & 0x0F)
        result["action"] = wb6.ActionStrs[result["actionNr"]]
        result["dwell"] = ((b1 & 0x30) /16)
        result["UDPRemNr"] = ((b1 & 0xC0) /64)
        result["UDPRem"] = wb6.UDPRemStrs[result["UDPRemNr"]]
        result["RemNode"] = b3
        
        if ((b2 & 0x80) != 0):
            result["typeNr"] = 2
            result["setPoint"] = (b2 & 0x0F)
            result["pairChn"] = ((b2 & 0x70) /16)
        else:
            if ((b2 & 0x40) != 0):
                result["typeNr"] = 1
            else:
                result["typeNr"] = 0
            result["setPoint"] = 0
            result["pairChn"] = (b2 & 0x0F)
        result["type"] = wb6.ChannelTypeStrs[result["typeNr"]]
        
        return result
        
    def getDigInTrigger(self, idx):
        nodeList = self.dom.getElementsByTagName("CD")
        node = nodeList[idx]
        result = self.decodeTrigger( node.getElementsByTagName("Trg")[0])
        result["name"] = node.attributes["Name"].value
        result["options"] = int(node.attributes["Opt"].value)
        return result
        
    def getTempTriggerLow(self, idx):
        nodeList = self.dom.getElementsByTagName("CT")
        node = nodeList[idx]
        trg = node.getElementsByTagName("TrgL")[0]
        result = self.decodeTrigger( trg)
        result["name"] = node.attributes["Name"].value
        result["threshold"] = float(trg.attributes["Lo"].value)/16.0
        return result
        
    def getTempTriggerHigh(self, idx):
        nodeList = self.dom.getElementsByTagName("CT")
        node = nodeList[idx]
        trg = node.getElementsByTagName("TrgH")[0]
        result = self.decodeTrigger( trg)
        result["name"] = node.attributes["Name"].value
        result["threshold"] = float(trg.attributes["Hi"].value)/16.0
        return result
        
    def getAnalogueTriggerLow(self, idx):
        nodeList = self.dom.getElementsByTagName("CI")
        node = nodeList[idx]
        trg = node.getElementsByTagName("TrgL")[0]
        result = self.decodeTrigger( trg)
        result["name"] = node.attributes["Name"].value
        result["threshold"] = int(trg.attributes["Lo"].value)
        return result
        
    def getAnalogueTriggerHigh(self, idx):
        nodeList = self.dom.getElementsByTagName("CI")
        node = nodeList[idx]
        trg = node.getElementsByTagName("TrgH")[0]
        result = self.decodeTrigger( trg)
        result["name"] = node.attributes["Name"].value
        result["threshold"] = int(trg.attributes["Hi"].value)
        return result
        
    def getScheduledEvent(self, idx):
        nodeList = self.dom.getElementsByTagName("CE")
        node = nodeList[idx]
        result = self.decodeTrigger( node.getElementsByTagName("Trg")[0])
        result["days"] = int(node.attributes["Days"].value)
        result["hours"] = int(node.attributes["Hours"].value)
        result["mins"] = int(node.attributes["Mins"].value)
        return result
        
    def getScene(self, idx):
        result = dict() # empty dictionary
        nodeList = self.dom.getElementsByTagName("CC")
        node = nodeList[idx]
        Dm = int(node.attributes["Dm"].value)
        Ds = int(node.attributes["Ds"].value)
        Am = int(node.attributes["Am"].value)
        Av = int(node.attributes["Av"].value)
        
        for i in range(8):
            if ( ( Dm & ( 1 << i) ) != 0 ):
                if ( ( Ds  & ( 1 << i) ) != 0 ):
                    result["Digital"+str(i)] = "On"
                else:
                    result["Digital"+str(i)] = "Off"
            else:
                result["Digital"+str(i)] = "Ignore"
                
        for i in range(4):
            if ( ( Am & ( 1 << i) ) != 0 ):
                result["Analogue"+str(i)] = "SetPoint"+str(Av & 0x0F)
            else:
                result["Analogue"+str(i)] = "Ignore"
            Av >>= 4
        
        return result
