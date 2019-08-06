#if !defined(AFX_SETTINGDLG_H__D3D31FE3_927A_48ED_877E_94DB1F3A4616__INCLUDED_)
#define AFX_SETTINGDLG_H__D3D31FE3_927A_48ED_877E_94DB1F3A4616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingDlg.h : header file
//

#include "CSeriesPortPro.h"
/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog

class CSettingDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CSettingDlg)

// Construction
public:
	CSettingDlg();
	~CSettingDlg();
public:
	void InitData();
	void InitGraphParaTable();

public:
	void ShowResponseMsg();
	
private:
	CSeriesPortPro* m_packPro;

private:
	int  m_FocesID;
	long m_startTime;

public:
	void SaveAllData();

	void UpdateGraphParaArr(int index);
	void UpdateGraphParaTable(int index);

// Dialog Data
	//{{AFX_DATA(CSettingDlg)
	enum { IDD = IDD_DLG_SETTING };
	CListCtrl	m_listParaList;
	int		m_iComNum;
	int		m_iBitRate;
	int		m_iVerifyLen;
	int		m_iDataLen;
	int		m_iStopLen;
	int		m_iPulsWide;
	int		m_iWave;
	int		m_iLength;
	int		m_iBMultipleA;
	int		m_iBMultipleT;
	int		m_iLDPower;
	int		m_iAPD;
	int		m_iAPDOpen;
	int		m_iAPDMode;
	CString	m_editSendData;
	int		m_iSamplingTimes;
	int		m_iReflect;
	int		m_iAttenuation;
	int		m_iEventTimes;
	int		m_iSwing;
	//}}AFX_DATA
private:
	
public:
	BOOL ComOpen();
	
	void WriteData(byte* buffer,int size,BOOL isSend);

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSettingDlg)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSettingDlg)
	afx_msg LRESULT OnComRevc(WPARAM, LPARAM);   //
	afx_msg void OnSelchangeCMBComNum();
	afx_msg void OnSelchangeCMBBitRate();
	afx_msg void OnSelchangeCMBVerify();
	afx_msg void OnSelchangeCMBDataLen();
	afx_msg void OnSelchangeCMBStopLen();
	afx_msg void OnBTNOpenPort();
	afx_msg void OnBtnWrite();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnClear();
	afx_msg void OnDblclkListParaList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBTNStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// ¹¤¾ßº¯Êý
	long	Time2Long( SYSTEMTIME& st )
	{
		CTime lTime(st.wYear, 
			st.wMonth,  
			st.wDay, 
			st.wHour,  
			st.wMinute,  
			st.wSecond,  
			st.wMilliseconds); 
		
		return lTime.GetTime(); 
	}

  int countxx;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGDLG_H__D3D31FE3_927A_48ED_877E_94DB1F3A4616__INCLUDED_)
