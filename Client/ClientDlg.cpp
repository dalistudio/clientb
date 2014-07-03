
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
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
	DDX_Control(pDX, IDC_EDIT_USER, m_user);
	DDX_Control(pDX, IDC_EDIT_PWD, m_pwd);
	DDX_Control(pDX, IDC_EDIT_DANHAO, m_id);
	DDX_Control(pDX, IDC_BUTTON_FANGXING, m_fangxing);
	DDX_Control(pDX, IDC_EDIT_CHEHAO, m_chehao);
	DDX_Control(pDX, IDC_EDIT_DIANHUA, m_dianhua);
	DDX_Control(pDX, IDC_EDIT_SHOUHUO, m_shouhuo);
	DDX_Control(pDX, IDC_EDIT_HUOWU, m_huowu);
	DDX_Control(pDX, IDC_EDIT_GUIGE, m_guige);
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

	//
	// CURL
	// 
	CURLcode code = curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

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
	strcpy_s(conf.title,"海南港口文峰石场");
	strcpy_s(conf.ip,"192.168.1.5");
	conf.port = 80;
	conf.com1_id=1;
	strcpy_s(conf.com1_para,"baud=9600 parity=N data=8 stop=1");

	// 打开配置文件 JSON 格式
	FILE *f;
	fopen_s(&f,"config","rb"); // 配置文件 config
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
		strcpy_s(conf.title,cJSON_GetObjectItem(jsonroot,"title")->valuestring); // 获得标题

		cJSON *jsonServer=cJSON_GetObjectItem(jsonroot,"server");//取 Server
		strcpy_s(conf.ip,cJSON_GetObjectItem(jsonServer,"ip")->valuestring); // 获得IP地址
		conf.port = cJSON_GetObjectItem(jsonServer,"port")->valueint; // 获得端口

		cJSON *jsonCOM1=cJSON_GetObjectItem(jsonroot,"com1");//取 COM1
		conf.com1_id = cJSON_GetObjectItem(jsonCOM1,"port")->valueint; // 获得COM端口
		strcpy_s(conf.com1_para,cJSON_GetObjectItem(jsonCOM1,"para")->valuestring); // 获得COM的属性
	}
	
	USES_CONVERSION;
	// 显示网络服务器的IP地址和端口
	m_ip.SetWindowText(A2CW(conf.ip));
	char port[8] = {0};
	_ltoa_s(conf.port,port,6,10);
	m_port.SetWindowText(A2CW(port));

	// 设置定时器
	SetTimer(1,60000,0); // 60秒

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

	// 标题
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

// 登录
void CClientDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	CString strUser, strPasswd;
	m_user.GetWindowText(strUser); // 获得用户名
	m_pwd.GetWindowText(strPasswd); // 获得密码

	char url[256]={0};
	strcat_s(url,"http://");
	strcat_s(url,conf.ip);
	strcat_s(url,"/");
	strcat_s(url,"login.php");
	strcat_s(url,"?");
	strcat_s(url,"User=");
	strcat_s(url,W2A(strUser));
	strcat_s(url,"&");
	strcat_s(url,"Passwd=");
	strcat_s(url,W2A(strPasswd));
	strcat_s(url,"&");
	strcat_s(url,"Level=2");

	CURLcode res;
	
	curl_easy_setopt(curl,CURLOPT_URL,url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, login_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curl,CURLOPT_COOKIEFILE,"");
	res = curl_easy_perform(curl);

	struct curl_slist *cookies = NULL;
	curl_easy_getinfo(curl,CURLINFO_COOKIELIST,&cookies);
	if(cookies->next == NULL) // 判断是否有第二个cookie。这里应该判断AID。
	{
		m_isLogin = 0;
		MessageBox(L"密码错误！！");
		return; // 退出
	}
	else
	{
		m_isLogin = 1;
	}
}

// 登出
void CClientDlg::OnBnClickedButtonLogout()
{
	// TODO: 在此添加控件通知处理程序代码
	m_isLogin = 0; 
	m_user.SetWindowText(_T("")); // 设置用户框为空
	m_user.EnableWindow(TRUE); // 设置用户框为可读写

	m_pwd.SetWindowText(_T("")); // 设置密码框为空
	m_pwd.EnableWindow(TRUE); // 设置密码框为可读写

	m_btn_login.EnableWindow(TRUE); // 设置登陆按钮为可操作
}

// 定时器
void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		// 60秒定时器，目前无用
		return;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// 提交按钮
void CClientDlg::OnBnClickedButtonTijiao()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strDanHao;
	m_id.GetWindowText(strDanHao); // 获取单号
	int i = _ttoi(strDanHao); // 转换单号为数字类型
	if(strDanHao=="" || i==0) // 如果单号为空，或者等于0
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
		OnFangXing(); // 直接放行
		MessageBox(L"放行成功！");
	}
	else if(i>=100000) // 正常交易的单号
	{
		USES_CONVERSION;
		char url[256]={0};
		strcat_s(url,"http://");
		strcat_s(url,conf.ip);
		strcat_s(url,"/");
		strcat_s(url,"getid.php");
		strcat_s(url,"?");
		strcat_s(url,"id=");
		strcat_s(url,W2A(strDanHao)); // 单号

		CURLcode res;
	
		curl_easy_setopt(curl,CURLOPT_URL,url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getid_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		res = curl_easy_perform(curl);
	}
}

// 放行按钮
void CClientDlg::OnBnClickedButtonFangxing()
{
	// TODO: 在此添加控件通知处理程序代码

	// 提交一个POST放行
	// 需要提交门岗放行的状态。

	USES_CONVERSION;
	char data[1024]={0};
	CString strDanHao;
	m_id.GetWindowText(strDanHao); // 单号
	sprintf_s(data,"DanHao=%s&ZhuangTai=8&",W2A(strDanHao)); // 单号和状态

	CString strUser;
	m_user.GetWindowText(strUser);
	sprintf_s(data,"%sFangXingYuan=%s",data, W2A(strUser)); // 放行员

	char url[256]={0};
	strcat_s(url,"http://");
	strcat_s(url,conf.ip);
	strcat_s(url,"/");
	strcat_s(url,"post.php");

	CURLcode res;
	curl_easy_setopt(curl,CURLOPT_URL,url);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);  
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, post_data);
	curl_easy_setopt( curl, CURLOPT_WRITEDATA, this ); 
	res = curl_easy_perform(curl);
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
		sprintf_s(tmp,"COM%d:%s",conf.com1_id,conf.com1_para);
		USES_CONVERSION;
		m_com1.SetWindowText(A2CW(tmp)); // 设置串口1框的值
	}
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

// 登陆返回数据
size_t CClientDlg::login_data(void *ptr, size_t size, size_t nmemb, void *userp)
{
	CClientDlg *client =(CClientDlg*)userp;
	client->m_user.EnableWindow(FALSE); // 禁用用户输入框
	client->m_pwd.EnableWindow(FALSE);  // 禁用密码输入框 
	client->m_btn_login.EnableWindow(FALSE); // 禁用登录按钮
	client->m_id.EnableWindow(TRUE); // 启用单号控件
	client->m_id.SetFocus(); // 设置单号为焦点
	
	return size*nmemb;
}

// 单号返回数据
size_t CClientDlg::getid_data(void *ptr, size_t size, size_t nmemb, void *userp)
{
	CClientDlg *client =(CClientDlg*)userp;

	USES_CONVERSION; 
	cJSON *jsonroot = cJSON_Parse((const char*)ptr); //json根
	if(jsonroot)
	{
		char *strid = cJSON_GetObjectItem(jsonroot,"id")->valuestring; // 单号
		if(strcmp(strid,"0")==0)
		{
			client->MessageBox(L"无此单号！！！");
			return size*nmemb; // 返回函数
		}

		char *ZhuangTai = cJSON_GetObjectItem(jsonroot,"zt")->valuestring; // 获得放行状态
		if(strcmp(ZhuangTai,"0")==0) 
		{
			client->MessageBox(L"不可放行，未第二次过磅！！！");
			// 设置单号为焦点
			client->m_id.SetFocus();
			client->m_fangxing.EnableWindow(FALSE); // 禁用放行按钮
		}
		if(strcmp(ZhuangTai,"1")==0) // 如果放行状态为1，这继续分析json，并放行。
		{
			client->m_chehao.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"ch")->valuestring))); // 车号
			client->m_dianhua.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"dh")->valuestring)); // 电话
			client->m_shouhuo.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"dw")->valuestring))); // 单位
			client->m_huowu.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"hw")->valuestring))); // 货物
			client->m_guige.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"gg")->valuestring)); // 规格
			client->m_chexing.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"cx")->valuestring))); // 车型
			client->m_pizhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"pz")->valuestring)); // 皮重
			client->m_maozhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"mz")->valuestring)); // 毛重
			client->m_jingzhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"jz")->valuestring)); // 净重
			client->m_danjia.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"dj")->valuestring))); // 单价
			client->m_jine.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"je")->valuestring))); // 金额
			client->m_fangxing.EnableWindow(TRUE); // 启用放行按钮
		}
		if(strcmp(ZhuangTai,"2")==0) 
		{
			// 获取出场时间
			char *strChuChang = new char[32];
			memset(strChuChang,0,32);
			strChuChang = cJSON_GetObjectItem(jsonroot,"cc")->valuestring; // 出场时间
			if(strcmp(strChuChang,"")==0)
			{
				client->MessageBox(L"已经放行出场，不可再次放行！！！");
			}
			else
			{
				// 将时间字符串转成整数
				// 判断在5分钟内，则可再放行。
				CString stime = A2CW(strChuChang); 
				COleDateTime CurrTime = COleDateTime::GetCurrentTime(); 
				COleDateTime tempTime; 
				tempTime.ParseDateTime(stime); 
				COleDateTimeSpan  result = CurrTime - tempTime; 
				double sec = result.GetTotalSeconds();
				if(sec<=5*60) // 5分钟
				{
					client->OnFangXing(); // 放行
					client->MessageBox(L"放行成功！");
				}
				else
				{
					client->MessageBox(L"超出放行时间，不可放行！！！");
				}
				delete[] strChuChang;
			}
		}
	}
//	cJSON_Delete(jsonroot);
	return size*nmemb;
}

// 提交数据返回数据
size_t CClientDlg::post_data(void *ptr, size_t size, size_t nmemb, void *userp)
{
	CClientDlg *client =(CClientDlg*)userp;

	char str[16]={0};
	memcpy(str,ptr,5);
	// 这里判断提交是否成功
	if(strcmp(str,"post3")==0)
	{
		client->OnFangXing(); // 放行
		client->MessageBox(L"放行成功！");
	}
	return size*nmemb;
}