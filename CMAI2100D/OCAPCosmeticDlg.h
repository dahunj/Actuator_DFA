#pragma once

// OCAPCosmeticDlg 대화 상자입니다.

class OCAPCosmeticDlg : public CDialogEx
{
	DECLARE_DYNAMIC(OCAPCosmeticDlg)

public:
	OCAPCosmeticDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~OCAPCosmeticDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OCAP_COSMETIC_DLG };
	CGroupCS		m_Group[2];
	CLabelCS		m_Label[7];
	//CStaticCS		m_stcOption[9];
	CStaticCS		m_stcConsMZ[3];
	CStaticCS		m_stcPercent[3];
	CStaticCS		m_stcMinCount;

	CGridCS			m_grdData;

	CString			m_strLog;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnStcConstMagazineClick(UINT nID);
	afx_msg void OnStcNGPercentClick(UINT nID);

private:
	BOOL m_bError;

	void Initial_NameGrid(CGridCS *pGrid, int nRows, int nCols);
	void Initial_DataGrid(CGridCS *pGrid, int nRows, int nCols);

	void Initial_Controls();
	void Display_Option();
	void Display_Grid(int nDp, int nIndex);

public:
	void AddModuleToCarrier(int nSlotNo, CString sType, int nJudge, CString sNGCode, int& nCurrentMzIdx);
	void AddCarToMZ(int nSlotNo, CString sType); //Type - 0 :Good, 1: NG 
	void AddMZOut(CString sMZid, CString sType);
	
	void Display_Status();
	void Set_AddMZData(int nPortNo);
	void Set_AddDEFECT(CString sMZid, int nPortNo, int nInfo);
	void Check_DEFECT(CString sMZID);
	void Check_DEFECTF(int nNo);
	
	
	void DelMZData(int nMZNo);
	afx_msg void OnBnClickedBtnTest();
};

extern OCAPCosmeticDlg g_dlgOCAPCosmetic;