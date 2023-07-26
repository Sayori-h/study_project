﻿
// RemoteClientDlg.h: 头文件
//

#pragma once
#include "RemoteClient.h"
#include "afxdialogex.h"
#include "CWatchDialog.h"
#include "ClientSocket.h"
#include "StatusDlg.h"

//class CClientController;

//#define WM_SEND_PACKET (WM_USER+1)//发送数据包消息，自定义消息①
// CRemoteClientDlg 对话框
class CRemoteClientDlg : public CDialogEx
{
// 构造
public:
	CRemoteClientDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMOTECLIENT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	
	CImage& GetImage() {
		return m_image;
	}
	
private:
	CImage m_image;//缓存
	bool m_isClosed;//监视是否关闭
	CString GetPath(HTREEITEM hTree);
	void DelTreeChildItem(HTREEITEM hTree);
	void LoadFileInfo();
	void LoadCurInfo();
	//static void threadEntryOfDownFile(void*arg);
	//void threadDownFile();
	//静态函数不能使用this指针
	//static void threadEntryOfWatchData(void*arg);
	////成员函数可以使用this指针
	//void threadWatchData();

// 实现
protected:
	HICON m_hIcon;
	CStatusDlg m_dlgStatus;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTest();
	// 端口号
	CString m_nPort;
	DWORD m_server_address;
	afx_msg void OnBnClickedBtnFileinfo();
	// 对方的文件目录
	CTreeCtrl m_Tree;
	afx_msg void OnNMDblclkTreeDir(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickTreeDir(NMHDR* pNMHDR, LRESULT* pResult);
	// 显示文件
	CListCtrl m_List;
	afx_msg void OnNMRClickListFile(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDownloadFile();
	afx_msg void OnDeleteFile();
	afx_msg void OnRunFile();
	//afx_msg LRESULT OnSendPacket(WPARAM wParam, LPARAM lParam);//自定义消息响应函数②
	afx_msg void OnBnClickedBtnStartWatch();
	afx_msg void OnIpnFieldchangedIpaddressServ(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEditPort();
};
