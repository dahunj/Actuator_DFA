#pragma once


// COCAPSelectDlg 대화 상자입니다.

class COCAPSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COCAPSelectDlg)

public:
	COCAPSelectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COCAPSelectDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OCAP_SELECT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CButtonCS	m_btnCosmeticOCAP; 
	CButtonCS	m_btnFAIOCAP; 
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnCosmetic();
	afx_msg void OnBnClickedBtnFai();
};

extern COCAPSelectDlg g_dlgOCAPSelect;