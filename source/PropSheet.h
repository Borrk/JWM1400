#if !defined(AFX_PROPSHEET_H__3D17C38B_475B_439E_9040_921532272C3E__INCLUDED_)
#define AFX_PROPSHEET_H__3D17C38B_475B_439E_9040_921532272C3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropSheet

class CPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSheet)

// Construction
public:
	CPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSHEET_H__3D17C38B_475B_439E_9040_921532272C3E__INCLUDED_)
