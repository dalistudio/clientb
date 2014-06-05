
// ClientDlg.cpp : ʵ���ļ�
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
//	conf.com2_id=2;
//	strcpy(conf.com2_para,"baud=9600 parity=N data=8 stop=1");
//	strcpy(conf.cookie,"");

	// �������ļ� JSON ��ʽ
	FILE *f=fopen("config","rb"); // �����ļ� config
	if(f==NULL)
	{
		MessageBox(_T("�������ļ�ʧ�ܣ�"), _T("config"), MB_OK);
	}
	else
	{
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

//		cJSON *jsonCOM2=cJSON_GetObjectItem(jsonroot,"com1");//ȡ COM1
//		conf.com2_id = cJSON_GetObjectItem(jsonCOM2,"port")->valueint; // ���COM�˿�
//		strcpy(conf.com2_para,cJSON_GetObjectItem(jsonCOM2,"para")->valuestring); // ���COM������
	}
	

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
		_T("����"));				// ��������

	// ��˾LOGO
	GetDlgItem(IDC_STATIC_LOGO)->SetFont(m_Font);

	// ����
	USES_CONVERSION;  // dali
	GetDlgItem(IDC_EDIT_TITLE)->SetWindowText(A2CW(conf.title));
	GetDlgItem(IDC_EDIT_TITLE)->SetFont(m_Font);
	//////////////////////////////////////

	// ����
	GetDlgItem(IDC_STATIC_DANHAO)->SetFont(m_Font);
	GetDlgItem(IDC_EDIT_DANHAO)->SetFont(m_Font);
	m_id.EnableWindow(FALSE); // ���á����š��ı���
	
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

	// ���а�ť
	GetDlgItem(IDC_BUTTON_FANGXING)->SetFont(m_Font);
	m_fangxing.EnableWindow(FALSE); // ���÷��а�ť

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
	// �Ÿڿͻ��ˣ�һ���������Ҫ����բ�ڷ���������Ϣ
	// ��ʽ����ַ(1�ֽ�) + ����(1�ֽ�) + ����(1�ֽ�) + ����(n�ֽ�) + У��(1�ֽ�)
	//       ��ַ��00h �㲥��ַ		01h - 0Fh �豸��ַ
	//       ���
	//            00h ��ѯ״̬(���ɹ㲥) 03000003 ���� 03000003(δ֪)  0309000A(��)    030C000F(��)
	//            01h ֹͣ 05010004 ���� 05010004
	//            03h ��բ 05030006 ���� 05030006 �㲥 00030003 �޷���
	//            05h ��բ 0905000C ���� 0905000C �㲥 00050005 �޷���
	//            09h ����� 0A090003 ���� 0A0902030406 ���� 0304Ϊ����
	//            0Fh ����� 0F0F0000 ���� 0F0F0000
	//
	//       ���ȣ�Ĭ�ϳ���Ϊ0����������ķ���ֵ�г���Ϊ2
	//       ���ݣ�Ĭ�ϲ����ڣ���������ķ���ֵ��2�ֽڵ����ݡ�
	//       У�飺��ǰ��������ֽڽ����������(XOR)��
	//

	// ���账����յ��Ĵ�������


	return 0;
}


void CClientDlg::OnBnClickedButtonCom1Send()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CClientDlg::OnBnClickedButtonCom2Send()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

// ��������
void CClientDlg::OnConnected()
{
	USES_CONVERSION;  // dali
	m_ip.SetWindowText(A2CW(conf.ip));
	char tmp[16]={0};
	sprintf(tmp,"%d",conf.port);
	m_port.SetWindowText(A2CW(tmp));
	printf("���ӷ������ɹ�\n");
	m_btn_net.SetWindowText(_T("���ӳɹ�"));
	m_btn_net.EnableWindow(FALSE);
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
		USES_CONVERSION;  // dali
		GetDlgItem(IDC_EDIT_INFO)->SetWindowText(A2CW(m_net_rvc_data));
		switch(m_post_id)
		{
		case 1:
			OnKeepalive(); // ��������
			break;
		case 2:
			OnLogin(); // ��¼������
			break;
		case 3:
			OnPost(); // ��õ�����Ϣ
			break;
		case 4: 
			OnFangXing(); // �ύ����״̬
			break;
		}
	}
}

// �ر�����
void CClientDlg::OnClose()
{
	m_ip.SetWindowText(_T(""));
	m_port.SetWindowText(_T(""));
	m_btn_net.SetWindowText(_T("��������(&R)..."));
	m_btn_net.EnableWindow(TRUE); // ���Ӱ�ť
	m_Conn.Close();

	// �Զ���������
	Sleep(3000); // ��Ϣ 3������
	OnBnClickedButtonNetConn();
}

void CClientDlg::GetData(char *url, char *para)
{
	char Data[1024] = {0};
	strcat(Data,"GET ");
	strcat(Data,url);
	strcat(Data,"?"); // ע�����Ӧ�����ж�para�Ƿ�Ϊ��
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
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN: // �س�����
			CWnd *wnd =GetFocus();
			if(wnd != NULL)
			{
				TCHAR str[256];
				CString ClassName = _T("Button");
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
				if(i == IDC_BUTTON_FANGXING) // ���а�ť��Ϣ
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
			MessageBox(_T("F4"));
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
		MessageBox(_T("����Socketʧ�ܣ�"));
	}
	USES_CONVERSION;  // dali
	m_Conn.Connect(A2CW(conf.ip),conf.port);

}

// ��¼
void CClientDlg::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION; // dali
	m_post_id = 2; // �ύIDΪ2
	char Data[256] ={0};
	strcat(Data,"User=");
	CString strUser =_T("");
	m_user.GetWindowText(strUser); // ����û���
	strcat(Data,W2A(strUser)); // �����û���
	strcat(Data,"&Passwd=");
	CString strPwd = _T("");
	m_pwd.GetWindowText(strPwd); // �������
	strcat(Data,W2A(strPwd)); // ��������
	strcat(Data,"&Level=2"); // ����Level=2
	GetData("/login.php",(char*)&Data); // ���� GET ����
}

// �ǳ�
void CClientDlg::OnBnClickedButtonLogout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_user.SetWindowText(_T("")); // ����û�����
	m_user.EnableWindow(TRUE); // �����û�����
	m_pwd.SetWindowText(_T("")); // ��������
	m_pwd.EnableWindow(TRUE); // ���������
	m_btn_login.EnableWindow(TRUE); // ���õ�¼��ť
	m_id.EnableWindow(FALSE); // ���á����š��ı���

	memset(&(conf.cookie),0,256); // ���Cookie
	memset(&(conf.sid),0,256); // ���SID
	memset(&(conf.aid),0,256); // ���AID
}

// ��ʱ��
void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case 1:
		m_post_id = 1; // �ύIDΪ1
		GetData("/keepalive.php",""); // ���� GET ���󣬱�������
		return;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// �ύ��ť
void CClientDlg::OnBnClickedButtonTijiao()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SYSTEMTIME st;
	GetLocalTime(&st);
	char str[32]={0};
	sprintf(str,"date=%4d%02d%02d&id=",st.wYear,st.wMonth,st.wDay); // ��ǰ����

	CString strId;
	m_id.GetWindowText(strId); // ��ȡ����
	int i = _ttoi(strId); // ת������Ϊ��������
	if(strId=="" || i==0) // �������Ϊ�գ����ߵ���0
	{
		MessageBox(_T("���Ų���Ϊ�գ���\n ����ȫΪ��0"));
		m_fangxing.EnableWindow(FALSE); // ���а�ť
		return;
	}
	if(i<100000) // ����С��10�򣬱������ڲ�ʹ��
	{
		// �ڲ�ʹ��
		// ���ﲻͨ�����ݿ⣬ֱ�ӷ��С�
		// 00000 - 99999
		m_chehao.SetWindowText(_T("�ڲ���Ա"));
		m_chexing.SetWindowText(_T("�ڲ���Ա"));
		m_dianhua.SetWindowText(_T("�ڲ���Ա"));
		m_shouhuo.SetWindowText(_T("�ڲ���Ա"));
		m_huowu.SetWindowText(_T("�ڲ���Ա"));
		m_guige.SetWindowText(_T("�ڲ���Ա"));
	}
	else if(i>=100000) // �������׵ĵ���
	{
		m_post_id = 3; // �ύIDΪ3
		USES_CONVERSION; // dali
		strcat(str,W2A(strId)); // ���õ���
		GetData("/getid.php",str); // �ύ��������
	}

	// �ж�״̬���ϴη���ʱ������Ƿ�ɷ���
	m_fangxing.EnableWindow(TRUE); // �������а�ť
}

// ���а�ť
void CClientDlg::OnBnClickedButtonFangxing()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �ύһ��POST����
	// ��Ҫ�ύ�Ÿڷ��е�״̬��ֻ��ID�ͱ����û������ɡ�
	m_post_id = 4; // �ύIDΪ4
	PostData("/post.php",""); // �ύid level status�������ݿ⼴��
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
	if(!com1.open(conf.com1_id,conf.com1_para)) // �򿪴���1
	{
		m_com1.SetWindowText(_T("��ʧ��"));
	}
	else
	{
		com1.set_hwnd(m_hWnd);
		char tmp[64] = {0};
		sprintf(tmp,"COM%d:%s",conf.com1_id,conf.com1_para);
		USES_CONVERSION;  // dali
		m_com1.SetWindowText(A2CW(tmp)); // ���ô���1���ֵ
	}

//	if(!com2.open(conf.com2_id,conf.com2_para)) // �򿪴���2
//	{
//		m_com2.SetWindowText(_T("��ʧ��"));
//	}
//	else
//	{
//		com2.set_hwnd(m_hWnd);
//		char tmp[64] = {0};
//		sprintf(tmp,"COM%d,%s",conf.com2_id,conf.com2_para);
//		USES_CONVERSION;  // dali
//		m_com2.SetWindowText(A2CW(tmp)); // ���ô���2���ֵ
//	}
}

// ��������
void CClientDlg::OnKeepalive()
{
	// ���账���ص����ݡ�
	// �����������ж������Ƿ�����
}

// �û���¼
void CClientDlg::OnLogin()
{
	// ������� 200 �� SetCookie ������õ�¼��ؿؼ�
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
				// ����Ӧ���ж��Ƿ���ڡ�AID����ݱ�ʶ��
				// �������¼�ɹ��������¼ʧ�ܡ�
				cStr = strtoken(c,NULL,":");
				aStr = strtoken(a,cStr,"=");
				if(strcmp(aStr," PHPSESSID")==0) // �ỰID
				{
					strcpy(conf.sid,cStr); // ����SID��ֵ
				}

				if(strcmp(aStr," AID")==0) // ���ID
				{
					strcpy(conf.aid,cStr); // ����AID��ֵ
				}
			}
		} // while

		if(strcmp(conf.sid,"") > 0 && strcmp(conf.aid,"")>0)
		{
			strcpy(conf.cookie,conf.sid); // ����Cookie��ֵ
			strcat(conf.cookie,";");
			strcat(conf.cookie,conf.aid);
			m_user.EnableWindow(FALSE); // �����û������
			m_pwd.EnableWindow(FALSE);  // ������������� 
			m_btn_login.EnableWindow(FALSE); // ���õ�¼��ť
			m_id.EnableWindow(TRUE); // ���á����š��ı���
			m_id.SetFocus(); // ���á����š��ı���Ϊ����
		}
		else
		{
			MessageBox(_T("�û��� �� ���� ����"),_T("��¼ʧ��"));
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
		if(strcmp(tStr,"")==0) // ������ص�����Ϊ��
		{
			MessageBox(L"���Ų�����!",L"���ݿ�");
			// ���õ���Ϊ����
			m_id.SetFocus();
			m_fangxing.EnableWindow(FALSE); // ���÷��а�ť
		}
		else
		{
			USES_CONVERSION;  // dali
			cJSON *jsonroot = cJSON_Parse(tStr); //json��
			char *ZhuangTai = cJSON_GetObjectItem(jsonroot,"zt")->valuestring;
			if(strcmp(ZhuangTai,"1")==0) // �������״̬Ϊ1�����������json�������С�
			{
				m_chehao.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"ch")->valuestring))); // ����
				m_dianhua.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"dh")->valuestring)); // �绰
				m_shouhuo.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"dw")->valuestring))); // ��λ
				m_huowu.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"hw")->valuestring))); // ����
				m_guige.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"gg")->valuestring)); // ���
				m_liuxiang.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"lx")->valuestring))); // �����������ɾ��
				m_chexing.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"cx")->valuestring))); // ����
				m_pizhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"pz")->valuestring)); // Ƥ��
				m_maozhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"mz")->valuestring)); // ë��
				m_jingzhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"jz")->valuestring)); // ����
				m_danjia.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"dj")->valuestring))); // ����
				m_jine.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"je")->valuestring))); // ���
			}
			else
			{
				MessageBox(L"û�з��б�־!",L"���ݿ�");
				// ���õ���Ϊ����
				m_id.SetFocus();
				m_fangxing.EnableWindow(FALSE); // ���÷��а�ť
			}
			
		}
		// ���ﻹӦ���з��еı�־�������ж��Ƿ���Է��С�
		// �ڷ���˿������ö��������ڶ�η���ͬһ�������롣
		
	}
	if(strcmp(lStr,"400")==0)
		MessageBox(_T("�ļ�û���ҵ���"),_T("��������"));
	if(strcmp(lStr,"500")==0)
		MessageBox(_T("���������󣡣���"),_T("��������"));

}

void CClientDlg::OnFangXing()
{
	// �������״̬�����������

	// ��ʽ����ַ(1�ֽ�) + ����(1�ֽ�) + ����(1�ֽ�) + ����(n�ֽ�) + У��(1�ֽ�)
	//       ��ַ��00h �㲥��ַ		01h - 0Fh �豸��ַ
	//       ���
	//            00h ��ѯ״̬(���ɹ㲥) 03000003 ���� 03000003(δ֪)  0309000A(��)    030C000F(��)
	//            01h ֹͣ 05010004 ���� 05010004
	//            03h ��բ 05030006 ���� 05030006 �㲥 00030003 �޷���
	//            05h ��բ 0905000C ���� 0905000C �㲥 00050005 �޷���
	//            09h ����� 0A090003 ���� 0A0902030406 ���� 0304Ϊ����
	//            0Fh ����� 0F0F0000 ���� 0F0F0000
	//
	//       ���ȣ�Ĭ�ϳ���Ϊ0����������ķ���ֵ�г���Ϊ2
	//       ���ݣ�Ĭ�ϲ����ڣ���������ķ���ֵ��2�ֽڵ����ݡ�
	//       У�飺��ǰ��������ֽڽ����������(XOR)��
	//

	// ��Ҫ���ÿ����źŵ�����
	char buf[64]={0};
	buf[0] = 0x00; // �㲥
	buf[1] = 0x03; // ��բ
	buf[2] = 0x00; // ����
	               // ����
	buf[3] = 0x03; // У��
	com1.write((char*)&buf,4); // ���Ϳ����źŵ�����1

	// ����ı�������ݣ��������������ڵ��Ŵ���
	m_id.SetWindowText(_T("")); // ����
	m_chehao.SetWindowText(_T("")); // ����
	m_dianhua.SetWindowText(_T("")); // �绰
	m_shouhuo.SetWindowText(_T("")); // �ջ���λ
	m_huowu.SetWindowText(_T("")); // ��������
	m_guige.SetWindowText(_T("")); // ������
	m_liuxiang.SetWindowText(_T("")); // ��������
	m_chexing.SetWindowText(_T("")); // ����
	m_pizhong.SetWindowText(_T("")); // Ƥ��
	m_maozhong.SetWindowText(_T("")); // ë��
	m_jingzhong.SetWindowText(_T("")); // ����
	m_danjia.SetWindowText(_T("")); // ����
	m_jine.SetWindowText(_T("")); // ���

	// ���õ���Ϊ����
	m_id.SetFocus();
	m_fangxing.EnableWindow(FALSE); // ���÷��а�ť
}