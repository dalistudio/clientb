
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "strtoken.h"
#include "../cjson/cJSON.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg 对话框




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IP, m_ip);
	DDX_Control(pDX, IDC_EDIT_PORT, m_port);
	DDX_Control(pDX, IDC_EDIT_COM1, m_com1);
	DDX_Control(pDX, IDC_EDIT_COM2, m_com2);
	DDX_Control(pDX, IDC_BUTTON_NET_CONN, m_btn_net);
	DDX_Control(pDX, IDC_EDIT_USER, m_user);
	DDX_Control(pDX, IDC_EDIT_PWD, m_pwd);
	DDX_Control(pDX, IDC_EDIT_DANHAO, m_id);
	DDX_Control(pDX, IDC_BUTTON_FANGXING, m_fangxing);
	DDX_Control(pDX, IDC_EDIT_CHEHAO, m_chehao);
	DDX_Control(pDX, IDC_EDIT_DIANHUA, m_dianhua);
	DDX_Control(pDX, IDC_EDIT_SHOUHUO, m_shouhuo);
	DDX_Control(pDX, IDC_EDIT_HUOWU, m_huowu);
	DDX_Control(pDX, IDC_EDIT_GUIGE, m_guige);
	DDX_Control(pDX, IDC_EDIT_LIUXIANG, m_liuxiang);
	DDX_Control(pDX, IDC_EDIT_CHEXING, m_chexing);
	DDX_Control(pDX, IDC_EDIT_PIZHONG, m_pizhong);
	DDX_Control(pDX, IDC_EDIT_MAOZHONG, m_maozhong);
	DDX_Control(pDX, IDC_EDIT_JINGZHONG, m_jingzhong);
	DDX_Control(pDX, IDC_EDIT_DANJIA, m_danjia);
	DDX_Control(pDX, IDC_EDIT_JINE, m_jine);
	DDX_Control(pDX, IDC_BUTTON_LOGIN, m_btn_login);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(ON_COM_RECEIVE, On_Receive) // 如果收到串口 ON_COM_RECEIVE 消息，调用接收函数
	ON_BN_CLICKED(IDC_BUTTON_COM1_SEND, &CClientDlg::OnBnClickedButtonCom1Send)
	ON_BN_CLICKED(IDC_BUTTON_COM2_SEND, &CClientDlg::OnBnClickedButtonCom2Send)
	ON_BN_CLICKED(IDC_BUTTON_NET_CONN, &CClientDlg::OnBnClickedButtonNetConn)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CClientDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_LOGOUT, &CClientDlg::OnBnClickedButtonLogout)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TIJIAO, &CClientDlg::OnBnClickedButtonTijiao)
	ON_BN_CLICKED(IDC_BUTTON_FANGXING, &CClientDlg::OnBnClickedButtonFangxing)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_COM_CONN, &CClientDlg::OnBnClickedButtonComConn)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_post_id = 0; // 初始化提交ID
	memset(&m_net_rvc_data,0,1025);
	m_net_rvc_len = 0;

#ifdef _DEBUG // 调试版开启
	// 开启命令行窗口 
	int hCrt;
	FILE *hf;
	AllocConsole();
	hCrt = _open_osfhandle(
		(long) GetStdHandle(STD_OUTPUT_HANDLE),
		0x4000
	);
	hf = _fdopen( hCrt, "w" );
	*stdout = *hf;
	int i = setvbuf( stdout, NULL, _IONBF, 0 );
#endif

	// 默认配置
	memset(&conf,0,sizeof(CONF));
	strcpy(conf.title,"海南港口文峰石场");
	strcpy(conf.ip,"192.168.1.5");
	conf.port = 80;
	conf.com1_id=1;
	strcpy(conf.com1_para,"baud=9600 parity=N data=8 stop=1");
	conf.com2_id=2;
	strcpy(conf.com2_para,"baud=9600 parity=N data=8 stop=1");
	strcpy(conf.cookie,"");

	// 打开配置文件 JSON 格式
	FILE *f=fopen("config","rb"); // 配置文件 config
	if(f==NULL)
	{
		MessageBox("打开配置文件失败！", "config", MB_OK);
	}
	fseek(f,0,SEEK_END); // 跳到文件尾
	long len=ftell(f); // 获得文件长度
	fseek(f,0,SEEK_SET); // 跳到文件头
	char *data=(char*)malloc(len+1); // 分配文件空间
	fread(data,1,len,f); // 读取文件数据
	fclose(f); // 关闭文件
	cJSON *jsonroot = cJSON_Parse(data); //json根
	strcpy(conf.title,cJSON_GetObjectItem(jsonroot,"title")->valuestring); // 获得标题

	cJSON *jsonServer=cJSON_GetObjectItem(jsonroot,"server");//取 Server
	strcpy(conf.ip,cJSON_GetObjectItem(jsonServer,"ip")->valuestring); // 获得IP地址
	conf.port = cJSON_GetObjectItem(jsonServer,"port")->valueint; // 获得端口

	cJSON *jsonCOM1=cJSON_GetObjectItem(jsonroot,"com1");//取 COM1
	conf.com1_id = cJSON_GetObjectItem(jsonCOM1,"port")->valueint; // 获得COM端口
	strcpy(conf.com1_para,cJSON_GetObjectItem(jsonCOM1,"para")->valuestring); // 获得COM的属性

	cJSON *jsonCOM2=cJSON_GetObjectItem(jsonroot,"com1");//取 COM1
	conf.com2_id = cJSON_GetObjectItem(jsonCOM2,"port")->valueint; // 获得COM端口
	strcpy(conf.com2_para,cJSON_GetObjectItem(jsonCOM2,"para")->valuestring); // 获得COM的属性

	// 设置定时器
	SetTimer(1,20000,0); // 20秒

	// 连接服务器
	OnBnClickedButtonNetConn();

	// 初始化串口
	OnBnClickedButtonComConn();

	///////////////////////

	CFont *m_Font;
	m_Font = new CFont; 
	m_Font->CreateFont(
		32, // 字体高度
		13,	// 字体宽度
		0,	// 文本行的倾斜度
		0,	// 字符基线的倾斜度
		100,	// 字体的粗细
		FALSE,	// 是否斜体
		FALSE,	// 是否下划线
		0,		// 是否删除线
		ANSI_CHARSET,			// 字体的字符集
		OUT_DEFAULT_PRECIS,		// 字符的输出精度
		CLIP_DEFAULT_PRECIS,	// 字符的裁剪精度
		DEFAULT_QUALITY,		// 字符的输出质量
		FF_SWISS,				// 字符间距和字体族
		"隶书");				// 字体名称

	// 标题
	GetDlgItem(IDC_EDIT_TITLE)->SetWindowText(conf.title);
	GetDlgItem(IDC_EDIT_TITLE)->SetFont(m_Font);
	//////////////////////////////////////

	// 单号
	GetDlgItem(IDC_STATIC_DANHAO)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_DANHAO)->SetFont(m_Font);
	
	// 车号
	GetDlgItem(IDC_STATIC_CHEHAO)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_CHEHAO)->SetFont(m_Font);

	// 电话
	GetDlgItem(IDC_STATIC_DIANHUA)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_DIANHUA)->SetFont(m_Font);

	// 货物名称
	GetDlgItem(IDC_STATIC_HUOWU)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_HUOWU)->SetFont(m_Font);

	// 货物规格
	GetDlgItem(IDC_STATIC_GUIGE)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_GUIGE)->SetFont(m_Font);

	// 收货单位
	GetDlgItem(IDC_STATIC_SHOUHUO)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_SHOUHUO)->SetFont(m_Font);

	// 货物流向
	GetDlgItem(IDC_STATIC_LIUXIANG)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_LIUXIANG)->SetFont(m_Font);


	////////////////////////////////

	// 车型
	GetDlgItem(IDC_STATIC_CHEXING)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_CHEXING)->SetFont(m_Font);

	// 皮重
	GetDlgItem(IDC_STATIC_PIZHONG)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_PIZHONG)->SetFont(m_Font);

	// 毛重
	GetDlgItem(IDC_STATIC_MAOZHONG)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_MAOZHONG)->SetFont(m_Font);

	// 净重
	GetDlgItem(IDC_STATIC_JINGZHONG)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_JINGZHONG)->SetFont(m_Font);

	// 单价
	GetDlgItem(IDC_STATIC_DANJIA)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_DANJIA)->SetFont(m_Font);

	// 金额
	GetDlgItem(IDC_STATIC_JINE)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_JINE)->SetFont(m_Font);

	/////////////////////////////////

	// 提交按钮
	GetDlgItem(IDC_BUTTON_TIJIAO)->SetFont(m_Font);

	// 打印按钮
	GetDlgItem(IDC_BUTTON_DAYIN)->SetFont(m_Font);

	/////////////////////////////////




	//return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
	return FALSE;
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 接收串口的消息，并处理
LRESULT CClientDlg::On_Receive(WPARAM wp, LPARAM lp)
{
	
	int len;
	char str[100];

	if(wp==3)
	{
		len = com1.read(str, 100);
		printf("COM1: %s\n",str);
	}

	if(wp==9)
	{
		len = com2.read(str, 100);
		printf("COM2: %s\n",str);
	}
	if(len > 0)
	{
		char com_str[10];
		strcpy(com_str, "COM");
		ltoa((long)wp, com_str + 3, 10); //	WPARAM 保存端口号
		m_e += com_str;
		m_e += " : ";
		m_e += str;
		m_e += "\r\n";
		GetDlgItem(IDC_EDIT_INFO)->SetWindowText(m_e);
	}
	
	return 0;
}


void CClientDlg::OnBnClickedButtonCom1Send()
{
	// TODO: 在此添加控件通知处理程序代码
//	com1.write("COM1-test");
	CString strEdit("");
	GetDlgItem(IDC_EDIT_CHEHAO)->GetWindowText(strEdit);
	if (strEdit.IsEmpty())
	{
		return;
	}
	char SendBuf[512] = {0};
	strcpy(SendBuf,strEdit);
	m_Conn.Send(SendBuf,strlen(SendBuf));
}


void CClientDlg::OnBnClickedButtonCom2Send()
{
	// TODO: 在此添加控件通知处理程序代码
	com2.write("COM2-test");
}

// 连接网络
void CClientDlg::OnConnected()
{
	m_ip.SetWindowText(conf.ip);
	char tmp[16]={0};
	sprintf(tmp,"%d",conf.port);
	m_port.SetWindowText(tmp);
	printf("连接服务器成功\n");
	m_btn_net.SetWindowText("连接成功");
	m_btn_net.EnableWindow(FALSE);
	//MessageBox("连接成功");
}

// 接收网络数据
void CClientDlg::OnRvc()
{
	//m_post_id = 1 为保持连接请求
	//m_post_id = 2 为用户登录请求
	//m_post_id = 3 为提交单据请求

	memset(&m_net_rvc_data,0,1025);
	m_net_rvc_len = m_Conn.Receive(m_net_rvc_data,1024);
	if (SOCKET_ERROR != m_net_rvc_len)
	{
		printf("%s\n",m_net_rvc_data);
		GetDlgItem(IDC_EDIT_INFO)->SetWindowText(m_net_rvc_data);
		switch(m_post_id)
		{
		case 1:
			OnKeepalive();
			break;
		case 2:
			OnLogin();
			break;
		case 3:
			OnPost();
			break;
		}
	}
}

// 关闭网络
void CClientDlg::OnClose()
{
	m_ip.SetWindowText("");
	m_port.SetWindowText("");
	m_btn_net.SetWindowText("重新连接(&R)...");
	m_btn_net.EnableWindow(TRUE); // 连接按钮
	m_Conn.Close();
	//MessageBox("连接已关闭！\n请重新连接服务器。","网络连接");
	// 自动重新连接
	Sleep(3000); // 休息 3秒重连
	OnBnClickedButtonNetConn();
}

void CClientDlg::GetData(char *url, char *para)
{
	char Data[1024] = {0};
	strcat(Data,"GET ");
	strcat(Data,url);
	strcat(Data,para);
	strcat(Data," HTTP/1.1\r\n");
	strcat(Data,"Host: ");
	strcat(Data,conf.ip);
	strcat(Data,"\r\n");
	strcat(Data,"Connection: Keep-Alive\r\n");
	strcat(Data,"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n");
	strcat(Data,"User-Agent: Mozilla/5.0 (Windows Client; Dali Wang<wangdali@qq.com>)\r\n");
	strcat(Data,"Accept-Language: zh-CN,zh;q=0.8\r\n");
	strcat(Data,"Cookie: ");
	strcat(Data,conf.cookie);
	strcat(Data,"\r\n");
	strcat(Data,"\r\n");
	m_Conn.Send(Data,strlen(Data));
}

void CClientDlg::PostData(char *url, char *para)
{
	char Data[1024] = {0};
	strcat(Data,"POST ");
	strcat(Data,url);
	strcat(Data," HTTP/1.1\r\n");
	strcat(Data,"Host: ");
	strcat(Data,conf.ip);
	strcat(Data,"\r\n");
	strcat(Data,"Connection: Keep-Alive\r\n");
	strcat(Data,"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n");
	strcat(Data,"User-Agent: Mozilla/5.0 (Windows Client; Dali Wang<wangdali@qq.com>)\r\n");
	strcat(Data,"Accept-Language: zh-CN,zh;q=0.8\r\n");
	strcat(Data,"Cookie: ");
	strcat(Data,conf.cookie);
	strcat(Data,"\r\n");
	strcat(Data,"Content-Type: application/x-www-form-urlencoded\r\n");
	strcat(Data,"Content-Length: ");
	char tmp[16] = {0};
	sprintf(tmp,"%d",strlen(para));
	strcat(Data,tmp);
	strcat(Data,"\r\n");
	strcat(Data,"\r\n");
	strcat(Data,para);
	m_Conn.Send(Data,strlen(Data));
}

BOOL CClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN: // 回车按键
			//MessageBox("回车");
			//if(GetFocus()->GetDlgCtrlID() != IDOK)
			//{
			//	pMsg->wParam = VK_TAB;
			//}
			CWnd *wnd =GetFocus();
			if(wnd != NULL)
			{
				char str[256];
				CString ClassName = "Button";
				GetClassName(wnd->m_hWnd,str,256);
				UINT i = wnd->GetDlgCtrlID();
				if(ClassName == str)
				{
					SendMessage(WM_COMMAND,i,(LPARAM)wnd->m_hWnd); // 发送按钮消息
				}
				if(i == IDC_EDIT_DANHAO)
				{
					SendMessage(WM_COMMAND,IDC_BUTTON_TIJIAO,(LPARAM)wnd->m_hWnd); // 发送按钮消息
				}
				if(i == IDC_BUTTON_FANGXING)
				{
					return TRUE;
				}
					pMsg->wParam = VK_TAB; // 跳到下一个控件
			}
			break;
//		case VK_ESCAPE: // ESC 按键
//			break;
		}
	}

	if(pMsg->message==WM_SYSKEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_F4: // ALT + F4按键 通常用来退出程序
			MessageBox("F4");
			return TRUE; // 跳出，不执行退出程序
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

// 连接服务器
void CClientDlg::OnBnClickedButtonNetConn()
{
	// TODO: 在此添加控件通知处理程序代码
	// 初始化Socket
	m_Conn.SetDlg(this);
	if (m_Conn.Create())
	{
		printf("创建Socket成功\n");
	}
	else
	{
		MessageBox("创建Socket失败！");
	}
	m_Conn.Connect(conf.ip,conf.port);

}

// 登录
void CClientDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	m_post_id = 2; // 提交ID为2
	char Data[256] ={0};
	strcat(Data,"?name=");
	CString strUser ="";
	m_user.GetWindowText(strUser);
	strcat(Data,strUser.GetBuffer());
	strcat(Data,"&pwd=");
	CString strPwd = "";
	m_pwd.GetWindowText(strPwd);
	strcat(Data,strPwd.GetBuffer());
	GetData("/login.php",(char*)&Data);
}

// 登出
void CClientDlg::OnBnClickedButtonLogout()
{
	// TODO: 在此添加控件通知处理程序代码
	m_user.SetWindowText("");
	m_user.EnableWindow(TRUE);
	m_pwd.SetWindowText("");
	m_pwd.EnableWindow(TRUE);
	m_btn_login.EnableWindow(TRUE);
	memset(&(conf.cookie),0,256);
}

// 定时器
void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		m_post_id = 1; // 提交ID为1
		GetData("/keepalive.php","");
		return;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// 提交按钮
void CClientDlg::OnBnClickedButtonTijiao()
{
	// TODO: 在此添加控件通知处理程序代码
	m_post_id = 3; // 提交ID为3
	PostData("/post.php",""); // 提交放行请求
}

// 放行按钮
void CClientDlg::OnBnClickedButtonFangxing()
{
	// TODO: 在此添加控件通知处理程序代码
	char buf[64]={0};
	buf[0] = 0xAA;
	buf[1] = 0xAA;
	buf[2] = 0x01;
	buf[3] = 0xBB;
	buf[4] = 0xCC;
	buf[5] = 0xDD;
	int len = strlen(buf);
	com1.write((char*)&buf,len); // 发送开门信号到串口1

	// 清空文本框的数据，并将焦点设置在单号处。
	m_id.SetWindowText(""); // 单号
	m_chehao.SetWindowText(""); // 车号
	m_dianhua.SetWindowText(""); // 电话
	m_shouhuo.SetWindowText(""); // 收货单位
	m_huowu.SetWindowText(""); // 货物名称
	m_guige.SetWindowText(""); // 货物规格
	m_liuxiang.SetWindowText(""); // 货物流向
	m_chexing.SetWindowText(""); // 车型
	m_pizhong.SetWindowText(""); // 皮重
	m_maozhong.SetWindowText(""); // 毛重
	m_jingzhong.SetWindowText(""); // 净重
	m_danjia.SetWindowText(""); // 单价
	m_jine.SetWindowText(""); // 金额
	// 设置单号为焦点
	m_id.SetFocus();
}


HBRUSH CClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
//	if(nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
//	{
		//pDC->SetTextColor(RGB(0,0,255));//字体色
//		pDC->SetBkColor(RGB(192,192,192));//字体背景色******
//		HBRUSH B = CreateSolidBrush(RGB(192,192,192)); //背景色
//		return B;
//	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

// 重新打开串口
void CClientDlg::OnBnClickedButtonComConn()
{
	// TODO: 在此添加控件通知处理程序代码
	// 初始化串口
	if(!com1.open(conf.com1_id,conf.com1_para))
	{
		m_com1.SetWindowText("打开失败");
//		MessageBox("串口1打开失败", "串口", MB_OK);
	}
	else
	{
		com1.set_hwnd(m_hWnd);
		char tmp[64] = {0};
		sprintf(tmp,"COM%d:%s",conf.com1_id,conf.com1_para);
		m_com1.SetWindowText(tmp);
	}

	if(!com2.open(conf.com2_id,conf.com2_para))
	{
		m_com2.SetWindowText("打开失败");
//		MessageBox("串口2打开失败", "串口", MB_OK);
	}
	else
	{
		com2.set_hwnd(m_hWnd);
		char tmp[64] = {0};
		sprintf(tmp,"COM%d,%s",conf.com2_id,conf.com2_para);
		m_com2.SetWindowText(tmp);
	}
}

// 保持连接
void CClientDlg::OnKeepalive()
{
	// 无需处理返回的数据。
}

// 用户登录
void CClientDlg::OnLogin()
{
	// 如果返回 200 和 SetCookie ，则禁用登录相关控件
	TOKEN * t = (TOKEN *)malloc(sizeof(TOKEN));
	TOKEN * l = (TOKEN *)malloc(sizeof(TOKEN));
	TOKEN * c = (TOKEN *)malloc(sizeof(TOKEN));
	char *tStr ,*lStr, *cStr;
	tStr = strtoken(t,(char*)&m_net_rvc_data,"\r\n");
	lStr = strtoken(l,(char*)tStr," "); // HTTP/1.1
	lStr = strtoken(l,NULL," "); // 200 
	if(strcmp(lStr,"200")==0)
	{
		while(t->data!=NULL)
		{
			tStr = strtoken(t,NULL, "\r\n");
			cStr = strtoken(c,tStr,":");
			if(strcmp(cStr,"Set-Cookie")==0)
			{
				cStr = strtoken(c,NULL,":");
				strcpy(conf.cookie,cStr);
				m_user.EnableWindow(FALSE);
				m_pwd.EnableWindow(FALSE);
				m_btn_login.EnableWindow(FALSE);
			}
		}
		
	}
}

// 提交单据
void CClientDlg::OnPost()
{
	// 分析返回数据，并显示出来。
	TOKEN * t = (TOKEN *)malloc(sizeof(TOKEN));
	TOKEN * l = (TOKEN *)malloc(sizeof(TOKEN));

	char *tStr ,*lStr, *cStr;
	tStr = strtoken(t,(char*)&m_net_rvc_data,"\r\n"); // 响应行
	lStr = strtoken(l,(char*)tStr," "); // HTTP/1.1
	lStr = strtoken(l,NULL," "); // 200 
	if(strcmp(lStr,"200")==0)
	{
		while(t->data!=NULL)
		{
			tStr = strtoken(t,NULL, "\r\n");
			if(strcmp(tStr,"")==0) // 空行
			{
				tStr = strtoken(t,NULL, "\r\n"); // 得到相应体长度
				tStr = strtoken(t,NULL, "\r\n"); // 得到相应体内容
				break;
			}
		}
		cJSON *jsonroot = cJSON_Parse(tStr); //json根
		m_chehao.SetWindowText(cJSON_GetObjectItem(jsonroot,"ch")->valuestring);
		m_dianhua.SetWindowText(cJSON_GetObjectItem(jsonroot,"dh")->valuestring);
		m_shouhuo.SetWindowText(cJSON_GetObjectItem(jsonroot,"dw")->valuestring);
		m_huowu.SetWindowText(cJSON_GetObjectItem(jsonroot,"hw")->valuestring);
		m_guige.SetWindowText(cJSON_GetObjectItem(jsonroot,"gg")->valuestring);
		m_liuxiang.SetWindowText(cJSON_GetObjectItem(jsonroot,"lx")->valuestring);
		m_chexing.SetWindowText(cJSON_GetObjectItem(jsonroot,"cx")->valuestring);
		m_pizhong.SetWindowText(cJSON_GetObjectItem(jsonroot,"pz")->valuestring);
		m_maozhong.SetWindowText(cJSON_GetObjectItem(jsonroot,"mz")->valuestring);
		m_jingzhong.SetWindowText(cJSON_GetObjectItem(jsonroot,"jz")->valuestring);
		m_danjia.SetWindowText(cJSON_GetObjectItem(jsonroot,"dj")->valuestring);
		m_jine.SetWindowText(cJSON_GetObjectItem(jsonroot,"je")->valuestring);
		
	}
	if(strcmp(lStr,"400")==0)
		MessageBox("文件没有找到！","网络连接");
	if(strcmp(lStr,"500")==0)
		MessageBox("服务器错误！！！","网络连接");

}