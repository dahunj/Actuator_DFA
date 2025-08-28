// ManualMzUnloadDlg.h : 헤더 파일
//
#pragma once

#include "LoopTimer.h"
// CManualMzUnloadDlg 대화 상자입니다.

class CManualMzUnloadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualMzUnloadDlg)

public:
	CManualMzUnloadDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualMzUnloadDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_MZ_UNLOAD_DLG };
	CGroupCS	m_Group[12];
	CLabelCS	m_Label[5];
	CStaticCS	m_stcAxisPos[5];
	CButtonCS	m_btnNGFeederY[3];
	CButtonCS	m_btnNGFeederIO[2];
	CButtonCS	m_btnGoodFeederY[3];
	CButtonCS	m_btnGoodFeederIO[2];
	CButtonCS	m_btnNGElevatorZ[3];
	CButtonCS	m_btnNGElevatorIO[4];
	CButtonCS	m_btnGoodElevatorZ[3];
	CButtonCS	m_btnGoodElevatorIO[4];
	CButtonCS	m_btnULCVElevatorZ[3];
	CCheckCS	m_chkULCVElevatorIO[2];
	CCheckCS	m_chkMZUnloadCVIO[8];

	CLedCS		m_ledNGFeederIO[7];
	CLedCS		m_ledGoodFeederIO[7];
	CLedCS		m_ledNGElevatorIO[5];
	CLedCS		m_ledGoodElevatorIO[5];
	CLedCS		m_ledULCVElevatorIO[2];
	CLedCS		m_ledMZUnloadCVIO[14];

	CLabelCS	m_lblBarcode[4];
	CStaticCS	m_stcBarcode[4];
	CButtonCS	m_btnBarcode[9];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg void OnbtnNGFeederYClick(UINT nID);
	afx_msg void OnbtnNGFeederIOClick(UINT nID);
	afx_msg void OnbtnGoodFeederYClick(UINT nID);
	afx_msg void OnbtnGoodFeederIOClick(UINT nID);
	afx_msg void OnbtnNGElevatorZClick(UINT nID);
	afx_msg void OnbtnNGElevatorIOClick(UINT nID);
	afx_msg void OnbtnGoodElevatorZClick(UINT nID);
	afx_msg void OnbtnGoodElevatorIOClick(UINT nID);
	afx_msg void OnbtnULCVElevatorZClick(UINT nID);
	afx_msg void OnchkULCVElevatorIOClick(UINT nID);
	afx_msg void OnchkMZUnlaodCVIOClick(UINT nID);
	afx_msg void OnBtnULBarcodeClick(UINT nID);

private:
	CString		m_strLog;

	void Initial_Controls();

protected:

public:
	void Display_Status();
};
