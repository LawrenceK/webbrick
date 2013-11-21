This directory contains patch files against Forest Electronic Developments wizc V14.01 
(http://www.fored.co.uk/html/wiz_c_mx_-__pic_rapid_developm.HTM)

You have two choices there is a single large patch file full.patch which can be against the installed copy of pixie 
or there are in the various sub directories various new files and patch files for each modified files.

Changes I can remember.

i) Add functions to 1 wire to handle single bit transmission and reception, used for the address search.
ii) The 1 Wire reset/presence detect needs to wait for a time period before looking for the low pulse from the device, on
long wires with a fast CPU (40Mhz PIX18F6722) the bus does not return high from the master reset pulse before the 
bus is sampled looking for low. By waiting a time slot first it has a chnace to return high.
iii) Duplicate the interrupt drivene serial element to use the second USART on the 18F6722 and family.
iv) Add the IIC library from David Kerr. This was modified to handle single byte addresses as it was used in
the webbrick to talk to a RTC chip over I2C
v) Add an element to do software PWM of an 8 bit port. This uses an interrupt handler to chnage the output port.
The element uses an array of memory bytes so the interrupt handler just sets the new port value and increments the
array index. The interrupt is circa 20mS. In a webbrick this is used to drive the LED mimic outputs.

There are possibly other changes, these may of been applied within later versions of wizc.

