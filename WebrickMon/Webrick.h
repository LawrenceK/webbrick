// Webrick.h : main header file for the WEBRICK application
//

#if !defined(AFX_WEBRICK_H__5D30D8FD_C1E6_4DAD_BC52_DF0C8777353A__INCLUDED_)
#define AFX_WEBRICK_H__5D30D8FD_C1E6_4DAD_BC52_DF0C8777353A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWebrickApp:
// See Webrick.cpp for the implementation of this class
//

class CWebrickApp : public CWinApp
{
public:
	CWebrickApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebrickApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWebrickApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBRICK_H__5D30D8FD_C1E6_4DAD_BC52_DF0C8777353A__INCLUDED_)
