// WaveAnaylse.h: interface for the WaveAnaylse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAVEANAYLSE_H__9EA126E1_156E_40CE_8C22_0981D3890EA3__INCLUDED_)
#define AFX_WAVEANAYLSE_H__9EA126E1_156E_40CE_8C22_0981D3890EA3__INCLUDED_

#include "clPlot.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _Data 
{
	long x;
	double y;
}Data;

typedef struct _Node 
{
	Data left;
	Data right;
	Data cusp;
	//int  type;         // 0 - ˥����  1 - ���䣻  3 - ����˥��      
	
}Node;

class WaveAnaylse  
{
public:
	WaveAnaylse();
	virtual ~WaveAnaylse();
public:
	serie* m_pSerie;
	int m_iNodeNum;         // ����˥������	
	Node* m_pNode;          // ����˥������

	void Reset();
	void AddNode(long leftX=0, double leftY=0, long rightX=0, double rightY=0, long cuspX=0, double cuspY=0); // ��ӷ���

	// ��ͼ�ν��з���
	void AnaylseWave(serie* pSerie, double dSwing);                
	void AnaylseWave2(serie* pSerie, double dSwing);  
	void AnaylseWave3(serie* pSerie, double dSwing,int eventLong); // ����
	void AnaylseWave4(serie* pSerie, double dSwing,int eventLong); // ����
	void ProAnaylseWave(serie* pSerie, double dSwing,int eventLong);


	int GetMaxInSize(long startIndex,int size);
	int GetMinInSize(long startIndex,int size);
	int CheckMinInSize(long startIndex,long endIndex,double dSwing);
	void GetRealRight(Data& cusp,Data& right,double dSwing);



	void NodeCheckAdd(Node* node, double dSwing);
	void NodeCheckAdd(long leftX, double leftY, long rightX, double rightY, long cuspX, double cuspY,double dSwing);

};


#endif // !defined(AFX_WAVEANAYLSE_H__9EA126E1_156E_40CE_8C22_0981D3890EA3__INCLUDED_)
