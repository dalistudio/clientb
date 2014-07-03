
// ClientDlg.cpp : ʵ���ļ�
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

	ON_MESSAGE(ON_COM_RECEIVE, On_Receive) // ����յ����� ON_COM_RECEIVE ��Ϣ�����ý��պ���
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

	//
	// CURL
	// 
	CURLcode code = curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

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
	strcpy_s(conf.title,"���ϸۿ��ķ�ʯ��");
	strcpy_s(conf.ip,"192.168.1.5");
	conf.port = 80;
	conf.com1_id=1;
	strcpy_s(conf.com1_para,"baud=9600 parity=N data=8 stop=1");

	// �������ļ� JSON ��ʽ
	FILE *f;
	fopen_s(&f,"config","rb"); // �����ļ� config
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
		strcpy_s(conf.title,cJSON_GetObjectItem(jsonroot,"title")->valuestring); // ��ñ���

		cJSON *jsonServer=cJSON_GetObjectItem(jsonroot,"server");//ȡ Server
		strcpy_s(conf.ip,cJSON_GetObjectItem(jsonServer,"ip")->valuestring); // ���IP��ַ
		conf.port = cJSON_GetObjectItem(jsonServer,"port")->valueint; // ��ö˿�

		cJSON *jsonCOM1=cJSON_GetObjectItem(jsonroot,"com1");//ȡ COM1
		conf.com1_id = cJSON_GetObjectItem(jsonCOM1,"port")->valueint; // ���COM�˿�
		strcpy_s(conf.com1_para,cJSON_GetObjectItem(jsonCOM1,"para")->valuestring); // ���COM������
	}
	
	USES_CONVERSION;
	// ��ʾ�����������IP��ַ�Ͷ˿�
	m_ip.SetWindowText(A2CW(conf.ip));
	char port[8] = {0};
	_ltoa_s(conf.port,port,6,10);
	m_port.SetWindowText(A2CW(port));

	// ���ö�ʱ��
	SetTimer(1,60000,0); // 60��

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

	// ����
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

// ��¼
void CClientDlg::OnBnClickedButtonLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	USES_CONVERSION;
	CString strUser, strPasswd;
	m_user.GetWindowText(strUser); // ����û���
	m_pwd.GetWindowText(strPasswd); // �������

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
	if(cookies->next == NULL) // �ж��Ƿ��еڶ���cookie������Ӧ���ж�AID��
	{
		m_isLogin = 0;
		MessageBox(L"������󣡣�");
		return; // �˳�
	}
	else
	{
		m_isLogin = 1;
	}
}

// �ǳ�
void CClientDlg::OnBnClickedButtonLogout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_isLogin = 0; 
	m_user.SetWindowText(_T("")); // �����û���Ϊ��
	m_user.EnableWindow(TRUE); // �����û���Ϊ�ɶ�д

	m_pwd.SetWindowText(_T("")); // ���������Ϊ��
	m_pwd.EnableWindow(TRUE); // ���������Ϊ�ɶ�д

	m_btn_login.EnableWindow(TRUE); // ���õ�½��ťΪ�ɲ���
}

// ��ʱ��
void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case 1:
		// 60�붨ʱ����Ŀǰ����
		return;
	}

	CDialogEx::OnTimer(nIDEvent);
}

// �ύ��ť
void CClientDlg::OnBnClickedButtonTijiao()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strDanHao;
	m_id.GetWindowText(strDanHao); // ��ȡ����
	int i = _ttoi(strDanHao); // ת������Ϊ��������
	if(strDanHao=="" || i==0) // �������Ϊ�գ����ߵ���0
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
		OnFangXing(); // ֱ�ӷ���
		MessageBox(L"���гɹ���");
	}
	else if(i>=100000) // �������׵ĵ���
	{
		USES_CONVERSION;
		char url[256]={0};
		strcat_s(url,"http://");
		strcat_s(url,conf.ip);
		strcat_s(url,"/");
		strcat_s(url,"getid.php");
		strcat_s(url,"?");
		strcat_s(url,"id=");
		strcat_s(url,W2A(strDanHao)); // ����

		CURLcode res;
	
		curl_easy_setopt(curl,CURLOPT_URL,url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, getid_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		res = curl_easy_perform(curl);
	}
}

// ���а�ť
void CClientDlg::OnBnClickedButtonFangxing()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	// �ύһ��POST����
	// ��Ҫ�ύ�Ÿڷ��е�״̬��

	USES_CONVERSION;
	char data[1024]={0};
	CString strDanHao;
	m_id.GetWindowText(strDanHao); // ����
	sprintf_s(data,"DanHao=%s&ZhuangTai=8&",W2A(strDanHao)); // ���ź�״̬

	CString strUser;
	m_user.GetWindowText(strUser);
	sprintf_s(data,"%sFangXingYuan=%s",data, W2A(strUser)); // ����Ա

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
		sprintf_s(tmp,"COM%d:%s",conf.com1_id,conf.com1_para);
		USES_CONVERSION;
		m_com1.SetWindowText(A2CW(tmp)); // ���ô���1���ֵ
	}
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

// ��½��������
size_t CClientDlg::login_data(void *ptr, size_t size, size_t nmemb, void *userp)
{
	CClientDlg *client =(CClientDlg*)userp;
	client->m_user.EnableWindow(FALSE); // �����û������
	client->m_pwd.EnableWindow(FALSE);  // ������������� 
	client->m_btn_login.EnableWindow(FALSE); // ���õ�¼��ť
	client->m_id.EnableWindow(TRUE); // ���õ��ſؼ�
	client->m_id.SetFocus(); // ���õ���Ϊ����
	
	return size*nmemb;
}

// ���ŷ�������
size_t CClientDlg::getid_data(void *ptr, size_t size, size_t nmemb, void *userp)
{
	CClientDlg *client =(CClientDlg*)userp;

	USES_CONVERSION; 
	cJSON *jsonroot = cJSON_Parse((const char*)ptr); //json��
	if(jsonroot)
	{
		char *strid = cJSON_GetObjectItem(jsonroot,"id")->valuestring; // ����
		if(strcmp(strid,"0")==0)
		{
			client->MessageBox(L"�޴˵��ţ�����");
			return size*nmemb; // ���غ���
		}

		char *ZhuangTai = cJSON_GetObjectItem(jsonroot,"zt")->valuestring; // ��÷���״̬
		if(strcmp(ZhuangTai,"0")==0) 
		{
			client->MessageBox(L"���ɷ��У�δ�ڶ��ι���������");
			// ���õ���Ϊ����
			client->m_id.SetFocus();
			client->m_fangxing.EnableWindow(FALSE); // ���÷��а�ť
		}
		if(strcmp(ZhuangTai,"1")==0) // �������״̬Ϊ1�����������json�������С�
		{
			client->m_chehao.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"ch")->valuestring))); // ����
			client->m_dianhua.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"dh")->valuestring)); // �绰
			client->m_shouhuo.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"dw")->valuestring))); // ��λ
			client->m_huowu.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"hw")->valuestring))); // ����
			client->m_guige.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"gg")->valuestring)); // ���
			client->m_chexing.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"cx")->valuestring))); // ����
			client->m_pizhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"pz")->valuestring)); // Ƥ��
			client->m_maozhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"mz")->valuestring)); // ë��
			client->m_jingzhong.SetWindowText(A2CW(cJSON_GetObjectItem(jsonroot,"jz")->valuestring)); // ����
			client->m_danjia.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"dj")->valuestring))); // ����
			client->m_jine.SetWindowText(A2CW(UTF8ToEncode(cJSON_GetObjectItem(jsonroot,"je")->valuestring))); // ���
			client->m_fangxing.EnableWindow(TRUE); // ���÷��а�ť
		}
		if(strcmp(ZhuangTai,"2")==0) 
		{
			// ��ȡ����ʱ��
			char *strChuChang = new char[32];
			memset(strChuChang,0,32);
			strChuChang = cJSON_GetObjectItem(jsonroot,"cc")->valuestring; // ����ʱ��
			if(strcmp(strChuChang,"")==0)
			{
				client->MessageBox(L"�Ѿ����г����������ٴη��У�����");
			}
			else
			{
				// ��ʱ���ַ���ת������
				// �ж���5�����ڣ�����ٷ��С�
				CString stime = A2CW(strChuChang); 
				COleDateTime CurrTime = COleDateTime::GetCurrentTime(); 
				COleDateTime tempTime; 
				tempTime.ParseDateTime(stime); 
				COleDateTimeSpan  result = CurrTime - tempTime; 
				double sec = result.GetTotalSeconds();
				if(sec<=5*60) // 5����
				{
					client->OnFangXing(); // ����
					client->MessageBox(L"���гɹ���");
				}
				else
				{
					client->MessageBox(L"��������ʱ�䣬���ɷ��У�����");
				}
				delete[] strChuChang;
			}
		}
	}
//	cJSON_Delete(jsonroot);
	return size*nmemb;
}

// �ύ���ݷ�������
size_t CClientDlg::post_data(void *ptr, size_t size, size_t nmemb, void *userp)
{
	CClientDlg *client =(CClientDlg*)userp;

	char str[16]={0};
	memcpy(str,ptr,5);
	// �����ж��ύ�Ƿ�ɹ�
	if(strcmp(str,"post3")==0)
	{
		client->OnFangXing(); // ����
		client->MessageBox(L"���гɹ���");
	}
	return size*nmemb;
}