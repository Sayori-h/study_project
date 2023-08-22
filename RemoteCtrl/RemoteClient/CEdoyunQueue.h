#pragma once

template<class T>
class CEdoyunQueue
{//�̰߳�ȫ�Ķ��У�����IOCPʵ��
public:
	CEdoyunQueue();
	~CEdoyunQueue();
	bool PushBack(const T& data);
	bool PopFront(T& data);
	size_t Size();
	void Clear();
private:
	static void threadEntry(void* arg);
	void threadMain();
private:
	std::list<T> m_lstData;
	HANDLE m_hCompeletionPort;
	HANDLE m hThread;
public:
	typedef struct IocpParam {
		int nOperator;//����
		T strData;//����
		_beginthread_proc_type cbFunc;//�ص�
		HANDLE hEvent;//pop������Ҫ��
		IocpParam(int op, const char* sData, _beginthread_proc_type cb = NULL) {
			nOperator = op;
			strData = sData;
			cbFunc = cb;
		}
		IocpParam() {
			nOperator = -1;
		}
	}PPARAM;//Post Parameter ����Ͷ����Ϣ�Ľṹ��
	enum {
		EQPush,
		EQPop,
		EQSize,
		EQClear
	};
};
