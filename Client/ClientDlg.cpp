
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "strtoken.h"
#include "uni2utf8.h"
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
//	conf.com2_id=2;
//	strcpy(conf.com2_para,"baud=9600 parity=N data=8 stop=1");
//	strcpy(conf.cookie,"");

	// 打开配置文件 JSON 格式
	FILE *f=fopen("config","rb"); // 配置文件 config
	if(f==NULL)
	{
		MessageBox(_T("打开配置文件失败！"), _T("config"), MB_OK);
	}
	else
	{
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

//		cJSON *jsonCOM2=cJSON_GetObjectItem(jsonroot,"com1");//取 COM1
//		conf.com2_id = cJSON_GetObjectItem(jsonCOM2,"port")->valueint; // 获得COM端口
//		strcpy(conf.com2_para,cJSON_GetObjectItem(jsonCOM2,"para")->valuestring); // 获得COM的属性
	}
	

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
		_T("隶书"));				// 字体名称

	// 公司LOGO
	GetDlgItem(IDC_STATIC_LOGO)->SetFont(m_Font);

	// 标题
	USES_CONVERSION;  // dali
	GetDlgItem(IDC_EDIT_TITLE)->SetWindowText(A2CW(conf.title));
	GetDlgItem(IDC_EDIT_TITLE)->SetFont(m_Font);
	//////////////////////////////////////

	// 单号
	GetDlgItem(IDC_STATIC_DANHAO)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_DANHAO)->SetFont(m_Font);
	m_id.EnableWindow(FALSE); // 禁用“单号”文本框
	
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

	// 放行按钮
	GetDlgItem(IDC_BUTTON_FANGXING)->SetFont(m_Font);
	m_fangxing.EnableWindow(FALSE); // 禁用放行按钮

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
	// 门岗客户端，一般情况不需要处理闸口发过来的信息
	// 格式：地址(1字节) + 命令(1字节) + 长度(1字节) + 数据(n字节) + 校验(1字节)
	//       地址：00h 广播地址		01h - 0Fh 设备地址
	//       命令：
	//            00h 查询状态(不可广播) 03000003 返回 03000003(未知)  0309000A(开)    030C000F(关)
	//            01h 停止 05010004 返回 05010004
	//            03h 开闸 05030006 返回 05030006 广播 00030003 无返回
	//            05h 关闸 0905000C 返回 0905000C 广播 00050005 无返回
	//            09h 查计数 0A090003 返回 0A0902030406 其中 0304为计数
	//            0Fh 清计数 0F0F0000 返回 0F0F0000
	//
	//       长度：默认长度为0，仅查计数的返回值有长度为2
	//       数据：默认不存在，仅查计数的返回值有2字节的数据。
	//       校验：将前面的所有字节进行异或运算(XOR)。
	//

	// 无需处理接收到的串口数据


	return 0;
}


void CClientDlg::OnBnClickedButtonCom1Send()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CClientDlg::OnBnClickedButtonCom2Send()
{
	// TODO: 在此添加控件通知处理程序代码
}

// 连接网络
void CClientDlg::OnConnected()
{
	USES_CONVERSION;  // dali
	m_ip.SetWindowText(A2CW(conf.ip));
	char tmp[16]={0};
	sprintf(tmp,"%d",conf.port);
	m_port.SetWindowText(A2CW(tmp));
	printf("连接服务器成功\n");
	m_btn_net.SetWindowText(_T("连接成功"));
	m_btn_net.EnableWindow(FALSE);
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
		USES_CONVERSION;  // dali
		GetDlgItem(IDC_EDIT_INFO)->SetWindowText(A2CW(m_net_rvc_data));
		switch(m_post_id)
		{
		case 1:
			OnKeepalive(); // 保持连接
			break;
		case 2:
			OnLogin(); // 登录服务器
			break;
		case 3:
			OnPost(); // 获得单据信息
			break;
		case 4: 
			OnFangXing(); // 提交放行状态
			break;
		}
	}
}

// 关闭网络
void CClientDlg::OnClose()
{
	m_ip.SetWindowText(_T(""));
	m_port.SetWindowText(_T(""));
	m_btn_net.SetWindowText(_T("重新连接(&R)..."));
	m_btn_net.EnableWindow(TRUE); // 连接按钮
	m_Conn.Close();

	// 自动重新连接
	Sleep(3000); // 休息 3秒重连
	OnBnClickedButtonNetConn();
}

void CClientDlg::GetData(char *url, char *para)
{
	char Data[1024] = {0};
	strcat(Data,"GET ");
	strcat(Data,url);
	strcat(Data,"?"); // 注意这里，应该先判断para是否为空
	strcat(Data,para);
	strcat(Data," HTTP/1.1\r\n");
	strcat(Data,"Host: ");
	strcat(Data,conf.ip);
	strcat(Data,"\r\n");
	strcat(Data,"Connection: Keep-Alive\r\n");
	strcat(Data,"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n");
	strcat(Data,"User-Agent: Mozilla/5.0 (Windows Client; Dali Wang<wangdali@qq.com>)\r\n");
	strcat(Data,"Accept-Language: zh-CN,zh;q=0.8\r\n");
	if(strcmp(conf.cookie,"")!=0)
	{
		strcat(Data,"Cookie: ");
		strcat(Data,conf.cookie);
		strcat(Data,"\r\n");
	}
	strcat(Data,"\r\n");
	m_Conn.Send(Data,strlen(Data));
	printf("%s\n",Data);
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
	if(strcmp(conf.cookie,"")!=0)
	{
		strcat(Data,"Cookie: ");
		strcat(Data,conf.cookie);
		strcat(Data,"\r\n");
	}
	strcat(Data,"Content-Type: application/x-www-form-urlencoded\r\n");
	strcat(Data,"Content-Length: ");
	char tmp[16] = {0};
	sprintf(tmp,"%d",strlen(para));
	strcat(Data,tmp);
	strcat(Data,"\r\n");
	strcat(Data,"\r\n");
	strcat(Data,para);
	m_Conn.Send(Data,strlen(Data));
	printf("%s\n",Data);
}

BOOL CClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN: // 回车按键
			CWnd *wnd =GetFocus();
			if(wnd != NULL)
			{
				TCHAR str[256];
				CString ClassName = _T("Button");
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
				if(i == IDC_BUTTON_FANGXING) // 放行按钮消息
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
			MessageBox(_T("F4"));
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
		MessageBox(_T("创建Socket失败！"));
	}
	USES_CONVERSION;  // dali
	m_Conn.Connect(A2CW(conf.ip),conf.port);

}

// 登录
void CClientDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION; // dali
	m_post_id = 2; // 提交ID为2
	char Data[256] ={0};
	strcat(Data,"User=");
	CString strUser =_T("");
	m_user.GetWindowText(strUser); // 获得用户名
	strcat(Data,W2A(strUser)); // 设置用户名
	strcat(Data,"&Passwd=");
	CString strPwd = _T("");
	m_pwd.GetWindowText(strPwd); // 获得密码
	strcat(Data,W2A(strPwd)); // 设置密码
	strcat(Data,"&Level=2"); // 这里Level=2
	GetData("/login.php",(char*)&Data); // 发送 GET 请求
}

// 登出
void CClientDlg::OnBnClickedButtonLogout()
{
	// TODO: 在此添加控件通知处理程序代码
	m_user.SetWindowText(_T("")); // 清空用户名框
	m_user.EnableWindow(TRUE); // 启用用户名框
	m_pwd.SetWindowText(_T("")); // 清空密码框
	m_pwd.EnableWindow(TRUE); // 启用密码框
	m_btn_login.EnableWindow(TRUE); // 启用登录按钮
	m_id.EnableWindow(FALSE); // 禁用“单号”文本框

	memset(&(conf.cookie),0,256); // 清空Cookie
	memset(&(conf.sid),0,256); // 清空SID
	memset(&(conf.aid),0,256); // 清空AID
}

// 定时器
void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		m_post_id = 1; // 提交ID为1
		GetData("/keepalive.php",""); // 发送 GET 请求，保持连接
		return;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// 提交按钮
void CClientDlg::OnBnClickedButtonTijiao()
{
	// TODO: 在此添加控件通知处理程序代码
	SYSTEMTIME st;
	GetLocalTime(&st);
	char str[32]={0};
	sprintf(str,"date=%4d%02d%02d&id=",st.wYear,st.wMonth,st.wDay); // 当前日期

	CString strId;
	m_id.GetWindowText(strId); // 获取单号
	int i = _ttoi(strId); // 转换单号为数字类型
	if(strId=="" || i==0) // 如果单号为空，或者等于0
	{
		MessageBox(_T("单号不能为空！！\n 或者全为：0"));
		m_fangxing.EnableWindow(FALSE); // 放行按钮
		return;
	}
	if(i<100000) // 单号小于10万，保留给内部使用
	{
		// 内部使用
		// 这里不通过数据库，直接放行。
		// 00000 - 99999
		m_chehao.SetWindowText(_T("内部人员"));
		m_chexing.SetWindowText(_T("内部人员"));
		m_dianhua.SetWindowText(_T("内部人员"));
		m_shouhuo.SetWindowText(_T("内部人员"));
		m_huowu.SetWindowText(_T("内部人员"));
		m_guige.SetWindowText(_T("内部人员"));
	}
	else if(i>=100000) // 正常交易的单号
	{
		m_post_id = 3; // 提交ID为3
		USES_CONVERSION; // dali
		strcat(str,W2A(strId)); // 设置单号
		GetData("/getid.php",str); // 提交放行请求
	}

	// 判断状态和上次放行时间决定是否可放行
	m_fangxing.EnableWindow(TRUE); // 开启放行按钮
}

// 放行按钮
void CClientDlg::OnBnClickedButtonFangxing()
{
	// TODO: 在此添加控件通知处理程序代码

	// 提交一个POST放行
	// 需要提交门岗放行的状态，只需ID和保安用户名即可。
	m_post_id = 4; // 提交ID为4
	PostData("/post.php",""); // 提交id level status更新数据库即可
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
	if(!com1.open(conf.com1_id,conf.com1_para)) // 打开串口1
	{
		m_com1.SetWindowText(_T("打开失败"));
	}
	else
	{
		com1.set_hwnd(m_hWnd);
		char tmp[64] = {0};
		sprintf(tmp,"COM%d:%s",conf.com1_id,conf.com1_para);
		USES_CONVERSION;  // dali
		m_com1.SetWindowText(A2CW(tmp)); // 设置串口1框的值
	}

//	if(!com2.open(conf.com2_id,conf.com2_para)) // 打开串口2
//	{
//		m_com2.SetWindowText(_T("打开失败"));
//	}
//	else
//	{
//		com2.set_hwnd(m_hWnd);
//		char tmp[64] = {0};
//		sprintf(tmp,"COM%d,%s",conf.com2_id,conf.com2_para);
//		USES_CONVERSION;  // dali
//		m_com2.SetWindowText(A2CW(tmp)); // 设置串口2框的值
//	}
}

// 保持连接
void CClientDlg::OnKeepalive()
{
	// 无需处理返回的数据。
	// 可以在这里判断连接是否正常
}

// 用户登录
void CClientDlg::OnLogin()
{
	// 如果返回 200 和 SetCookie ，则禁用登录相关控件
	TOKEN * t = (TOKEN *)malloc(sizeof(TOKEN));
	TOKEN * l = (TOKEN *)malloc(sizeof(TOKEN));
	TOKEN * c = (TOKEN *)malloc(sizeof(TOKEN));
	TOKEN * a = (TOKEN *)malloc(sizeof(TOKEN));
	char *tStr ,*lStr, *cStr, *aStr;
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
				// 这里应该判断是否存在“AID”身份标识，
				// 存在则登录成功，否则登录失败。
				cStr = strtoken(c,NULL,":");
				aStr = strtoken(a,cStr,"=");
				if(strcmp(aStr," PHPSESSID")==0) // 会话ID
				{
					strcpy(conf.sid,cStr); // 设置SID的值
				}

				if(strcmp(aStr," AID")==0) // 身份ID
				{
					strcpy(conf.aid,cStr); // 设置AID的值
				}
			}
		} // while

		if(strcmp(conf.sid,"") > 0 && strcmp(conf.aid,"")>0)
		{
			strcpy(conf.cookie,conf.sid); // 设置Cookie的值
			strcat(conf.cookie,";");
			strcat(conf.cookie,conf.aid);
			m_user.EnableWindow(FALSE); // 禁用用户输入框
			m_pwd.EnableWindow(FALSE);  // 禁用密码输入框 
			m_btn_login.EnableWindow(FALSE); // 禁用登录按钮
			m_id.EnableWindow(TRUE); // 启用“单号”文本框
			m_id.SetFocus(); // 设置“单号”文本框为焦点
		}
		else
		{
			MessageBox(_T("用户名 或 密码 错误！"),_T("登录失败"));
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
		if(strcmp(tStr,"")==0) // 如果返回的数据为空
		{
			MessageBox(L"单号不存在!",L"数据库");
			// 设置单号为焦点
			m_id.SetFocus();
			m_fangxing.EnableWindow(FALSE); // 禁用放行按钮
		}
		else
		{
			USES_CONVERSION;  // dali
			cJSON *jsonroot = cJSON_Parse(tStr); //json根
			char *ZhuangTai = cJSON_GetObjectItem(jsonroot,"zt")->valuestring;
			if(strcmp(ZhuangTai,"1")==0) // 如果放行状态为1，这继续分析json，并放行。
			{
				m_chehao.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"ch")->valuestring))); // 车号
				m_dianhua.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"dh")->valuestring)); // 电话
				m_shouhuo.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"dw")->valuestring))); // 单位
				m_huowu.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"hw")->valuestring))); // 货物
				m_guige.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"gg")->valuestring)); // 规格
				m_liuxiang.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"lx")->valuestring))); // 流向。这个可以删除
				m_chexing.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"cx")->valuestring))); // 车型
				m_pizhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"pz")->valuestring)); // 皮重
				m_maozhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"mz")->valuestring)); // 毛重
				m_jingzhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"jz")->valuestring)); // 净重
				m_danjia.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"dj")->valuestring))); // 单价
				m_jine.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"je")->valuestring))); // 金额
			}
			else
			{
				MessageBox(L"没有放行标志!",L"数据库");
				// 设置单号为焦点
				m_id.SetFocus();
				m_fangxing.EnableWindow(FALSE); // 禁用放行按钮
			}
			
		}
		// 这里还应该有放行的标志，用于判断是否可以放行。
		// 在服务端可以设置多少秒钟内多次放行同一个条形码。
		
	}
	if(strcmp(lStr,"400")==0)
		MessageBox(_T("文件没有找到！"),_T("网络连接"));
	if(strcmp(lStr,"500")==0)
		MessageBox(_T("服务器错误！！！"),_T("网络连接"));

}

void CClientDlg::OnFangXing()
{
	// 如果放行状态错误，则结束。

	// 格式：地址(1字节) + 命令(1字节) + 长度(1字节) + 数据(n字节) + 校验(1字节)
	//       地址：00h 广播地址		01h - 0Fh 设备地址
	//       命令：
	//            00h 查询状态(不可广播) 03000003 返回 03000003(未知)  0309000A(开)    030C000F(关)
	//            01h 停止 05010004 返回 05010004
	//            03h 开闸 05030006 返回 05030006 广播 00030003 无返回
	//            05h 关闸 0905000C 返回 0905000C 广播 00050005 无返回
	//            09h 查计数 0A090003 返回 0A0902030406 其中 0304为计数
	//            0Fh 清计数 0F0F0000 返回 0F0F0000
	//
	//       长度：默认长度为0，仅查计数的返回值有长度为2
	//       数据：默认不存在，仅查计数的返回值有2字节的数据。
	//       校验：将前面的所有字节进行异或运算(XOR)。
	//

	// 需要设置开门信号的数据
	char buf[64]={0};
	buf[0] = 0x00; // 广播
	buf[1] = 0x03; // 开闸
	buf[2] = 0x00; // 长度
	               // 数据
	buf[3] = 0x03; // 校验
	com1.write((char*)&buf,4); // 发送开门信号到串口1

	// 清空文本框的数据，并将焦点设置在单号处。
	m_id.SetWindowText(_T("")); // 单号
	m_chehao.SetWindowText(_T("")); // 车号
	m_dianhua.SetWindowText(_T("")); // 电话
	m_shouhuo.SetWindowText(_T("")); // 收货单位
	m_huowu.SetWindowText(_T("")); // 货物名称
	m_guige.SetWindowText(_T("")); // 货物规格
	m_liuxiang.SetWindowText(_T("")); // 货物流向
	m_chexing.SetWindowText(_T("")); // 车型
	m_pizhong.SetWindowText(_T("")); // 皮重
	m_maozhong.SetWindowText(_T("")); // 毛重
	m_jingzhong.SetWindowText(_T("")); // 净重
	m_danjia.SetWindowText(_T("")); // 单价
	m_jine.SetWindowText(_T("")); // 金额

	// 设置单号为焦点
	m_id.SetFocus();
	m_fangxing.EnableWindow(FALSE); // 禁用放行按钮
}