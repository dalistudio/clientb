
// ClientDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CClientDlg �Ի���




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

	ON_MESSAGE(ON_COM_RECEIVE, On_Receive) // ����յ����� ON_COM_RECEIVE ��Ϣ�����ý��պ���
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


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_post_id = 0; // ��ʼ���ύID
	memset(&m_net_rvc_data,0,1025);
	m_net_rvc_len = 0;

#ifdef _DEBUG // ���԰濪��
	// ���������д��� 
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

	// Ĭ������
	memset(&conf,0,sizeof(CONF));
	strcpy(conf.title,"���ϸۿ��ķ�ʯ��");
	strcpy(conf.ip,"192.168.1.5");
	conf.port = 80;
	conf.com1_id=1;
	strcpy(conf.com1_para,"baud=9600 parity=N data=8 stop=1");
	conf.com2_id=2;
	strcpy(conf.com2_para,"baud=9600 parity=N data=8 stop=1");
	strcpy(conf.cookie,"");

	// �������ļ� JSON ��ʽ
	FILE *f=fopen("config","rb"); // �����ļ� config
	if(f==NULL)
	{
		MessageBox("�������ļ�ʧ�ܣ�", "config", MB_OK);
	}
	fseek(f,0,SEEK_END); // �����ļ�β
	long len=ftell(f); // ����ļ�����
	fseek(f,0,SEEK_SET); // �����ļ�ͷ
	char *data=(char*)malloc(len+1); // �����ļ��ռ�
	fread(data,1,len,f); // ��ȡ�ļ�����
	fclose(f); // �ر��ļ�
	cJSON *jsonroot = cJSON_Parse(data); //json��
	strcpy(conf.title,cJSON_GetObjectItem(jsonroot,"title")->valuestring); // ��ñ���

	cJSON *jsonServer=cJSON_GetObjectItem(jsonroot,"server");//ȡ Server
	strcpy(conf.ip,cJSON_GetObjectItem(jsonServer,"ip")->valuestring); // ���IP��ַ
	conf.port = cJSON_GetObjectItem(jsonServer,"port")->valueint; // ��ö˿�

	cJSON *jsonCOM1=cJSON_GetObjectItem(jsonroot,"com1");//ȡ COM1
	conf.com1_id = cJSON_GetObjectItem(jsonCOM1,"port")->valueint; // ���COM�˿�
	strcpy(conf.com1_para,cJSON_GetObjectItem(jsonCOM1,"para")->valuestring); // ���COM������

	cJSON *jsonCOM2=cJSON_GetObjectItem(jsonroot,"com1");//ȡ COM1
	conf.com2_id = cJSON_GetObjectItem(jsonCOM2,"port")->valueint; // ���COM�˿�
	strcpy(conf.com2_para,cJSON_GetObjectItem(jsonCOM2,"para")->valuestring); // ���COM������

	// ���ö�ʱ��
	SetTimer(1,20000,0); // 20��

	// ���ӷ�����
	OnBnClickedButtonNetConn();

	// ��ʼ������
	OnBnClickedButtonComConn();

	///////////////////////

	CFont *m_Font;
	m_Font = new CFont; 
	m_Font->CreateFont(
		32, // ����߶�
		13,	// ������
		0,	// �ı��е���б��
		0,	// �ַ����ߵ���б��
		100,	// ����Ĵ�ϸ
		FALSE,	// �Ƿ�б��
		FALSE,	// �Ƿ��»���
		0,		// �Ƿ�ɾ����
		ANSI_CHARSET,			// ������ַ���
		OUT_DEFAULT_PRECIS,		// �ַ����������
		CLIP_DEFAULT_PRECIS,	// �ַ��Ĳü�����
		DEFAULT_QUALITY,		// �ַ����������
		FF_SWISS,				// �ַ�����������
		"����");				// ��������

	// ����
	GetDlgItem(IDC_EDIT_TITLE)->SetWindowText(conf.title);
	GetDlgItem(IDC_EDIT_TITLE)->SetFont(m_Font);
	//////////////////////////////////////

	// ����
	GetDlgItem(IDC_STATIC_DANHAO)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_DANHAO)->SetFont(m_Font);
	
	// ����
	GetDlgItem(IDC_STATIC_CHEHAO)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_CHEHAO)->SetFont(m_Font);

	// �绰
	GetDlgItem(IDC_STATIC_DIANHUA)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_DIANHUA)->SetFont(m_Font);

	// ��������
	GetDlgItem(IDC_STATIC_HUOWU)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_HUOWU)->SetFont(m_Font);

	// ������
	GetDlgItem(IDC_STATIC_GUIGE)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_GUIGE)->SetFont(m_Font);

	// �ջ���λ
	GetDlgItem(IDC_STATIC_SHOUHUO)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_SHOUHUO)->SetFont(m_Font);

	// ��������
	GetDlgItem(IDC_STATIC_LIUXIANG)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_LIUXIANG)->SetFont(m_Font);


	////////////////////////////////

	// ����
	GetDlgItem(IDC_STATIC_CHEXING)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_CHEXING)->SetFont(m_Font);

	// Ƥ��
	GetDlgItem(IDC_STATIC_PIZHONG)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_PIZHONG)->SetFont(m_Font);

	// ë��
	GetDlgItem(IDC_STATIC_MAOZHONG)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_MAOZHONG)->SetFont(m_Font);

	// ����
	GetDlgItem(IDC_STATIC_JINGZHONG)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_JINGZHONG)->SetFont(m_Font);

	// ����
	GetDlgItem(IDC_STATIC_DANJIA)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_DANJIA)->SetFont(m_Font);

	// ���
	GetDlgItem(IDC_STATIC_JINE)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_JINE)->SetFont(m_Font);

	/////////////////////////////////

	// �ύ��ť
	GetDlgItem(IDC_BUTTON_TIJIAO)->SetFont(m_Font);

	// ��ӡ��ť
	GetDlgItem(IDC_BUTTON_DAYIN)->SetFont(m_Font);

	/////////////////////////////////




	//return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ���մ��ڵ���Ϣ��������
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
		ltoa((long)wp, com_str + 3, 10); //	WPARAM ����˿ں�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	com2.write("COM2-test");
}

// ��������
void CClientDlg::OnConnected()
{
	m_ip.SetWindowText(conf.ip);
	char tmp[16]={0};
	sprintf(tmp,"%d",conf.port);
	m_port.SetWindowText(tmp);
	printf("���ӷ������ɹ�\n");
	m_btn_net.SetWindowText("���ӳɹ�");
	m_btn_net.EnableWindow(FALSE);
	//MessageBox("���ӳɹ�");
}

// ������������
void CClientDlg::OnRvc()
{
	//m_post_id = 1 Ϊ������������
	//m_post_id = 2 Ϊ�û���¼����
	//m_post_id = 3 Ϊ�ύ��������

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

// �ر�����
void CClientDlg::OnClose()
{
	m_ip.SetWindowText("");
	m_port.SetWindowText("");
	m_btn_net.SetWindowText("��������(&R)...");
	m_btn_net.EnableWindow(TRUE); // ���Ӱ�ť
	m_Conn.Close();
	//MessageBox("�����ѹرգ�\n���������ӷ�������","��������");
	// �Զ���������
	Sleep(3000); // ��Ϣ 3������
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
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN: // �س�����
			//MessageBox("�س�");
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
					SendMessage(WM_COMMAND,i,(LPARAM)wnd->m_hWnd); // ���Ͱ�ť��Ϣ
				}
				if(i == IDC_EDIT_DANHAO)
				{
					SendMessage(WM_COMMAND,IDC_BUTTON_TIJIAO,(LPARAM)wnd->m_hWnd); // ���Ͱ�ť��Ϣ
				}
				if(i == IDC_BUTTON_FANGXING)
				{
					return TRUE;
				}
					pMsg->wParam = VK_TAB; // ������һ���ؼ�
			}
			break;
//		case VK_ESCAPE: // ESC ����
//			break;
		}
	}

	if(pMsg->message==WM_SYSKEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_F4: // ALT + F4���� ͨ�������˳�����
			MessageBox("F4");
			return TRUE; // ��������ִ���˳�����
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

// ���ӷ�����
void CClientDlg::OnBnClickedButtonNetConn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ��ʼ��Socket
	m_Conn.SetDlg(this);
	if (m_Conn.Create())
	{
		printf("����Socket�ɹ�\n");
	}
	else
	{
		MessageBox("����Socketʧ�ܣ�");
	}
	m_Conn.Connect(conf.ip,conf.port);

}

// ��¼
void CClientDlg::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_post_id = 2; // �ύIDΪ2
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

// �ǳ�
void CClientDlg::OnBnClickedButtonLogout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_user.SetWindowText("");
	m_user.EnableWindow(TRUE);
	m_pwd.SetWindowText("");
	m_pwd.EnableWindow(TRUE);
	m_btn_login.EnableWindow(TRUE);
	memset(&(conf.cookie),0,256);
}

// ��ʱ��
void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case 1:
		m_post_id = 1; // �ύIDΪ1
		GetData("/keepalive.php","");
		return;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// �ύ��ť
void CClientDlg::OnBnClickedButtonTijiao()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_post_id = 3; // �ύIDΪ3
	PostData("/post.php",""); // �ύ��������
}

// ���а�ť
void CClientDlg::OnBnClickedButtonFangxing()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char buf[64]={0};
	buf[0] = 0xAA;
	buf[1] = 0xAA;
	buf[2] = 0x01;
	buf[3] = 0xBB;
	buf[4] = 0xCC;
	buf[5] = 0xDD;
	int len = strlen(buf);
	com1.write((char*)&buf,len); // ���Ϳ����źŵ�����1

	// ����ı�������ݣ��������������ڵ��Ŵ���
	m_id.SetWindowText(""); // ����
	m_chehao.SetWindowText(""); // ����
	m_dianhua.SetWindowText(""); // �绰
	m_shouhuo.SetWindowText(""); // �ջ���λ
	m_huowu.SetWindowText(""); // ��������
	m_guige.SetWindowText(""); // ������
	m_liuxiang.SetWindowText(""); // ��������
	m_chexing.SetWindowText(""); // ����
	m_pizhong.SetWindowText(""); // Ƥ��
	m_maozhong.SetWindowText(""); // ë��
	m_jingzhong.SetWindowText(""); // ����
	m_danjia.SetWindowText(""); // ����
	m_jine.SetWindowText(""); // ���
	// ���õ���Ϊ����
	m_id.SetFocus();
}


HBRUSH CClientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
//	if(nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
//	{
		//pDC->SetTextColor(RGB(0,0,255));//����ɫ
//		pDC->SetBkColor(RGB(192,192,192));//���屳��ɫ******
//		HBRUSH B = CreateSolidBrush(RGB(192,192,192)); //����ɫ
//		return B;
//	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

// ���´򿪴���
void CClientDlg::OnBnClickedButtonComConn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ��ʼ������
	if(!com1.open(conf.com1_id,conf.com1_para))
	{
		m_com1.SetWindowText("��ʧ��");
//		MessageBox("����1��ʧ��", "����", MB_OK);
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
		m_com2.SetWindowText("��ʧ��");
//		MessageBox("����2��ʧ��", "����", MB_OK);
	}
	else
	{
		com2.set_hwnd(m_hWnd);
		char tmp[64] = {0};
		sprintf(tmp,"COM%d,%s",conf.com2_id,conf.com2_para);
		m_com2.SetWindowText(tmp);
	}
}

// ��������
void CClientDlg::OnKeepalive()
{
	// ���账���ص����ݡ�
}

// �û���¼
void CClientDlg::OnLogin()
{
	// ������� 200 �� SetCookie ������õ�¼��ؿؼ�
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

// �ύ����
void CClientDlg::OnPost()
{
	// �����������ݣ�����ʾ������
	TOKEN * t = (TOKEN *)malloc(sizeof(TOKEN));
	TOKEN * l = (TOKEN *)malloc(sizeof(TOKEN));

	char *tStr ,*lStr, *cStr;
	tStr = strtoken(t,(char*)&m_net_rvc_data,"\r\n"); // ��Ӧ��
	lStr = strtoken(l,(char*)tStr," "); // HTTP/1.1
	lStr = strtoken(l,NULL," "); // 200 
	if(strcmp(lStr,"200")==0)
	{
		while(t->data!=NULL)
		{
			tStr = strtoken(t,NULL, "\r\n");
			if(strcmp(tStr,"")==0) // ����
			{
				tStr = strtoken(t,NULL, "\r\n"); // �õ���Ӧ�峤��
				tStr = strtoken(t,NULL, "\r\n"); // �õ���Ӧ������
				break;
			}
		}
		cJSON *jsonroot = cJSON_Parse(tStr); //json��
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
		MessageBox("�ļ�û���ҵ���","��������");
	if(strcmp(lStr,"500")==0)
		MessageBox("���������󣡣���","��������");

}