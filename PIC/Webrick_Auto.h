//
// Helper to ease transition and enable support of both chips.
//
#ifdef __18F4525
#include "Webrick5_Auto.h"

#endif

#ifdef __18F6722
#include "Webrick6_Auto.h"

#define USE_RTC 1
#define USE_MIMICS 1
#define USE_SERIAL 1
#pragma wizcpp uselib "$(FEDPATH)\libs\I2cLib.c"
#endif

#ifdef __18F6622
#include "Webrick6_rev3_Auto.h"

#define USE_RTC 1
#define USE_MIMICS 1
#define USE_SERIAL 1
#pragma wizcpp uselib "$(FEDPATH)\libs\I2cLib.c"
#endif

#pragma wizcpp uselib "$(FEDPATH)\libs\mem.c"

#include <pic.h>
#include <delays.h>
#include <I2C.h>
#include <SerInt2.h>
#include <Strings.h>
#include <mem.h>
#include <Maths.h>

#include "version.h"
#include "Errors.h"
// spLocations before defaults
#include "spLocations.h"
#include "defaults.h"
#include "debug.h"
#include "spComms.h"
#include "persist.h"
#include "Globals.h"
#include "Dallas1Wire.h"
#include "Temperature.h"
#include "Analogue.h"
#include "Digital.h"
#include "Clock.h"
#include "WebBrick.h"
#include "SitePlayer.h"
#include "Dmx.h"
#include "Parse.h"
#include "InfraRed.h"
#include "Mimic.h"
#include "Serial2.h"
#include "Dmx.h"
