// CommuMrg.cpp: implementation of the CCommuMrg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JWM1400CE.h"
#include "CommuMrg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CnCommMrg
//////////////////////////////////////////////////////////////////////
/*
CnCommMrg::~CommMrg()
{ 
	ClosePort(); 
};

void CnCommMrg::ClosePort() 
{
	if (mComm.IsOpen())
	{
		mComm.Close();
	}
};

inline
void CnCommMrg::SetNotigyNum(int number)
{
	mComm.SetNotifyNum(number);
}

inline
void CnCommMrg::SetClientWnd(HWND hWnd)
{
	mClientWnd = hWnd;
}

inline
HWND CnCommMrg::GetClientWnd()
{
	return mClientWnd;
}
*/
//////////////////////////////////////////////////////////////////////
// CCommuMrg
//////////////////////////////////////////////////////////////////////
CCommuMrg::CCommuMrg()
{
	// init communion data
	InitCommuMrg();

	// init set CnComm
	InitCnCom();
}

CCommuMrg::~CCommuMrg()
{
    ClosePort();
}

void CCommuMrg::InitCommuMrg()
{
	/// init sample tiems
	miSamplingTimes = 1;

	/// init graphic limit parameter
    mGraphLimt.miSwing = Swing;
	mGraphLimt.miReflect = Reflect;
	mGraphLimt.miAttenuation = Attenuation;
	mGraphLimt.miEventTimes = EventTimes;

	// set not recevie data
	mbIsStartRec = false;
}

void CCommuMrg::InitCnCom()
{
	mComm.SetOption(CnComm::EN_THREAD | CnComm::EN_TX_BUFFER | CnComm::EN_TX_THREAD | CnComm::EN_RX_BUFFER | CnComm::EN_RX_THREAD);
	mComm.SetNotifyNum(1);
	mComm.Setup(1024*64);

}


BOOL  CCommuMrg::OpenPort(int nPort, int nBaudRate, int nDataBits, int nStopBits, int nParity)
{
	if (!mComm.IsOpen())
	{
		return mComm.Open(nPort,nBaudRate,nDataBits,nStopBits,nParity);
	}
	return TRUE;
}

inline
void  CCommuMrg::ClosePort()
{
	if (mComm.IsOpen())
	{
		mComm.Close();
	}
}


BOOL CCommuMrg::IsOpen()
{
	return mComm.IsOpen();
}

int  CCommuMrg::ReceiveData()
{
	if (!mComm.IsOpen())
	{
		return 0;
	}

	int sum = 0;
	do {
		const int size = 1024 * 2;
	    unsigned char buffer[size];
		
		int len = mComm.Read(buffer, size);//127
		if (len > 0)
		{
			mPackPro.Push(buffer,len);
			sum += len;
		}

	} while(mComm.IsRxBufferMode() &&  mComm.Input().SafeSize());

	return sum;
}

void CCommuMrg::SendData(LPCVOID lpBuf, DWORD dwSize)
{
	if (!mComm.IsOpen())
	{
		return;
	}
	mComm.Write(lpBuf,dwSize);
}


