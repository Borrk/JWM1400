
#ifndef _SERIERIESPORTPRO_H_
#define _SERIERIESPORTPRO_H_


#define RECEIVE_LEN 1024
#define PACK_NUM 20

enum CMD_KIND{ CMD_Length=1, CMD_PulsWide=2, CMD_BMultipleT=3, CMD_BMultipleA=4, CMD_Wave=34, 
				CMD_LDPower_1=5, CMD_LDPower_2=10, CMD_LDPower_3=15, CMD_LDPower_4=25, 
				CMD_APD=30, CMD_APDOpen=33, CMD_APDMode=32
				};

class CSeriesPortPro
{
public:
	CSeriesPortPro();
	~CSeriesPortPro();

	void Init();
public:	
	CByteArray m_packData;                 // 得到的数据包 处理一个
	CByteArray m_TempData;                 // 缓冲
	int        m_packLen;
	BOOL       m_IsTemp;

	BOOL       m_IsInTemp;                 // receive data is in temp buffer
	CByteArray m_buff;
	
public :
	BOOL CheckPack();
	BOOL OnComm(byte* buffer,int size);	
	void Push(byte* buffer,int size);
	bool IsAPack();

	CByteArray* GetPortPack(CByteArray& sendData, byte CommandNum, int data);
	CByteArray* GetSendCmdPack(CByteArray& sendData, CMD_KIND CommandNum, int data);

	int  ProPackKind();
	BOOL IsDataInTemp() { return m_IsInTemp;};
	CString GetResponseMsg(CByteArray& tempArry);
private:
};


#endif //_SERIERIESPORTPRO_H_