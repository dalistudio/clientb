#ifndef __MYSOCK_H__
#define __MYSOCK_H__
#pragma once

class CClientDlg;
// CMySock ÃüÁîÄ¿±ê

class CMySock : public CAsyncSocket
{
public:
	CMySock();
	virtual ~CMySock();
	void SetDlg(CClientDlg* pDlg);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	
private:
	CClientDlg* m_pDlg;
};
#endif

