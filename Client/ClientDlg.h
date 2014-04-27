
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "Com_class.h"
#include "io.h"
#include "MySock.h"

// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��
	LRESULT On_Receive(WPARAM wp, LPARAM lp); // ���մ��ڵ���Ϣ��������

	void OnConnected(); // ����Socket������
	void OnRvc(); // ���� Socket ����
	void OnClose(); // �ر� Socket ����
	CMySock m_Conn; // 

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	struct CONF{
		char title[32]; // ����
		char ip[16]; // ��ַ
		short port; // �˿�
		short com1_id; // ����1�ı��
		char com1_para[64]; // ����1�Ĳ���
		short com2_id; // ����2�ı��
		char com2_para[64]; // ����2�Ĳ���
		char cookie[256]; // Cookie
		char sid[256]; // �ỰID 
		char aid[256]; // ���ID
	};
	CONF conf;
	int len;
	int m_post_id; // �ύ�ı�ţ��������ֲ�ͬ�ύ��
	char m_net_rvc_data[1025]; // ���յ�������
	int m_net_rvc_len; // ���յ������ݳ���

// ʵ��
protected:
	HICON m_hIcon;
	_thread_com com1; // ��������1
	_thread_com com2; // ��������2
	CString	m_e;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboHuowu();
	afx_msg void OnBnClickedButtonCom();
	afx_msg void OnBnClickedButtonCom1Send();
	afx_msg void OnBnClickedButtonCom2Send();

	void GetData(char *url, char *para);
	void PostData(char *url, char *para);
	CEdit m_ip;
	CEdit m_port;
	CEdit m_com1;
	CEdit m_com2;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_btn_net;
	afx_msg void OnBnClickedButtonNetConn();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLogout();
	CEdit m_user;
	CEdit m_pwd;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonTijiao();
	CEdit m_id;
	CButton m_fangxing;
	afx_msg void OnBnClickedButtonFangxing();
	CEdit m_chehao;
	CEdit m_dianhua;
	CEdit m_shouhuo;
	CEdit m_huowu;
	CEdit m_guige;
	CEdit m_liuxiang;
	CEdit m_chexing;
	CEdit m_pizhong;
	CEdit m_maozhong;
	CEdit m_jingzhong;
	CEdit m_danjia;
	CEdit m_jine;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonComConn();
	CButton m_btn_login;
	void OnKeepalive(); // ��������
	void OnLogin(); // �û���¼
	void OnPost(); // �ύ����
};
