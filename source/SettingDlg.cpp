// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JWM1400CE.h"
#include "SettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg property page

int PulsWide[] = {1,2,4,8,16,32,64};
int Wave[] = {1,2,3,4,5};
int Length[] = {1,2,4,8,16};
int BMultipleT[] = {4,3,2,1};
int BMultipleA[] = {4,3,2,1};

CString WaveStr[] = {_T("850 nm"), _T("1300 nm"), _T("1310 nm"), _T("1490 nm"), _T("1550 nm")};	
CString PulsWideStr[] = {"10 ns", " 20 ns", "40 ns", "80 ns", "160 ns nm", "320 ns", "640 ns"};
CString LengthStr[] = {"4 k", "8 k", "16 k", "32 k", "64 k"};
CString BMultipleTStr[] = {"1", " 2", "3", "4", "5"};
CString BMultipleAStr[] = {"1", " 2", "3", "4", "5"};
CString SwitchStr[] = {_T("开") , _T("关")};
CString ModeStr[] = {_T("单模"), _T("多模")};
GraphPara s_GraphPara[5];



IMPLEMENT_DYNCREATE(CSettingDlg, CPropertyPage)

CSettingDlg::CSettingDlg() : CPropertyPage(CSettingDlg::IDD)
{
	//{{AFX_DATA_INIT(CSettingDlg)
	m_iComNum = -1;
	m_iBitRate = -1;
	m_iVerifyLen = -1;
	m_iDataLen = -1;
	m_iStopLen = -1;
	m_iPulsWide = -1;
	m_iWave = -1;
	m_iLength = -1;
	m_iBMultipleA = -1;
	m_iBMultipleT = -1;
	m_iLDPower = 0;
	m_iAPD = 0;
	m_iAPDOpen = -1;
	m_iAPDMode = -1;
	m_editSendData = _T("");
	m_iSamplingTimes = 0;
	m_iReflect = 0;
	m_iAttenuation = 0;
	m_iEventTimes = 0;
	m_iSwing = 0;
	//}}AFX_DATA_INIT
	
   countxx = 0;
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingDlg)
	DDX_Control(pDX, IDC_List_ParaList, m_listParaList);
	DDX_CBIndex(pDX, IDC_CMB_ComNum, m_iComNum);
	DDX_CBIndex(pDX, IDC_CMB_BitRate, m_iBitRate);
	DDX_CBIndex(pDX, IDC_CMB_Verify, m_iVerifyLen);
	DDX_CBIndex(pDX, IDC_CMB_DataLen, m_iDataLen);
	DDX_CBIndex(pDX, IDC_CMB_StopLen, m_iStopLen);
	DDX_CBIndex(pDX, IDC_COMBO_PulsWide, m_iPulsWide);
	DDX_CBIndex(pDX, IDC_COMBO_Wave, m_iWave);
	DDX_CBIndex(pDX, IDC_COMBO_Length, m_iLength);
	DDX_CBIndex(pDX, IDC_COMBO_BMultipleA, m_iBMultipleA);
	DDX_CBIndex(pDX, IDC_COMBO_BMultipleT, m_iBMultipleT);
	DDX_Text(pDX, IDC_EDIT_LDPOWER, m_iLDPower);
	DDX_Text(pDX, IDC_EDIT_APD, m_iAPD);
	DDX_Radio(pDX, IDC_RAD_APDOpen, m_iAPDOpen);
	DDX_Radio(pDX, IDC_RAD_SINGLE, m_iAPDMode);
	DDX_Text(pDX, IDC_EDIT_SendData, m_editSendData);
	DDX_Text(pDX, IDC_EDIT_SAMPLING_TIMES, m_iSamplingTimes);
	DDV_MinMaxInt(pDX, m_iSamplingTimes, 1, 30);
	DDX_Text(pDX, IDC_EDIT_Reflect, m_iReflect);
	DDX_Text(pDX, IDC_EDIT_Attenuation, m_iAttenuation);
	DDX_Text(pDX, IDC_EDIT_EventTimes, m_iEventTimes);
	DDX_Text(pDX, IDC_EDIT_Swing, m_iSwing);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CSettingDlg)
	ON_MESSAGE(ON_COM_RECEIVE, OnComRevc)
	ON_CBN_SELCHANGE(IDC_CMB_ComNum, OnSelchangeCMBComNum)
	ON_CBN_SELCHANGE(IDC_CMB_BitRate, OnSelchangeCMBBitRate)
	ON_CBN_SELCHANGE(IDC_CMB_Verify, OnSelchangeCMBVerify)
	ON_CBN_SELCHANGE(IDC_CMB_DataLen, OnSelchangeCMBDataLen)
	ON_CBN_SELCHANGE(IDC_CMB_StopLen, OnSelchangeCMBStopLen)
	ON_BN_CLICKED(IDC_BTN_OpenPort, OnBTNOpenPort)
	ON_BN_CLICKED(IDC_BTN_WRITE, OnBtnWrite)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_NOTIFY(NM_DBLCLK, IDC_List_ParaList, OnDblclkListParaList)
	ON_BN_CLICKED(IDC_BTN_Start, OnBTNStart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

BOOL CSettingDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_packPro = CJWM1400CEApp::GetApp().GetAppCommuMgr()->GetPortPro();

	InitData();

	// init graphic parameter array;
	for (int i = 0; i < sizeof(s_GraphPara)/sizeof(s_GraphPara[0]); i++)
	{
		UpdateGraphParaArr(i);
	}

	InitGraphParaTable();

	m_startTime = 0;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

inline
void CSettingDlg::InitGraphParaTable()
{
	m_listParaList.SetExtendedStyle(LVS_EX_FULLROWSELECT|
		LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES);
	m_listParaList.InsertColumn(0,_T("") ,LVCFMT_CENTER,50);
	m_listParaList.InsertColumn(1,_T("波长") ,LVCFMT_CENTER,90);
	m_listParaList.InsertColumn(2,_T("脉冲宽度") ,LVCFMT_CENTER,90);
	m_listParaList.InsertColumn(3,_T("长度") ,LVCFMT_CENTER,90);
	m_listParaList.InsertColumn(4,_T("TIA量程") ,LVCFMT_CENTER,65);
	m_listParaList.InsertColumn(5,_T("AMP量程") ,LVCFMT_CENTER,65);
	m_listParaList.InsertColumn(6,_T("LD功率") , LVCFMT_CENTER,80);
	m_listParaList.InsertColumn(7,_T("APD偏压") ,LVCFMT_CENTER,60);
	m_listParaList.InsertColumn(8,_T("APD开关") ,LVCFMT_CENTER,65);
	m_listParaList.InsertColumn(9,_T("APD模式") ,LVCFMT_CENTER,65);

	for(unsigned int i = 0; i < sizeof(s_GraphPara)/sizeof(s_GraphPara[0]); i++)
	{
		int index = m_listParaList.GetItemCount();
		CString strTemp;
		strTemp.Format(_T("%d"), Wave[i]);
		m_listParaList.InsertItem(index,strTemp);
		UpdateGraphParaTable(index);
		m_listParaList.SetItemText(index,1,WaveStr[index]);
	}
}

void CSettingDlg::UpdateGraphParaTable(int index)
{
	CString strTemp;
	m_listParaList.SetItemText(index,1,WaveStr[s_GraphPara[index].Wave]);
	m_listParaList.SetItemText(index,2,PulsWideStr[s_GraphPara[index].PulsWid]);
	m_listParaList.SetItemText(index,3,LengthStr[s_GraphPara[index].Length]);
	m_listParaList.SetItemText(index,4,BMultipleTStr[s_GraphPara[index].BMultipleT]);
	m_listParaList.SetItemText(index,5,BMultipleAStr[s_GraphPara[index].BMultipleA]);

	strTemp.Format(_T("%d"), s_GraphPara[index].LDPower);
	m_listParaList.SetItemText(index,6,strTemp);
	strTemp.Format(_T("%d"), s_GraphPara[index].APD);
	m_listParaList.SetItemText(index,7,strTemp);
	m_listParaList.SetItemText(index,8,SwitchStr[s_GraphPara[index].APDOpen]);
	m_listParaList.SetItemText(index,9,ModeStr[s_GraphPara[index].APDMode]);

}

void  CSettingDlg::InitData()
{
	// 串口设置，初值设定
	m_iComNum = 1;
	m_iBitRate =1;
	m_iDataLen = 0;
	m_iVerifyLen = 0;
	m_iStopLen = 0;


	// 其他
    m_iPulsWide = 5;
	m_iWave = 0;
	m_iLength = 4;
	m_iBMultipleT = 0;
	m_iBMultipleA = 0;
	m_iLDPower = 0;
	m_iAPD = 0;

	// APD开关/模式
	m_iAPDMode = 0;
	m_iAPDOpen = 1;
	
   CCommuMrg* commMrg = CJWM1400CEApp::GetApp().GetAppCommuMgr();

	// init sample times
	m_iSamplingTimes = commMrg->miSamplingTimes;

	// Graphic limit 
	m_iSwing = commMrg->mGraphLimt.miSwing;
	m_iReflect = commMrg->mGraphLimt.miReflect;
	m_iAttenuation = commMrg->mGraphLimt.miAttenuation;
	m_iEventTimes = commMrg->mGraphLimt.miEventTimes ;

	GetDlgItem(IDC_EDIT_Reflect)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Attenuation)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_EventTimes)->EnableWindow(FALSE);
}

void CSettingDlg::OnSelchangeCMBComNum() 
{
	ComOpen();	
}

void CSettingDlg::OnSelchangeCMBBitRate() 
{
	ComOpen();
}

void CSettingDlg::OnSelchangeCMBVerify() 
{
	ComOpen();
}

void CSettingDlg::OnSelchangeCMBDataLen() 
{
	ComOpen();
}

void CSettingDlg::OnSelchangeCMBStopLen() 
{
	ComOpen();
}

LRESULT CSettingDlg::OnComRevc(WPARAM, LPARAM)
{
	CCommuMrg* commuMrg = CJWM1400CEApp::GetApp().GetAppCommuMgr();
	commuMrg->ReceiveData();
	countxx = m_packPro->m_buff.GetSize();
	SetDlgItemInt(IDC_STATIC_RNum , countxx);
	return TRUE;
}

// 从读取的包中取数据
void CSettingDlg::ShowResponseMsg()
{
	CString strMsg;
    WriteData(m_packPro->m_buff.GetData(),m_packPro->m_buff.GetSize(),FALSE);
	strMsg = m_packPro->GetResponseMsg(m_packPro->m_buff);	
	m_packPro->m_buff.RemoveAll();

	AfxMessageBox(strMsg);	
	Invalidate(FALSE);	
}


void CSettingDlg::OnBTNOpenPort() 
{
	CCommuMrg* commuMrg = CJWM1400CEApp::GetApp().GetAppCommuMgr();

	UpdateData(TRUE);
	// 串口关闭    
	if (!commuMrg->IsOpen())
	{
		// 打开成功
		if (ComOpen())
		{
			SetDlgItemText(IDC_BTN_OpenPort,_T("关闭串口"));
		}
	}
	// 串口被打开
	else
	{
		// 关闭串口
		commuMrg->ClosePort();
		SetDlgItemText(IDC_BTN_OpenPort,_T("打开串口"));
	}
}

BOOL CSettingDlg::ComOpen() 
{
	UpdateData();
	CCommuMrg* commuMrg = CJWM1400CEApp::GetApp().GetAppCommuMgr();
	if (commuMrg->IsOpen())
	{
	   commuMrg->ClosePort();	  
	}

	// 波特率
	int BitRate[] = {9600,115200,230400};
	// 校验位
	byte Paritys[] = {NOPARITY,EVENPARITY,ODDPARITY};
	// 停止位
	BYTE StopBits[] = {ONESTOPBIT,TWOSTOPBITS};
	BYTE ByteSizes[] = {8};
	
	TCHAR szBuf[1024];
	if (!commuMrg->OpenPort(m_iComNum+1, BitRate[m_iBitRate],Paritys[m_iVerifyLen],ByteSizes[m_iDataLen],StopBits[m_iStopLen]))
	{
		TCHAR buff[1024];
		wsprintf(buff,_T("last error %d"),GetLastError());
		AfxMessageBox(buff);
		return FALSE;
	}
    return TRUE;
	UpdateData(FALSE);
}

void CSettingDlg::OnBtnWrite() 
{
	UpdateData(TRUE);    
	//int ID = GetFocus()->GetDlgCtrlID();
	int ID = m_FocesID;
	CByteArray sendData;
//	m_packPro->GetPortPack(sendData,0,Wave[m_iWave]);

	switch(ID)
	{
	case IDC_COMBO_PulsWide:  // 脉冲宽度
		m_packPro->GetPortPack(sendData,2,PulsWide[m_iPulsWide]);
		break;
	case IDC_COMBO_Wave:      // 波长
		m_packPro->GetPortPack(sendData,34,Wave[m_iWave]);
		break;                    
	case IDC_COMBO_Length:    // 长度
		m_packPro->GetPortPack(sendData,1,Length[m_iLength]);
	    break;
	case IDC_COMBO_BMultipleT: // 放大倍数 TIA量程
		m_packPro->GetPortPack(sendData,3,BMultipleT[m_iBMultipleT]);
	    break;
	case IDC_COMBO_BMultipleA: // 放大倍数 AMP量程
		m_packPro->GetPortPack(sendData,4,BMultipleA[m_iBMultipleA]);
	    break;
	case IDC_EDIT_LDPOWER:    // LD功率
		{ 
			 switch(m_iWave)
			 {
			 case 0:
				 m_packPro->GetPortPack(sendData,5,m_iLDPower);
				 break;
			 case 1:
				 m_packPro->GetPortPack(sendData,10,m_iLDPower);
				 break;
		     case 2:
				 m_packPro->GetPortPack(sendData,15,m_iLDPower);
				 break;
		     case 3:
				 m_packPro->GetPortPack(sendData,20,m_iLDPower);
				 break;
			 case 4:
				 m_packPro->GetPortPack(sendData,25,m_iLDPower);
				 break;
			 }
		     break;
		}
	case IDC_EDIT_APD:        // APD偏压
		m_packPro->GetPortPack(sendData,30,m_iAPD);
		break;
	case IDC_RAD_APDOpen:   // APD偏压开
		m_packPro->GetPortPack(sendData,33,m_iAPDOpen);
		break;
	case IDC_RAD_APDClose:   // APD偏压关
		m_packPro->GetPortPack(sendData,33,m_iAPDOpen);

		break;
	case IDC_RAD_SINGLE:      // APD模式- 单模
		m_packPro->GetPortPack(sendData,32,m_iAPDMode);
		break;
	case IDC_RAD_MUL:         // APD模式- 多模
		m_packPro->GetPortPack(sendData,32,m_iAPDMode);
		break;

	default:
	    break;
	}
	
	// 保存数据
	SaveAllData();

	// 更新
	UpdateGraphParaArr(m_iWave);
	UpdateGraphParaTable(m_iWave);

	// 界面数据显示
	WriteData(sendData.GetData(),sendData.GetSize(),TRUE);
	
	// 串口发数据
	CCommuMrg* commuMrg = CJWM1400CEApp::GetApp().GetAppCommuMgr();
	commuMrg->SendData(sendData.GetData(),sendData.GetSize()); //发送十六进制数据
	m_packPro->Init();
}

// 写数据到到界面上
void CSettingDlg::WriteData(byte* buffer,int size,BOOL isSend)
{
	UpdateData(TRUE);
	CString strTemp;
	// 写发送数据
	if (isSend)
	{
		m_editSendData = "";

		for (int i=0; i < size; i++)
		{
			 strTemp.Format(_T("%02X"),buffer[i]);
			 m_editSendData += strTemp;
			 m_editSendData += " ";			 
		}
		
		CString strSize;
		strSize.Format(_T("%d"),size);
        GetDlgItem(IDC_STATIC_TNum)->SetWindowText(strSize);
	}
	else
	{
		CString strSize;
		strSize.Format(_T("%d"),size);
        GetDlgItem(IDC_STATIC_RNum)->SetWindowText(strSize);
	}
	
	UpdateData(FALSE);	
}


void CSettingDlg::OnBtnClear() 
{	
	countxx = 0 ;
	m_packPro->m_buff.RemoveAll();
	SetDlgItemInt(IDC_STATIC_RNum,countxx);

	m_editSendData = "";
	GetDlgItem(IDC_STATIC_TNum)->SetWindowText(_T("0"));
	GetDlgItem(IDC_STATIC_RNum)->SetWindowText(_T("0"));
	UpdateData(FALSE);
}

BOOL CSettingDlg::OnKillActive() 
{	
	// save sample times
	//SaveAllData();

	return CPropertyPage::OnKillActive();
}

BOOL CSettingDlg::OnSetActive() 
{
	CJWM1400CEApp::GetApp().GetAppCommuMgr()->GetCnComm()->SetWnd(this->m_hWnd);
	return CPropertyPage::OnSetActive();
}

void CSettingDlg::SaveAllData()
{
	UpdateData();
	CCommuMrg* commMrg = CJWM1400CEApp::GetApp().GetAppCommuMgr();
	commMrg->SetSampleTimes(m_iSamplingTimes);
	commMrg->mWave = Wave[m_iWave];
	commMrg->mGraphPara.PulsWid = PulsWide[m_iPulsWide];
	commMrg->mGraphPara.Wave = Wave[m_iWave];
	commMrg->mGraphPara.Length = Length[m_iLength];
	commMrg->mGraphPara.BMultipleT = BMultipleT[m_iBMultipleT];
	commMrg->mGraphPara.BMultipleA = BMultipleA[m_iBMultipleA];
	commMrg->mGraphPara.APD = m_iAPD;
	commMrg->mGraphPara.APDOpen = m_iAPDOpen;
	commMrg->mGraphPara.APDMode = m_iAPDMode;
	commMrg->mGraphPara.LDPower = m_iLDPower;

	commMrg->mGraphLimt.miSwing = m_iSwing;
	commMrg->mGraphLimt.miReflect = m_iReflect;
	commMrg->mGraphLimt.miAttenuation = m_iAttenuation;
	commMrg->mGraphLimt.miEventTimes = m_iEventTimes;
}

void CSettingDlg::UpdateGraphParaArr(int index)
{
	if (index < 0 || index > sizeof(s_GraphPara)/sizeof(s_GraphPara[0]))
	{
		return;
	}
		s_GraphPara[index].Wave = m_iWave;
		s_GraphPara[index].PulsWid = m_iPulsWide;
		s_GraphPara[index].Length = m_iLength;
		s_GraphPara[index].BMultipleT = m_iBMultipleT;
		s_GraphPara[index].BMultipleA = m_iBMultipleA;
		s_GraphPara[index].LDPower = m_iLDPower;
		s_GraphPara[index].APD = m_iAPD;
		s_GraphPara[index].APDOpen = m_iAPDOpen;
		s_GraphPara[index].APDMode = m_iAPDMode;
}

void CSettingDlg::OnDblclkListParaList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listParaList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		int item = m_listParaList.GetNextSelectedItem(pos);
		m_iWave = s_GraphPara[item].Wave;
		m_iPulsWide = s_GraphPara[item].PulsWid;
		m_iLength = s_GraphPara[item].Length;
		m_iBMultipleT = s_GraphPara[item].BMultipleT;
		m_iBMultipleA = s_GraphPara[item].BMultipleA;
		m_iLDPower = s_GraphPara[item].LDPower;
		m_iAPD = s_GraphPara[item].APD;
		m_iAPDOpen = s_GraphPara[item].APDOpen;
		m_iAPDMode = s_GraphPara[item].APDMode;
		UpdateData(FALSE);
	}
	*pResult = 0;
}

BOOL CSettingDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->hwnd == GetDlgItem(IDC_COMBO_PulsWide)->m_hWnd)
	{
		m_FocesID = GetFocus()->GetDlgCtrlID(); goto result;	 
	}
  
 	if(pMsg->hwnd == GetDlgItem(IDC_COMBO_Wave)->m_hWnd)
	{
		m_FocesID = GetFocus()->GetDlgCtrlID();     goto result;
	}

	if(pMsg->hwnd == GetDlgItem(IDC_COMBO_Length)->m_hWnd)
	{
		m_FocesID = GetFocus()->GetDlgCtrlID();   goto result;	
	}

	if(pMsg->hwnd == GetDlgItem(IDC_COMBO_BMultipleT)->m_hWnd)
	{
		m_FocesID = GetFocus()->GetDlgCtrlID();goto result;
	}

	if(pMsg->hwnd == GetDlgItem(IDC_COMBO_BMultipleA)->m_hWnd)
	{
		m_FocesID = GetFocus()->GetDlgCtrlID();goto result;
	}

	if(pMsg->hwnd == GetDlgItem(IDC_EDIT_LDPOWER)->m_hWnd)
	{
		m_FocesID = GetFocus()->GetDlgCtrlID();    goto result;
	}
	if(pMsg->hwnd == GetDlgItem(IDC_EDIT_APD)->m_hWnd)
	{
		m_FocesID = GetFocus()->GetDlgCtrlID();		 goto result;
	}

	if(pMsg->hwnd == GetDlgItem(IDC_RAD_SINGLE)->m_hWnd)
	{
		m_FocesID = IDC_RAD_SINGLE;      goto result;
	}

	if(pMsg->hwnd == GetDlgItem(IDC_RAD_MUL)->m_hWnd)
	{
		m_FocesID = IDC_RAD_MUL;      goto result;
	}

	if(pMsg->hwnd == GetDlgItem(IDC_RAD_APDOpen)->m_hWnd)
	{
		m_FocesID = IDC_RAD_APDOpen;	    goto result;
	}

	if(pMsg->hwnd == GetDlgItem(IDC_RAD_APDClose)->m_hWnd)
	{
		m_FocesID = IDC_RAD_APDOpen;     goto result;
	}

	
result: 	return CPropertyPage::PreTranslateMessage(pMsg);
}


BOOL CSettingDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::PreCreateWindow(cs);
}




void CSettingDlg::OnBTNStart() 
{
	if (m_packPro->IsAPack())
	{
		ShowResponseMsg();
	}
	
}
