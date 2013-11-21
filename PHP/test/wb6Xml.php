<html>
<head>
<title>Standard WebBrick Control</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<body bgcolor="#00CC66">
<?php
include("../API/wb.php") ;
include("../API/wb6.php") ;

$test_ip = "10.100.100.100";

echo "------------ <BR>Status<BR>";
$xml = getStatusXml( $test_ip );
$xml = str_replace( "<", "&lt;", $xml );
$xml = str_replace( ">", "&gt;", $xml );
echo $xml ."<BR>";
echo "------------ <BR>Config<BR>";
$xml = getConfigXml( $test_ip );
$xml = str_replace( "<", "&lt;", $xml );
$xml = str_replace( ">", "&gt;", $xml );
echo $xml ."<BR>";
?>

</body>
</html>
 