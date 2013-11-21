# The IP address we expect the webbrick under test to be on.
underTestAddress  = "10.100.100.100"

LpkTestEnv = True
#LpkTestEnv = False

FirstTest = True
# Numbering in pyserial is from 0 = COM1

if LpkTestEnv:
    useSerialForRs485  = True  # Test rig use False here
    controlWbAddress  = "10.100.100.100"
    rs232PortName = 8
    rs485PortName = 15  # only used for LPK test
else:
    useSerialForRs485  = False  # Test rig use False here
    # The IP address for the control webbrick
    controlWbAddress  = "10.100.100.250"
    rs232PortName = "COM1"  # Test Rig

underTestPassword  = "installer"
controlPassword  = "installer"

# switches relay over
CONTROL_SERIAL_RELAY    = 0

