<html>
<head>
<title>Standard WebBrick Control</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<link rel="stylesheet" href="standard.css">
<link rel="SHORTCUT ICON" href="/favicon.ico">
</head>
<? 
include("../API/wb.php") ;
include("../API/wb6.php") ;
include("../API/wb6status.php") ;
include("../API/wb6Cfg.php") ;
include("PanelLib.php") ;

$http_cmd = "" ;
import_request_variables("gp", "http_");

function showUser()
{
echo getenv("REMOTE_USER") ;
}

?> 

<body bgcolor="#00CC66">
<table class=wide>
  <tr> 
    <td colspan= 2 class=line>&nbsp;</td>
  </tr>
  <tr> 
    <td class=TitleRight>As Shipped WebBrick Control</td>
    <td width=5%> </td>
  </tr>
  <tr> 
    <td colspan=2 class=line>&nbsp;</td>
  </tr>
  <tr> 
    
  </tr>
</table>
<table class= wide>
  <tr> 
    <td class=left> </td>
  </tr>
</table>

<? processCmd(); ?>
<? populatePanel('standard.xml'); ?>
<br>

</body>
</html>
