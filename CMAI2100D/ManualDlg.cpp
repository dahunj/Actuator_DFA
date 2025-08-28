// ManualDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMAI2100.h"
#include "ManualDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "Common.h"
#include "DataManager.h"

#include "CMAI2100Dlg.h"

// CManualDlg 대화 상자입니다.

CManualDlg g_dlgManual;

IMPLEMENT_DYNAMIC(CManualDlg, CDialogEx)

CManualDlg::CManualDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualDlg::IDD, pParent)
{
}

CManualDlg::~CManualDlg()
{
}

void CManualDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC_MANUAL_BACK, m_picManualBack);
	DDX_Control(pDX, IDC_RDO_MANUAL_ELEVATOR, m_rdoManualElevator);
	DDX_Control(pDX, IDC_RDO_MANUAL_LOAD, m_rdoManualLoad);
	DDX_Control(pDX, IDC_RDO_MANUAL_INSP, m_rdoManualInsp);
	DDX_Control(pDX, IDC_RDO_MANUAL_UNLOAD, m_rdoManualUnload);
	DDX_Control(pDX, IDC_RDO_MANUAL_MZ_LOAD, m_rdoManualMzLoad);
	DDX_Control(pDX, IDC_RDO_MANUAL_MZ_UNLOAD, m_rdoManualMzUnload);
	DDX_Control(pDX, IDC_RDO_MANUAL_DOOR_LOCK, m_rdoManualDoorLock);
	DDX_Control(pDX, IDC_RDO_MANUAL_DOOR_UNLOCK, m_rdoManualDoorUnlock);
}

BEGIN_MESSAGE_MAP(CManualDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RDO_MANUAL_ELEVATOR, &CManualDlg::OnBnClickedRdoManualElevator)
	ON_BN_CLICKED(IDC_RDO_MANUAL_LOAD, &CManualDlg::OnBnClickedRdoManualLoad)
	ON_BN_CLICKED(IDC_RDO_MANUAL_INSP, &CManualDlg::OnBnClickedRdoManualInsp)
	ON_BN_CLICKED(IDC_RDO_MANUAL_UNLOAD, &CManualDlg::OnBnClickedRdoManualUnload)
	ON_BN_CLICKED(IDC_RDO_MANUAL_MZ_LOAD, &CManualDlg::OnBnClickedRdoManualMzLoad)
	ON_BN_CLICKED(IDC_RDO_MANUAL_MZ_UNLOAD, &CManualDlg::OnBnClickedRdoManualMzUnload)
	ON_BN_CLICKED(IDC_RDO_MANUAL_DOOR_LOCK, &CManualDlg::OnBnClickedRdoManualDoorLock)
	ON_BN_CLICKED(IDC_RDO_MANUAL_DOOR_UNLOCK, &CManualDlg::OnBnClickedRdoManualDoorUnlock)
END_MESSAGE_MAP()

// CManualDlg 메시지 처리기입니다.

BOOL CManualDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_pManualElevatorDlg = new CManualElevatorDlg(this);
	m_pManualElevatorDlg->Create(IDD_MANUAL_ELEVATOR_DLG, this);

	m_pManualLoadDlg = new CManualLoadDlg(this);
	m_pManualLoadDlg->Create(IDD_MANUAL_LOAD_DLG, this);

	m_pManualInspDlg = new CManualInspDlg(this);
	m_pManualInspDlg->Create(IDD_MANUAL_INSP_DLG, this);

	m_pManualUnloadDlg = new CManualUnloadDlg(this);
	m_pManualUnloadDlg->Create(IDD_MANUAL_UNLOAD_DLG, this);

	m_pManualMzLoadDlg = new CManualMzLoadDlg(this);
	m_pManualMzLoadDlg->Create(IDD_MANUAL_MZ_LOAD_DLG, this);

	m_pManualMzUnloadDlg = new CManualMzUnloadDlg(this);
	m_pManualMzUnloadDlg->Create(IDD_MANUAL_MZ_UNLOAD_DLG, this);

	// Elevator Dlg Visible
	m_rdoManualElevator.SetCheck(TRUE);
	m_rdoManualElevator.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualDlg::OnDestroy() 
{
	CDialogEx::OnDestroy();

	m_pManualElevatorDlg->DestroyWindow();
	m_pManualLoadDlg->DestroyWindow();
	m_pManualInspDlg->DestroyWindow();
	m_pManualUnloadDlg->DestroyWindow();
	m_pManualMzLoadDlg->DestroyWindow();
	m_pManualMzUnloadDlg->DestroyWindow();

	if (m_pManualElevatorDlg) delete m_pManualElevatorDlg;
	if (m_pManualLoadDlg) delete m_pManualLoadDlg;
	if (m_pManualInspDlg) delete m_pManualInspDlg;
	if (m_pManualUnloadDlg) delete m_pManualUnloadDlg;
	if (m_pManualMzLoadDlg) delete m_pManualMzLoadDlg;
	if (m_pManualMzUnloadDlg) delete m_pManualMzUnloadDlg;

	m_pManualElevatorDlg = NULL;
	m_pManualLoadDlg = NULL;
	m_pManualInspDlg = NULL;
	m_pManualUnloadDlg = NULL;
	m_pManualMzLoadDlg = NULL;
	m_pManualMzUnloadDlg = NULL;
}

void CManualDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		if (m_rdoManualElevator.GetCheck()) m_pManualElevatorDlg->ShowWindow(SW_SHOW);
		if (m_rdoManualLoad.GetCheck()) m_pManualLoadDlg->ShowWindow(SW_SHOW);
		if (m_rdoManualInsp.GetCheck()) m_pManualInspDlg->ShowWindow(SW_SHOW);
		if (m_rdoManualUnload.GetCheck()) m_pManualUnloadDlg->ShowWindow(SW_SHOW);
		if (m_rdoManualMzLoad.GetCheck()) m_pManualMzLoadDlg->ShowWindow(SW_SHOW);
		if (m_rdoManualMzUnload.GetCheck()) m_pManualMzUnloadDlg->ShowWindow(SW_SHOW);

		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		if (pEquipData->bUseDoorLock) {
			g_objCommon.Locking_MainDoor(TRUE);
			m_rdoManualDoorLock.SetCheck(TRUE);
			m_rdoManualDoorLock.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
			m_rdoManualDoorUnlock.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
		} else {
			g_objCommon.Locking_MainDoor(FALSE);
			m_rdoManualDoorUnlock.SetCheck(TRUE);
			m_rdoManualDoorUnlock.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
			m_rdoManualDoorLock.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
		}

		if (gData.nLanguage == 0) {
			m_rdoManualElevator.SetWindowText("엘리베이터");
			m_rdoManualLoad.SetWindowText("로드");
			m_rdoManualUnload.SetWindowText("언로드");
			m_rdoManualMzLoad.SetWindowText("매거진 로드");
			m_rdoManualMzUnload.SetWindowText("매거진 언로드");
		} else {
			m_rdoManualElevator.SetWindowText("Elevator");
			m_rdoManualLoad.SetWindowText("Load");
			m_rdoManualUnload.SetWindowText("Unload");
			m_rdoManualMzLoad.SetWindowText("MZ Load");
			m_rdoManualMzUnload.SetWindowText("MZ Unload");
		}
		SetTimer(0, 100, NULL);

	} else {
		KillTimer(0);

		g_objCommon.Locking_MainDoor(TRUE);

		if (m_rdoManualElevator.GetCheck()) m_pManualElevatorDlg->ShowWindow(SW_HIDE);
		if (m_rdoManualLoad.GetCheck()) m_pManualLoadDlg->ShowWindow(SW_HIDE);
		if (m_rdoManualInsp.GetCheck()) m_pManualInspDlg->ShowWindow(SW_HIDE);
		if (m_rdoManualUnload.GetCheck()) m_pManualUnloadDlg->ShowWindow(SW_HIDE);
		if (m_rdoManualMzLoad.GetCheck()) m_pManualMzLoadDlg->ShowWindow(SW_HIDE);
		if (m_rdoManualMzUnload.GetCheck()) m_pManualMzUnloadDlg->ShowWindow(SW_HIDE);
	}
}

void CManualDlg::OnTimer(UINT_PTR nIDEvent) 
{
	KillTimer(0);

	CCMAI2100Dlg *pMainDlg = (CCMAI2100Dlg*)AfxGetMainWnd();
	int nState = pMainDlg->Get_MainState();
	if (nState != STATE_ALARM && nState != STATE_EMER) g_objCommon.Check_MainEmgAir();	// Emg & Main Air

	if (m_pManualElevatorDlg->IsWindowVisible()) m_pManualElevatorDlg->Display_Status();
	else if (m_pManualLoadDlg->IsWindowVisible()) m_pManualLoadDlg->Display_Status();
	else if (m_pManualInspDlg->IsWindowVisible()) m_pManualInspDlg->Display_Status();
	else if (m_pManualUnloadDlg->IsWindowVisible()) m_pManualUnloadDlg->Display_Status();
	else if (m_pManualMzLoadDlg->IsWindowVisible()) m_pManualMzLoadDlg->Display_Status();
	else if (m_pManualMzUnloadDlg->IsWindowVisible()) m_pManualMzUnloadDlg->Display_Status();

	SetTimer(0, 100, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

void CManualDlg::OnBnClickedRdoManualElevator()
{
	if (m_pManualElevatorDlg->IsWindowVisible()) return;
	Hide_Windows();
	g_objLogFile.Save_HandlerLog("[Manual - Elevator] Start");
	m_rdoManualElevator.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pManualElevatorDlg->ShowWindow(SW_SHOW);
}

void CManualDlg::OnBnClickedRdoManualLoad()
{
	if (m_pManualLoadDlg->IsWindowVisible()) return;
	Hide_Windows();
	g_objLogFile.Save_HandlerLog("[Manual - Load] Start");
	m_rdoManualLoad.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pManualLoadDlg->ShowWindow(SW_SHOW);
}

void CManualDlg::OnBnClickedRdoManualInsp()
{
	if (m_pManualInspDlg->IsWindowVisible()) return;
	Hide_Windows();
	g_objLogFile.Save_HandlerLog("[Manual - Insp] Start");
	m_rdoManualInsp.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pManualInspDlg->ShowWindow(SW_SHOW);
}

void CManualDlg::OnBnClickedRdoManualUnload()
{
	if (m_pManualUnloadDlg->IsWindowVisible()) return;
	Hide_Windows();
	g_objLogFile.Save_HandlerLog("[Manual - Unload] Start");
	m_rdoManualUnload.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pManualUnloadDlg->ShowWindow(SW_SHOW);
}

void CManualDlg::OnBnClickedRdoManualMzLoad()
{
	if (m_pManualMzLoadDlg->IsWindowVisible()) return;
	Hide_Windows();
	g_objLogFile.Save_HandlerLog("[Manual - MZ Load] Start");
	m_rdoManualMzLoad.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pManualMzLoadDlg->ShowWindow(SW_SHOW);
}

void CManualDlg::OnBnClickedRdoManualMzUnload()
{
	if (m_pManualMzUnloadDlg->IsWindowVisible()) return;
	Hide_Windows();
	g_objLogFile.Save_HandlerLog("[Manual - MZ Unload] Start");
	m_rdoManualMzUnload.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_pManualMzUnloadDlg->ShowWindow(SW_SHOW);
}

void CManualDlg::OnBnClickedRdoManualDoorLock()
{
	if (!m_rdoManualDoorLock.GetCheck()) return;
	m_rdoManualDoorLock.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualDoorUnlock.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	g_objCommon.Locking_MainDoor(TRUE, TRUE);
}

void CManualDlg::OnBnClickedRdoManualDoorUnlock()
{
	if (!m_rdoManualDoorUnlock.GetCheck()) return;
	m_rdoManualDoorUnlock.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualDoorLock.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	g_objCommon.Locking_MainDoor(FALSE, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CManualDlg::Initial_Controls() 
{
	m_picManualBack.Init_Ctrl(COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xF0));
	
	m_rdoManualElevator.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualLoad.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualInsp.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualUnload.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualMzLoad.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualMzUnload.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualDoorLock.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoManualDoorUnlock.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
}

void CManualDlg::Hide_Windows()
{
	m_pManualElevatorDlg->ShowWindow(SW_HIDE);
	m_pManualLoadDlg->ShowWindow(SW_HIDE);
	m_pManualInspDlg->ShowWindow(SW_HIDE);
	m_pManualUnloadDlg->ShowWindow(SW_HIDE);
	m_pManualMzLoadDlg->ShowWindow(SW_HIDE);
	m_pManualMzUnloadDlg->ShowWindow(SW_HIDE);
	
	m_rdoManualElevator.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualLoad.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualInsp.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualUnload.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualMzLoad.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
	m_rdoManualMzUnload.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
}

///////////////////////////////////////////////////////////////////////////////
