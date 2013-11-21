<html>
<head>
<title>Standard WebBrick Control</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<body bgcolor="#00CC66">
<?php
include("../API/wb.php") ;
include("../API/wb6.php") ;
include("../API/wb6status.php") ;

$test_ip = "10.100.100.100";

echo "New<BR>";
$wb6Sts = new wb6Status();
echo "Load<BR>";
$wb6Sts->load( $test_ip );
echo "Display<BR>";

echo "OWBus = " .$wb6Sts->getOneWireBus() ."<BR>";

echo "Temp 0 = " .$wb6Sts->getTemp( 0) ."<BR>";
echo "Temp 1 = " .$wb6Sts->getTemp( 1) ."<BR>";
echo "Temp 2 = " .$wb6Sts->getTemp( 2) ."<BR>";
echo "Temp 3 = " .$wb6Sts->getTemp( 3) ."<BR>";
echo "Temp 4 = " .$wb6Sts->getTemp( 4) ."<BR>";

echo "An Out 0 = " .$wb6Sts->getAnOut( 0) ."<BR>";
echo "An Out 1 = " .$wb6Sts->getAnOut( 1) ."<BR>";
echo "An Out 2 = " .$wb6Sts->getAnOut( 2) ."<BR>";
echo "An Out 3 = " .$wb6Sts->getAnOut( 3) ."<BR>";

echo "An In 0 = " .$wb6Sts->getAnIn( 0) ."<BR>";
echo "An In 1 = " .$wb6Sts->getAnIn( 1) ."<BR>";
echo "An In 2 = " .$wb6Sts->getAnIn( 2) ."<BR>";
echo "An In 3 = " .$wb6Sts->getAnIn( 3) ."<BR>";

echo "Dig In 0 = " .$wb6Sts->getDigIn( 0) ."<BR>";
echo "Dig In 1 = " .$wb6Sts->getDigIn( 1) ."<BR>";
echo "Dig In 2 = " .$wb6Sts->getDigIn( 2) ."<BR>";
echo "Dig In 3 = " .$wb6Sts->getDigIn( 3) ."<BR>";
echo "Dig In 4 = " .$wb6Sts->getDigIn( 4) ."<BR>";
echo "Dig In 5 = " .$wb6Sts->getDigIn( 5) ."<BR>";
echo "Dig In 6 = " .$wb6Sts->getDigIn( 6) ."<BR>";
echo "Dig In 7 = " .$wb6Sts->getDigIn( 7) ."<BR>";

echo "Monitor 0 = " .$wb6Sts->getMonitor( 0) ."<BR>";
echo "Monitor 1 = " .$wb6Sts->getMonitor( 1) ."<BR>";
echo "Monitor 2 = " .$wb6Sts->getMonitor( 2) ."<BR>";
echo "Monitor 3 = " .$wb6Sts->getMonitor( 3) ."<BR>";

echo "Dig Out 0 = " .$wb6Sts->getDigOut( 0) ."<BR>";
echo "Dig Out 1 = " .$wb6Sts->getDigOut( 1) ."<BR>";
echo "Dig Out 2 = " .$wb6Sts->getDigOut( 2) ."<BR>";
echo "Dig Out 3 = " .$wb6Sts->getDigOut( 3) ."<BR>";
echo "Dig Out 4 = " .$wb6Sts->getDigOut( 4) ."<BR>";
echo "Dig Out 5 = " .$wb6Sts->getDigOut( 5) ."<BR>";
echo "Dig Out 6 = " .$wb6Sts->getDigOut( 6) ."<BR>";
echo "Dig Out 7 = " .$wb6Sts->getDigOut( 7) ."<BR>";

echo "Last cmd error = " .$wb6Sts->getCmdError() ."<BR>";
echo "Operational State = " .$wb6Sts->getOperationalState() ."<BR>";
echo "LoginState = " .$wb6Sts->getLoginState() ."<BR>";
echo "Date = " .$wb6Sts->getWbDate() ."<BR>";
echo "Time = " .$wb6Sts->getTime() ."<BR>";
echo "Day = " .$wb6Sts->getDay() ."<BR>";
?>

</body>
</html>
 