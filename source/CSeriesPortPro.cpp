#include "StdAfx.h"
#include "CSeriesPortPro.h"


CSeriesPortPro::CSeriesPortPro()
{
	Init();
}

void CSeriesPortPro::Init()
{
	m_packData.RemoveAll();
	m_TempData.RemoveAll();
	m_buff.RemoveAll();
	m_packLen = 0;
	m_IsTemp = FALSE;
	m_IsInTemp = FALSE;

}

CSeriesPortPro::~CSeriesPortPro()
{
	m_packData.RemoveAll();
	m_TempData.RemoveAll();
}

// ��װ������
CByteArray* CSeriesPortPro::GetPortPack(CByteArray& sendData, byte CommandNum, int data)
{
	sendData.RemoveAll();
	sendData.SetSize(16);
	// pack head
	sendData[0] = 0xaa;
	sendData[1] = 0x55;

	// data length
	unsigned int dataNum = 16 - 6;
    sendData[2] = dataNum >> 24;
	sendData[3] = dataNum >> 16;
	sendData[4] = dataNum >> 8;
	sendData[5] = dataNum;

	sendData[6] = 0x00;
	sendData[7] = CommandNum;

	// data
	sendData[8] = data >> 24;
	sendData[9] = data >> 16;
	sendData[10] = data >> 8;
	sendData[11] = data;

	int sum = 0;
	for (int i = 0; i < 16; i++)
	{
        sum += sendData[i];
	}

	// verity 
	sendData[12] = sum >> 24;
	sendData[13] = sum >> 16;
	sendData[14] = sum >> 8;
	sendData[15] = sum;

	return &sendData;
}

CByteArray* CSeriesPortPro::GetSendCmdPack(CByteArray& sendData, CMD_KIND CommandNum, int data)
{
	sendData.RemoveAll();
	sendData.SetSize(16);
	// pack head
	sendData[0] = 0xaa;
	sendData[1] = 0x55;

	// data length
	unsigned int dataNum = 16 - 6;
    sendData[2] = dataNum >> 24;
	sendData[3] = dataNum >> 16;
	sendData[4] = dataNum >> 8;
	sendData[5] = dataNum;

	sendData[6] = 0x00;
	sendData[7] = CommandNum;

	// data
	sendData[8] = data >> 24;
	sendData[9] = data >> 16;
	sendData[10] = data >> 8;
	sendData[11] = data;

	int sum = 0;
	for (int i = 0; i < 16; i++)
	{
        sum += sendData[i];
	}

	// verity 
	sendData[12] = sum >> 24;
	sendData[13] = sum >> 16;
	sendData[14] = sum >> 8;
	sendData[15] = sum;

	return &sendData;
}

void CSeriesPortPro::Push(byte* buffer,int size)
{
	for (int i = 0;i <size; i++)
	{
		m_buff.Add(buffer[i]);
	}
}

bool CSeriesPortPro::IsAPack()
{
	int len = m_buff.GetSize();
	if (m_packLen == 0 && len >= 6)
	{
		int dataLen = 0;
		dataLen += (m_buff[2] << 24);
		dataLen += (m_buff[3] << 16);
		dataLen += (m_buff[4] << 8);
		dataLen += m_buff[5] ;

        m_packLen = dataLen;
		AfxMessageBox(_T("m_packLen"));
	}

	if (m_packLen != 0 && (m_packLen == (len - 6)))
	{
		// У����ж�
		
		// �����ۼ�
		int Sum = 0;
		for (int i = 0; i < len - 4; i++)
		{
			Sum += m_buff[i];
		}

		// ���У��λ���ۼӺ�
		int iVerity = 0;
		iVerity += m_buff[len-4] << 24;
		iVerity += m_buff[len-3] << 16; 
		iVerity += m_buff[len-2] << 8;
		iVerity += m_buff[len-1];

		// ����У���
		if (iVerity == Sum)
		{
			return TRUE;
		}
	}
    return FALSE;
}

// �����ڽ��ܵ�������
BOOL CSeriesPortPro::OnComm(byte* buffer,int size)
{
	m_packData.RemoveAll();
	for (int i = 0; i < size; i++)
	{
		if (buffer[i] == 0xaa && buffer[i+1] == 0x55)
		{
			// ������һ�����İ�ͷ������λ
			if (size < 6)
			{
				return FALSE;
			}

			// �õ��������ݳ���
		    int dataLen = 0;
			dataLen += (buffer[i+2] << 24);
			dataLen += (buffer[i+3] << 16);
			dataLen += (buffer[i+4] << 8);
			dataLen += buffer[i+5] ;

            m_packLen = dataLen;

			// ���ܵİ���ȫ
			if ( (dataLen+6) <= size)       
			{
				// �õ�У���
				int iVerityLen = 0;
				int lastIndex = i+6+dataLen-1;
				iVerityLen += buffer[lastIndex];
				iVerityLen += buffer[lastIndex - 1] << 8;
				iVerityLen += buffer[lastIndex - 2] << 16;
				iVerityLen += buffer[lastIndex - 3] << 24;

				// �ۼӺ�
				int iVeritySum = 0;
				for (int j = 0; j < dataLen+6-4 ; j++)
				{		
					int realNum = buffer[i+j];
					iVeritySum += buffer[i+j];
				}

				// �������ݰ�
				if (iVeritySum == iVerityLen)
				{
					for (int k = i; k < dataLen + 6; k++)
					{				
						m_packData.Add(buffer[k]);
					}

					// ��ջ�����
					m_TempData.RemoveAll();
					m_packLen = 0;

					return TRUE;
				}

                return FALSE;
			}		
			else 	
			{
				// ���ܵ��İ�����ȫ �����滺���� m_TempData��
				for (int j = i; j < size ; j++)
				{
					m_TempData.Add(buffer[j]);
				}				
				return FALSE;
			}
		}
		else if (m_TempData.GetSize() >= 6 )
		{

			int NeedLen = 0;
			// �ӵ����Ѿ��պ�һ�����ݰ�
			if (m_packLen <= (m_TempData.GetSize() - 6 + size))
			{
				NeedLen = m_packLen - (m_TempData.GetSize() - 6) + i;
				for (int j = i; j < NeedLen ; j++)
				{
					m_TempData.Add(buffer[j]);
				}
				// ����������Ƿ���ȷ
				m_IsTemp = CheckPack();
				m_packData.RemoveAll();
				return TRUE;
			}			 
			    
			NeedLen = size;
			// ���ܵ��İ�����ȫ
			for (int j = i; j < NeedLen ; j++)
			{
				m_TempData.Add(buffer[j]);
			}
			return FALSE;
			
		}
	}
   return FALSE;
}

// �����������ȷ���[ͨ���ۼӺ�]
BOOL CSeriesPortPro::CheckPack()
{
	if (m_TempData.GetSize() > 0)
	{

		// �����ۼ�
		int Sum = 0;
		for (int i = 0; i < m_TempData.GetSize() - 4; i++)
		{
			Sum += m_TempData[i];
		}

		// ���У��λ���ۼӺ�
		int iVerity = 0;
		int iLastIndex = m_TempData.GetSize() -1;
		iVerity += m_TempData[iLastIndex-3] << 24;
		iVerity += m_TempData[iLastIndex-2] << 16; 
		iVerity += m_TempData[iLastIndex-1] << 8;
		iVerity += m_TempData[iLastIndex];

		// ����У���
		if (iVerity == Sum)
		{
			return TRUE;
		}
		
	}
	return FALSE;
}

// 0---response command pack   ,   1--- point data pack
int  CSeriesPortPro::ProPackKind()
{
	int  iCommand = 0;
	if (!m_IsInTemp)
	{	
		if (m_packData.GetSize() == 13)     // response command 
			iCommand = 0;   
		else		     
			 iCommand = 1;                  // point data pack	
	}
	else
	{
		if (m_TempData.GetSize() == 13)		// response command 	
			iCommand = 0 ;
		else		     
			 iCommand = 1;                  // point data pack	    
		
	}
	return iCommand;
}

// get pesponse message from enter command pack
CString CSeriesPortPro::GetResponseMsg(CByteArray& tempArry)
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
		case 10:
			strMsg += _T("�趨������2�ĵ���DAC");
			break;
		case 15:
			strMsg += _T("�趨������3�ĵ���DAC");
			break;
		case 20:
			strMsg += _T("�趨������4�ĵ���DAC");
			break;
		case 25:
			strMsg += _T("�趨������5�ĵ���DAC");
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
	return strMsg;
}