
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
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
	DDX_Control(pDX, IDC_COMBO_CHEXING, m_cbCheXing);
	DDX_Control(pDX, IDC_COMBO_HUOWU, m_cbHuoWu);
	DDX_Control(pDX, IDC_COMBO_GUIGE, m_cbGuiGe);
	DDX_Control(pDX, IDC_COMBO_LIUXIANG, m_cbLiuXiang);
	DDX_Control(pDX, IDC_EDIT6, m_ip);
	DDX_Control(pDX, IDC_EDIT7, m_port);
	DDX_Control(pDX, IDC_EDIT8, m_com1);
	DDX_Control(pDX, IDC_EDIT10, m_com2);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_MESSAGE(ON_COM_RECEIVE, On_Receive) // 如果收到 ON_COM_RECEIVE 消息，调用接收函数
	ON_BN_CLICKED(IDC_BUTTON_COM, &CClientDlg::OnBnClickedButtonCom)
	ON_BN_CLICKED(IDC_BUTTON_COM1_SEND, &CClientDlg::OnBnClickedButtonCom1Send)
	ON_BN_CLICKED(IDC_BUTTON_COM2_SEND, &CClientDlg::OnBnClickedButtonCom2Send)
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
	strcpy(conf.ip,"192.168.203.5");
	conf.port = 80;
	conf.com1_name=1;
	strcpy(conf.com1_parm,"9600,8,N,1");
	conf.com2_name=2;
	strcpy(conf.com2_parm,"9600,8,N,1");

	// 打开配置文件 JSON 格式
	FILE *f=fopen("config","rb"); // 配置文件 config
	if(f==NULL)
	{
		MessageBox("打开配置文件失败！", "config", MB_OK);
	}


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

	// 连接服务器
	if (0 == m_Conn.Connect(conf.ip,conf.port))
	{
		m_ip.SetWindowText(conf.ip);
		char tmp[16]={0};
		sprintf(tmp,"%d",conf.port);
		m_port.SetWindowText(tmp);
		printf("连接服务器成功\n");
	}
	else
	{
		MessageBox("连接服务器失败", "网络", MB_OK);
	}

	// 初始化串口
	if(!com1.open(conf.com1_name))
	{
		m_com2.SetWindowText("打开失败");
//		MessageBox("串口1打开失败", "串口", MB_OK);
	}
	else
	{
		com1.set_hwnd(m_hWnd);
		char tmp[64] = {0};
		sprintf(tmp,"COM%d,%s",conf.com1_name,conf.com1_parm);
		m_com1.SetWindowText(tmp);
	}

	if(!com2.open(conf.com2_name))
	{
		m_com2.SetWindowText("打开失败");
//		MessageBox("串口2打开失败", "串口", MB_OK);
	}
	else
	{
		com2.set_hwnd(m_hWnd);
		char tmp[64] = {0};
		sprintf(tmp,"COM%d,%s",conf.com2_name,conf.com2_parm);
		m_com2.SetWindowText(tmp);
	}

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
	GetDlgItem(IDC_COMBO_HUOWU)->SetFont(m_Font);

	// 货物规格
	GetDlgItem(IDC_STATIC_GUIGE)->SetFont(m_Font);
	GetDlgItem(IDC_COMBO_GUIGE)->SetFont(m_Font);

	// 收货单位
	GetDlgItem(IDC_STATIC_SHOUHUO)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_SHOUHUO)->SetFont(m_Font);

	// 货物流向
	GetDlgItem(IDC_STATIC_LIUXIANG)->SetFont(m_Font);
	GetDlgItem(IDC_COMBO_LIUXIANG)->SetFont(m_Font);


	////////////////////////////////

	// 车型
	GetDlgItem(IDC_STATIC_CHEXING)->SetFont(m_Font);
	GetDlgItem(IDC_COMBO_CHEXING)->SetFont(m_Font);

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

	m_cbCheXing.AddString("大车");
	m_cbCheXing.AddString("小车");
	m_cbCheXing.SetCurSel(0); // 选择第一项“大车”

	m_cbHuoWu.AddString("青石");
	m_cbHuoWu.AddString("半青石");
	m_cbHuoWu.AddString("石粉");
	m_cbHuoWu.AddString("混合");
	m_cbHuoWu.AddString("峰石");
	m_cbHuoWu.AddString("片石");
	m_cbHuoWu.SetCurSel(0);

	m_cbGuiGe.AddString("0.5");
	m_cbGuiGe.AddString("1.2");
	m_cbGuiGe.AddString("1.3");
	m_cbGuiGe.AddString("1.45");
	m_cbGuiGe.AddString("1.55");
	m_cbGuiGe.AddString("2.4");

	m_cbLiuXiang.AddString("提货");
	m_cbLiuXiang.AddString("出厂");


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

void CClientDlg::OnBnClickedButtonCom()
{
	// TODO: 在此添加控件通知处理程序代码
	//GetData("/","");
	PostData("/post.php","name=dali");

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
	//MessageBox("连接成功");
}

// 接收网络数据
void CClientDlg::OnRvc()
{
	char szRcv[1025] = {0};
	int nRcved = m_Conn.Receive(szRcv,1024);
	if (SOCKET_ERROR != nRcved)
	{
		//strcat(szRcv,"\r\n");
		printf("%s\n",szRcv);
		GetDlgItem(IDC_EDIT_INFO)->SetWindowText(szRcv);
	}
}

// 关闭网络
void CClientDlg::OnClose()
{
	m_Conn.Close();
}

void CClientDlg::GetData(char *url, char *parm)
{
	char Data[1024] = {0};
	strcat(Data,"GET ");
	strcat(Data,url);
	strcat(Data,parm);
	strcat(Data," HTTP/1.1\r\n");
	strcat(Data,"Host: ");
	strcat(Data,conf.ip);
	strcat(Data,"\r\n");
	strcat(Data,"Connection: Keep-Alive\r\n");
	strcat(Data,"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n");
	strcat(Data,"User-Agent: Mozilla/5.0 (Windows Client; Dali Wang<wangdali@qq.com>)\r\n");
	strcat(Data,"Accept-Language: zh-CN,zh;q=0.8\r\n");
	strcat(Data,"\r\n");
	m_Conn.Send(Data,strlen(Data));
}

void CClientDlg::PostData(char *url, char *parm)
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
	strcat(Data,"Content-Type: application/x-www-form-urlencoded\r\n");
	strcat(Data,"Content-Length: ");
	char tmp[16] = {0};
	sprintf(tmp,"%d",strlen(parm));
	strcat(Data,tmp);
	strcat(Data,"\r\n");
	strcat(Data,"\r\n");
	strcat(Data,parm);
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
			MessageBox("回车");
			return TRUE;
		case VK_ESCAPE: // ESC 按键
			return TRUE;
		}
	}

	if(pMsg->message==WM_SYSKEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_F4: // ALT + F4按键 通常用来退出程序
			MessageBox("F4");
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
