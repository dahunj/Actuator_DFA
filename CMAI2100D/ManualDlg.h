// ManualDlg.h : 헤더 파일
//
#pragma once

#include "ManualElevatorDlg.h"
#include "ManualInspDlg.h"
#include "ManualLoadDlg.h"
#include "ManualUnloadDlg.h"
#include "ManualMzLoadDlg.h"
#include "ManualMzUnloadDlg.h"

// CManualDlg 대화 상자입니다.

class CManualDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CManualDlg)

public:
	CManualDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CManualDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MANUAL_DLG };
	CPictureCS	m_picManualBack;
	CRadioCS	m_rdoManualElevator;
	CRadioCS	m_rdoManualLoad;
	CRadioCS	m_rdoManualInsp;
	CRadioCS	m_rdoManualUnload;
	CRadioCS	m_rdoManualMzLoad;
	CRadioCS	m_rdoManualMzUnload;
	CRadioCS	m_rdoManualDoorLock;
	CRadioCS	m_rdoManualDoorUnlock;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRdoManualElevator();
	afx_msg void OnBnClickedRdoManualConveyor();
	afx_msg void OnBnClickedRdoManualLoad();
	afx_msg void OnBnClickedRdoManualInsp();
	afx_msg void OnBnClickedRdoManualUnload();
	afx_msg void OnBnClickedRdoManualMzLoad();
	afx_msg void OnBnClickedRdoManualMzUnload();

public:
	CManualInspDlg		*m_pManualInspDlg;
	CManualElevatorDlg	*m_pManualElevatorDlg;
	CManualLoadDlg		*m_pManualLoadDlg;
	CManualUnloadDlg	*m_pManualUnloadDlg;
	CManualMzLoadDlg	*m_pManualMzLoadDlg;
	CManualMzUnloadDlg	*m_pManualMzUnloadDlg;

private:
	void Initial_Controls();
	void Hide_Windows();
public:
	afx_msg void OnBnClickedRdoManualDoorLock();
	afx_msg void OnBnClickedRdoManualDoorUnlock();
};

extern CManualDlg g_dlgManual;

///////////////////////////////////////////////////////////////////////////////
