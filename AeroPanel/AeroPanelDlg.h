
// AeroPanelDlg.h : ͷ�ļ�
//

#pragma once


// CAeroPanelDlg �Ի���
class CAeroPanelDlg : public CDialogEx
{
// ����
public:
	CAeroPanelDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AEROPANEL_DIALOG };
	BOOL bDwm; // �Ƿ�֧��Aero�ı�־
	BOOL m_use_aero; // �Ƿ���Aero�ı�־
	BOOL m_trywin10; // �Ƿ�Win10�ı�־
	ULONGLONG m_prevtick; // ��һ������

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnAppAbout();
	afx_msg void OnPopClose();
	void LoadProgSettings(void);
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnPopIconic();
//	afx_msg void OnNcPaint();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg BOOL OnNcActivate(BOOL bActive);
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnPopSwitch();
	void AdjustBack(void);
	void TryAeroAndBlur(void);
//	afx_msg void OnNcPaint();
	LRESULT OnThemeChanged();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPopAdvTransparent();
//	afx_msg void OnPopMaximize();
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	UINT m_hotkey;
//	afx_msg void OnUpdatePopAdvTransparent(CCmdUI *pCmdUI);
	void OnPopAdvHotkey(UINT nID);
	int m_hotkeyid;
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPopWin10();
	void MakeAeroOrBlur();
};
