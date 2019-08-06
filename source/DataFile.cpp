// DataFile.cpp: implementation of the DataFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PARA_LEN   100
#define DATA_STATR 201
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


DataFile::~DataFile()
{
	Close();
}

BOOL DataFile::Open(CString fileName, UINT nOpenFlags)
{
	if (!m_file.Open(fileName,nOpenFlags))	
		m_isOpen = FALSE;	
	else
		m_isOpen = TRUE;
		
	m_dataLen = 0;
	return m_isOpen;
}

void DataFile::Close()
{
	if (m_isOpen)
	{
		m_file.Close();
	}
	m_isOpen = FALSE;

}
BOOL DataFile::ReadPara(GraphPara &para)
{
	if (!m_isOpen)
	{
		return FALSE;
	}
	m_file.SeekToBegin();
	char* buffer[PARA_LEN];
	m_file.Read(buffer, PARA_LEN);
	memcpy(&para,buffer,sizeof(para));

	int len = m_file.GetLength();
	if ((para.Length + 12 + DATA_STATR) != len)
	{
		return FALSE;
	}

	SetDataGraphLength(para.Length + 12);    // 数据包头 -8 ，包尾-4
    return TRUE;
}

BOOL DataFile::ReadData(void* data, long size)
{
	if (!m_isOpen)
	{
		return FALSE;
	}
	m_file.Seek(DATA_STATR,CFile::begin);
	m_file.Read(data, size);	
    return TRUE;

}

void DataFile::WritePara(GraphPara &para)
{
	if (!m_isOpen)
	{
		return ;
	}
	m_file.SeekToBegin();
	m_file.Write((char*)&para,sizeof(GraphPara));

}

void DataFile::WriteData(const void* data, long size)
{
	if (!m_isOpen)
	{
		return ;
	}
    m_file.Seek(DATA_STATR,CFile::begin);
	m_file.Write(data, size);
}


