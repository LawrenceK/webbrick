import socket, wbUdp

class setIp:

    def __init__ (self):
        self.addr = [10,100,100,102]
        self.macAddr = [0x00,0x03,0x75,0x0F,0x72,0x79]
        self.password = "password"
        self.error = 'None'
        self.debug = 0

    def login(self):
        wbUdp.sendFrame( ':LG;'+self.password+':')

    def setIp(self):
        cmd = ':SA'
        for b in self.macAddr:
            cmd = cmd + ';' + str(b)
        for b in self.addr:
            cmd = cmd + ';' + str(b)
        cmd = cmd + ':'
        wbUdp.sendFrame( cmd )

if __name__ == "__main__":
    # WbCfg(sys.argv[1:])
    set = setIp()
    set.login()
    set.setIp()

