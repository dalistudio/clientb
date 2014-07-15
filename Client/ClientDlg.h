
// ClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "Com_class.h"
#include "io.h"
#include "../curl/include/curl/curl.h"

// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数
	LRESULT On_Receive(WPARAM wp, LPARAM lp); // 接收串口的消息，并处理

// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	struct CONF{
		char title[32]; // 标题
		char ip[16]; // 地址
		short port; // 端口
		short com1_id; // 串口1的编号
		char com1_para[64]; // 串口1的参数
	};
	CONF conf;
	int m_isLogin; // 是否登陆

// 实现
protected:
	HICON m_hIcon;
	_thread_com com1; // 创建串口1

	// 生成的消息映射函数
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
	void OnFangXing(); // 放行处理
	CList<CString,CString> m_UserList; // 用户名列表

	// CURL 
	CURL *curl;
	static size_t login_data(void *ptr, size_t size, size_t nmemb, void *userp); // 登陆返回数据
	static size_t getid_data(void *ptr, size_t size, size_t nmemb, void *userp); // 获得单号返回数据
	static size_t post_data(void *ptr, size_t size, size_t nmemb, void *userp); // 提交数据返回数据
	static size_t card_data(void *ptr, size_t size, size_t nmemb, void *userp); // 门禁卡返回数据
};
