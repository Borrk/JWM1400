// DataFile.h: interface for the DataFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAFILE_H__0F8B0D06_76BE_4D9E_935F_9ACE3D7E5DEF__INCLUDED_)
#define AFX_DATAFILE_H__0F8B0D06_76BE_4D9E_935F_9ACE3D7E5DEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _GraphPara  
{
	int PulsWid;
	int Wave;
	int Length;
	int BMultipleA;
	int BMultipleT;
	int LDPower;
	int APD;
	int APDOpen;
	int APDMode;
}GraphPara;

class DataFile  
{
private:
	CFile m_file;
	BOOL  m_isOpen;
	long  m_dataLen;
public:
	virtual ~DataFile();
public:	
	BOOL Open(CString fileName, UINT nOpenFlags);
	void Close();
	BOOL ReadPara(GraphPara &para);
	BOOL ReadData(void* data,long size);

	void WritePara(GraphPara &para);
	void WriteData(const void* data,long size);
public:
	inline long GetDataGraphLength(){  return m_dataLen; };
	inline void SetDataGraphLength(long len){  m_dataLen = len; };


};

#endif // !defined(AFX_DATAFILE_H__0F8B0D06_76BE_4D9E_935F_9ACE3D7E5DEF__INCLUDED_)
