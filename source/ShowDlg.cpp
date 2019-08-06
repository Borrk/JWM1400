// ShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JWM1400CE.h"
#include "ShowDlg.h"
#include "DataFile.h"
#include "WaveAnaylse.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  EXTEND_X 50
#define  EXTEND_Y 15

/////////////////////////////////////////////////////////////////////////////
// CShowDlg property page

IMPLEMENT_DYNCREATE(CShowDlg, CPropertyPage)

CShowDlg::CShowDlg() : CPropertyPage(CShowDlg::IDD)
{
	//{{AFX_DATA_INIT(CShowDlg)
	m_iTimeOut = 0;
	m_strXModulus = _T("");
	m_iExtendSizeX = 0;
	m_iExtendSizeY = 0;
	m_iStaff = -1;
	m_iZoom = -1;
	//}}AFX_DATA_INIT
    countxx = 0;
}

CShowDlg::~CShowDlg()
{
}

void CShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShowDlg)
	DDX_Control(pDX, IDC_SLIDER_Staff, m_scliderStaff);
	DDX_Control(pDX, IDC_LIST_DataAnaylseTable, m_listAnaylseTable);
	DDX_Control(pDX, IDC_STATIC_PLOT, m_Plot);
	DDX_Text(pDX, IDC_EDIT_TimeOut, m_iTimeOut);
	DDX_Text(pDX, IDC_EDIT_XModulus, m_strXModulus);
	DDX_Text(pDX, IDC_EDIT_EXTENDSIZEX, m_iExtendSizeX);
	DDX_Text(pDX, IDC_EDIT_EXTENDSIZEY, m_iExtendSizeY);
	DDX_Radio(pDX, IDC_RADIO_StaffA, m_iStaff);
	DDX_Radio(pDX, IDC_RADIO_XUp, m_iZoom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShowDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CShowDlg)
	ON_MESSAGE(ON_COM_RECEIVE, OnComRevc)
	ON_BN_CLICKED(IDC_BTN_Setting, OnBTNSetting)
	ON_BN_CLICKED(IDC_BTN_Start, OnBTNStart)
	ON_BN_CLICKED(IDC_BTN_DataAnaylse, OnBTNDataAnaylse)
	ON_EN_CHANGE(IDC_EDIT_EXTENDSIZEX, OnChangeEditExtendsizex)
	ON_EN_CHANGE(IDC_EDIT_EXTENDSIZEY, OnChangeEditExtendsizey)
	ON_BN_CLICKED(IDC_BTN_AllClear, OnBTNAllClear)
	ON_BN_CLICKED(IDC_BTN_OpenFile, OnBTNOpenFile)
	ON_BN_CLICKED(IDC_BTN_SaveData, OnBTNSaveData)
	ON_BN_CLICKED(IDC_RADIO_StaffA, OnRADIOStaffA)
	ON_BN_CLICKED(IDC_RADIO_StaffB, OnRADIOStaffB)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Staff, OnCustomdrawSLIDERStaff)
	ON_BN_CLICKED(IDC_CHECK_AnalyseCircle, OnCHECKAnalyseCircle)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DataAnaylseTable, OnDblclkLISTDataAnaylseTable)
	ON_BN_CLICKED(IDC_RADIO_XUp, OnRADIOXUp)
	ON_BN_CLICKED(IDC_RADIO_XDown, OnRADIOXDown)
	ON_BN_CLICKED(IDC_RADIO_YUp, OnRADIOYUp)
	ON_BN_CLICKED(IDC_RADIO_YDown, OnRADIOYDown)
	ON_BN_CLICKED(IDC_RADIO_UP, OnRadioUp)
	ON_BN_CLICKED(IDC_RADIO_DOWN, OnRadioDown)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShowDlg message handlers

BOOL CShowDlg::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

BOOL CShowDlg::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	CJWM1400CEApp::GetApp().GetAppCommuMgr()->GetCnComm()->SetWnd(this->m_hWnd);
	
	m_iSampleTimes = CJWM1400CEApp::GetApp().GetAppCommuMgr()->GetSampleTimes();
    m_Wave = CJWM1400CEApp::GetApp().GetAppCommuMgr()->mWave;
	m_GraphLimt = CJWM1400CEApp::GetApp().GetAppCommuMgr()->mGraphLimt;

	return CPropertyPage::OnSetActive();
}

BOOL CShowDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_packPro = CJWM1400CEApp::GetApp().GetAppCommuMgr()->GetPortPro();
	InitData();

	m_Plot.InitData();
	for (int i =0 ; i < MAXSERIES; i++)
	{
		m_Plot.SetSerie(i, PS_SOLID, RGB(255,0,0), 0.0, 2000, "Pressure");		
	}

	InitExtend();			// ��ʼ���������Ų���
	InitDataAnaylseTable(); // 
	InitAnaylseData();
	initStaff();
	

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShowDlg::InitData()
{
	// ��ʼ�����Ŵ���
	m_iExtendSizeX = EXTEND_X;
	m_iExtendSizeY = EXTEND_Y;
	m_Plot.SerieExtendX(m_iExtendSizeX);
	m_Plot.SerieExtendY(m_iExtendSizeY);


	// ��ʼ����ǰ��ʾ����
	m_iIndexSerival = 0;	
}

inline void CShowDlg::InitDataAnaylseTable()
{
	m_listAnaylseTable.SetExtendedStyle(LVS_EX_FULLROWSELECT|
		LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES);
	m_listAnaylseTable.InsertColumn(0,_T("�¼�����") ,LVCFMT_CENTER,80);
	m_listAnaylseTable.InsertColumn(1,_T("λ��(X-������ʱ)") ,LVCFMT_CENTER,150);
	m_listAnaylseTable.InsertColumn(2,_T("λ��(m)") ,LVCFMT_CENTER,150);
	m_listAnaylseTable.InsertColumn(3,_T("����(ADC)") ,LVCFMT_CENTER,150);
	m_listAnaylseTable.InsertColumn(4,_T("����(dB)") ,LVCFMT_CENTER,150);
}

void CShowDlg::InitAnaylseData()
{
	m_strXModulus = "2";
	m_iTimeOut = 32;
}

void CShowDlg::initStaff()
{
	m_iStaff = 0;
	CRect Rect;
	GetDlgItem(IDC_STATIC_PLOT)->GetWindowRect(&Rect); 
	ScreenToClient(&Rect);

	m_scliderStaff.SetRange(Rect.left,Rect.Width());	
}

void CShowDlg::InitExtend()
{
	m_iExtendSizeX = EXTEND_X;
	m_iExtendSizeY = EXTEND_Y;
	m_Plot.SerieExtendX(m_iExtendSizeX);
	m_Plot.SerieExtendY(m_iExtendSizeY);

	m_iZoom = 0;
}

void CShowDlg::OnBTNSetting() 
{
	if (m_packPro->IsAPack())
	{
		ProPackData();
		AfxMessageBox(_T("is a pack"));
	}
		
}

// LRESULT CShowDlg::OnComRevc(WPARAM, LPARAM)
// {
// 	do {
// 		const int size = 512;
// 	    unsigned char buffer[size+1];
// 
// 		int len = m_comm->Read(buffer, size);//127
// 		buffer[len] = '\0';
// 		
// 		// ������ܵ���������
// 		if (m_packPro->OnComm(buffer,len))
// 		{
// 			// �õ�����������ڵ�����
// 			int command = 1; m_packPro->ProPackKind();	
// 			if (1 == command)
// 			{
// 				// �������ݵ�ͼ����
// 				ProPackData();
// 
// 				// ����ɼ�����
// 				if (m_iIndexSerival < m_iSampleTimes)
// 				{
// 					m_iIndexSerival++;
// 					m_Plot.SetIndexSerieNum(m_iIndexSerival);
// 				
// 					// ���ɼ������ݣ�����������ʾ
// 					if (m_iIndexSerival == m_iSampleTimes)
// 					{					
// 					//	SetEndSamplingState();					
// 						return TRUE;
// 					}
// 					else 
// 					{
// 						// ÿ�β��ϵط���������ɼ�����
// 						SetSamplingNumberCommand();
// 					}
// 				}
// 		
// 			}
// 		}
// 		
// 	} while( m_comm->IsRxBufferMode() && m_comm->Input().SafeSize());
// 		
// 
// 	return TRUE;
// }


LRESULT CShowDlg::OnComRevc(WPARAM, LPARAM)
{
	CCommuMrg* commuMrg = CJWM1400CEApp::GetApp().GetAppCommuMgr();
	commuMrg->ReceiveData();
	countxx = m_packPro->m_buff.GetSize();
    SetDlgItemInt(IDC_LAB_CoorInfo,countxx);

	return TRUE;
}

void CShowDlg::ProPackData()
{
	AddPlotPointAndShow(m_packPro->m_buff,1);
	m_packPro->m_buff.RemoveAll();
}

// ���ݶ�����������ʾ����
void CShowDlg::AddPlotPointAndShow(CByteArray& tempArry,int style)
{

	if (tempArry.GetSize() < 10)
	{
		AfxMessageBox(_T("�õ��˴������ݰ�!"));
		return ;
	}
	switch(style)
	{
	case 1:   // 1���ֽ�һ������
		{
			for (int i = 8; i<tempArry.GetSize(); i++)
			{		
				double y = tempArry[i];
				m_Plot.AddPoint(m_iIndexSerival,CTime(0),y);
			}
			break;		
		}
	
	case 2:  // 2���ֽ�һ������
		{
			for (int i = 6; i<tempArry.GetSize()-4; i+=2)
			{		
				double y = tempArry[i] << 8;
				y += tempArry[i+1];
				m_Plot.AddPoint(m_iIndexSerival,CTime(0),y);
			}
			break;		

		}	    
	default:
	    break;
	}

	m_Plot.SetRealSerial2();
	m_Plot.Invalidate(FALSE);	
}

void CShowDlg::OnBTNStart() 
{
	CCommuMrg* commuMrg = CJWM1400CEApp::GetApp().GetAppCommuMgr();
	if (!commuMrg->IsOpen())
		AfxMessageBox(_T("�������!"));


	m_Plot.Reset();
	m_Plot.Invalidate(FALSE);
	

	if (m_iSampleTimes > 30)
	{
		AfxMessageBox(_T("ȡ������Ӧ <= 30 !,����������!"));
		return ;
	}

	if (!m_Plot.SetSerieNum(m_iSampleTimes))
	{
		return;
	}

	m_iIndexSerival = 0;
	if (!m_Plot.SetIndexSerieNum(m_iIndexSerival))
	{
		return;
	}
	
	SetSamplingNumberCommand();
	
}

// ���Ͳɼ�����ָ��
void CShowDlg::SetSamplingNumberCommand() 
{
	m_packPro->m_packData.RemoveAll();
	m_packPro->m_TempData.RemoveAll();

	UpdateData(TRUE);
	// ������������
	CByteArray sendData;

	m_packPro->GetPortPack(sendData,0,m_Wave);

	// ������
	CCommuMrg* commuMrg = CJWM1400CEApp::GetApp().GetAppCommuMgr();
	commuMrg->SendData(sendData.GetData(),sendData.GetSize()); //����ʮ���������� 
	m_packPro->Init();
}

void CShowDlg::OnBTNDataAnaylse() 
{
	DataAnaylse();

	// ˢ���α�����
	StaffNumber();
	
}
//////////////////////////////////////////////////////////////////////////
// DataAnaylse [��������]
//////////////////////////////////////////////////////////////////////////
void CShowDlg::DataAnaylse()
{
	WaveAnaylse anaylse;
	int index = m_Plot.m_allSerialNum;
	serie* mySerie = &m_Plot.m_series[index];

	UpdateData(TRUE);
	anaylse.ProAnaylseWave(mySerie,m_GraphLimt.miSwing,EventTimes); 
	m_Plot.m_CircleDataArray.Reset();
	
	if (anaylse.m_iNodeNum < 2)
	{
		return;
	}
	CString strPosition = "";
	CString strValue = "";

	m_listAnaylseTable.DeleteAllItems();

	for (int i=1; i < anaylse.m_iNodeNum-1; i++)
	{
		Node nodeTemp = anaylse.m_pNode[i];
		if (nodeTemp.left.y > nodeTemp.right.y && (nodeTemp.left.y - nodeTemp.right.y) > Attenuation) //˥��
		{
			if (nodeTemp.cusp.y > nodeTemp.left.y && (nodeTemp.cusp.y - nodeTemp.left.y) > Reflect)
			{
				value valeTemp;
				valeTemp.dValue = nodeTemp.left.y;
				valeTemp.ValueTime = CTime(0)  + CTimeSpan(nodeTemp.left.x);

				value otherData;
				otherData.dValue = nodeTemp.cusp.y;
				otherData.ValueTime = CTime(0) + CTimeSpan(nodeTemp.cusp.x);
				m_Plot.AddCircleData(valeTemp,RGB(255,255,0),1,otherData);  //�� -����


				valeTemp.dValue = nodeTemp.right.y;
				valeTemp.ValueTime = CTime(0)  + CTimeSpan(nodeTemp.right.x);
			
				otherData.dValue = nodeTemp.left.y;
				otherData.ValueTime = CTime(0) + CTimeSpan(nodeTemp.left.x);
				m_Plot.AddCircleData(valeTemp,RGB(0,255,0),0,otherData);    //�� - ˥��
		
			} 
			else
			{	
				value valeTemp;
				valeTemp.dValue = nodeTemp.right.y;
				valeTemp.ValueTime = CTime(0)  + CTimeSpan(nodeTemp.right.x);

				value otherData;
				otherData.dValue = nodeTemp.left.y;
				otherData.ValueTime = CTime(0) + CTimeSpan(nodeTemp.left.x);
				m_Plot.AddCircleData(valeTemp,RGB(0,255,0),0,otherData);    //�� - ˥��
			}
		}
		else 
		{	
			value valeTemp;
			valeTemp.dValue = nodeTemp.left.y;
			valeTemp.ValueTime = CTime(0)  + CTimeSpan(nodeTemp.left.x);

			value otherData;
			otherData.dValue = nodeTemp.cusp.y;
			otherData.ValueTime = CTime(0) + CTimeSpan(nodeTemp.cusp.x);
			m_Plot.AddCircleData(valeTemp,RGB(255,255,0),1,otherData);  //�� -����	
		}
	}

	UpdateData(TRUE);
	// show data int anaylse table 
	double iModulus = atof((char*)(LPCTSTR)m_strXModulus);
	for (int j=0; j<m_Plot.m_CircleDataArray.GetLength(); j++)
	{
		CircleData data = m_Plot.m_CircleDataArray.m_pCircleData[j];
		switch(data.type)
		{
		case 0:  // ˥��
			{
				long   x = data.m_Data.ValueTime.GetTime();
				double ADCY =  data.m_Data.dValue - data.m_otherData.dValue;
				double dBY  = ADC2dB( data.m_Data.dValue , data.m_otherData.dValue);
				insertAnaylseTable(_T("˥��"),x - m_iTimeOut, (double)(x - m_iTimeOut) * iModulus, ADCY,dBY);  // ��ʾ�ڽ����ϵ����� -32
			}
			break;
		case 1:  // ����
			{
				long   x = data.m_Data.ValueTime.GetTime();
				double ADCY = data.m_otherData.dValue - data.m_Data.dValue;
				double dBY  = ADC2dB( data.m_otherData.dValue , data.m_Data.dValue);
				insertAnaylseTable(_T("����"),x - m_iTimeOut, (double)(x - m_iTimeOut) * iModulus, ADCY,dBY); // ��ʾ�ڽ����ϵ����� -32
			}
		    break;
		default:
		    break;
		}

	}
	UpdateData(FALSE);
    m_Plot.Invalidate(FALSE);
	AfxMessageBox(_T("�������!"));	
	
}

inline void CShowDlg::insertAnaylseTable(CString strCaption, long position1, double position2,double para1,double para2)
{
	int index = m_listAnaylseTable.GetItemCount();
	m_listAnaylseTable.InsertItem(index,strCaption);
	CString str;
	str.Format(_T("%ld"), position1);
	m_listAnaylseTable.SetItemText(index,1,str);
	str.Format(_T("%lf"), position2);
	m_listAnaylseTable.SetItemText(index,2,str);
	str.Format(_T("%lf"),para1);
	m_listAnaylseTable.SetItemText(index,3,str);
	str.Format(_T("%lf"),para2);
	m_listAnaylseTable.SetItemText(index,4,str);
}

void CShowDlg::OnChangeEditExtendsizex() 
{
	int oldExtendSize = m_iExtendSizeX;
	UpdateData(TRUE);	
	if (m_iExtendSizeX > EXTEND_MAX || m_iExtendSizeX < 1)
	{
		AfxMessageBox(_T("����Ӧ��<= 10000 && >= 1"));		
		m_iExtendSizeX = oldExtendSize;		
	}
	
	m_Plot.SerieExtendX(m_iExtendSizeX);
	UpdateData(FALSE);	
	
}

void CShowDlg::OnChangeEditExtendsizey() 
{
	int oldExtendSize = m_iExtendSizeY;
	UpdateData(TRUE);
	if (m_iExtendSizeY > EXTEND_MAX || m_iExtendSizeY < 1)
	{
		AfxMessageBox(_T("����Ӧ��<= 10000 && >0"));		
		m_iExtendSizeY = oldExtendSize;		
	}
	
	m_Plot.SerieExtendY(m_iExtendSizeY);
	UpdateData(FALSE);	
	
}

void CShowDlg::OnBTNAllClear() 
{
	m_Plot.m_iIndex = MAXSERIES +1;
	m_Plot.m_series[m_Plot.m_allSerialNum].Reset();
    m_Plot.m_CircleDataArray.Reset();
	

	m_listAnaylseTable.DeleteAllItems();
	
	//  ˢ���α���ʾ����
	StaffNumber();

	m_Plot.Invalidate(FALSE);
	UpdateData(FALSE);	
	
	countxx = 0;
	m_packPro->m_buff.RemoveAll();
	SetDlgItemInt(IDC_LAB_CoorInfo,countxx );
	
}

void CShowDlg::OnBTNOpenFile() 
{
	CFileDialog fileDlg(TRUE,NULL, NULL, OFN_READONLY| OFN_OVERWRITEPROMPT, _T("Jwm (*.Jwm)|*.Jwm|"),NULL);	

	CString fileName;
	if (fileDlg.DoModal() == IDOK)
	{
		fileName = fileDlg.GetPathName();
		//ReadDataFromFile(fileName);
		
		if (ReadGraph(fileName))
		{
			// ��շ�������
			m_listAnaylseTable.DeleteAllItems();
			// ˢ���α���ʾ����
			//StaffNumber();
		}	
	}	
}

BOOL CShowDlg::ReadGraph(CString fileName)
{
	DataFile dGraph;
	if (!dGraph.Open(fileName,CFile::modeRead))
	{
		AfxMessageBox(_T("��ͼ��ʧ��!"));
		return FALSE;
	}
	GraphPara para;
	if (!dGraph.ReadPara(para))           // ��ȡͼ�β���
	{
		AfxMessageBox(_T("������Ч��.Jwmͼ��,����ʧ��!"));
		return FALSE;
	}
	// ���ز�����
//	ShowGraphPara(para);
	CJWM1400CEApp::GetApp().GetAppCommuMgr()->mGraphPara = para;

	long size = dGraph.GetDataGraphLength();
	unsigned char *buffer = new unsigned char[size];
	if (!dGraph.ReadData(buffer,size))   // ��ȡͼ������
	{
		AfxMessageBox(_T("ͼ�μ���ʧ��!"));
		return FALSE;
	}

	// ����ͼ��
	// ��ʼ״̬   
	m_Plot.SetIndexSerieNum(0);
	m_Plot.SetSerieNum(0);
	m_Plot.Reset();
	m_Plot.m_CircleDataArray.Reset();

	m_Plot.m_allSerialNum = 0;
	for (int i=0; i< size; i++)
	{		
		double y = buffer[i];
		m_Plot.AddPoint(0,CTime(0),y);
	}	
	delete[] buffer;
	dGraph.Close();
	m_Plot.Invalidate(FALSE);
	return TRUE;
}


void CShowDlg::OnBTNSaveData() 
{
	CFileDialog fileDlg(FALSE,NULL, NULL, OFN_READONLY| OFN_OVERWRITEPROMPT, _T( "Jwm (*.Jwm)|*.Jwm|"),NULL);	


	CString fileName;
	if (fileDlg.DoModal() == IDOK)
	{
		fileName = fileDlg.GetPathName();
		fileName += _T(".Jwm");
	
		WriteGraph(fileName);
	}	
	
}

void CShowDlg::WriteGraph(CString fileName)
{
	DataFile dGraph;

	if (!dGraph.Open(fileName,CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox(_T("ͼ��ʧ������!"));
		return ;
	}

	// ����ͼ�β���
	UpdateData(TRUE);
	GraphPara para = CJWM1400CEApp::GetApp().GetAppCommuMgr()->mGraphPara;

	dGraph.WritePara(para);

	// ����ͼ��
	int len = m_Plot.m_series[m_Plot.m_allSerialNum].m_lNoValues;
	unsigned char* buffer = new unsigned char[len];	
	for (int i=0 ;i < len; i++)
	{
		int numTemp = (int)m_Plot.m_series[m_Plot.m_allSerialNum].m_pvalues[i].dValue;
		buffer[i] = numTemp;
	}
	dGraph.WriteData(buffer,len);
	
	delete[] buffer;
	dGraph.Close();
	AfxMessageBox(_T("���ݱ���ɹ�"));
}

void CShowDlg::StaffNumber()
{
		value valA,valB;
		valA.ValueTime = CTime(0);
		valA.dValue = 0;

		valB.ValueTime = CTime(0);
		valB.dValue = 0;
		double ax1 = 0,ay1 = 0,bx1 = 0,by1 = 0;
		double ax2 = 0,ay2 = 0,bx2 = 0,by2 = 0;

		int resultA = m_Plot.GetStaffANumber(valA);
		switch(resultA)
		{
		case 0:
			ax1 = valA.ValueTime.GetTime();
			ay1 = valA.dValue;
			ax2 = ConverToRealX(valA.ValueTime);
			ay2 = ConverToRealY(valA.dValue);
			break;
		case 1:
			ax1 = 0;  ay1 = 0;
			ax2 = 0;  ay2 = 0;
			break;
		case 2:
			ax1 = valA.ValueTime.GetTime();
		    ay1 = 0;
			ax2 = ConverToRealX(valA.ValueTime);
			ay2 = 0;
			break;
		case 3:
			ax1 = valA.ValueTime.GetTime();
		    ay1 = 0;
			ax2 = ConverToRealX(valA.ValueTime);
			ay2 = 0;
		    break;
		}
	
		int resultB = m_Plot.GetStaffBNumber(valB);
		switch(resultB)
		{
		case 0:
			bx1 = valB.ValueTime.GetTime();
			by1 = valB.dValue;
			bx2 = ConverToRealX(valB.ValueTime);
			by2 = ConverToRealY(valB.dValue);
			break;
		case 1:
			bx1 = 0;  by1 = 0;
			bx2 = 0;  by2 = 0;
			break;
		case 2:
			bx1 = valB.ValueTime.GetTime();
			by1 = 0;
			bx2 = ConverToRealX(valB.ValueTime);
			by2 = 0;
		    break;
		case 3:
			bx1 = valB.ValueTime.GetTime();
			by1 = 0;
			bx2 = ConverToRealX(valB.ValueTime);
			by2 = 0;
		    break;
		}

		ShownStaffNumber(ax1,ay1,ax2,ay2,bx1,by1,bx2,by2);

}

void CShowDlg::ShownStaffNumber(double ax1, double ay1, double ax2,double  ay2,double bx1, double by1,double bx2,double by2)
{
	/*
	CString strTemp;
	strTemp.Format(_T("%.03lf"),ax1);
	SetDlgItemText(IDC_STATIC_XStaffANum,strTemp);
	strTemp.Format(_T("%.03lf"),ay1);
	SetDlgItemText(IDC_STATIC_YStaffANum,strTemp);

	strTemp.Format(_T("%.03lf"),ax2);
	SetDlgItemText(IDC_STATIC_XStaffANumM,strTemp);
	strTemp.Format(_T("%.03lf"),ay2);
	SetDlgItemText(IDC_STATIC_YStaffANumM,strTemp);

	strTemp.Format(_T("%.03lf"),bx1);
	SetDlgItemText(IDC_STATIC_XStaffBNum,strTemp);
	strTemp.Format(_T("%.03lf"),by1);
	SetDlgItemText(IDC_STATIC_YStaffBNum,strTemp);

	strTemp.Format(_T("%.03lf"),bx2);
	SetDlgItemText(IDC_STATIC_XStaffBNumM,strTemp);
	strTemp.Format(_T("%.03lf"),by2);
	SetDlgItemText(IDC_STATIC_YStaffBNumM,strTemp);


	if (ax1 != 0 && ay1 != 0 && bx1 != 0 && by1 != 0)
	{
		strTemp.Format(_T("%.03lf"),(double)fabs(bx1 - ax1));
		SetDlgItemText(IDC_STATIC_DistanceXNumX,strTemp);
		strTemp.Format(_T("%.03lf "),(double)fabs(by1 - ay1));
		SetDlgItemText(IDC_STATIC_DistanceYNumY,strTemp);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_DistanceXNumX,_T("0"));
		SetDlgItemText(IDC_STATIC_DistanceYNumY,_T("0"));
	}
	
	if (ax2 != 0 && ay2 != 0 && bx2 != 0 && by2 != 0)
	{
		strTemp.Format(_T("%.03lf"),(double)fabs(bx2 - ax2));
		SetDlgItemText(IDC_STATIC_DistanceXNum,strTemp);
		strTemp.Format(_T("%.03lf"),(double)fabs(by2 - ay2));
		SetDlgItemText(IDC_STATIC_DistanceYNum,strTemp);
	}
	else
	{
		SetDlgItemText(IDC_STATIC_DistanceXNum,_T("0"));
		SetDlgItemText(IDC_STATIC_DistanceYNum,_T("0"));
	}
	*/
}


void CShowDlg::OnRADIOStaffA() 
{
	UpdateData(TRUE);
	m_scliderStaff.SetPos(m_Plot.GetStaffA());
	UpdateData(FALSE);	
}

void CShowDlg::OnRADIOStaffB() 
{
	UpdateData(TRUE);
	m_scliderStaff.SetPos(m_Plot.GetStaffB());
	UpdateData(FALSE);		
}

void CShowDlg::OnCustomdrawSLIDERStaff(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	long pos = m_scliderStaff.GetPos();
	switch(m_iStaff)
	{
	case 0:
		m_Plot.SetStaffA(pos);
		break;
	case 1:
		m_Plot.SetStaffB(pos);
	    break;
	}
	m_Plot.Invalidate(FALSE);
	StaffNumber();
	*pResult = 0;
}

void CShowDlg::OnCHECKAnalyseCircle() 
{
	if (((CButton*)GetDlgItem(IDC_CHECK_AnalyseCircle))->GetCheck() == 1 )
	{
		m_Plot.SetDataCircle(TRUE);
		m_Plot.Invalidate(FALSE);
	}
	else
	{
		m_Plot.SetDataCircle(FALSE);
		m_Plot.Invalidate(FALSE);
	}	
	
}

void CShowDlg::OnDblclkLISTDataAnaylseTable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	CString buffer;
	POSITION pos  = m_listAnaylseTable.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int item = m_listAnaylseTable.GetNextSelectedItem(pos);
	
		buffer = m_listAnaylseTable.GetItemText(item,1);	
		
		char byteStr[50];
		WideCharToMultiByte(CP_OEMCP,NULL,buffer.GetBuffer(0),-1,byteStr,50,NULL,FALSE);
		long timeX = atol((char*)(LPCTSTR)byteStr);
		buffer.ReleaseBuffer(0);
	
		value ptVal;
		ptVal.ValueTime = CTime(0)+ CTimeSpan(timeX + m_iTimeOut);
		m_Plot.SetActiveCircleDataCoor(ptVal.ValueTime);  // ��ʾ�ڽ����ϵ����� -32 ��������Ҫ����
	
		ptVal.dValue = m_Plot.GetSerieYFromX(ptVal.ValueTime);
		m_Plot.SerieMoveDependAPoint(ptVal);
		m_Plot.Invalidate(FALSE);	 
	}
	*pResult = 0;
}

void CShowDlg::OnRADIOXUp() 
{
	CircleData data;
	if (m_Plot.GetActiveCircleData(data))
	{
		XChange(true,data);	
	}
   
   m_iZoom = 0;
   UpdateData(FALSE);
	
}

void CShowDlg::OnRADIOXDown() 
{
	CircleData data;
	if (m_Plot.GetActiveCircleData(data))
	{
		XChange(false,data);	
	}
	
	m_iZoom = 1;
   UpdateData(FALSE);
	
}

void CShowDlg::OnRADIOYUp() 
{
	CircleData data;
	if (m_Plot.GetActiveCircleData(data))
	{
		YChange(true,data);	
	}
	
	m_iZoom = 2;
   UpdateData(FALSE);
}

void CShowDlg::OnRADIOYDown() 
{
	CircleData data;
	if (m_Plot.GetActiveCircleData(data))
	{
		YChange(false,data);	
	}
	 
   m_iZoom = 3;
   UpdateData(FALSE);
	
}

void CShowDlg::OnRadioUp() 
{
	PlotWholeExtend2(true);	
	m_iZoom = 4;
   UpdateData(FALSE);
	
}

void CShowDlg::OnRadioDown() 
{
	PlotWholeExtend2(false);
    m_iZoom = 5;
   UpdateData(FALSE);		
}

void CShowDlg::YChange(bool isUp,CircleData &data)
{
	int oldExtendSize = m_iExtendSizeY;
	if (isUp)
	{
		m_iExtendSizeY = m_iExtendSizeY / 2;
	}
	else
	{
		m_iExtendSizeY = m_iExtendSizeY * 2;
	}
	
	if (m_iExtendSizeY > EXTEND_MAX || m_iExtendSizeY < 1)
	{
		m_iExtendSizeY = oldExtendSize;		
	}

	double y = m_Plot.m_leftaxis.minrange;

	y = data.m_Data.dValue;
	
	m_Plot.SerieExtendYOnePoint(y,m_iExtendSizeY);
	

	UpdateData(FALSE);
	m_Plot.Invalidate(FALSE);

}
void CShowDlg::XChange(bool isUp,CircleData &data)
{
	int oldExtendSize = m_iExtendSizeX;
	if (isUp)
	{
		m_iExtendSizeX = m_iExtendSizeX / 2;
	} 
	else
	{
		m_iExtendSizeX = m_iExtendSizeX * 2;
	}

	if (m_iExtendSizeX > EXTEND_MAX || m_iExtendSizeX < 1)
	{
		m_iExtendSizeX = oldExtendSize;		
	}

	CTime time = m_Plot.m_timeaxis.m_mintime;

	time = data.m_Data.ValueTime;

	
	m_Plot.SerieExtendXOnePoint(time,m_iExtendSizeX);

	UpdateData(FALSE);
	m_Plot.Invalidate(FALSE);

}

void CShowDlg::PlotWholeExtend2(bool isUp)
{
	if (isUp)
	{
		OnRADIOXUp();
		OnRADIOYUp();
	}
	else
	{
		OnRADIOXDown();
		OnRADIOYDown();
	}
}

BOOL CShowDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
		{
			switch(pMsg->wParam)
			{
			case VK_PRIOR:		
				PlotWholeExtend2(true);	
				break;
			case VK_NEXT:
				PlotWholeExtend2(false);
				break;
			default:
				break;
			}
			
		}	
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CShowDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	UpdateData(TRUE);

	CircleData val;	
	if (m_Plot.CovertToValue(m_Plot.m_dClickPoint,val))
	{
		switch(m_iZoom)
		{
		case 0:  // X - Up
			XChange(true,val);
			break;
		case 1:  // X - down
			XChange(false,val);
			break;
		case 2:  // Y - Up
			YChange(true,val);
		    break;
		case 3:  // Y - down
			YChange(false,val);
		    break;
		case 4:  // up
			XChange(true,val);
			YChange(true,val);
			break;
		case 5:  // down
			XChange(false,val);
			YChange(false,val);
			break;
		
		}
	}
	
	//  ˢ���α���ʾ����
	StaffNumber();
	
	CPropertyPage::OnLButtonDblClk(nFlags, point);
}

void CShowDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_Plot.m_isBtnDown)
	{
		// ˢ���α���ʾ����	
		StaffNumber();
	}
	
	CPropertyPage::OnMouseMove(nFlags, point);
}



double CShowDlg::ADC2dB(double adc1, double adc2)
{
	adc1 = 10 * log(adc1);
	adc2 = 10 * log(adc2);
	double result = adc1 - adc2;
	return result;
}

inline double CShowDlg::ConverToRealX(CTime valX)
{
	UpdateData(TRUE);
	double modulus = atof((char*)(LPCTSTR)m_strXModulus);
	double x = (valX.GetTime() - m_iTimeOut) * modulus;
	return x;
	
}

inline double CShowDlg::ConverToRealY(double valY)
{	
	double y = 10 * log(valY); 
	return y;
}

void CShowDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	OnLButtonDblClk(nFlags, point);	
}

BOOL CShowDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
