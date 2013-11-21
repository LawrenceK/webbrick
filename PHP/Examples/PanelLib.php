<?
/* ================================================================================================
*   
*   Open Source routines for supporting websites that use O2M8 technologies
*
*    STATUSLIB  -- these routines are used to create tabular sections of web pages
*        based on XML input.  The most common usage examples are:
*
*				populateStatus('standard.xml','panel'); 
*
*				This builds a control panel based on the file 'standard.xml'
*
*				populateStatus('10.100.100.100','config');
*
*				This builds a configuration table from the information xml
*				available at the WebBrick with IP address '10.100.100.100'
*
*    Version 5.1
*
*    Original File
*    Andy Harris 21th October 2003
*	 Updated
*    Andy Harris 28th October 2004 - deprecated routines removed
*
*    IF YOU ARE READING THIS --- please be away that we are working on a better version of the 
*     panel that uses mouseOver just like the WebBrick.  This will be released at a later date
*     the beauty of this version is that it doesn't need to know about the version of web browser
*     of if its rendering to a PDA -- Its likely that more functional future versions will need
*     this info ----- see http://www.o2m8.com for details
* =================================================================================================
*/

function processCmd()
{
/*
*  This is the main routine for parsing commands passed back  
*  through URL ?cmd=somecommand.
*
*  This has been changed for the WebBrick 4.x and 5.x to use generic commands
*
*
*
*  Note that $cmdstatus gets set with return status.
*  Therefore return status could be processed.
*/
global $cmdstatus;
global $http_cmd;
$cmdParts = explode(';',$http_cmd,5) ;

switch ($cmdParts[0])
	{
	case 'trig':
        /*
         *  Issue a direct trigger to a WebBrick
         */
        wb6Trigger($cmdParts[1],$cmdParts[2]) ;
        break;	

	case 'analogue':
        /*
         *  Issue a direct analogue set point to a WebBrick
         */
        wb6SetPoint($cmdParts[1],$cmdParts[2],$cmdParts[3]) ;
        break;

	case 'refresh':
        break;	
	
	}

/*
*  The sleep command command here is to allow for any command to execute so that 
*  when status is collected from various WebBricks it will reflect the command
*  issued
*/
if ($http_cmd != 'refresh') sleep (1) ;

$http_cmd='';
}


/*   ----------------------------------------------------------------
*     CALL the following function
*	 ----------------------------------------------------------------
*/

function populatePanel($xmlfile)
{
/*
*  Read through status configuration and output data 
*/

global $wbStati;
$wbStati = Array();
startStatus() ;
startRow() ;
$panelXml = loadPanel($xmlfile);
createPanel( $panelXml );
endRow() ;
endStatus() ;
}

function loadPanel($file)
{
    return DOMDocument::load( $file );
}

function createPanel( $panelXml )
{
    global $numCols;
    global $itNo;
    $items = $panelXml->getElementsByTagName( "status" );
    if ( $items->length > 0 ) :
        $numCols = $items->item(0)->getAttribute('cols');
    else:
        $numCols = 4;
    endif;
    
    $itNo = 0;
    
    $items = $panelXml->getElementsByTagName( "item" );
    foreach ( $items as $item )
    {
        if (fmod($itNo,$numCols) == 0)
        {
            endRow() ;
            startRow() ;
        }
        $itNo++;
        populateItem($item);
    }
    if ( $itNo > 0 )
    {
        endRow() ;
    }
}

function populateItem($item)
{
global $wbStati;
global $numCols;
global $itNo;
$wbStatus;
$elements;
$type;
$ipAdr;
$title;
$chan;
$trig;

$elements = $item->getElementsByTagName( "IP" );
if ( $elements->length > 0 ) :
    $ipAdr = $elements->item(0)->nodeValue;
    if ( array_key_exists($ipAdr, $wbStati) ):
        $wbStatus = $wbStati[$ipAdr];
    else:
        $wbStatus = new wb6Status();
        $wbStatus->load($ipAdr );
        $wbStati[$ipAdr] = $wbStatus;
    endif;
endif;

$elements = $item->getElementsByTagName( "Title" );
if ( $elements->length > 0 ) :
    $title = $elements->item(0)->nodeValue;
endif;

$elements = $item->getElementsByTagName( "Chan" );
if ( $elements->length > 0 ) :
    $chan = $elements->item(0)->nodeValue;
endif;

$elements = $item->getElementsByTagName( "Trig" );
if ( $elements->length > 0 ) :
    $trig = $elements->item(0)->nodeValue;
endif;

$type = $item->getElementsByTagName( "Type" )->item(0)->nodeValue;

//echo $type . " " .$ipAdr . " " .$title . " " .$chan . " " .$trig;

switch ($type)
	{
		case "Header" :

            $elements = $item->getElementsByTagName( "Cols" );
            if ( $elements->length > 0 ) :
                $cols = $elements->item(0)->nodeValue;
            else:
                $cols = 1;
            endif;
            
			$hdrStr = "<td class='midTitle' colspan='" . $cols . "'>" ;
			echo $hdrStr;
			echo $title;
			endCol();
            
			break ;

		case "NewRow" :
            if ( $itNo > 0 )
            {
                endRow();
                startRow() ;
                $itNo = 0 ;
            }
			break ;

		case "Temp" :
			startCol();
			echo $title;
			echo " " ;
			echo $wbStatus->getTemp($chan);
			echo "&ordm;C" ;
			endCol();
			break ;
			
		case "inAnalogue" :
        
            td('sts', $title ." " .$wbStatus->getAnIn($chan) ."%" ) ;
			endCol() ;
			break ;

		case "Analogue" :
			$anVal = $wbStatus->getAnOut($chan);
			if ($anVal == 0)
				{
				startSwCol(0, $trig) ;
				}
			else
				{
				startSwCol(1, $trig) ;
				}
			echo $title ." " . $anVal . "%</a>" ;
			endCol() ;
			break ;
            
		case "Monitor" :
			startCol();
			$alarmValue = $wbStatus->getMonitor($chan) ;
            if ( $alarmValue ):
                echo $title;
            else:
                echo $item->getElementsByTagName( "TitleOff" )->item(0)->nodeValue;
            endif;
			endCol();
			break ;

		case "Switch" :
			/*
			*   Read through to DigOutStatus
			*/
			$opState = $wbStatus->getDigOut($chan) ;
			startSwCol($opState, $trig);
			echo $title . "</a>" ;
			endCol();
			break ;
			
			
		case "Refresh" :
			//startCol() ;
			echo "<td class=genBut> <a href=\"?cmd=" . $trig ."\">" .$title ." </a>" ;
			endCol() ;
			break ;	
			
			
	}
}

function startStatus()
{
echo "<table class=wide>" ;
}

function endStatus()
{
echo "</table>" ;
}

function startRow()
{
echo "<tr>" ;
}

function endRow()
{
echo "</tr>" ;
}

function startCol()
{
echo "<td class=sts>" ;
}

function startSwCol($val, $action)
{
$onclick = '';
if ( $action ) :
    $onclick = 'onClick="document.location=\'\\standard.php?cmd='.$action.'\'"';
    if ($val == 1)
        {
        echo "<td class=isON onMouseOver=\"this.className='isONover';\" onMouseOut=\"this.className='isON';\"" .$onclick .">" ;
    //	echo "<td class=isON " .$onclick .">" ;
        }
    else
        {
        echo "<td class=isOFF onMouseOver=\"this.className='isOFFover';\" onMouseOut=\"this.className='isOFF';\"" .$onclick .">" ;
    //	echo "<td class=isOFF " .$onclick .">" ;
        }
else:
    if ($val == 1)
        {
    	echo "<td class=isON>" ;
        }
    else
        {
    	echo "<td class=isOFF>" ;
        }
endif;
}

function endCol()
{
echo "</td>\n";
}

function td ($style,$content)
{
echo "<td class='" . $style . "'>" . $content . "</td>" ;
}

?>