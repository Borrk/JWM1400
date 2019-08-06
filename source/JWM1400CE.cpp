// JWM1400CE.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "JWM1400CE.h"
#include "JWM1400CEDlg.h"

#include "PropSheet.h"
#include "SettingDlg.h"
#include "ShowDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJWM1400CEApp

BEGIN_MESSAGE_MAP(CJWM1400CEApp, CWinApp)
	//{{AFX_MSG_MAP(CJWM1400CEApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJWM1400CEApp construction

CJWM1400CEApp::CJWM1400CEApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CJWM1400CEApp object

CJWM1400CEApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CJWM1400CEApp initialization
CJWM1400CEApp& CJWM1400CEApp::GetApp()
{
    return theApp;
}

BOOL CJWM1400CEApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
/*
	CJWM1400CEDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
*/
	comMrg.InitCommuMrg();

	CPropSheet sheet(_T("JWM1400"));
	CSettingDlg page1;
	CShowDlg    page2;

	sheet.AddPage(&page1);
	sheet.AddPage(&page2);
	m_pMainWnd = &sheet;
	int nResponse = sheet.DoModal();
	
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
