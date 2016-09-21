
// AeroPanelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AeroPanel.h"
#include "AeroPanelDlg.h"
#include "afxdialogex.h"
#include "win10.h"

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


// CAeroPanelDlg �Ի���


CAeroPanelDlg::CAeroPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAeroPanelDlg::IDD, pParent)
	, m_hotkeyid(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bDwm = FALSE;
	m_use_aero = FALSE;
	m_hotkey = 'Z';
	m_trywin10 = TRUE;
	m_prevtick = GetTickCount64();
}

void CAeroPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAeroPanelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_WM_NCRBUTTONUP()
	ON_COMMAND(ID_APP_ABOUT, &CAeroPanelDlg::OnAppAbout)
	ON_COMMAND(ID_POP_CLOSE, &CAeroPanelDlg::OnPopClose)
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_COMMAND(ID_POP_ICONIC, &CAeroPanelDlg::OnPopIconic)
	ON_COMMAND(ID_POP_SWITCH, &CAeroPanelDlg::OnPopSwitch)
//	ON_WM_NCPAINT()
	ON_WM_THEMECHANGED()
//	ON_WM_TIMER()
	ON_WM_TIMER()
	ON_COMMAND(ID_POP_ADV_TRANSPARENT, &CAeroPanelDlg::OnPopAdvTransparent)
	//ON_COMMAND(ID_POP_MAXIMIZE, &CAeroPanelDlg::OnPopMaximize)
	ON_WM_HOTKEY()
//	ON_UPDATE_COMMAND_UI(ID_POP_ADV_TRANSPARENT, &CAeroPanelDlg::OnUpdatePopAdvTransparent)
	ON_COMMAND_RANGE(ID_POP_ADV_HK_Z, ID_POP_ADV_HK_E, &CAeroPanelDlg::OnPopAdvHotkey)
//	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_POP_WIN10, &CAeroPanelDlg::OnPopWin10)
	ON_WM_SYSKEYDOWN()
END_MESSAGE_MAP()


// CAeroPanelDlg ��Ϣ�������

BOOL CAeroPanelDlg::OnInitDialog()
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
	m_hotkeyid = GlobalAddAtom(_T("AeroPanelKey")) - 0xC000; 

	// Ҫʵ��WS_EX_TRANSPARENT������WS_EX_LAYERED�ұ�������ALPHA������������
	SetWindowLong(m_hWnd, GWL_EXSTYLE,
		GetWindowLong(m_hWnd, GWL_EXSTYLE)|WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 255, LWA_ALPHA);

	// ��ע�����ش��ڴ�С
	LoadProgSettings();
	
	// ���Լ���Aero
	TryAeroAndBlur();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAeroPanelDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAeroPanelDlg::OnPaint()
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
		SetTimer(5, 10, NULL); // ����������ֹAboutBoxʧȥ��Ӧ
	}
}

void CAeroPanelDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 5) {
		KillTimer(nIDEvent);
		TryAeroAndBlur();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAeroPanelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL ToggleWin7Aero(HWND hwnd, BOOL bEnable)
{
	BOOL bDwm;
	DwmIsCompositionEnabled(&bDwm);
	if (bEnable && bDwm)
	{
		// ��Aero
		MARGINS mrg = {-1}; 
		DwmExtendFrameIntoClientArea(hwnd , &mrg);
	}
	else
	{
		// �ر�Aero
		MARGINS mrg = {0, 0, 0, 0}; 
		DwmExtendFrameIntoClientArea(hwnd , &mrg);
		return FALSE;
	}
}

BOOL ToggleWin10Blur(HWND hwnd, BOOL bEnable)
{
	HMODULE hUser = GetModuleHandle(L"user32.dll");
	if (hUser == NULL) return FALSE;
	pfnSetWindowCompositionAttribute setWindowCompositionAttribute = (pfnSetWindowCompositionAttribute)GetProcAddress(hUser, "SetWindowCompositionAttribute");
	if (setWindowCompositionAttribute == NULL) return FALSE;
	// second = 0x20|0x40|0x80|0x100 : show hidden borders
	ACCENT_POLICY accent = { bEnable ? ACCENT_ENABLE_BLURBEHIND : ACCENT_DISABLED, 0, 0, 0 };
	WINDOWCOMPOSITIONATTRIBDATA data;
	data.dwAttrib = WCA_ACCENT_POLICY;
	data.pvData = &accent;
	data.cbData = sizeof(accent);
	return setWindowCompositionAttribute(hwnd, &data) && bEnable;
}

void CAeroPanelDlg::AdjustBack(void)
{
	if (m_use_aero || m_trywin10)
	{
		SetBackgroundColor(RGB(0, 0, 0), TRUE);
	}
	else
	{
		SetBackgroundColor(GetSysColor(COLOR_INACTIVEBORDER), TRUE);
	}
	if (m_trywin10)
	{
		DWORD style = GetWindowLong(m_hWnd, GWL_STYLE);
		style &= ~WS_SIZEBOX;
		SetWindowLong(m_hWnd, GWL_STYLE, style);
	}
	else
	{
		DWORD style = GetWindowLong(m_hWnd, GWL_STYLE);
		style |= WS_SIZEBOX;
		SetWindowLong(m_hWnd, GWL_STYLE, style);
	}
}

#if 0
CString caption;
caption.LoadString(IDS_CAPTION_NOAERO);
SetWindowText(caption);
#endif

void CAeroPanelDlg::MakeAeroOrBlur()
{
	if (m_trywin10 && m_use_aero)
	{
		m_trywin10 = false;
		m_use_aero = false;
	}
	m_trywin10 = ToggleWin10Blur(m_hWnd, m_trywin10);
	m_use_aero = ToggleWin7Aero(m_hWnd, m_use_aero);
	AdjustBack();
}

void CAeroPanelDlg::TryAeroAndBlur()
{
	MakeAeroOrBlur();
}

void CAeroPanelDlg::OnPopWin10()
{
	m_trywin10 = !m_trywin10;
	m_use_aero = m_use_aero && !m_trywin10;
	TryAeroAndBlur();
}

LRESULT CAeroPanelDlg::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	LRESULT retval = CDialogEx::OnNcHitTest(point);

	return (retval == HTCLIENT) ? HTCAPTION : retval; // ʹ�ͻ�����Ϊ����������
}

void CAeroPanelDlg::OnNcRButtonUp(UINT nHitTest, CPoint point)
{
	// �����Ҽ��˵�

	//ClientToScreen(&point);
	this->SetForegroundWindow();
	CMenu menu, *pmenu, *padvmenu;
	menu.LoadMenu(IDR_POPMENU);
	pmenu = menu.GetSubMenu(0);
	padvmenu = pmenu->GetSubMenu(3);

	// ���¿�ݼ�ָʾ
	CString fmt; fmt.LoadString(IDS_ADVTRANS);
	fmt.Format(fmt, m_hotkey);
	padvmenu->ModifyMenu(0, MF_BYPOSITION|MF_STRING, ID_POP_ADV_TRANSPARENT, fmt);

	pmenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

	CDialogEx::OnNcRButtonUp(nHitTest, point);
}


void CAeroPanelDlg::OnAppAbout()
{
	// ��ʾ���ڶԻ���
	CAboutDlg dlg;
	dlg.DoModal();
}

void CAeroPanelDlg::OnPopIconic()
{
	// ��С������
	CloseWindow();
}

void CAeroPanelDlg::OnPopClose()
{
	// �رմ���
	EndDialog(IDOK);
}

void CAeroPanelDlg::OnPopSwitch()
{
	// �л�Aero
	m_use_aero = !m_use_aero;
	m_trywin10 = m_trywin10 && !m_use_aero;
	TryAeroAndBlur();
}

void CAeroPanelDlg::LoadProgSettings(void)
{
	// ��ע�����ش��ڴ�С
	// ע�⣺ֻ��GetWindowPlacement��SetWindowPlacement�ɶ�ʹ�òſɱ�֤��С���仯
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	wp.rcNormalPosition.left = AfxGetApp()->GetProfileInt(_T("windowlocation"), _T("left"), 10);
	wp.rcNormalPosition.top = AfxGetApp()->GetProfileInt(_T("windowlocation"), _T("top"), 10);
	wp.rcNormalPosition.right = AfxGetApp()->GetProfileInt(_T("windowlocation"), _T("right"), 500);
	wp.rcNormalPosition.bottom = AfxGetApp()->GetProfileInt(_T("windowlocation"), _T("bottom"), 100);
	SetWindowPlacement(&wp);
	
	m_use_aero = AfxGetApp()->GetProfileInt(_T("windowstyle"), _T("aero"), TRUE);
	m_trywin10 = AfxGetApp()->GetProfileInt(_T("windowstyle"), _T("win10"), TRUE);

	m_hotkey = AfxGetApp()->GetProfileInt(_T("advfunc"), _T("transhotkey"), 'Z');
	RegisterHotKey(m_hWnd, m_hotkeyid, MOD_CONTROL|MOD_ALT, m_hotkey);
}

void CAeroPanelDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// �����ڴ�С���浽ע���
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);

	AfxGetApp()->WriteProfileInt(_T("windowlocation"), _T("left"), wp.rcNormalPosition.left);
	AfxGetApp()->WriteProfileInt(_T("windowlocation"), _T("top"), wp.rcNormalPosition.top);
	AfxGetApp()->WriteProfileInt(_T("windowlocation"), _T("right"), wp.rcNormalPosition.right);
	AfxGetApp()->WriteProfileInt(_T("windowlocation"), _T("bottom"), wp.rcNormalPosition.bottom);
	
	AfxGetApp()->WriteProfileInt(_T("windowstyle"), _T("aero"), m_use_aero);
	AfxGetApp()->WriteProfileInt(_T("windowstyle"), _T("win10"), m_trywin10);

	AfxGetApp()->WriteProfileInt(_T("advfunc"), _T("transhotkey"), m_hotkey);
}

void CAeroPanelDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// ˢ��Aero״̬
	COLORREF bkclr;
	if (m_use_aero && bDwm) {
		bkclr = RGB(0, 0, 0);
		SetBackgroundColor(bkclr);
	}
}


LRESULT CAeroPanelDlg::OnThemeChanged()
{
	// �ù���Ҫ��ʹ�� Windows XP ����߰汾��
	// ���� _WIN32_WINNT ���� >= 0x0501��
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	TryAeroAndBlur(); // Ȼ����û��ʲô��

	return 1;
}

void CAeroPanelDlg::OnPopAdvTransparent()
{
	// ���ô��ڶ����͸�� - ��ҪWS_EX_LAYERED��ALPHA=255
	SetWindowLong(m_hWnd, GWL_EXSTYLE,
		GetWindowLong(m_hWnd, GWL_EXSTYLE)|WS_EX_TRANSPARENT);
}

void CAeroPanelDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// �ӵ��ȼ� - ���ô��ڶ���겻͸�� - ��ҪWS_EX_LAYERED��ALPHA=255
	SetWindowLong(m_hWnd, GWL_EXSTYLE,
		GetWindowLong(m_hWnd, GWL_EXSTYLE)&~WS_EX_TRANSPARENT);

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

void CAeroPanelDlg::OnPopAdvHotkey(UINT nID)
{
	switch (nID)
	{
	case ID_POP_ADV_HK_Z: m_hotkey = 'Z'; break;
	case ID_POP_ADV_HK_X: m_hotkey = 'X'; break;
	case ID_POP_ADV_HK_C: m_hotkey = 'C'; break;
	case ID_POP_ADV_HK_A: m_hotkey = 'A'; break;
	case ID_POP_ADV_HK_S: m_hotkey = 'S'; break;
	case ID_POP_ADV_HK_D: m_hotkey = 'D'; break;
	case ID_POP_ADV_HK_Q: m_hotkey = 'Q'; break;
	case ID_POP_ADV_HK_W: m_hotkey = 'W'; break;
	case ID_POP_ADV_HK_E: m_hotkey = 'E'; break;
	}
	UnregisterHotKey(m_hWnd, m_hotkeyid);
	RegisterHotKey(m_hWnd, m_hotkeyid, MOD_CONTROL|MOD_ALT, m_hotkey);
}


void CAeroPanelDlg::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	RECT rc;
	GetWindowRect(&rc);
	rc.bottom -= rc.top;
	rc.right -= rc.left;
	rc.top = rc.left = 0;

	static float fac = 1.0f;
	ULONGLONG tick = GetTickCount64();
	if (tick - m_prevtick >= 300)
	{
		fac = 1.0f;
	}
	else
	{
		fac += 0.5f;
	}
	m_prevtick = tick;

	CWindowDC dc(this);
	float step = dc.GetDeviceCaps(LOGPIXELSX) / 96.0f * fac;
	step = max(1.0f, step);
	
	if (nChar == VK_LEFT || nChar == 'A')
	{
		rc.right -= step;
	}
	if (nChar == VK_RIGHT || nChar == 'D')
	{
		rc.right += step;
	}
	if (nChar == VK_UP || nChar == 'W')
	{
		rc.bottom -= step;
	}
	if (nChar == VK_DOWN || nChar == 'S')
	{
		rc.bottom += step;
	}
	SetWindowPos(NULL, 0, 0, rc.right, rc.bottom, SWP_NOMOVE);
	CDialogEx::OnSysKeyDown(nChar, nRepCnt, nFlags);
}
