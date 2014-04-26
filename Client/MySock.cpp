// MySock.cpp : 实现文件
//

#include "stdafx.h"
#include "MySock.h"
#include "Client.h"
#include "ClientDlg.h"


// CMySock

CMySock::CMySock()
{
	m_pDlg = NULL;
}

CMySock::~CMySock()
{
	m_pDlg = NULL;
}

// CMySock 成员函数
// 连接服务器
void CMySock::OnConnect(int nErrorCode)
{
	if (0 == nErrorCode)
	{
		m_pDlg->OnConnected();
	}
	CAsyncSocket::OnConnect(nErrorCode);
}

// 接收数据
void CMySock::OnReceive(int nErrorCode)
{
	if (0 == nErrorCode)
	{
		m_pDlg->OnRvc();
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

// 发送数据
void CMySock::OnClose(int nErrorCode)
{
	if ( 0 == nErrorCode)
	{
		m_pDlg->OnClose();
	}
	CAsyncSocket::OnClose(nErrorCode);
}

void CMySock::SetDlg(CClientDlg* pDlg)
{
	if (NULL != pDlg)
	{
		m_pDlg = pDlg;
	}
}