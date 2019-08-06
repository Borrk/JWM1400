#if !defined(AFX_SHOWDLG_H__E62E9CDC_C6DD_44B9_AC73_5B58914B3EA9__INCLUDED_)
#define AFX_SHOWDLG_H__E62E9CDC_C6DD_44B9_AC73_5B58914B3EA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShowDlg.h : header file
//
#include "clPlot.h"

/////////////////////////////////////////////////////////////////////////////
// CShowDlg dialog

class CShowDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CShowDlg)

// Construction
public:
	CShowDlg();
	~CShowDlg();

private:
	CSeriesPortPro* m_packPro;

	int m_iSampleTimes;
	int m_Wave;
	GraphLimit m_GraphLimt;

public:
	inline void InitData();
	inline void InitDataAnaylseTable();	// 初始化分析数据表的列表框
	inline void InitAnaylseData();
	inline void initStaff();
	inline void InitExtend();

public:
	void SetSamplingNumberCommand();
	void ProPackData();
	void AddPlotPointAndShow(CByteArray& tempArry,int style);
	void SetEndSamplingState();	


	// 读写图形-new
	BOOL ReadGraph(CString fileName);
	void WriteGraph(CString fileName);

		// 分析数据
	void DataAnaylse();
    void insertAnaylseTable(CString strCaption, long position1, double position2,double para1,double para2);

	// 游标数据分析
	void ShownStaffNumber(double ax1, double ay1, double ax2,double  ay2,double bx1, double by1,double bx2,double by2);
	void StaffNumber();

    void PlotWholeExtend2(bool isUp);							// 整体移动

	void XChange(bool isUp,CircleData &data);
	void YChange(bool isUp,CircleData &data);
	
protected:
    int m_csliOldXIndex;
	int m_csliOldYIndex;
	
	int m_iIndexSerival;
	
// Dialog Data
	//{{AFX_DATA(CShowDlg)
	enum { IDD = IDD_DLG_SHOW };
	CSliderCtrl	m_scliderStaff;
	CListCtrl	m_listAnaylseTable;
	clPlot	m_Plot;
	int		m_iTimeOut;
	CString	m_strXModulus;
	int		m_iExtendSizeX;
	int		m_iExtendSizeY;
	int		m_iStaff;
	int		m_iZoom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CShowDlg)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CShowDlg)
	afx_msg LRESULT OnComRevc(WPARAM, LPARAM);   //
	virtual BOOL OnInitDialog();
	afx_msg void OnBTNSetting();
	afx_msg void OnBTNStart();
	afx_msg void OnBTNDataAnaylse();
	afx_msg void OnChangeEditExtendsizex();
	afx_msg void OnChangeEditExtendsizey();
	afx_msg void OnBTNAllClear();
	afx_msg void OnBTNOpenFile();
	afx_msg void OnBTNSaveData();
	afx_msg void OnRADIOStaffA();
	afx_msg void OnRADIOStaffB();
	afx_msg void OnCustomdrawSLIDERStaff(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCHECKAnalyseCircle();
	afx_msg void OnDblclkLISTDataAnaylseTable(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRADIOXUp();
	afx_msg void OnRADIOXDown();
	afx_msg void OnRADIOYUp();
	afx_msg void OnRADIOYDown();
	afx_msg void OnRadioUp();
	afx_msg void OnRadioDown();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// 工具函数
	double ADC2dB(double adc1, double adc2);
	inline double ConverToRealX(CTime valX);
	inline double ConverToRealY(double valY);
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

#endif // !defined(AFX_SHOWDLG_H__E62E9CDC_C6DD_44B9_AC73_5B58914B3EA9__INCLUDED_)
