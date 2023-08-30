#pragma once
#include "HuxlThread.h"
#include "CEdoyunQueue.h"
#include <MSWSock.h>
#include <map>


enum HuxlOperator{
    ENone,
    EAccept,
    ERecv,
    ESend,
    EError
};

class HuxlServer;
class HuxlClient;
typedef std::shared_ptr<HuxlClient> PCLIENT;

class HuxlOverlapped {
public:
    OVERLAPPED m_overlapped;
    DWORD m_operator;//���� �μ�HuxlOperator
    std::vector<char> m_buffer;//������
    ThreadWorker m_worker;//��������
    HuxlServer* m_server;//����������
    PCLIENT m_client;//��Ӧ�Ŀͻ���
    WSABUF m_wsabuffer;


};

template<HuxlOperator>class AcceptOverlapped;
typedef AcceptOverlapped<EAccept> ACCEPTOVERLAPPED;
template<HuxlOperator>class RecvOverlapped;
typedef RecvOverlapped<ERecv> RECVOVERLAPPED;
template<HuxlOperator>class SendOverlapped;
typedef SendOverlapped<ESend> SENDOVERLAPPED;


class HuxlClient {
public:
    HuxlClient();

    ~HuxlClient() {
        closesocket(m_sock);
    }

    void SetOverlapped(PCLIENT& ptr);
    operator SOCKET() {
        return m_sock;
    }

    operator PVOID() {
        return &m_buffer[0];
    }

    operator LPOVERLAPPED();

    operator LPDWORD() {
        return &m_received;
    }

    LPWSABUF RecvWSABuffer();

    LPWSABUF SendWSABuffer();
    DWORD& flags() { return m_flags; }
    sockaddr_in* GetLocalAddr()  { return &m_laddr; }
    sockaddr_in* GetRemoteAddr() { return &m_raddr; }
    size_t GetBufferSize() const { return m_buffer.size(); }

    int Recv() {
        int ret=recv(m_sock, m_buffer.data()+m_used, m_buffer.size()-m_used, 0);
        if (ret <= 0)return -1;
        m_used += (size_t)ret;
        //TODO:��������
        return 0;
    }
private:
    SOCKET m_sock;
    DWORD m_received;
    DWORD m_flags;
    std::shared_ptr<ACCEPTOVERLAPPED> m_overlapped;
    std::shared_ptr<RECVOVERLAPPED>m_recv;
    std::shared_ptr<SENDOVERLAPPED>m_send;
    std::vector<char> m_buffer;
    size_t m_used;//�Ѿ�ʹ�õĻ�������С
    sockaddr_in m_laddr;
    sockaddr_in m_raddr;
    bool m_isbusy;
};

template<HuxlOperator>
class AcceptOverlapped :public HuxlOverlapped,ThreadFuncBase
{
public:
    AcceptOverlapped();
    int AcceptWorker();
    PCLIENT m_client;
};


template<HuxlOperator>
class RecvOverlapped :public HuxlOverlapped, ThreadFuncBase
{
public:
    RecvOverlapped();
    int RecvWorker() {
        int ret = m_client->Recv();
        return ret;
    }
};



template<HuxlOperator>
class SendOverlapped :public HuxlOverlapped, ThreadFuncBase
{
public:
    SendOverlapped();
    int SendWorker() {
        //TODO:
        return -1;
    }
};


template<HuxlOperator>
class ErrorOverlapped :public HuxlOverlapped, ThreadFuncBase
{
public:
    ErrorOverlapped() :m_operator(EError), m_worker(this, &ErrorOverlapped::ErrorWorker) {
        memset(&m_overlapped, 0, sizeof(m_overlapped));
        m_buffer.resize(1024);
    }
    int ErrorWorker() {
        //TODO:
        return -1;
    }
};

typedef ErrorOverlapped<EError> ERROROVERLAPPED;




class HuxlServer :
    public ThreadFuncBase
{
public:
    HuxlServer(const std::string& ip="0.0.0.0",short port=9527):m_pool(10){
        m_hIOCP = INVALID_HANDLE_VALUE;
        m_sock = INVALID_SOCKET;
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = htons(port);
        m_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }

    ~HuxlServer(){}

    bool StartService();

    bool NewAccept() {
        PCLIENT pClient(new HuxlClient());//���е����õ��Ķ������ã�һ��shared_ptr
        pClient->SetOverlapped(pClient);
        m_client.insert(std::pair<SOCKET, PCLIENT>(*pClient, pClient));
        if (!AcceptEx(m_sock, *pClient, *pClient, 0,
            sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, *pClient, *pClient)) {
            closesocket(m_sock);
            m_sock = INVALID_SOCKET;
            m_hIOCP = INVALID_HANDLE_VALUE;
            return false;
        }
        return true;
    }

    int AcceptClient() {
    
    };
private:
    void CreateSocket() {
        m_sock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
        int opt = 1;
        setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
    }
    
    int threadIocp();
public:
    std::map<SOCKET, std::shared_ptr<HuxlClient>> m_client;
private:
    HuxlThreadPool m_pool;
    HANDLE m_hIOCP;
    SOCKET m_sock;
    //map����ͻ��ˣ���Ҫά�ֿͻ��˵�״̬���ͻ���Ψһ����socket�׽��֣�ÿ���׽��ֻ�ά��һ������
    //ÿ���ͻ���������׷�ӵ�map����ȥ����Ҫ�Ŀͻ�����Ϣ��������*HuxlClient��
    sockaddr_in m_addr;
    CEdoyunQueue<HuxlClient> m_lstClient;
};



