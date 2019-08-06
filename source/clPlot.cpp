// clPlot.cpp : implementation file
//

#include "stdafx.h"
#include "JWM1400CE.h"
#include "clPlot.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

long	clPlot::m_lMaxDataPrSerie;	// max allowed data pr. serie.
//*******************************************************************************************************/
//* Function:		serie::serie
//*******************************************************************************************************/
serie::serie()
{
	m_bIAmInUse		= FALSE;
	m_color			= RGB(0,0,0);
	m_iLineStyle	= PS_SOLID;
	m_bRightAxisAlign	= FALSE;
	m_lNoValues		= 0;
	m_lbegin		= 0;
	m_lend			= 0;
	m_pvalues		= NULL;

}

//*******************************************************************************************************/
//* Function:		serie::~serie
//*******************************************************************************************************/
serie::~serie()
{
	if(m_pvalues !=NULL)
		free(m_pvalues);
}

//*******************************************************************************************************/
//* Function:		serie::AddPoint
//*
//* Description:	AddPoint add new data to the end of a data serie. It will simply append the data,
//*					update the list index and get out. 
//*
//*					This function will also call realloc or malloc to re-size or create the plot array as
//*					needed.
//*
//*					The nice thing about circular lists is that they are multi thread enabled as is. You
//*					must however implement a syncronization mechanism if more than one thread is supposed
//*					to enter data into the plot.
//*
//* Parameters:		valuetime		Time (x value).
//*					y				y value
//*
//* Return Value:	-none-
//*
//* Author:			Jan Vidar Berger
//*******************************************************************************************************/
void serie::AddPoint(CTime &valuetime , double &y)
{
	if(m_lNoValues > 0)
		m_pvalues = (value*)realloc(m_pvalues, (m_lNoValues+1)*sizeof(value));
	else
		m_pvalues = (value*)malloc((m_lNoValues+1)*sizeof(value));

	m_pvalues[m_lend].ValueTime = valuetime;
	m_pvalues[m_lend].dValue	= y;
	m_lNoValues++;
	m_lend++;
	if(m_lend >= clPlot::m_lMaxDataPrSerie)
		m_lend=0;
	if(m_lbegin == m_lend){
		m_lbegin++;
		if(m_lbegin >= clPlot::m_lMaxDataPrSerie)
			m_lbegin=0;
	}
}

//*******************************************************************************************************/
//* Function:		serie::Reset
//*
//* Description:	Reset the serie. Remove data and reset indexes and pointers.
//*
//* Parameters:		-none-
//*
//* Return Value:	-none-
//*
//* Author:			Jan Vidar Berger
//*******************************************************************************************************/
void serie::Reset()
{
	m_lNoValues=0;

	if(m_pvalues !=NULL)
		free(m_pvalues);

	m_pvalues = NULL;

	m_lbegin		= 0;
	m_lend			= 0;
}

//*******************************************************************************************************/
// CircleDataArray
//*******************************************************************************************************/
CircleDataArray::CircleDataArray()
{
	m_iCircleDataNum = 0;
	m_pCircleData = NULL;
}
CircleDataArray::~CircleDataArray()
{
	if (m_pCircleData != NULL)
	{
		free(m_pCircleData);
	}
	m_pCircleData = NULL;
}

void CircleDataArray::AddPoint(CTime &valuetime, double &y,COLORREF color,int type,value& otherData)
{
	if (m_iCircleDataNum > 0)
		m_pCircleData = (CircleData*)realloc(m_pCircleData,sizeof(CircleData)*(m_iCircleDataNum+1));
	else
		m_pCircleData = (CircleData*)malloc(sizeof(CircleData)*(m_iCircleDataNum+1));

	m_pCircleData[m_iCircleDataNum].m_Data.ValueTime = valuetime;
	m_pCircleData[m_iCircleDataNum].m_Data.dValue = y;
	m_pCircleData[m_iCircleDataNum].m_color = color;
	m_pCircleData[m_iCircleDataNum].type = type;
	m_pCircleData[m_iCircleDataNum].m_isCoorEnable = FALSE;
    m_pCircleData[m_iCircleDataNum].m_otherData = otherData;

	m_iCircleDataNum++;	
}

BOOL CircleDataArray::SetCoorEnable(int index, BOOL isEnable)
{
	if (index < 0 && index >= GetLength())
	{
		return FALSE;
	}
	m_pCircleData[index].m_isCoorEnable = isEnable;
	return TRUE;
}

BOOL CircleDataArray::SetCoorEnable(CTime x, BOOL isEnable)
{
	for(int i = 0; i < GetLength(); i++)
	{
		if (m_pCircleData[i].m_Data.ValueTime == x)
		{
			m_pCircleData[i].m_isCoorEnable = isEnable;
			return TRUE;
		}
	}
    return FALSE;
}
	
void CircleDataArray::Reset()
{
	m_iCircleDataNum = 0;
	if (m_pCircleData != NULL)
		free(m_pCircleData);
	m_pCircleData = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// clPlot

clPlot::clPlot()
{
		m_ctlBkColor		= RGB(255,255,255);
	m_plotBkColor		= RGB(255,255,255);

	m_gridColor			= RGB(127,127,127);
	m_bctlBorder		= TRUE;


	m_lMaxDataPrSerie	= 65535;

	m_dNoData			= 0.0;

	m_dzoom				= 1.0;
	
	lArraySize			= 65536;			// only points with differebt x,y will be put into the array

	pLineArray			= new CPoint[lArraySize];
	
	
	m_extendSizeX = 1;
	m_extendSizeY = 1;
	SetBXRange(CTime(0) ,CTime(0) + CTimeSpan(X_GRID_NUM*m_extendSizeX));

	m_logFont.lfHeight			= -13;
	m_logFont.lfWidth			= 0;
	m_logFont.lfEscapement		= 0;
	m_logFont.lfOrientation		= 0;
	m_logFont.lfWeight			= 400;
	m_logFont.lfItalic			= FALSE;
	m_logFont.lfUnderline		= FALSE;
	m_logFont.lfStrikeOut		= FALSE;
	m_logFont.lfCharSet			= ANSI_CHARSET;
	m_logFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	m_logFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
//	m_logFont.lfQuality			= PROOF_QUALITY;
	m_logFont.lfPitchAndFamily	= DEFAULT_PITCH;
//	strcpy(m_logFont.lfFaceName,"Ariel");

	m_zoomFont.lfHeight			= -13;
	m_zoomFont.lfWidth			= 0;
	m_zoomFont.lfEscapement		= 0;
	m_zoomFont.lfOrientation		= 0;
	m_zoomFont.lfWeight			= 400;
	m_zoomFont.lfItalic			= FALSE;
	m_zoomFont.lfUnderline		= FALSE;
	m_zoomFont.lfStrikeOut		= FALSE;
	m_zoomFont.lfCharSet			= ANSI_CHARSET;
	m_zoomFont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	m_zoomFont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
//	m_zoomFont.lfQuality			= PROOF_QUALITY;
	m_zoomFont.lfPitchAndFamily	= DEFAULT_PITCH;
//	strcpy(m_zoomFont.lfFaceName,"Ariel");

	m_font.CreateFontIndirect(&m_zoomFont);


	// 当前波形
	m_iIndex = 0;
	// 所有波形数量
    m_allSerialNum = 0;
	m_isBtnDown = FALSE;

	m_iLimitNum = 20;
	m_isDrawDataCircle = FALSE;

	SetStaff(0,0);
}

clPlot::~clPlot()
{
}


BEGIN_MESSAGE_MAP(clPlot, CStatic)
	//{{AFX_MSG_MAP(clPlot)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()	
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// clPlot message handlers
void clPlot::InitData()
{
	CRect rect;
	this->GetWindowRect(&rect);
	m_ctlRect = rect;
	ScreenToClient(m_ctlRect);
	ComputeRects(TRUE);
}
void clPlot::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC  pdc(&dc);  // non flickering painting
    Draw(&pdc);
	
	// Do not call CStatic::OnPaint() for painting messages
}


void clPlot::ComputeRects(BOOL bInitialization)
{
	// adjust the client rect for borders

	//GetClientRect(m_ctlRect);
	CClientDC dc(this);
	int w = 0;
	int n=0;
	CSize z=dc.GetTextExtent(CString("A"));

	m_dzoom = ((double)m_ctlRect.Height()/(double)z.cy) / 25.0;

	m_zoomFont.lfWidth = (int)(m_logFont.lfWidth * m_dzoom);
	m_zoomFont.lfHeight = (int)(m_logFont.lfHeight * m_dzoom);
	m_font.Detach();
	m_font.CreateFontIndirect(&m_zoomFont);
	CFont *oFont = dc.SelectObject(&m_font);
	SetFont(&m_font);
	z=dc.GetTextExtent(CString("A"));
	m_TextHeight = z.cy;

	m_clientRect = m_ctlRect;


	if(bInitialization)
	{
		m_iMtop = m_iMbottom = m_clientRect.Height()/10;
		m_iMleft = m_iMright = m_clientRect.Width()/10;
	}

	// compute plot rect.
	m_plotRect.left = m_clientRect.left + m_iMleft - 15;
	m_plotRect.right = m_clientRect.right - m_iMright + 15;;
	m_plotRect.top = m_clientRect.top + m_iMtop - 10;
	m_plotRect.bottom = m_clientRect.bottom - m_iMbottom + 10;

	m_timeaxis.m_dSecondsPrPixel = ((double)(m_timeaxis.m_maxtime.GetTime() - m_timeaxis.m_mintime.GetTime())) / (double)m_plotRect.Width();
	m_leftaxis.m_dValuePrPixel   = ((double)(m_leftaxis.maxrange- m_leftaxis.minrange) / (double)m_plotRect.Height());
	m_rightaxis.m_dValuePrPixel   = ((double)(m_rightaxis.maxrange- m_rightaxis.minrange) / (double)m_plotRect.Height());

	dc.SelectObject(oFont);
}
//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::Draw(CDC * dc)
{
	CFont *oFont = dc->SelectObject(&m_font);
		
	DrawBasic(dc);
	DrawGrid(dc);
	DrawPlot(dc);
	
	if (m_isDrawDataCircle)
	{
		DrawDataCircle(dc);
	}

	DrawWhite(dc);
    DrawCaption(dc);
	DrawStadff(dc);

	dc->SelectObject(oFont);
}

//*******************************************************************************************************/
// clPlot::SetStaff
//*******************************************************************************************************/
void clPlot::DrawStadff(CDC * dc)
{
	CPen *old,staffPen(PS_SOLID, 2,m_staff.m_staffColor);
    old = dc->SelectObject(&staffPen);
	if (m_staff.m_isEnable)
	{
		if (m_staff.m_posA >= m_plotRect.left && m_staff.m_posA <= m_plotRect.right)
		{
			dc->MoveTo(m_staff.m_posA, m_ctlRect.top);
			dc->LineTo(m_staff.m_posA, m_ctlRect.bottom);
			char a = 'a';
			dc->DrawText(a, CRect(m_staff.m_posA,m_ctlRect.top,m_staff.m_posA+15,m_ctlRect.top+15), DT_RIGHT|DT_BOTTOM);
		}
		if (m_staff.m_posB >= m_plotRect.left && m_staff.m_posB <= m_plotRect.right)
		{
			dc->MoveTo(m_staff.m_posB, m_ctlRect.top);
			dc->LineTo(m_staff.m_posB, m_ctlRect.bottom);
			char b = 'b';
			dc->DrawText(b, CRect(m_staff.m_posB,m_ctlRect.top,m_staff.m_posB+15,m_ctlRect.top+15), DT_RIGHT|DT_BOTTOM);
		}
	}
	dc->SelectObject(old);
}  

void clPlot::SetStaff(long posA, long posB, COLORREF color,BOOL isEnable)
{
	m_staff.m_posA = posA;
	m_staff.m_posB = posB;

	m_staff.m_staffColor = color;
	m_staff.m_isEnable = isEnable;
}

//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::DrawBasic(CDC * dc)
{
	CBrush brushctlBkColor(m_ctlBkColor);
	dc->FillRect(m_ctlRect,&brushctlBkColor);
	if(m_bctlBorder)
	{
		dc->DrawEdge(m_ctlRect,BDR_SUNKENINNER|BDR_SUNKENOUTER, BF_RECT); 
	}
	dc->Rectangle(m_plotRect);	
}

//*******************************************************************************************************/
//*
//*******************************************************************************************************/
void clPlot::DrawGrid(CDC * dc)
{
	DrawXAxisGrid(dc);
	DrawYAxisGrid(dc);
}

//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::DrawXAxisGrid(CDC * dc)
{
	long lMax = m_timeaxis.m_maxtime.GetTime();
	long lMin = m_timeaxis.m_mintime.GetTime();
	long yGrid = lMin;	
    double prPixel = m_timeaxis.m_dSecondsPrPixel;
	long delta = (long)(10*m_extendSizeX);
	double d10 = (double)(delta / 10.0);

	// todo: delta switch
	long diff = fabs((yGrid)%((long)delta));  
	
	if(yGrid >= 0)
		yGrid = yGrid / delta * delta; 
	else
		yGrid = ( yGrid / delta  - 1) *delta ;
	
	CPen *old, pen(PS_SOLID, 1, m_gridColor);
	CPen stick(PS_SOLID,0,RGB(0,0,0));
	CPen mline(PS_SOLID,0,RGB(192,192,192));

	for( long sx = yGrid; sx < lMax; sx+=d10)
	{
		int off=3;
		if(sx/delta == 0){
			off=5;
		}

		if(sx > (double)(lMin)){
			int x = (int)(m_plotRect.left + ((sx-(lMin))/prPixel)); 

			old = dc->SelectObject(&stick); 
			dc->MoveTo(CPoint(x,m_plotRect.bottom));
			dc->LineTo(CPoint(x+off,m_plotRect.bottom));
 			dc->SelectObject(old);

			old = dc->SelectObject(&mline);
			dc->MoveTo(CPoint(x,m_plotRect.bottom-1));
			dc->LineTo(CPoint(x,m_plotRect.top+1));
			dc->SelectObject(old);
		}
	}

	old = dc->SelectObject(&pen);
	yGrid = (long)yGrid;
	while(yGrid <= lMax)
	{
		int x = (int)(m_plotRect.left + ((yGrid-lMin)/prPixel));

		if(yGrid > lMin && yGrid< lMax)
		{
			dc->MoveTo(CPoint(x,m_plotRect.bottom-1));
			dc->LineTo(CPoint(x,m_plotRect.top+1));
		

		   char b[100];
		   sprintf(b, "%d", yGrid);
		   dc->DrawText(b, CRect(x-m_TextHeight*3,m_plotRect.bottom,x+m_TextHeight/2,m_plotRect.bottom+m_TextHeight), DT_RIGHT|DT_BOTTOM);

		}
		yGrid += delta;
	}
	dc->SelectObject(old);
}

//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::DrawYAxisGrid(CDC * dc)
{
	double yGrid = (long)m_leftaxis.minrange;
	//double delta = 25.0 + (long)(((m_leftaxis.m_dValuePrPixel)))*25;
	//if((long)delta%50 != 0 && delta > 20.0)
	//	delta +=25;
	double delta = 5 * m_extendSizeY; //50
	
	double d10 = delta / 5.0;
	// todo: delta switch
	long diff = ((long)yGrid)%((long)delta);  
	
	if(yGrid >= 0)
		yGrid = yGrid - diff; 		
    else
		yGrid = yGrid - diff - delta ;
	
	
	CPen *old, pen(PS_SOLID, 1, m_gridColor);
	CPen stick(PS_SOLID,0,RGB(0,0,0));
	CPen mline(PS_SOLID,0,RGB(192,192,192));
	
 	for(long sy = yGrid /*(long)((long)(m_leftaxis.minrange) - diff)*/; sy < m_leftaxis.maxrange; sy+=(long)d10)  // 类型的不同导致精确度偏差
	{
//	for( double sy = m_leftaxis.minrange - diff; sy < m_leftaxis.maxrange; sy+=(long)d10)
//	{
		int off=3;
		if((long)sy%(long)delta == 0){
			off=5;
		}
		if(sy > m_leftaxis.minrange){
			int y = (int)(m_plotRect.bottom - ((sy-m_leftaxis.minrange)/m_leftaxis.m_dValuePrPixel));
			old = dc->SelectObject(&stick); 
			dc->MoveTo(CPoint(m_plotRect.left,y));
			dc->LineTo(CPoint(m_plotRect.left-off,y));
			dc->MoveTo(CPoint(m_plotRect.right,y));
			dc->LineTo(CPoint(m_plotRect.right+off,y));
			dc->SelectObject(old);

			old = dc->SelectObject(&mline);
			dc->MoveTo(CPoint(m_plotRect.left+1,y));
			dc->LineTo(CPoint(m_plotRect.right-1,y));
			dc->SelectObject(old);
		}
	}

	old = dc->SelectObject(&pen);
	yGrid = (long)yGrid;
	while(yGrid <= m_leftaxis.maxrange)
	{
		double yy = m_plotRect.bottom - ((yGrid-m_leftaxis.minrange)/m_leftaxis.m_dValuePrPixel);
		int y = (int)yy;
		if(yGrid > m_leftaxis.minrange && yGrid<m_leftaxis.maxrange){
			dc->MoveTo(CPoint(m_plotRect.left+1,y));
			dc->LineTo(CPoint(m_plotRect.right-1,y));
		

			char b[100];
			sprintf(b, "%.0f", yGrid);
			dc->DrawText(b, CRect(m_clientRect.left, y-m_TextHeight/2,m_plotRect.left-5,y+m_TextHeight/2), DT_RIGHT|DT_BOTTOM);
			dc->DrawText(b, CRect(m_plotRect.right+5, y-m_TextHeight/2,m_clientRect.right,y+m_TextHeight/2), DT_LEFT|DT_BOTTOM);
		}
		yGrid += delta;
	}
	dc->SelectObject(old);
}

//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::DrawPlot(CDC * dc)
{
	if(m_series[m_allSerialNum].m_bIAmInUse)
	{
		if (m_allSerialNum < MAXSERIES)
		{
			// 画波形
			DrawSerie(dc, m_allSerialNum);		//m_iIndex
		}		
	}	

}
//*******************************************************************************************************/
//*******************************************************************************************************/
BOOL clPlot::SetRealSerial2()
{
	// 清空重置
	m_series[m_allSerialNum].Reset();

	for (int i=0; i < m_series[0].m_lNoValues; i++)
	{
		double average;
		double sum = 0;
		for (int j = 0; j <= m_iIndex; j++)
		{
			sum += m_series[j].m_pvalues[i].dValue;
		}
		average = sum/(m_iIndex+1);
		m_series[m_allSerialNum].AddPoint(m_series[0].m_pvalues[i].ValueTime,average);		
	}
	//m_iIndex = realNum;
    return TRUE;
}

void clPlot::DrawDataCircle(CDC * dc)
{	
	CPoint p;
	time_t valuetime; 
	for (int i = 0; i< m_CircleDataArray.GetLength(); i++)
	{	
	    valuetime = m_CircleDataArray.m_pCircleData[i].m_Data.ValueTime.GetTime();
		p.x = (int)(m_plotRect.left + ((valuetime-m_timeaxis.m_mintime.GetTime())/m_timeaxis.m_dSecondsPrPixel));
		p.y = (int)(m_plotRect.bottom - ((m_CircleDataArray.m_pCircleData[i].m_Data.dValue-m_leftaxis.minrange)/m_leftaxis.m_dValuePrPixel));

		CBrush brush(m_CircleDataArray.m_pCircleData[i].m_color);
		CBrush* oldBrush = dc->SelectObject(&brush);
		if ( p.x >= m_plotRect.left && p.x <= m_plotRect.right && p.y >= m_plotRect.top && p.y <= m_plotRect.bottom)
		{
			dc->Ellipse(p.x-4, p.y-4, p.x+4, p.y+4);
		}
		dc->SelectObject(oldBrush);

		// draw coordinate of circle data 
		if (m_CircleDataArray.m_pCircleData[i].m_isCoorEnable && p.x >= m_plotRect.left && p.x <= m_plotRect.right)
		{
			CPen *old,coorLine(PS_SOLID,2,RGB(0,0,255));
			old = dc->SelectObject(&coorLine);
			dc->MoveTo(p.x,m_plotRect.bottom);
			dc->LineTo(p.x,m_plotRect.top);
		    dc->SelectObject(old);

			// show distance		
			CPoint otherPoint;
			valuetime = m_CircleDataArray.m_pCircleData[i].m_otherData.ValueTime.GetTime();
			otherPoint.x = (int)(m_plotRect.left + ((valuetime-m_timeaxis.m_mintime.GetTime())/m_timeaxis.m_dSecondsPrPixel));
			otherPoint.y = (int)(m_plotRect.bottom - ((m_CircleDataArray.m_pCircleData[i].m_otherData.dValue-m_leftaxis.minrange)/m_leftaxis.m_dValuePrPixel));
		

			otherPoint.x = m_plotRect.left   > otherPoint.x ? m_plotRect.left  : otherPoint.x;
			otherPoint.x = m_plotRect.right  < otherPoint.x ? m_plotRect.right : otherPoint.x;
			otherPoint.y = m_plotRect.top    >= otherPoint.y ? m_plotRect.top - 10   : otherPoint.y;
			otherPoint.y = m_plotRect.bottom < otherPoint.y ? m_plotRect.bottom+1: otherPoint.y;


			if ((p.x - 20) >=  m_plotRect.left && (p.x - 20) <= m_plotRect.right )
			{
				CPen disPen1(PS_SOLID,1,RGB(0,0,255));
				old = dc->SelectObject(&disPen1);
				dc->MoveTo(p.x - 10 , p.y);
				dc->LineTo(p.x - 10,otherPoint.y);
				dc->MoveTo(p.x , p.y);
				dc->LineTo(p.x - 20, p.y);
				dc->MoveTo(p.x , otherPoint.y);
				dc->LineTo(p.x - 20,otherPoint.y);
			
				CPen disPen2(PS_DASH,1,RGB(0,0,255));
				dc->SelectObject(&disPen2);
				dc->MoveTo(otherPoint.x , otherPoint.y);
				dc->LineTo(p.x - 10,otherPoint.y);
				
				dc->SelectObject(old);
			}
			
		}
		
	}
	
}

//*******************************************************************************************************/
// 把波形线以外的地方用背景色盖掉
//*******************************************************************************************************/
void clPlot::DrawWhite(CDC * dc)
{
	// 去掉网格上部多余的图形
	CRect rectTemp(m_ctlRect);
	rectTemp.bottom = m_plotRect.top;
	CBrush brushctlBkColor(m_ctlBkColor);
	dc->FillRect(rectTemp,&brushctlBkColor);

	// 去掉网格下部多余的图形
	rectTemp = m_ctlRect;
	rectTemp.top = m_plotRect.bottom + 1;
	dc->FillRect(rectTemp,&brushctlBkColor);

	// 刻度复原
	DrawWYScale(dc);
}

//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::DrawCaption(CDC * dc)
{
	CString strCaption;		
	// 取样数据，caption显示
	if (m_iIndex < m_allSerialNum)
	{
			strCaption.Format(_T("取样:%d"),m_iIndex+1);			
	}
	// 平均取样结果，caption 显示
	else if(m_allSerialNum != 0 && m_iIndex == m_allSerialNum)
	{
			strCaption.Format(_T("最终结果"));
	}			
	dc->DrawText(strCaption,CRect(m_ctlRect.left,m_ctlRect.top,m_ctlRect.left+100, m_ctlRect.top+20),DT_LEFT|DT_TOP);

}


//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::DrawSerie(CDC *dc,int s)
{
	BOOL bMore=TRUE;
	BOOL bDraw;
	CPoint p;
	int ly;

	if (m_series[s].m_lNoValues < 12)
	{
		return;
	}

	// lets get some serie parameters now and save the time of indexing during the loop
	long y = m_series[s].m_lbegin ;  y = 8;                            // 前八位是数据包头不予画出
	long m = m_series[s].m_lend;     m = m_series[s].m_lNoValues -4;   // 后四位是校验位不予画出
	long a = m_series[s].m_lNoValues;
	BOOL bRightAxis = m_series[s].m_bRightAxisAlign;
	CPen pen(m_series[s].m_iLineStyle, 1, m_series[s].m_color);
	CPen *old = dc->SelectObject(&pen);
	while(bMore){
		bDraw=FALSE;
		bMore=FALSE;
		ly=0;
		while(y != m && !bDraw)
		{
               
//				Scaling. We do scaling inline to save some time
				time_t valuetime = m_series[s].m_pvalues[y].ValueTime.GetTime();
				p.x = (int)(m_plotRect.left + ((valuetime-m_timeaxis.m_mintime.GetTime())/m_timeaxis.m_dSecondsPrPixel));
				p.x = (int)(m_plotRect.left + ((valuetime-m_timeaxis.m_mintime.GetTime())/m_timeaxis.m_dSecondsPrPixel));

				if(bRightAxis){
					p.y = (int)(m_plotRect.bottom - ((m_series[s].m_pvalues[y].dValue-m_rightaxis.minrange)/m_rightaxis.m_dValuePrPixel));
				}else{
					p.y = (int)(m_plotRect.bottom - ((m_series[s].m_pvalues[y].dValue-m_leftaxis.minrange)/m_leftaxis.m_dValuePrPixel));
				}

				
				if((ly == 0 || p.x != pLineArray[ly].x || p.y != pLineArray[ly].y)
					&& (p.x >= m_plotRect.left && p.x <= m_plotRect.right) )
				{
					
					 // 在门限范围之内才取得
						BOOL isAdd = TRUE;
						if (ly != 0  && pLineArray[ly-1].y < p.y)
						{
							if (fabs(pLineArray[ly-1].y - p.y)*m_leftaxis.m_dValuePrPixel > m_iLimitNum)
							{
								isAdd = FALSE;
							}
						}
						isAdd = TRUE;
						if (isAdd)
						{
							pLineArray[ly].x = p.x ;
							pLineArray[ly].y = p.y;
							ly++;

						}
				}

			y++;
			if(y > a)	// wrap list index ?
				y=0;
		}
		if(ly > 0){
			dc->Polyline(pLineArray, ly);	 // 画图形	
		}
	}
	dc->SelectObject(old);
}

//*******************************************************************************************************/
// 画X轴的刻度数字
//*******************************************************************************************************/
void clPlot::DrawWYScale(CDC * dc)
{
	long lMin = m_timeaxis.m_mintime.GetTime();
	long lMax = m_timeaxis.m_maxtime.GetTime();
	long yGrid = lMin;
    double prPixel = m_timeaxis.m_dSecondsPrPixel;
	long delta = (long)(10*m_extendSizeX);
	
	// todo: delta switch
	long diff = (yGrid)%((long)delta);
	yGrid = yGrid - diff;
	CPen *old, pen(PS_SOLID, 1, m_gridColor); 

	old = dc->SelectObject(&pen);

	yGrid = (int)yGrid;
	while(yGrid <= lMax)
	{
		int x = (int)(m_plotRect.left + ((yGrid-lMin)/prPixel));

		if(yGrid > lMin && yGrid < lMax)
		{

		   char b[100];
		   sprintf(b, "%d", yGrid);
		   dc->DrawText(b, CRect(x-m_TextHeight*3,m_plotRect.bottom,x+m_TextHeight/2,m_plotRect.bottom+m_TextHeight), DT_RIGHT|DT_BOTTOM);

		}
		yGrid += delta;
	}
	dc->SelectObject(old);
}

//*******************************************************************************************************/
//* Function:		clPlot::AddPoint
//*******************************************************************************************************/
BOOL clPlot::AddPoint(int serie, CTime &valuetime, double &value)
{
	if(m_series[serie].m_lNoValues < m_lMaxDataPrSerie)
	{
		valuetime = CTime(0)  + CTimeSpan(m_series[serie].m_lNoValues);
		m_series[serie].AddPoint(valuetime, value);
		
		/*
		if(m_bAutoScrollX *&& valuetime > m_timeaxis.m_maxtime*)
		{
			time_t span = m_timeaxis.m_maxtime.GetTime() - m_timeaxis.m_mintime.GetTime();
			time_t mintime = valuetime.GetTime() - span;

			SetBXRange(CTime(mintime), valuetime);
		}
		return TRUE;
		*/
	}
	return FALSE;
}
//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::SetBXRange(CTime &fromtime, CTime &totime, BOOL bMove)
{
	m_timeaxis.m_mintime = fromtime;
	m_timeaxis.m_maxtime = totime;
	if(!bMove){
		double wide = (double)m_plotRect.Height();
		m_timeaxis.m_dSecondsPrPixel = ((double)(m_timeaxis.m_maxtime.GetTime() - m_timeaxis.m_mintime.GetTime()+1)) / (double)m_plotRect.Width();
	}
}
//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::SetBYRange(double fromNum, double toNum, BOOL bMove)
{
	m_leftaxis.minrange = fromNum;
	m_leftaxis.maxrange = toNum;
	if(!bMove){
		double wide = (double)m_plotRect.Width();
		m_leftaxis.m_dValuePrPixel = (m_leftaxis.maxrange - m_leftaxis.minrange + 1) / (double)m_plotRect.Height();
	}
}

//*******************************************************************************************************/
//*******************************************************************************************************/
// move to left
void clPlot::LeftMoveX(int serie,int index)
{
	SetBXRange(m_timeaxis.m_mintime - CTimeSpan(index), m_timeaxis.m_maxtime - CTimeSpan(index));	
	Invalidate(FALSE);
}

// move to right
void clPlot::RightMoveX(int serie,int index)
{
	SetBXRange(m_timeaxis.m_mintime + CTimeSpan(index), m_timeaxis.m_maxtime + CTimeSpan(index));
	Invalidate(FALSE);
}

//*******************************************************************************************************/
//*******************************************************************************************************/
// move to left
void clPlot::DownMoveY(int serie,int index)
{
	SetBYRange(m_leftaxis.minrange - index, m_leftaxis.maxrange - index);	
	Invalidate(FALSE);
}

// move to right
void clPlot::UpMoveY(int serie,int index)
{
	SetBYRange(m_leftaxis.minrange + index, m_leftaxis.maxrange + index);	
	Invalidate(FALSE);
}
//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::SerieExtendX(double extendSizeX)
{
    m_extendSizeX = extendSizeX;
	SetBXRange(m_timeaxis.m_mintime , m_timeaxis.m_mintime + CTimeSpan(X_GRID_NUM * extendSizeX),FALSE);
	Invalidate(FALSE);
}

void clPlot::SerieExtendY(double extendSizeY)
{
    m_extendSizeY = extendSizeY;
	SetBYRange(m_leftaxis.minrange , m_leftaxis.minrange + (Y_GRID_NUM * extendSizeY),FALSE);
	Invalidate(FALSE);
}

//*******************************************************************************************************/
//
//*******************************************************************************************************/
//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::SerieExtendXOnePoint(CTime &extendPoint,int extendSize)
{
	int oldExtend = m_extendSizeX;
	double distance = (double)(extendPoint.GetTime() - m_timeaxis.m_mintime.GetTime()) / m_timeaxis.m_dSecondsPrPixel;
	SerieExtendX(extendSize);

	long nowExtend = distance * m_timeaxis.m_dSecondsPrPixel;
	CTime nowMinTime  = extendPoint - CTimeSpan(nowExtend);
	m_timeaxis.m_mintime = nowMinTime;
	m_timeaxis.m_maxtime = m_timeaxis.m_mintime + CTimeSpan(X_GRID_NUM * m_extendSizeX);
	Invalidate(FALSE);
}

void clPlot::SerieExtendYOnePoint(double &extendPoint,int extendSize)
{
	int oldExtend = m_extendSizeY;
	double distance = (double)(extendPoint - m_leftaxis.minrange) / m_leftaxis.m_dValuePrPixel;
	SerieExtendY(extendSize);

	long nowExtend = distance * m_leftaxis.m_dValuePrPixel;
	double nowMinY  = extendPoint - nowExtend ;
	m_leftaxis.minrange = nowMinY;
	m_leftaxis.maxrange = m_leftaxis.minrange + (Y_GRID_NUM * m_extendSizeY);

	//Invalidate(FALSE);
}

void clPlot::SerieMoveDependAPoint(value &pt)
{
	if (pt.ValueTime < m_timeaxis.m_mintime || pt.ValueTime > m_timeaxis.m_maxtime)
	{
		m_timeaxis.m_mintime = pt.ValueTime - CTimeSpan((m_timeaxis.m_maxtime.GetTime() - m_timeaxis.m_mintime.GetTime())/2);
		m_timeaxis.m_maxtime = m_timeaxis.m_mintime + CTimeSpan(X_GRID_NUM * m_extendSizeX);
	}
	if (pt.dValue < m_leftaxis.minrange || pt.dValue > m_leftaxis.maxrange)
	{
		m_leftaxis.minrange = pt.dValue - ((m_leftaxis.maxrange - m_leftaxis.minrange)/2);
		m_leftaxis.maxrange = m_leftaxis.minrange + (Y_GRID_NUM * m_extendSizeY);
	}	
	
}
//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::SetSerie(int s, int style, COLORREF color, double minrange, double maxrange, const char *szTitle, BOOL Rightalign)
{
	m_series[s].m_bIAmInUse = TRUE;
	m_series[s].m_color = color;
	m_series[s].m_iLineStyle = style;
	m_series[s].m_bRightAxisAlign = Rightalign;
}

//*******************************************************************************************************/
//*******************************************************************************************************/
void clPlot::Reset()
{
	m_iIndex = 0;
	m_allSerialNum = 0;
	for (int i=0; i < MAXSERIES; i++)
	{
		m_series[i].Reset();
	}

	m_CircleDataArray.Reset();
}

//*******************************************************************************************************/
//*******************************************************************************************************/
BOOL clPlot::SetIndexSerieNum(int index)
{
	if (m_allSerialNum < index)
	{
		AfxMessageBox(_T("当前波形越界,设置失败!"));
		return FALSE;
	}
	m_iIndex  = index;
	return TRUE;
}

//*******************************************************************************************************/
//*******************************************************************************************************/
BOOL clPlot::SetSerieNum(int iNum)
{
	if (MAXSERIES <= iNum)
	{
		AfxMessageBox(_T("波形个数超出有效值,设置失败!"));
		return FALSE;
	}
	m_allSerialNum = iNum;
	return TRUE;
}

void clPlot::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_moveState.endPoint = point;
	m_isBtnDown = FALSE;
//	SetCursor(AfxGetApp()->LoadCursor(IDC_CUR_MoveHand));
}

void clPlot::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_isBtnDown = TRUE;	
	m_moveState.startPoint = point;
	m_moveState.startMinX = m_timeaxis.m_mintime;
	m_moveState.StartMinY = m_leftaxis.minrange;
//	SetCursor(AfxGetApp()->LoadCursor(IDC_CUR_CatchHand));
}

void clPlot::OnMouseMove(UINT nFlags, CPoint point) 
{
		BOOL isInGrid = point.x >= m_plotRect.left && point.x <= m_plotRect.right && point.y >= m_plotRect.top && point.y <= m_plotRect.bottom;
		if (!isInGrid)
		{
			m_isBtnDown = FALSE;
		}
		if (m_isBtnDown)
		{
		    m_moveState.endPoint = point;	

			int moveDostanceX  = ((m_moveState.endPoint.x - m_moveState.startPoint.x) * m_timeaxis.m_dSecondsPrPixel);
			int moveDostanceY  = (m_moveState.endPoint.y - m_moveState.startPoint.y) * m_leftaxis.m_dValuePrPixel;
			ReSetCoorMaxMin();
		
			CTime nowTimeHead  = m_timeaxis.m_mintime - CTimeSpan(moveDostanceX);
			CTime nowTimeEnd   = m_timeaxis.m_maxtime - CTimeSpan(moveDostanceX); 
			if (nowTimeHead < MinX)		  
				moveDostanceX = moveDostanceX - (MinX.GetTime() - nowTimeHead.GetTime());		
			if (nowTimeEnd > MaxX)		
				moveDostanceX = moveDostanceX + (nowTimeEnd.GetTime() - MaxX.GetTime());
			CTime minTime = m_moveState.startMinX - CTimeSpan(moveDostanceX);
			SetBXRange(minTime , minTime +CTimeSpan(X_GRID_NUM * m_extendSizeX));
	
			double nowYUp   = m_leftaxis.minrange + moveDostanceY;
			double nowYDown = m_leftaxis.maxrange + moveDostanceY; 
			if (nowYUp < MinY)		
				moveDostanceY = moveDostanceY + (MinY - nowYUp);
			if (nowYDown > MaxY)
				moveDostanceY = moveDostanceY - (nowYDown - MaxY);	
			double minY = m_moveState.StartMinY + moveDostanceY;
			SetBYRange(minY, minY + (Y_GRID_NUM *m_extendSizeY));			
			
			Invalidate(FALSE);
			GetParent()->SendMessage(WM_MOUSEMOVE);		
		}		
		
}

void clPlot::OnLButtonDblClk(UINT nFlags, CPoint point) 
{	
	m_dClickPoint = point;
	GetParent()->SendMessage(WM_LBUTTONDBLCLK);	
	CWnd::OnLButtonDblClk(nFlags, point);
}

BOOL clPlot::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (m_isBtnDown)
	{		
	//	SetCursor(AfxGetApp()->LoadCursor(IDC_CUR_CatchHand));		
		return TRUE;
	}

	GetFocus();
//	SetCursor(AfxGetApp()->LoadCursor(IDC_CUR_MoveHand));
	return TRUE;
}

int clPlot::GetStaffANumber(value &dataA)
{
	// 游标在图形外
	if (m_staff.m_posA<m_plotRect.left || m_staff.m_posA>m_plotRect.right )
	{
		return 1;
	}

	// 没有加载图形
	CTime timeTemp = m_timeaxis.m_mintime + fabs(m_staff.m_posA - m_plotRect.left) * m_timeaxis.m_dSecondsPrPixel;
	if ( 0 == m_series[m_allSerialNum].m_lNoValues)
	{
		dataA.ValueTime = timeTemp;
		return 2;
	}


	long indexA = timeTemp.GetTime();
	if (indexA < 0 || indexA > m_series[m_allSerialNum].m_lNoValues)
	{
		dataA.ValueTime = timeTemp;
		return 3;
	}

	dataA.ValueTime = m_series[m_allSerialNum].m_pvalues[indexA].ValueTime;
	dataA.dValue = m_series[m_allSerialNum].m_pvalues[indexA].dValue;
	return 0;
}

int clPlot::GetStaffBNumber(value &dataB)
{
		// 游标在图形外
	if (m_staff.m_posB<m_plotRect.left || m_staff.m_posB>m_plotRect.right )
	{
		return 1;
	}

	// 没有加载图形
	CTime timeTemp = m_timeaxis.m_mintime + fabs(m_staff.m_posB - m_plotRect.left) * m_timeaxis.m_dSecondsPrPixel;
	if ( 0 == m_series[m_allSerialNum].m_lNoValues)
	{
		dataB.ValueTime = timeTemp;
		return 2;
	}
	
	long indexB = timeTemp.GetTime();
	if (indexB < 0 || indexB > m_series[m_allSerialNum].m_lNoValues)
	{
		dataB.ValueTime = timeTemp;
		return 3;
	}

	dataB.ValueTime = m_series[m_allSerialNum].m_pvalues[indexB].ValueTime;
	dataB.dValue = m_series[m_allSerialNum].m_pvalues[indexB].dValue;
	return 0;
}

//*******************************************************************************************************/
//clPlot::CovertToValue
//*******************************************************************************************************/
BOOL clPlot::CovertToValue(CPoint &point, CircleData &val)
{
	if (point.x >= m_plotRect.left && point.x <= m_plotRect.right && point.y >= m_plotRect.top && point.y <= m_plotRect.bottom)
	{
		long distanceTime = fabs(point.x - m_plotRect.left) * m_timeaxis.m_dSecondsPrPixel;
		double distanceY = fabs(point.y - m_plotRect.bottom) * m_leftaxis.m_dValuePrPixel;
		val.m_Data.ValueTime = m_timeaxis.m_mintime +  CTimeSpan(distanceTime);
		val.m_Data.dValue    = m_leftaxis.minrange + distanceY;
		return TRUE;
	}
    return FALSE;
}

double clPlot::GetSerieYFromX(CTime time)
{
	double y = 0;
	int index = time.GetTime();
	if (index >= 0 && index <= m_series[m_allSerialNum].m_lNoValues)
	{
		y = m_series[m_allSerialNum].m_pvalues[index].dValue ;
	}
	return y;
}

void  clPlot::ReSetCoorMaxMin()
{
		MinX = CTime(0) - CTimeSpan(21 * m_extendSizeX);
		MaxX = CTime(10000) + CTimeSpan(21 * m_extendSizeX);
		if (m_series[m_allSerialNum].m_lNoValues != 0)
		{
			MaxX = CTime(m_series[m_allSerialNum].m_lNoValues) + CTimeSpan(20*m_extendSizeX);
		}
		MinY = 0 - 11 * m_extendSizeY;
		MaxY = 256 + 11 * m_extendSizeY;
}

BOOL clPlot::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//this->GetFocus();
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		this->PostMessage(WM_LBUTTONDBLCLK);
	}
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_PRIOR)
		{
			
		}
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL clPlot::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	UINT nClassStyle = CS_DBLCLKS;
    CString strClassName = AfxRegisterWndClass(nClassStyle, 0, 0, 0);
	return CWnd::Create(strClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void clPlot::OnRButtonDown(UINT nFlags, CPoint point) 
{
	OnLButtonDblClk(nFlags, point);	
}




