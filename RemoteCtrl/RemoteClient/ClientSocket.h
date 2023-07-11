#pragma once

#include "pch.h"
#include "framework.h"
#include <string>
#include <vector>

typedef struct MouseEvent {
	MouseEvent() {
		nAction = 0;
		nButton = -1;//Ĭ��ûЧ��
		ptXY.x = 0;
		ptXY.y = 0;
	}
	WORD nAction;//������ƶ���˫��
	WORD nButton;//������Ҽ����м�
	POINT ptXY;//����
}MOUSEEV, * PMOUSEEV;

typedef struct file_info {
	file_info() {
		isInvalid = 0;
		isDirectory = 0;
		hasNext = TRUE;
		memset(szFileName, 0, sizeof(szFileName));
	}
	BOOL isInvalid;//�Ƿ���Ч 1��Ч
	BOOL isDirectory;//�Ƿ�ΪĿ¼ 1��
	BOOL hasNext;//�Ƿ��к���  1��
	char szFileName[256];//�ļ���
}FILEINFO, * PFILEINFO;

#pragma pack(push)
#pragma pack(1)
class CPacket
{
private:
public:
	WORD sHead;//��ͷ �̶�λ��0xFEFF
	DWORD nLength;//���ĳ��ȣ��ӿ������ʼ������У�������
	WORD sCmd;//��������
	std::string strData;//��������
	WORD sSum;//��У��
	std::string strOut;//������������
	CPacket();
	CPacket& operator=(const CPacket& pack);
	~CPacket() {};
	CPacket(WORD sCmd, const BYTE* pData, size_t nSize);
	CPacket(const BYTE* pData, size_t& nSize);
	CPacket(const CPacket& pack);
	int pacSize();//�����ݵĴ�С
	const char* pacData();//�������ݵ�����
};
#pragma pack(pop)
class CClientSocket
{
private:
	//��Ա����
	CClientSocket& operator=(const CClientSocket& ss) {};
	CClientSocket(const CClientSocket& ss);
	CClientSocket();
	~CClientSocket();
	BOOL InitSockEnv();
	static void releaseInstance();
	//Ƕ����
	class CNewAndDel;
	//��Ա����
	CPacket m_packet;
	SOCKET m_sock;
	static CClientSocket* m_instance;
	static CNewAndDel m_newdel;
	std::vector<char>m_buffer;
public:
	bool initSocket(int nIP, int nPort);
	int dealCommand();
	bool sendCom(CPacket& pData);
	static CClientSocket* getInstance();
	bool getFilePath(std::string& strPath);
	bool getMouseEvent(MOUSEEV& mouse);
	CPacket& GetPacket();
	void CloseSocket();
	void dump(BYTE* pData, size_t nSize);
};

extern CClientSocket* gpClient;

