# Remove all comment lines, starting lines with //.

# Remove all whitespace
# Need to watch out for spaces in strings
# replace 4 spaces by single space.
# repeated 5 times so it catches up to 20 spaces.
# For final ship may strip all spaces.
s/    //
s/    //
s/    //
s/    //
s/    //
#repeated to handle multiple tabs
s/\t//
s/\t//
s/\t//

# Watch out as // is part of  URL embedded in a string for o2m8.
# Look for // at start of line and delete to end of line
s\^//.*\\
# Look for // followed by a space
s\// .*\\
# Delete blank lines.
/^$/ d
/^\s*$/ d
# trailing white space
s/\s*$//
#
# Make sure no duplicate output strings.
#
s/NrDigInputs/a1/
s/NrDigOutputs/a2/
s/NrDigMonInputs/a3/
s/NrAnInputs/a4/
s/NrAnOutputs/a5/
s/NrTemps/a6/
s/NrDwells/a7/
s/NrSetPoints/a8/
s/NrRotaryEnc/a9/
# do not use b<n>
s/NrScheduleEntries/z0/
s/NrSceneEntries/z1/
s/SetPointStr/z2/
s/AnToImageBar/z3/
s/TempToTenths/z4/
s/TempToImageBar/z5/
s/UrlParam/z6/
s/AddTriggerFields/z7/
s/TrigHeaders/z8/
s/Postamble/z9/
s/Preamble/c1/
s/CfgDigitalOut/c2/
s/CfgDigitalIns/c3/
s/CfgAnalogueOuts/c4/
s/CfgAnalogueIns/c5/
s/CfgTemperatures/c6/
s/CfgDwellTable/c7/
s/CfgSetPointTable/c8/
s/ShowDigitalOut/c9/
s/ShowDigitalIns/d0/
s/ShowAnalogueOuts/d1/
s/ShowAnalogueIns/d2/
s/ShowTemperatures/d3/
s/ShowDwellTable/d4/
s/ShowSetPointTable/d5/
s/DaysTest/d6/
s/DaysStr/d7/
s/LoggedIn/d8/
s/MonShow/d9/
s/ToDStrs/e0/
s/ChannelTypeStrs/e1/
s/ActionStrs/e2/
s/UDPRemStrs/e3/
s/DayStrs/e4/
s/TrigActions/e5/
s/DisDwell/e6/
# can occur multiple times on 1 line.
s/NumPad/e7/
s/NumPad/e7/
s/NumPad/e7/
s/tbclk/e9/
s/trclk/f0/
s/tdclass/f1/
s/tdclk/f2/
s/td2clk/f3/
s/tdhclk/f4/
s/UrlThreshold/f5/
s/UrlChn/f6/
s/DOShow/f7/
s/MonShow/f8/
s/MonShow/f9/
s/LoggedIn/g0/
s/LoggedIn/g1/
s/AddTimeFields/g2/
s/AnHdr/g3/
s/clkStrAlt/g4/
s/clkStr/g5/
s/ = /=/
s/ == /==/
s/ShowLogin/x1/
s/DoSchedules/x2/
s/DoScenes/x3/
s/DoConfig/x4/
s/ShowAdvanced/x5/
s/CfgSrv/x6/
s/ShowManualCfg/x7/
s/DoNames/x8/
s/CfgAnalogueIn/x9/
s/CfgDigitalIn/x0/
s/CfgTemperature/y1/
s/CfgDwell/y2/
s/CfgSetPoints/y3/
s/CfgScheduleEntry/y4/
s/CfgSceneEntry/y5/
s/SimpleStatus/y6/
s/doConfigSc/y7/
s/doConfigTemp/y8/
s/doConfigAn/y9/
s/doConfigDi/w1/
s/writeSelectOptions/w2/
s/onActionChange/w3/
s/GetTriggerString/w4/
s/UpdateOutputChannels/w5/
s/onOutputTypeChange/w6/
s/changeTypes/w7/
s/UpdateSelectElement/w8/
