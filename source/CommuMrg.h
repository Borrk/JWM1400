// CommuMrg.h: interface for the CCommuMrg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMUMRG_H__48E698D0_27CD_4D75_8230_F37E868924AC__INCLUDED_)
#define AFX_COMMUMRG_H__48E698D0_27CD_4D75_8230_F37E868924AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CnComm.h"
#include "CSeriesPortPro.h"
#include "DataFile.h"

#define TIMER_OUT  60
#define EXTEND_X   50
#define EXTEND_Y   12
#define EXTEND_MAX 10000

#define Swing		10
#define Attenuation 10
#define EventTimes  60
#define Reflect		11

#define MAX_SAPLE_TIMES  30

typedef struct _GraphLimit
{
public:
	int miSwing;
	int miReflect;
	int miAttenuation;
	int miEventTimes;
}GraphLimit;

struct CnCommPara
{
	int miComNum ;
	int miBitRate;
	int miDataLen;
	int miVerifyLen;
	int miStopLen;
};


typedef struct _GrapParaCaption 
{

}GrapParaCaption ;

class CCommuMrg  
{
public:
	CCommuMrg();
	virtual ~CCommuMrg();

public:
	void InitCommuMrg();
	void InitCnCom();
public:
	CnComm*			GetCnComm()	     { return &mComm; };
	CSeriesPortPro* GetPortPro()     { return &mPackPro; };

	int             GetSampleTimes() { return miSamplingTimes; };
	void            SetSampleTimes(int times) {  miSamplingTimes = times; };

	int            ReceiveData();
	void           SendData(LPCVOID lpBuf, DWORD dwSize);

    BOOL  OpenPort(int nPort, int nBaudRate, int nDataBits, int nStopBits, int nParity);
	void  ClosePort();
   	BOOL  IsOpen();


	
public:
	int        mWave;
	bool       mbIsStartRec;
	
public:
	CnComm     mComm;              // series port 
	int        miSamplingTimes;    // sample times

	CSeriesPortPro mPackPro;    
	GraphLimit     mGraphLimt;
	GraphPara      mGraphPara;
};

#endif // !defined(AFX_COMMUMRG_H__48E698D0_27CD_4D75_8230_F37E868924AC__INCLUDED_)
