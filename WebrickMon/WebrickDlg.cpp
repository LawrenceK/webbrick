// WebrickDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Webrick.h"
#include "WebrickDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_BUFFERED_DATA   65535

/////////////////////////////////////////////////////////////////////////////
// CWebrickDlg dialog

CWebrickDlg::CWebrickDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebrickDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWebrickDlg)
	m_Report = _T("");
	m_IpAddressStr = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
        m_Shutdown = false;
        m_Thread = NULL;
        ::memset(&m_BrdAdr, 0, sizeof(m_BrdAdr) );
        m_BrdAdr.sin_family = AF_INET;
        m_BrdAdr.sin_addr.S_un.S_addr = 0xFFFFFFFF;
        m_BrdAdr.sin_port = htons(26482); // webbrick port
        m_NewAdr.sin_addr.S_un.S_addr = INADDR_NONE;
        m_LogFile = NULL;
}

void CWebrickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebrickDlg)
	DDX_Text(pDX, IDC_EDIT1, m_Report);
	DDX_Text(pDX, IDC_EDIT2, m_IpAddressStr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWebrickDlg, CDialog)
	//{{AFX_MSG_MAP(CWebrickDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnSetAddress)
	ON_BN_CLICKED(IDC_BUTTON2, OnOpenLog)
	ON_BN_CLICKED(IDC_BUTTON3, OnCloseLog)
	//}}AFX_MSG_MAP
        ON_MESSAGE(WM_USER+1, OnWmUser)
END_MESSAGE_MAP()

static socketError( const char* msg )
{
    char txt2[256];
    int err = WSAGetLastError();
    sprintf( txt2, "%i", err );
    ::MessageBox( NULL, txt2, msg, MB_OK );
}

void CWebrickDlg::SendPacket( const char* msg )
{
    // turn into UDP frame
    char buf[256];// working buffer
    char len = strlen(msg);

    buf[0] = len;
    buf[1] = 255-len;
    buf[2] = 0x19;
    buf[3] = (char)0xFF;

    ::memcpy( buf+4, msg, len );

    len += 4;
    buf[len++] = 0;
    buf[len++] = 0;
    int sendLen;
    sendLen = sendto( m_UdpSocket, buf, len, 0, (SOCKADDR*)&m_BrdAdr, sizeof(m_BrdAdr) );
    if ( sendLen == SOCKET_ERROR )
    {
        socketError( "Socket sendto" );
    }
}

void CWebrickDlg::DoLogin()
{
    static const char* lg = ":LG;password:";
    SendPacket( lg );
}

static char work1[32];
static const char * toStr( unsigned char val )
{
    return itoa( val, work1, 10 );
}

char work2[256];// working buffer
void CWebrickDlg::SetIp( const unsigned char *mac )
{
    if ( m_NewAdr.sin_addr.S_un.S_addr != INADDR_NONE )
    {
        // send login
        DoLogin();
        char idx;
        strcpy( work2, ":SA" );
        for ( idx = 0; idx < 6; ++idx )
        {
            strcat( work2, ";" );
            strcat( work2, toStr(mac[idx]) );
        }
        strcat( work2, ";" );
        strcat( work2, toStr(m_NewAdr.sin_addr.S_un.S_un_b.s_b1) );
        strcat( work2, ";" );
        strcat( work2, toStr(m_NewAdr.sin_addr.S_un.S_un_b.s_b2) );
        strcat( work2, ";" );
        strcat( work2, toStr(m_NewAdr.sin_addr.S_un.S_un_b.s_b3) );
        strcat( work2, ";" );
        strcat( work2, toStr(m_NewAdr.sin_addr.S_un.S_un_b.s_b4) );
        strcat( work2, ":" );

        // send new ip address
        SendPacket( work2 );
        // done it.
        m_NewAdr.sin_addr.S_un.S_addr = INADDR_NONE;
        PostMessage( WM_USER+1, 0, (LPARAM)work2 );
    }
}

// Background Thread.
static char preamble[64];
static char eventStr[256];   //passed in windows message

void makePreamble( tag_UDP_Packet  *UdpPacket, sockaddr_in &fromAdr )
{
    char timeStr[32];
    sprintf( preamble, "%s %u.%u.%u.%u - %u%C:%C%C", 
            _strtime(timeStr),
            fromAdr.sin_addr.S_un.S_un_b.s_b1, 
            fromAdr.sin_addr.S_un.S_un_b.s_b2, 
            fromAdr.sin_addr.S_un.S_un_b.s_b3, 
            fromAdr.sin_addr.S_un.S_un_b.s_b4,
            UdpPacket->seqNr,
            UdpPacket->udpType,
            UdpPacket->source[0],
            UdpPacket->source[1] );
}

static unsigned char lastSeq = 0;

void CWebrickDlg::formatBinaryEvent( tag_UDP_Packet  *UdpPacket, sockaddr_in &fromAdr )
{
    tag_UDP_Packet_Bin *UdpBin = (tag_UDP_Packet_Bin*)UdpPacket;
    if ( lastSeq != UdpBin->seqNr )
    {
        char str[32];
        unsigned idx = 0;
        lastSeq = UdpBin->seqNr;

        sprintf( eventStr, "%s %u.%u.%u.%u - %u:%u %02.02x(%u) :", 
                _strtime(str),
                fromAdr.sin_addr.S_un.S_un_b.s_b1, 
                fromAdr.sin_addr.S_un.S_un_b.s_b2, 
                fromAdr.sin_addr.S_un.S_un_b.s_b3, 
                fromAdr.sin_addr.S_un.S_un_b.s_b4,
                UdpBin->fromNodeNr,
                UdpBin->seqNr,
                UdpBin->udpType,
                UdpBin->len
                );
        while ( ( idx < (UdpBin->len-4) ) && ( idx < MAX_EVENT_PAYLOAD ) )
        {
            sprintf( str, " %02.02x", UdpBin->data[idx] );
            strcat( eventStr,str );
            ++idx;
        }
        idx = 0;
        strcat( eventStr, " : " );
        while ( ( idx < (UdpBin->len-4) ) && ( idx < MAX_EVENT_PAYLOAD ) )
        {
            if ( isalnum( UdpBin->data[idx] ) )
            {
                sprintf( str, "%c", UdpBin->data[idx] );
                strcat( eventStr, str );
            }
            else
            {
                strcat( eventStr, "." );
            }
            ++idx;
        }

        // post to main window.
        PostMessage( WM_USER+1, 0, (LPARAM)eventStr );
    }
}

void CWebrickDlg::formatOldEvent( tag_UDP_Packet  *UdpPacket, sockaddr_in &fromAdr )
{
    bool fnd = true;
    if ( lastSeq != UdpPacket->seqNr )
    {
        lastSeq = UdpPacket->seqNr;
        makePreamble( UdpPacket, fromAdr );
        if ( ( UdpPacket->source[0] == 'N' ) && ( UdpPacket->source[1] == 'N' ) )
        {
            sprintf( eventStr, "%s NewNode mac %02.02x:%02.02x:%02.02x:%02.02x:%02.02x:%02.02x", 
                    preamble,
                    UdpPacket->source[2],
                    UdpPacket->source[3],
                    UdpPacket->source[4],
                    UdpPacket->source[5],
                    UdpPacket->source[6],
                    UdpPacket->source[7]
                    );
        }
        else
        if ( ( UdpPacket->source[0] == 'A' ) && ( UdpPacket->source[1] == 'A' ) )
        {
            sprintf( eventStr, "%s Alert mac %02.02x:%02.02x:%02.02x:%02.02x:%02.02x:%02.02x", 
                    preamble,
                    UdpPacket->source[2],
                    UdpPacket->source[3],
                    UdpPacket->source[4],
                    UdpPacket->source[5],
                    UdpPacket->source[6],
                    UdpPacket->source[7]
                    );
        }
        else
        if ( ( UdpPacket->source[0] == 'S' ) && ( UdpPacket->source[1] == 'T' ) )
        {
            sprintf( eventStr, "%s from %u Time %01.01u %02.02u:%02.02u:%02.02u up: %u reset reason: %X", 
                    preamble,
                    UdpPacket->fromNodeNr,
                    UdpPacket->setPointNr,    // day
                    UdpPacket->srcChannel,   // hours
                    UdpPacket->tgtChannel,   // minutes
                    UdpPacket->toNodeNr,   // seconds
                    ((UdpPacket->curValH << 8 ) | UdpPacket->curValL),
                    UdpPacket->action   // reset reason
                    );
        }
        else
        if ( ( UdpPacket->source[0] == 'D' ) && ( UdpPacket->source[1] == 'B' ) )
        {
            sprintf( eventStr, "%s Debug Up %02u reset %02x data %02x %02x %02x %02x %02x", 
                    preamble,
                    ((UdpPacket->source[2] << 8 ) | UdpPacket->source[3]),
                    UdpPacket->source[4],
                    UdpPacket->source[5],
                    UdpPacket->source[6],
                    UdpPacket->source[7],
                    UdpPacket->source[8],
                    UdpPacket->source[9]
                    );
        }
        else
        if ( ( UdpPacket->source[0] == 'S' ) && ( UdpPacket->source[1] == 'I' ) )
        {
            sprintf( eventStr, "%s Serial "
                    " Len %02.02u %02x:%02x:%02x:%02x:%02x:%02x %c%c%c%c%c%c",

                    preamble,
                    UdpPacket->source[2],
                    UdpPacket->source[3],
                    UdpPacket->source[4],
                    UdpPacket->source[6],
                    UdpPacket->source[7],
                    UdpPacket->source[8],
                    UdpPacket->source[9],
                    UdpPacket->source[3],
                    UdpPacket->source[4],
                    UdpPacket->source[6],
                    UdpPacket->source[7],
                    UdpPacket->source[8],
                    UdpPacket->source[9]
                    );
        }
        else
        if ( UdpPacket->source[0] == 'R' )
        {
            sprintf( eventStr, "%s RTC "
                    "%02.02u:%02.02u "
                    "%02.02u:%02.02u:%02.02u "
                    "%02.02x/%02.02x", 

                    preamble,
                    UdpPacket->source[2],
                    UdpPacket->source[3],

                    UdpPacket->source[6],
                    UdpPacket->source[5],
                    UdpPacket->source[4],

                    UdpPacket->source[7],
                    UdpPacket->source[9]
                    );
        }
        else
        if ( UdpPacket->source[0] == 'T' )
        {
            sprintf( eventStr, "%s Trigger src chn:%u tgt type %u chn:%u act:%u dw:%u from:%u to:%u set:%u cnt:%u", 
                    preamble,
                    UdpPacket->srcChannel,
                    ((UdpPacket->tgtChannel >> 4) & 0x0F),  // type
                    (UdpPacket->tgtChannel & 0x0F),         // channel
                    (UdpPacket->action & 0x0F),
                    ((UdpPacket->action >> 4) & 0x0F),
                    UdpPacket->fromNodeNr,
                    UdpPacket->toNodeNr,
                    UdpPacket->setPointNr,
                    UdpPacket->curValL
                    );
            if ( UdpPacket->source[1] == 'A' )
            {
            }
            else
            if ( UdpPacket->source[1] == 'a' )
            {
            }
            else
            if ( UdpPacket->source[1] == 'D' )
            {
            }
            else
            if ( UdpPacket->source[1] == 'd' )
            {
            }
            else
            if ( UdpPacket->source[1] == 'T' )
            {
            }
            else
            if ( UdpPacket->source[1] == 't' )
            {
            }
            else
            if ( UdpPacket->source[1] == 'S' )
            {
            }
            else
            {
                fnd = false;
            }
        }
        else
        if ( UdpPacket->source[0] == 'C' )
        {
            if ( UdpPacket->source[1] == 'T' )
            {
                sprintf( eventStr, "%s Temperature src chn:%u from:%u val:%f", 
                        preamble,
                        UdpPacket->srcChannel,
                        UdpPacket->fromNodeNr,
                        ((UdpPacket->curValH << 8 ) | UdpPacket->curValL)/16.0
                        );
            }
            else
            if ( UdpPacket->source[1] == 'C' )
            {
                sprintf( eventStr, "%s Configuration Changed - from:%u", 
                        preamble,
                        UdpPacket->fromNodeNr
                        );
            }
            else
            {
                fnd = false;
            }
        }
        else
        if ( UdpPacket->source[0] == 'D' )
        {
            sprintf( eventStr, "%s Digital Out src chn:%u act:%u from:%u", 
                    preamble,
                    UdpPacket->srcChannel,
                    (UdpPacket->action & 0x0F),
                    UdpPacket->fromNodeNr
                    );
        }
        else
        if ( UdpPacket->source[0] == 'A' )
        {
            if ( UdpPacket->source[1] == 'I' )
            {
                sprintf( eventStr, "%s Analogue In src chn:%u from:%u val:%u", 
                        preamble,
                        UdpPacket->srcChannel,
                        UdpPacket->fromNodeNr,
                        UdpPacket->curValL
                        );
            }
            else
            if ( UdpPacket->source[1] == 'O' )
            {
                sprintf( eventStr, "%s Analogue Out src chn:%u from:%u val:%u", 
                        preamble,
                        UdpPacket->srcChannel,
                        UdpPacket->fromNodeNr,
                        UdpPacket->curValL
                        );
            }
            else
            if ( UdpPacket->source[1] == 'T' )
            {
                // attention
                sprintf( eventStr, "%s Attention - from:%u", 
                        preamble,
                        UdpPacket->fromNodeNr
                        );
            }
            else
            {
                fnd = false;
            }
        }
        else
        if ( UdpPacket->source[0] == 'I' )
        {
            sprintf( eventStr, "%s Infra Red "
                    "from:%u, rc5 adr:%02.02x chn:%02.02x",

                    preamble,
                    UdpPacket->fromNodeNr,

                    UdpPacket->toNodeNr,   // RC5 address
                    UdpPacket->tgtChannel   // RC5 channel
                    );
        }
        else
        {
            fnd = false;
        }

        if ( !fnd )
        {
            sprintf( eventStr, "%s Unknown src chn:%u tgt type %u chn:%u act:%u dw:%u from:%u to:%u setpoint:%u val:%u", 
                    preamble,
                    UdpPacket->srcChannel,
                    ((UdpPacket->tgtChannel >> 4) & 0x0F),  // type
                    (UdpPacket->tgtChannel & 0x0F),         // channel
                    (UdpPacket->action & 0x0F),
                    ((UdpPacket->action >> 4) & 0x0F),
                    UdpPacket->fromNodeNr,
                    UdpPacket->toNodeNr,
                    UdpPacket->setPointNr,
                    ((UdpPacket->curValH << 8 ) | UdpPacket->curValL)
                    );
        }

        // post to main window.
        PostMessage( WM_USER+1, 0, (LPARAM)eventStr );
    }
}

DWORD WINAPI ThreadProc( LPVOID lpParam ) 
{ 
    CWebrickDlg *dlg = (CWebrickDlg*)lpParam;
    unsigned char buf[128];
    tag_UDP_Packet  *UdpPacket = (tag_UDP_Packet*)buf;

    int recvLen;
    sockaddr_in fromAdr;
    int fromAdrLen;

  
    while ( !dlg->m_Shutdown )
    {
        fromAdrLen = sizeof(fromAdr);
        recvLen = recvfrom( dlg->m_UdpSocket, (char*)buf, sizeof(buf), 0, (SOCKADDR*)&fromAdr, &fromAdrLen );
        if ( recvLen < 0 )
        {
            socketError( "Socket recvFrom" );
        }
        else
        if ( recvLen > 0 )
        {
            // format
            if ( UdpPacket->udpType < UDP_Packet_NEXTBINARY )
            {
                // binary formatted event packet
                dlg->formatBinaryEvent( UdpPacket, fromAdr );
            }
            else
            {
                dlg->formatOldEvent( UdpPacket, fromAdr );
                if (   ( ( UdpPacket->source[0] == 'N' ) && ( UdpPacket->source[1] == 'N' ) )
                    || ( ( UdpPacket->source[0] == 'A' ) && ( UdpPacket->source[1] == 'A' ) ) )
                {
                    dlg->SetIp( UdpPacket->source+2 ); // pass mac address
                }
            }
        }
        else
        if ( recvLen < 0 )
        {
            // Error. 
            WSAGetLastError();
        }
    }

    return 0; 
} 
 
void CWebrickDlg::OnWmUser(WPARAM WParam, LPARAM LParam)
{
    UpdateData(TRUE);
    if ( m_LogFile != NULL )
    {
        fputs( ((char*)LParam), m_LogFile );
        fputs( "\n", m_LogFile );
    }
    m_Report = CString((char*)LParam) + "\r\n" + m_Report;
//    m_Report += (char*)LParam;
//    m_Report += "\r\n";
    m_Report = m_Report.Left( MAX_BUFFERED_DATA ); // Limit data to this amount
    // repaint.
    UpdateData(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// CWebrickDlg message handlers

BOOL CWebrickDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

    // Create UDP socket
    m_UdpSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( m_UdpSocket != INVALID_SOCKET )
    {
        sockaddr_in service;
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr("0.0.0.0"); // all local addresses
        service.sin_port = htons( 2552 );
        if ( bind( m_UdpSocket, 
                (SOCKADDR*) &service, 
                sizeof(service)) != SOCKET_ERROR) 
        {
            BOOL optval = TRUE;
            if ( setsockopt( m_UdpSocket, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval) ) != 0 )
            {
                socketError( "Socket setsockopt" );
            }
            // Create background thread to monitor the socket
            m_Thread = CreateThread( NULL, 0, ThreadProc, this, 0, NULL );
            if ( m_Thread != NULL )
            {
                // we are running.
            }
            else
            {
                MessageBox( "Unable to create Thread" );
            }
        }
        else
        {
            MessageBox( "Unable to bind socket" );
        }
    }
    else
    {
        MessageBox( "Unable to create socket" );
    }
    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWebrickDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWebrickDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWebrickDlg::OnOK() 
{
    // eXIT APPLICATION.	
    if ( m_LogFile != NULL )
    {
        fclose( m_LogFile );
        m_LogFile = NULL;
    }
    CDialog::OnOK();
}


void CWebrickDlg::OnSetAddress() 
{
    UpdateData(TRUE);
    // Flag address to be set next time NewNode seen.
    // Decode address and save.
    m_NewAdr.sin_addr.S_un.S_addr = inet_addr( m_IpAddressStr );
    if ( m_NewAdr.sin_addr.S_un.S_addr == 0 )
    {
        m_NewAdr.sin_addr.S_un.S_addr = INADDR_NONE;
    }
    if ( m_NewAdr.sin_addr.S_un.S_addr == INADDR_NONE )
    {
        // invalid address
        MessageBox( m_IpAddressStr + " is not a valid ip address" );
    }
    m_IpAddressStr = "";
    UpdateData(FALSE);
}

void CWebrickDlg::OnOpenLog() 
{
    const char * fname = "WebBrickMon.log";
    m_LogFile = fopen( fname, "w" );
}

void CWebrickDlg::OnCloseLog() 
{
    if ( m_LogFile != NULL )
    {
        fclose( m_LogFile );
        m_LogFile = NULL;
    }
}
