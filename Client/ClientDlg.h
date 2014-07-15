
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "Com_class.h"
#include "io.h"
#include "../curl/include/curl/curl.h"

// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��
	LRESULT On_Receive(WPARAM wp, LPARAM lp); // ���մ��ڵ���Ϣ��������

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
	};
	CONF conf;
	int m_isLogin; // �Ƿ��½

// ʵ��
protected:
	HICON m_hIcon;
	_thread_com com1; // ��������1

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboHuowu();
	afx_msg void OnBnClickedButtonCom();

	CEdit m_ip;
	CEdit m_port;
	CEdit m_com1;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonLogout();
	CComboBox m_user;
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
	void OnFangXing(); // ���д���
	CList<CString,CString> m_UserList; // �û����б�

	// CURL 
	CURL *curl;
	static size_t login_data(void *ptr, size_t size, size_t nmemb, void *userp); // ��½��������
	static size_t getid_data(void *ptr, size_t size, size_t nmemb, void *userp); // ��õ��ŷ�������
	static size_t post_data(void *ptr, size_t size, size_t nmemb, void *userp); // �ύ���ݷ�������
	static size_t card_data(void *ptr, size_t size, size_t nmemb, void *userp); // �Ž�����������
};
