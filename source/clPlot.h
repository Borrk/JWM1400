#if !defined(AFX_CLPLOT_H__CC8A3CB9_2F72_4A0F_BBF0_A3AB8D970B4E__INCLUDED_)
#define AFX_CLPLOT_H__CC8A3CB9_2F72_4A0F_BBF0_A3AB8D970B4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// clPlot.h : header file
//
#define MAXLEGENDS	10
#define MAXSERIES	50

#define X_GRID_NUM  100
#define Y_GRID_NUM  25.6

//*******************************************************************************************************/
//* simple data value struct. used in dynamic array
//*******************************************************************************************************/
typedef struct _value{
	double	dValue;
	CTime	ValueTime;
}value;

//*******************************************************************************************************/
//* non-time axis. used for left and right y axis. might be used for x as well.
//*******************************************************************************************************/
class axis
{
public:
	CString szTitle;
	double	minrange;
	double	maxrange;

	double	m_dValuePrPixel;

	axis()
	{
		szTitle	= "Title";
		minrange	= 0.0;
		maxrange	= 25.6;//256.0;

		m_dValuePrPixel = 1;
	};
};

//*******************************************************************************************************/
//* time axis
//*******************************************************************************************************/
class timeaxis
{
public:
	CString		m_szTitle;		// time axis title;
	CTime		m_mintime;		// min time
	CTime		m_maxtime;		// max time
	int			m_iTimeMode;	// axis grid and legend interval index

	double		m_dSecondsPrPixel;

	timeaxis()
	{
		m_szTitle = "Time";
		m_mintime = 0;
		m_maxtime = 600;
		m_iTimeMode=0;
		m_dSecondsPrPixel=1;
	}
};



//*******************************************************************************************************/
//* data serie
//*******************************************************************************************************/
class serie
{
public:
	BOOL		m_bIAmInUse;
	COLORREF	m_color;		// serie line color
	int			m_iLineStyle;	// line style
	BOOL		m_bRightAxisAlign; // align to right axis
	value	*	m_pvalues;		// value array
	long		m_lNoValues;	// no values (used for array size)
	long		m_lbegin;		// list begin
	long		m_lend;			// list end
//	CPen		m_pen;			// pre-calculated pen (for speed)

	serie();
	~serie();

	void AddPoint(CTime &valuetime, double &y);	
	void Reset();
};

/////////////////////////////////////////////////////////////////////////////
// CircleData 

typedef struct _CircleData
{
	value     m_Data;
	COLORREF  m_color;
	BOOL      m_isCoorEnable;
	int		  type;                  // 0-衰减  1-反射
	value     m_otherData;

}CircleData;

class CircleDataArray
{
public:
	CircleData* m_pCircleData;
	int  m_iCircleDataNum;

	CircleDataArray();
	~CircleDataArray();

	void AddPoint(CTime &valuetime, double &y,COLORREF color,int type,value& otherData);
	BOOL SetCoorEnable(int index, BOOL isEnable);
	BOOL SetCoorEnable(CTime x, BOOL isEnable);
	inline int GetLength(){ return m_iCircleDataNum; };
	void Reset();
};

typedef struct _Staff 
{
public:
	long     m_posA;
	long	 m_posB;
	COLORREF m_staffColor;
	BOOL     m_isEnable;
}Staff;

typedef struct _MoveStartState 
{
	CPoint startPoint;
	CPoint endPoint;
	CTime  startMinX;
	double StartMinY;
}MoveStartState;
/////////////////////////////////////////////////////////////////////////////
// clPlot window

class clPlot : public CStatic
{
// Construction
public:
	clPlot();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(clPlot)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~clPlot();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(clPlot)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	// Generated message map functions
protected:
	//{{AFX_MSG(clPlot)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	CRect		m_ctlRect;			// control rect
	CRect		m_clientRect;		// ctlRect - borderspace
	CRect		m_plotRect;			// clientRect - margins
	CRect		m_legendRect;		// any rect within clientRect


	int			m_iMleft;			// left margin
	int			m_iMright;			// right margin
	int			m_iMtop;			// top margin
	int			m_iMbottom;			// bottom margin

	COLORREF	m_ctlBkColor;		// control background color
	COLORREF	m_plotBkColor;		// plot bacground color	
	COLORREF	m_gridColor;		// grid line color

	BOOL		m_bctlBorder;		// control border
	BOOL		m_bAutoScrollX;		// automatic x range scrolling


	static long	m_lMaxDataPrSerie;	// max allowed data pr. serie.

	double		m_dNoData;			// No Data Value (used for gaps)


	serie		m_series[MAXSERIES];

	axis		m_leftaxis;			// left axis
	axis		m_rightaxis;		// right axis
	timeaxis	m_timeaxis;			// bottom axis

	CPoint		*pLineArray;		// pre-calculated when new data are entered into the system
	long		lArraySize;			// current size of pLineArray

	CFont		m_font;
    LOGFONT     m_logFont;
	LOGFONT		m_zoomFont;
	double		m_dzoom;
	int			m_TextHeight;

	// 当前图形
    int m_iIndex;
	
	// 所有波形数量
	int m_allSerialNum;	

	// 圈点
	BOOL  m_isDrawDataCircle;
	CircleDataArray m_CircleDataArray;

	 // 坐标线点 -- 蓝色
	CircleData  m_activeCoorData;
    Staff       m_staff;
	
public:
    virtual void InitData();
	virtual void ComputeRects(BOOL bInitialize);

	virtual void Draw(CDC * dc);			// Draw the entire plot
	virtual void DrawBasic(CDC * dc);		// Draw plot basics
	virtual void DrawCaption(CDC * dc);
	virtual void DrawPlot(CDC * dc);		// Draw the plot series
	virtual void DrawSerie(CDC *dc, int serie);
	virtual void DrawGrid(CDC * dc);		// Draw grids
	virtual void DrawWhite(CDC * dc);       // 覆盖曲线图形多于的地方
	virtual void DrawWYScale(CDC * dc);     // 绘制X轴上的刻度数字
	virtual void DrawDataCircle(CDC * dc);  //  画标记数据
	virtual void DrawStadff(CDC * dc);		    //  a,b 标尺

	int GetStaffANumber(value &dataA);
	int GetStaffBNumber(value &dataB);
	void SetStaff(long posA, long posB, COLORREF color = RGB(255,139,23),BOOL isEnable = TRUE);
	inline void SetStaffA(long posA){  m_staff.m_posA = posA;};
	inline long GetStaffA() { return m_staff.m_posA; };
	inline void SetStaffB(long posB){  m_staff.m_posB = posB;};
	inline long GetStaffB() { return m_staff.m_posB; };	
    void StaffEnable(BOOL isEnable)
	{
		m_staff.m_isEnable = isEnable;
	};
	void AddCircleData(value& data,COLORREF color,int type,value& otherData)
	{	
		m_CircleDataArray.AddPoint(data.ValueTime,data.dValue,color,type, otherData);		
	};
    void  SetDataCircle(BOOL isDraw)
	{
		m_isDrawDataCircle = isDraw;	
	};
    BOOL SetActiveCircleDataCoor(CTime x)
	{
		for (int i = 0; i < m_CircleDataArray.GetLength(); i++)
		{
			m_CircleDataArray.m_pCircleData[i].m_isCoorEnable = FALSE;
		}
		
		return m_CircleDataArray.SetCoorEnable(x, TRUE);
	}
	BOOL GetActiveCircleData(CircleData &activeData)
	{
		for (int i = 0; i < m_CircleDataArray.GetLength(); i++)
		{
			if (m_CircleDataArray.m_pCircleData[i].m_isCoorEnable)
			{
				activeData =  m_CircleDataArray.m_pCircleData[i];
				return TRUE;
			} 
		}
        return FALSE;
	}

	
	virtual void DrawYAxisGrid(CDC * dc);
	virtual void DrawXAxisGrid(CDC * dc);

	virtual void SetBXRange(CTime &fromtime, CTime &totime,BOOL bMove=TRUE);
	virtual void SetBYRange(double fromNum, double toNum, BOOL bMove = TRUE);

	void LeftMoveX(int serie,int index);
	void RightMoveX(int serie,int index);

	void DownMoveY(int serie,int index);
	void UpMoveY(int serie,int index);
	
	virtual void SetSerie(int s, int style, COLORREF color, double minrange, double maxrange, const char *szTitle, BOOL Rightalign=FALSE);

	virtual BOOL AddPoint(int serie, CTime &valuetime, double &y);
public:
	double      m_extendSizeX;
	double      m_extendSizeY;
	CPoint      m_dClickPoint;
	void SerieExtendX(double extendSizeX);
	void SerieExtendY(double extendSizeY);
	void SerieExtendXOnePoint(CTime &extendPoint,int extendSize);
	void SerieExtendYOnePoint(double &y,int extendSize);
	void SerieMoveDependAPoint(value &pt);

	void Reset();
	BOOL SetIndexSerieNum(int iNum);
	BOOL SetSerieNum(int iNum);
	BOOL SetRealSerial2();



	BOOL    m_isBtnDown;
	MoveStartState m_moveState;
//	CPoint  m_startPoint;
//	CPoint  m_endPoint;	

	int    m_iLimitNum;
	CSliderCtrl*	m_CSliCtrX;
	CSliderCtrl*	m_CSliCtrY;
	void SetLimitNum(int iLimit){  m_iLimitNum = iLimit; };


	// 工具函数  CPoint ->CircleData
	BOOL CovertToValue(CPoint &point, CircleData &val);
	double GetSerieYFromX(CTime time);

	// 取当前横轴和纵轴的限定值
	CTime  MinX;
	CTime  MaxX;
	double MinY;
	double MaxY;
	void ReSetCoorMaxMin();                                     //  设置坐标轴的移动边界值

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLPLOT_H__CC8A3CB9_2F72_4A0F_BBF0_A3AB8D970B4E__INCLUDED_)
