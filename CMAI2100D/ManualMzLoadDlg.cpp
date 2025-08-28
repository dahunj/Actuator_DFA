// ManualMzLoadDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMAI2100.h"
#include "ManualMzLoadDlg.h"
#include "afxdialogex.h"
#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"
#include "Common.h"
#include "SequenceMain.h"
#include "BarcodeLot_Cognex.h"
#include "BarcodeLot_RS232.h"
#include "CarrierRFID_Load.h"

// CManualMzLoadDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualMzLoadDlg, CDialogEx)

CManualMzLoadDlg::CManualMzLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualMzLoadDlg::IDD, pParent)
{
}

CManualMzLoadDlg::~CManualMzLoadDlg()
{
}

void CManualMzLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 11; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i <  5; i++) DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);
	for (int i = 0; i <  8; i++) DDX_Control(pDX, IDC_CHK_MZ_LDCV_IO_0 + i,	   m_chkMZLoadCVIO[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_MZ_LDCV_IO_0 + i,	   m_btnMZLoadCVIO[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_LDCV_ELEVATOR_Z_0 + i,  m_btnLDCVElevatorZ[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_CHK_LDCV_ELEVATOR_IO_0 + i, m_chkLDCVElevatorIO[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_BTN_LDCV_ELEVATOR_IO_0 + i, m_btnLDCVElevatorIO[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_LOAD_FEEDER_Y_0 + i,	   m_btnLoadFeederY[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_LOAD_FEEDER_IO_0 + i,   m_btnLoadFeederIO[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_LOAD_ELEVATOR_Z_0 + i,  m_btnLoadElevatorZ[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_LOAD_ELEVATOR_IO_0 + i, m_btnLoadElevatorIO[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_BTN_MZ_TRANSFER_X_0 + i,    m_btnMZTransferX[i]);
	for (int i = 0; i <  7; i++) DDX_Control(pDX, IDC_BTN_MZ_TRANSFER_Z_0 + i,    m_btnMZTransferZ[i]);
	for (int i = 0; i <  8; i++) DDX_Control(pDX, IDC_BTN_MZ_TRANSFER_IO_0 + i,   m_btnMZTransferIO[i]);

	for (int i = 0; i < 18; i++) DDX_Control(pDX, IDC_LED_MZ_LDCV_IO_0 + i, m_ledMZLoadCVIO[i]);
	for (int i = 0; i <  9; i++) DDX_Control(pDX, IDC_LED_LDCV_ELEVATOR_IO_0 + i, m_ledLDCVElevatorIO[i]);
	for (int i = 0; i <  7; i++) DDX_Control(pDX, IDC_LED_LOAD_FEEDER_IO_0 + i, m_ledLoadFeederIO[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_LED_LOAD_ELEVATOR_IO_0 + i, m_ledLoadElevatorIO[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LED_MZ_TRANSFER_IO_0 + i, m_ledMZTransferIO[i]);

	for (int i = 0; i < 3; i++)  DDX_Control(pDX, IDC_LBL_BARCODE_0 + i, m_lblBarcode[i]);
	for (int i = 0; i < 3; i++)  DDX_Control(pDX, IDC_STC_BARCODE_0 + i, m_stcBarcode[i]);
	for (int i = 0; i < 9; i++)  DDX_Control(pDX, IDC_BTN_BARCODE_0 + i, m_btnBarcode[i]);
	
}

BEGIN_MESSAGE_MAP(CManualMzLoadDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_MZ_LDCV_IO_0, IDC_CHK_MZ_LDCV_IO_7, OnchkMZLoadCVIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_MZ_LDCV_IO_0, IDC_BTN_MZ_LDCV_IO_3, OnbtnMZLoadCVIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LDCV_ELEVATOR_Z_0, IDC_BTN_LDCV_ELEVATOR_Z_2, OnbtnLDCVElevatorZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_LDCV_ELEVATOR_IO_0, IDC_CHK_LDCV_ELEVATOR_IO_1, OnchkLDCVElevatorIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LDCV_ELEVATOR_IO_0, IDC_BTN_LDCV_ELEVATOR_IO_5, OnbtnLDCVElevatorIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_FEEDER_Y_0, IDC_BTN_LOAD_FEEDER_Y_2, OnbtnLoadFeederYClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_FEEDER_IO_0, IDC_BTN_LOAD_FEEDER_IO_1, OnbtnLoadFeederIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_ELEVATOR_Z_0, IDC_BTN_LOAD_ELEVATOR_Z_2, OnbtnLoadElevatorZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_LOAD_ELEVATOR_IO_0, IDC_BTN_LOAD_ELEVATOR_IO_3, OnbtnLoadElevatorIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_MZ_TRANSFER_X_0, IDC_BTN_MZ_TRANSFER_X_5, OnbtnMZTransferXClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_MZ_TRANSFER_Z_0, IDC_BTN_MZ_TRANSFER_Z_6, OnbtnMZTransferZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_MZ_TRANSFER_IO_0, IDC_BTN_MZ_TRANSFER_IO_7, OnbtnMZTransferIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BARCODE_0, IDC_BTN_BARCODE_8, OnBtnBarcodeClick)

END_MESSAGE_MAP()

// CManualMzLoadDlg 메시지 처리기입니다.
BOOL CManualMzLoadDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualMzLoadDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualMzLoadDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	Display_Status();

	m_strLog.Format("[Manual Elevator] Show Window");
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::Initial_Controls() 
{
	for (int i = 0; i <  3; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 3; i <  7; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x10, 0x10, 0xFF), COLOR_DEFAULT);
	for (int i = 7; i < 11; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x10, 0x90, 0x00), COLOR_DEFAULT);
	for (int i = 0; i <  5; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));
	for (int i = 0; i <  5; i++) m_stcAxisPos[i].Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x10, 0x10, 0x60));
	for (int i = 0; i <  8; i++) m_chkMZLoadCVIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnMZLoadCVIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnLDCVElevatorZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_chkLDCVElevatorIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  6; i++) m_btnLDCVElevatorIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnLoadFeederY[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnLoadFeederIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnLoadElevatorZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnLoadElevatorIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  6; i++) m_btnMZTransferX[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  7; i++) m_btnMZTransferZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  8; i++) m_btnMZTransferIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);

	for (int i = 0; i <18; i++) m_ledMZLoadCVIO[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 9; i++) m_ledLDCVElevatorIO[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 7; i++) m_ledLoadFeederIO[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 6; i++) m_ledLoadElevatorIO[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <10; i++) m_ledMZTransferIO[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);

	for (int i = 0; i < 3; i++)  m_lblBarcode[i].Init_Ctrl("Arial", 11, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));
	for (int i = 0; i < 3; i++)  m_stcBarcode[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xFF, 0x00, 0x00), RGB(0xF0, 0xF0, 0xD0));
	for (int i = 0; i < 9; i++)  m_btnBarcode[i].Init_Ctrl("Arial", 11, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
}

void CManualMzLoadDlg::Display_Status()
{
	CString strPos;
	double dPos = g_objAJinAXL.Get_Position(AX_LOAD_FEEDER_Y);
	strPos.Format("%0.3lf", dPos);
	m_stcAxisPos[0].SetWindowText(strPos);
	for (int i = 0; i < 2; i++) {
		dPos = g_objAJinAXL.Get_Position(AX_LDCV_ELEVATOR_Z + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i + 1].SetWindowText(strPos);
	}
	for (int i = 0; i < 2; i++) {
		dPos = g_objAJinAXL.Get_Position(AX_MZ_TRANSFER_X + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i + 3].SetWindowText(strPos);
	}

	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
	DX_DATA_16 *pDX16 = g_objAJinAXL.Get_pDX16();
	DX_DATA_17 *pDX17 = g_objAJinAXL.Get_pDX17();
	DX_DATA_18 *pDX18 = g_objAJinAXL.Get_pDX18();
	DX_DATA_20 *pDX20 = g_objAJinAXL.Get_pDX20();

	m_ledMZLoadCVIO[ 0].Set_On(pDX16->iLDCV2FCnt1);
	m_ledMZLoadCVIO[ 1].Set_On(pDX16->iLDCV2FCnt2);
	m_ledMZLoadCVIO[ 2].Set_On(pDX16->iLDCV2FCnt3);
	m_ledMZLoadCVIO[ 3].Set_On(pDX16->iLDCV2FCnt4);
	m_ledMZLoadCVIO[ 4].Set_On(pDX16->iLDCV2FCnt5);
	m_ledMZLoadCVIO[ 5].Set_On(pDX16->iLDCV2FCnt6);
	m_ledMZLoadCVIO[ 6].Set_On(pDX16->iLDVC2FStop);
	m_ledMZLoadCVIO[ 7].Set_On(pDX16->iLDCV1FCnt1);
	m_ledMZLoadCVIO[ 8].Set_On(pDX16->iLDCV1FCnt2);
	m_ledMZLoadCVIO[ 9].Set_On(pDX16->iLDCV1FCnt3);
	m_ledMZLoadCVIO[10].Set_On(pDX16->iLDCV1FCnt4);
	m_ledMZLoadCVIO[11].Set_On(pDX16->iLDCV1FCnt5);
	m_ledMZLoadCVIO[12].Set_On(pDX16->iLDCV1FCnt6);
	m_ledMZLoadCVIO[13].Set_On(pDX16->iLDVC1FStop);
	m_ledMZLoadCVIO[14].Set_On(pDX16->iLDCVStopper2FUp);
	m_ledMZLoadCVIO[15].Set_On(pDX16->iLDCVStopper2FDn);
	m_ledMZLoadCVIO[16].Set_On(pDX16->iLDCVStopper1FUp);
	m_ledMZLoadCVIO[17].Set_On(pDX16->iLDCVStopper1FDn);

	m_ledLDCVElevatorIO[0].Set_On(pDX17->iLDCVElevatorCVStart);
	m_ledLDCVElevatorIO[1].Set_On(pDX17->iLDCVElevatorCVStop);
	m_ledLDCVElevatorIO[2].Set_On(pDX17->iLDCVElevatorStopperUp);
	m_ledLDCVElevatorIO[3].Set_On(pDX17->iLDCVElevatorStopperDn);
	m_ledLDCVElevatorIO[4].Set_On(pDX17->iLDCVElevatorAlignUp);
	m_ledLDCVElevatorIO[5].Set_On(pDX17->iLDCVElevatorAlignDn);
	m_ledLDCVElevatorIO[6].Set_On(pDX17->iLDCVElevatorAlignIn);
	m_ledLDCVElevatorIO[7].Set_On(pDX17->iLDCVElevatorAlignOut);
	m_ledLDCVElevatorIO[8].Set_On(pDX17->iLDCVElevatorAlarm);

	m_ledLoadFeederIO[0].Set_On(pDX15->iLDGripOpen);
	m_ledLoadFeederIO[1].Set_On(pDX15->iLDGripClose);
	m_ledLoadFeederIO[2].Set_On(pDX15->iLDGripCarrierChk);
	m_ledLoadFeederIO[3].Set_On(pDX15->iLDGripOverload);
	m_ledLoadFeederIO[4].Set_On(pDX15->iLDRailCarrierChk1);
	m_ledLoadFeederIO[5].Set_On(pDX15->iLDRailCarrierChk2);
	m_ledLoadFeederIO[6].Set_On(pDX15->iLDMZCarrierExist);

	m_ledLoadElevatorIO[0].Set_On(pDX18->iLDMZElevatorClamp12In);
	m_ledLoadElevatorIO[1].Set_On(pDX18->iLDMZElevatorClamp12Out);
	m_ledLoadElevatorIO[2].Set_On(pDX18->iLDMZElevatorClamp34In);
	m_ledLoadElevatorIO[3].Set_On(pDX18->iLDMZElevatorClamp34Out);
	m_ledLoadElevatorIO[4].Set_On(pDX18->iLDMZElevatorExist);
	m_ledLoadElevatorIO[5].Set_On(pDX18->iMZBufferExist);

	m_ledMZTransferIO[0].Set_On(pDX20->iMZTransGrip12Open);
	m_ledMZTransferIO[1].Set_On(pDX20->iMZTransGrip12Close);
	m_ledMZTransferIO[2].Set_On(pDX20->iMZTransGrip34Open);
	m_ledMZTransferIO[3].Set_On(pDX20->iMZTransGrip34Close);
	m_ledMZTransferIO[4].Set_On(pDX20->iMZTransLockBarUp);
	m_ledMZTransferIO[5].Set_On(pDX20->iMZTransLockBarDn);
	m_ledMZTransferIO[6].Set_On(pDX20->iMZTransLockBarClose);
	m_ledMZTransferIO[7].Set_On(pDX20->iMZTransLockBarOpen);
	m_ledMZTransferIO[8].Set_On(pDX20->iMZTransExist);
	m_ledMZTransferIO[9].Set_On(pDX20->iMZLoackBarCheck);
}

///////////////////////////////////////////////////////////////////////////////
void CManualMzLoadDlg::OnchkMZLoadCVIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_CHK_MZ_LDCV_IO_0;	

	DY_DATA_16 *pDY16 = g_objAJinAXL.Get_pDY16();

	if (nIndex == 0) {
		if (m_chkMZLoadCVIO[0].GetCheck()) {
			if (m_chkMZLoadCVIO[1].GetCheck()) { 
				m_chkMZLoadCVIO[0].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Load 2F Left Conveyor CW방향으로 회전 중입니다..."));				
				return;
			}
			pDY16->oLDCV1CCW2F = FALSE; pDY16->oLDCV1CW2F = TRUE;

		} else {
			pDY16->oLDCV1CCW2F = FALSE; pDY16->oLDCV1CW2F = FALSE;
		}	
	} 
	else if (nIndex == 1) {
		if (m_chkMZLoadCVIO[1].GetCheck()) {
			if (m_chkMZLoadCVIO[0].GetCheck()) { 
				m_chkMZLoadCVIO[1].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Load 2F Left Conveyor CCW방향으로 회전 중입니다..."));				
				return;
			}
			pDY16->oLDCV1CCW2F = TRUE; pDY16->oLDCV1CW2F = TRUE;

		} else {
			pDY16->oLDCV1CCW2F = FALSE; pDY16->oLDCV1CW2F = FALSE;
		}
	}

	if (nIndex == 2) {
		if (m_chkMZLoadCVIO[2].GetCheck()) {
			if (m_chkMZLoadCVIO[3].GetCheck()) { 
				m_chkMZLoadCVIO[2].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Load 2F Right Conveyor CW방향으로 회전 중입니다..."));				
				return;
			}
			pDY16->oLDCV2CCW2F = FALSE; pDY16->oLDCV2CW2F = TRUE;

		} else {
			pDY16->oLDCV2CCW2F = FALSE; pDY16->oLDCV2CW2F = FALSE;
		}	
	} 
	else if (nIndex == 3) {
		if (m_chkMZLoadCVIO[3].GetCheck()) {
			if (m_chkMZLoadCVIO[2].GetCheck()) { 
				m_chkMZLoadCVIO[3].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Load 2F Right Conveyor CCW방향으로 회전 중입니다..."));				
				return;
			}
			pDY16->oLDCV2CCW2F = TRUE; pDY16->oLDCV2CW2F = TRUE;

		} else {
			pDY16->oLDCV2CCW2F = FALSE; pDY16->oLDCV2CW2F = FALSE;
		}
	}


	if (nIndex == 4) {
		if (m_chkMZLoadCVIO[4].GetCheck()) {
			if (m_chkMZLoadCVIO[5].GetCheck()) { 
				m_chkMZLoadCVIO[4].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Load 1F Left Conveyor CW방향으로 회전 중입니다..."));				
				return;
			}
			pDY16->oLDCV3CCW1F = FALSE; pDY16->oLDCV3CW1F = TRUE;

		} else {
			pDY16->oLDCV3CCW1F = FALSE; pDY16->oLDCV3CW1F = FALSE;
		}	
	} 
	else if (nIndex == 5) {
		if (m_chkMZLoadCVIO[5].GetCheck()) {
			if (m_chkMZLoadCVIO[4].GetCheck()) { 
				m_chkMZLoadCVIO[5].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Load 1F Left Conveyor CCW방향으로 회전 중입니다..."));				
				return;
			}
			pDY16->oLDCV3CCW1F = TRUE; pDY16->oLDCV3CW1F = TRUE;

		} else {
			pDY16->oLDCV3CCW1F = FALSE; pDY16->oLDCV3CW1F = FALSE;
		}

	}

	if (nIndex == 6) {
		if (m_chkMZLoadCVIO[6].GetCheck()) {
			if (m_chkMZLoadCVIO[7].GetCheck()) { 
				m_chkMZLoadCVIO[6].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Load 1F Right Conveyor CW방향으로 회전 중입니다..."));				
				return;
			}
			pDY16->oLDCV4CCW1F = FALSE; pDY16->oLDCV4CW1F = TRUE;

		} else {
			pDY16->oLDCV4CCW1F = FALSE; pDY16->oLDCV4CW1F = FALSE;
		}
	} 
	else if (nIndex == 7) {
		if (m_chkMZLoadCVIO[7].GetCheck()) {
			if (m_chkMZLoadCVIO[6].GetCheck()) { 
				m_chkMZLoadCVIO[7].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Load 1F Right Conveyor CCW방향으로 회전 중입니다..."));				
				return;
			}
			pDY16->oLDCV4CCW1F = TRUE; pDY16->oLDCV4CW1F = TRUE;

		} else {
			pDY16->oLDCV4CCW1F = FALSE; pDY16->oLDCV4CW1F = FALSE;
		}
	}

	g_objAJinAXL.Write_Output(16);

	m_strLog.Format("[Manual MZ Load] MZ Load Conveyor CCW/CW IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnbtnMZLoadCVIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_MZ_LDCV_IO_0;	

	DY_DATA_16 *pDY16 = g_objAJinAXL.Get_pDY16();

	if (nIndex == 0)	  { pDY16->oLDCVStopper2FUp = TRUE; pDY16->oLDCVStopper2FDn = FALSE; } 
	else if (nIndex == 1) { pDY16->oLDCVStopper2FUp = FALSE; pDY16->oLDCVStopper2FDn = TRUE; } 
	else if (nIndex == 2) { pDY16->oLDCVStopper1FUp = TRUE; pDY16->oLDCVStopper1FDn = FALSE; } 
	else if (nIndex == 3) { pDY16->oLDCVStopper1FUp = FALSE; pDY16->oLDCVStopper1FDn = TRUE; }

	g_objAJinAXL.Write_Output(16);

	m_strLog.Format("[Manual MZ Load] MZ Load Conveyor IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnbtnLDCVElevatorZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LDCV_ELEVATOR_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_LDCV_ELEVATOR_Z_0;	

	g_objCommon.Move_Position(AX_LDCV_ELEVATOR_Z, nIndex);

	m_strLog.Format("[Manual MZ Load] Load Conveyor Elevator Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnchkLDCVElevatorIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_CHK_LDCV_ELEVATOR_IO_0;	

	DY_DATA_17 *pDY17 = g_objAJinAXL.Get_pDY17();

	if (nIndex == 0)	  {
		if (m_chkLDCVElevatorIO[0].GetCheck()) {
			if (m_chkLDCVElevatorIO[1].GetCheck()) { 
				m_chkLDCVElevatorIO[0].SetCheck(FALSE);
				AfxMessageBox(_T("Load C/V Elevator CW방향으로 회전 중입니다..."));				
				return;
			}
			pDY17->oLDCVElevatorCVCCW = FALSE; pDY17->oLDCVElevatorCVCW = TRUE;

		} else {
			pDY17->oLDCVElevatorCVCCW = FALSE; pDY17->oLDCVElevatorCVCW = FALSE;
		}		
	} 
	else if (nIndex == 1) {
		if (m_chkLDCVElevatorIO[1].GetCheck()) {
			if (m_chkLDCVElevatorIO[0].GetCheck()) { 
				m_chkLDCVElevatorIO[1].SetCheck(FALSE);
				AfxMessageBox(_T("Load C/V Elevator CCW방향으로 회전 중입니다..."));				
				return;
			}
			pDY17->oLDCVElevatorCVCCW = TRUE; pDY17->oLDCVElevatorCVCW = TRUE;

		} else {
			pDY17->oLDCVElevatorCVCCW = FALSE; pDY17->oLDCVElevatorCVCW = FALSE;
		}
	}

	g_objAJinAXL.Write_Output(17);

	m_strLog.Format("[Manual MZ Load] Load C/V Elevator CCW/CW IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnbtnLDCVElevatorIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_LDCV_ELEVATOR_IO_0;	

	DY_DATA_17 *pDY17 = g_objAJinAXL.Get_pDY17();

	if (nIndex == 0)	  { pDY17->oLDCVElevatorStopperUp = TRUE; pDY17->oLDCVElevatorStopperDn = FALSE; } 
	else if (nIndex == 1) { pDY17->oLDCVElevatorStopperUp = FALSE; pDY17->oLDCVElevatorStopperDn = TRUE; } 
	else if (nIndex == 2) { pDY17->oLDCVElevatorAlignUp = TRUE; pDY17->oLDCVElevatorAlignDn = FALSE; } 
	else if (nIndex == 3) { pDY17->oLDCVElevatorAlignUp = FALSE; pDY17->oLDCVElevatorAlignDn = TRUE; } 
	else if (nIndex == 4) { pDY17->oLDCVElevatorAlignIn = TRUE; pDY17->oLDCVElevatorAlignOut = FALSE; } 
	else if (nIndex == 5) { pDY17->oLDCVElevatorAlignIn = FALSE; pDY17->oLDCVElevatorAlignOut = TRUE; }
	
	g_objAJinAXL.Write_Output(17);

	m_strLog.Format("[Manual MZ Load] Load C/V Elevator IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnbtnLoadFeederYClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LOAD_FEEDER_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_LOAD_FEEDER_Y_0;	

	//Interlock
	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
	if (g_objCommon.Check_Position(AX_LDMZ_ELEVATOR_Z, 0)) {
		AfxMessageBox(_T("Load Elevator Z축 MZ Down 위치입니다. 작업 위치로 이동하여 주십시오....."));
		return;
	}
	if (nIndex == 0 || nIndex == 1) {
		if (pDX15->iLDGripClose) {
			AfxMessageBox(_T("Load Feeder Grip Close 상태입니다. Grip Open 하여 주십시오....."));
			return;
		}
	}

	g_objCommon.Move_Position(AX_LOAD_FEEDER_Y, nIndex);

	m_strLog.Format("[Manual MZ Load] Load Feeder Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnbtnLoadFeederIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_LOAD_FEEDER_IO_0;	

	DY_DATA_15 *pDY15 = g_objAJinAXL.Get_pDY15();

	if (nIndex == 0)	  { pDY15->oLDGripOpen = TRUE; pDY15->oLDGripClose = FALSE; } 
	else if (nIndex == 1) { pDY15->oLDGripOpen = FALSE; pDY15->oLDGripClose = TRUE; }

	g_objAJinAXL.Write_Output(15);

	m_strLog.Format("[Manual MZ Load] Load Feeder IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnbtnLoadElevatorZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_LDMZ_ELEVATOR_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_LOAD_ELEVATOR_Z_0;	

	//Interlock
	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
	if (!g_objCommon.Check_Position(AX_LOAD_FEEDER_Y, 0) && !g_objCommon.Check_Position(AX_LOAD_FEEDER_Y, 2)) {
		AfxMessageBox(_T("Load Feeder Y축 Ready(Pull Next), Pull 위치에서만 작업 가능합니다....."));
		return;
	}
	if (pDX15->iLDRailCarrierChk1) {
		AfxMessageBox(_T("Load Rail Tray Check1(Front) 감지 상태 입니다....."));
		return;
	}

	g_objCommon.Move_Position(AX_LDMZ_ELEVATOR_Z, nIndex);

	m_strLog.Format("[Manual MZ Load] Load Elevator Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnbtnLoadElevatorIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_LOAD_ELEVATOR_IO_0;	

	DY_DATA_18 *pDY18 = g_objAJinAXL.Get_pDY18();

	if (nIndex == 0)      { pDY18->oLDMZElevatorClamp12In = TRUE; pDY18->oLDMZElevatorClamp12Out = FALSE; } 
	else if (nIndex == 1) { pDY18->oLDMZElevatorClamp12In = FALSE; pDY18->oLDMZElevatorClamp12Out = TRUE; } 
	else if (nIndex == 2) { pDY18->oLDMZElevatorClamp34In = TRUE; pDY18->oLDMZElevatorClamp34Out = FALSE; } 
	else if (nIndex == 3) { pDY18->oLDMZElevatorClamp34In = FALSE; pDY18->oLDMZElevatorClamp34Out = TRUE; }
	
	g_objAJinAXL.Write_Output(18);

	m_strLog.Format("[Manual MZ Load] Load Elevator IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnbtnMZTransferXClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_MZ_TRANSFER_X)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_MZ_TRANSFER_X_0 + 1;	

	// Interlock
	if (!g_objCommon.Check_Position(AX_MZ_TRANSFER_Z, 0)) {
		AfxMessageBox(_T("MZ Transfer Z축 Ready Up 위치에서만 작업 가능합니다....."));
		return;
	}

	// Move
	g_objCommon.Move_Position(AX_MZ_TRANSFER_X, nIndex);

	m_strLog.Format("[Manual MZ Load] MZ Transfer X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnbtnMZTransferZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_MZ_TRANSFER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_MZ_TRANSFER_Z_0;

	DX_DATA_20 *pDX20 = g_objAJinAXL.Get_pDX20();

	// Interlock
	if (nIndex != 0 && !g_objCommon.Check_Position(AX_MZ_TRANSFER_X, nIndex)) {
		if (nIndex == 1) { AfxMessageBox(_T("MZ Transfer X축이 Load Elevator 위치가 아닙니다.....")); return; }
		if (nIndex == 2) { AfxMessageBox(_T("MZ Transfer X축이 Load MZ 위치가 아닙니다.....")); return; }
		if (nIndex == 3) { AfxMessageBox(_T("MZ Transfer X축이 Buffer MZ 위치가 아닙니다.....")); return; }
		if (nIndex == 4) { AfxMessageBox(_T("MZ Transfer X축이 NG MZ 위치가 아닙니다.....")); return; }
		if (nIndex == 5) { AfxMessageBox(_T("MZ Transfer X축이 Good MZ 위치가 아닙니다.....")); return; }
		if (nIndex == 6) { AfxMessageBox(_T("MZ Transfer X축이 Unload Elevator 위치가 아닙니다.....")); return; }		
	}
	if (nIndex == 2) {
		if (!g_objCommon.Check_Position(AX_LDMZ_ELEVATOR_Z, 0)) { AfxMessageBox(_T("Load MZ Elevator Z축이 MZ Down 위치가 아닙니다.....")); return; }
	} else if (nIndex == 4) {
		if (!g_objCommon.Check_Position(AX_NGMZ_ELEVATOR_Z, 0)) { AfxMessageBox(_T("NG MZ Elevator Z축이 MZ Down 위치가 아닙니다.....")); return; }
	} else if (nIndex == 5) {
		if (!g_objCommon.Check_Position(AX_GDMZ_ELEVATOR_Z, 0)) { AfxMessageBox(_T("Good MZ Elevator Z축이 MZ Down 위치가 아닙니다.....")); return; }
	}

	if (nIndex != 0) {	// Ready Up 외 Down할때 Clamp 확인.
		if (!pDX20->iMZTransGrip12Open || !pDX20->iMZTransGrip34Open) {
			if (g_objCommon.Show_MsgBox(2, "MZ Transfer Grip Open 상태가 아닙니다. Z축 Down 하시겠습니까?") != IDOK) return;
		}
	}

	// Move
	g_objCommon.Move_Position(AX_MZ_TRANSFER_Z, nIndex);

	m_strLog.Format("[Manual MZ Load] MZ Transfer Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnbtnMZTransferIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_MZ_TRANSFER_IO_0;	

	DY_DATA_20 *pDY20 = g_objAJinAXL.Get_pDY20();

	if (nIndex == 0)      { pDY20->oMZTransGrip12Open = TRUE; pDY20->oMZTransGrip12Close = FALSE; } 
	else if (nIndex == 1) { pDY20->oMZTransGrip12Open = FALSE; pDY20->oMZTransGrip12Close = TRUE; } 
	else if (nIndex == 2) { pDY20->oMZTransGrip34Open = TRUE; pDY20->oMZTransGrip34Close = FALSE; } 
	else if (nIndex == 3) { pDY20->oMZTransGrip34Open = FALSE; pDY20->oMZTransGrip34Close = TRUE; }
	else if (nIndex == 4) { pDY20->oMZTransLockBarUp = TRUE; pDY20->oMZTransLockBarDn = FALSE; } 
	else if (nIndex == 5) { pDY20->oMZTransLockBarUp = FALSE; pDY20->oMZTransLockBarDn = TRUE; } 
	else if (nIndex == 6) { pDY20->oMZTransLockBarClose = TRUE; pDY20->oMZTransLockBarOpen = FALSE; } 
	else if (nIndex == 7) { pDY20->oMZTransLockBarClose = FALSE; pDY20->oMZTransLockBarOpen = TRUE; }

	g_objAJinAXL.Write_Output(20);

	m_strLog.Format("[Manual MZ Load] Load Elevator IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzLoadDlg::OnBtnBarcodeClick(UINT nID)
{
	int nIndex = nID - IDC_BTN_BARCODE_0;

	if (nIndex == 5) {	// 통신 해제 및 재연결
		DWORD dTemp = GetTickCount();
		g_objBarcodeLot_Cognex.Terminate(); 
		while (TRUE) { if (GetTickCount() - dTemp > 2000) break;}
		g_objBarcodeLot_Cognex.Initialize();	Sleep(5000);

	} else if (nIndex == 6) {	// 통신 해제 및 재연결
		DWORD dTemp = GetTickCount();
		g_objCarrierRFID_Load.Terminate(); 
		while (TRUE) { if (GetTickCount() - dTemp > 2000) break;}
		g_objCarrierRFID_Load.Initialize();	Sleep(5000);

	} else if (nIndex == 4) {
		int nIdx = nIndex / 2;
		CString strData = "";

		m_stcBarcode[nIdx].SetWindowText("");
		g_objCarrierRFID_Load.Send_RFIDRead();

		DWORD dwStart = GetTickCount();
		while (GetTickCount() - dwStart < 3000) {
			if (g_objCarrierRFID_Load.Is_RecvComplete()) {
				strData = g_objCarrierRFID_Load.Get_CarrierID();
				break;
			}
			theApp.DoEvents();
		}

		if (strData.GetLength() > 0) m_stcBarcode[nIdx].SetWindowText(strData);
		if (strData.GetLength() < 1) AfxMessageBox("Reading Fail.");

		m_strLog.Format("[Manual MZ Load] RFID Reading (%d) Click - No:%d, Data:%s", nIndex, nIdx+1, strData);
		g_objLogFile.Save_HandlerLog(m_strLog);

	} else if (nIndex == 7) {	// To Buffer
		CString strData = "";
		m_stcBarcode[0].GetWindowText(strData);
		gData.sMZID[5] = strData;

	} else if (nIndex == 8) {	// To Buffer
		CString strData = "";
		m_stcBarcode[1].GetWindowText(strData);
		gData.sMZID[5] = strData;

	} else {
		int nIdx = nIndex / 2;							// Barcode Index
		BOOL bOn = (nIndex % 2  == 0 ? TRUE : FALSE);	// Trigger On/Off

		m_stcBarcode[nIdx].SetWindowText("");
		g_objBarcodeLot_Cognex.Set_Trigger(nIdx+1, bOn);

		CString strData = "";
		if (bOn) {
			DWORD dwStart = GetTickCount();
			while (GetTickCount() - dwStart < 3000) {
				strData = g_objBarcodeLot_Cognex.Get_BarcodeLot(nIdx+1);
				if (strData != "") break;
				theApp.DoEvents();
			}
			if (strData.GetLength() > 0) m_stcBarcode[nIdx].SetWindowText(strData);
			if (strData.GetLength() < 1) { g_objBarcodeLot_Cognex.Set_Trigger(nIdx+1, FALSE); AfxMessageBox("Reading Fail."); }
		}

		m_strLog.Format("[Manual MZ Load] Barcode Trigger (%d) Click - No:%d, Data:%s", nIndex, nIdx+1, strData);
		g_objLogFile.Save_HandlerLog(m_strLog);
	}
}
///////////////////////////////////////////////////////////////////////////////
