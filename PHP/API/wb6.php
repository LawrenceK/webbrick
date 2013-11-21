<?
/* ================================================================================================
*   
*   Open Source routines for supporting websites that use O2M8 technologies
*
*   This include file contains routines that are specific to WebBrick Version 6.
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

function wb6Trigger($srv,$cct)
{
    wbSendCmd( $srv, "DI" .$cct );
}

function wb6DigOn($srv,$cct)
{
    wbSendCmd( $srv, "DO" .$cct .";N");
}

function wb6DigOff($srv,$cct)
{
    wbSendCmd( $srv, "DO" .$cct .";F");
}

function wb6DigToggle($srv,$cct)
{
    wbSendCmd( $srv, "DO" .$cct .";T");
}

function wb6DigDwell($srv,$cct,$dw)
{
    wbSendCmd( $srv, "DO" .$cct .";D;" .$dw);
}

function wb6SetPoint($srv,$cct,$sp)
{
    wbSendCmd( $srv, "AA" .$cct .";S" .$sp );
}

function wb6AnOutValue($srv,$cct,$val)
{
    wbSendCmd( $srv, "AA" .$cct .";" .$val );
}

# day 0-6, hour 0-23, minute 0-59    
function wb6Login($srv,$password)
{
    wbSendCmd( $srv, "LG;" .$password );
}

# day 0-6, hour 0-23, minute 0-59    
function wb6SetTime($srv,$day, $hr, $min)
{
    wbSendCmd( $srv, "ST;" .$day .";" . $hr .";" . $min );
}

function getStatusXml($wbaddr)
{
    return getWbUrl( $wbaddr,'wbstatus.xml' );
}

function getConfigXml($wbaddr)
{
    return getWbUrl( $wbaddr,'wbcfg.xml' );
}
?>