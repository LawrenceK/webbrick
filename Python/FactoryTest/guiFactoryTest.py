import Tkinter

from wbFactoryTestConfig import *

import wb6
import wbUdpEvents2
from wb6Status import wb6Status

# need to be able to parrallel some tests?
testDef = [
        # Name, Count
        ( "FactoryReset", 12, ),
        ( "Digital", 12, ),
        ( "Analogue", 4, ),
        ( "Temperature", 5, ),
        ( "Relays", 2, ),
        ( "Serial", 1, ),
        ( "Clock", 1, ),
        ( "testMimicVolts", 3, ),   # 3 tests off, 5V and 12V
        ]

# when started read wbStatus and wbCmd. Create logging channel based on MAC address.
# This is target for UDP events from webbricks.        
class myTarget( wbUdpEvents2.wbEventTarget ):
    def __init__(self):
        logging.info( 'myTarget for UDP events' )
        self._testWb = list()
        self._controlWb = list()

    def handleEvent( self, event ):
        if event.other_data()["ipAdr"] == underTestAddress:
            if not self._testWb or self._testWb[-1].other_data()["seqNr"] != event.other_data()["seqNr"]:
                logging.debug( 'From Under Test %s ' % (event) )
                self._testWb.append( event )
            else:
                logging.debug( 'Duplicate %s ' % (event) )
        elif event.other_data()["ipAdr"] == controlWbAddress:
            if not self._controlWb or self._controlWb[-1].other_data()["seqNr"] != event.other_data()["seqNr"]:
                logging.debug( 'From Control %s ' % (event) )
                self._controlWb.append( event )
            else:
                logging.debug( 'Duplicate %s ' % (event) )
        else:
            logging.debug( 'Not webbricks involved in test %s ' % (event) )

    def getTestEvents( self, filter=None ):
        # return a list of events that match the filter from the under test webbrick
        if filter:
            result = list()
            for evt in self._testWb:
                if filter == evt.type() :
                    result.append(evt)
            return result
        else:
            return self._testWb

    def getControlEvents( self, filter=None ):
        # return a list of events that match the filter from the control webbrick
        if filter:
            result = list()
            for evt in self._controlWb:
                if filter == evt.type() :
                    result.append(evt)
            return result
        else:
            return self._controlWb

class FactoryTestDialog:
# for each test
# label, button,traffic light(s) and free text.
    def __init__(self, master):

        frame = Tkinter.Frame(master)
        frame.pack()
        self.redLight = Tkinter.PhotoImage(file="red.gif")
        self.greenLight = Tkinter.PhotoImage(file="green.gif")
        self.amberLight = Tkinter.PhotoImage(file="amber.gif")

        self.loadTests(frame)

    def sendTestCommand(self, cmd):
        wb6.SendHTTP( underTestAddress,cmd )

    def sendControlCommand(self, cmd):
        # send a command to a webbrick
        wb6.SendHTTP( controlWbAddress,cmd )

    def waitForTestStatus(self):
        # wait for next read of wbstatus and return it.
        # in this test harness we are going to read periodically.
        return wb6Status(underTestAddress)  # for now.

    def waitForControlStatus(self):
        # wait for next read of wbstatus and return it.
        # in this test harness we are going to read periodically.
        return wb6Status(controlWbAddress)

    # Digital In and Digital outs are connected together so testing can be simple.
    def testADigitalChannel( self, chn ):
        # switch all off
        for idx in range(0,8):
            if ( idx == chn ):
                wb6.DigOn( underTestAddress, chn )
            else:
                wb6.DigOff( underTestAddress, idx )

        self.setTest( 'On test Digital channel ' + str(chn) + ' ip address ' + underTestAddress )

        time.sleep(0.1)
        # first read the status XMl until we get what we expect
        # both the control and under test webbrick
        repeat = 0
        while repeat < 3:
            sts = wb6Status(underTestAddress)
            sts2 = wb6Status(controlWbAddress)
            if sts2.getDigIn(chn):
                # mimic correct
                if ( chn <= 3 ) and not sts.getDigIn(idx):
                    # OC correct
                    break
                if ( chn > 3 ) and sts.getDigIn(idx):
                    # DigOut correct
                    break
            repeat = repeat + 1
            time.sleep(0.5)

        # verify only channel under test on
        for idx in range(0,8):
            # if the same
            if ( idx > 3 ):
                if ( idx == chn ):
                    if not sts.getDigIn(idx):
                        self.error( 'channel ' + str(chn) + ' Should be On' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
                else:
                    if sts.getDigIn(idx):
                        self.error( 'channel ' + str(idx) + ' Should be Off' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
            else:
                if ( idx == chn ):
                    if sts.getDigIn(idx):
                        self.error( 'channel ' + str(chn) + ' Should be On' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
                else:
                    if not sts.getDigIn(idx):
                        self.error( 'channel ' + str(idx) + ' Should be Off' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1

            if ( idx == chn ):
                if not sts2.getDigIn(idx):
                    self.error( 'channel ' + str(chn) + ' Mimic Should be On' )
                    self.MimicErrorCount = self.MimicErrorCount + 1
            else:
                if sts2.getDigIn(idx):
                    self.error( 'channel ' + str(idx) + ' Mimic Should be Off' )
                    self.MimicErrorCount = self.MimicErrorCount + 1

        # verify relays
        for idx in range(8,12):
            if (idx/2) == chn :
                if not sts.getDigIn(idx):
                    self.error( 'test Relay channel ' + str(chn) + 'Monitor ' + str(idx) + ' High' )
                    self.DigitalErrorCount = self.DigitalErrorCount + 1
            else:
                if sts.getDigIn(idx):
                    self.error( 'test Relay channel ' + str(chn) + 'Monitor ' + str(idx) +  ' Low' )
                    self.DigitalErrorCount = self.DigitalErrorCount + 1

        # switch all on
        self.setTest( 'Off test Digital channel ' + str(chn) + ' ip address ' + underTestAddress )

        for idx in range(0,8):
            if ( idx == chn ):
                wb6.DigOff( underTestAddress, chn )
            else:
                wb6.DigOn( underTestAddress, idx )
        time.sleep(0.1)

        # read the status XMl until we get what we expect
        # both the control and under test webbrick
        repeat = 0
        while repeat < 3:
            sts = wb6Status(underTestAddress)
            sts2 = wb6Status(controlWbAddress)
            if not sts2.getDigIn(chn):
                # mimic correct
                if ( chn <= 3 ) and sts.getDigIn(idx):
                    # OC correct
                    break
                if ( chn > 3 ) and not sts.getDigIn(idx):
                    # DigOut correct
                    break
            repeat = repeat + 1
            time.sleep(0.5)

        # verify only channel under test off
        for idx in range(0,8):
            # if the same
            if ( idx > 3 ):
                if ( idx == chn ):
                    if sts.getDigIn(idx):
                        self.error( 'channel ' + str(chn) +' Should be Off' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
                else:
                    if not sts.getDigIn(idx):
                        self.error( 'channel ' + str(idx) + ' Should be On' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
            else:
                if ( idx == chn ):
                    if not sts.getDigIn(idx):
                        self.error( 'channel ' + str(chn) +' Should be Off' )
                        self.DigitalErrorCount = self.DigitalErrorCount + 1
                else:
                    if sts.getDigIn(idx):
                        self.error( 'channel ' + str(idx) + ' Should be On' )
                        self.MimicErrorCount = self.DigitalErrorCount + 1
            if ( idx == chn ):
                if sts2.getDigIn(idx):
                    self.error( 'channel ' + str(chn) + ' Mimic Should be Off' )
                    self.MimicErrorCount = self.MimicErrorCount + 1
            else:
                if not sts2.getDigIn(idx):
                    self.error( 'channel ' + str(idx) + ' Mimic Should be On' )
                    self.MimicErrorCount = self.MimicErrorCount + 1

        # verify relays again
        for idx in range(8,12):
            if (idx/2) == chn :
                if sts.getDigIn(idx):
                    self.error( 'test Relay channel ' + str(chn) + 'Monitor ' + str(idx) + ' High' )
                    self.DigitalErrorCount = self.DigitalErrorCount + 1
            else:
                if not sts.getDigIn(idx):
                    self.error( 'test Relay channel ' + str(chn) + 'Monitor ' + str(idx) +  ' Low' )
                    self.DigitalErrorCount = self.DigitalErrorCount + 1
                    
    def testAn( self, chn ):
        self.setTest( 'test Analogue channel ' + str(chn) + ' ip address ' + underTestAddress )
        # for analogue level from 0 to 100% in 10% steps.

        # We are going to stop at 80 % due to power supply problems limiting an out 
        for i in range(0,81,20):
            # set all channels to this level.
            for j in range(0, 4):
                if j != chn:
                    wb6.AnOutPercent( underTestAddress, j, i )
                else:
                    wb6.AnOutPercent( underTestAddress, j, 0 )
            time.sleep(0.4) # let settle, possibly long Fade time
            # for channel under analogue level from 0 to 100% in 10% steps.
            for k in range(0, 81, 20):
                self.showWorking()
                wb6.AnOutPercent( underTestAddress, chn, k )
                time.sleep(0.2) # Fade time
                sts = wb6Status(underTestAddress)
                for j in range(0, 4):
                    val = sts.getAnIn(j)
                    if j == chn :
                        # should be k
                        if ( val < ( k - 5 ) ) or ( val > ( k + 5 ) ) :
                            self.error( 'test Analogue channel %i expecting %s got %s' % (chn, k, val) )
                    else:
                        # should be i
                        if ( val < ( i - 5 ) ) or ( val > ( i + 5 ) ) :
                            self.error( 'test Analogue channel %i verify %i expecting %s got %s' % (chn, j, i, val) )
                    
    def testAnFast( self, chn ):
        self.setTest( 'test Analogue channel ' + str(chn) + ' ip address ' + underTestAddress )
        # for analogue level from 0 to 100% in 10% steps.

        # set all channels to 50 %
        wb6.AnOutPercent( underTestAddress, 0, 0 )
        for j in range(0, 4):
            if j != chn:
                wb6.AnOutPercent( underTestAddress, j, 50 )
            
        #time.sleep(0.4) # let settle, possibly long Fade time
        # for channel under analogue level from 0 to 100% in 10% steps.
        for k in range(0, 81, 20):
            self.showWorking()
            wb6.AnOutPercent( underTestAddress, chn, k )
#            time.sleep(0.2) # Fade time
            time.sleep(0.1) # Fade time
            sts = wb6Status(underTestAddress)
            for j in range(0, 4):
                val = sts.getAnIn(j)
                if j == chn :
                    # should be k
                    if ( val < ( k - 5 ) ) or ( val > ( k + 5 ) ) :
                        self.error( 'test Analogue channel %i expecting %s got %s' % (chn, k, val) )
                else:
                    # should be i
                    if ( val < ( 45 ) ) or ( val > ( 55 ) ) :
                        self.error( 'test Analogue channel %i verify %i expecting %s got %s' % (chn, j, 50, val) )

    def expectMimicVolts(self, lo, hi):
        # retries Mimic read until in range
        rdLimit = 5
        val = 0

        while rdLimit > 0:
            sts2 = self.waitForControlStatus()
            val = sts2.getAnIn(0)
            if ( val >= lo ) and ( val <= hi ) :
                break
            time.sleep(1.0)
            rdLimit = rdLimit - 1

        if ( val < lo ) or ( val > hi ) :
            self.error( 'test Mimic volts got %s expecting between %s and %s' % (val, lo, hi) )

    def testMimicVolts(self):
        # default should be five volts
        self.sendTestCommand("SM0;63;1;0" )    # make sure Mimics full off or full on and fade fast & 5V

        wb6.DigOn( underTestAddress, 0 )
        self.expectMimicVolts(15, 30)
        
        self.sendTestCommand("SM0;63;1;1" )    # make sure Mimics full off or full on and fade fast & 12V
        self.expectMimicVolts(50, 80)

        self.sendTestCommand("SM0;63;1;0" )    # make sure Mimics full off or full on and fade fast & 5V
        wb6.DigOff( underTestAddress, 0 )
        
        self.expectMimicVolts(-5, 5)    # now off

        self.assertEqual( self.ErrorCount, 0 )

    def startTest(self, name, lbl):
        lbl.configure(text="run %s"%(name))
        
    def loadTests(self, frame):
        row = 0
        for tst in testDef:
                lb1 = Tkinter.Label(frame, text=tst[0])
                lb1.grid(row=row, column=0)
                lb2 = Tkinter.Label(frame, text="", width=20)
                lb2.grid(row=row, column=2)
                lb3 = Tkinter.Label(frame, image=self.redLight)
                lb3.grid(row=row, column=3)
                # The jiggery pokery in the lambda is so that the binding is not to the other values.
                # http://mail.python.org/pipermail/python-list/2006-September/403259.html
                bt1 = Tkinter.Button(frame, text="Run", command=lambda name=tst[0], lbl=lb2: self.startTest(name, lbl))
                bt1.grid(row=row, column=1)
#                bt1 = Button(frame, text="Run", command=self.runTest(tst[0], lb2)).grid(row=row, column=1)
                row = row + 1

root = Tkinter.Tk()

app = FactoryTestDialog(root)

root.mainloop()


"""
from Tkinter import *

class MyDialog:

    def __init__(self, parent):

        top = self.top = Toplevel(parent)

        Label(top, text="Value").pack()

        self.e = Entry(top)
        self.e.pack(padx=5)

        b = Button(top, text="OK", command=self.ok)
        b.pack(pady=5)

    def ok(self):

        print "value is", self.e.get()

        self.top.destroy()


root = Tk()
Button(root, text="Hello!").pack()
root.update()

d = MyDialog(root)

root.wait_window(d.top)
"""