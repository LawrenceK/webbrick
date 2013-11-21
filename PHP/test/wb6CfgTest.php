<html>
<head>
<title>Standard WebBrick Control</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="thirtover.css">
<link rel="SHORTCUT ICON" href="/favicon.ico">
</head>
<body bgcolor="#00CC66">
<?php
include("../API/wb.php") ;
include("../API/wb6.php") ;
include("../API/wb6Cfg.php") ;

$test_ip = "10.100.100.100";

echo "New<BR>";
$wb6Cfg = new wb6Config();
echo "Load<BR>";
$wb6Cfg->load( $test_ip );
echo "Display<BR>";

echo "NodeNumber = " .$wb6Cfg->getNodeNumber() ."<BR>";
echo "NodeName = " .$wb6Cfg->getNodeName() ."<BR>";
echo "FadeRate = " .$wb6Cfg->getFadeRate() ."<BR>";
echo "IpAddress = " .$wb6Cfg->getIpAddress() ."<BR>";
echo "MacAddress = " .$wb6Cfg->getMacAddress() ."<BR>";
echo "Rotary 0 = " .$wb6Cfg->getRotary(0) ."<BR>";
echo "<BR>";

echo "Dwell 0 = " .$wb6Cfg->getDwell(0) ."<BR>";
echo "Dwell 1 = " .$wb6Cfg->getDwell(1) ."<BR>";
echo "Dwell 2 = " .$wb6Cfg->getDwell(2) ."<BR>";
echo "Dwell 3 = " .$wb6Cfg->getDwell(3) ."<BR>";
echo "<BR>";

echo "SetPoint 0 = " .$wb6Cfg->getSetPoint(0) ."<BR>";
echo "SetPoint 1 = " .$wb6Cfg->getSetPoint(1) ."<BR>";
echo "SetPoint 2 = " .$wb6Cfg->getSetPoint(2) ."<BR>";
echo "SetPoint 3 = " .$wb6Cfg->getSetPoint(3) ."<BR>";
echo "SetPoint 4 = " .$wb6Cfg->getSetPoint(4) ."<BR>";
echo "SetPoint 5 = " .$wb6Cfg->getSetPoint(5) ."<BR>";
echo "SetPoint 6 = " .$wb6Cfg->getSetPoint(6) ."<BR>";
echo "SetPoint 7 = " .$wb6Cfg->getSetPoint(7) ."<BR>";
echo "<BR>";

echo "Digital Out 0 = " .$wb6Cfg->getDigOutName(0) ."<BR>";
echo "Digital Out 1 = " .$wb6Cfg->getDigOutName(1) ."<BR>";
echo "Analogue Out 0 = " .$wb6Cfg->getAnalogueOutName(0) ."<BR>";
echo "Analogue Out 1 = " .$wb6Cfg->getAnalogueOutName(1) ."<BR>";
echo "<BR>";

echo "Digital In 0 = " .print_r($wb6Cfg->getDigInTrigger(0), True ) ."<BR>";
echo "Digital In 1 = " .print_r($wb6Cfg->getDigInTrigger(1), True ) ."<BR>";
echo "<BR>";

echo "Scene 0 = " .print_r($wb6Cfg->getScene(0), True ) ."<BR>";
echo "Scene 1 = " .print_r($wb6Cfg->getScene(1), True ) ."<BR>";
echo "<BR>";

echo "Temp 0 Low = " .print_r($wb6Cfg->getTempTriggerLow(0), True ) ."<BR>";
echo "Temp 0 High= " .print_r($wb6Cfg->getTempTriggerHigh(0), True ) ."<BR>";
echo "<BR>";

echo "Analogue 0 Low = " .print_r($wb6Cfg->getAnalogueTriggerLow(0), True ) ."<BR>";
echo "Analogue 0 High= " .print_r($wb6Cfg->getAnalogueTriggerHigh(0), True ) ."<BR>";
echo "<BR>";

echo "Scheduled Event 0 = " .print_r($wb6Cfg->getScheduledEvent(0), True ) ."<BR>";
echo "Scheduled Event 1 = " .print_r($wb6Cfg->getScheduledEvent(1), True ) ."<BR>";
echo "<BR>";

?>

</body>
</html>
 