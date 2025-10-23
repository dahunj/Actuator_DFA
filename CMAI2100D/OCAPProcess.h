#pragma once

// OCAPProcess 대화 상자입니다.

class OCAPProcess : public CDialogEx
{
	DECLARE_DYNAMIC(OCAPProcess)

public:
	OCAPProcess(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~OCAPProcess();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OCAP_DLG };
	CGroupCS		m_Group[2];
	CLabelCS		m_Label[7];
	CStaticCS		m_stcOption[9];
	CGridCS			m_grdData;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnStcOptionClick(UINT nID);

private:
	BOOL m_bError;

	void Initial_NameGrid(CGridCS *pGrid, int nRows, int nCols);
	void Initial_DataGrid(CGridCS *pGrid, int nRows, int nCols);

	void Initial_Controls();
	void Display_Option();
	void Display_Grid(int nDp, int nIndex);

public:
	void Display_Status();
	void Set_AddMZData(int nPortNo);
	void Set_AddDEFECT(CString sMZid, CString sCode);
	void Check_DEFECT(int nNo);
	void Check_DEFECTF(int nNo);
	void AddCarToMZ(int nPortNo, int nIndex);
	void AddMZOut(CString sMZid);
	void DelMZData(int nMZNo);
};

extern OCAPProcess g_dlgOCAP;