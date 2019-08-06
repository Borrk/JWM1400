// WaveAnaylse.cpp: implementation of the WaveAnaylse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveAnaylse.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WaveAnaylse::WaveAnaylse()
{
	m_iNodeNum = 0;	
	m_pNode = NULL;
}

WaveAnaylse::~WaveAnaylse()
{
	if (m_pNode != NULL)
	{
		free(m_pNode);
	}
	m_pNode = NULL;
}

void WaveAnaylse::Reset()
{
	m_iNodeNum = 0;


	if (m_pNode != NULL)
	{
		free(m_pNode);
	}
	m_pNode = NULL;
}


void WaveAnaylse::AddNode(long leftX, double leftY, long rightX, double rightY, long cuspX, double cuspY)
{
	if(m_iNodeNum > 0)
		m_pNode = (Node*)realloc(m_pNode, (m_iNodeNum+1)*sizeof(Node));
	else
		m_pNode = (Node*)malloc((m_iNodeNum+1)*sizeof(Node));

	m_iNodeNum++;
	
	m_pNode[m_iNodeNum -1].left.x = leftX;
	m_pNode[m_iNodeNum -1].left.y	= leftY;

	m_pNode[m_iNodeNum -1].right.x = rightX;
	m_pNode[m_iNodeNum -1].right.y = rightY;

	m_pNode[m_iNodeNum -1].cusp.x = cuspX;
	m_pNode[m_iNodeNum -1].cusp.y	= cuspY;
	
}

// 对图形进行分析
void WaveAnaylse::AnaylseWave(serie* pSerie, double dSwing)
{
	if (pSerie->m_lNoValues < 1000)
	{
		return;
	}
	Data dLeft; 
	Data dRight;
	Data dCusp;
	double high = 0;

	dLeft.x = 1000;
	dLeft.y = pSerie->m_pvalues[1000].dValue;

	dRight = dLeft;
	dCusp = dLeft;

    int state;

	state = 0;

	for (int i = 1001;i < pSerie->m_lNoValues; i++)
	{
		long   x = pSerie->m_pvalues[i].ValueTime.GetTime();
		double y = pSerie->m_pvalues[i].dValue;

		// 波峰
		if (y >= dLeft.y)
		{	
			do 
			{
				// 记录波峰最大值, 并且是最先发生的点
				if (y > dCusp.y )
				{
					dCusp.x = x;
					dCusp.y = y;
				}

				i++;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;
			} while( y >= dLeft.y && i < pSerie->m_lNoValues);

			dRight.x = x;
			dRight.y = y;

			do 
			{
				//  记录右值最小值
				if (y < dRight.y)
				{
					dRight.x = x;
					dRight.y = y;
				}

				i++;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;

			} while(y <= dRight.y && i < pSerie->m_lNoValues);
			
            NodeCheckAdd(dLeft.x, dLeft.y, dRight.x, dRight.y, dCusp.x, dCusp.y,dSwing);
			// 恢复初态重新取
			dLeft.x = x;
			dLeft.y = y;
			dRight = dLeft;
			dCusp = dLeft;

			continue;
		}
		// 波谷
		else
		{
			do 
			{
				// 记录波峰最大值, 并且是最先发生的点
				if (y < dCusp.y )
				{
					dCusp.x = x;
					dCusp.y = y;
				}

				i++;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;
			} while( y <= dLeft.y && i < pSerie->m_lNoValues);

			dRight.x = x;
			dRight.y = y;

			do 
			{
				//  记录右值最小值
				if (y > dRight.y)
				{
					dRight.x = x;
					dRight.y = y;
				}

				i++;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;

			} while(y >= dRight.y && i < pSerie->m_lNoValues);
			
            NodeCheckAdd(dLeft.x, dLeft.y, dRight.x, dRight.y, dCusp.x, dCusp.y,dSwing);
			// 恢复初态重新取
			dLeft.x = x;
			dLeft.y = y;
			dRight = dLeft;
			dCusp = dLeft;

			continue;

		}

	}

}   

void WaveAnaylse::AnaylseWave2(serie* pSerie, double dSwing)
{
	if (pSerie->m_lNoValues < 500)
	{
		return;
	}
	Data dLeft; 
	Data dRight;
	Data dCusp;
	double high = 0;

	dLeft.x = 1000;
	dLeft.y = pSerie->m_pvalues[1000].dValue;

	dRight = dLeft;
	dCusp = dLeft;

    int state;

	state = 0;

	for (int i = 1001;i < pSerie->m_lNoValues; i++)
	{
		long   x = pSerie->m_pvalues[i].ValueTime.GetTime();
		double y = pSerie->m_pvalues[i].dValue;
		if (y > dLeft.y)
		{
AddUp:		while(y >= dCusp.y && i < pSerie->m_lNoValues)
			{
				dCusp.x = x;
				dCusp.y = y;

				i++;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;
			} 

			if (y - dLeft.x >= dSwing)
			{
				i++;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;

				while (y < dCusp.y && i < pSerie->m_lNoValues)
				{
					i++;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}
				goto AddUp;				
			}
			else
			{
				dRight.x = x;
				dRight.y = y;
				while (y <= dRight.y && i < pSerie->m_lNoValues)
				{
					dRight.x = x;
					dRight.y = y;

					i++;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}
				
				// 验证数据
				NodeCheckAdd(dLeft.x, dLeft.y, dRight.x, dRight.y, dCusp.x, dCusp.y,dSwing);
				// 重新一次开始
				dLeft.x = x;
				dLeft.y = y;
				
				dCusp = dLeft;
				dRight = dLeft;	
				continue;

			}
		}
		else
		{
AddDown:	while(y <= dCusp.y && i < pSerie->m_lNoValues)
			{
				dCusp.x = x;
				dCusp.y = y;

				i++;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;
			} 

			if (dLeft.x - y >= dSwing && i < pSerie->m_lNoValues)
			{
				i++;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;

				while (y > dCusp.y && i < pSerie->m_lNoValues)
				{
					i++;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}
				goto AddDown;				
			}
			else
			{
				dRight.x = x;
				dRight.y = y;
				while (y >= dRight.y && i < pSerie->m_lNoValues)
				{
					dRight.x = x;
					dRight.y = y;

					i++;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}
				
				// 验证数据
				NodeCheckAdd(dLeft.x, dLeft.y, dRight.x, dRight.y, dCusp.x, dCusp.y,dSwing);
				// 重新一次开始
				dLeft.x = x;
				dLeft.y = y;
				
				dCusp = dLeft;
				dRight = dLeft;	
				continue;
			}
		}

	}


} 

void WaveAnaylse::AnaylseWave3(serie* pSerie, double dSwing,int eventLong)
{
	if (pSerie->m_lNoValues < 9)
	{
		return;
	}
	m_pSerie = pSerie;

	Data dLeft; 
	Data dRight;
	Data dCusp;
	Node node;
	double high = 0;

	dLeft.x = 8;
	dLeft.y = pSerie->m_pvalues[8].dValue;

	dRight = dLeft;
	dCusp = dLeft;

    int state;

	state = 0;
	for (long i = 9;i < pSerie->m_lNoValues; i++)
	{

		long   x = pSerie->m_pvalues[i].ValueTime.GetTime();
		double y = pSerie->m_pvalues[i].dValue;
		if (y >= dLeft.y)
		{
			dCusp.x = x;
			dCusp.y = y;
			while (y >= dCusp.y && i < pSerie->m_lNoValues)
			{
				dCusp.x = x;
				dCusp.y = y;

				i++;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;
			}

			if (dCusp.y - dLeft.y <= dSwing)
			{
				dLeft.x = x;
				dLeft.y = y;

				dCusp = dLeft;
				continue;
			}
			else
			{
				node.left = dLeft;
				
				dRight.x = x;
				dRight.y = y;
				
				// 取波峰最高点
				int cuspIndex = GetMaxInSize(dLeft.x,eventLong);
				node.cusp.x = pSerie->m_pvalues[cuspIndex].ValueTime.GetTime(); 
				node.cusp.y = pSerie->m_pvalues[cuspIndex].dValue;

				x = pSerie->m_pvalues[cuspIndex+1].ValueTime.GetTime();
				y = pSerie->m_pvalues[cuspIndex+1].dValue;
				i = cuspIndex + 1;
				while (y > node.cusp.y && i < pSerie->m_lNoValues)
				{
					node.cusp.x = x;
					node.cusp.y = y;

					i++;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}

			
				// 取下降点
				int rightIndex = GetMinInSize(node.cusp.x,eventLong);
				node.right.x = pSerie->m_pvalues[rightIndex].ValueTime.GetTime(); 
				node.right.y = pSerie->m_pvalues[rightIndex].dValue;

				x = pSerie->m_pvalues[rightIndex+1].ValueTime.GetTime();
				y = pSerie->m_pvalues[rightIndex+1].dValue;
				i = rightIndex + 1;
				while (y <= node.right.y && i < pSerie->m_lNoValues)
				{
					node.right.x = x;
					node.right.y = y;

					i++;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}

				// 验证数据
				NodeCheckAdd(&node,dSwing);

				i = node.right.x;
				// 重新一次开始
				dLeft = node.right;	
				dCusp = dLeft;
				dRight = dLeft;	

				
				node.left = dLeft;
				node.right = dRight;
				node.cusp = dCusp;
				continue;		
				
			}
		}
		else
		{
			dCusp.x = x;
			dCusp.y = y;
			while (y <= dCusp.y && i < pSerie->m_lNoValues)
			{
				dCusp.x = x;
				dCusp.y = y;

				i++;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;
			}

			if (dLeft.y - dCusp.y <= dSwing)
			{
				dLeft.x = x;
				dLeft.y = y;

				dCusp = dLeft;
				continue;
			}
			else
			{
				node.left = dLeft;
				
				dRight.x = x;
				dRight.y = y;
				
				// 取波谷最低点
				int cuspIndex = GetMinInSize(dLeft.x,eventLong);
				node.cusp.x = pSerie->m_pvalues[cuspIndex].ValueTime.GetTime(); 
				node.cusp.y = pSerie->m_pvalues[cuspIndex].dValue;

				x = pSerie->m_pvalues[cuspIndex+1].ValueTime.GetTime();
				y = pSerie->m_pvalues[cuspIndex+1].dValue;
				i = cuspIndex + 1;
				while (y < node.cusp.y && i < pSerie->m_lNoValues)
				{
					node.cusp.x = x;
					node.cusp.y = y;

					i++;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}

				//  取上升点
				int rightIndex = GetMaxInSize(node.cusp.x,eventLong);
				node.right.x = pSerie->m_pvalues[rightIndex].ValueTime.GetTime(); 
				node.right.y = pSerie->m_pvalues[rightIndex].dValue;

				x = pSerie->m_pvalues[rightIndex+1].ValueTime.GetTime();
				y = pSerie->m_pvalues[rightIndex+1].dValue;
				i = rightIndex + 1;
				while (y >= node.right.y && i < pSerie->m_lNoValues)
				{
					node.right.x = x;
					node.right.y = y;

					i++;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}


				// 验证数据
				NodeCheckAdd(&node,dSwing);
				i = node.right.x;
				// 重新一次开始
				dLeft = node.right;		
				dCusp = dLeft;
				dRight = dLeft;	

				node.left = dLeft;
				node.right = dRight;
				node.cusp = dCusp;
				continue;		
				
			}

		}

	}


}

void WaveAnaylse::AnaylseWave4(serie* pSerie, double dSwing,int eventLong)
{
	if (pSerie->m_lNoValues < 9)
	{
		return;
	}
	m_pSerie = pSerie;

	Data dLeft; 
	Data dRight;
	Data dCusp;
	Node node;
	double high = 0;

	dLeft.x = pSerie->m_lNoValues -1;
	dLeft.y = pSerie->m_pvalues[pSerie->m_lNoValues -1].dValue;

	dRight = dLeft;
	dCusp = dLeft;

    int state;

	state = 0;
//	for (long i = 9;i < pSerie->m_lNoValues; i++)
//	{
	for (long i = pSerie->m_lNoValues -1;i >= 8; i--)
	{
		long   x = pSerie->m_pvalues[pSerie->m_lNoValues -1].ValueTime.GetTime();
		double y = pSerie->m_pvalues[pSerie->m_lNoValues -1].dValue;
		if (y >= dLeft.y)
		{
			dCusp.x = x;
			dCusp.y = y;
			while (y >= dCusp.y && i >= 8)
			{
				dCusp.x = x;
				dCusp.y = y;

				i--;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;
			}

			if (dCusp.y - dLeft.y <= dSwing)
			{
				dLeft.x = x;
				dLeft.y = y;

				dCusp = dLeft;
				dRight = dCusp;
				continue;
			}
			else
			{
				node.left = dLeft;
				
				dRight.x = x;
				dRight.y = y;
				
				// 取波峰最高点
				int maxIndex = 0;
				if (dLeft.x - eventLong > 8)
				{
					maxIndex = eventLong;
				}
				else
				{
					maxIndex = dLeft.x - 8;
				}
				int cuspIndex = GetMaxInSize(dLeft.x - maxIndex,maxIndex);
				node.cusp.x = pSerie->m_pvalues[cuspIndex].ValueTime.GetTime(); 
				node.cusp.y = pSerie->m_pvalues[cuspIndex].dValue;

				x = pSerie->m_pvalues[cuspIndex-1].ValueTime.GetTime();
				y = pSerie->m_pvalues[cuspIndex-1].dValue;
				i = cuspIndex - 1;
				while (y >= node.cusp.y && i >= 8)
				{
					node.cusp.x = x;
					node.cusp.y = y;

					i--;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}

			
				// 取下降点
				int minIndex = 0;
				if (node.cusp.x - eventLong > 8)
				{
					minIndex = eventLong;
				}
				else
				{
					minIndex = node.cusp.x - 8;
				}
				int rightIndex = GetMinInSize(node.cusp.x - minIndex,minIndex);
				node.right.x = pSerie->m_pvalues[rightIndex].ValueTime.GetTime(); 
				node.right.y = pSerie->m_pvalues[rightIndex].dValue;

				x = pSerie->m_pvalues[rightIndex-1].ValueTime.GetTime();
				y = pSerie->m_pvalues[rightIndex-1].dValue;
				i = rightIndex - 1;
				while (y <= node.right.y && i >= 8)
				{
					node.right.x = x;
					node.right.y = y;

					i--;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}

				// 验证数据
				NodeCheckAdd(&node,dSwing);

				i = node.right.x;
				// 重新一次开始
				dLeft = node.right;	
				dCusp = dLeft;
				dRight = dLeft;	

				
				node.left = dLeft;
				node.right = dRight;
				node.cusp = dCusp;
				continue;		
				
			}
		}
		else
		{
			dCusp.x = x;
			dCusp.y = y;
			while (y <= dCusp.y && i >= 8)
			{
				dCusp.x = x;
				dCusp.y = y;

				i--;
				x = pSerie->m_pvalues[i].ValueTime.GetTime();
				y = pSerie->m_pvalues[i].dValue;
			}

			if (dLeft.y - dCusp.y <= dSwing)
			{
				dLeft.x = x;
				dLeft.y = y;

				dCusp = dLeft;
				continue;
			}
			else
			{
				node.left = dLeft;
				
				dRight.x = x;
				dRight.y = y;
				
				// 取波谷最低点
				int minIndex = 0;
				if (dLeft.x - eventLong > 8)
				{
					minIndex = eventLong;
				}
				else
				{
					minIndex = dLeft.x - 8;
				}				
				int cuspIndex = GetMinInSize(dLeft.x - minIndex,minIndex);
				node.cusp.x = pSerie->m_pvalues[cuspIndex].ValueTime.GetTime(); 
				node.cusp.y = pSerie->m_pvalues[cuspIndex].dValue;

				x = pSerie->m_pvalues[cuspIndex-1].ValueTime.GetTime();
				y = pSerie->m_pvalues[cuspIndex-1].dValue;
				i = cuspIndex - 1;
				while (y <= node.cusp.y && i >= 8)
				{
					node.cusp.x = x;
					node.cusp.y = y;

					i--;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}

				//  取上升点
				int maxIndex = 0;
				if (node.cusp.x - eventLong > 8)
				{
					maxIndex = eventLong;
				}
				else
				{
					maxIndex = node.cusp.x - 8;
				}				
				int rightIndex = GetMaxInSize(node.cusp.x - maxIndex,maxIndex);
				node.right.x = pSerie->m_pvalues[rightIndex].ValueTime.GetTime(); 
				node.right.y = pSerie->m_pvalues[rightIndex].dValue;

				x = pSerie->m_pvalues[rightIndex-1].ValueTime.GetTime();
				y = pSerie->m_pvalues[rightIndex- 1].dValue;
				i = rightIndex - 1;
				while (y >= node.right.y && i >= 8)
				{
					node.right.x = x;
					node.right.y = y;

					i--;
					x = pSerie->m_pvalues[i].ValueTime.GetTime();
					y = pSerie->m_pvalues[i].dValue;
				}
				
			

				// 验证数据
				NodeCheckAdd(&node,dSwing);
				i = node.right.x;
				// 重新一次开始
				dLeft = node.right;		
				dCusp = dLeft;
				dRight = dLeft;	

				node.left = dLeft;
				node.right = dRight;
				node.cusp = dCusp;
				continue;
			}
		}
	}
}

int WaveAnaylse::GetMaxInSize(long startIndex,int size)
{
	double y;
	long index = startIndex;
	double max = m_pSerie->m_pvalues[startIndex].dValue;

	for (long i=startIndex+1; i <= startIndex+size && i < m_pSerie->m_lNoValues; i++)
	{	
		 y = m_pSerie->m_pvalues[i].dValue;
         if (y > max)
         {
		    max = y;
			index = i;
         }
	}

	return index;
}
int WaveAnaylse::GetMinInSize(long startIndex,int size)
{
	double y;

	long index = startIndex;
	double min = m_pSerie->m_pvalues[startIndex].dValue;

	for (long i = startIndex+size; i > startIndex && i < m_pSerie->m_lNoValues; i--)
	{
		 y = m_pSerie->m_pvalues[i].dValue;
         if (y < min)
         {
			min = y;
			index = i;
         }
	}
	return index;
}

void WaveAnaylse::GetRealRight(Data& cusp,Data& right,double dSwing)
{
	Data data,left = right;
	for (int i = right.x; i >= cusp.x; i--)
	{
	
		data.x = m_pSerie->m_pvalues[i].ValueTime.GetTime();
		data.y = m_pSerie->m_pvalues[i].dValue;
		if (data.y >= right.y)
		{
			left.x = data.x;
			left.y = data.y;
			while (data.y >= left.y && i >= cusp.x)
			{
				left.x = data.x;
				left.y = data.y;

				i--;
				data.x = m_pSerie->m_pvalues[i].ValueTime.GetTime();
				data.y = m_pSerie->m_pvalues[i].dValue;
			}

			if ((left.y - right.y) <= dSwing)
			{
				right.x = left.x;
				right.y = left.y;
				left = right;

				i = right.x;
				continue;
			}
			else
			{
				return;
			}
		
			
		}
		else
		{
			left.x = data.x;
			left.y = data.y;
			while (data.y <= left.y && i >= cusp.x)
			{
				left.x = data.x;
				left.y = data.y;

				i--;
				data.x = m_pSerie->m_pvalues[i].ValueTime.GetTime();
				data.y = m_pSerie->m_pvalues[i].dValue;
			}

			if ((left.y - right.y) <= dSwing)
			{
				right.x = left.x;
				right.y = left.y;
				left = right;

				i = right.x;
				continue;
			}
			else
			{
				return;
			}
			
		}
	}


}

//////////////////////////////////////////////////////////////////////////
//  No use
//////////////////////////////////////////////////////////////////////////
int WaveAnaylse::CheckMinInSize(long startIndex,long endIndex,double dSwing)
{
	double y;
	Data dCusp;
    Data dRight;
	dRight.x = m_pSerie->m_pvalues[startIndex].ValueTime.GetTime();
	dRight.y = m_pSerie->m_pvalues[startIndex].dValue;
	dCusp = dRight;

	for (int i = endIndex - 1; i >= startIndex; i--)
	{
		y = m_pSerie->m_pvalues[i].dValue;
		if (y >= dCusp.y)
		{
			while (y >= dCusp.y && i <= endIndex)
			{
				dCusp.y = y;

				i++;
				y = m_pSerie->m_pvalues[i].dValue;
			}
			dCusp.y = y;
			continue;
		}
		else
		{
			while (y <= dCusp.y && i <= endIndex)
			{
				dCusp.y = y;

				i++;
				y = m_pSerie->m_pvalues[i].dValue;
			}
			dCusp.y = y;
			continue;
		}		
		
	}
   return 1;
}
//////////////////////////////////////////////////////////////////////////

void WaveAnaylse::NodeCheckAdd(long leftX, double leftY, long rightX, double rightY, long cuspX, double cuspY,double dSwing)
{
	if (leftY <= cuspY)
	{
		int minNum = leftY<rightY? leftY:rightY;
		int iswing =  cuspY - minNum;
		if (iswing >= dSwing)
		{
			AddNode(leftX, leftY, rightX, rightY, cuspX, cuspY);
		}
		
	}
	else
	{
		int maxNum = leftY>rightY? leftY:rightY;
		int iswing = maxNum - cuspY;
		if (iswing >= dSwing)
		{	
			AddNode(leftX, leftY, rightX, rightY, cuspX, cuspY);
		}
	}	
} 

void WaveAnaylse::NodeCheckAdd(Node* node, double dSwing)
{
	if (node->left.y <= node->cusp.y)
	{
		int minNum = node->left.y < node->right.y? node->left.y:node->right.y;
		int iswing =  node->cusp.y - minNum;
		if (iswing >= dSwing)
		{
		
			GetRealRight(node->cusp,node->right,dSwing);		
			AddNode(node->left.x, node->left.y, node->right.x, node->right.y, node->cusp.x, node->cusp.y);
		}

	}
	else
	{
		int maxNum = (int)node->left.y > node->right.y? node->left.y:node->right.y;
		int iswing =  maxNum - node->cusp.y ;
		if (iswing >= dSwing)
		{
		
			GetRealRight(node->cusp,node->right,dSwing);		
			AddNode(node->left.x, node->left.y, node->right.x, node->right.y, node->cusp.x, node->cusp.y);
		}
	}
}

// 根据正反两次来取数据
void WaveAnaylse::ProAnaylseWave(serie* pSerie, double dSwing,int eventLong)
{
	WaveAnaylse m_eStartanaylse;
	
	AnaylseWave3(pSerie, dSwing, eventLong);                  // 正向
	m_eStartanaylse.AnaylseWave4(pSerie, dSwing, eventLong);  //反向

	for (int i = 0; i < m_iNodeNum; i++)
	{
		for (int j = m_eStartanaylse.m_iNodeNum -1; j >= 0 ; j--)
		{
			Node eAnaylseNode = m_eStartanaylse.m_pNode[j];
			if (eAnaylseNode.cusp.x >= m_pNode[i].left.x && eAnaylseNode.cusp.x <= m_pNode[i].right.x)
			{
				//m_pNode[i].right = eAnaylseNode.left;
				break;
			}
		}
		
	}
}  
  


