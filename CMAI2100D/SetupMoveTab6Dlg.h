// SetupMoveTab6Dlg.h : 헤더 파일
//
#pragma once

// CSetupMoveTab6Dlg 대화 상자입니다.

class CSetupMoveTab6Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupMoveTab6Dlg)

public:
	CSetupMoveTab6Dlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupMoveTab6Dlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_MOVE_TAB6_DLG };
	CGroupCS	m_Group[10];
	CLabelCS	m_Label[37];
	CStaticCS	m_stcLoadFeederY[3];
	CStaticCS	m_stcNGFeederY[3];
	CStaticCS	m_stcGoodFeederY[3];
	CStaticCS	m_stcLDCVElevatorZ[3];
	CStaticCS	m_stcLDMZElevatorZ[3];
	CStaticCS	m_stcNGMZElevatorZ[3];
	CStaticCS	m_stcGDMZElevatorZ[3];
	CStaticCS	m_stcULCVElevatorZ[3];
	CStaticCS	m_stcMZTransferX[6];
	CStaticCS	m_stcMZTransferZ[7];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStcLoadFeederY(UINT nID);
	afx_msg void OnStcNGFeederY(UINT nID);
	afx_msg void OnStcGoodFeederY(UINT nID);
	afx_msg void OnStcLDCVElevator(UINT nID);
	afx_msg void OnStcLDMZElevatorZ(UINT nID);
	afx_msg void OnStcNGMZElevatorZ(UINT nID);
	afx_msg void OnStcGDMZElevatorZ(UINT nID);
	afx_msg void OnStcULCVElevator(UINT nID);
	afx_msg void OnStcMZTransferX(UINT nID);
	afx_msg void OnStcMZTransferZ(UINT nID);

private:
	CString	m_strLog;
	void Initial_Controls();
	void Display_MoveData();

public:
	void Save_MoveData();
	void Cancel_MoveData();
};

///////////////////////////////////////////////////////////////////////////////
