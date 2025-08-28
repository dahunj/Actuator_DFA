// WorkDlg.h : 헤더 파일
//
#pragma once
#include "LoopTimer.h"

#define UM_UPDATE_MODEL			WM_USER+1
#define UM_UPDATE_TRAY_INFO		WM_USER+2
#define UM_UPDATE_BARCODE_RS232	WM_USER+3
#define UM_RESET_CYCLE_STOP		WM_USER+4
#define UM_UPDATE_UPH			WM_USER+5
#define UM_JOB_COMPELTE			WM_USER+6
#define UM_LOT_END_MSG			WM_USER+7	//2018.8.27+
#define UM_UPDATE_BARCODE_COGNEX WM_USER+8

// CWorkDlg 대화 상자입니다.

class CWorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkDlg)

public:
	CWorkDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWorkDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_WORK_DLG };
	CGroupCS		m_Group[9];
	CLabelCS		m_Label[4];
	CRadioCS		m_rdoSlectNo[28];
	CStaticCS		m_stcLotsIdS[28];
	CStaticCS		m_stcCmsCountS[28];
	CPictureCS		m_imgEquipment;
	CLedCS			m_ledEquipOption[9];
	CLedCS			m_ledVisionSts[6];
	CLedCS			m_ledBarcodeSts[6];
	CStaticCS		m_stcEquipType;
	CStaticCS		m_stcJobLotID[17];
	CStaticCS		m_stcJobCarNo[17];
	CRadioCS		m_rdoWorkStart;
	CRadioCS		m_rdoWorkStop;
	CLedCS			m_ledInitComplete;
	CCheckCS		m_chkCycleStop;
	CPictureCS		m_picTrayBack[5];
	CStaticCS		m_stcAlignLine;
	CStaticCS		m_stcCarrierLine;
	CStaticCS		m_stcNGLine;
	CStaticCS		m_stcGoodLine;
	CGridCS			m_grdAlign;
	CGridCS			m_grdCarrier;
	CGridCS			m_grdNGTray;
	CGridCS			m_grdGoodTray;
	CGridCS			m_grdJob;
	CStaticCS		m_stcWorkCase[33];
	CStaticCS		m_stcMZLotID[7];
	CStaticCS		m_stcMZCarNo[7];
	CStaticCS		m_stcCVMZCheck[7];
	CLedCS			m_ledTrayCheck[15];
	CStaticCS		m_stcStageInfor[4];

	CLabelCS		m_lblOperId;
	CStaticCS		m_stcOperId;
	CStaticCS		m_stcMesConnect;
	CStaticCS		m_stcMesOnline;
	CStaticCS		m_stcNGMZCar[8];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStcLotsIdSClick(UINT nID);
	afx_msg void OnStcCmsCountSClick(UINT nID);
	afx_msg void OnBnClickedRdoSlectNo(UINT nID);
	afx_msg void OnBnClickedRdoWorkStart();
	afx_msg void OnBnClickedRdoWorkStop();
	afx_msg void OnBnClickedChkCycleStop();
	afx_msg LRESULT OnUpdateModel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateTrayInfo(WPARAM nTray, LPARAM lParam);
	afx_msg LRESULT OnUpdateBarcode_RS232(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateBarcode_Cognex(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnResetCycleStop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateUph(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnJobComplete(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowLotEndMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnBuzzerOff();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedBtnLotCancel();

	afx_msg void OnBnClickedBtnMesConnect();
	afx_msg void OnBnClickedBtnMesDisconnect();
	afx_msg void OnBnClickedBtnMesOnline();
	afx_msg void OnBnClickedBtnMesOffline();
	afx_msg void OnBnClickedBtnMesAbort();
	afx_msg void OnBnClickedBtnIdleReport();
	afx_msg void OnStnClickedOperId();
	afx_msg void OnStnClickedCarrier();
	afx_msg void OnStnClickedStcMzLotId6();

	BOOL		m_bThreadElevatorRun;
	CWinThread	*m_pThreadElevatorRun;
	static UINT	Thread_ElevatorRun(LPVOID lpVoid);

private:
	CBitmap	m_bmpEquipment;
	BOOL	m_bShowWindow;
	DWORD	dwStopSTime;

	CString m_sJobListLotID[8];
	CString	m_sJobListLotCnt[8];
	CString	m_sJobListTryCnt[8];

private:
	int			m_nWorkEleatorCase1;
	int			m_nWorkEleatorCase2;
	int			m_nWorkEleatorCase3;
	int			m_nWorkEleatorCase4;
	int			m_nWorkEleatorCase5;
	int			m_nWorkEleatorCase6;
	int			m_nWorkEleatorCase7;
	int			m_nWorkUnloadConveyor1F;
	int			m_nWorkUnloadConveyor2F;

	CLoopTimer	m_tWorkElevatorLoop1;
	CLoopTimer	m_tWorkElevatorLoop2;	
	CLoopTimer	m_tWorkElevatorLoop3;	
	CLoopTimer	m_tWorkElevatorLoop4;	
	CLoopTimer	m_tWorkElevatorLoop5;	
	CLoopTimer	m_tWorkElevatorLoop6;
	CLoopTimer	m_tWorkElevatorLoop7;
	CLoopTimer	m_tnWorkUnloadConveyor1F;
	CLoopTimer	m_tnWorkUnloadConveyor2F;

	BOOL Work_Start();
	void Initial_Controls();
	void Initial_Grid(CGridCS *pGrid, int nRows, int nCols);
	void Initial_JobGrid(CGridCS *pGrid, int nRows, int nCols);
	void Display_Status();
	void Display_TrayCheck();
	void Reset_AlarmLog();
	BOOL LotID_Check();
	void Reset_TackData();
	void ReadJobList();
	void SaveJobList();
	BOOL BarcodeLotID_Check(CString sLotID);

	void DoEvents(int nSleep = 0);
	void ElevatorOpen(int nEleNo);
	BOOL ElevatorOpen1();
	BOOL ElevatorOpen2();
	BOOL ElevatorOpen3();
	BOOL ElevatorOpen4();
	BOOL ElevatorOpen5();
	BOOL ElevatorOpen6();
	BOOL ElevatorOpen7();
	BOOL UnloadConveyor1F();
	BOOL UnloadConveyor2F();
	void ConveyorOpen(int nCyNo, int nCmd);
	void UnloadConveyorStop();
public:
	BOOL m_bAutoRunning;

	void Begin_ElevatorRunThread();
	void End_ElevatorRunThread();

	void Set_AutoRun(BOOL bAutoRun);
	void Set_AutoRunStatus(BOOL bAutoRun) { m_bAutoRunning = bAutoRun; }
};

extern CWorkDlg g_dlgWork;

///////////////////////////////////////////////////////////////////////////////
