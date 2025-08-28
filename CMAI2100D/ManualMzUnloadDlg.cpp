// ManualMzUnloadDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMAI2100.h"
#include "ManualMzUnloadDlg.h"
#include "afxdialogex.h"
#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"
#include "Common.h"
#include "SequenceMain.h"
#include "BarcodeLot_Cognex.h"
#include "BarcodeLot_RS232.h"
#include "CarrierRFID_Good.h"
#include "CarrierRFID_NG.h"

// CManualMzUnloadDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualMzUnloadDlg, CDialogEx)

CManualMzUnloadDlg::CManualMzUnloadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualMzUnloadDlg::IDD, pParent)
{
}

CManualMzUnloadDlg::~CManualMzUnloadDlg()
{
}

void CManualMzUnloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i <12; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_BTN_NG_FEEDER_Y_0 + i, m_btnNGFeederY[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_NG_FEEDER_IO_0 + i, m_btnNGFeederIO[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_BTN_GOOD_FEEDER_Y_0 + i, m_btnGoodFeederY[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_BTN_GOOD_FEEDER_IO_0 + i, m_btnGoodFeederIO[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_BTN_NG_ELEVATOR_Z_0 + i, m_btnNGElevatorZ[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_NG_ELEVATOR_IO_0 + i, m_btnNGElevatorIO[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_BTN_GOOD_ELEVATOR_Z_0 + i, m_btnGoodElevatorZ[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_BTN_GOOD_ELEVATOR_IO_0 + i, m_btnGoodElevatorIO[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_BTN_ULCV_ELEVATOR_Z_0 + i, m_btnULCVElevatorZ[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_CHK_ULCV_ELEVATOR_IO_0 + i, m_chkULCVElevatorIO[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_CHK_MZ_ULCV_IO_0 + i, m_chkMZUnloadCVIO[i]);

	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_LED_NG_FEEDER_IO_0 + i, m_ledNGFeederIO[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_LED_GOOD_FEEDER_IO_0 + i, m_ledGoodFeederIO[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_NG_ELEVATOR_IO_0 + i, m_ledNGElevatorIO[i]);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_LED_GOOD_ELEVATOR_IO_0 + i, m_ledGoodElevatorIO[i]);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_LED_ULCV_ELEVATOR_IO_0 + i, m_ledULCVElevatorIO[i]);
	for (int i = 0; i <14; i++) DDX_Control(pDX, IDC_LED_MZ_ULCV_IO_0 + i, m_ledMZUnloadCVIO[i]);

	for (int i = 0; i < 4; i++)  DDX_Control(pDX, IDC_LBL_UL_BARCODE_0 + i, m_lblBarcode[i]);
	for (int i = 0; i < 4; i++)  DDX_Control(pDX, IDC_STC_UL_BARCODE_0 + i, m_stcBarcode[i]);
	for (int i = 0; i < 9; i++)  DDX_Control(pDX, IDC_BTN_UL_BARCODE_0 + i, m_btnBarcode[i]);

}

BEGIN_MESSAGE_MAP(CManualMzUnloadDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_NG_FEEDER_Y_0, IDC_BTN_NG_FEEDER_Y_2,    OnbtnNGFeederYClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_NG_FEEDER_IO_0, IDC_BTN_NG_FEEDER_IO_1,    OnbtnNGFeederIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_FEEDER_Y_0, IDC_BTN_GOOD_FEEDER_Y_2,    OnbtnGoodFeederYClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_FEEDER_IO_0, IDC_BTN_GOOD_FEEDER_IO_1,    OnbtnGoodFeederIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_NG_ELEVATOR_Z_0, IDC_BTN_NG_ELEVATOR_Z_2,    OnbtnNGElevatorZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_NG_ELEVATOR_IO_0, IDC_BTN_NG_ELEVATOR_IO_3,    OnbtnNGElevatorIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_ELEVATOR_Z_0, IDC_BTN_GOOD_ELEVATOR_Z_2,    OnbtnGoodElevatorZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_GOOD_ELEVATOR_IO_0, IDC_BTN_GOOD_ELEVATOR_IO_3,    OnbtnGoodElevatorIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_ULCV_ELEVATOR_Z_0, IDC_BTN_ULCV_ELEVATOR_Z_2,    OnbtnULCVElevatorZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_ULCV_ELEVATOR_IO_0, IDC_CHK_ULCV_ELEVATOR_IO_1,    OnchkULCVElevatorIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_MZ_ULCV_IO_0, IDC_CHK_MZ_ULCV_IO_7,    OnchkMZUnlaodCVIOClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_UL_BARCODE_0, IDC_BTN_UL_BARCODE_8, OnBtnULBarcodeClick)

END_MESSAGE_MAP()

// CManualMzUnloadDlg 메시지 처리기입니다.
BOOL CManualMzUnloadDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualMzUnloadDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualMzUnloadDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	Display_Status();

	m_strLog.Format("[Manual Elevator] Show Window");
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::Initial_Controls() 
{
	for (int i = 0; i <  2; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 2; i <  4; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x10, 0x10, 0xFF), COLOR_DEFAULT);
	for (int i = 4; i <  6; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 6; i <  8; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x10, 0x10, 0xFF), COLOR_DEFAULT);
	for (int i = 8; i < 12; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x10, 0x90, 0x00), COLOR_DEFAULT);
	for (int i = 0; i <  5; i++) m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));
	for (int i = 0; i <  5; i++) m_stcAxisPos[i].Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x10, 0x10, 0x60));
	for (int i = 0; i <  3; i++) m_btnNGFeederY[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnNGFeederIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnGoodFeederY[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnGoodFeederIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnNGElevatorZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnNGElevatorIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnGoodElevatorZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnGoodElevatorIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnULCVElevatorZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_chkULCVElevatorIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  8; i++) m_chkMZUnloadCVIO[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);

	for (int i = 0; i <  7; i++) m_ledNGFeederIO[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  7; i++) m_ledGoodFeederIO[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  5; i++) m_ledNGElevatorIO[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  5; i++) m_ledGoodElevatorIO[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  2; i++) m_ledULCVElevatorIO[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 14; i++) m_ledMZUnloadCVIO[i].Init_Ctrl("Arial", 9, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);

	for (int i = 0; i < 4; i++)  m_lblBarcode[i].Init_Ctrl("Arial", 11, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));
	for (int i = 0; i < 4; i++)  m_stcBarcode[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xFF, 0x00, 0x00), RGB(0xF0, 0xF0, 0xD0));
	for (int i = 0; i < 9; i++)  m_btnBarcode[i].Init_Ctrl("Arial", 11, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
}

void CManualMzUnloadDlg::Display_Status()
{
	CString strPos;
	double dPos = 0.0;
	for (int i = 0; i < 2; i++) {
		dPos = g_objAJinAXL.Get_Position(AX_NG_FEEDER_Y + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i].SetWindowText(strPos);
	}
	for (int i = 0; i < 3; i++) {
		dPos = g_objAJinAXL.Get_Position(AX_NGMZ_ELEVATOR_Z + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i + 2].SetWindowText(strPos);
	}

	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
	DX_DATA_17 *pDX17 = g_objAJinAXL.Get_pDX17();
	DX_DATA_18 *pDX18 = g_objAJinAXL.Get_pDX18();
	DX_DATA_19 *pDX19 = g_objAJinAXL.Get_pDX19();

	m_ledNGFeederIO[0].Set_On(pDX15->iNGGripOpen);
	m_ledNGFeederIO[1].Set_On(pDX15->iNGGripClose);
	m_ledNGFeederIO[2].Set_On(pDX15->iNGGripCarrierChk);
	m_ledNGFeederIO[3].Set_On(pDX15->iNGGripOverload);
	m_ledNGFeederIO[4].Set_On(pDX15->iNGRailCarrierChk1);
	m_ledNGFeederIO[5].Set_On(pDX15->iNGRailCarrierChk2);
	m_ledNGFeederIO[6].Set_On(pDX15->iNGMZCarrierExist);

	m_ledGoodFeederIO[0].Set_On(pDX15->iGDGripOpen);
	m_ledGoodFeederIO[1].Set_On(pDX15->iGDGripClose);
	m_ledGoodFeederIO[2].Set_On(pDX15->iGDGripCarrierChk);
	m_ledGoodFeederIO[3].Set_On(pDX15->iGDGripOverload);
	m_ledGoodFeederIO[4].Set_On(pDX15->iGDRailCarrierChk1);
	m_ledGoodFeederIO[5].Set_On(pDX15->iGDRailCarrierChk2);
	m_ledGoodFeederIO[6].Set_On(pDX15->iGDMZCarrierExist);

	m_ledNGElevatorIO[0].Set_On(pDX18->iNGMZElevatorClamp12In);
	m_ledNGElevatorIO[1].Set_On(pDX18->iNGMZElevatorClamp12Out);
	m_ledNGElevatorIO[2].Set_On(pDX18->iNGMZElevatorClamp34In);
	m_ledNGElevatorIO[3].Set_On(pDX18->iNGMZElevatorClamp34Out);
	m_ledNGElevatorIO[4].Set_On(pDX18->iNGMZElevatorExist);

	m_ledGoodElevatorIO[0].Set_On(pDX18->iGDMZElevatorClamp12In);
	m_ledGoodElevatorIO[1].Set_On(pDX18->iGDMZElevatorClamp12Out);
	m_ledGoodElevatorIO[2].Set_On(pDX18->iGDMZElevatorClamp34In);
	m_ledGoodElevatorIO[3].Set_On(pDX18->iGDMZElevatorClamp34Out);
	m_ledGoodElevatorIO[4].Set_On(pDX18->iGDMZElevatorExist);

	m_ledULCVElevatorIO[0].Set_On(pDX17->iULCVElevatorMZExist);
	m_ledULCVElevatorIO[1].Set_On(pDX17->iULCVElevatorStop);

	m_ledMZUnloadCVIO[ 0].Set_On(pDX19->iULCV2FCnt1);
	m_ledMZUnloadCVIO[ 1].Set_On(pDX19->iULCV2FCnt2);
	m_ledMZUnloadCVIO[ 2].Set_On(pDX19->iULCV2FCnt3);
	m_ledMZUnloadCVIO[ 3].Set_On(pDX19->iULCV2FCnt4);
	m_ledMZUnloadCVIO[ 4].Set_On(pDX19->iULCV2FCnt5);
	m_ledMZUnloadCVIO[ 5].Set_On(pDX19->iULCV2FCnt6);
	m_ledMZUnloadCVIO[ 6].Set_On(pDX19->iULCV2FStop);
	m_ledMZUnloadCVIO[ 7].Set_On(pDX19->iULCV1FCnt1);
	m_ledMZUnloadCVIO[ 8].Set_On(pDX19->iULCV1FCnt2);
	m_ledMZUnloadCVIO[ 9].Set_On(pDX19->iULCV1FCnt3);
	m_ledMZUnloadCVIO[10].Set_On(pDX19->iULCV1FCnt4);
	m_ledMZUnloadCVIO[11].Set_On(pDX19->iULCV1FCnt5);
	m_ledMZUnloadCVIO[12].Set_On(pDX19->iULCV1FCnt6);
	m_ledMZUnloadCVIO[13].Set_On(pDX19->iULCV1FStop);
}

///////////////////////////////////////////////////////////////////////////////
void CManualMzUnloadDlg::OnbtnNGFeederYClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_NG_FEEDER_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_NG_FEEDER_Y_0;	

	//Interlock
	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
	if (g_objCommon.Check_Position(AX_NGMZ_ELEVATOR_Z, 0)) {
		AfxMessageBox(_T("NG Elevator Z축 MZ Down 위치입니다. 작업 위치로 이동하여 주십시오....."));
		return;
	}
	if (nIndex == 0 || nIndex == 1) {
		if (pDX15->iNGGripClose) {
			AfxMessageBox(_T("NG Feeder Grip Close 상태입니다. Grip Open 하여 주십시오....."));
			return;
		}
	}

	g_objCommon.Move_Position(AX_NG_FEEDER_Y, nIndex);

	m_strLog.Format("[Manual MZ Unload] NG Feeder Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::OnbtnNGFeederIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_NG_FEEDER_IO_0;	

	DY_DATA_15 *pDY15 = g_objAJinAXL.Get_pDY15();

	if (nIndex == 0)	  { pDY15->oNGGripOpen = TRUE; pDY15->oNGGripClose = FALSE; }
	else if (nIndex == 1) { pDY15->oNGGripOpen = FALSE; pDY15->oNGGripClose = TRUE; }
	
	g_objAJinAXL.Write_Output(15);

	m_strLog.Format("[Manual MZ Unload] NG Feeder IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::OnbtnGoodFeederYClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_GOOD_FEEDER_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_GOOD_FEEDER_Y_0;	

	//Interlock
	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
	if (g_objCommon.Check_Position(AX_GDMZ_ELEVATOR_Z, 0)) {
		AfxMessageBox(_T("Good Elevator Z축 MZ Down 위치입니다. 작업 위치로 이동하여 주십시오....."));
		return;
	}
	if (nIndex == 0 || nIndex == 1) {
		if (pDX15->iGDGripClose) {
			AfxMessageBox(_T("Good Feeder Grip Close 상태입니다. Grip Open 하여 주십시오....."));
			return;
		}
	}

	g_objCommon.Move_Position(AX_GOOD_FEEDER_Y, nIndex);

	m_strLog.Format("[Manual MZ Unload] Good Feeder Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::OnbtnGoodFeederIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_GOOD_FEEDER_IO_0;	

	DY_DATA_15 *pDY15 = g_objAJinAXL.Get_pDY15();

	if (nIndex == 0)	  { pDY15->oGDGripOpen = TRUE; pDY15->oGDGripClose = FALSE; }
	else if (nIndex == 1) { pDY15->oGDGripOpen = FALSE; pDY15->oGDGripClose = TRUE; }
	
	g_objAJinAXL.Write_Output(15);

	m_strLog.Format("[Manual MZ Unload] Good Feeder IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::OnbtnNGElevatorZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_NGMZ_ELEVATOR_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_NG_ELEVATOR_Z_0;	

	//Interlock
	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
	if (!g_objCommon.Check_Position(AX_NG_FEEDER_Y, 0) && !g_objCommon.Check_Position(AX_NG_FEEDER_Y, 2)) {
		AfxMessageBox(_T("NG Feeder Y축 Ready(Pull Next) or Pull 위치에서만 작업 가능합니다....."));
		return;
	}
	if (pDX15->iNGRailCarrierChk1) {
		AfxMessageBox(_T("NG Rail Tray Check1(Front) 감지 상태 입니다....."));
		return;
	}
	
	g_objCommon.Move_Position(AX_NGMZ_ELEVATOR_Z, nIndex);

	m_strLog.Format("[Manual MZ Unload] NG Elevator Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::OnbtnNGElevatorIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_NG_ELEVATOR_IO_0;	

	DY_DATA_18 *pDY18 = g_objAJinAXL.Get_pDY18();

	if (nIndex == 0)	  { pDY18->oNGMZElevatorClamp12In = TRUE; pDY18->oNGMZElevatorClamp12Out = FALSE; }
	else if (nIndex == 1) { pDY18->oNGMZElevatorClamp12In = FALSE; pDY18->oNGMZElevatorClamp12Out = TRUE; }
	else if (nIndex == 2) { pDY18->oNGMZElevatorClamp34In = TRUE; pDY18->oNGMZElevatorClamp34Out = FALSE; }
	else if (nIndex == 3) { pDY18->oNGMZElevatorClamp34In = FALSE; pDY18->oNGMZElevatorClamp34Out = TRUE; }
	
	g_objAJinAXL.Write_Output(18);

	m_strLog.Format("[Manual MZ Unload] NG Elevator IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::OnbtnGoodElevatorZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_GDMZ_ELEVATOR_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_GOOD_ELEVATOR_Z_0;	

	//Interlock
	DX_DATA_15 *pDX15 = g_objAJinAXL.Get_pDX15();
	if (!g_objCommon.Check_Position(AX_GOOD_FEEDER_Y, 0) && !g_objCommon.Check_Position(AX_GOOD_FEEDER_Y, 2)) {
		AfxMessageBox(_T("Good Feeder Y축 Ready(Pull Next) or Pull 위치에서만 작업 가능합니다....."));
		return;
	}
	if (pDX15->iGDRailCarrierChk1) {
		AfxMessageBox(_T("Good Rail Tray Check1(Front) 감지 상태 입니다....."));
		return;
	}

	g_objCommon.Move_Position(AX_GDMZ_ELEVATOR_Z, nIndex);

	m_strLog.Format("[Manual MZ Unload] Good Elevator Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::OnbtnGoodElevatorIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_GOOD_ELEVATOR_IO_0;	

	DY_DATA_18 *pDY18 = g_objAJinAXL.Get_pDY18();

	if (nIndex == 0)	  { pDY18->oGDMZElevatorClamp12In = TRUE; pDY18->oGDMZElevatorClamp12Out = FALSE; }
	else if (nIndex == 1) { pDY18->oGDMZElevatorClamp12In = FALSE; pDY18->oGDMZElevatorClamp12Out = TRUE; }
	else if (nIndex == 2) { pDY18->oGDMZElevatorClamp34In = TRUE; pDY18->oGDMZElevatorClamp34Out = FALSE; }
	else if (nIndex == 3) { pDY18->oGDMZElevatorClamp34In = FALSE; pDY18->oGDMZElevatorClamp34Out = TRUE; }
	
	g_objAJinAXL.Write_Output(18);

	m_strLog.Format("[Manual MZ Unload] Good Elevator IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::OnbtnULCVElevatorZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_ULCV_ELEVATOR_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_ULCV_ELEVATOR_Z_0;	

	g_objCommon.Move_Position(AX_ULCV_ELEVATOR_Z, nIndex);

	m_strLog.Format("[Manual MZ Unload] Unload Conveyor Elevator Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::OnchkULCVElevatorIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_CHK_ULCV_ELEVATOR_IO_0;	

	DY_DATA_17 *pDY17 = g_objAJinAXL.Get_pDY17();

	if (nIndex == 0)	  {
		if (m_chkULCVElevatorIO[0].GetCheck()) {
			if (m_chkULCVElevatorIO[1].GetCheck()) { 
				m_chkULCVElevatorIO[0].SetCheck(FALSE);
				AfxMessageBox(_T("Uload C/V Elevator CW방향으로 회전 중입니다..."));				
				return;
			}
			pDY17->oULCVElevatorCVCCW = FALSE; pDY17->oULCVElevatorCVCW = TRUE;

		} else {
			pDY17->oULCVElevatorCVCCW = FALSE; pDY17->oULCVElevatorCVCW = FALSE;
		}		
	} 
	else if (nIndex == 1) {
		if (m_chkULCVElevatorIO[1].GetCheck()) {
			if (m_chkULCVElevatorIO[0].GetCheck()) { 
				m_chkULCVElevatorIO[1].SetCheck(FALSE);
				AfxMessageBox(_T("Unload C/V Elevator CCW방향으로 회전 중입니다..."));				
				return;
			}
			pDY17->oULCVElevatorCVCCW = TRUE; pDY17->oULCVElevatorCVCW = TRUE;

		} else {
			pDY17->oULCVElevatorCVCCW = FALSE; pDY17->oULCVElevatorCVCW = FALSE;
		}
	}

	g_objAJinAXL.Write_Output(17);

	m_strLog.Format("[Manual MZ Unload] Unload C/V Elevator CCW/CW IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::OnchkMZUnlaodCVIOClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_CHK_MZ_ULCV_IO_0;	

	DY_DATA_19 *pDY19 = g_objAJinAXL.Get_pDY19();

	if (nIndex == 0) {
		if (m_chkMZUnloadCVIO[0].GetCheck()) {
			if (m_chkMZUnloadCVIO[1].GetCheck()) { 
				m_chkMZUnloadCVIO[0].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Unload 2F Left Conveyor CW방향으로 회전 중입니다..."));				
				return;
			}
			pDY19->oULCV1CCW2F = FALSE; pDY19->oULCV1CW2F = TRUE;

		} else {
			pDY19->oULCV1CCW2F = FALSE; pDY19->oULCV1CW2F = FALSE;
		}	
	} 
	else if (nIndex == 1) {
		if (m_chkMZUnloadCVIO[1].GetCheck()) {
			if (m_chkMZUnloadCVIO[0].GetCheck()) { 
				m_chkMZUnloadCVIO[1].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Unload 2F Left Conveyor CCW방향으로 회전 중입니다..."));				
				return;
			}
			pDY19->oULCV1CCW2F = TRUE; pDY19->oULCV1CW2F = TRUE;

		} else {
			pDY19->oULCV1CCW2F = FALSE; pDY19->oULCV1CW2F = FALSE;
		}
	}

	if (nIndex == 2) {
		if (m_chkMZUnloadCVIO[2].GetCheck()) {
			if (m_chkMZUnloadCVIO[3].GetCheck()) { 
				m_chkMZUnloadCVIO[2].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Unload 2F Right Conveyor CW방향으로 회전 중입니다..."));				
				return;
			}
			pDY19->oULCV2CCW2F = FALSE; pDY19->oULCV2CW2F = TRUE;

		} else {
			pDY19->oULCV2CCW2F = FALSE; pDY19->oULCV2CW2F = FALSE;
		}	
	} 
	else if (nIndex == 3) {
		if (m_chkMZUnloadCVIO[3].GetCheck()) {
			if (m_chkMZUnloadCVIO[2].GetCheck()) { 
				m_chkMZUnloadCVIO[3].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Unload 2F Right Conveyor CCW방향으로 회전 중입니다..."));				
				return;
			}
			pDY19->oULCV2CCW2F = TRUE; pDY19->oULCV2CW2F = TRUE;

		} else {
			pDY19->oULCV2CCW2F = FALSE; pDY19->oULCV2CW2F = FALSE;
		}
	}


	if (nIndex == 4) {
		if (m_chkMZUnloadCVIO[4].GetCheck()) {
			if (m_chkMZUnloadCVIO[5].GetCheck()) { 
				m_chkMZUnloadCVIO[4].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Unload 1F Left Conveyor CW방향으로 회전 중입니다..."));				
				return;
			}
			pDY19->oULCV3CCW1F = FALSE; pDY19->oULCV3CW1F = TRUE;

		} else {
			pDY19->oULCV3CCW1F = FALSE; pDY19->oULCV3CW1F = FALSE;
		}	
	} 
	else if (nIndex == 5) {
		if (m_chkMZUnloadCVIO[5].GetCheck()) {
			if (m_chkMZUnloadCVIO[4].GetCheck()) { 
				m_chkMZUnloadCVIO[5].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Unload 1F Left Conveyor CCW방향으로 회전 중입니다..."));				
				return;
			}
			pDY19->oULCV3CCW1F = TRUE; pDY19->oULCV3CW1F = TRUE;

		} else {
			pDY19->oULCV3CCW1F = FALSE; pDY19->oULCV3CW1F = FALSE;
		}

	}

	if (nIndex == 6) {
		if (m_chkMZUnloadCVIO[6].GetCheck()) {
			if (m_chkMZUnloadCVIO[7].GetCheck()) { 
				m_chkMZUnloadCVIO[6].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Unload 1F Right Conveyor CW방향으로 회전 중입니다..."));				
				return;
			}
			pDY19->oULCV4CCW1F = FALSE; pDY19->oULCV4CW1F = TRUE;

		} else {
			pDY19->oULCV4CCW1F = FALSE; pDY19->oULCV4CW1F = FALSE;
		}
	} 
	else if (nIndex == 7) {
		if (m_chkMZUnloadCVIO[7].GetCheck()) {
			if (m_chkMZUnloadCVIO[6].GetCheck()) { 
				m_chkMZUnloadCVIO[7].SetCheck(FALSE);
				AfxMessageBox(_T("MZ Unload 1F Right Conveyor CCW방향으로 회전 중입니다..."));				
				return;
			}
			pDY19->oULCV4CCW1F = TRUE; pDY19->oULCV4CW1F = TRUE;

		} else {
			pDY19->oULCV4CCW1F = FALSE; pDY19->oULCV4CW1F = FALSE;
		}
	}

	g_objAJinAXL.Write_Output(19);

	m_strLog.Format("[Manual MZ Unload] MZ Unload Conveyor CCW/CW IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualMzUnloadDlg::OnBtnULBarcodeClick(UINT nID)
{
	int nIndex = nID - IDC_BTN_UL_BARCODE_0;

	if (nIndex == 6) {	// 통신 해제 및 재연결
		DWORD dTemp = GetTickCount();
		g_objBarcodeLot_Cognex.Terminate(); 
		while (TRUE) { if (GetTickCount() - dTemp > 2000) break;}
		g_objBarcodeLot_Cognex.Initialize();	Sleep(5000);

	} else if (nIndex == 7) {	// 통신 해제 및 재연결
		DWORD dTemp = GetTickCount();
		g_objCarrierRFID_Good.Terminate(); 
		while (TRUE) { if (GetTickCount() - dTemp > 2000) break;}
		g_objCarrierRFID_Good.Initialize();	Sleep(5000);

	} else if (nIndex == 8) {	// 통신 해제 및 재연결
		DWORD dTemp = GetTickCount();
		g_objCarrierRFID_NG.Terminate(); 
		while (TRUE) { if (GetTickCount() - dTemp > 2000) break;}
		g_objCarrierRFID_Good.Initialize();	Sleep(5000);

	} else if (nIndex == 4 || nIndex == 5) {
		int nIdx = nIndex / 2 + nIndex % 2;
		CString strData = "";

		m_stcBarcode[nIdx].SetWindowText("");
		if (nIndex == 4) g_objCarrierRFID_Good.Send_RFIDRead();
		if (nIndex == 5) g_objCarrierRFID_NG.Send_RFIDRead();

		DWORD dwStart = GetTickCount();
		while (GetTickCount() - dwStart < 3000) {
			if ((nIndex == 4 && g_objCarrierRFID_Good.Is_RecvComplete()) ||
				(nIndex == 5 && g_objCarrierRFID_NG.Is_RecvComplete()) ) {

				if (nIndex == 4) strData = g_objCarrierRFID_Good.Get_CarrierID();
				if (nIndex == 5) strData = g_objCarrierRFID_NG.Get_CarrierID();
				break;
			}
			theApp.DoEvents();
		}

		if (strData.GetLength() > 0) m_stcBarcode[nIdx].SetWindowText(strData);
		if (strData.GetLength() < 1) AfxMessageBox("Reading Fail.");

		m_strLog.Format("[Manual MZ Unload] RFID Reading (%d) Click - No:%d, Data:%s", nIndex, nIdx+1, strData);
		g_objLogFile.Save_HandlerLog(m_strLog);

	} else {
		int nIdx = nIndex / 2;							// Barcode Index, Good:3, NG:4
		BOOL bOn = (nIndex % 2  == 0 ? TRUE : FALSE);	// Trigger On/Off

		m_stcBarcode[nIdx].SetWindowText("");
		g_objBarcodeLot_Cognex.Set_Trigger(nIdx+1+2, bOn);

		CString strData = "";
		if (bOn) {
			DWORD dwStart = GetTickCount();
			while (GetTickCount() - dwStart < 3000) {
				strData = g_objBarcodeLot_Cognex.Get_BarcodeLot(nIdx+1+2);
				if (strData != "") break;
				theApp.DoEvents();
			}
			if (strData.GetLength() > 0) m_stcBarcode[nIdx].SetWindowText(strData);
			if (strData.GetLength() < 1) { g_objBarcodeLot_Cognex.Set_Trigger(nIdx+1, FALSE); AfxMessageBox("Reading Fail."); }
		}

		m_strLog.Format("[Manual MZ Unload] Barcode Trigger (%d) Click - No:%d, Data:%s", nIndex, nIdx+1, strData);
		g_objLogFile.Save_HandlerLog(m_strLog);
	}
}
///////////////////////////////////////////////////////////////////////////////
