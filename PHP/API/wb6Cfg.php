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

/*
*  Global Strings -- These match up with WebBrick definitions, and therefore should not
*  be changed unless the WebBrick definitions in PCAINC.DEF or library.js are changed
*/
$ToDStrs = array ('Commands Disabled', 'Startup', 'Normal Operation', 'Quiescent Operation') ;
$AStrs = array ('Off', 'Enabled') ;
$OneWStrs = array ('No Sensors Found', 'Bus Good, Sensor Found', 'Bus Good, Reading Good', 'Bus Good, Software Error', 'Bus Bad, Held Low') ;
$ChannelTypeStrs = array ('Digital', 'Scene', 'Analogue' ) ;
$ActionStrs =  array ( 'None', 'Off', 'On', 'Momentary', 'Toggle', 'Dwell', 'Dwell-Can', 'Next', 'Prev' );
$UDPRemStrs = array ('None', 'General', 'Remote', 'Alarm') ;

class wb6Config
{
    private $m_Xml;
    
    function load($wbaddr)
    {
        $this->m_Xml = DOMDocument::loadXML( getConfigXml($wbaddr) );
    }
    
    function getNodeName()
    {
        $vals = $this->m_Xml->getElementsByTagName( 'NN' );
        $val = $vals->item( 0 );
        return $val->nodeValue;
    }
    
    function getNodeNumber()
    {
        $vals = $this->m_Xml->getElementsByTagName( 'SN' );
        $val = $vals->item( 0 );
        return $val->nodeValue;
    }
    
    function getFadeRate()
    {
        $vals = $this->m_Xml->getElementsByTagName( 'SF' );
        $val = $vals->item( 0 );
        return $val->nodeValue;
    }
    
    function getIpAddress()
    {
        $vals = $this->m_Xml->getElementsByTagName( 'SI' );
        $val = $vals->item( 0 );
        return $val->getAttribute( 'ip' );
    }
    
    function getMacAddress()
    {
        $vals = $this->m_Xml->getElementsByTagName( 'SI' );
        $val = $vals->item( 0 );
        return $val->getAttribute( 'mac' );
    }
    
    function getRotary($idx)
    {
        $vals = $this->m_Xml->getElementsByTagName( 'SR' );
        $val = $vals->item( $idx );
        return $val->getAttribute( 'Value' );
    }
    
    private function decodeTrigger($trgEl)
    {
        global $ActionStrs, $UDPRemStrs, $ChannelTypeStrs;
        
        $ret = array();		// this is the return array
        
        $cfgB1 = $trgEl->getAttribute( 'B1' );
        $cfgB2 = $trgEl->getAttribute( 'B2' );
        $cfgB3 = $trgEl->getAttribute( 'B3' );
        
        $ret['actionNr'] = ($cfgB1 & 0x0F); 
        $ret['action'] = $ActionStrs[$ret['actionNr']]; 
        $ret['dwell'] = (($cfgB1 & 0x30) /16); 
        $ret['UDPRemNr'] = (($cfgB1 & 0xC0) /64); 
        $ret['UDPRem'] = $UDPRemStrs[$ret['UDPRemNr']]; 
        $ret['RemNode'] = $cfgB3; 
        
        if (($cfgB2 & 0x80) != 0):
            $ret['typeNr'] = 2; 
            $ret['setPoint'] = ($cfgB2 & 0x0F);
            $ret['pairChn'] = (($cfgB2 & 0x70) /16); 
        else:
            if (($cfgB2 & 0x40) != 0):
                $ret['typeNr'] = 1; 
            else:
                $ret['typeNr'] = 0; 
            endif;
            $ret['setPoint'] = 0;
            $ret['pairChn'] = ($cfgB2 & 0x0F); 
        endif;
        $ret['type'] = $ChannelTypeStrs[$ret['typeNr']];
        
        return $ret ;
    }
    
    function getDwell($idx)
    {
        $vals = $this->m_Xml->getElementsByTagName( 'CW' );
        $dw = $vals->item( $idx )->nodeValue;
        return $dw;
    }
    
    function getDwellStr($idx)
    {
        $dw = getDwell($idx);
        if ($dw<60)
            {
            return ($dw . ' Secs') ;
            }
        if ($dw>=60 && $dw<=3600)
            {
            return (round($dw/60) . ' Mins') ;
            }
        if ($dw>3600)
            {
            return (round($dw/360)/10 . ' Hours') ;
            }
        return 'Out Of Range' ;
    }
    
    function getSetPoint($idx)
    {
        $vals = $this->m_Xml->getElementsByTagName( 'CS' );
        $sp = $vals->item( $idx )->nodeValue;
        return $sp .'%';
    }
    
    private function getName($type, $idx)
    {
        $vals = $this->m_Xml->getElementsByTagName( $type );
        $val = $vals->item( $idx );
        return $val->getAttribute('Name');
    }
    
    function getDigOutName($idx)
    {
        return $this->getName('NO', $idx);
    }
    
    function getMonitorName($idx)
    {
        return $this->getName('NM', $idx);
    }
    
    function getAnalogueOutName($idx)
    {
        return $this->getName('NA', $idx);
    }
    
    function getDigInTrigger($idx)
    {
        $vals = $this->m_Xml->getElementsByTagName( 'CD' );
        $cd = $vals->item( $idx );
        $trg = $cd->getElementsByTagName( 'Trg' )->item( 0 );
        $result = $this->decodeTrigger($trg);		// this is the return array
        $result['Name'] = $cd->getAttribute( 'Name' );
        return $result;
    }
    
    function getTempTriggerLow($idx)
    {
        $vals = $this->m_Xml->getElementsByTagName( 'CT' );
        $temp = $vals->item( $idx );
        $trg = $temp->getElementsByTagName( 'TrgL' )->item( 0 );
        $result = $this->decodeTrigger($trg);
        $result['Name'] = $temp->getAttribute( 'Name' );
        $result['Value'] = $trg->getAttribute( 'Val' );
        return $result;
    }
    
    function getTempTriggerHigh($idx)
    {
        $vals = $this->m_Xml->getElementsByTagName( 'CT' );
        $temp = $vals->item( $idx );
        $trg = $temp->getElementsByTagName( 'TrgH' )->item( 0 );
        $result = $this->decodeTrigger($trg);
        $result['Name'] = $temp->getAttribute( 'Name' );
        $result['Value'] = $trg->getAttribute( 'Val' );
        return $result;
    }
    
    function getAnalogueTriggerLow($idx)
    {
        $vals = $this->m_Xml->getElementsByTagName( 'CI' );
        $temp = $vals->item( $idx );
        $trg = $temp->getElementsByTagName( 'TrgL' )->item( 0 );
        $result = $this->decodeTrigger($trg);
        $result['Name'] = $temp->getAttribute( 'Name' );
        $result['Value'] = $trg->getAttribute( 'Val' );
        return $result;
    }
    
    function getAnalogueTriggerHigh($idx)
    {
        $vals = $this->m_Xml->getElementsByTagName( 'CI' );
        $temp = $vals->item( $idx );
        $trg = $temp->getElementsByTagName( 'TrgH' )->item( 0 );
        $result = $this->decodeTrigger($trg);
        $result['Name'] = $temp->getAttribute( 'Name' );
        $result['Value'] = $trg->getAttribute( 'Val' );
        return $result;
    }
    
    function getScheduledEvent($idx)
    {
        $vals = $this->m_Xml->getElementsByTagName( 'CE' );
        $val = $vals->item( $idx );
        $trg = $val->getElementsByTagName( 'Trg' )->item( 0 );
        $result = $this->decodeTrigger($trg);
        $result['Days'] = $val->getAttribute( 'Days' );
        $result['Hours'] = $val->getAttribute( 'Hours' );
        $result['Minutes'] = $val->getAttribute( 'Mins' );
        return $result;
    }
    
    function getScene($idx)
    {
        $ret = array();		// this is the return array
        $vals = $this->m_Xml->getElementsByTagName( 'CC' );
        $scene = $vals->item( $idx );
        $dm = $scene->getAttribute( 'Dm' );
        $ds = $scene->getAttribute( 'Ds' );
        $am = $scene->getAttribute( 'Am' );
        $av = $scene->getAttribute( 'Av' );
        for ($i = 0; $i < 8; $i++) 
        {
            if ( ( $dm & pow(2,$i) ) != 0 ):
                if ( ( $ds & pow(2,$i) ) != 0 ):
                    $ret['Digital'.$i] = 'On';
                else:
                    $ret['Digital'.$i] = 'Off';
                endif;
            else:
                $ret['Digital'.$i] = 'Ignore';
            endif;
        }
        for ($i = 0; $i < 4; $i++) 
        {
            if ( ( $am & pow(2,$i) ) != 0 ):
                $ret['Analogue'.$i] = 'SetPoint'.($av & 0x0F);
            else:
                $ret['Analogue'.$i] = 'Ignore';
            endif;
            $Av = $Av / 16;
        }
        return $ret;
    }
    
}

?>