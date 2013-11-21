Files in this fiolder are intended to be used to regression test a webbrick when firmware 
is changed. 

The main TestAll file is run against a webbrick with minimal connections.
The TestFactory is run against a webbick on a factory test rig that cross connects some inputs and has some 
hardware connected. This later test suite is intended to test the hardware for full operation and not to test all
firmware paths.

The test code uses files from the webbrick python API, these are accessed from ../../python/api.

Lawrence.


30 Mar 2007 Tests thaat could be added

Boundary conditions. Attempt to configure out of range index for all types of channel.
