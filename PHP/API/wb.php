<?
/* ================================================================================================
*   
*   Open Source routines for supporting websites that use O2M8 technologies
*
*   This include file contains routines that are generic across all versions of WebBricks.
*
*    Version 6.0
*
*    Original File
*    Andy Harris 21th October 2003
*	 Updated
*    Andy Harris 28th October 2004 - deprecated routines removed
*    Andy Harris 1st February 2005 - added Analogue set point routines
*    Lawrence Klyne 10th Decemeber 2005 - WebBrick 6.0
*
* =================================================================================================
*/

function getURLContents($url)
{
    /*
            *   Get the URL using file, and just display results  
            */
    $ret = "" ;
    $lines = file($url, 'r');
    foreach ($lines as $line_num => $line) 
    {
        $ret .= $line ;
    }
    return $ret ;
}

function getWbUrl($wbaddr,$url)     
{
    $header = "GET /" .$url . " HTTP/1.0\r\nHost: me.com \r\n\r\n"; 
    $fp = @fsockopen($wbaddr, 80, &$err_num, &$err_msg, 10); 
    if ($fp) 
    { 
        fputs($fp, $header); 
        $response = ""; 
        /*
                            Look for HTTP headetr and strip it off include remove blank line after header
                    */
        $response .= fgets($fp, 4096); 
        if (stristr( $response, "http" ) != FALSE)
        {
            $response .= fgets($fp, 4096); 
            $response = ""; 
        }
        while (!feof($fp))
		{ 
            $response .= fgets($fp, 4096); 
        } 
        return $response;
    } 
	else
	{
        return "<error>Web Error " . $err_num .$err_msg ."</error>";
	}
}

function wbSendUdpFrame($srv,$cmd)
{
    $result = False;
    $srv = 'udp://' . $srv ;
    $udphandle = fsockopen($srv, 26482, &$errno, &$errstr, 2) ;
    global $cmdstatus;
    if (!$udphandle) 
        {
        $cmdstatus .= " Failed with " .$errno ."  " .$errstr ;
        }
    
    socket_set_timeout ($udphandle, 5);
    // Generate udp frame.
    $len = strlen($cmd) + 7; // need to include Framing
    $operand = chr($len) .chr(255-$len) ."\x19\xFF" .$cmd ."\x3A\x00\x00" ;
    $write = fwrite($udphandle,$operand);
    if (!$write) 
        {
        $cmdstatus .= " UDP write failed" ;
        } 
    else
        {
        $cmdstatus .= " Success" ;
        $result = True;
        }
    fclose ($udphandle) ;
    return $result;
}

function wbSendCmd($srv,$cmd)
{
    // Place holder so can change to HTTP instead of UDP.
    wbSendUdpFrame( $srv, $cmd );
}

?>