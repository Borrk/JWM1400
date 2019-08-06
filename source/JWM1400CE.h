// JWM1400CE.h : main header file for the JWM1400CE application
//

#if !defined(AFX_JWM1400CE_H__7B382259_034D_4D31_87C5_CC04374D8434__INCLUDED_)
#define AFX_JWM1400CE_H__7B382259_034D_4D31_87C5_CC04374D8434__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "CommuMrg.h"
/////////////////////////////////////////////////////////////////////////////
// CJWM1400CEApp:
// See JWM1400CE.cpp for the implementation of this class
//

class CJWM1400CEApp : public CWinApp
{
public:
	CJWM1400CEApp();

	static CJWM1400CEApp& GetApp();

public:	
	CCommuMrg* GetAppCommuMgr() { return &comMrg; };	

protected:
	CCommuMrg  comMrg;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJWM1400CEApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CJWM1400CEApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JWM1400CE_H__7B382259_034D_4D31_87C5_CC04374D8434__INCLUDED_)
