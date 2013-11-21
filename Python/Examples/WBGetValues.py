#
#  Kill a 6.1 WebBrick
#
#
import httplib, threading

adrs = "10.100.100.100"
uri = "/values.inc"


def doConn(i):
        print "Starting WebBrick Connection %d" % i
        conn = httplib.HTTPConnection(adrs)
        print "Sending  try"
        conn.request("GET",uri)
        res = conn.getresponse()
        print "Try %d gave %d %s" % (i,res.status,res.reason)


def main():
    tasks = {}
    for i in range(0,10):
        tasks[i] = threading.Thread(target=doConn(i))
    for i in range(0,10):
        tasks[i].start()        

if __name__ == '__main__':
    main()    
