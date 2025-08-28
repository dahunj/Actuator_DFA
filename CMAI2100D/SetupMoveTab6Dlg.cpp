// SetupMoveTab6Dlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMAI2100.h"
#include "SetupMoveTab6Dlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"

#include "AJinDefine.h"

// CSetupMoveTab6Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSetupMoveTab6Dlg, CDialogEx)

CSetupMoveTab6Dlg::CSetupMoveTab6Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupMoveTab6Dlg::IDD, pParent)
{
}

CSetupMoveTab6Dlg::~CSetupMoveTab6Dlg()
{
}

void CSetupMoveTab6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 37; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_LOAD_FEEDER_Y_0 + i, m_stcLoadFeederY[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_NG_FEEDER_Y_0 + i, m_stcNGFeederY[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_GOOD_FEEDER_Y_0 + i, m_stcGoodFeederY[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_LDCV_ELEVATOR_Z_0 + i, m_stcLDCVElevatorZ[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_LDMZ_ELEVATOR_Z_0 + i, m_stcLDMZElevatorZ[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_NGMZ_ELEVATOR_Z_0 + i, m_stcNGMZElevatorZ[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_GDMZ_ELEVATOR_Z_0 + i, m_stcGDMZElevatorZ[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_ULCV_ELEVATOR_Z_0 + i, m_stcULCVElevatorZ[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_STC_MZ_TRANSFER_X_0 + i, m_stcMZTransferX[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_STC_MZ_TRANSFER_Z_0 + i, m_stcMZTransferZ[i]);
}

BEGIN_MESSAGE_MAP(CSetupMoveTab6Dlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOAD_FEEDER_Y_0,		IDC_STC_LOAD_FEEDER_Y_2,	OnStcLoadFeederY)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_NG_FEEDER_Y_0,		IDC_STC_NG_FEEDER_Y_2,		OnStcNGFeederY)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_GOOD_FEEDER_Y_0,		IDC_STC_GOOD_FEEDER_Y_2,	OnStcGoodFeederY)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LDCV_ELEVATOR_Z_0,	IDC_STC_LDCV_ELEVATOR_Z_2,	OnStcLDCVElevator)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LDMZ_ELEVATOR_Z_0,	IDC_STC_LDMZ_ELEVATOR_Z_2,	OnStcLDMZElevatorZ)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_NGMZ_ELEVATOR_Z_0,	IDC_STC_NGMZ_ELEVATOR_Z_2,	OnStcNGMZElevatorZ)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_GDMZ_ELEVATOR_Z_0,	IDC_STC_GDMZ_ELEVATOR_Z_2,	OnStcGDMZElevatorZ)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_ULCV_ELEVATOR_Z_0,	IDC_STC_ULCV_ELEVATOR_Z_2,	OnStcULCVElevator)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_MZ_TRANSFER_X_0,		IDC_STC_MZ_TRANSFER_X_5,	OnStcMZTransferX)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_MZ_TRANSFER_Z_0,		IDC_STC_MZ_TRANSFER_Z_6,	OnStcMZTransferZ)
END_MESSAGE_MAP()

// CSetupMoveTab6Dlg 메시지 처리기입니다.

BOOL CSetupMoveTab6Dlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(NULL, 5, 65, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSetupMoveTab6Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupMoveTab6Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_MoveData();

	g_objLogFile.Save_HandlerLog("[Setup Move] Show Window - Tab6");
}

void CSetupMoveTab6Dlg::OnStcLoadFeederY(UINT nID)
{
	int ID = nID - IDC_STC_LOAD_FEEDER_Y_0;

	CString strOld, strNew;
	m_stcLoadFeederY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LOAD_FEEDER_Y) != IDOK) return;
	if (g_objCommon.Check_MoveData(AX_LOAD_FEEDER_Y, ID, strNew)==FALSE) return;

	m_stcLoadFeederY[ID].SetWindowText(strNew);

	m_strLog.Format("[Setup - MoveTab Dialog] OnStcLoadFeederY - Data(%d-%s)", ID, strNew);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMoveTab6Dlg::OnStcNGFeederY(UINT nID)
{
	int ID = nID - IDC_STC_NG_FEEDER_Y_0;

	CString strOld, strNew;
	m_stcNGFeederY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_NG_FEEDER_Y) != IDOK) return;
	if (g_objCommon.Check_MoveData(AX_NG_FEEDER_Y, ID, strNew)==FALSE) return;

	m_stcNGFeederY[ID].SetWindowText(strNew);

	m_strLog.Format("[Setup - MoveTab Dialog] OnStcNGFeederY - Data(%d-%s)", ID, strNew);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMoveTab6Dlg::OnStcGoodFeederY(UINT nID)
{
	int ID = nID - IDC_STC_GOOD_FEEDER_Y_0;

	CString strOld, strNew;
	m_stcGoodFeederY[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_GOOD_FEEDER_Y) != IDOK) return;
	if (g_objCommon.Check_MoveData(AX_GOOD_FEEDER_Y, ID, strNew)==FALSE) return;

	m_stcGoodFeederY[ID].SetWindowText(strNew);

	m_strLog.Format("[Setup - MoveTab Dialog] OnStcGoodFeederY - Data(%d-%s)", ID, strNew);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMoveTab6Dlg::OnStcLDCVElevator(UINT nID)
{
	int ID = nID - IDC_STC_LDCV_ELEVATOR_Z_0;

	CString strOld, strNew;
	m_stcLDCVElevatorZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LDCV_ELEVATOR_Z) != IDOK) return;
	if (g_objCommon.Check_MoveData(AX_LDCV_ELEVATOR_Z, ID, strNew)==FALSE) return;

	m_stcLDCVElevatorZ[ID].SetWindowText(strNew);

	m_strLog.Format("[Setup - MoveTab Dialog] OnStcLDCVElevator - Data(%d-%s)", ID, strNew);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMoveTab6Dlg::OnStcLDMZElevatorZ(UINT nID)
{
	int ID = nID - IDC_STC_LDMZ_ELEVATOR_Z_0;

	CString strOld, strNew;
	m_stcLDMZElevatorZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_LDMZ_ELEVATOR_Z) != IDOK) return;
	if (g_objCommon.Check_MoveData(AX_LDMZ_ELEVATOR_Z, ID, strNew)==FALSE) return;

	m_stcLDMZElevatorZ[ID].SetWindowText(strNew);

	m_strLog.Format("[Setup - MoveTab Dialog] OnStcLDMZElevatorZ - Data(%d-%s)", ID, strNew);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMoveTab6Dlg::OnStcNGMZElevatorZ(UINT nID)
{
	int ID = nID - IDC_STC_NGMZ_ELEVATOR_Z_0;

	CString strOld, strNew;
	m_stcNGMZElevatorZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_NGMZ_ELEVATOR_Z) != IDOK) return;
	if (g_objCommon.Check_MoveData(AX_NGMZ_ELEVATOR_Z, ID, strNew)==FALSE) return;

	m_stcNGMZElevatorZ[ID].SetWindowText(strNew);

	m_strLog.Format("[Setup - MoveTab Dialog] OnStcNGMZElevatorZ - Data(%d-%s)", ID, strNew);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMoveTab6Dlg::OnStcGDMZElevatorZ(UINT nID)
{
	int ID = nID - IDC_STC_GDMZ_ELEVATOR_Z_0;

	CString strOld, strNew;
	m_stcGDMZElevatorZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_GDMZ_ELEVATOR_Z) != IDOK) return;
	if (g_objCommon.Check_MoveData(AX_GDMZ_ELEVATOR_Z, ID, strNew)==FALSE) return;

	m_stcGDMZElevatorZ[ID].SetWindowText(strNew);

	m_strLog.Format("[Setup - MoveTab Dialog] OnStcGDMZElevatorZ - Data(%d-%s)", ID, strNew);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMoveTab6Dlg::OnStcULCVElevator(UINT nID)
{
	int ID = nID - IDC_STC_ULCV_ELEVATOR_Z_0;

	CString strOld, strNew;
	m_stcULCVElevatorZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_ULCV_ELEVATOR_Z) != IDOK) return;
	if (g_objCommon.Check_MoveData(AX_ULCV_ELEVATOR_Z, ID, strNew)==FALSE) return;

	m_stcULCVElevatorZ[ID].SetWindowText(strNew);

	m_strLog.Format("[Setup - MoveTab Dialog] OnStcULCVElevator - Data(%d-%s)", ID, strNew);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMoveTab6Dlg::OnStcMZTransferX(UINT nID)
{
	int ID = nID - IDC_STC_MZ_TRANSFER_X_0;

	CString strOld, strNew;
	m_stcMZTransferX[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_MZ_TRANSFER_X) != IDOK) return;
	if (g_objCommon.Check_MoveData(AX_MZ_TRANSFER_X, ID+1, strNew)==FALSE) return;

	m_stcMZTransferX[ID].SetWindowText(strNew);

	m_strLog.Format("[Setup - MoveTab Dialog] OnStcMZTransferX - Data(%d-%s)", ID, strNew);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CSetupMoveTab6Dlg::OnStcMZTransferZ(UINT nID)
{
	int ID = nID - IDC_STC_MZ_TRANSFER_Z_0;

	CString strOld, strNew;
	m_stcMZTransferZ[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew, AX_MZ_TRANSFER_Z) != IDOK) return;
	if (g_objCommon.Check_MoveData(AX_MZ_TRANSFER_Z, ID, strNew)==FALSE) return;

	m_stcMZTransferZ[ID].SetWindowText(strNew);

	m_strLog.Format("[Setup - MoveTab Dialog] OnStcMZTransferZ - Data(%d-%s)", ID, strNew);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupMoveTab6Dlg::Initial_Controls() 
{
	for (int i = 0; i < 10; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 37; i++) m_Label[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));

	for (int i = 0; i < 3; i++)m_stcLoadFeederY[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 3; i++)m_stcNGFeederY[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));
	for (int i = 0; i < 3; i++)m_stcGoodFeederY[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xA0, 0xF0, 0xC0));

	for (int i = 0; i < 3; i++) m_stcLDCVElevatorZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));	
	for (int i = 0; i < 3; i++) m_stcLDMZElevatorZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 3; i++) m_stcNGMZElevatorZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 3; i++) m_stcGDMZElevatorZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 3; i++) m_stcULCVElevatorZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	
	for (int i = 0; i < 6; i++) m_stcMZTransferX[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
	for (int i = 0; i < 7; i++) m_stcMZTransferZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xEF, 0xD5));
}

void CSetupMoveTab6Dlg::Display_MoveData()
{
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();
	CString strData;

	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dLDFeederY[i]);		m_stcLoadFeederY[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dNGFeederY[i]);		m_stcNGFeederY[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dGDFeederY[i]);		m_stcGoodFeederY[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dLDCVElevatorZ[i]);	m_stcLDCVElevatorZ[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dLDMZElevatorZ[i]);	m_stcLDMZElevatorZ[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dNGMZElevatorZ[i]);	m_stcNGMZElevatorZ[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dGDMZElevatorZ[i]);	m_stcGDMZElevatorZ[i].SetWindowText(strData); }
	for (int i = 0; i < 3; i++) { strData.Format("%0.3lf", pMoveData->dULCVElevatorZ[i]);	m_stcULCVElevatorZ[i].SetWindowText(strData); }
	for (int i = 0; i < 6; i++) { strData.Format("%0.3lf", pMoveData->dMZTransferX[i+1]);	m_stcMZTransferX[i].SetWindowText(strData); }
	for (int i = 0; i < 7; i++) { strData.Format("%0.3lf", pMoveData->dMZTransferZ[i]);		m_stcMZTransferZ[i].SetWindowText(strData); }
}

void CSetupMoveTab6Dlg::Save_MoveData()
{
	CIniFileCS INI(gsCurrentDir + "\\System\\MoveData.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("MoveData.ini File Not Found!!!");
		return;
	}

	g_objCommon.Backup_File(gsCurrentDir + "\\System", "MoveData");

	CString strKey, strData;
	double dData;

	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcLoadFeederY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("45_LOAD_FEEDER_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcNGFeederY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("46_NG_FEEDER_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcGoodFeederY[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("47_GOOD_FEEDER_Y", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcLDCVElevatorZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("48_LDCV_ELEVATOR_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcLDMZElevatorZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("49_LDMZ_ELEVATOR_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcNGMZElevatorZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("50_NGMZ_ELEVATOR_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcGDMZElevatorZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("51_GDMZ_ELEVATOR_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 3; i++) { strKey.Format("%02d", i);	m_stcULCVElevatorZ[i].GetWindowText(strData);	dData = atof(strData);	INI.Set_Double("52_ULCV_ELEVATOR_Z", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 6; i++) { strKey.Format("%02d", i+1);	m_stcMZTransferX[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("53_MZ_TRANSFER_X", strKey, dData, "%0.3lf"); }
	for (int i = 0; i < 7; i++) { strKey.Format("%02d", i);	m_stcMZTransferZ[i].GetWindowText(strData);		dData = atof(strData);	INI.Set_Double("54_MZ_TRANSFER_Z", strKey, dData, "%0.3lf"); }

	g_objLogFile.Save_HandlerLog("[Setup - MoveTab Dialog 6] save");

	Cancel_MoveData();
}

void CSetupMoveTab6Dlg::Cancel_MoveData()
{
	g_objDataManager.Read_MoveData();
	Display_MoveData();
}

///////////////////////////////////////////////////////////////////////////////
