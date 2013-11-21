// WebrickDlg.h : header file
//

#if !defined(AFX_WEBRICKDLG_H__FE17D527_1863_4E9F_A749_EE3A9B4E7714__INCLUDED_)
#define AFX_WEBRICKDLG_H__FE17D527_1863_4E9F_A749_EE3A9B4E7714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../PIC/UDPPackets.h"

/////////////////////////////////////////////////////////////////////////////
// CWebrickDlg dialog

class CWebrickDlg : public CDialog
{
// Construction
public:
	CWebrickDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWebrickDlg)
	enum { IDD = IDD_WEBRICK_DIALOG };
	CString	m_Report;
	CString	m_IpAddressStr;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebrickDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWebrickDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnSetAddress();
	afx_msg void OnOpenLog();
	afx_msg void OnCloseLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
        void OnWmUser(WPARAM WParam, LPARAM LParam);
private:
    void SendPacket( const char* msg );
    void DoLogin();

public:
    void formatOldEvent( tag_UDP_Packet  *UdpPacket, sockaddr_in &fromAdr );
    void formatBinaryEvent( tag_UDP_Packet  *UdpPacket, sockaddr_in &fromAdr );
    void SetIp( const unsigned char *mac );

public:
    bool    m_Shutdown;
    SOCKET  m_UdpSocket;
    HANDLE  m_Thread;
    sockaddr_in m_NewAdr;
    sockaddr_in m_BrdAdr;
    FILE    *m_LogFile;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBRICKDLG_H__FE17D527_1863_4E9F_A749_EE3A9B4E7714__INCLUDED_)
