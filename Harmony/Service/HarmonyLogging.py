import logging, logging.handlers

def configure():
    # replace with a configuration file.
    _logger = logging.getLogger('')
    handler = logging.handlers.RotatingFileHandler( ".\\HarmonyWebBrick.log", "a", 1000000, 10)
    formatter = logging.Formatter("%(asctime)s %(levelname)s\t%(message)s")
    handler.setFormatter(formatter)
    _logger.addHandler(handler)
    _logger.setLevel(logging.DEBUG)

    handler = logging.handlers.NTEventLogHandler( "HarmonyWebBrick" )
    formatter = logging.Formatter("%(asctime)s %(levelname)s\t%(message)s")
    handler.setFormatter(formatter)
    handler.setLevel(logging.ERROR)
    _logger.addHandler(handler)

#   fname = win32serviceutil.GetServiceCustomOption(self, "LoggingConfiguration")
#HarmonyLogging.__FILE__
# get path part
