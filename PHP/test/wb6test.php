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

$test_ip = "10.100.100.100";

echo "On" .wb6DigOn($test_ip,0) ."<BR>";
echo "On" .wb6DigOn($test_ip,6) ."<BR>";
echo "Off" .wb6DigOff($test_ip,1) ."<BR>";
echo "Toggle" .wb6DigToggle($test_ip,2) ."<BR>";

echo "Trigger" .wb6Trigger($test_ip,4) ."<BR>";
echo "Toggle" .wb6DigToggle($test_ip,5) ."<BR>";
echo "Toggle" .wb6DigToggle($test_ip,6) ."<BR>";
echo "Dwell" .wb6DigDwell($test_ip,7, 2) ."<BR>";

echo "Setpoint 1 " .wb6SetPoint($test_ip,0,7) ."<BR>";
echo "Setpoint 2 " .wb6SetPoint($test_ip,1,5) ."<BR>";
echo "Setpoint 3 " .wb6SetPoint($test_ip,2,3) ."<BR>";
echo "Setpoint 4 " .wb6AnOutValue($test_ip,3,50) ."<BR>";

echo "Login";
wb6Login($test_ip,"password");
echo "SetTime";
wb6SetTime($test_ip, 5, 6, 7);
?>

</body>
</html>
 