#
#  wb6 Excercise
#
#   Trigger Digital and Analogue outputs a lot
#
import sys
sys.path.append('../API')
from time import sleep

import wb6
test_ip = "10.100.100.100";


for trig in range(0,8):
    wb6.DigOff(test_ip,trig)


while 1:
    for trig in range(0,8):
        wb6.DigTrigger(test_ip,trig)
        for ana in range(0,4):
            wb6.AnOutSp(test_ip,ana,trig)
        sleep(0.2)





