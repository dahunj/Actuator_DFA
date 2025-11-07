// WorkDlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMAI2100.h"
#include "WorkDlg.h"
#include "afxdialogex.h"
#include "LogFile.h"
#include "Common.h"
#include "Inspector.h"
#include "BarcodeLot_RS232.h"
#include "BarcodeLot_Cognex.h"
#include "CarrierRFID_Load.h"
#include "CarrierRFID_Good.h"
#include "CarrierRFID_NG.h"
#include "IdleReportDlg.h"
#include "MesAgent.h"
#include "Dispatcher.h"
#include "SequenceInit.h"
#include "SequenceMain.h"
#include "OperatorDlg.h"
#include "CMAI2100Dlg.h"
#include "NoWorkDlg.h"

// CWorkDlg 대화 상자입니다.
CWorkDlg g_dlgWork;

IMPLEMENT_DYNAMIC(CWorkDlg, CDialogEx)

CWorkDlg::CWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWorkDlg::IDD, pParent)
{
}

CWorkDlg::~CWorkDlg()
{
}

void CWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 9; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i <28; i++) DDX_Control(pDX, IDC_RDO_SELECT_NO_0 + i, m_rdoSlectNo[i]);
	for (int i = 0; i <28; i++) DDX_Control(pDX, IDC_STC_LOTS_ID_S_0 + i, m_stcLotsIdS[i]);
	for (int i = 0; i <28; i++) DDX_Control(pDX, IDC_STC_CMS_COUNT_S_0 + i, m_stcCmsCountS[i]);
	DDX_Control(pDX, IDC_IMG_EQUIPMENT, m_imgEquipment);
	for (int i = 0; i < 9; i++) DDX_Control(pDX, IDC_LED_EQUIP_OPTION_0 + i, m_ledEquipOption[i]);
	for (int i = 0; i < 6; i++) DDX_Control(pDX, IDC_LED_VISION_STATUS_0 + i, m_ledVisionSts[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_LED_BARCODE_STATUS_0 + i, m_ledBarcodeSts[i]);
	DDX_Control(pDX, IDC_STC_EQUIP_TYPE, m_stcEquipType);
	for (int i = 0; i < 17; i++) DDX_Control(pDX, IDC_STC_TRAY_NO_0 + i, m_stcJobLotID[i]);
	for (int i = 0; i < 17; i++) DDX_Control(pDX, IDC_STC_LINE_NO_0 + i, m_stcJobCarNo[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_STC_MZ_LOT_ID_0 + i, m_stcMZLotID[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_STC_MZ_NO_0 + i, m_stcMZCarNo[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_CV_MZ_CHK_0 + i, m_stcCVMZCheck[i]);
	DDX_Control(pDX, IDC_RDO_WORK_START, m_rdoWorkStart);
	DDX_Control(pDX, IDC_RDO_WORK_STOP, m_rdoWorkStop);
	DDX_Control(pDX, IDC_LED_INIT_COMPLETE, m_ledInitComplete);
	DDX_Control(pDX, IDC_CHK_CYCLE_STOP, m_chkCycleStop);
	for (int i = 0; i < 5; i++) DDX_Control(pDX, IDC_PIC_TRAY_BACK_0 + i, m_picTrayBack[i]);
	DDX_Control(pDX, IDC_STC_ALIGN_TRAY_COUNT, m_stcAlignLine);
	DDX_Control(pDX, IDC_STC_NG_TRAY_COUNT, m_stcCarrierLine);
	DDX_Control(pDX, IDC_STC_CAP_TRAY_COUNT, m_stcNGLine);
	DDX_Control(pDX, IDC_STC_SHIP_TRAY_COUNT, m_stcGoodLine);
	DDX_Control(pDX, IDC_GRD_ALIGN_TRAY, m_grdAlign);
	DDX_Control(pDX, IDC_GRD_NG_TRAY, m_grdCarrier);
	DDX_Control(pDX, IDC_GRD_CAP_TRAY, m_grdNGTray);
	DDX_Control(pDX, IDC_GRD_SHIP_TRAY, m_grdGoodTray);
	DDX_Control(pDX, IDC_GRD_JOB_LIST, m_grdJob);
	for (int i = 0; i <33; i++) DDX_Control(pDX, IDC_STC_WORK_CASE_0 + i, m_stcWorkCase[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_ELEVATOR_INFOR_0 + i, m_stcStageInfor[i]);
	for (int i = 0; i <15; i++) DDX_Control(pDX, IDC_LED_TRAY_CHECK_0 + i, m_ledTrayCheck[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_NGMZ_CAR_0 + i, m_stcNGMZCar[i]);

	DDX_Control(pDX, IDC_LBL_OPER_ID, m_lblOperId);
	DDX_Control(pDX, IDC_STC_OPER_ID, m_stcOperId);
	DDX_Control(pDX, IDC_STC_MES_CONNECT, m_stcMesConnect);
	DDX_Control(pDX, IDC_STC_MES_ONLINE, m_stcMesOnline);
}

BEGIN_MESSAGE_MAP(CWorkDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_LOTS_ID_S_0, IDC_STC_LOTS_ID_S_27, OnStcLotsIdSClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_CMS_COUNT_S_0, IDC_STC_CMS_COUNT_S_27, OnStcCmsCountSClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_RDO_SELECT_NO_0, IDC_RDO_SELECT_NO_27, OnBnClickedRdoSlectNo)
	ON_BN_CLICKED(IDC_RDO_WORK_START, &CWorkDlg::OnBnClickedRdoWorkStart)
	ON_BN_CLICKED(IDC_RDO_WORK_STOP, &CWorkDlg::OnBnClickedRdoWorkStop)
	ON_BN_CLICKED(IDC_CHK_CYCLE_STOP, &CWorkDlg::OnBnClickedChkCycleStop)
	ON_MESSAGE(UM_UPDATE_MODEL, &CWorkDlg::OnUpdateModel)
	ON_MESSAGE(UM_UPDATE_TRAY_INFO, &CWorkDlg::OnUpdateTrayInfo)
	ON_MESSAGE(UM_UPDATE_BARCODE_RS232, &CWorkDlg::OnUpdateBarcode_RS232)
	ON_MESSAGE(UM_UPDATE_BARCODE_COGNEX, &CWorkDlg::OnUpdateBarcode_Cognex)
	ON_MESSAGE(UM_RESET_CYCLE_STOP, &CWorkDlg::OnResetCycleStop)
	ON_MESSAGE(UM_UPDATE_UPH, &CWorkDlg::OnUpdateUph)
	ON_MESSAGE(UM_JOB_COMPELTE, &CWorkDlg::OnJobComplete)
	ON_MESSAGE(UM_LOT_END_MSG, &CWorkDlg::OnShowLotEndMsg)
	ON_BN_CLICKED(IDC_BUTTON1, &CWorkDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CWorkDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BTN_BUZZER_OFF, &CWorkDlg::OnBnClickedBtnBuzzerOff)
	ON_BN_CLICKED(IDC_BTN_LOT_CANCEL, &CWorkDlg::OnBnClickedBtnLotCancel)

	ON_BN_CLICKED(IDC_BTN_MES_CONNECT, &CWorkDlg::OnBnClickedBtnMesConnect)
	ON_BN_CLICKED(IDC_BTN_MES_DISCONNECT, &CWorkDlg::OnBnClickedBtnMesDisconnect)
	ON_BN_CLICKED(IDC_BTN_MES_ONLINE, &CWorkDlg::OnBnClickedBtnMesOnline)
	ON_BN_CLICKED(IDC_BTN_MES_OFFLINE, &CWorkDlg::OnBnClickedBtnMesOffline)
	ON_BN_CLICKED(IDC_BTN_MES_ABORT, &CWorkDlg::OnBnClickedBtnMesAbort)
	ON_BN_CLICKED(IDC_BTN_IDLE_REPORT, &CWorkDlg::OnBnClickedBtnIdleReport)
	ON_STN_CLICKED(IDC_STC_OPER_ID, &CWorkDlg::OnStnClickedOperId)
	ON_STN_CLICKED(IDC_STC_EQUIP_TYPE, &CWorkDlg::OnStnClickedCarrier)
	ON_STN_CLICKED(IDC_STC_MZ_LOT_ID_6, &CWorkDlg::OnStnClickedStcMzLotId6)
END_MESSAGE_MAP()

// CWorkDlg 메시지 처리기입니다.

BOOL CWorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_bAutoRunning = FALSE;
	m_bShowWindow = TRUE;

	ReadJobList();

#ifdef RESULT_TEST
	gData.bResultTest = TRUE;
#else
	gData.bResultTest = FALSE;
#endif

	m_rdoWorkStop.SetCheck(TRUE);
	m_rdoWorkStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CWorkDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CWorkDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	KillTimer(0);

	SaveJobList();
}

void CWorkDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

		m_bmpEquipment.DeleteObject();
		if		(gData.bUseDryRun)			m_bmpEquipment.LoadBitmap(IDB_EQUIP_DOOR);
		else if (!pEquipData->bUseDoorLock) m_bmpEquipment.LoadBitmap(IDB_EQUIP_DOOR_KOR);
		else								m_bmpEquipment.LoadBitmap(IDB_EQUIP_WORK);
		m_imgEquipment.SetBitmap(m_bmpEquipment);

		if (pEquipData->bUseROS) m_stcMZCarNo[6].ShowWindow(TRUE);
		else					 m_stcMZCarNo[6].ShowWindow(FALSE);

 		m_ledEquipOption[0].Set_On(pEquipData->bUseBottom);
		m_ledEquipOption[1].Set_On(pEquipData->bUseTop1);
		m_ledEquipOption[2].Set_On(pEquipData->bUseTop2);
		m_ledEquipOption[3].Set_On(pEquipData->bUseROS);
		m_ledEquipOption[4].Set_On(pEquipData->bUseAlign1);
//		m_ledEquipOption[5].Set_On(pEquipData->bUseAlign2);
		m_ledEquipOption[5].Set_On(pEquipData->bUseCosmeticNG);
		m_ledEquipOption[6].Set_On(pEquipData->bUseMES);
		m_ledEquipOption[7].Set_On(pEquipData->bUseBotAng);
		m_ledEquipOption[8].Set_On(pEquipData->bUseTopAng);
		g_objCommon.Locking_MainDoor(TRUE);

		m_bShowWindow = TRUE;
//		OnBnClickedRdoSlectNo(gData.nSelectNo);
		m_bShowWindow = FALSE; 

		m_tWorkElevatorLoop1.Set_LoopTime(1000);
		m_tWorkElevatorLoop2.Set_LoopTime(1000);	
		m_tWorkElevatorLoop3.Set_LoopTime(1000);	
		m_tWorkElevatorLoop4.Set_LoopTime(1000);	
		m_tWorkElevatorLoop5.Set_LoopTime(1000);	
		m_tWorkElevatorLoop6.Set_LoopTime(1000);
		m_tWorkElevatorLoop7.Set_LoopTime(1000);
//		Begin_ElevatorRunThread();
		m_stcOperId.SetWindowText(gData.sOperID);

		SetTimer(0, 100, NULL);
	} else {
		End_ElevatorRunThread();
		KillTimer(0);
	}
}

void CWorkDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);

	CCMAI2100Dlg *pMainDlg = (CCMAI2100Dlg*)AfxGetMainWnd();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	DX_DATA_13 *pDX13 = g_objAJinAXL.Get_pDX13();
	if (pDX13->iStartSw && !m_rdoWorkStart.GetCheck()) {
		g_objLogFile.Save_HandlerLog("[Work Mode] START S/W push");
		m_rdoWorkStart.SetCheck(TRUE);
	} else if (pDX13->iStopSw && !m_rdoWorkStop.GetCheck()) {
		g_objLogFile.Save_HandlerLog("[Work Mode] STOP S/W push");
		m_rdoWorkStop.SetCheck(TRUE);
		pMainDlg->Set_MainState(STATE_INITEND);
	}

	DX_DATA_20 *pDX20 = g_objAJinAXL.Get_pDX20();
	if (pDX20->iLDMZOpenSw) ConveyorOpen(1, 1);
	if (pDX20->iLDMZRunSw)  ConveyorOpen(1, 2);
	if (pDX20->iULMZOpenSw) ConveyorOpen(2, 1);
	if (pDX20->iULMZRunSw)  ConveyorOpen(2, 2);

	Display_Status();

	if (m_rdoWorkStart.GetCheck()) {
		if (!m_bAutoRunning) {		// First AutoRun
			if (!Work_Start()) { SetTimer(0, 100, NULL); m_rdoWorkStop.SetCheck(TRUE); return; }

			if (g_objSequenceInit.Get_InitComplete()) {
				m_bAutoRunning = TRUE;
				g_objCommon.Locking_MainDoor(TRUE, TRUE);
				pMainDlg->Enable_ModeButton(FALSE);
				if (gAlm.bBegin) Reset_AlarmLog();
				else {
					if (gLot.nRunPortNo > 0) gLot.dwStopTime[gLot.nRunPortNo-1] += (GetTickCount() - dwStopSTime);
				}
				pMainDlg->Set_MainState(STATE_RUN);

				g_objInspector.Set_StatusUpdate(INSPECTOR_ALL, 2);
				g_objDispatcher.Set_StatusUpdate(1);	// 0:Stop, 1:Run, 2:Error
				g_objSequenceMain.Begin_MainRunThread();

				pMainDlg->Set_EquipRunStart();
				g_objMesAgent.Set_EquipState(5);	//Run

			} else {
				g_objCommon.Show_Error(40);		// 초기화 완료 에러
			}

		} else {				// Auto Running
			if (!g_objSequenceMain.Is_MainThreadRun()) {
				g_objLogFile.Save_HandlerLog("[Work Mode] Auto STOP");
				pMainDlg->Set_MainState(STATE_INITEND);
			}
		}

	} else if (m_rdoWorkStop.GetCheck()) {
		if (m_bAutoRunning) {	// First AutoStop
			m_bAutoRunning = FALSE;

			g_objSequenceMain.End_MainRunThread();
			g_objCommon.Stop_Conveyor();

			m_rdoWorkStart.Set_Color(RGB(0x00, 0x00, 0x00), COLOR_DEFAULT);
			m_rdoWorkStop.Set_Color(RGB(0xFF, 0x00, 0x00), COLOR_DEFAULT);

			g_objDispatcher.Set_StatusUpdate(0);	// 0:Stop, 1:Run, 2:Error

			pMainDlg->Enable_ModeButton(TRUE);
			g_objCommon.Locking_MainDoor(FALSE);

			pMainDlg->Save_EquipRunTime();
			g_objCommon.Save_MotionPos();

		} else {				// Stop
			int nState = pMainDlg->Get_MainState();
			if (nState != STATE_ALARM && nState != STATE_EMER) g_objCommon.Check_MainEmgAir();
		}
	}

	SetTimer(0, 100, NULL);
	CDialogEx::OnTimer(nIDEvent);
}

void CWorkDlg::OnStcLotsIdSClick(UINT nID)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (pEquipData->bUseMES) {
		AfxMessageBox(_T("MES사용시 Lot정보 수정할 수 없습니다."));
		return;
	}

	int ID = nID - IDC_STC_LOTS_ID_S_0;

	if (m_rdoWorkStart.GetCheck()) {
		if (gData.nLanguage == 0) g_objCommon.Show_MsgBox(1, "장비 Stop 상태에서 진행이 가능합니다.....");
		else					  g_objCommon.Show_MsgBox(1, "You can proceed with the equipment stopped.");
		return;
	}

	CString strKey, strNew, strMsg;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;
	if (strKey.Find("_") >= 0) {
		if (gData.nLanguage == 0) strMsg.Format("[%s] Lot ID ( _ ) 입력불가...", strKey);
		else					  strMsg.Format("[%s] Lot ID Unable to endter(_)...", strKey);
		g_objCommon.Show_MsgBox(1, strMsg);
		return;
	}

	m_stcLotsIdS[ID].SetWindowText(strKey);

	strNew.Format("[Work Mode] Lot ID Input(%d-%s)", ID, strKey);
	g_objLogFile.Save_HandlerLog(strNew);

	OnStcCmsCountSClick(IDC_STC_CMS_COUNT_S_0+ID);	//2018.9.11+
}

void CWorkDlg::OnStcCmsCountSClick(UINT nID)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (pEquipData->bUseMES) {
		AfxMessageBox(_T("MES사용시 Lot정보 수정할 수 없습니다."));
		return;
	}

	int ID = nID - IDC_STC_CMS_COUNT_S_0;

	if (m_rdoWorkStart.GetCheck()) {
		if (gData.nLanguage == 0) g_objCommon.Show_MsgBox(1, "장비 Stop 상태에서 진행이 가능합니다.....");
		else					  g_objCommon.Show_MsgBox(1, "You can proceed with the equipment stopped.");
		return;
	}

	CString strOld, strNew, strValue;

	m_stcCmsCountS[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	int nCmCnt = atoi(strNew);
	if (nCmCnt < 1 || nCmCnt > TRAY_MAX_CM) {
		m_stcCmsCountS[ID].SetWindowText("");
		if (gData.nLanguage == 0) AfxMessageBox(_T("Lot당 CM수량은 40개이상 입력할수 없습니다.........."));
		else					  AfxMessageBox(_T("CM quantity per lot cannot be entered more than 40."));
		return;
	}
	strValue.Format("%d", nCmCnt);
	m_stcCmsCountS[ID].SetWindowText(strValue);

	if (LotID_Check()==FALSE) return;

	strNew.Format("[Work Mode] Module Count Input(%d-%d-%d)", ID, nCmCnt);
	g_objLogFile.Save_HandlerLog(strNew);
}

void CWorkDlg::OnBnClickedRdoSlectNo(UINT nID)
{
	CString strTemp, strLog;
	int ID = nID - IDC_RDO_SELECT_NO_0;
	if (nID > 0 && nID < 30) ID = nID - 1;

	if (m_rdoWorkStart.GetCheck()) {
		if (gData.nLanguage == 0) AfxMessageBox(_T("장비 Stop 상태에서 진행이 가능합니다....."));
		else					  AfxMessageBox(_T("You can proceed with the equipment stopped."));
		m_rdoSlectNo[ID].SetCheck(FALSE);
		return;
	}

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (pEquipData->bUseMES) {
		AfxMessageBox(_T("MES사용시 Lot정보 수정할 수 없습니다."));
		m_rdoSlectNo[ID].SetCheck(FALSE);
		return;
	}

	static int cnt[28] = {0, };
	static int time = GetTickCount();
	
	int nSno = 0, nSno1 = 0, nSno2 = 0;
	gData.nSelectNo = nSno = ID + 1;
	for (int i=0; i<28; i++) {
		if (i == ID) {
			m_rdoSlectNo[i].SetCheck(TRUE);
			if(m_bShowWindow==FALSE){
				if(GetTickCount()-time > 500){
					cnt[i] = 0;
					time = GetTickCount();
				}

				if(cnt[i] == 1){ /*
					if (g_objSequenceMain.Get_IsAutoRun()) {
						AfxMessageBox(_T("진행중인 Lot 완료후 다음 Lot 진행이 가능합니다."));
						m_rdoSlectNo[i].SetCheck(FALSE);
						m_bShowWindow = FALSE;
						return;
					}
					*/
					nSno1++;
					if (g_objCommon.Show_MsgBox(2, "Lot Data를 삭제 하시겠습니까?") != IDOK) return;

					m_stcLotsIdS[i].GetWindowText(strTemp);
					strLog.Format("[Work Mode] OnBnClickedRdoSlectNo Click.. LotID[%s-%s] Port[%d] Count[%d]", strTemp, gLot.sLotID[i], i, gLot.nCmCount[i]);

					m_stcLotsIdS[i].SetWindowText("");
					m_stcCmsCountS[i].SetWindowText("");

					//2018.11.13+ 자동모드 상태에서 LotID를 지우면서 밑의 변수들을 초기화 시켜주지 않아 투입 되면 안되는 Lot가 투입 된다.
					g_objCommon.Set_LotDataClear(i);
					/////////////////////////
					cnt[i] = 0;
					time = GetTickCount();

					g_objLogFile.Save_HandlerLog(strLog);
				} else {
					cnt[i]++;
					time = GetTickCount();
					nSno2++;
				}
			}

		} else m_rdoSlectNo[i].SetCheck(FALSE);
	}

	if (nSno1 == 0 && nSno2 > 0) {
		CString sNo, sLotID[2], sCnt;
		m_stcLotsIdS[0].GetWindowText(sLotID[0]);
		m_stcLotsIdS[1].GetWindowText(sLotID[1]);
		m_stcCmsCountS[0].GetWindowText(sCnt);
		if (nSno > 1 && sLotID[0].GetLength() > 0 && sLotID[1].GetLength() < 1) {
			for (int i=0; i<nSno; i++) {
				sNo.Format("%s-%03d", sLotID[0], i+1);
				m_stcLotsIdS[i].SetWindowText(sNo);
				if (sCnt.GetLength() > 0) m_stcCmsCountS[i].SetWindowText(sCnt);
				else					  m_stcCmsCountS[i].SetWindowText("40");
			}
		} else {
			SYSTEMTIME time;
			GetLocalTime(&time);

			for (int i=0; i<nSno; i++) {
				m_stcLotsIdS[i].GetWindowText(sLotID[0]);
				if (sLotID[0].GetLength() < 1) {
					sNo.Format("LOT%02d-%02d%02d%02d", i+1, time.wHour, time.wMinute, time.wSecond);
					m_stcLotsIdS[i].SetWindowText(sNo);
					m_stcCmsCountS[i].SetWindowText("40");
				}
			}
		}
		for (int i=nSno; i<28; i++) {
			m_stcLotsIdS[i].SetWindowText("");
			m_stcCmsCountS[i].SetWindowText("");
		}
		LotID_Check();
	}
}

void CWorkDlg::OnBnClickedRdoWorkStart()
{
	g_objLogFile.Save_HandlerLog("[Work Mode] START button push");
}

void CWorkDlg::OnBnClickedRdoWorkStop()
{
	CCMAI2100Dlg *pMainDlg = (CCMAI2100Dlg*)AfxGetMainWnd();
	pMainDlg->Set_MainState(STATE_INITEND);
	dwStopSTime = GetTickCount();
	g_objMesAgent.Set_EquipState(6);	//Pause

	g_objLogFile.Save_HandlerLog("[Work Mode] STOP button push");
}

void CWorkDlg::OnBnClickedChkCycleStop()
{
	if (m_rdoWorkStart.GetCheck()) {
		if (gData.nLanguage == 0) AfxMessageBox(_T("장비 Stop 상태에서 진행이 가능합니다....."));
		else					  AfxMessageBox(_T("You can proceed with the equipment stopped."));
		m_chkCycleStop.SetCheck(FALSE);
		return;
	}

	if (m_chkCycleStop.GetCheck()) {
		m_chkCycleStop.Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0xFF));
		if (g_objCommon.Show_MsgBox(2, "Are you sure you want to Cycle Stop?") == IDOK) {
			gData.bCycleStop = TRUE;
			g_objLogFile.Save_HandlerLog("[Work Mode] OnBnClickedChkCycleStop = TRUE");
		} else {
			m_chkCycleStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xF0));
			m_chkCycleStop.SetCheck(FALSE);
		}
	} else {
		m_chkCycleStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xF0));
		gData.bCycleStop = FALSE;
		g_objLogFile.Save_HandlerLog("[Work Mode] OnBnClickedChkCycleStop = FALSE");
	}
}

void CWorkDlg::OnStnClickedStcMzLotId6()
{
	CString strKey, sLog;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	if (strKey.GetLength() < 4) {
		if (gData.nLanguage == 0) AfxMessageBox(_T("MZ ID가 잘못되었습니다..."));
		else					  AfxMessageBox(_T("MZ ID Error......"));
		return;
	}
	
	gData.sMZID[5] = strKey;
	CIniFileCS INI_EQP(gsCurrentDir + "\\System\\EquipData.ini");
	if (INI_EQP.Check_File()) {
		INI_EQP.Set_String("DAY_TOTAL", "BUFFER_MZID", gData.sMZID[5]);
	}

	sLog.Format("[Work Dialog] Buffer MZ ID Click. [%s]", gData.sMZID[5]);
	g_objLogFile.Save_HandlerLog(sLog);
}

void CWorkDlg::OnStnClickedCarrier()
{
	CString strKey, sLog, strText, strOld, strNew;

	strOld.Format("%d", gData.nCarrierCount);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	gData.nCarrierCount = atoi(strNew);

	strText.Format("Carrier 수량:  %d", gData.nCarrierCount);
	m_stcEquipType.SetWindowText(strText);

	CIniFileCS INI_EQP(gsCurrentDir + "\\System\\EquipData.ini");
	if (INI_EQP.Check_File()) {
		INI_EQP.Set_Integer("DAY_TOTAL", "CARRIER_COUNT", gData.nCarrierCount);
	}

	sLog.Format("[Work Dialog] Carrier Click. [%d]", gData.nCarrierCount);
	g_objLogFile.Save_HandlerLog(sLog);
}

//------------------MES------------------------------------------------------------//
void CWorkDlg::OnStnClickedOperId()
{
	CString strKey, sLog;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	m_stcOperId.SetWindowText(strKey);
	gData.sOperID = strKey;
	g_objMesAgent.Set_OperUpdate(gData.sOperID);
	g_dlgOperator.m_stcOperOperId.SetWindowText(strKey);

	sLog.Format("[Work Dialog] Oper ID Button Click. [%s]", gData.sOperID);
	g_objLogFile.Save_HandlerLog(sLog);
}

void CWorkDlg::OnBnClickedBtnMesConnect()
{
	if (gData.sOperID.GetLength() < 4) { AfxMessageBox("Input the Operator ID....."); return; }

	g_objMesAgent.Initialize();
	g_objLogFile.Save_HandlerLog("[Work Dialog] MES Connect Button Click.");
}

void CWorkDlg::OnBnClickedBtnMesDisconnect()
{
	if (gData.sOperID.GetLength() < 4) { AfxMessageBox("Input the Operator ID....."); return; }

	g_objMesAgent.Terminate();
	g_objLogFile.Save_HandlerLog("[Work Dialog] MES Disconnect Button Click.");
}

void CWorkDlg::OnBnClickedBtnMesOnline()
{
	if (gData.sOperID.GetLength() < 4) { AfxMessageBox("Input the Operator ID....."); return; }

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseMES) return;

	g_objMesAgent.Set_ControlState(1, gData.sOperID);

	g_objLogFile.Save_HandlerLog("[Work Dialog] MES Online Button Click.");
}

void CWorkDlg::OnBnClickedBtnMesOffline()
{
	if (gData.sOperID.GetLength() < 4) { AfxMessageBox("Input the Operator ID....."); return; }

	g_objMesAgent.Set_ControlState(2, gData.sOperID);

	g_objLogFile.Save_HandlerLog("[Work Dialog] MES Offline Button Click.");
}

void CWorkDlg::OnBnClickedBtnMesAbort()
{
	if (gData.sOperID.GetLength() < 4) { AfxMessageBox("Input the Operator ID....."); return; }

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseMES) return;

	if (!g_objMesAgent.Is_Connected()) { AfxMessageBox("MES Disconnect 상태에서는 처리를 할수 없습니다."); return; }
	if (!g_objMesAgent.Is_HostOnline()) { AfxMessageBox("MES Offline 상태에서는 처리를 할수 없습니다."); return; }
	if (gData.nSelectNo < 1 || gData.nSelectNo > 6) { AfxMessageBox("Abort Lot을 먼저 선택해 주세요."); return; }
	if (!m_rdoWorkStop.GetCheck()) { AfxMessageBox("장비 Stop상태에서 Abort처리 하세요."); return; }
	if (gMes.nLotStatus[gData.nSelectNo-1] == 0) { AfxMessageBox("진행중인 Lot만 Abort처리가 가능합니다."); return; }

	int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_STAGE_1);
	int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_STAGE_2);
	if ((nCase1 > 5 && nCase1 < 8) || (nCase2 > 5 && nCase2 < 8)) {

		CString sData;
		sData.Format("Are you want to cancel this Port[%d] Lot[%s]?", gData.nSelectNo, gLot.sLotID[gData.nSelectNo-1]);
		if (g_objCommon.Show_MsgBox(2, sData) != IDOK) return;

		gMes.nLotStatus[gData.nSelectNo-1] = 0;
		g_objMesAgent.Set_LotAbort(gLot.sLotID[gData.nSelectNo-1]);

	//	int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_TRANSFER_1);
	//	if (nCase1 == 7) g_objSequenceMain.Set_MainRunCase(AUTO_TRANSFER_1, 0);
		if (nCase1 > 5 && nCase1 < 8) g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_STAGE_1, 20);
		if (nCase2 > 5 && nCase2 < 8) g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_STAGE_2, 20);

		m_stcLotsIdS[gData.nSelectNo-1].SetWindowText("");
		m_stcCmsCountS[gData.nSelectNo-1].SetWindowText("");
		g_objCommon.Set_LotDataClear(gData.nSelectNo-1);

		sData.Format("[Work Dialog] MES Abort Button Click. PortNo[%d] LotID[%s]", gData.nSelectNo, gLot.sLotID[gData.nSelectNo-1]);
		g_objLogFile.Save_HandlerLog(sData);
	} else {
		AfxMessageBox("진행중인 Lot만 Abort처리가 가능합니다.");
	}
}

void CWorkDlg::OnBnClickedBtnIdleReport()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseMES) return;

	if (m_rdoWorkStart.GetCheck()) {
		if (gData.nLanguage == 0) AfxMessageBox(_T("장비 Stop 상태에서 진행이 가능합니다....."));
		else					  AfxMessageBox(_T("You can proceed with the equipment stopped."));
		return;
	}

	if (g_dlgNoWork.IsWindowVisible()) g_dlgNoWork.ShowWindow(SW_HIDE);
	else g_dlgNoWork.ShowWindow(SW_SHOW);	
}
//------------------MES------------------------------------------------------------//

///////////////////////////////////////////////////////////////////////////////
// User Functions

BOOL CWorkDlg::Work_Start()
{
	CString strTemp, strTemp2, sText, strMsg;

	g_objCommon.Locking_Slide(TRUE, 0);
	if (gData.bAlarmShow) {
		g_objCommon.Show_MsgBox(1, "Alaram 화면을 Close하고 Run 진행하세요.....");
		m_rdoWorkStop.SetCheck(TRUE);
		return FALSE;
	}
	if (gData.nMZDoorOpen[0] > 0) {
		sText.Format("Load Door Open 상태입니다... Door Run 상태에서 진행이 가능합니다.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}
	if (gData.nMZDoorOpen[1] > 0) {
		sText.Format("Unload Door Open 상태입니다... Door Run 상태에서 진행이 가능합니다.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (pEquipData->bUseDoorLock==FALSE) {
		sText.Format("Door lock 해제 상태입니다.  진행하시겠습니까?");
		if (g_objCommon.Show_MsgBox(2, sText) != IDOK){
			m_rdoWorkStop.SetCheck(TRUE);
			return FALSE;
		}
	}
	if(gData.bUseDryRun) {
		sText.Format("Dry Run으로 설정되어 있습니다.\nDry Run으로 START 하시겠습니까?");
		if (g_objCommon.Show_MsgBox(2, sText) != IDOK){
			m_rdoWorkStop.SetCheck(TRUE);
			return FALSE;
		}
	}
//	if (pEquipData->bUseBottom==FALSE || pEquipData->bUseTop1==FALSE || pEquipData->bUseTop2==FALSE || pEquipData->bUseBotAng==FALSE || pEquipData->bUseTopAng==FALSE) {
	if (pEquipData->bUseBottom==FALSE || pEquipData->bUseTop1==FALSE || pEquipData->bUseTop2==FALSE || pEquipData->bUseTopAng==FALSE) {
		sText.Format("Vision Inspection을 사용하지 않고 START 하시겠습니까?");
		if (g_objCommon.Show_MsgBox(2, sText) != IDOK){
			m_rdoWorkStop.SetCheck(TRUE);
			return FALSE;
		}
	}
	if (pEquipData->bUseROS==FALSE) {
		sText.Format("ROS를 사용하지 않고 START 하시겠습니까?");
		if (g_objCommon.Show_MsgBox(2, sText) != IDOK){
			m_rdoWorkStop.SetCheck(TRUE);
			return FALSE;
		}
	} else {
		ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
		if (pRosData->lstMdjIp.GetCount() < 1) {
			g_objCommon.Show_MsgBox(1, "이 설비와 연결되 MDJ(ROS)가 없습니다. 확인후 Run 진행하세요.....");
			m_rdoWorkStop.SetCheck(TRUE);
			return FALSE;
		}
	}
	if (gAlm.dMotionChkPos < 0.01) {
		sText.Format("Motion Interlock 해제 상태입니다.\n그대로 START 하시겠습니까?");
		if (g_objCommon.Show_MsgBox(2, sText) != IDOK){
			m_rdoWorkStop.SetCheck(TRUE);
			return FALSE;
		}
	}

	if (pEquipData->bUseMES || pEquipData->bUseMZIDLoad || pEquipData->bUseMZIDNG || pEquipData->bUseMZIDUnload) {
		if (g_objBarcodeLot_Cognex.Is_Connected()==FALSE) {
			sText.Format("MZID-Barcode 연결에 실패하였습니다. 확인후 진행해 주세요.");
			g_objCommon.Show_MsgBox(1, sText);
			m_rdoWorkStop.SetCheck(TRUE);
			return FALSE;
		}
	}
	if (pEquipData->bUseMES || pEquipData->bUseRFIDLoad) {
		if (g_objCarrierRFID_Load.Is_Connected()==FALSE) g_objCarrierRFID_Load.Initialize();
	}
	if (pEquipData->bUseMES || pEquipData->bUseRFIDNG) {
		if (g_objCarrierRFID_NG.Is_Connected()==FALSE) g_objCarrierRFID_NG.Initialize();
	}
	if (pEquipData->bUseMES || pEquipData->bUseRFIDGood) {
		if (g_objCarrierRFID_Good.Is_Connected()==FALSE) g_objCarrierRFID_Good.Initialize();
	}
	if (pEquipData->bUseMES) {
		if (!g_objMesAgent.Is_Connected() || !g_objMesAgent.Is_HostOnline()) {
			sText.Format("MES Run은 Mes Connect - Online 상태에서만 진행할 수 있습니다.");
			g_objCommon.Show_MsgBox(1, sText);
			m_rdoWorkStop.SetCheck(TRUE);
			return FALSE;
		}
	}
	if (gData.nCarrierCount < 1) {
		sText.Format("Buffer Elevator에 Carrier를 넣고 진행해 주세요. (수량확인)");
		g_objCommon.Show_MsgBox(1, sText);
		m_rdoWorkStop.SetCheck(TRUE);
		return FALSE;
	}

	CCMAI2100Dlg *pMainDlg = (CCMAI2100Dlg*)AfxGetMainWnd();
	int nState = gData.m_nMS;
	if (nState == STATE_ALARM || nState == STATE_EMER) {
		m_rdoWorkStop.SetCheck(TRUE);
		return FALSE;
	}

	if (!g_objCommon.Check_MainDoor()) {
		m_rdoWorkStop.SetCheck(TRUE);
		return FALSE;
	}
	if (!g_objCommon.Check_TraySlide(9)) {
		m_rdoWorkStop.SetCheck(TRUE);
		return FALSE;
	}
	if (!g_objCommon.Check_SlideLock()) {
		if (gData.nLanguage == 0) sText.Format("Slide Lock 하시고 진행해 주세요.");
		else					  sText.Format("Slide lock and proceed.");
		g_objCommon.Show_MsgBox(1, sText);
		m_rdoWorkStop.SetCheck(TRUE);
		return FALSE;
	}

	int nMotionNo = g_objCommon.Check_MotionPos();
	if (nMotionNo < 99) {
		double dCurrentPos = g_objAJinAXL.Get_Position(nMotionNo);
		CString strName = g_objAJinAXL.Get_AxisName(nMotionNo);
		if (gData.nLanguage == 0) sText.Format("Motion(%s) 위치를 Check 하세요.\n이전위치(%0.3lf) != 현재위치(%0.3lf)", strName, gAlm.dMotionPos[nMotionNo], dCurrentPos);
		else					  sText.Format("Motion(%s) Check Position. => Pre-Position(%0.3lf) != Current-Position(%0.3lf)", strName, gAlm.dMotionPos[nMotionNo], dCurrentPos);
		g_objLogFile.Save_HandlerLog(sText);

		g_objCommon.Show_MsgBox(1, sText);
		m_rdoWorkStop.SetCheck(TRUE);
		return FALSE;
	}
	if (g_objCommon.Check_StageUpDown(sText)==FALSE) {
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		m_rdoWorkStop.SetCheck(TRUE);
		return FALSE;
	}

	if (gData.sOperID.GetLength() < 1) {
		g_objCommon.Show_MsgBox(1, "Input Operator ID");
		return FALSE;
	}

#ifdef AJIN_BOARD_USE
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	if (!pDX00->iElevator2TrayExist) {
		sText.Format("Buffer Elevator에 Carrier를 넣어주세요.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}
#endif

	if (LotID_Check() == FALSE) return FALSE;
/*
	if (pEquipData->bUseMES && gMes.nLotPortNo > 0 && gMes.nLotPortNo < 30 && gMes.nLotStatus[gMes.nLotPortNo-1] == 1) {
		if (gLot.nCmCount[gMes.nLotPortNo-1] == gMes.nHostRcvCmCount) gMes.nLotStatus[gMes.nLotPortNo-1] = 2;
		else {
			sText.Format("MES Module 수량(%d)과 같아야 Lot 진행이 가능합니다.", gMes.nHostRcvCmCount);
			g_objLogFile.Save_HandlerLog(sText);
			g_objCommon.Show_MsgBox(1, sText);
			return FALSE;
		}
	}
*/
	// Job처음 Start시 Check
//	if (g_objSequenceMain.Get_IsAutoRun()) return TRUE;	// Auto Run이면 스킵
	if (gLot.nJobStatus > 0) return TRUE;	// Auto Run이면 스킵 gjc

	DX_DATA_16 *pDX16 = g_objAJinAXL.Get_pDX16();
	DX_DATA_17 *pDX17 = g_objAJinAXL.Get_pDX17();
	DX_DATA_18 *pDX18 = g_objAJinAXL.Get_pDX18();
	DX_DATA_19 *pDX19 = g_objAJinAXL.Get_pDX19();
	DX_DATA_20 *pDX20 = g_objAJinAXL.Get_pDX20();
#ifndef AJIN_BOARD_USE
	pDX18->iMZBufferExist = TRUE;
	pDX18->iGDMZElevatorExist = FALSE;
	gData.nSimCount = 0;
	pDX16->iLDCV2FCnt1 = TRUE;
	pDX16->iLDCV2FCnt2 = TRUE;
	pDX16->iLDCV2FCnt3 = TRUE;
	pDX16->iLDCV1FCnt1 = TRUE;
	pDX16->iLDCV1FCnt2 = TRUE;
	pDX16->iLDCV1FCnt3 = TRUE;

	pDX16->iLDCV1FCnt4 = TRUE;
	pDX16->iLDCV2FCnt4 = TRUE;
	pDX16->iLDCV1FCnt5 = TRUE;
	pDX16->iLDCV2FCnt5 = TRUE;

	pDX16->iLDCV1FCnt6 = TRUE;
	pDX16->iLDCV2FCnt6 = TRUE;
	pDX16->iLDCV1FCnt7 = TRUE;
	pDX16->iLDCV2FCnt7 = TRUE;

	pDX20->iMZTransExist = FALSE;
	g_objAJinAXL.Move_Absolute(AX_ELEVATOR_Z2, 290);
#endif

	if (!pDX18->iMZBufferExist) {
		sText.Format("Buffer에 Empty MZ을 넣어주세요.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}
	if (gData.sMZID[5].GetLength() < 4) {	//Buffer-MZID
		sText.Format("Buffer MZ-ID를 입력해 주세요.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}
	if (pDX18->iLDMZElevatorExist || pDX18->iNGMZElevatorExist || pDX18->iGDMZElevatorExist) {
		sText.Format("Load, NG, Good MZ을 제거해주세요.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}
	if (pDX17->iLDCVElevatorCVStart || pDX17->iLDCVElevatorCVStop) {
		sText.Format("Load Conveyor Elevator에 MZ을 제거해주세요.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}
	if (pDX17->iULCVElevatorMZExist || pDX17->iULCVElevatorStop) {
		sText.Format("Unload Conveyor Elevator에 MZ을 제거해주세요.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}
	if (pDX19->iULCV2FCnt1 || pDX19->iULCV2FCnt2 || pDX19->iULCV2FCnt3 || pDX19->iULCV2FCnt4 || pDX19->iULCV2FCnt5 || pDX19->iULCV2FCnt6) {
		sText.Format("Unload Upper(2F) Conveyor 에 MZ을 제거해주세요.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}
	if (pDX19->iULCV1FCnt1 || pDX19->iULCV1FCnt2 || pDX19->iULCV1FCnt3 || pDX19->iULCV1FCnt4 || pDX19->iULCV1FCnt5 || pDX19->iULCV1FCnt6) {
		sText.Format("Unload Lower(1F) Conveyor 에 MZ을 제거해주세요.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}
	if (!pDX16->iLDCV2FCnt1 && !pDX16->iLDCV2FCnt2 && !pDX16->iLDCV2FCnt3 && !pDX16->iLDCV2FCnt4 && !pDX16->iLDCV2FCnt5 && !pDX16->iLDCV2FCnt6) {
		sText.Format("Load Upper(2F) Conveyor 에 작업할 MZ을 넣어주세요.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}
	if (!pDX16->iLDCV1FCnt1 && !pDX16->iLDCV1FCnt2 && !pDX16->iLDCV1FCnt3 && !pDX16->iLDCV1FCnt4 && !pDX16->iLDCV1FCnt5 && !pDX16->iLDCV1FCnt6) {
		sText.Format("Load Lower(1F) Conveyor 에 작업할 MZ을 넣어주세요.");
		g_objLogFile.Save_HandlerLog(sText);
		g_objCommon.Show_MsgBox(1, sText);
		return FALSE;
	}

	for (int i=0; i<6; i++) {
		gLot.sRstLotID[i]   = "";
		gLot.nRstCmCount[i] = gLot.nRstGoodCount[i] = gLot.nRstNgCount[i] = 0;
		gLot.nUnlGdTrayCount[i] = gLot.nUnlNGTrayCount[i] = 0;
	}
	for (int i=0; i<6; i++) {
		if (gLot.nCmCount[i] > 0) { gData.sLotID_Start = gLot.sLotID[i]; break; }
	}

	if (pEquipData->bUseInspectBlow) {
		DY_DATA_04 *pDY04 = g_objAJinAXL.Get_pDY04();
		pDY04->oBTMIonizerOn = TRUE;
		pDY04->oBTMIonizerBlow = TRUE;
		pDY04->oBTMSuctionOn = TRUE;

		pDY04->oTOPIonizerOn = TRUE;
		pDY04->oTOPIonizerBlow = TRUE;
		pDY04->oTOPSuctionOn = TRUE;
		g_objAJinAXL.Write_Output(4);
	}
	g_objSequenceMain.Set_MainRunCase(AUTO_ELEVATOR_2, 51);
	gData.nLoadStageSeqNo[0] = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_STAGE_1);
	gData.nLoadStageSeqNo[1] = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_STAGE_2);
	gData.nLoadStageSeqNo[2] = g_objSequenceMain.Get_MainRunCase(AUTO_TRANSFER_1);
	if (gData.nLoadStageSeqNo[2] == 0) {
		if (gData.nLoadStageSeqNo[0] == 0 || gData.nLoadStageSeqNo[0] == 50) {
			if (gData.nLoadStageSeqNo[1] == 0 || gData.nLoadStageSeqNo[1] == 50) {
				gData.nLoadStageSeqNo[3] = 0;
				g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_STAGE_1, 81);
			}
		}
	}
	gLot.nJobStatus = gData.nCarrierStart = 1;
	gLot.nJobCount = gLot.dwJobtEnd = gLot.dwJobStart = 0;
	for(int i=0; i<3; i++) for(int j=0; j<2; j++) gNG->nTrayOX[i][j] = 0;

	g_objLogFile.Save_HandlerLog("[Work Mode] Work_Start().........");

	return TRUE;
}

BOOL CWorkDlg::LotID_Check()
{
	int nCMCnt, nLotCnt;
	CString strMsg, strTemp, strTemp2, sLog;

	nLotCnt = 0;
	for(int i=0; i<28; i++) {
		m_stcLotsIdS[i].GetWindowText(strTemp);
		m_stcCmsCountS[i].GetWindowText(strTemp2);
		nCMCnt = atoi(strTemp2);

		// Input Error Check
		if (nCMCnt > 40) {
			if (gData.nLanguage == 0) strMsg.Format("[%d] Check Lot 수량 (Max:40)....................", i+1);
			else					  strMsg.Format("[%d] Check Lot Quantity ....................", i+1);
			g_objCommon.Show_MsgBox(1, strMsg);
			return FALSE;
		}
		if (nCMCnt > 0) {
			if (strTemp.GetLength() < 1) {
				if (gData.nLanguage == 0) strMsg.Format("[%d] Check Lot ID, 수량 ....................", i+1);
				else					  strMsg.Format("[%d] Check Lot ID, Quantity ....................", i+1);
				g_objCommon.Show_MsgBox(1, strMsg);
				return FALSE;
			}
		} else {
			if (strTemp.GetLength() > 0) {
				if (gData.nLanguage == 0) strMsg.Format("[%d] Check Lot ID, 수량 ....................", i+1);
				else					  strMsg.Format("[%d] Check Lot ID, Quantity ....................", i+1);
				g_objCommon.Show_MsgBox(1, strMsg);
				return FALSE;
			}
		}

		if (nCMCnt > 0) {
			gLot.sLotID[i] = strTemp;
			gLot.nCmCount[i] = nCMCnt;
			nLotCnt++;
			if (gLot.nLotStatus[i] == 3) gLot.bEmptyTray[i] = FALSE;

			sLog.Format("[Work Mode] LotID_Copy] (%d-%s)", i+1, gLot.sLotID[i]);
			g_objLogFile.Save_HandlerLog(sLog);
		} else {
			sLog.Format("[Work Mode] LotID_Delete] (%d-%s)", i+1, gLot.sLotID[i]);
			g_objLogFile.Save_HandlerLog(sLog);

			gLot.nCmCount[i] = gLot.nGoodCount[i] = gLot.nNgCount[i] = gLot.nLotStatus[i] = gLot.nSkipCount[i] = 0;
			gLot.sLotID[i] = "";
		}
	}

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (pEquipData->bUseMES) {
		if (nLotCnt > 0 && gLot.nJobStatus == 0) {
			g_objCommon.Show_MsgBox(1, "MES사용시 Lot 정보를 입력하지 마세요.....");
			return FALSE;
		}
		return TRUE;
	} else {
		if (nLotCnt < 1 && !g_objSequenceMain.Get_IsAutoRun()) {
			g_objCommon.Show_MsgBox(1, "Lot 정보를 입력해야 합니다.....");
			return FALSE;
		}
	}

	for(int i=0; i<28; i++) {
		for(int j=0; j<28; j++) {
			if (i != j && gLot.sLotID[i].GetLength() > 0 && gLot.sLotID[i] == gLot.sLotID[j]) {
				gLot.nCmCount[j] = 0;
				if (gData.nLanguage == 0) strMsg.Format("[%d-%d]에 동일 Lot ID가 있습니다........", i+1, j+1);
				else					  strMsg.Format("[%d-%d] has the same Lot ID........", i+1, j+1);
				g_objCommon.Show_MsgBox(1, strMsg);
				return FALSE;
			}
		}
	}
/*
	if (gLot.nCmCount[1] > 0 || gLot.nCmCount[2] > 0) {
		if (gLot.nCmCount[0] < 1) {
			gLot.nCmCount[1] = gLot.nCmCount[2] = 0;
			g_objCommon.Show_MsgBox(1, "[1] Lot ID는 첫번째부터 입력해야 합니다.....");
			return FALSE;
		}
	}
	if (gLot.nCmCount[2] > 0) {
		if (gLot.nCmCount[1] < 1) {
			gLot.nCmCount[2] = 0;
			g_objCommon.Show_MsgBox(1, "[2] Lot ID는 순서되로 입력해야 합니다.....");
			return FALSE;
		}
	}
	if (gLot.nCmCount[4] > 0 || gLot.nCmCount[5] > 0) {
		if (gLot.nCmCount[3] < 1) {
			gLot.nCmCount[4] = gLot.nCmCount[5] = 0;
			g_objCommon.Show_MsgBox(1, "[4] Lot ID는 첫번째부터 입력해야 합니다.....");
			return FALSE;
		}
	}
	if (gLot.nCmCount[5] > 0) {
		if (gLot.nCmCount[4] < 1) {
			gLot.nCmCount[5] = 0;
			g_objCommon.Show_MsgBox(1, "[5] Lot ID는 순서되로 입력해야 합니다.....");
			return FALSE;
		}
	}
*/
	return TRUE;
}

void CWorkDlg::Initial_Controls() 
{
	for (int i = 0; i < 9; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 4; i++) m_Label[i].Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x80, 0x80));		// Tray
	for (int i = 0; i <28; i++) m_stcLotsIdS[i].Init_Ctrl("Arial", 11, FALSE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
	for (int i = 0; i <28; i++) m_stcCmsCountS[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));
	for (int i = 0; i <28; i++) m_rdoSlectNo[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoSlectNo[7].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xCC, 0xFF), 0, 0);
	m_rdoSlectNo[15].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xCC, 0xFF), 0, 0);
	m_rdoSlectNo[23].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xCC, 0xFF), 0, 0);
//	m_bmpEquipment.LoadBitmap(IDB_EQUIP_WORK);
//	m_imgEquipment.SetBitmap(m_bmpEquipment);
	for (int i = 0; i < 9; i++) m_ledEquipOption[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 6; i++) m_ledVisionSts[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 4; i++) m_ledBarcodeSts[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	m_stcEquipType.Init_Ctrl("Arial", 12, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
//	for (int i = 0; i < 17; i++) m_stcJobLotID[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD0, 0xD0, 0xD0));
//	for (int i = 0; i < 17; i++) m_stcJobCarNo[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xD0, 0xD0, 0xD0));
	for (int i = 0; i < 17; i++) m_stcJobLotID[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xD0, 0xFF));
	for (int i = 0; i < 17; i++) m_stcJobCarNo[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xFF));

	for (int i = 0; i < 7; i++) m_stcMZLotID[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xD0, 0xFF));
	m_stcMZLotID[6].Init_Ctrl("바탕", 13, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xD0, 0xFF));
	for (int i = 0; i < 6; i++) m_stcMZCarNo[i].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xFF));
	m_stcMZCarNo[6].Init_Ctrl("바탕", 16, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x66, 0xFF, 0xCC));
	for (int i = 0; i < 4; i++) m_stcCVMZCheck[i].Init_Ctrl("바탕", 10, TRUE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));

	m_rdoWorkStart.Init_Ctrl("바탕", 20, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_rdoWorkStop.Init_Ctrl("바탕", 20, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_ledInitComplete.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em24);
	m_chkCycleStop.Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 5; i++) m_picTrayBack[i].Set_Color(COLOR_DEFAULT, RGB(0xF0, 0xF0, 0xC0));
	m_stcAlignLine.Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	m_stcCarrierLine.Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	m_stcNGLine.Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	m_stcGoodLine.Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	Initial_Grid(&m_grdAlign, TRAY_CM_Y, TRAY_CM_X);
	Initial_Grid(&m_grdCarrier, TRAY_CM_Y, TRAY_CM_X);
	Initial_Grid(&m_grdGoodTray, TRAY_CM_Y, TRAY_CM_X);
	Initial_Grid(&m_grdNGTray, TRAY_CM_Y, TRAY_CM_X);
	Initial_JobGrid(&m_grdJob, 100, 9);
	for (int i = 0; i < 33; i++) m_stcWorkCase[i].Init_Ctrl("바탕", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 4; i++) m_stcStageInfor[i].Init_Ctrl("바탕", 12, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0xFF));
	for (int i = 0; i <15; i++) m_ledTrayCheck[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);

	m_stcMesConnect.Init_Ctrl("바탕", 8, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	m_stcMesOnline.Init_Ctrl("바탕", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00));
	m_lblOperId.Init_Ctrl("바탕", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x20, 0x20, 0x80));
	m_stcOperId.Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));

}

void CWorkDlg::Initial_Grid(CGridCS *pGrid, int nRows, int nCols)
{
	pGrid->Set_RowCount(nRows);
	pGrid->Set_ColCount(nCols);
	pGrid->Set_FixRowCount(0);
	pGrid->Set_FixColCount(0);

	long lTotalW = pGrid->Get_GridWidth();
	long lTotalH = pGrid->Get_GridHeight();
	long lCellW = lTotalW / nCols;
	long lCellH = lTotalH / nRows;

	int nNo=1;
	CString str;
	for (int i=0; i<nRows; i++) {
		pGrid->Set_RowHeight(i, lCellH);
		for (int j = nCols-1; j>=0; j--) {
			if(i==0) pGrid->Set_ColWidth(j, lCellW);
			str.Format(_T("%d"), nNo++);
			pGrid->Set_CellText(i, j, str);
		}
	}
/*
	for (int j = nCols-1; j>=0; j--) {
		pGrid->Set_ColWidth(j, lCellW);
		for (int i = nRows-1; i>=0; i--) {
			if(j==nCols-1) pGrid->Set_RowHeight(i, lCellH);
			str.Format(_T("%d"), nNo++);
			pGrid->Set_CellText(i, j, str);
		}
	}
*/
}

void CWorkDlg::Initial_JobGrid(CGridCS *pGrid, int nRows, int nCols)
{
	pGrid->Set_RowCount(nRows);
	pGrid->Set_ColCount(nCols);
	pGrid->Set_FixRowCount(0);
	pGrid->Set_FixColCount(0);

	long lTotalW = pGrid->Get_GridWidth();
	long lTotalH = pGrid->Get_GridHeight();
	long lCellW = lTotalW / nCols;
//	long lCellH = lTotalH / nRows;
	long lCellH = lTotalH / 10;
	long  lWidth[9]= {   50,      150,     70,    70,   70,   74,     160,         160,      98};
	CString sTxt[9]= { "PortNo","Lot_ID","Count","Good","NG","Rate","Start_Time","End_Time","Tack"};

	for (int i=0; i<nRows; i++) {
		pGrid->Set_RowHeight(i, lCellH);
		for (int j=0; j<nCols; j++) {
			if(i==0) {
				pGrid->Set_ColWidth(j, lWidth[j]);
				pGrid->Set_CellText(i, j, sTxt[j]);
				pGrid->Set_CellBackClr(i, j, RGB(0xF0, 0xF0, 0xC0));
			}
		}
	}
}

void CWorkDlg::Display_Status()
{
	CString strTemp, strText;

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (g_objMesAgent.Is_Connected()) { m_stcMesConnect.Set_Text("Connected"); m_stcMesConnect.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00)); }
	else { m_stcMesConnect.Set_Text("Disconnected"); m_stcMesConnect.Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00)); }

	if (g_objMesAgent.Is_HostOnline()) { m_stcMesOnline.Set_Text("Online"); m_stcMesOnline.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00)); }
	else { m_stcMesOnline.Set_Text("Offline"); m_stcMesOnline.Set_Color(RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0x00)); }

	BOOL bInitComplete = g_objSequenceInit.Get_InitComplete();
	m_ledInitComplete.Set_On(bInitComplete);
/*
	if (gLot.sLotID[0].GetLength() > 0) m_stcLotsIdS[0].SetWindowText(gLot.sLotID[0]);
	if (gLot.sLotID[1].GetLength() > 0) m_stcLotsIdS[1].SetWindowText(gLot.sLotID[1]);
	if (gLot.sLotID[2].GetLength() > 0) m_stcLotsIdS[2].SetWindowText(gLot.sLotID[2]);
	if (gLot.sLotID[3].GetLength() > 0) m_stcLotsIdS[3].SetWindowText(gLot.sLotID[3]);
	if (gLot.sLotID[4].GetLength() > 0) m_stcLotsIdS[4].SetWindowText(gLot.sLotID[4]);
	if (gLot.sLotID[5].GetLength() > 0) m_stcLotsIdS[5].SetWindowText(gLot.sLotID[5]);
*/
	//Inspection Stage 1,2,3,4 위치 Display
	for (int i=0; i<4; i++) {
		if		(gData.nVisionPos[0][i] > 0 && gData.nVisionPos[1][i] > 0) strText.Format("%d,%d", gData.nVisionPos[1][i], gData.nVisionPos[0][i]);
		else if (gData.nVisionPos[0][i] > 0)							   strText.Format("%d", gData.nVisionPos[0][i]);
		else if (gData.nVisionPos[1][i] > 0)							   strText.Format("%d", gData.nVisionPos[1][i]);
		else															   strText = "";
		m_stcStageInfor[i].SetWindowText(strText);
	}

	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();
	DX_DATA_16 *pDX16 = g_objAJinAXL.Get_pDX16();
	DX_DATA_19 *pDX19 = g_objAJinAXL.Get_pDX19();

	m_stcJobLotID[ 0].SetWindowText(gData.sLotID_Tansfer[0]);
	m_stcJobLotID[ 1].SetWindowText(gData.sLotID_Tansfer[1]);
	m_stcJobLotID[ 4].SetWindowText(gData.sLotID_LoadPicker[0]);
	m_stcJobLotID[ 5].SetWindowText(gData.sLotID_LoadPicker[1]);
	m_stcJobLotID[ 6].SetWindowText(gData.sLotID_VisionStage[0]);
	m_stcJobLotID[ 7].SetWindowText(gData.sLotID_VisionStage[1]);
	m_stcJobLotID[ 8].SetWindowText(gData.sLotID_VisionStage[2]);
	m_stcJobLotID[ 9].SetWindowText(gData.sLotID_VisionStage[3]);
	m_stcJobLotID[10].SetWindowText(gData.sLotID_UnloadPicker[0]);
	m_stcJobLotID[11].SetWindowText(gData.sLotID_UnloadPicker[1]);

	if (gData.nPortNo_LoadStage[0] > 0) {
		strText.Format("%s,%s", gData.sLotID_LoadStage[0], gData.sCarID_LoadStage[0]);
	} else strText = "";
	m_stcJobLotID[2].SetWindowText(strText);

	if (gData.nPortNo_LoadStage[1] > -1) {
		strText.Format("%s,%s", gData.sLotID_LoadStage[1], gData.sCarID_LoadStage[1]);
	} else strText = "";
	m_stcJobLotID[3].SetWindowText(strText);

	if (gData.nTrayNo_NGTray[0] > 0) {
		strText.Format("%s,%s", gData.sLotID_NGTray[0], gData.sCarID_NGTray[0]);
	} else strText = "";
	m_stcJobLotID[12].SetWindowText(strText);

	if (gData.nTrayNo_NGTray[1] > 0) {
		strText.Format("%s,%s", gData.sLotID_NGTray[1], gData.sCarID_NGTray[1]);
	} else strText = "";
	m_stcJobLotID[13].SetWindowText(strText);

	if (gData.nPortNo_GoodTray[0] > 0) {
		strText.Format("%s,%s", gData.sLotID_GoodTray[0], gData.sCarID_GoodTray[0]);
	} else strText = "";
	m_stcJobLotID[14].SetWindowText(strText);

	if (gData.nPortNo_GoodTray[1] > 0) {
		strText.Format("%s,%s", gData.sLotID_GoodTray[1], gData.sCarID_GoodTray[1]);
	} else strText = "";
	m_stcJobLotID[15].SetWindowText(strText);
	m_stcJobLotID[16].SetWindowText(gData.sLotID_NGBuffer);

	//CString	sMZID[10];				//0:UCV, 1:ECV, 2:Elavator, 3:MZTransfer, 4:Load, 5:Buffer, 6:NG, 7:Good, 8:Unload-Elevator
	m_stcMZLotID[0].SetWindowText(gData.sMZID[3]);
	m_stcMZLotID[1].SetWindowText(gData.sMZID[2]);
	m_stcMZLotID[2].SetWindowText(gData.sMZID[8]);
	m_stcMZLotID[3].SetWindowText(gData.sMZID[4]); if (gData.sMZID[4].GetLength() > 0) strText.Format("%d/%d", gData.nMZSlotNo[0], gData.nMZSlotNo[3]); else strText = ""; m_stcMZCarNo[3].SetWindowText(strText);
	m_stcMZLotID[4].SetWindowText(gData.sMZID[6]); if (gData.sMZID[6].GetLength() > 0) strText.Format("%d", gData.nMZSlotNo[1]); else strText = ""; m_stcMZCarNo[4].SetWindowText(strText);
	int nDipNo[9] = {0, 8, 7, 6, 5, 4, 3, 2, 1 };
	int nMZSno = gData.nMZSlotNo[2];
	if (gData.sMZID[7].GetLength() > 0 && nMZSno > 0 && nMZSno < 9) strText.Format("%d", nDipNo[nMZSno]);
	else															strText = "";
	m_stcMZLotID[5].SetWindowText(gData.sMZID[7]); m_stcMZCarNo[5].SetWindowText(strText);
	strText.Format("Buffer: %s", gData.sMZID[5]);
	m_stcMZLotID[6].SetWindowText(strText);
	strText.Format("ROS 대기수량: %d", gData.nROSReqCount);
	m_stcMZCarNo[6].SetWindowText(strText);

/*
	if (gData.nPortNo_MZTransfer > 0) strText.Format("%d", gData.nPortNo_MZTransfer);
	else							  strText = "";
	m_stcMZCarNo[0].SetWindowText(strText);
	if (gData.nPortNo_MZElevator[0] > 0) strText.Format("%d", gData.nPortNo_MZElevator[0]);
	else								 strText = "";
	m_stcMZCarNo[1].SetWindowText(strText);
	if (gData.nPortNo_MZElevator[1] > 0) strText.Format("%d", gData.nPortNo_MZElevator[1]);
	else								 strText = "";
	m_stcMZCarNo[2].SetWindowText(strText);
	if (gData.nPortNo_Magazine[0] > 0) strText.Format("C%d-P%d", gData.nTrayNo_Magazine[0], gData.nPortNo_Magazine[0]);
	else							   strText = "";
	m_stcMZCarNo[3].SetWindowText(strText);
	if (gData.nPortNo_Magazine[1] > 0) strText.Format("C%d-P%d", gData.nTrayNo_Magazine[1], gData.nPortNo_Magazine[1]);
	else							   strText = "";
	m_stcMZCarNo[4].SetWindowText(strText);
	if (gData.nPortNo_Magazine[2] > 0) strText.Format("C%d-P%d", gData.nTrayNo_Magazine[2], gData.nPortNo_Magazine[2]);
	else							   strText = "";
	m_stcMZCarNo[5].SetWindowText(strText);
*/

	if (gData.nPortNo_Tansfer[0] > 0) strText.Format("%d-%d", gData.nTrayNo_Tansfer[0], gData.nPortNo_Tansfer[0]);
	else						 strText = "";
	m_stcJobCarNo[0].SetWindowText(strText);
	if (gData.nPortNo_Tansfer[1] > 0) strText.Format("%d-%d", gData.nTrayNo_Tansfer[1], gData.nPortNo_Tansfer[1]);
	else						 strText = "";
	m_stcJobCarNo[1].SetWindowText(strText);
	if (gData.nPortNo_LoadStage[0] > 0) strText.Format("%d-%d", gData.nTrayNo_LoadStage[0], gData.nPortNo_LoadStage[0]);
	else						 strText = "";
	m_stcJobCarNo[2].SetWindowText(strText);
	if (gData.nPortNo_LoadStage[1] > 0) strText.Format("%d-%d", gData.nTrayNo_LoadStage[1], gData.nPortNo_LoadStage[1]);
	else						 strText = "";
	m_stcJobCarNo[3].SetWindowText(strText);
	if (gData.nPortNo_LoadPicker[0] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_LoadPicker[0], gData.nPortNo_LoadPicker[0], gData.InfoLoadPick[0][9]);
	else						 strText = "";
	m_stcJobCarNo[4].SetWindowText(strText);
	if (gData.nPortNo_LoadPicker[1] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_LoadPicker[1], gData.nPortNo_LoadPicker[1], gData.InfoLoadPick[1][9]);
	else						 strText = "";
	m_stcJobCarNo[5].SetWindowText(strText);

	if (gData.nPortNo_VisionStage[0] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_VisionStage[0], gData.nPortNo_VisionStage[0], gData.InfoVision[0][9]);
	else						 strText = "";
	m_stcJobCarNo[6].SetWindowText(strText);
	if (gData.nPortNo_VisionStage[1] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_VisionStage[1], gData.nPortNo_VisionStage[1], gData.InfoVision[1][9]);
	else						 strText = "";
	m_stcJobCarNo[7].SetWindowText(strText);
	if (gData.nPortNo_VisionStage[2] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_VisionStage[2], gData.nPortNo_VisionStage[2], gData.InfoVision[2][9]);
	else						 strText = "";
	m_stcJobCarNo[8].SetWindowText(strText);
	if (gData.nPortNo_VisionStage[3] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_VisionStage[3], gData.nPortNo_VisionStage[3], gData.InfoVision[3][9]);
	else						 strText = "";
	m_stcJobCarNo[9].SetWindowText(strText);

	if (gData.nPortNo_UnloadPicker[0] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_UnloadPicker[0], gData.nPortNo_UnloadPicker[0], gData.InfoUnloadPick[0][9]);
	else						 strText = "";
	m_stcJobCarNo[10].SetWindowText(strText);
	if (gData.nPortNo_UnloadPicker[1] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_UnloadPicker[1], gData.nPortNo_UnloadPicker[1], gData.InfoUnloadPick[1][9]);
	else						 strText = "";
	m_stcJobCarNo[11].SetWindowText(strText);
	if (gData.nPortNo_NGTray[0] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_NGTray[0], gData.nPortNo_NGTray[0], gLot.nOutTrayCnt[gData.nPortNo_NGTray[0]-1][1]);
	else						 strText = "";
	m_stcJobCarNo[12].SetWindowText(strText);
	if (gData.nPortNo_NGTray[1] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_NGTray[1], gData.nPortNo_NGTray[1], gLot.nOutTrayCnt[gData.nPortNo_NGTray[1]-1][1]);
	else						 strText = "";
	m_stcJobCarNo[13].SetWindowText(strText);
	if (gData.nPortNo_GoodTray[0] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_GoodTray[0], gData.nPortNo_GoodTray[0], gLot.nOutTrayCnt[gData.nPortNo_GoodTray[0]-1][0]);
	else						 strText = "";
	m_stcJobCarNo[14].SetWindowText(strText);
	if (gData.nPortNo_GoodTray[1] > 0) strText.Format("%d-%d-%d", gData.nTrayNo_GoodTray[1], gData.nPortNo_GoodTray[1], gLot.nOutTrayCnt[gData.nPortNo_GoodTray[1]-1][0]);
	else						 strText = "";
	m_stcJobCarNo[15].SetWindowText(strText);
	if (gData.nPortNo_NGBuffer > 0) strText.Format("P%d-C%d", gData.nPortNo_NGBuffer, gData.nTrayNo_NGBuffer);
	else						 strText = "";
	m_stcJobCarNo[16].SetWindowText(strText);

	if (pDX16->iLDCV2FCnt1 || pDX16->iLDCV2FCnt2 || pDX16->iLDCV2FCnt3 || pDX16->iLDCV2FCnt4 || pDX16->iLDCV2FCnt5 || pDX16->iLDCV2FCnt6 || pDX16->iLDVC2FStop)
							m_stcCVMZCheck[0].Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00));
	else					m_stcCVMZCheck[0].Set_Color(RGB(0xFF, 0xFF, 0x00), RGB(0xFF, 0x00, 0x00));
	if (pDX16->iLDCV1FCnt1 || pDX16->iLDCV1FCnt2 || pDX16->iLDCV1FCnt3 || pDX16->iLDCV1FCnt4 || pDX16->iLDCV1FCnt5 || pDX16->iLDCV1FCnt6 || pDX16->iLDVC1FStop)
							m_stcCVMZCheck[1].Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00));
	else					m_stcCVMZCheck[1].Set_Color(RGB(0xFF, 0xFF, 0x00), RGB(0xFF, 0x00, 0x00));
	if (pDX19->iULCV2FCnt1 || pDX19->iULCV2FCnt2 || pDX19->iULCV2FCnt3 || pDX19->iULCV2FCnt4 || pDX19->iULCV2FCnt5 || pDX19->iULCV2FCnt6 || pDX19->iULCV2FStop)
							m_stcCVMZCheck[2].Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00));
	else					m_stcCVMZCheck[2].Set_Color(RGB(0xFF, 0xFF, 0x00), RGB(0xFF, 0x00, 0x00));
	if (pDX19->iULCV1FCnt1 || pDX19->iULCV1FCnt2 || pDX19->iULCV1FCnt3 || pDX19->iULCV1FCnt4 || pDX19->iULCV1FCnt5 || pDX19->iULCV1FCnt6 || pDX19->iULCV1FStop)
							m_stcCVMZCheck[3].Set_Color(RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00));
	else					m_stcCVMZCheck[3].Set_Color(RGB(0xFF, 0xFF, 0x00), RGB(0xFF, 0x00, 0x00));		

	m_ledVisionSts[0].Set_On(g_objInspector.Get_VisionStatus(INSPECTOR_PC1));
	m_ledVisionSts[1].Set_On(g_objInspector.Get_VisionStatus(INSPECTOR_PC2));
	m_ledVisionSts[2].Set_On(g_objInspector.Get_VisionStatus(INSPECTOR_PC3));
	m_ledVisionSts[3].Set_On(g_objInspector.Get_VisionStatus(INSPECTOR_PC4));
	m_ledVisionSts[4].Set_On(g_objInspector.Get_VisionStatus(INSPECTOR_PC5));
	m_ledVisionSts[5].Set_On(g_objDispatcher.Is_Connected());

	m_ledBarcodeSts[0].Set_On(g_objBarcodeLot_Cognex.Is_Connected());
	m_ledBarcodeSts[1].Set_On(g_objCarrierRFID_Load.Is_Connected());
	m_ledBarcodeSts[2].Set_On(g_objCarrierRFID_NG.Is_Connected());
	m_ledBarcodeSts[3].Set_On(g_objCarrierRFID_Good.Is_Connected());

	for(int i=0; i<28; i++) {
		if	(gLot.nLotStatus[i] == 1 || gLot.nLotStatus[i] == 2)
			m_stcLotsIdS[i].Init_Ctrl("Arial", 11, FALSE, COLOR_DEFAULT, RGB(0x00, 0xA0, 0x00));	//작업중일때
		else
			m_stcLotsIdS[i].Init_Ctrl("Arial", 11, FALSE, COLOR_DEFAULT, RGB(0x80, 0xF0, 0xF0));	//매거진 없을때
	}

	int *pCase = g_objSequenceMain.Get_pMainRunCase();
	CString strCase;
	for (int i = 0; i < 33; i++) {
		strCase.Format("%02d", *(pCase + i));
		m_stcWorkCase[i].SetWindowText(strCase);
	}

	strText = "";
	if (pEquipData->bUseAlignOffset) {
		strText.Format("%0.3lf", gData.dAlignOffset[0][1]);
	} else {
		if (*(pCase +  9) > 18 && *(pCase +  9) < 30 && gData.nTrayNo_LoadStage[0] > 0) strText.Format("%d", gData.nTrayNo_LoadStage[0]);
		if (*(pCase + 10) > 30 && *(pCase + 10) < 30 && gData.nTrayNo_LoadStage[1] > 0) strText.Format("%d", gData.nTrayNo_LoadStage[1]);
	}
	m_stcAlignLine.SetWindowText(strText);	//Align Tray

	strText = "";
	if (pEquipData->bUseAlignOffset) {
		strText.Format("%0.3lf", gData.dAlignOffset[1][1]);
	} else {
		if (*(pCase +  9) == 30 && gData.nTrayNo_LoadStage[0] > 0) strText.Format("%d/%d", gData.nTrayNo_LoadStage[0], gLot.nTrayPutCnt[gData.nPortNo_LoadStage[0]-1][2]);
		if (*(pCase + 10) == 30 && gData.nTrayNo_LoadStage[1] > 0) strText.Format("%d/%d", gData.nTrayNo_LoadStage[1], gLot.nTrayPutCnt[gData.nPortNo_LoadStage[1]-1][2]);
	}
	m_stcCarrierLine.SetWindowText(strText);	//Load Tray

	strText = "";
	if (*(pCase + 19) == 30 && gData.nTrayNo_NGTray[0] > 0) strText.Format("%d/%d", gLot.nOutTrayCnt[gData.nPortNo_NGTray[0]-1][1], gLot.nTrayPutCnt[gData.nPortNo_NGTray[0]-1][1]);
	if (*(pCase + 20) == 30 && gData.nTrayNo_NGTray[1] > 0) strText.Format("%d/%d", gLot.nOutTrayCnt[gData.nPortNo_NGTray[1]-1][1], gLot.nTrayPutCnt[gData.nPortNo_NGTray[1]-1][1]);
	m_stcNGLine.SetWindowText(strText);		//NG Tray

	strText = "";
	if (*(pCase + 21) == 30 && gData.nTrayNo_GoodTray[0] > 0) strText.Format("%d/%d", gLot.nOutTrayCnt[gData.nPortNo_GoodTray[0]-1][0], gLot.nTrayPutCnt[gData.nPortNo_GoodTray[0]-1][0]);
	if (*(pCase + 22) == 30 && gData.nTrayNo_GoodTray[1] > 0) strText.Format("%d/%d", gLot.nOutTrayCnt[gData.nPortNo_GoodTray[1]-1][0], gLot.nTrayPutCnt[gData.nPortNo_GoodTray[1]-1][0]);
	m_stcGoodLine.SetWindowText(strText);	//Good Tray

	Display_TrayCheck();

	strText.Format("Carrier 수량:  %d", gData.nCarrierCount);
	m_stcEquipType.SetWindowText(strText);

	for (int i = 0; i < 8; i++) {
		if		(gLot.nCarrierExist[1][i] == 1) m_stcNGMZCar[i].Init_Ctrl("바탕", 8, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x00, 0xFF));
		else if (gLot.nCarrierExist[1][i] == 2) m_stcNGMZCar[i].Init_Ctrl("바탕", 8, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x00, 0xFF, 0x00));
		else									m_stcNGMZCar[i].Init_Ctrl("바탕", 8, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xCC, 0xCC, 0xCC));
	}
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 0, NULL);
}

void CWorkDlg::Display_TrayCheck()
{
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();
	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();
	DX_DATA_11 *pDX11 = g_objAJinAXL.Get_pDX11();
	DX_DATA_12 *pDX12 = g_objAJinAXL.Get_pDX12();
	DX_DATA_17 *pDX17 = g_objAJinAXL.Get_pDX17();
	DX_DATA_18 *pDX18 = g_objAJinAXL.Get_pDX18();
	DX_DATA_20 *pDX20 = g_objAJinAXL.Get_pDX20();

	m_ledTrayCheck[ 0].Set_On(pDX20->iMZTransExist);
	m_ledTrayCheck[ 1].Set_On(pDX17->iLDCVElevatorCVStop);
	m_ledTrayCheck[ 2].Set_On(pDX17->iULCVElevatorMZExist);
	m_ledTrayCheck[ 3].Set_On(pDX18->iLDMZElevatorExist);
	m_ledTrayCheck[ 4].Set_On(pDX18->iNGMZElevatorExist);
	m_ledTrayCheck[ 5].Set_On(pDX18->iGDMZElevatorExist);
	m_ledTrayCheck[ 6].Set_On(pDX18->iMZBufferExist);
	m_ledTrayCheck[ 7].Set_On(pDX03->iTransferLTrayExist);
	m_ledTrayCheck[ 8].Set_On(pDX03->iTransferRTrayExist);
	m_ledTrayCheck[ 9].Set_On(pDX04->iLoadStage1TrayExist);
	m_ledTrayCheck[10].Set_On(pDX04->iLoadStage2TrayExist);
	m_ledTrayCheck[11].Set_On(pDX11->iNGStage1TrayExist);
	m_ledTrayCheck[12].Set_On(pDX11->iNGStage2TrayExist);
	m_ledTrayCheck[13].Set_On(pDX12->iGoodStage1TrayExist);
	m_ledTrayCheck[14].Set_On(pDX12->iGoodStage2TrayExist);
}

void CWorkDlg::Reset_AlarmLog()
{
	CString strLog, strErrNo;
	SYSTEMTIME time;

	gAlm.bBegin = FALSE;
	GetLocalTime(&time);

	gAlm.dwEndTime = GetTickCount();
	gAlm.sEndTime.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	gAlm.dwProcTime = gAlm.dwEndTime - gAlm.dwStartTime;
	strLog.Format("%s,%04d,%s,%s,%s,%d", gAlm.sLotID, gAlm.nAlmNo, gAlm.sAlmMsg, gAlm.sStartTime, gAlm.sEndTime, gAlm.dwProcTime);
	g_objLogFile.Save_AlarmLog(strLog);	// Alarm Reset

	double dSec, dMin;
	dSec = gAlm.dwProcTime / 1000.0;
	dMin = dSec / 60.0;
	strLog.Format("%s,%04d,%s,%s,%s,%0.3lf,%0.3lf", gAlm.sLotID, gAlm.nAlmNo, gAlm.sAlmMsg, gAlm.sStartTime, gAlm.sEndTime, dSec, dMin);
	g_objLogFile.Save_AlarmResetLog(strLog);

	strLog.Format(",,,,,,,,,,ErrorTime,,,%0.3lf", (double)(gAlm.dwProcTime/1000.0));
	g_objLogFile.Save_SpcMccLog(strLog, gAlm.sLotID);

	strLog.Format("%s,%04d,%s,%s,%s,%s,%d", gAlm.sLotID, gAlm.nAlmNo, gAlm.sSpcInfo, gAlm.sAlmMsg, gAlm.sStartTime, gAlm.sEndTime, gAlm.dwProcTime);
	g_objLogFile.Save_SpcErrorLog(strLog, gAlm.sLotID);

	strErrNo.Format("%04d", gAlm.nAlmNo);
	g_objMesAgent.Set_ErrorUpdate(0, strErrNo);

	if (gAlm.nPortNo > 0) {
		gLot.dwErrorTime[gAlm.nPortNo-1] += gAlm.dwProcTime; gLot.nErrorCount[gAlm.nPortNo-1]++;
	}
}

void CWorkDlg::Set_AutoRun(BOOL bAutoRun)
{
	if (bAutoRun) m_rdoWorkStart.SetCheck(TRUE);
	else m_rdoWorkStop.SetCheck(TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// Message

LRESULT CWorkDlg::OnUpdateModel(WPARAM wParam, LPARAM lParam)
{
	int nType = (int) wParam;
	if (nType == 1) {
		for(int i=0; i<28; i++) {
			if (gLot.nCmCount[i] > 0) {
				CString sCnt;
				sCnt.Format("%d", gLot.nCmCount[i]);
				m_stcLotsIdS[i].SetWindowText(gLot.sLotID[i]);
				m_stcCmsCountS[i].SetWindowText(sCnt);
			}
		}
		return 0;
	}

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	m_bmpEquipment.DeleteObject();
	if		(gData.bUseDryRun)			m_bmpEquipment.LoadBitmap(IDB_EQUIP_DOOR);
	else if (!pEquipData->bUseDoorLock) m_bmpEquipment.LoadBitmap(IDB_EQUIP_DOOR_KOR);
	else								m_bmpEquipment.LoadBitmap(IDB_EQUIP_WORK);
	m_imgEquipment.SetBitmap(m_bmpEquipment);

	return 0;
}

LRESULT CWorkDlg::OnUpdateTrayInfo(WPARAM nTray, LPARAM lParam)
{
	int nj;
	CString strText;

	for (int i = 0; i < TRAY_CM_Y; i++) {
		for (int j = 0; j <TRAY_CM_X; j++) {
			if (j == 0) nj = 3; if (j == 1) nj = 2; if (j == 2) nj = 1; if (j == 3) nj = 0;
			// (y,x) 0:Empty, 1:NG      2:Good
			if		(gData.InfoAlignTray[i][j] == 0)	m_grdAlign.Set_CellBackClr(i, nj, RGB(0xFF, 0xFF, 0xFF));	// 0 Empty
			else if (gData.InfoAlignTray[i][j] == 1)	m_grdAlign.Set_CellBackClr(i, nj, RGB(0xFF, 0x00, 0x00));	// 1 NG
			else if (gData.InfoAlignTray[i][j] == 2)	m_grdAlign.Set_CellBackClr(i, nj, RGB(0x00, 0xFF, 0x00));	// 2 Good

			// (y,x) 0:Empty, 1:Module, 2:Good, NG:3~
			if		(gData.InfoLoadTray[i][j] > 0)	m_grdCarrier.Set_CellBackClr(i, nj, RGB(0xFF, 0x80, 0xFF));
			else									m_grdCarrier.Set_CellBackClr(i, nj, RGB(0xFF, 0xFF, 0xFF));	// 0 Empty

			if		(gData.InfoNgTray[i][j] > 0)	m_grdNGTray.Set_CellBackClr(i, nj, RGB(0xFF, 0x00, 0x00));
			else									m_grdNGTray.Set_CellBackClr(i, nj, RGB(0xFF, 0xFF, 0xFF));	// 0 Empty

			if		(gData.InfoGoodTray[i][j] == 9)	m_grdGoodTray.Set_CellBackClr(i, nj, RGB(0x00, 0x00, 0xFF));	//Marginal
			else if	(gData.InfoGoodTray[i][j] > 0)	m_grdGoodTray.Set_CellBackClr(i, nj, RGB(0x00, 0xFF, 0x00));	//Good
			else									m_grdGoodTray.Set_CellBackClr(i, nj, RGB(0xFF, 0xFF, 0xFF));	//Empty

		}
	}
	g_dlgOperator.Update_TrayInfo(nTray);

/*
				if		(gData.InfoNgTray[i][j] == 1) m_grdNgTray.Set_CellBackClr(i, j, RGB(0xFF, 0x80, 0xFF));	// 1 Module
				else if	(gData.InfoNgTray[i][j] == 2) m_grdNgTray.Set_CellBackClr(i, j, RGB(0x00, 0xFF, 0x00));	// 2 Good
				else if (gData.InfoNgTray[i][j] == 3) m_grdNgTray.Set_CellBackClr(i, j, RGB(0xFF, 0x00, 0x00));	// 3 Normal NG
				else if (gData.InfoNgTray[i][j] == 4) m_grdNgTray.Set_CellBackClr(i, j, RGB(0xF0, 0xF0, 0x00));	// 4 MES 성능불량
				else if (gData.InfoNgTray[i][j] == 5) m_grdNgTray.Set_CellBackClr(i, j, RGB(0xFF, 0x60, 0xFF));	// 5 Barcode Mismatch
				else if (gData.InfoNgTray[i][j] == 6) m_grdNgTray.Set_CellBackClr(i, j, RGB(0x80, 0x40, 0xF0));	// 6 HREN
				else if (gData.InfoNgTray[i][j] == 7) m_grdNgTray.Set_CellBackClr(i, j, RGB(0x00, 0x80, 0xFF));	// 7 Spider
				else if (gData.InfoNgTray[i][j] == 8) m_grdNgTray.Set_CellBackClr(i, j, RGB(0x80, 0x40, 0x00));	// 8 HREP
				else if (gData.InfoNgTray[i][j] == 9) m_grdNgTray.Set_CellBackClr(i, j, RGB(0xC0, 0x80, 0x00));	// 9 Chipping
				else								  m_grdNgTray.Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xFF));	// 0 Empty
*/

	return 0;
}

LRESULT CWorkDlg::OnUpdateBarcode_RS232(WPARAM wParam, LPARAM lParam)
{
	CString strTemp;
	CString sData = g_objBarcodeLot_RS232.Get_BarcodeLot();
	if (sData.GetLength() < 1) return 0;

	if (m_rdoWorkStart.GetCheck()) {
		if (gData.nLanguage == 0) AfxMessageBox(_T("장비 Stop 상태에서 진행이 가능합니다....."));
		else					  AfxMessageBox(_T("You can proceed with the equipment stopped."));
		return 0;
	}

	DX_DATA_18 *pDX18 = g_objAJinAXL.Get_pDX18();
	if (pDX18->iMZBufferExist) {
		gData.sMZID[5] = sData;
	}

/*
	if (gData.nSelectNo < 1 || gData.nSelectNo > 6) {
		if (gData.nLanguage == 0) AfxMessageBox(_T("Lot을 먼저 선택후 진행하세요..."));
		else					  AfxMessageBox(_T("Select Lot first and proceed."));
		return 0;
	}

	m_stcLotsIdS[gData.nSelectNo-1].GetWindowText(strTemp);
	if (strTemp.GetLength() > 0) {
		if (gData.nLanguage == 0) AfxMessageBox(_T("기존 Lot ID Clear를 먼저하고 진행하세요..."));
		else					  AfxMessageBox(_T("Please proceed with the existing Lot ID Clear first."));
		return 0;
	}

	//바코드 인식 후 작업 모델이 맞는지 확인.
	if(BarcodeLotID_Check(sData) == FALSE){
		if (gData.nLanguage == 0) g_objCommon.Show_MsgBox(1, "모델 확인 후 다시 입력해 주십시오.....");
		else					  g_objCommon.Show_MsgBox(1, "Please check the model and re-enter...");
		return 0;
	}
	///////////////////////////////////////
	m_stcLotsIdS[gData.nSelectNo-1].SetWindowText(sData);

	//2018.9.11+
	UINT nID = IDC_STC_CMS_COUNT_S_0+gData.nSelectNo-1;
	OnStcCmsCountSClick(nID);
*/
//	g_objSequenceMain.Beep_Post(500);
//	DY_DATA_00 *pDY00 = g_objAJinAXL.Get_pDY00();
//	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	return 0;
}

LRESULT CWorkDlg::OnUpdateBarcode_Cognex(WPARAM wParam, LPARAM lParam)
{
	int nBarcodeNo = (int) wParam;		// 1:Load C/V 2F, 2:Load C/V 1F, 3:Unload C/V 2F, 4:Unload C/V 1F
	int nPortNo = (int)lParam;
	if (nBarcodeNo < 1 || nBarcodeNo > 4) return 0;
	if (nPortNo < 1 || nPortNo > 30) return 0;

	CString strData = g_objBarcodeLot_Cognex.Get_BarcodeLot(nBarcodeNo);
	if (strData.GetLength() < 1) return 0;

	m_stcLotsIdS[nPortNo-1].SetWindowText(strData);

	return 0;
}

LRESULT CWorkDlg::OnResetCycleStop(WPARAM wParam, LPARAM lParam)
{
	gData.bCycleStop = FALSE;
	m_chkCycleStop.SetCheck(FALSE);
	m_chkCycleStop.Set_Color(RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xF0, 0xF0));
	if (wParam == 1) g_objSequenceInit.Set_InitComplete(FALSE);	// 초기화 필요

	return 0;
}

LRESULT CWorkDlg::OnUpdateUph(WPARAM wParam, LPARAM lParam)
{
	int nStartNo = gLot.nJobNo - 1;
	int nDispyNo = 0;
	CString sTxt;
	if (nStartNo >= 0) {
		for(int i=nStartNo; i>=0; i--) {
			nDispyNo++; if (nDispyNo > 99) return 0;
			for(int j=0; j<9; j++) {
				if (gLot.nJobPortNo[i] > 0) {
					if (j == 0) sTxt.Format("%d", gLot.nJobPortNo[i]);
					if (j == 1) sTxt.Format("%s", gLot.sJobLotID[i]);
					if (j == 2) sTxt.Format("%d", gLot.nJobCmCount[i]);
					if (j == 3) sTxt.Format("%d", gLot.nJobGoodCount[i]);
					if (j == 4) sTxt.Format("%d", gLot.nJobNgCount[i]);
					if (j == 5) sTxt.Format("%0.2lf", gLot.dJobRate[i]);
					if (j == 6) sTxt.Format("%s", gLot.sJobStartTime[i]);
					if (j == 7) sTxt.Format("%s", gLot.sJobEndTime[i]);
					if (j == 8) sTxt.Format("%0.5lf", gLot.dJobTack[i]);
				} else sTxt = "";

				m_grdJob.Set_CellText(nDispyNo, j, sTxt);
			}
		}
	}
	for(int i=9; i>=nStartNo; i--) {
		nDispyNo++; if (nDispyNo > 99) return 0;
		for(int j=0; j<9; j++) {
			if (gLot.nJobPortNo[i] > 0) {
				if (j == 0) sTxt.Format("%d", gLot.nJobPortNo[i]);
				if (j == 1) sTxt.Format("%s", gLot.sJobLotID[i]);
				if (j == 2) sTxt.Format("%d", gLot.nJobCmCount[i]);
				if (j == 3) sTxt.Format("%d", gLot.nJobGoodCount[i]);
				if (j == 4) sTxt.Format("%d", gLot.nJobNgCount[i]);
				if (j == 5) sTxt.Format("%0.2lf", gLot.dJobRate[i]);
				if (j == 6) sTxt.Format("%s", gLot.sJobStartTime[i]);
				if (j == 7) sTxt.Format("%s", gLot.sJobEndTime[i]);
				if (j == 8) sTxt.Format("%0.5lf", gLot.dJobTack[i]);
			} else sTxt = "";

			m_grdJob.Set_CellText(nDispyNo, j, sTxt);
		}
	}

	return 0;
}

LRESULT CWorkDlg::OnJobComplete(WPARAM wParam, LPARAM lParam)
{
//	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	DY_DATA_04 *pDY04 = g_objAJinAXL.Get_pDY04();
	pDY04->oBTMIonizerOn = FALSE;
	pDY04->oBTMIonizerBlow = FALSE;
	pDY04->oBTMSuctionOn = FALSE;

	pDY04->oTOPIonizerOn = FALSE;
	pDY04->oTOPIonizerBlow = FALSE;
	pDY04->oTOPSuctionOn = FALSE;
	g_objAJinAXL.Write_Output(4);

	for (int i=0; i<28; i++) {
		m_stcLotsIdS[i].SetWindowText("");
		m_stcCmsCountS[i].SetWindowText("");
		gLot.nCmCount[i] = gLot.nGoodCount[i] = gLot.nNgCount[i] = gLot.nLotStatus[i] = gLot.nSkipCount[i] = 0;
	}
	g_objSequenceMain.Beep_Post(1000);

	if (gData.nLanguage == 0) g_objCommon.Show_MsgBox(1, "Job 완료.");
	else					  g_objCommon.Show_MsgBox(1, "Job complete.");
	g_objMesAgent.Set_EquipState(2);	//Idle
	g_objSequenceMain.Set_MainRunCase(AUTO_ELEVATOR_2, 0);

	CIniFileCS INI_EQP(gsCurrentDir + "\\System\\EquipData.ini");
	if (INI_EQP.Check_File()) {
		INI_EQP.Set_Integer("DAY_TOTAL", "CARRIER_COUNT", gData.nCarrierCount);
	}

	return 0;
}

LRESULT CWorkDlg::OnShowLotEndMsg(WPARAM wParam, LPARAM lParam)
{
	CString sMsg;
	int nNo = wParam - 1;
	if (nNo >= 0) {
		m_stcLotsIdS[nNo].SetWindowText("");
		m_stcCmsCountS[nNo].SetWindowText("");
		g_objCommon.Set_LotDataClear(nNo);
	}
/*
	int nNo2 = lParam;
	if(nNo2 == 0) {
//		g_objSequenceMain.Beep_Post(1000);
		if (gData.nLanguage == 0) sMsg.Format("%d번 Port Lot이 끝났습니다.", nNo+1);
		else					  sMsg.Format("No%d Port Lot is complete.", nNo+1);
		g_objCommon.Show_MsgBox(1, sMsg);
	}
	if(nNo2 == 9) {
		if (gData.nLanguage == 0) sMsg.Format("Reject %dea 이상 검출!!!\nTag 부착후 검사자\n인계 바랍니다.(Reject=%d)", gData.nRejectMaxCount, gData.nRejectLotCount);
		else					  sMsg.Format("More than %d reject detected!!!\nPlease attach the tag and hand it\nover to the inspector.(Reject=%d)", gData.nRejectMaxCount, gData.nRejectLotCount);
		g_objCommon.Show_MsgBox(9, sMsg);
	}
*/
	return 0;
/*
	//g_dlgWork.PostMessage(UM_LOT_END_MSG, NULL, NULL);
	if(wParam == 1) {
		if (gData.nLanguage == 0) g_objCommon.Show_MsgBox(1, "Job이 끝나고 초기화 중 입니다.\n부저 및 완료 메세지가 뜰때까지 기다려 주세요.");
		else					  g_objCommon.Show_MsgBox(1, "Job is finished and initializing.\nPlease wait for the buzzer and completion message to appear.");
	}
	if(wParam == 2) {
		if (gData.nLanguage == 0) g_objCommon.Show_Alarm("Lot이 끝났습니다.\n입력수량과 검사수량이 맞지 않습니다.\n확인해 주세요");
		else					  g_objCommon.Show_Alarm("Lot is complete.\nThe input quantity and the inspection quantity do not match.\nPlease check.");
	}
	if(wParam == 9) {
		if (gData.nLanguage == 0) sMsg.Format("Reject %dea 이상 검출!!!\nTag 부착후 검사자\n인계 바랍니다.(Reject=%d)", gData.nRejectMaxCount, gData.nRejectLotCount);
		else					  sMsg.Format("More than %d reject detected!!!\nPlease attach the tag and hand it\nover to the inspector.(Reject=%d)", gData.nRejectMaxCount, gData.nRejectLotCount);
		g_objCommon.Show_MsgBox(9, sMsg);
	} else {
		if (gData.nLanguage == 0) g_objCommon.Show_MsgBox(1, "Lot이 끝났습니다.\nClear 후 꺼내 주세요.");
		else					  g_objCommon.Show_MsgBox(1, "Lot is complete.\nClear and take it out.");
	}
*/
}

void CWorkDlg::OnBnClickedButton1()
{
	DY_DATA_14 *pDY14 = g_objAJinAXL.Get_pDY14();

	if (pDY14->oInsideLight)	pDY14->oInsideLight = FALSE;
	else						pDY14->oInsideLight = TRUE;
	g_objAJinAXL.Write_Output(14);
}

///////////////////////////////////////////////////////////////////////////////
void CWorkDlg::ReadJobList()
{
	CIniFileCS INI(gsCurrentDir + "\\System\\JobList.ini");
	if (!INI.Check_File()) {
		//AfxMessageBox("JobList.ini File Not Found!!!");
		return;
	}

	CString strSection;
	for (int i=0; i<6; i++) {
		strSection.Format("LOT_%d", i);
		m_sJobListLotID[i]  = INI.Get_String(strSection, "ID", "");
		m_sJobListLotCnt[i] = INI.Get_String(strSection, "CNT", "");
		m_sJobListTryCnt[i] = INI.Get_String(strSection, "TRY", "");

		 m_stcLotsIdS[i].SetWindowText(m_sJobListLotID[i]);
		 m_stcCmsCountS[i].SetWindowText(m_sJobListLotCnt[i]);
	}
}

void CWorkDlg::SaveJobList()
{
	CIniFileCS INI(gsCurrentDir + "\\System\\JobList.ini");
	if (!INI.Check_File()) {
		//AfxMessageBox("JobList.ini File Not Found!!!");
		return;
	}

	CString strSection;
	for (int i=0; i<6; i++) {
		m_stcLotsIdS[i].GetWindowText(m_sJobListLotID[i]);
		m_stcCmsCountS[i].GetWindowText(m_sJobListLotCnt[i]);

		strSection.Format("LOT_%d", i);
		INI.Set_String(strSection, "ID", m_sJobListLotID[i]);
		INI.Set_String(strSection, "CNT", m_sJobListLotCnt[i]);
		INI.Set_String(strSection, "TRY", m_sJobListTryCnt[i]);
	}
}

//바코드를 찍었을때 LotID가 설비 모델과 비교해준다.
BOOL CWorkDlg::BarcodeLotID_Check(CString sLotID)
{
	CString sPath, strKey;
	CString sData; // ini 파일에 있는 모델별 데이터

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CIniFileCS INI(gsCurrentDir + "\\System\\BarcodeData.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("EquipData.ini File Not Found!!!");
		return FALSE;
	}

	strKey = pEquipData->sModelName;
	
	sData = INI.Get_String(strKey, "DATA", "");

	if(sData == "") return TRUE;	// 데이터가 없으면 무조건 입력할수있게 해준다.
	if(sLotID.Find(sData) == 2) return TRUE; //같은 문자열 시작이 2이면 같은 모델임.(GSXXXX, XXXX가 모델별로 정해진 데이터)

	return FALSE;	//그외 나머지는 다른 모델인걸로 간주.
}

void CWorkDlg::OnBnClickedBtnLotCancel()
{
	if (m_rdoWorkStart.GetCheck()) {
		if (gData.nLanguage == 0) AfxMessageBox(_T("장비 Stop 상태에서 진행이 가능합니다....."));
		else					  AfxMessageBox(_T("You can proceed with the equipment stopped."));
		return;
	}

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseMES) {
		if (gData.nLanguage == 0) AfxMessageBox(_T("MES 사용 설정후 진행이 가능합니다....."));
		else					  AfxMessageBox(_T("Unable to proceed while the mes is not used."));
		return;
	}

	if (gData.nLanguage == 0) {
		if (g_objCommon.Show_MsgBox(2, "Lot Cancel를 하시겠습니까?") != IDOK) return;
	} else {
		if (g_objCommon.Show_MsgBox(2, "Are you sure you want to Lot Cancel?") != IDOK) return;
	}

	int nCount = 0;
	for (int i=0; i<30; i++) {
		if (gLot.sLotID[i].GetLength() > 0 && gLot.nCmCount[i] > 0) {
//			g_objMesAgent.Set_LotCancel(gLot.sLotID[i], "O");
			nCount++;
		}
	}
	if (nCount == 0) {
		if (gData.nLanguage == 0) AfxMessageBox(_T("취소할 Lot이 없습니다....."));
		else					  AfxMessageBox(_T("There is no Lot to cancel.."));
		return;
	}

	CString sLog;
	if (gData.nLanguage == 0) sLog.Format("Lot Cancel OK...  => Lot 취소를 MES에 전송하였습니다.");
	else					  sLog.Format("Lot Cancel OK...  => Lot cancel has been send to MES.");
	AfxMessageBox(_T(sLog));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWorkDlg::ConveyorOpen(int nCyNo, int nCmd)
{
	CString sLog;

	DY_DATA_20 *pDY20 = g_objAJinAXL.Get_pDY20();
	if (nCyNo == 1) {
		if (nCmd == 1) {	//Open
			pDY20->oLDMZOpenLamp = TRUE;
			pDY20->oLDMZRunLamp = FALSE;
			pDY20->oDoor01Unlock = TRUE;
			pDY20->oDoor23Unlock = TRUE;
			gData.nMZDoorOpen[0] = 1;
		} else {
			pDY20->oLDMZOpenLamp = FALSE;
			pDY20->oLDMZRunLamp = TRUE;
			pDY20->oDoor01Unlock = FALSE;
			pDY20->oDoor23Unlock = FALSE;
			gData.nMZDoorOpen[0] = 0;
		}
	}
	if (nCyNo == 2) {
		if (nCmd == 1) {	//Open
			pDY20->oULMZOpenLamp = TRUE;
			pDY20->oULMZRunLamp = FALSE;
			pDY20->oDoor05Unlock = TRUE;
			pDY20->oDoor22Unlock = TRUE;
			gData.nMZDoorOpen[1] = 1;
		} else {
			pDY20->oULMZOpenLamp = FALSE;
			pDY20->oULMZRunLamp = TRUE;
			pDY20->oDoor05Unlock = FALSE;
			pDY20->oDoor22Unlock = FALSE;
			gData.nMZDoorOpen[1] = 0;
		}
	}
	g_objAJinAXL.Write_Output(20);
			
	sLog.Format("[Work Mode] ConveyorOpen button push. [%d-%d] [%d-%d]", nCyNo, nCmd, gData.nMZDoorOpen[0], gData.nMZDoorOpen[1]);
	g_objLogFile.Save_HandlerLog(sLog);
}

void CWorkDlg::UnloadConveyorStop()
{
	DY_DATA_19 *pDY19 = g_objAJinAXL.Get_pDY19();
	if (pDY19->oULCV3CCW1F || pDY19->oULCV3CW1F || pDY19->oULCV4CCW1F || pDY19->oULCV4CW1F) {
		pDY19->oULCV3CCW1F = FALSE;   pDY19->oULCV3CW1F = FALSE;
		pDY19->oULCV4CCW1F = FALSE;   pDY19->oULCV4CW1F = FALSE;
		g_objAJinAXL.Write_Output(19);
	}
	if (pDY19->oULCV1CCW2F || pDY19->oULCV1CW2F || pDY19->oULCV2CCW2F || pDY19->oULCV2CW2F) {
		pDY19->oULCV1CCW2F = FALSE;   pDY19->oULCV1CW2F = FALSE;
		pDY19->oULCV2CCW2F = FALSE;   pDY19->oULCV2CW2F = FALSE;
		g_objAJinAXL.Write_Output(19);
	}
}

void CWorkDlg::ElevatorOpen(int nEleNo)
{
	CString sLog;
	static DWORD dwSwitchOnTime[7] = {0,};

	if (dwSwitchOnTime[nEleNo-1] > 0 && (GetTickCount() - dwSwitchOnTime[nEleNo-1]) < 1000) return;
	dwSwitchOnTime[nEleNo-1] = GetTickCount();

	if (gData.nElevatorOpen[nEleNo] == 0) {
			if (g_objSequenceInit.Get_InitComplete()==FALSE) {
				g_objCommon.Show_MsgBox(1, "장비 초기화후 진행하세요....."); return;
			}
//			if (g_objCommon.Show_MsgBox(2, "Elevator Door를 Open 하시겠습니까?") != IDOK) return;

			//1(LS1),2(LS2),3(L1),4(L2),5(EN),6(EG),7(NB),8(U1),9(U2),10(NG1),11(NG2),12(Good1),13(Good2)
			//if ((gData.nTransferX1Pos-2) == nEleNo) { g_objCommon.Show_MsgBox(1, "Transfer 1 Elevator 작업중입니다.  작업후 진행하세요."); return; }
			//if ((gData.nTransferX2Pos-2) == nEleNo) { g_objCommon.Show_MsgBox(1, "Transfer 2 Elevator 작업중입니다.  작업후 진행하세요."); return; }
//			if (!g_objCommon.Check_Position(AX_TRANSFER_Z1, 0)) { g_objSequenceMain.Beep_Post(500); g_objCommon.Show_MsgBox(1, "Transfer Z1 Up상태에서 작업가능합니다.."); return; }
//			if (!g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) { g_objSequenceMain.Beep_Post(500); g_objCommon.Show_MsgBox(1, "Transfer Z2 Up상태에서 작업가능합니다.."); return; }

			gData.nElevatorOpen[nEleNo] = 1;
			if (nEleNo == 1) { g_objSequenceMain.Set_MainRunCase(AUTO_ELEVATOR_1, 51); if (m_nWorkEleatorCase1==0) m_nWorkEleatorCase1 = 1; }
			if (nEleNo == 2) { g_objSequenceMain.Set_MainRunCase(AUTO_ELEVATOR_2, 51); if (m_nWorkEleatorCase2==0) m_nWorkEleatorCase2 = 1; }
			if (nEleNo == 3) { g_objSequenceMain.Set_MainRunCase(AUTO_ELEVATOR_3, 51); if (m_nWorkEleatorCase3==0) m_nWorkEleatorCase3 = 1; }
			if (nEleNo == 4) { g_objSequenceMain.Set_MainRunCase(AUTO_ELEVATOR_4, 51); if (m_nWorkEleatorCase4==0) m_nWorkEleatorCase4 = 1; }
			if (nEleNo == 5) { g_objSequenceMain.Set_MainRunCase(AUTO_ELEVATOR_5, 51); if (m_nWorkEleatorCase5==0) m_nWorkEleatorCase5 = 1; }
			if (nEleNo == 6) { g_objSequenceMain.Set_MainRunCase(AUTO_ELEVATOR_6, 51); if (m_nWorkEleatorCase6==0) m_nWorkEleatorCase6 = 1; }
			if (nEleNo == 7) { g_objSequenceMain.Set_MainRunCase(AUTO_ELEVATOR_7, 51); if (m_nWorkEleatorCase7==0) m_nWorkEleatorCase7 = 1; }

	} else if (gData.nElevatorOpen[nEleNo] == 1) {
//			gData.nElevatorOpen[nEleNo] = 0;
//			g_objCommon.Show_MsgBox(1, "Elevator Open중입니다.  기다려 주세요."); return;
			return;
	} else if (gData.nElevatorOpen[nEleNo] == 2) {
			g_objCommon.Locking_TrayDoor(TRUE, nEleNo);
			g_objCommon.Locking_Slide(TRUE, nEleNo);
			gData.nElevatorOpen[nEleNo] = 0;

			if (nEleNo == 1) { g_objAJinAXL.Move_Absolute(AX_ELEVATOR_Z1, gData.dElevatorDown); m_nWorkEleatorCase1 = 0; }
			if (nEleNo == 2) { g_objAJinAXL.Move_Absolute(AX_ELEVATOR_Z2, gData.dElevatorDown); m_nWorkEleatorCase2 = 0; }
			if (nEleNo == 3) { g_objAJinAXL.Move_Absolute(AX_ELEVATOR_Z3, gData.dElevatorDown); m_nWorkEleatorCase3 = 0; }
			if (nEleNo == 4) { g_objAJinAXL.Move_Absolute(AX_ELEVATOR_Z4, gData.dElevatorDown); m_nWorkEleatorCase4 = 0; }
			if (nEleNo == 5) { g_objAJinAXL.Move_Absolute(AX_ELEVATOR_Z5, gData.dElevatorDown); m_nWorkEleatorCase5 = 0; }
			if (nEleNo == 6) { g_objAJinAXL.Move_Absolute(AX_ELEVATOR_Z6, gData.dElevatorDown); m_nWorkEleatorCase6 = 0; }
			if (nEleNo == 7) { g_objAJinAXL.Move_Absolute(AX_ELEVATOR_Z7, gData.dElevatorDown); m_nWorkEleatorCase7 = 0; }

			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13(); Sleep(500);
// 			if (nEleNo == 1) { pDY13->oElevator1Lamp = FALSE; }
// 			if (nEleNo == 2) { pDY13->oElevator2Lamp = FALSE; }
// 			if (nEleNo == 3) { pDY13->oElevator3Lamp = FALSE; }
// 			if (nEleNo == 4) { pDY13->oElevator4Lamp = FALSE; }
// 			if (nEleNo == 5) { pDY13->oElevator5Lamp = FALSE; }
// 			if (nEleNo == 6) { pDY13->oElevator6Lamp = FALSE; }
// 			if (nEleNo == 7) { pDY13->oElevator7Lamp = FALSE; }
			g_objAJinAXL.Write_Output(13);
	}

	sLog.Format("[Work Mode] ElevatorOpen button push. [%d-%d]", nEleNo, gData.nElevatorOpen[nEleNo]);
	g_objLogFile.Save_HandlerLog(sLog);
}

void CWorkDlg::Begin_ElevatorRunThread()
{
	if (m_pThreadElevatorRun) return;
	m_bThreadElevatorRun = TRUE;
	m_pThreadElevatorRun = AfxBeginThread(Thread_ElevatorRun, NULL);
}

void CWorkDlg::End_ElevatorRunThread()
{
	if (m_pThreadElevatorRun) {
		m_bThreadElevatorRun = FALSE;
		WaitForSingleObject(m_pThreadElevatorRun->m_hThread, INFINITE);
	}
	UnloadConveyorStop();
}

UINT CWorkDlg::Thread_ElevatorRun(LPVOID lpVoid)
{
	while (g_dlgWork.m_bThreadElevatorRun) {

		if (!g_dlgWork.ElevatorOpen1()) break;
		if (!g_dlgWork.ElevatorOpen2()) break;
		if (!g_dlgWork.ElevatorOpen3()) break;
		if (!g_dlgWork.ElevatorOpen4()) break;
		if (!g_dlgWork.ElevatorOpen5()) break;
		if (!g_dlgWork.ElevatorOpen6()) break;
		if (!g_dlgWork.ElevatorOpen7()) break;
		if (!g_dlgWork.UnloadConveyor1F()) break;
		if (!g_dlgWork.UnloadConveyor2F()) break;

		Sleep(10);
	}

	g_dlgWork.m_bThreadElevatorRun = FALSE;
	g_dlgWork.m_pThreadElevatorRun = NULL;

	return 0;
}

BOOL CWorkDlg::ElevatorOpen1()
{
	int nElevNo = 1;
	int nMotionNo = AX_ELEVATOR_Z1;
	static DWORD dwSwitchTime1 = 0;

	if (gData.nElevatorOpen[nElevNo] != 1) return TRUE;
	if (m_nWorkEleatorCase1 > 1 && m_nWorkEleatorCase1 < 5) {
		if ((GetTickCount() - dwSwitchTime1) >= 400) {
			dwSwitchTime1 = GetTickCount();
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			if (pDY13->oElevator1Lamp == TRUE)	pDY13->oElevator1Lamp = FALSE;
// 			else								pDY13->oElevator1Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);
		}
	}

	switch (m_nWorkEleatorCase1) {
	case 0:		// Wait
		m_tWorkElevatorLoop1.Set_LoopTime(5000);
		return TRUE;

	case 1:
		dwSwitchTime1 = GetTickCount();
		g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen1 Start");
		m_nWorkEleatorCase1++; m_tWorkElevatorLoop1.Set_LoopTime(5000);
		break;

	case 2:
		g_objAJinAXL.Stop_Motion(nMotionNo);
		m_nWorkEleatorCase1++; m_tWorkElevatorLoop1.Set_LoopTime(5000);
		break;
	case 3:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			g_objAJinAXL.Move_Absolute(nMotionNo, gData.dElevatorDown);
			m_nWorkEleatorCase1++; m_tWorkElevatorLoop1.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			m_nWorkEleatorCase1++; m_tWorkElevatorLoop1.Set_LoopTime(30000);
		}
		break;
	case 5:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			pDY13->oElevator1Lamp = TRUE;
// 			g_objAJinAXL.Write_Output(13);

			g_objCommon.Locking_TrayDoor(FALSE, nElevNo);
			g_objCommon.Locking_Slide(FALSE, nElevNo);
			gData.nElevatorOpen[nElevNo] = 2;

			m_nWorkEleatorCase1 = 0; m_tWorkElevatorLoop1.Set_LoopTime(30000);
			g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen1 End");
		}
		break;
	}
	return TRUE;
}
BOOL CWorkDlg::ElevatorOpen2()
{
	int nElevNo = 2;
	int nMotionNo = AX_ELEVATOR_Z2;
	static DWORD dwSwitchTime2 = 0;

	if (gData.nElevatorOpen[nElevNo] != 1) return TRUE;
	if (m_nWorkEleatorCase2 > 1 && m_nWorkEleatorCase2 < 5) {
		if ((GetTickCount() - dwSwitchTime2) >= 400) {
			dwSwitchTime2 = GetTickCount();
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			if (pDY13->oElevator2Lamp == TRUE)	pDY13->oElevator2Lamp = FALSE;
// 			else								pDY13->oElevator2Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);
		}
	}

	switch (m_nWorkEleatorCase2) {
	case 0:		// Wait
		m_tWorkElevatorLoop2.Set_LoopTime(5000);
		return TRUE;

	case 1:
		dwSwitchTime2 = GetTickCount();
		g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen2 Start");
		m_nWorkEleatorCase2++; m_tWorkElevatorLoop2.Set_LoopTime(5000);
		break;

	case 2:
		g_objAJinAXL.Stop_Motion(nMotionNo);
		m_nWorkEleatorCase2++; m_tWorkElevatorLoop2.Set_LoopTime(5000);
		break;
	case 3:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			g_objAJinAXL.Move_Absolute(nMotionNo, gData.dElevatorDown);
			m_nWorkEleatorCase2++; m_tWorkElevatorLoop2.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			m_nWorkEleatorCase2++; m_tWorkElevatorLoop2.Set_LoopTime(30000);
		}
		break;
	case 5:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			pDY13->oElevator2Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);

			g_objCommon.Locking_TrayDoor(FALSE, nElevNo);
			g_objCommon.Locking_Slide(FALSE, nElevNo);
			gData.nElevatorOpen[nElevNo] = 2;

			m_nWorkEleatorCase2 = 0; m_tWorkElevatorLoop2.Set_LoopTime(30000);
			g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen2 End");
		}
		break;
	}
	return TRUE;
}
BOOL CWorkDlg::ElevatorOpen3()
{
	int nElevNo = 3;
	int nMotionNo = AX_ELEVATOR_Z3;
	static DWORD dwSwitchTime3 = 0;

	if (gData.nElevatorOpen[nElevNo] != 1) return TRUE;
	if (m_nWorkEleatorCase3 > 1 && m_nWorkEleatorCase3 < 5) {
		if ((GetTickCount() - dwSwitchTime3) >= 400) {
			dwSwitchTime3 = GetTickCount();
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			if (pDY13->oElevator3Lamp == TRUE)	pDY13->oElevator3Lamp = FALSE;
// 			else								pDY13->oElevator3Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);
		}
	}

	switch (m_nWorkEleatorCase3) {
	case 0:		// Wait
		m_tWorkElevatorLoop3.Set_LoopTime(5000);
		return TRUE;

	case 1:
		dwSwitchTime3 = GetTickCount();
		g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen3 Start");
		m_nWorkEleatorCase3++; m_tWorkElevatorLoop3.Set_LoopTime(5000);
		break;

	case 2:
		g_objAJinAXL.Stop_Motion(nMotionNo);
		m_nWorkEleatorCase3++; m_tWorkElevatorLoop3.Set_LoopTime(5000);
		break;
	case 3:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			g_objAJinAXL.Move_Absolute(nMotionNo, gData.dElevatorDown);
			m_nWorkEleatorCase3++; m_tWorkElevatorLoop3.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			m_nWorkEleatorCase3++; m_tWorkElevatorLoop3.Set_LoopTime(30000);
		}
		break;
	case 5:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			pDY13->oElevator3Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);

			g_objCommon.Locking_TrayDoor(FALSE, nElevNo);
			g_objCommon.Locking_Slide(FALSE, nElevNo);
			gData.nElevatorOpen[nElevNo] = 2;

			m_nWorkEleatorCase3 = 0; m_tWorkElevatorLoop3.Set_LoopTime(30000);
			g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen3 End");
		}
		break;
	}
	return TRUE;
}
BOOL CWorkDlg::ElevatorOpen4()
{
	int nElevNo = 4;
	int nMotionNo = AX_ELEVATOR_Z4;
	static DWORD dwSwitchTime4 = 0;

	if (gData.nElevatorOpen[nElevNo] != 1) return TRUE;
	if (m_nWorkEleatorCase4 > 1 && m_nWorkEleatorCase4 < 5) {
		if ((GetTickCount() - dwSwitchTime4) >= 400) {
			dwSwitchTime4 = GetTickCount();
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			if (pDY13->oElevator4Lamp == TRUE)	pDY13->oElevator4Lamp = FALSE;
// 			else								pDY13->oElevator4Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);
		}
	}

	switch (m_nWorkEleatorCase4) {
	case 0:		// Wait
		m_tWorkElevatorLoop4.Set_LoopTime(5000);
		return TRUE;

	case 1:
		dwSwitchTime4 = GetTickCount();
		g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen4 Start");
		m_nWorkEleatorCase4++; m_tWorkElevatorLoop4.Set_LoopTime(5000);
		break;

	case 2:
		g_objAJinAXL.Stop_Motion(nMotionNo);
		m_nWorkEleatorCase4++; m_tWorkElevatorLoop4.Set_LoopTime(5000);
		break;
	case 3:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			g_objAJinAXL.Move_Absolute(nMotionNo, gData.dElevatorDown);
			m_nWorkEleatorCase4++; m_tWorkElevatorLoop4.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			m_nWorkEleatorCase4++; m_tWorkElevatorLoop4.Set_LoopTime(30000);
		}
		break;
	case 5:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			pDY13->oElevator4Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);

			g_objCommon.Locking_TrayDoor(FALSE, nElevNo);
			g_objCommon.Locking_Slide(FALSE, nElevNo);
			gData.nElevatorOpen[nElevNo] = 2;

			m_nWorkEleatorCase4 = 0; m_tWorkElevatorLoop4.Set_LoopTime(30000);
			g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen4 End");
		}
		break;
	}
	return TRUE;
}
BOOL CWorkDlg::ElevatorOpen5()
{
	int nElevNo = 5;
	int nMotionNo = AX_ELEVATOR_Z5;
	static DWORD dwSwitchTime5 = 0;

	if (gData.nElevatorOpen[nElevNo] != 1) return TRUE;
	if (m_nWorkEleatorCase5 > 1 && m_nWorkEleatorCase5 < 5) {
		if ((GetTickCount() - dwSwitchTime5) >= 400) {
			dwSwitchTime5 = GetTickCount();
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			if (pDY13->oElevator5Lamp == TRUE)	pDY13->oElevator5Lamp = FALSE;
// 			else								pDY13->oElevator5Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);
		}
	}

	switch (m_nWorkEleatorCase5) {
	case 0:		// Wait
		m_tWorkElevatorLoop5.Set_LoopTime(5000);
		return TRUE;

	case 1:
		dwSwitchTime5 = GetTickCount();
		g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen5 Start");
		m_nWorkEleatorCase5++; m_tWorkElevatorLoop5.Set_LoopTime(5000);
		break;

	case 2:
		g_objAJinAXL.Stop_Motion(nMotionNo);
		m_nWorkEleatorCase5++; m_tWorkElevatorLoop5.Set_LoopTime(5000);
		break;
	case 3:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			g_objAJinAXL.Move_Absolute(nMotionNo, gData.dElevatorDown);
			m_nWorkEleatorCase5++; m_tWorkElevatorLoop5.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			m_nWorkEleatorCase5++; m_tWorkElevatorLoop5.Set_LoopTime(30000);
		}
		break;
	case 5:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			pDY13->oElevator5Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);

			g_objCommon.Locking_TrayDoor(FALSE, nElevNo);
			g_objCommon.Locking_Slide(FALSE, nElevNo);
			gData.nElevatorOpen[nElevNo] = 2;

			m_nWorkEleatorCase5 = 0; m_tWorkElevatorLoop5.Set_LoopTime(30000);
			g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen5 End");
		}
		break;
	}
	return TRUE;
}
BOOL CWorkDlg::ElevatorOpen6()
{
	int nElevNo = 6;
	int nMotionNo = AX_ELEVATOR_Z6;
	static DWORD dwSwitchTime6 = 0;

	if (gData.nElevatorOpen[nElevNo] != 1) return TRUE;
	if (m_nWorkEleatorCase6 > 1 && m_nWorkEleatorCase6 < 5) {
		if ((GetTickCount() - dwSwitchTime6) >= 400) {
			dwSwitchTime6 = GetTickCount();
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			if (pDY13->oElevator6Lamp == TRUE)	pDY13->oElevator6Lamp = FALSE;
// 			else								pDY13->oElevator6Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);
		}
	}

	switch (m_nWorkEleatorCase6) {
	case 0:		// Wait
		m_tWorkElevatorLoop6.Set_LoopTime(5000);
		return TRUE;

	case 1:
		dwSwitchTime6 = GetTickCount();
		g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen6 Start");
		m_nWorkEleatorCase6++; m_tWorkElevatorLoop6.Set_LoopTime(5000);
		break;

	case 2:
		g_objAJinAXL.Stop_Motion(nMotionNo);
		m_nWorkEleatorCase6++; m_tWorkElevatorLoop6.Set_LoopTime(5000);
		break;
	case 3:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			g_objAJinAXL.Move_Absolute(nMotionNo, gData.dElevatorDown);
			m_nWorkEleatorCase6++; m_tWorkElevatorLoop6.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			m_nWorkEleatorCase6++; m_tWorkElevatorLoop6.Set_LoopTime(30000);
		}
		break;
	case 5:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			pDY13->oElevator6Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);

			g_objCommon.Locking_TrayDoor(FALSE, nElevNo);
			g_objCommon.Locking_Slide(FALSE, nElevNo);
			gData.nElevatorOpen[nElevNo] = 2;

			m_nWorkEleatorCase6 = 0; m_tWorkElevatorLoop6.Set_LoopTime(30000);
			g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen6 End");
		}
		break;
	}
	return TRUE;
}
BOOL CWorkDlg::ElevatorOpen7()
{
	int nElevNo = 7;
	int nMotionNo = AX_ELEVATOR_Z7;
	static DWORD dwSwitchTime7 = 0;

	if (gData.nElevatorOpen[nElevNo] != 1) return TRUE;
	if (m_nWorkEleatorCase7 > 1 && m_nWorkEleatorCase7 < 5) {
		if ((GetTickCount() - dwSwitchTime7) >= 400) {
			dwSwitchTime7 = GetTickCount();
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			if (pDY13->oElevator7Lamp == TRUE)	pDY13->oElevator7Lamp = FALSE;
// 			else								pDY13->oElevator7Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);
		}
	}

	switch (m_nWorkEleatorCase7) {
	case 0:		// Wait
		m_tWorkElevatorLoop7.Set_LoopTime(5000);
		return TRUE;

	case 1:
		dwSwitchTime7 = GetTickCount();
		g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen7 Start");
		m_nWorkEleatorCase7++; m_tWorkElevatorLoop7.Set_LoopTime(5000);
		break;

	case 2:
		g_objAJinAXL.Stop_Motion(nMotionNo);
		m_nWorkEleatorCase7++; m_tWorkElevatorLoop7.Set_LoopTime(5000);
		break;
	case 3:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			g_objAJinAXL.Move_Absolute(nMotionNo, gData.dElevatorDown);
			m_nWorkEleatorCase7++; m_tWorkElevatorLoop7.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			m_nWorkEleatorCase7++; m_tWorkElevatorLoop7.Set_LoopTime(30000);
		}
		break;
	case 5:
		if (g_objAJinAXL.Is_Done(nMotionNo)) {
			DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
// 			pDY13->oElevator7Lamp = TRUE;
			g_objAJinAXL.Write_Output(13);

			g_objCommon.Locking_TrayDoor(FALSE, nElevNo);
			g_objCommon.Locking_Slide(FALSE, nElevNo);
			gData.nElevatorOpen[nElevNo] = 2;

			m_nWorkEleatorCase7 = 0; m_tWorkElevatorLoop7.Set_LoopTime(30000);
			g_objLogFile.Save_HandlerLog("[Work Elevator] - ElevatorOpen7 End");
		}
		break;
	}

	return TRUE;
}

BOOL CWorkDlg::UnloadConveyor1F()
{
	DX_DATA_19 *pDX19 = g_objAJinAXL.Get_pDX19();
	DY_DATA_19 *pDY19 = g_objAJinAXL.Get_pDY19();
	int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_ULCV_ELEVATOR);
	int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_LDUP_CONVEYOR);
	int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_UNLOAD_CONVEYOR);
	if (nCase1 > 0 || nCase2 > 0 || nCase3 > 0) {
		if (m_nWorkUnloadConveyor1F > 0) {
			m_nWorkUnloadConveyor1F = 0;
			UnloadConveyorStop();
		}
		return TRUE;
	}

	switch (m_nWorkUnloadConveyor1F) {
	case 0:		// Wait
		if (gData.nMZDoorOpen[1] == 1) {
			if (!pDX19->iULCV1FCnt6 && (pDX19->iULCV1FCnt5 || pDX19->iULCV1FCnt4 ||pDX19->iULCV1FCnt3 ||pDX19->iULCV1FCnt2 ||pDX19->iULCV1FCnt1)) {
				if (!m_tnWorkUnloadConveyor1F.Waiting_Time(2000)) break;
				if (gData.nMZDoorOpen[1] == 1) {
					if (!pDX19->iULCV1FCnt6 && (pDX19->iULCV1FCnt5 || pDX19->iULCV1FCnt4 ||pDX19->iULCV1FCnt3 ||pDX19->iULCV1FCnt2 ||pDX19->iULCV1FCnt1)) {
						m_nWorkUnloadConveyor1F++;
					}
				}
			}
		}
			m_tnWorkUnloadConveyor1F.Set_LoopTime(5000);
		return TRUE;

	case 1:
		pDY19->oULCV3CCW1F = TRUE;   pDY19->oULCV3CW1F = TRUE;
		pDY19->oULCV4CCW1F = TRUE;   pDY19->oULCV4CW1F = TRUE;
		g_objAJinAXL.Write_Output(19);
		m_nWorkUnloadConveyor1F++; m_tnWorkUnloadConveyor1F.Set_LoopTime(5000);
		break;
	case 2:
		if (pDX19->iULCV1FCnt6) {
			pDY19->oULCV3CCW1F = FALSE;   pDY19->oULCV3CW1F = FALSE;
			pDY19->oULCV4CCW1F = FALSE;   pDY19->oULCV4CW1F = FALSE;
			g_objAJinAXL.Write_Output(19);
			m_nWorkUnloadConveyor1F = 0; m_tnWorkUnloadConveyor1F.Set_LoopTime(5000);
		}
		break;
	}

	if (m_tnWorkUnloadConveyor1F.Over_LoopTime()) {
		m_nWorkUnloadConveyor1F = 0 ;
		pDY19->oULCV3CCW1F = FALSE;   pDY19->oULCV3CW1F = FALSE;
		pDY19->oULCV4CCW1F = FALSE;   pDY19->oULCV4CW1F = FALSE;
		g_objAJinAXL.Write_Output(19);
	}
	return TRUE;
}

BOOL CWorkDlg::UnloadConveyor2F()
{
	DX_DATA_19 *pDX19 = g_objAJinAXL.Get_pDX19();
	DY_DATA_19 *pDY19 = g_objAJinAXL.Get_pDY19();
	int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_ULCV_ELEVATOR);
	int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_LDET_CONVEYOR);
	int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_UNLOAD_CONVEYOR);
	if (nCase1 > 0 || nCase2 > 0 || nCase3 > 0) {
		if (m_nWorkUnloadConveyor2F > 0) {
			m_nWorkUnloadConveyor2F = 0;
			UnloadConveyorStop();
		}
		return TRUE;
	}

	switch (m_nWorkUnloadConveyor2F) {
	case 0:		// Wait
		if (gData.nMZDoorOpen[1] == 1) {
			if (!pDX19->iULCV2FCnt6 && (pDX19->iULCV2FCnt5 || pDX19->iULCV2FCnt4 ||pDX19->iULCV2FCnt3 ||pDX19->iULCV2FCnt2 ||pDX19->iULCV2FCnt1)) {
				if (!m_tnWorkUnloadConveyor2F.Waiting_Time(2000)) break;
				if (gData.nMZDoorOpen[1] == 1) {
					if (!pDX19->iULCV2FCnt6 && (pDX19->iULCV2FCnt5 || pDX19->iULCV2FCnt4 ||pDX19->iULCV2FCnt3 ||pDX19->iULCV2FCnt2 ||pDX19->iULCV2FCnt1)) {
						m_nWorkUnloadConveyor2F++;
					}
				}
			}
		}
			m_tnWorkUnloadConveyor2F.Set_LoopTime(5000);
		return TRUE;

	case 1:
		pDY19->oULCV1CCW2F = TRUE;   pDY19->oULCV1CW2F = TRUE;
		pDY19->oULCV2CCW2F = TRUE;   pDY19->oULCV2CW2F = TRUE;
		g_objAJinAXL.Write_Output(19);
		m_nWorkUnloadConveyor2F++; m_tnWorkUnloadConveyor2F.Set_LoopTime(5000);
		break;
	case 2:
		if (pDX19->iULCV2FCnt6) {
			pDY19->oULCV1CCW2F = FALSE;   pDY19->oULCV1CW2F = FALSE;
			pDY19->oULCV2CCW2F = FALSE;   pDY19->oULCV2CW2F = FALSE;
			g_objAJinAXL.Write_Output(19);
			m_nWorkUnloadConveyor2F = 0; m_tnWorkUnloadConveyor2F.Set_LoopTime(5000);
		}
		break;
	}

	if (m_tnWorkUnloadConveyor2F.Over_LoopTime()) {
		m_nWorkUnloadConveyor2F = 0 ;
		pDY19->oULCV1CCW2F = FALSE;   pDY19->oULCV1CW2F = FALSE;
		pDY19->oULCV2CCW2F = FALSE;   pDY19->oULCV2CW2F = FALSE;
		g_objAJinAXL.Write_Output(19);
	}
	return TRUE;
}

void CWorkDlg::DoEvents(int nSleep)
{
	MSG msg;
	if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (nSleep > 0) Sleep(nSleep);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CWorkDlg::OnBnClickedButton2()
{
	CString str1, str2;
	str1 = "DFAI-13A";
	str2 = "DFAI-13";

	int a = str1.Find(str2);



//	g_dlgWork.PostMessage(UM_LOT_END_MSG, 1, 9);
//	gLot.nJobNo = 0;
//	g_dlgWork.PostMessage(UM_UPDATE_UPH, 0, NULL);
//	g_objSequenceMain.Set_MainRunCase(AUTO_TRANSFER_1, 0);
//	g_objCommon.Show_Error(3501);
	/*
	gLot.nJobNo = 9;
	gLot.nJobPortNo[8] = 1;
	gLot.sJobLotID[8] = "aaa";
	gLot.nJobCmCount[8] = 1;
	gLot.nJobGoodCount[8] = 1;
	gLot.nJobNgCount[8] = 1;
	gLot.dJobRate[8] = 1.1;
	gLot.sJobStartTime[8] = "aaa";
	gLot.sJobEndTime[8] = "bbbb";
	gLot.dJobTack[8] = 1.222;
	
	
	CString sLog;
	for(int i=0; i<10; i++) {
		for(int j=0; j<4; j++) {
			gData.InfoAlignTray[i][j] = gData.InfoLoadTray[i][j] = gData.InfoNgTray[i][j] = gData.InfoGoodTray[i][j] = 0;
		}
	}
	for(int i=0; i<1; i++) {
		gData.InfoAlignTray[i][0] = gData.InfoLoadTray[i][0] = gData.InfoNgTray[i][0] = gData.InfoGoodTray[i][0] = 1;
		gData.InfoAlignTray[i][1] = gData.InfoLoadTray[i][1] = gData.InfoNgTray[i][1] = gData.InfoGoodTray[i][1] = 2;
	}
	for(int i=1; i<2; i++) {
		for(int j=0; j<2; j++) {
			gData.InfoAlignTray[i][j] = gData.InfoLoadTray[i][j] = gData.InfoNgTray[i][j] = gData.InfoGoodTray[i][j] = 2;
		}
	}
	for(int i=2; i<3; i++) {
		for(int j=0; j<2; j++) {
			gData.InfoAlignTray[i][j] = gData.InfoLoadTray[i][j] = gData.InfoNgTray[i][j] = gData.InfoGoodTray[i][j] = 3;
		}
	}
	
//	ElevatorOpen(2);

	for(int i=0; i<4; i++) {
		for(int j=0; j<3; j++) {
			gData.dAlignVisionX[i][j] = 0.1;
			gData.dAlignVisionY[i][j] = 0.1;
		}
	}

	int    nPickerNo, nJobNo, nPosX, nPosY, nModuleNo;
	double dx, dy;
	nPickerNo = 2;	nJobNo    = 1;	nModuleNo = 1;
	nPosX     = 1;	nPosY     = 1;
	g_objSequenceMain.Get_TrayPosition(nPickerNo, nJobNo, nPosX, nPosY, dx, dy);
	dx = dx + (30.0 * (nModuleNo - nPosX));
	sLog.Format("A2 Data [Picker:%d Job:%d Option:%d] PosXY(%d,%d) => (%0.3lf,%0.3lf)", nPickerNo, nJobNo, nModuleNo, nPosX, nPosY, dx, dy);
	g_objLogFile.Save_HandlerLog(sLog);

	nPickerNo = 2;	nJobNo    = 1;	nModuleNo = 1;
	nPosX     = 4;	nPosY     = 1;
	g_objSequenceMain.Get_TrayPosition(nPickerNo, nJobNo, nPosX, nPosY, dx, dy);
	dx = dx + (30.0 * (nModuleNo - nPosX));
	sLog.Format("A2 Data [Picker:%d Job:%d Option:%d] PosXY(%d,%d) => (%0.3lf,%0.3lf)", nPickerNo, nJobNo, nModuleNo, nPosX, nPosY, dx, dy);
	g_objLogFile.Save_HandlerLog(sLog);

	nPickerNo = 2;	nJobNo    = 1;	nModuleNo = 4;
	nPosX     = 4;	nPosY     = 1;
	g_objSequenceMain.Get_TrayPosition(nPickerNo, nJobNo, nPosX, nPosY, dx, dy);
	dx = dx + (30.0 * (nModuleNo - nPosX));
	sLog.Format("A2 Data [Picker:%d Job:%d Option:%d] PosXY(%d,%d) => (%0.3lf,%0.3lf)", nPickerNo, nJobNo, nModuleNo, nPosX, nPosY, dx, dy);
	g_objLogFile.Save_HandlerLog(sLog);

	nPickerNo = 2;	nJobNo    = 1;	nModuleNo = 8;
	nPosX     = 4;	nPosY     = 1;
	g_objSequenceMain.Get_TrayPosition(nPickerNo, nJobNo, nPosX, nPosY, dx, dy);
	dx = dx + (30.0 * (nModuleNo - nPosX));
	sLog.Format("A2 Data [Picker:%d Job:%d Option:%d] PosXY(%d,%d) => (%0.3lf,%0.3lf)", nPickerNo, nJobNo, nModuleNo, nPosX, nPosY, dx, dy);
	g_objLogFile.Save_HandlerLog(sLog);
*/

}

void CWorkDlg::OnBnClickedBtnBuzzerOff()
{	
	g_objCommon.BuzzerOff();

#ifdef AJIN_BOARD_USE
	return;
#endif
/*
	gLot.sLotID[0] = "AAAAA";
	gLot.sBarCode[0][0][0] = "B1234567890";
	gLot.nMarginal[0][0] = 1;
	g_objMesAgent.Set_CmEnd(0, 1, 1, 1, 1, "CAR00001");

	CString sData, sData1, sData2, sData3, sData4, sData5;

	sData1 = "1,2,3,4,5,6,7,8,9,10,";			sData2 = "11,12,13,14,15,16,17,18,19,20,";
	sData3 = "21,22,23,24,25,26,27,28,29,30,";	sData4 = "31,32,33,34,35,36,37,38,39,40,";	sData5 = "41,42,43,44,45,46,47,48,49,50,";
	sData = "RECIPE,BODY,T1M,AAAAA,RecipeID,Recipe Version," + sData1 + sData2 + sData3 + sData4;
	g_objInspector.Test_Command(0, "T1M", sData);

	sData = "RECIPE,BODY,T1S,AAAAA,RecipeID,Recipe Version," + sData1 + sData2 + sData3 + sData4;
	g_objInspector.Test_Command(0, "T1S", sData);

	sData = "RECIPE,BODY,T2M,AAAAA,RecipeID,Recipe Version," + sData1 + sData2 + sData3 + sData4 + sData5;
	g_objInspector.Test_Command(0, "T2M", sData);

	sData = "RECIPE,BODY,T2S,AAAAA,RecipeID,Recipe Version," + sData1 + sData2 + sData3 + sData4 + sData5;
	g_objInspector.Test_Command(0, "T2S", sData);

//	g_objMesAgent.Set_PPSelectReport(gMes.sHostLotID, gMes.sRcpVersion);

/*
	DX_DATA_16 *pDX16 = g_objAJinAXL.Get_pDX16();
	pDX16->iLDCV2FCnt1 = TRUE;
	pDX16->iLDCV1FCnt1 = TRUE;

	int nNo = 0;
	gLot.sMZID_GD[nNo] = "MGZ"; 
	gLot.sCarID_GD[nNo] = "CAR"; 
	gLot.sRecipeName[nNo] = "RECIPE"; 
	gLot.nCmCount[nNo] = 100;
	gLot.nGoodCount[nNo] = 90;
	gLot.nNgCount[nNo] = 10;
	gLot.nRosJugCount[nNo][0] = 10;
	gLot.nRosJugCount[nNo][1] = 11;
	gLot.nRosJugCount[nNo][3] = 13;
	gLot.nRosJugCount[nNo][2] = 12;
	gLot.nRosJugCount[nNo][4] = 14;
	gLot.nRosJugCount[nNo][10] = 20;
	g_objMesAgent.Set_LotEnd(1, "AAAAA", "1", "2", "3", "4", "5");
*/
/*
	CString sMsg = "INSPECT,COMPLETE,T2,GPSAA1BDE2L15GA,13,1,8,N,BS_S_GLUECT1,1,2,BS-S-GLUECT1,FS-S-DA,BS-S-GLUECT1,BS-S-GLUECT1,FS-S-SC,,,,,,,,,,,,,,,";
	CString strArg[11];
	for (int i = 0; i < 11; i++) AfxExtractSubString(strArg[i], sMsg, i + 2, ',');
	g_objInspector.Get_InspectComplete_LG(0, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strArg[5], strArg[6], strArg[7], strArg[8], strArg[9], sMsg);
*/
/*
	g_objCommon.Show_Error(2416);
	for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) gData.dAlignOffset[i][j] = 1.1;

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	double dOffset2 = (!pEquipData->bUseAlignOffset ? 0.0 : (gData.dAlignOffset[1][1] * -1.0));
	g_objInspector.Test_Command(0, "T1", "AAA", "1", "1", "1");
	gCap.sAlmFAIName = "FAI-5LS";
	gCap.dAlmDefect = 0.9;
	gCap.dGiDefect = 0.5;
	gCap.nGiMZCnt = 3;
	gAlm.sAlmLotID[0] = "FAI-5LS. FAI-10LS. FAI-11. FAI-20. ";
	g_objCommon.Show_Error(9199);
*/
//	g_objInspector.Get_ErrorRequest(3, "T1", "AAAAAAAAA", "1", "2", "3", "1234", "Vision Error발생함");
//	g_objInspector.Set_ConnectRequest(INSPECTOR_PC1);
//	g_objInspector.Set_MotionB1(INSPECTOR_PC1, "B1", 1.001);
//	g_objInspector.Set_MotionT1(INSPECTOR_PC2, "T1", 2.002);
//	g_objInspector.Set_MotionT2(INSPECTOR_PC3, "T2", 3.003);


//	g_objSequenceMain.Set_MainRunCase(AUTO_ELEVATOR_2, 51);
//	g_objCarrierRFID_NG.Initialize();

//	CString RFID;
//	g_objCarrierRFID_Load.Send_RFIDRead();
//	while(!g_objCarrierRFID_Load.Is_RecvComplete()){ theApp.uSleep(10); }
//	RFID = g_objCarrierRFID_Load.Get_CarrierID(); 

//	gData.nTransferXLock = 0;
//	g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_STAGE_1, 50);
//	g_objSequenceMain.Set_MainRunCase(AUTO_LDUP_CONVEYOR, 0);
//	g_objSequenceMain.Set_MainRunCase(AUTO_LDET_CONVEYOR, 0);
//	for(int i=0; i<2; i++) gData.sLotID_MZElevator[i] = "";
/*

	g_objCarrierRFID_Good.Send_RFIDRead();
	while(!g_objCarrierRFID_Good.Is_RecvComplete()){ theApp.uSleep(10); }
	RFID = g_objCarrierRFID_Good.Get_CarrierID(); 

	g_objCarrierRFID_NG.Send_RFIDRead();
	while(!g_objCarrierRFID_NG.Is_RecvComplete()){ theApp.uSleep(10); }
	RFID = g_objCarrierRFID_NG.Get_CarrierID();
*/
/*
	CString sLog;
	m_stcCmsCountS[0].GetWindowText(sLog);
	int nNo = atoi(sLog);
	int nPosX = nNo % TRAY_CM_X;	if (nPosX==0) nPosX = 4;
	int nPosY = nNo / TRAY_CM_X;	if (nPosX!=4) nPosY++;
	sLog.Format("[%d]  y[%d] x[%d]", nNo, nPosY, nPosX);
	AfxMessageBox(sLog);
*/
//	g_objCommon.Show_Error(9002);
/*
	g_objCommon.Show_Error(5010);
	DX_DATA_00 *pDX00 = g_objAJinAXL.Get_pDX00();
	DX_DATA_01 *pDX01 = g_objAJinAXL.Get_pDX01();
	DX_DATA_02 *pDX02 = g_objAJinAXL.Get_pDX02();

	pDX00->iElevator1SlideClose = TRUE;
	pDX00->iElevator2SlideClose = TRUE;
	pDX01->iElevator3SlideClose = TRUE;
	pDX01->iElevator4SlideClose = TRUE;
	pDX02->iElevator5SlideClose = TRUE;
	pDX02->iElevator6SlideClose = TRUE;
	pDX02->iElevator7SlideClose = TRUE;
*/
//	g_objInspector.Set_MoveComplete(INSPECTOR_PC1, "B1");
//	ElevatorOpen(1);
//	ElevatorOpen(7);
//	CCMAI2100Dlg *pMainDlg = (CCMAI2100Dlg*)AfxGetMainWnd();
//	pMainDlg->StdLogAllDelete();
/*
	CString sLog;
	for(int i=0; i<3; i++) {
		for(int j=0; j<320; j++) {
			Gt[i].sData1[j] = "123456789012345678901234567890";
			Gt[i].sData2[j] = "123456789012345678901234567890";
			Gt[i].sData3[j] = "123456789012345678901234567890";
			Gt[i].sData4[j] = "123456789012345678901234567890";
			Gt[i].sData5[j] = "123456789012345678901234567890";
		}
	}
	sLog.Format("%d,%d,%d", sizeof(Gt), sizeof(Gt[0]), sizeof(Gt[0].sData1[0]));
	AfxMessageBox(sLog);
	sLog.Format("%d", (Gt[0].sData1[0].GetLength()*5*320*3));
	AfxMessageBox(sLog);
*/
}
