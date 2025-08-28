// ManualElevatorDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMAI2100.h"
#include "ManualElevatorDlg.h"
#include "afxdialogex.h"
#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"
#include "Common.h"
#include "SequenceMain.h"

// CManualElevatorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualElevatorDlg, CDialogEx)

CManualElevatorDlg::CManualElevatorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualElevatorDlg::IDD, pParent)
{
}

CManualElevatorDlg::~CManualElevatorDlg()
{
}

void CManualElevatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_LOAD2_ELEVATOR_Z_0 + i, m_btnLoad2ElevatorZ[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_LOAD2_ELEVATOR_IO_0 + i, m_btnLoad2ElevatorIO[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_TRAY_TRANSFER1_X_0 + i, m_btnTrayTrans1X[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_BTN_TRAY_TRANSFER1_Z_0 + i, m_btnTrayTrans1Z[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_TRAY_TRANSFER1_IO_0 + i, m_btnTrayTrans1IO[i]);
	for (int i = 0; i < 9; i++) DDX_Control(pDX, IDC_BTN_TRAY_TRANSFER2_X_0 + i, m_btnTrayTrans2X[i]);
	for (int i = 0; i <16; i++) DDX_Control(pDX, IDC_BTN_TRAY_TRANSFER2_Z_0 + i, m_btnTrayTrans2Z[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_TRAY_TRANSFER2_IO_0 + i, m_btnTrayTrans2IO[i]);
	
	for (int i = 0; i <11; i++) DDX_Control(pDX, IDC_LED_LOAD2_ELEVATOR_IO_0 + i, m_ledLoad2Elevator[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_TRAY_TRANS_IO_0 + i, m_ledTrayTrans1[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_TRAY_TRANS2_IO_0 + i, m_ledTrayTrans2[i]);
}

BEGIN_MESSAGE_MAP(CManualElevatorDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD2_ELEVATOR_Z_0, IDC_BTN_LOAD2_ELEVATOR_Z_1, OnbtnLoad2ElevatorZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD2_ELEVATOR_IO_0, IDC_BTN_LOAD2_ELEVATOR_IO_3, OnbtnLoad2ElevatorIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRAY_TRANSFER1_X_0, IDC_BTN_TRAY_TRANSFER1_X_3, OnbtnTrayTrans1XClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRAY_TRANSFER1_Z_0, IDC_BTN_TRAY_TRANSFER1_Z_7, OnbtnTrayTrans1ZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRAY_TRANSFER1_IO_0, IDC_BTN_TRAY_TRANSFER1_IO_1, OnbtnTrayTrans1IOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRAY_TRANSFER2_X_0, IDC_BTN_TRAY_TRANSFER2_X_8, OnbtnTrayTrans2XClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRAY_TRANSFER2_Z_0, IDC_BTN_TRAY_TRANSFER2_Z_15, OnbtnTrayTrans2ZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TRAY_TRANSFER2_IO_0, IDC_BTN_TRAY_TRANSFER2_IO_1, OnbtnTrayTrans2IOClick)
	

END_MESSAGE_MAP()

// CManualElevatorDlg 메시지 처리기입니다.
BOOL CManualElevatorDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	m_nRunCase1 = m_nRunCase2 = m_nRunCase3 = m_nRunCase4 = m_nRunCase5 = m_nRunCase6 = m_nRunCase7 = 0;
	m_bThreadElevator = FALSE;
	m_pThreadElevator = NULL;
	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualElevatorDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualElevatorDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) { End_ElevatorThread(); return; }

	m_nRunCase1 = m_nRunCase2 = m_nRunCase3 = m_nRunCase4 = m_nRunCase5 = m_nRunCase6 = m_nRunCase7 = 0;
	Display_Status();

	m_strLog.Format("[Manual Elevator] Show Window");
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualElevatorDlg::Initial_Controls() 
{
	for (int i = 0; i <  2; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 2; i <  5; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x10, 0x10, 0xFF), COLOR_DEFAULT);
	for (int i = 5; i <  8; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x10, 0x90, 0x00), COLOR_DEFAULT);
	for (int i = 0; i <  5; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));
	for (int i = 0; i <  5; i++) m_stcAxisPos[i].Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x10, 0x10, 0x60));
	for (int i = 0; i <  2; i++) m_btnLoad2ElevatorZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnLoad2ElevatorIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnTrayTrans1X[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  8; i++) m_btnTrayTrans1Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnTrayTrans1IO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  9; i++) m_btnTrayTrans2X[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 16; i++) m_btnTrayTrans2Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnTrayTrans2IO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);


	for (int i = 0; i <11; i++) m_ledLoad2Elevator[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 5; i++) m_ledTrayTrans1[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 5; i++) m_ledTrayTrans2[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
}

void CManualElevatorDlg::Display_Status()
{
	CString strPos;
	double dPos = g_objAJinAXL.Get_Position(AX_ELEVATOR_Z2);
	strPos.Format("%0.3lf", dPos);
	m_stcAxisPos[0].SetWindowText(strPos);
	for (int i = 1; i < 5; i++) {
		dPos = g_objAJinAXL.Get_Position(AX_TRANSFER_X1 + i - 1);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i].SetWindowText(strPos);
	}

	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();

	m_ledLoad2Elevator[0].Set_On(pDX00->iElevator2SlideOpen);
	m_ledLoad2Elevator[1].Set_On(pDX00->iElevator2SlideClose);
	m_ledLoad2Elevator[2].Set_On(pDX00->iElevator2Height1);
	m_ledLoad2Elevator[3].Set_On(pDX00->iElevator2Height2);
	m_ledLoad2Elevator[4].Set_On(pDX00->iElevator2TrayExist);
	m_ledLoad2Elevator[5].Set_On(pDX00->iElevator2Clamp1On);
	m_ledLoad2Elevator[6].Set_On(pDX00->iElevator2Clamp1Off);
	m_ledLoad2Elevator[7].Set_On(pDX00->iElevator2Clamp2On);
	m_ledLoad2Elevator[8].Set_On(pDX00->iElevator2Clamp2Off);
	m_ledLoad2Elevator[9].Set_On(pDX00->iElevator2SlideLock);
	m_ledLoad2Elevator[10].Set_On(pDX00->iElevator2SlideUnlock);

	m_ledTrayTrans1[0].Set_On(pDX03->iTransferLTrayExist);
	m_ledTrayTrans1[1].Set_On(pDX03->iTransferLGrab1Open);
	m_ledTrayTrans1[2].Set_On(pDX03->iTransferLGrab1Close);
	m_ledTrayTrans1[3].Set_On(pDX03->iTransferLGrab2Open);
	m_ledTrayTrans1[4].Set_On(pDX03->iTransferLGrab2Close);

	m_ledTrayTrans2[0].Set_On(pDX03->iTransferRTrayExist);
	m_ledTrayTrans2[1].Set_On(pDX03->iTransferRGrab1Open);
	m_ledTrayTrans2[2].Set_On(pDX03->iTransferRGrab1Close);
	m_ledTrayTrans2[3].Set_On(pDX03->iTransferRGrab2Open);
	m_ledTrayTrans2[4].Set_On(pDX03->iTransferRGrab2Close);	
}

///////////////////////////////////////////////////////////////////////////////
void CManualElevatorDlg::OnbtnLoad2ElevatorZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ELEVATOR_Z2)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_LOAD2_ELEVATOR_Z_0;	

	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	if (pDX00->iElevator2Clamp1On || pDX00->iElevator2Clamp2On) {
		AfxMessageBox(_T("Load2 Elevator Clamp On 상태 입니다....."));
		return;
	}
	if (!pDX00->iElevator2Clamp1On && !pDX00->iElevator2Clamp1Off) {
		AfxMessageBox(_T("Load2 Elevator Clamp1 Sensor 감지가 되지 않습니다....."));
		return;
	}
	if (pDX00->iElevator2Clamp2On && pDX00->iElevator2Clamp2Off) {
		AfxMessageBox(_T("Load2 Elevator Clamp2 Sensor 감지가 되지 않습니다....."));
		return;
	}
	if (!g_objCommon.Check_SlideLock(2)) {
		AfxMessageBox(_T("Load2 Elevator Slide Lock후 진행해 주세요...."));
		return;
	}

	if (nIndex == 0) {
		m_nRunCase2 = 0;
		g_objAJinAXL.Move_Absolute(AX_ELEVATOR_Z2, gData.dElevatorDown);
	} else {
		if (!pDX00->iElevator2TrayExist) {
			AfxMessageBox(_T("Elevator 2 -> Tray를 넣어 주세요......"));
			return;
		}
		m_nRunCase2 = 1;
		if (m_pThreadElevator == NULL) Begin_ElevatorThread();
	}
	gData.nElevatorOpen[2] = 0; g_objSequenceMain.Set_MainRunCase(AUTO_ELEVATOR_2, 51);
	DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 	pDY13->oElevator2Lamp = FALSE;
	g_objAJinAXL.Write_Output(13);

	m_strLog.Format("[Manual Elevator] Load2 Elevator Z2 (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualElevatorDlg::OnbtnLoad2ElevatorIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_LOAD2_ELEVATOR_IO_0;	

	DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
	DY_DATA_01 *pDY01 = g_objAJinAXL.Get_pDY01();
	
	if (nIndex == 0) {
		pDY00->oElevator2Clamp1On  = pDY00->oElevator2Clamp2On = TRUE;
		pDY00->oElevator2Clamp1Off = pDY00->oElevator2Clamp2Off = FALSE;
		g_objAJinAXL.Write_Output(0);
	} else if (nIndex == 1) {
		pDY00->oElevator2Clamp1On  = pDY00->oElevator2Clamp2On = FALSE;
		pDY00->oElevator2Clamp1Off = pDY00->oElevator2Clamp2Off = TRUE;
		g_objAJinAXL.Write_Output(0);
	} else if (nIndex == 2) {
		pDY01->oElevator2SlideLock = TRUE;	pDY01->oElevator2SlideUnlock = FALSE;
		g_objAJinAXL.Write_Output(1);
	} else if (nIndex == 3) {
		pDY01->oElevator2SlideLock = FALSE;	pDY01->oElevator2SlideUnlock = TRUE;
		g_objAJinAXL.Write_Output(1);
	}

	m_strLog.Format("[Manual Elevator] Load2 Elevator IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualElevatorDlg::OnbtnTrayTrans1XClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TRANSFER_X1)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_TRAY_TRANSFER1_X_0 + 1;	

	// Interlock
	if (!g_objCommon.Check_Position(AX_TRANSFER_Z1, 0)) {
		AfxMessageBox(_T("Transfer1 Z축 Ready Up 위치에서만 작업 가능합니다....."));
		return;
	}

	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	double dTrans2XPos = g_objAJinAXL.Get_Position(AX_TRANSFER_X2);
	double dCheckPos = 0.0;

	if (nIndex == 3) {
		dCheckPos = pMoveData->dTransferX2[1] - 1.0;	//Buffer Elevator
		if (dCheckPos < dTrans2XPos) {
			AfxMessageBox(_T("충돌 위험!! Transfer2 X축이 Buffer Elevator 위치를 넘어가 있습니다....."));
			return;
		}
	}
	if (nIndex == 4) {
		dCheckPos = pMoveData->dTransferX2[2] - 1.0;	//Load Carrier
		if (dCheckPos < dTrans2XPos) {
			AfxMessageBox(_T("충돌 위험!! Transfer2 X축이 Load Carrier 위치를 넘어가 있습니다....."));
			return;
		}
	}
	
	// Move
	g_objCommon.Move_Position(AX_TRANSFER_X1, nIndex);

	m_strLog.Format("[Manual Elevator] Tray Transfer1 X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualElevatorDlg::OnbtnTrayTrans1ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TRANSFER_Z1)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_TRAY_TRANSFER1_Z_0;

	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();

	// Interlock
	if (nIndex == 1 || nIndex == 9) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X1, 1)) {
			AfxMessageBox(_T("Transfer1 X축이 Load Stage1 위치가 아닙니다....."));
			return;
		}
	} else if (nIndex == 2 || nIndex == 10) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X1, 2)) {
			AfxMessageBox(_T("Transfer1 X축이 Load Stage2 위치가 아닙니다....."));
			return;
		}
	} else if (nIndex == 3 || nIndex == 5) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X1, 3)) {
			AfxMessageBox(_T("Transfer1 X축이 Buffer Carrier 위치가 아닙니다....."));
			return;
		}
	} else if (nIndex == 4) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X1, 4)) {
			AfxMessageBox(_T("Transfer1 X축이 Load Carrier 위치가 아닙니다....."));
			return;
		}
		if (!g_objCommon.Check_Position(AX_LOAD_FEEDER_Y, 0)) {
			AfxMessageBox(_T("Load Feeder의 위치가 Ready인 경우만 Down 가능합니다.."));
			return;
		}
	} 

	if (nIndex != 0) {	// Ready Up 외 Down할때 Clamp 확인.
		if (!pDX03->iTransferLGrab1Open || !pDX03->iTransferLGrab2Open) {
			if (g_objCommon.Show_MsgBox(2, "Transfer1 Clamp Open 상태가 아닙니다. Z축 Down 하시겠습니까?") != IDOK) return;
		}
	}

	if (g_objCommon.Check_Position(AX_TRANSFER_X1, 4)) {
		if (!g_objCommon.Check_Position(AX_LOAD_FEEDER_Y, 0)) {
			AfxMessageBox(_T("Load Feeder의 위치가 Ready인 경우만 Up/Down 가능합니다.."));
			return;
		}
	}

	// Move
	g_objCommon.Move_Position(AX_TRANSFER_Z1, nIndex);

	m_strLog.Format("[Manual Elevator] Tray Transfer1 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);

}

void CManualElevatorDlg::OnbtnTrayTrans1IOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_TRAY_TRANSFER1_IO_0;	

	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();

	if (nIndex == 0) {
		pDY03->oTransferLGrabOpen = TRUE;
		pDY03->oTransferLGrabClose = FALSE;
	} else {
		pDY03->oTransferLGrabOpen = FALSE;
		pDY03->oTransferLGrabClose = TRUE;
	}
	g_objAJinAXL.Write_Output(3);

	m_strLog.Format("[Manual Elevator] Tray Transfer1 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualElevatorDlg::OnbtnTrayTrans2XClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TRANSFER_X2)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_TRAY_TRANSFER2_X_0 + 1;	

	// Interlock
	if (!g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
		AfxMessageBox(_T("Transfer2 Z축 Ready Up 위치에서만 작업 가능합니다....."));
		return;
	}

	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	double dTrans1XPos = g_objAJinAXL.Get_Position(AX_TRANSFER_X1);
	double dCheckPos = 0.0;

	if (nIndex == 1) {	//Buffer
		dCheckPos = pMoveData->dTransferX1[3] - 1.0;	//Buffer
		if (dCheckPos < dTrans1XPos) {
			AfxMessageBox(_T("충돌 위험!! Transfer1 X축이 Load2 Elevator 위치를 넘어가 있습니다....."));
			return;
		}
	}
	if (nIndex == 2) {	//Load Carrier
		dCheckPos = pMoveData->dTransferX1[4] - 1.0;	//Load Carrier
		if (dCheckPos < dTrans1XPos) {
			AfxMessageBox(_T("충돌 위험!! Transfer1 X축이 NG Empty Elevator 위치를 넘어가 있습니다....."));
			return;
		}
	}

	// Move
	g_objCommon.Move_Position(AX_TRANSFER_X2, nIndex);

	m_strLog.Format("[Manual Elevator] Tray Transfer2 X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualElevatorDlg::OnbtnTrayTrans2ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TRANSFER_Z2)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_TRAY_TRANSFER2_Z_0;

	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();

	// Interlock
	if (nIndex == 1 || nIndex == 2) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X2, 1)) {
			AfxMessageBox(_T("Transfer2 X축이 Buffer Elevator 위치가 아닙니다....."));
			return;
		}
	} else if (nIndex == 3) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X2, 2)) {
			AfxMessageBox(_T("Transfer2 X축이 Load Carrier 위치가 아닙니다....."));
			return;
		}
		if (!g_objCommon.Check_Position(AX_LOAD_FEEDER_Y, 0)) {
			AfxMessageBox(_T("Load Feeder의 위치가 Ready인 경우만 Down 가능합니다.."));
			return;
		}
	} else if (nIndex == 4 || nIndex == 5) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X2, 3)) {
			AfxMessageBox(_T("Transfer2 X축이 NG Carrier 위치가 아닙니다....."));
			return;
		}
		if (!g_objCommon.Check_Position(AX_NG_FEEDER_Y, 0)) {
			AfxMessageBox(_T("NG Feeder의 위치가 Ready인 경우만 Down 가능합니다.."));
			return;
		}
	} else if (nIndex == 6 || nIndex == 7) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X2, 4)) {
			AfxMessageBox(_T("Transfer2 X축이 Good Carrier 위치가 아닙니다....."));
			return;
		}
		if (!g_objCommon.Check_Position(AX_GOOD_FEEDER_Y, 0)) {
			AfxMessageBox(_T("GOOD Feeder의 위치가 Ready인 경우만 Down 가능합니다.."));
			return;
		}
	} else if (nIndex == 8 || nIndex == 12) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X2, 6)) {
			AfxMessageBox(_T("Transfer2 X축이 NG Stage1 위치가 아닙니다....."));
			return;
		}
	} else if (nIndex == 9 || nIndex == 13) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X2, 7)) {
			AfxMessageBox(_T("Transfer2 X축이 NG Stage2 위치가 아닙니다....."));
			return;
		}
	}else if (nIndex == 10 || nIndex == 14) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X2, 8)) {
			AfxMessageBox(_T("Transfer2 X축이 Good Stage1 위치가 아닙니다....."));
			return;
		}
	} else if (nIndex == 11 || nIndex == 15) {
		if (!g_objCommon.Check_Position(AX_TRANSFER_X2, 9)) {
			AfxMessageBox(_T("Transfer2 X축이 Good Stage2 위치가 아닙니다....."));
			return;
		}
	}

	if (nIndex != 0) {	// Ready Up 외 Down할때 Clamp 확인.
		if (!pDX03->iTransferRGrab1Open || !pDX03->iTransferRGrab2Open) {
			if (g_objCommon.Show_MsgBox(2, "Transfer2 Clamp Open 상태가 아닙니다. Z축 Down 하시겠습니까?") != IDOK) return;
		}
	}

	if (g_objCommon.Check_Position(AX_TRANSFER_X2, 2)) {
		if (!g_objCommon.Check_Position(AX_LOAD_FEEDER_Y, 0)) {
			AfxMessageBox(_T("Load Feeder의 위치가 Ready인 경우만 Up/Down 가능합니다.."));
			return;
		}
	}
	if (g_objCommon.Check_Position(AX_TRANSFER_X2, 3)) {
		if (!g_objCommon.Check_Position(AX_NG_FEEDER_Y, 0)) {
			AfxMessageBox(_T("NG Feeder의 위치가 Ready인 경우만 Up/Down 가능합니다.."));
			return;
		}
	}
	if (g_objCommon.Check_Position(AX_TRANSFER_X2, 4)) {
		if (!g_objCommon.Check_Position(AX_GOOD_FEEDER_Y, 0)) {
			AfxMessageBox(_T("GOOD Feeder의 위치가 Ready인 경우만 Up/Down 가능합니다.."));
			return;
		}
	}

	// Move
	g_objCommon.Move_Position(AX_TRANSFER_Z2, nIndex);

	m_strLog.Format("[Manual Elevator] Tray Transfer2 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualElevatorDlg::OnbtnTrayTrans2IOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_TRAY_TRANSFER2_IO_0;	

	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();

	if (nIndex == 0) {
		pDY03->oTransferRGrabOpen = TRUE;
		pDY03->oTransferRGrabClose = FALSE;
	} else {
		pDY03->oTransferRGrabOpen = FALSE;
		pDY03->oTransferRGrabClose = TRUE;
	}
	g_objAJinAXL.Write_Output(3);

	m_strLog.Format("[Manual Elevator] Tray Transfer2 IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

///////////////////////////////////////////////////////////////////////////////
void CManualElevatorDlg::Begin_ElevatorThread()
{
	g_objCommon.Locking_MainDoor(TRUE);
	if (m_pThreadElevator) return;

	m_tEleLoop2.Set_LoopTime(3000);

//	if (m_pThreadElevator) End_ElevatorThread();
	m_bThreadElevator = TRUE;
	m_pThreadElevator = AfxBeginThread(Thread_Elevator, this);
}

void CManualElevatorDlg::End_ElevatorThread()
{
	if (m_pThreadElevator) {
		m_bThreadElevator = FALSE;
		WaitForSingleObject(m_pThreadElevator->m_hThread, INFINITE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// InspectMold Thread Function 
UINT CManualElevatorDlg::Thread_Elevator(LPVOID lpVoid)
{
	CManualElevatorDlg *pDlg = (CManualElevatorDlg*)lpVoid;

	while (pDlg->m_bThreadElevator) {
		if (!g_objCommon.Check_MainDoor()) break;
		if (!g_objCommon.Check_TraySlide()) break;

		if (!pDlg->Manual_ElevatorRun2()) break;
		Sleep(5);
	}
	pDlg->m_bThreadElevator = FALSE;
	pDlg->m_pThreadElevator = NULL;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
BOOL CManualElevatorDlg::Manual_ElevatorRun2()
{
	CString strVel, strMsg;

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();

	switch (m_nRunCase2) 
	{
	case 0:
		m_tEleLoop2.Set_LoopTime(3000);
		break;

	case 1:
//			g_objAJinAXL.Move_Absolute(AX_ELEVATOR_Z2, gData.dElevatorDown);
			m_nRunCase2 = 4; m_tEleLoop2.Set_LoopTime(30000);
		break;
	case 4:
		if (g_objAJinAXL.Is_Done(AX_ELEVATOR_Z2)) {
			m_nRunCase2++; m_tEleLoop2.Set_LoopTime(5000);
		}
		break;
	case 5:
		if (pDX00->iElevator2TrayExist) {
			m_nRunCase2 = 10; m_tEleLoop2.Set_LoopTime(5000);
		}
		break;

	case 10:
		if (pDX00->iElevator2Height1 || pDX00->iElevator2Height2) {
			m_nRunCase2 = 12; m_tEleLoop2.Set_LoopTime(30000);
		} else {
			g_objAJinAXL.Move_Relative(AX_ELEVATOR_Z2, ELEVATOR_UD);
			m_nRunCase2++; m_tEleLoop2.Set_LoopTime(30000);
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_Done(AX_ELEVATOR_Z2)) {
			m_nRunCase2 = 10; m_tEleLoop2.Set_LoopTime(30000);
		}
		break;
	case 12:
		if (pDX00->iElevator2Height1 || pDX00->iElevator2Height2) {
			g_objAJinAXL.Move_Relative(AX_ELEVATOR_Z2, ELEVATOR_DN*-1.0);
			m_nRunCase2++; m_tEleLoop2.Set_LoopTime(30000);
		} else {
			m_dEvtSPos2 = 0.0;
			m_nRunCase2 = 14; m_tEleLoop2.Set_LoopTime(30000);
		}
		break;
	case 13:
		if (g_objAJinAXL.Is_Done(AX_ELEVATOR_Z2)) {
			m_nRunCase2 = 12; m_tEleLoop2.Set_LoopTime(30000);
		}
		break;
	case 14:
		if (pDX00->iElevator2Height1 && pDX00->iElevator2Height2) {
			m_nRunCase2 = 16; m_tEleLoop2.Set_LoopTime(30000);
		} else {
			m_dEvtEPos2 = g_objAJinAXL.Get_Position(AX_ELEVATOR_Z2);
			if (m_dEvtSPos2 > 1.0 && pEquipData->dEevatOffset > 0.0 && (m_dEvtEPos2 - m_dEvtSPos2) > pEquipData->dEevatOffset) {
				m_nRunCase2 = 19; m_tEleLoop2.Set_LoopTime(3000);
			} else {
				g_objAJinAXL.Move_Relative(AX_ELEVATOR_Z2, ELEVATOR_SL);
				m_nRunCase2++; m_tEleLoop2.Set_LoopTime(30000);
			}
		}
		break;
	case 15:
		if (g_objAJinAXL.Is_Done(AX_ELEVATOR_Z2)) {
			if (m_dEvtSPos2 < 1.0) {
				if (pDX00->iElevator2Height1 || pDX00->iElevator2Height2) {
					m_dEvtSPos2 = g_objAJinAXL.Get_Position(AX_ELEVATOR_Z2);
				}
			}
			m_nRunCase2 = 14; m_tEleLoop2.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(AX_ELEVATOR_Z2)) {
			double dElelavtorZ = pMoveData->dElevatorZ2[2];
			g_objAJinAXL.Move_Relative(AX_ELEVATOR_Z2, dElelavtorZ);
			m_nRunCase2++; m_tEleLoop2.Set_LoopTime(30000);
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_Done(AX_ELEVATOR_Z2)) {
			m_nRunCase2 = 20; m_tEleLoop2.Set_LoopTime(30000);
		}
		break;
	case 19:
		break;

	case 20:
		m_nRunCase2 = 0;
		g_objCommon.Show_MsgBox(1, "Manual Elevator2 End.....");
		return TRUE;

	}

	if (m_tEleLoop2.Over_LoopTime()) {
		if		(m_nRunCase2 ==  5) strMsg = "Elevator 2 -> Tray를 넣어 주세요.";
		else if (m_nRunCase2 == 11) strMsg = "Elevator 2 -> High 센서 미 감지........";
		else if (m_nRunCase2 == 14) strMsg = "Elevator 2 -> High 센서 미 감지........";
		else if (m_nRunCase2 == 19) strMsg.Format("Elevator 2 -> High 센서 Start(%0.3lf) End(%0.3lf) Offset(%0.3lf)", m_dEvtSPos2, m_dEvtEPos2, pEquipData->dEevatOffset);
		else strMsg.Format("AX_ELEVATOR_Z2 미 완료 => [%d]", m_nRunCase2);
		m_nRunCase2 = 0;
		g_objCommon.Show_MsgBox(1, strMsg);
		return TRUE;
	}
	return TRUE;
}
