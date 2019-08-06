// JWM1400CEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JWM1400CE.h"
#include "JWM1400CEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define  EXTEND_X 50
#define  EXTEND_Y 15

int m_PulsWide[] = {1,2,4,8,16,32,64};
int m_Wave[] = {1,2,3,4,5};
int m_Length[] = {1,2,4,8,16};
int m_BMultipleT[] = {1,2,3,4};
int m_BMultipleA[] = {1,2,3,4};	

/////////////////////////////////////////////////////////////////////////////
// CJWM1400CEDlg dialog

CJWM1400CEDlg::CJWM1400CEDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJWM1400CEDlg::IDD, pParent)
	:m_com(CnComm::EN_THREAD | CnComm::EN_TX_BUFFER | CnComm::EN_TX_THREAD | CnComm::EN_RX_BUFFER)
{
	//{{AFX_DATA_INIT(CJWM1400CEDlg)
	m_iBitRate = -1;
	m_iComNum = -1;
	m_iDataLen = -1;
	m_iStopLen = -1;
	m_iVerifyLen = -1;
	m_iBMultipleA = -1;
	m_iBMultipleT = -1;
	m_iLength = -1;
	m_iPulsWide = -1;
	m_iWave = -1;
	m_iAPD = 0;
	m_iLDPower = 0;
	m_editSendData = _T("");
	m_iAPDOpen = -1;
	m_iAPDMode = -1;
	m_iSamplingTimes = 0;
	m_iLimit = 0;
	m_iExtendSizeX = 0;
	m_iExtendSizeY = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJWM1400CEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJWM1400CEDlg)
	DDX_Control(pDX, IDC_SLIDERY, m_CSliCtrY);
	DDX_Control(pDX, IDC_SLIDERX, m_CSliCtrX);
	DDX_Control(pDX, IDC_STATIC_PLOT, m_Plot);
	DDX_CBIndex(pDX, IDC_CMB_BitRate, m_iBitRate);
	DDX_CBIndex(pDX, IDC_CMB_ComNum, m_iComNum);
	DDX_CBIndex(pDX, IDC_CMB_DataLen, m_iDataLen);
	DDX_CBIndex(pDX, IDC_CMB_StopLen, m_iStopLen);
	DDX_CBIndex(pDX, IDC_CMB_Verify, m_iVerifyLen);
	DDX_CBIndex(pDX, IDC_COMBO_BMultipleA, m_iBMultipleA);
	DDX_CBIndex(pDX, IDC_COMBO_BMultipleT, m_iBMultipleT);
	DDX_CBIndex(pDX, IDC_COMBO_Length, m_iLength);
	DDX_CBIndex(pDX, IDC_COMBO_PulsWide, m_iPulsWide);
	DDX_CBIndex(pDX, IDC_COMBO_Wave, m_iWave);
	DDX_Text(pDX, IDC_EDIT_APD, m_iAPD);
	DDX_Text(pDX, IDC_EDIT_LDPOWER, m_iLDPower);
	DDX_Text(pDX, IDC_EDIT_SendData, m_editSendData);
	DDX_Radio(pDX, IDC_RAD_APDOpen, m_iAPDOpen);
	DDX_Radio(pDX, IDC_RAD_SINGLE, m_iAPDMode);
	DDX_Text(pDX, IDC_EDIT_SAMPLING_TIMES, m_iSamplingTimes);
	DDV_MinMaxInt(pDX, m_iSamplingTimes, 0, 30);
	DDX_Text(pDX, IDC_EDIT_LIMIT, m_iLimit);
	DDX_Text(pDX, IDC_EDIT_EXTENDSIZEX, m_iExtendSizeX);
	DDX_Text(pDX, IDC_EDIT_EXTENDSIZEY, m_iExtendSizeY);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJWM1400CEDlg, CDialog)
	//{{AFX_MSG_MAP(CJWM1400CEDlg)
	ON_MESSAGE(ON_COM_RECEIVE, OnComRevc)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERX, OnCustomdrawSliderx)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERY, OnCustomdrawSlidery)
	ON_BN_CLICKED(IDC_BTN_OpenPort, OnBTNOpenPort)
	ON_BN_CLICKED(IDC_BTN_WRITE, OnBtnWrite)
	ON_BN_CLICKED(IDC_BTN_Start, OnBTNStart)
	ON_EN_CHANGE(IDC_EDIT_EXTENDSIZEX, OnChangeEditExtendsizex)
	ON_EN_CHANGE(IDC_EDIT_EXTENDSIZEY, OnChangeEditExtendsizey)
	ON_EN_CHANGE(IDC_EDIT_LIMIT, OnChangeEditLimit)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_CBN_SELCHANGE(IDC_CMB_BitRate, OnSelchangeCMBBitRate)
	ON_CBN_SELCHANGE(IDC_CMB_ComNum, OnSelchangeCMBComNum)
	ON_CBN_SELCHANGE(IDC_CMB_DataLen, OnSelchangeCMBDataLen)
	ON_CBN_SELCHANGE(IDC_CMB_StopLen, OnSelchangeCMBStopLen)
	ON_CBN_SELCHANGE(IDC_CMB_Verify, OnSelchangeCMBVerify)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJWM1400CEDlg message handlers

BOOL CJWM1400CEDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// TODO: Add extra initialization here
	InitData();	

	m_Plot.InitData();
	for (int i =0 ; i < MAXSERIES; i++)
	{
		m_Plot.SetSerie(i, PS_SOLID, RGB(255,0,0), 0.0, 2000, "Pressure");		
	}
	
	ComSetState();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void  CJWM1400CEDlg::InitData()
{
	// ��ʼ������ؼ�
	m_CSliCtrX.SetRange(0,100000);
	m_csliOldXIndex = 0;

	m_CSliCtrY.SetRange(0,5000);//250
	m_csliOldYIndex = 0;

	// �������ã���ֵ�趨
	m_iComNum = 1;
	m_iBitRate =1;
	m_iDataLen = 0;
	m_iVerifyLen = 0;
	m_iStopLen = 0;
	m_comm.SetNotifyNum(1);
	m_comm.SetWnd(this->m_hWnd);

	// ����
    m_iPulsWide = 5;
	m_iWave = 0;
	m_iLength = 4;
	m_iBMultipleT = 0;
	m_iBMultipleA = 0;
	m_iLDPower = 0;
	m_iAPD = 0;

	// APD����/ģʽ
	m_iAPDMode = 0;
	m_iAPDOpen = 1;

	// ��ʼ�����Ŵ���
	m_iExtendSizeX = EXTEND_X;
	m_iExtendSizeY = EXTEND_Y;
	m_Plot.SerieExtendX(m_iExtendSizeX);
	m_Plot.SerieExtendY(m_iExtendSizeY);


	// ��ʼ����ǰ��ʾ����
	m_iIndexSerival = 0;
	m_iSamplingTimes = 1;
	m_iLimit = 20;
	UpdateData(FALSE);
}



void CJWM1400CEDlg::OnCustomdrawSliderx(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	int index = m_CSliCtrX.GetPos();
	
	if(index >= m_csliOldXIndex)
	{
		// ����
		m_Plot.RightMoveX(0,fabs(index-m_csliOldXIndex));//*m_Plot.m_extendSizeX);
	}
	else
	{
		// ����
		m_Plot.LeftMoveX(0,fabs(index-m_csliOldXIndex));//*m_Plot.m_extendSizeX);
	}
    m_csliOldXIndex = index;
	
	
	*pResult = 0;
}

void CJWM1400CEDlg::OnCustomdrawSlidery(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	int index = m_CSliCtrY.GetPos();
	if(index >= m_csliOldYIndex)
	{
		m_Plot.UpMoveY(0,fabs(index-m_csliOldYIndex));
	}
	else
	{
		m_Plot.DownMoveY(0,fabs(index-m_csliOldYIndex));
	}
    m_csliOldYIndex = index;
	
	*pResult = 0;
}

void CJWM1400CEDlg::ComSetState() 
{
	if (m_comm.IsOpen())
	{
	   m_comm.Close();	  
	}

	if (!m_comm.Open(m_iComNum + 1))
	{
		    TCHAR buff[1024];
			wsprintf(buff,_T("last error %d"),GetLastError());
			AfxMessageBox(buff);
			return;
	}	

	// ������
	int BitRate[] = {9600,115200,230400};
	// У��λ
	byte Paritys[] = {NOPARITY,EVENPARITY,ODDPARITY};
	// ֹͣλ
	BYTE StopBits[] = {ONESTOPBIT,TWOSTOPBITS};
	BYTE ByteSizes[] = {8};
	
	TCHAR szBuf[1024];
	if (!m_comm.SetState(BitRate[m_iBitRate],Paritys[m_iVerifyLen],ByteSizes[m_iDataLen],StopBits[m_iStopLen]))
	{
		wsprintf(szBuf, _T("SetState(%d, %d, %d, %d), Fail. Code:%d"), 
			BitRate[m_iBitRate], 
			Paritys[m_iVerifyLen], 
			ByteSizes[m_iDataLen], 
			StopBits[m_iStopLen],
			GetLastError());
		AfxMessageBox(szBuf);
		return;
	}
}

LRESULT CJWM1400CEDlg::OnComRevc(WPARAM, LPARAM)
{
	do {

		const int size = 1024;
	    unsigned char buffer[size];
		unsigned char hex_buffer[size*3];
		TCHAR tbuffer[size*3];

		
		int len = m_comm.Read(buffer, size);//127
		buffer[len] = '\0';
		
		// ������ܵ���������
		if (m_packPro.OnComm(buffer,len))
		{
			// �õ�����������ڵ�����
			int command = GetDataFromPack();	
		
			// ���������ݰ�
			if (command == 1)
			{
				// ����ɼ�����
				if (m_iIndexSerival < m_iSamplingTimes)
				{
					m_iIndexSerival++;
					m_Plot.SetIndexSerieNum(m_iIndexSerival);
				
					// ���ɼ������ݣ�����������ʾ
					if (m_iIndexSerival == m_iSamplingTimes)
					{					
						SetEndSamplingState();					
						return TRUE;
					}
					else 
					{
						// ÿ�β��ϵط���������ɼ�����
						SetSamplingNumberCommand();
					}
						
				}
			}
		}
	
	} while(m_comm.IsRxBufferMode() &&  m_comm.Input().SafeSize());
		
	return TRUE;
}
void CJWM1400CEDlg::OnBTNOpenPort() 
{
	UpdateData(TRUE);
	// ���ڹر�
	if (!m_comm.IsOpen())
	{
		// ��ʧ��
		if (!m_comm.Open(m_iComNum + 1))
		{
				TCHAR buff[1024];
				wsprintf(buff,_T("last error %d"),GetLastError());
				AfxMessageBox(buff);
				SetDlgItemText(IDC_BTN_OpenPort,_T("�򿪴���"));
				return;
		}
		// �򿪳ɹ�
		ComSetState();
		SetDlgItemText(IDC_BTN_OpenPort,_T("�رմ���"));
	}
	// ���ڱ���
	else
	{
		// �رմ���
		m_comm.Close();
		SetDlgItemText(IDC_BTN_OpenPort,_T("�򿪴���"));
	}
}

void CJWM1400CEDlg::OnBtnWrite() 
{
	UpdateData(TRUE);

	int ID = GetFocus()->GetDlgCtrlID();
	CByteArray sendData;
	switch(ID)
	{
	case IDC_COMBO_PulsWide:  // ������
		m_packPro.GetPortPack(sendData,2,m_PulsWide[m_iPulsWide]);
		break;
	case IDC_COMBO_Wave:      // ����
		m_packPro.GetPortPack(sendData,34,m_Wave[m_iWave]);
		break;                    
	case IDC_COMBO_Length:    // ����
		m_packPro.GetPortPack(sendData,1,m_Length[m_iLength]);
	    break;
	case IDC_COMBO_BMultipleT: // �Ŵ��� TIA����
		m_packPro.GetPortPack(sendData,3,m_BMultipleT[m_iBMultipleT]);
	    break;
	case IDC_COMBO_BMultipleA: // �Ŵ��� AMP����
		m_packPro.GetPortPack(sendData,4,m_BMultipleA[m_iBMultipleA]);
	    break;
	case IDC_EDIT_LDPOWER:    // LD����
		{ 
			 switch(m_iWave)
			 {
			 case 0:
				 m_packPro.GetPortPack(sendData,5,m_iLDPower);
				 break;
			 case 1:
				 m_packPro.GetPortPack(sendData,10,m_iLDPower);
				 break;
		     case 2:
				 m_packPro.GetPortPack(sendData,15,m_iLDPower);
				 break;
		     case 3:
				 m_packPro.GetPortPack(sendData,20,m_iLDPower);
				 break;
			 case 4:
				 m_packPro.GetPortPack(sendData,25,m_iLDPower);
				 break;
			 }
		     break;
		}
	case IDC_EDIT_APD:        // APDƫѹ
		m_packPro.GetPortPack(sendData,30,m_iAPD);
		break;
	case IDC_RAD_APDOpen:   // APDƫѹ��
		m_packPro.GetPortPack(sendData,33,m_iAPDOpen);
		break;
	case IDC_RAD_APDClose:   // APDƫѹ��
		m_packPro.GetPortPack(sendData,33,m_iAPDOpen);

		break;
	case IDC_RAD_SINGLE:      // APDģʽ- ��ģ
		m_packPro.GetPortPack(sendData,32,m_iAPDMode);
		break;
	case IDC_RAD_MUL:         // APDģʽ- ��ģ
		m_packPro.GetPortPack(sendData,32,m_iAPDMode);
		break;

	default:
	    break;
	}
	
	// ����������ʾ
	WriteData(sendData.GetData(),sendData.GetSize(),TRUE);

	// CByteArray ת char*
	int size = sendData.GetSize();
	char* pSendData = new char[size];
	for (int i = 0 ; i < size; i++)
	{
		pSendData[i] = sendData[i];
	}
	// ���ڷ�����
	m_comm.Write(pSendData,sendData.GetSize()); //����ʮ����������
	delete[] pSendData;
	
}


// �Ӷ�ȡ�ı���ȡ����
int CJWM1400CEDlg::GetDataFromPack()
{
	int  iCommand = 0;
	if (m_packPro.m_packData.GetSize() > 0)
	{	
     	WriteData(m_packPro.m_packData.GetData(),m_packPro.m_packData.GetSize(),FALSE);

		if (m_packPro.m_packData.GetSize() == 13)
		{
			ProReadPackCommand(m_packPro.m_packData);   // �����Ӧ����
			iCommand = 0;
		}
		else
		{
		     ProReadPackWaveData(m_packPro.m_packData,1);	 // ����ȡ����������
			 iCommand = 1;
		}

		m_packPro.m_packData.RemoveAll();
	}
	else if (m_packPro.m_TempData.GetSize() > 0)
	{
		WriteData(m_packPro.m_TempData.GetData(),m_packPro.m_TempData.GetSize(),FALSE);

		if (m_packPro.m_TempData.GetSize() == 13)
		{
			ProReadPackCommand(m_packPro.m_TempData);      // �����Ӧ����
			iCommand = 0 ;
		}
		else
		{
		     ProReadPackWaveData(m_packPro.m_TempData,1);	   // ����ȡ����������
			 iCommand = 1;
		}
		m_packPro.m_TempData.RemoveAll();
	}
	return iCommand;

}

// ���ݶ�����������ʾ����
void CJWM1400CEDlg::ProReadPackWaveData(CByteArray& tempArry,int style)
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
				m_Plot.AddPoint(0,CTime(0),y);
			}
			break;		

		}	    
	default:
	    break;
	}

	m_Plot.SetRealSerial2();
	m_Plot.Invalidate(FALSE);	
}

// ���ò���ʱ����״̬
void CJWM1400CEDlg::SetStartSamplingState() 
{
	m_Plot.Reset();
	m_Plot.Invalidate(FALSE);

	GetDlgItem(IDC_EDIT_SAMPLING_TIMES)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_PulsWide)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_Wave)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_Length)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_BMultipleT)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_BMultipleA)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LDPOWER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_APD)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAD_APDOpen)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAD_APDClose)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAD_SINGLE)->EnableWindow(FALSE);
	GetDlgItem(IDC_RAD_MUL)->EnableWindow(FALSE);

}

// ���ò������ʱ����״̬
void CJWM1400CEDlg::SetEndSamplingState() 
{
	GetDlgItem(IDC_EDIT_SAMPLING_TIMES)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_PulsWide)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_Wave)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_Length)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_BMultipleT)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_BMultipleA)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_LDPOWER)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_APD)->EnableWindow(TRUE);
	GetDlgItem(IDC_RAD_APDOpen)->EnableWindow(TRUE);
	GetDlgItem(IDC_RAD_APDClose)->EnableWindow(TRUE);
	GetDlgItem(IDC_RAD_SINGLE)->EnableWindow(TRUE);
	GetDlgItem(IDC_RAD_MUL)->EnableWindow(TRUE);
}

void CJWM1400CEDlg::OnBTNStart() 
{
	UpdateData(TRUE);
	m_Plot.m_iLimitNum = m_iLimit;

	SetStartSamplingState();
	
	if (m_iSamplingTimes > 30)
	{
		AfxMessageBox(_T("ȡ������Ӧ <= 30 !"));
		return ;
	}

	if (!m_Plot.SetSerieNum(m_iSamplingTimes))
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
void CJWM1400CEDlg::SetSamplingNumberCommand() 
{
	UpdateData(TRUE);
	// ������������
	CByteArray sendData;
	m_packPro.GetPortPack(sendData,0,m_Wave[m_iWave]);

	// ����ת��CByteArray ->  char*
	char* sendDataTemp = new char[sendData.GetSize()];
	for (int i= 0; i < sendData.GetSize(); i++)
	{
		sendDataTemp[i] = sendData[i];
	}
	delete[] sendDataTemp;

	// ���ڷ�������
	m_comm.Write(sendDataTemp,sendData.GetSize()); //����ʮ����������
	// ����д����
	WriteData(sendData.GetData(),sendData.GetSize(),TRUE);

	// ������� ������� 
	GetDlgItem(IDC_STATIC_RNum)->SetWindowText(_T("0"));
	UpdateData(FALSE);
}

// д���ݵ���������
void CJWM1400CEDlg::WriteData(byte* buffer,int size,BOOL isSend)
{
	UpdateData(TRUE);
	CString strTemp;
	// д��������
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

// ���ݶ��������ݽ���������Ӧ
void CJWM1400CEDlg::ProReadPackCommand(CByteArray& tempArry)
{
	CString strMsg = "";
		switch(tempArry[7])
		{
		case 0:
			strMsg += _T("��ʼ�ɼ�����");
			break;
		case 1:
			strMsg += _T( "�趨��������");
			break;
		case 2:
			strMsg += _T("�趨������");
		    break;
		case 3:
			strMsg += _T("�趨TIA����");
		    break;
		case 4:
			strMsg += _T("�趨AMP����");
			break;
		case 5:
			strMsg += _T("�趨������1�ĵ���DAC");
			break;
		case 30:
			strMsg += _T("�趨APD��ƫѹDAC");
		    break;
		case 32:
			strMsg += _T("�趨APD����ģʽ");
		    break;
		case 33:
			strMsg += _T("��APD����");
		    break;
		case 34:
			strMsg += _T("ͨ������");
		    break;
		default:				
		    break;
		}

        //  ���
		switch(tempArry[8])
		{
		case 0:
			strMsg += _T("ʧ��!");
			break;
		case 1:
			strMsg += _T("�ɹ�!");
		    break;
		default:
		    break;
		}
	AfxMessageBox(strMsg);
	UpdateData(FALSE);
}

void CJWM1400CEDlg::OnChangeEditExtendsizex() 
{
	int oldExtendSize = m_iExtendSizeX;
	UpdateData(TRUE);	
	if (m_iExtendSizeX >= 10000 || m_iExtendSizeX <= 0)
	{
		AfxMessageBox(_T("����Ӧ��< 10000 && >0"));		
		m_iExtendSizeX = oldExtendSize;
		return ;
	}
	
	m_Plot.SerieExtendX(m_iExtendSizeX);
	UpdateData(FALSE);
	
}

void CJWM1400CEDlg::OnChangeEditExtendsizey() 
{
	int oldExtendSize = m_iExtendSizeY;
	UpdateData(TRUE);
	if (m_iExtendSizeY >= 10000 || m_iExtendSizeY <= 0)
	{
		AfxMessageBox(_T("����Ӧ��< 10000 && >0"));		
		m_iExtendSizeY = oldExtendSize;
		return ;
	}
	
	m_Plot.SerieExtendY(m_iExtendSizeY);
	UpdateData(FALSE);
	
}

void CJWM1400CEDlg::OnChangeEditLimit() 
{
	int oldLimit = m_iLimit;
	UpdateData(TRUE);
	if (m_iLimit > 200 || m_iExtendSizeY < 0)
	{
		AfxMessageBox(_T("����Ӧ��<= 200 && >1"));		
		m_iLimit = oldLimit;
		return ;
	}
	m_Plot.m_iLimitNum = m_iLimit;
	m_Plot.Invalidate(FALSE);
	UpdateData(FALSE);		
}

void CJWM1400CEDlg::OnBtnClear() 
{
	m_Plot.m_series[m_Plot.m_allSerialNum].Reset();
	m_Plot.Invalidate(FALSE);
	
	 m_editSendData = "";
	 GetDlgItem(IDC_STATIC_TNum)->SetWindowText(_T("0"));
	 GetDlgItem(IDC_STATIC_RNum)->SetWindowText(_T("0"));
	 UpdateData(FALSE);		
	
}

void CJWM1400CEDlg::OnSelchangeCMBBitRate() 
{
	ComSetState();
	
}

void CJWM1400CEDlg::OnSelchangeCMBComNum() 
{
	ComSetState();
}

void CJWM1400CEDlg::OnSelchangeCMBDataLen() 
{
	ComSetState();	
}

void CJWM1400CEDlg::OnSelchangeCMBStopLen() 
{
	ComSetState();	
}

void CJWM1400CEDlg::OnSelchangeCMBVerify() 
{
	ComSetState();	
}

void CJWM1400CEDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_Plot.m_isBtnDown)
	{
		int moveDostanceX  = fabs(m_Plot.m_endPoint.x - m_Plot.m_startPoint.x)*100*m_Plot.m_extendSizeX/m_Plot.m_plotRect.Width();
		int moveDostanceY  = fabs(m_Plot.m_endPoint.y - m_Plot.m_startPoint.y)*25*m_Plot.m_extendSizeY/m_Plot.m_plotRect.Height();

		if (m_Plot.m_startPoint.x >= m_Plot.m_endPoint.x)
		{
			if ((m_csliOldXIndex+moveDostanceX)< m_CSliCtrX.GetRangeMax())
			{
				m_CSliCtrX.SetPos(m_csliOldXIndex+moveDostanceX);
				m_csliOldXIndex = m_csliOldXIndex+moveDostanceX;
				m_Plot.RightMoveX(0,moveDostanceX);
			}
			
		}
		else
		{
			if ((m_csliOldXIndex-moveDostanceX) > m_CSliCtrX.GetRangeMin())
			{
				m_CSliCtrX.SetPos(m_csliOldXIndex-moveDostanceX);
				m_csliOldXIndex = m_csliOldXIndex-moveDostanceX;
				m_Plot.LeftMoveX(0,moveDostanceX);
			}
			
		}
		
		// �����ƶ�
		if (m_Plot.m_startPoint.y <= m_Plot.m_endPoint.y)
		{
			if ((m_csliOldYIndex+moveDostanceY)< m_CSliCtrY.GetRangeMax())
			{
				m_CSliCtrY.SetPos(m_csliOldYIndex+moveDostanceY);
				m_csliOldYIndex = m_csliOldYIndex+moveDostanceY;
				m_Plot.UpMoveY(0,moveDostanceY);
			}
			
		}
		else
		{
			if ((m_csliOldYIndex-moveDostanceY) > m_CSliCtrY.GetRangeMin())
			{
				m_CSliCtrY.SetPos(m_csliOldYIndex-moveDostanceY);
				m_csliOldYIndex = m_csliOldYIndex-moveDostanceY;
				m_Plot.DownMoveY(0,moveDostanceY);
			}
			
		}
		
	}
	
	CDialog::OnMouseMove(nFlags, point);
}
