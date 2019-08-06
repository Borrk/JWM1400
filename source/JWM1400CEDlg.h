// JWM1400CEDlg.h : header file
//

#if !defined(AFX_JWM1400CEDLG_H__16DFA36B_AF97_4ACD_BE63_A66221FFFA5E__INCLUDED_)
#define AFX_JWM1400CEDLG_H__16DFA36B_AF97_4ACD_BE63_A66221FFFA5E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "clPlot.h"
#include "CnComm.h"
#include "CSeriesPortPro.h"
/////////////////////////////////////////////////////////////////////////////
// CJWM1400CEDlg dialog

class CJWM1400CEDlg : public CDialog
{
// Construction
public:
	CJWM1400CEDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CJWM1400CEDlg)
	enum { IDD = IDD_JWM1400CE_DIALOG };
	CSliderCtrl	m_CSliCtrY;
	CSliderCtrl	m_CSliCtrX;
	clPlot	m_Plot;
	int		m_iBitRate;
	int		m_iComNum;
	int		m_iDataLen;
	int		m_iStopLen;
	int		m_iVerifyLen;
	int		m_iBMultipleA;
	int		m_iBMultipleT;
	int		m_iLength;
	int		m_iPulsWide;
	int		m_iWave;
	int		m_iAPD;
	int		m_iLDPower;
	CString	m_editSendData;
	int		m_iAPDOpen;
	int		m_iAPDMode;
	int		m_iSamplingTimes;
	int		m_iLimit;
	int		m_iExtendSizeX;
	int		m_iExtendSizeY;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJWM1400CEDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CJWM1400CEDlg)
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnComRevc(WPARAM, LPARAM);   //
	afx_msg void OnCustomdrawSliderx(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSlidery(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBTNOpenPort();
	afx_msg void OnBtnWrite();
	afx_msg void OnBTNStart();
	afx_msg void OnChangeEditExtendsizex();
	afx_msg void OnChangeEditExtendsizey();
	afx_msg void OnChangeEditLimit();
	afx_msg void OnBtnClear();
	afx_msg void OnSelchangeCMBBitRate();
	afx_msg void OnSelchangeCMBComNum();
	afx_msg void OnSelchangeCMBDataLen();
	afx_msg void OnSelchangeCMBStopLen();
	afx_msg void OnSelchangeCMBVerify();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CnComm  m_comm;
	void ComSetState();

	int m_csliOldXIndex;
	int m_csliOldYIndex;
	
	int m_iIndexSerival;
	CSeriesPortPro m_packPro;
	int GetDataFromPack();
	void ProReadPackWaveData(CByteArray& tempArry,int style);



public:
	void  InitData();
	
	// byte 转 CString 显示在界面上
	void WriteData(byte* buffer,int size,BOOL isSend);

	void ProReadPackCommand(CByteArray& tempArry);

	void SetSamplingNumberCommand();

	void SetStartSamplingState();
	void SetEndSamplingState(); 
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JWM1400CEDLG_H__16DFA36B_AF97_4ACD_BE63_A66221FFFA5E__INCLUDED_)
