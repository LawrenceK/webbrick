# A Demo of services and named pipes.

# A multi-threaded service that simply echos back its input.

# * Install as a service using "pipeTestService.py install"
# * Use Control Panel to change the user name of the service
#   to a real user name (ie, NOT the SystemAccount)
# * Start the service.
# * Run the "pipeTestServiceClient.py" program as the client pipe side.

import win32serviceutil, win32service
import pywintypes, win32con, winerror
# Use "import *" to keep this looking as much as a "normal" service
# as possible.  Real code shouldn't do this.
#from win32event import *
#from win32file import *
#from win32pipe import *
from win32api import *
#from ntsecuritycon import *

# Old versions of the service framework would not let you import this
# module at the top-level.  Now you can, and can check 'Debugging()' and
# 'RunningAsService()' to check your context.
import servicemanager

import traceback
import thread
import time
import HarmonyMain, HarmonyLogging

def ApplyIgnoreError(fn, args):
    try:
        return apply(fn, args)
    except error: # Ignore win32api errors.
        return None

class HarmonyService(win32serviceutil.ServiceFramework):
    _svc_name_ = "HarmonyWebBrickInterface"
    _svc_display_name_ = "Harmony WebBrick Interface"
    _svc_description_ = "Acsts as an interface between Marmony Home automation server and one or more WebBricks (www.o2m8.com)"

    def __init__(self, args):
        HarmonyLogging.configure()
        win32serviceutil.ServiceFramework.__init__(self, args)
        self.running = False
        
    def SvcStop(self):
        self.ReportServiceStatus(win32service.SERVICE_STOP_PENDING)
        self.running = False
        self.harmony.stop()

    def SvcDoRun(self):
        # Write an event log record - in debug mode we will also
        # see this message printed.
        servicemanager.LogMsg(
                servicemanager.EVENTLOG_INFORMATION_TYPE,
                servicemanager.PYS_SERVICE_STARTED,
                (self._svc_name_, '')
                )

        # Create the active object.
        self.harmony = HarmonyMain.HarmonyMain()
        self.harmony.start()
        self.running = True

        num_connections = 0
        while self.running and self.harmony.alive():
            # it is all on deamon threads
            time.sleep(1)   

        # Write another event log record.
        servicemanager.LogMsg(
                servicemanager.EVENTLOG_INFORMATION_TYPE,
                servicemanager.PYS_SERVICE_STOPPED,
                (self._svc_name_, " after processing %d connections" % (num_connections,))
                )

 def customOptionHandler(opts):
    #This is only called when the service is installed.
    fFoundConfigFile = 0
    for opt, val in opts:
        if opt == "-c":
            # This installs the location of the configuration file:
            win32serviceutil.SetServiceCustomOption(
                HarmonyService._svc_name_,
                "LoggingConfiguration",
                val)
            fFoundConfigFile = 1
    if not fFoundConfigFile:
        print "Error: You forgot to pass in a path to your logging configuration file., use the '-c' option."
        raise ConfigFileNotFound

if __name__=='__main__':
#    win32serviceutil.HandleCommandLine(HarmonyService)
    win32serviceutil.HandleCommandLine(
        HarmonyService,
        customInstallOptions = "c:",
        customOptionHandler = customOptionHandler )
        
