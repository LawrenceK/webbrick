# $Id: TestAll.py 807 2006-07-03 14:54:44Z lawrence $
#
# Unit testing for WebBrick library functions (Functions.py)
# See http://pyunit.sourceforge.net/pyunit.html
#

import sys
import unittest, logging

import TestWebBrick

# Code to run unit tests from all library test modules
def getTestSuite():
    suite = unittest.TestSuite()
    suite.addTest(TestWebBrick.getTestSuite())
    return suite

if __name__ == "__main__":

    if len(sys.argv) > 1:
        logging.basicConfig(level=logging.DEBUG)
        tests = TestWbEvStatusQuery( sys.argv[1] )
    else:
        logging.basicConfig(level=logging.INFO)
        tests = getTestSuite()

    runner = unittest.TextTestRunner(verbosity=2)
    runner.run(tests)
