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
class wb6Status
{
    private $m_Xml;
    
    function load($wbaddr)
    {
        $this->m_Xml = DOMDocument::loadXML( getStatusXml($wbaddr) );
    }
    
    function getOneWireBus()
    {
        $temps = $this->m_Xml->getElementsByTagName( "OWBus" );
        $val = $temps->item( 0 );
        return $val->nodeValue;
    }
    
    function getTemp($idx)
    {
        $temps = $this->m_Xml->getElementsByTagName( "Tmp" );
        $temp = $temps->item( $idx );
        $CurTemp = ($temp->nodeValue / 16.0);
        if ($CurTemp > 127) $CurTemp = $CurTemp - 256 ;     
        return $CurTemp;
    }
    
    function getAnOut($idx)
    {
        $temps = $this->m_Xml->getElementsByTagName( "AO" );
        $val = $temps->item( $idx );
        return $val->nodeValue;
    }
    
    function getAnIn($idx)
    {
        $temps = $this->m_Xml->getElementsByTagName( "AI" );
        $val = $temps->item( $idx );
        return $val->nodeValue;
    }
    
    function getDigIn($idx)
    {
        $temps = $this->m_Xml->getElementsByTagName( "DI" );
        $val = $temps->item( 0 )->nodeValue;
        if ( ($val & pow(2,$idx)) != 0 ) return True;
        return False;
    }
    
    function getMonitor($idx)
    {
        return $this->getDigIn($idx+8);
    }
    
    function getDigOut($idx)
    {
        $temps = $this->m_Xml->getElementsByTagName( "DO" );
        $val = $temps->item( 0 )->nodeValue;
        if ( ($val & pow(2,$idx)) != 0 ) return True;
        return False;
    }
    
    function getCmdError()
    {
        $temps = $this->m_Xml->getElementsByTagName( "Error" );
        $val = $temps->item( 0 )->nodeValue;
        return $val;
    }
    
    function getOperationalState()
    {
        $temps = $this->m_Xml->getElementsByTagName( "Context" );
        $val = $temps->item( 0 )->nodeValue;
        return $val;
    }
    
    function getLoginState()
    {
        $temps = $this->m_Xml->getElementsByTagName( "LoginState" );
        $val = $temps->item( 0 )->nodeValue;
        return $val;
    }
    
    function getWbDate()
    {
        $temps = $this->m_Xml->getElementsByTagName( "Date" );
        $val = $temps->item( 0 )->nodeValue;
        return $val;
    }
    
    function getTime()
    {
        $temps = $this->m_Xml->getElementsByTagName( "Time" );
        $val = $temps->item( 0 )->nodeValue;
        return $val;
    }
    
    function getDay()
    {
        $temps = $this->m_Xml->getElementsByTagName( "Day" );
        $val = $temps->item( 0 )->nodeValue;
        return $val;
    }
}
?>