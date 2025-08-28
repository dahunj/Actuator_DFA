// ManualMzLoadDlg.h : 헤더 파일
//
#pragma once

#include "LoopTimer.h"
// CManualMzLoadDlg 대화 상자입니다.

class CManualMzLoadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualMzLoadDlg)

public:
	CManualMzLoadDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualMzLoadDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_MZ_LOAD_DLG };
	CGroupCS	m_Group[11];
	CLabelCS	m_Label[5];
	CStaticCS	m_stcAxisPos[5];
	CCheckCS	m_chkMZLoadCVIO[8];
	CButtonCS	m_btnMZLoadCVIO[4];
	CButtonCS	m_btnLDCVElevatorZ[3];
	CCheckCS	m_chkLDCVElevatorIO[2];
	CButtonCS	m_btnLDCVElevatorIO[6];
	CButtonCS	m_btnLoadFeederY[3];
	CButtonCS	m_btnLoadFeederIO[2];
	CButtonCS	m_btnLoadElevatorZ[3];
	CButtonCS	m_btnLoadElevatorIO[4];
	CButtonCS	m_btnMZTransferX[6];
	CButtonCS	m_btnMZTransferZ[7];
	CButtonCS	m_btnMZTransferIO[8];

	CLedCS		m_ledMZLoadCVIO[18];
	CLedCS		m_ledLDCVElevatorIO[8];
	CLedCS		m_ledLoadFeederIO[7];
	CLedCS		m_ledLoadElevatorIO[6];
	CLedCS		m_ledMZTransferIO[10];

	CLabelCS	m_lblBarcode[3];
	CStaticCS	m_stcBarcode[3];
	CButtonCS	m_btnBarcode[9];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnchkMZLoadCVIOClick(UINT nID);
	afx_msg void OnbtnMZLoadCVIOClick(UINT nID);
	afx_msg void OnbtnLDCVElevatorZClick(UINT nID);
	afx_msg void OnchkLDCVElevatorIOClick(UINT nID);
	afx_msg void OnbtnLDCVElevatorIOClick(UINT nID);
	afx_msg void OnbtnLoadFeederYClick(UINT nID);
	afx_msg void OnbtnLoadFeederIOClick(UINT nID);
	afx_msg void OnbtnLoadElevatorZClick(UINT nID);
	afx_msg void OnbtnLoadElevatorIOClick(UINT nID);
	afx_msg void OnbtnMZTransferXClick(UINT nID);
	afx_msg void OnbtnMZTransferZClick(UINT nID);
	afx_msg void OnbtnMZTransferIOClick(UINT nID);
	afx_msg void OnBtnBarcodeClick(UINT nID);

private:
	CString		m_strLog;

	void Initial_Controls();

protected:

public:
	void Display_Status();
};
