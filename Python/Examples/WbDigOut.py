#! /usr/local/bin/python
import sys, getopt, string
import wb6

def help_message():
    print '''WbDigOut.py   Direct control of a WebBrick output channel
operands are On,OfF,Toggle
Usage :  WbDigOut.py <Wb-IP-addr> chn operand'''
    sys.exit(0)

if (sys.argv[1] and sys.argv[2] and sys.argv[3]):
    if sys.argv[3] = "On":
        wb6.DigOn( sys.argv[1], sys.argv[2] )
    elif sys.argv[3] = "Off":
        wb6.DigOff( sys.argv[1], sys.argv[2] )
    elif sys.argv[3] = "Toggle":
        wb6.DigToggle( sys.argv[1], sys.argv[2] )
    else:
        print 'Bad action'
        help_message()
else:
    print 'Insufficient arguments'
    help_message()
