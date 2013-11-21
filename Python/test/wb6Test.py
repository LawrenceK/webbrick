import sys
sys.path.append('../API')

import datetime
import wb6

test_ip = "10.100.100.101";

now = datetime.datetime.now()

# datetime starts on monday
day = now.weekday() + 1
if ( day == 7 ):
    day = 0
    
wb6.Login( test_ip, "password" );
wb6.SetTime( test_ip, day, now.hour, now.minute )

