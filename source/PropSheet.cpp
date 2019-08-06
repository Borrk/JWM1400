// PropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "JWM1400CE.h"
#include "PropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropSheet

IMPLEMENT_DYNAMIC(CPropSheet, CPropertySheet)

CPropSheet::CPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropSheet::CPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CPropSheet::~CPropSheet()
{
}


BEGIN_MESSAGE_MAP(CPropSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CPropSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSheet message handlers

BOOL CPropSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	// TODO: Add your specialized code here
	int ids[] = { IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP };
    for(int i = 0; i < sizeof(ids)/sizeof(int); ++i ) 
    {  
        CWnd* pWnd = GetDlgItem(ids[i]);
		if ( pWnd )
			pWnd->ShowWindow(SW_HIDE);
    }
	
    CRect rectOK;
    CWnd* pWnd = GetDlgItem( IDOK ); 
    if ( pWnd )
		pWnd->GetWindowRect( &rectOK ); 

    CRect rect;
    GetWindowRect( &rect );
	SetWindowPos(NULL, 0, 0,rect.Width() + 100,rect.Height()-2,SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

#define FULL_SCREEN
#ifdef FULL_SCREEN
	/// Full Screen
	int iFullWidth = GetSystemMetrics(SM_CXSCREEN);
	int iFullHeight = GetSystemMetrics(SM_CYSCREEN);
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, 0, 0, iFullWidth, iFullHeight, SWP_NOOWNERZORDER|SWP_SHOWWINDOW);
#endif

	static CFont m_fontEdit;
	m_fontEdit.CreateFont( 25, 0, 0, 30, 700, 0, 0, 0, 1,
			0, 0, 0, 0, _T("Arial") );
	GetTabControl()->SetFont( &m_fontEdit );
 
	this->ModifyStyle( WS_SYSMENU | WS_CAPTION | CS_DBLCLKS , 0, 0 );

	return bResult;
}

BOOL CPropSheet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CPropertySheet::PreTranslateMessage(pMsg);
}
