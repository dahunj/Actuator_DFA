// SequenceMain.cpp : 구현 파일
//
#include "stdafx.h"
#include "CMAI2100.h"
#include "SequenceMain.h"
#include "LogFile.h"
#include "Common.h"
#include "Inspector.h"
#include "Dispatcher.h"
#include "CMAI2100Dlg.h"
#include "WorkDlg.h"
#include "MesAgent.h"
#include "BarcodeLot_RS232.h"
#include "BarcodeLot_Cognex.h"
#include "CarrierRFID_Load.h"
#include "CarrierRFID_Good.h"
#include "CarrierRFID_NG.h"
#include "OCAPCosmeticDlg.h"
#include "OCAPFaiDlg.h"
#include <math.h>

CSequenceMain g_objSequenceMain;

///////////////////////////////////////////////////////////////////////////////

CSequenceMain::CSequenceMain()
{
	for (int i = 0; i < AXIS_COUNT; i++) {
		m_pStatus[i] = g_objAJinAXL.Get_pStatus(i);
		m_pParam[i] = g_objAJinAXL.Get_pParam(i);
	}

	m_pDX00 = g_objAJinAXL.Get_pDX00();
	m_pDX01 = g_objAJinAXL.Get_pDX01();
	m_pDX02 = g_objAJinAXL.Get_pDX02();
	m_pDX03 = g_objAJinAXL.Get_pDX03();
	m_pDX04 = g_objAJinAXL.Get_pDX04();
	m_pDX05 = g_objAJinAXL.Get_pDX05();
	m_pDX06 = g_objAJinAXL.Get_pDX06();
	m_pDX07 = g_objAJinAXL.Get_pDX07();
	m_pDX08 = g_objAJinAXL.Get_pDX08();
	m_pDX09 = g_objAJinAXL.Get_pDX09();
	m_pDX10 = g_objAJinAXL.Get_pDX10();
	m_pDX11 = g_objAJinAXL.Get_pDX11();
	m_pDX12 = g_objAJinAXL.Get_pDX12();
	m_pDX13 = g_objAJinAXL.Get_pDX13();
	m_pDX14 = g_objAJinAXL.Get_pDX14();
	m_pDX15 = g_objAJinAXL.Get_pDX15();
	m_pDX16 = g_objAJinAXL.Get_pDX16();
	m_pDX17 = g_objAJinAXL.Get_pDX17();
	m_pDX18 = g_objAJinAXL.Get_pDX18();
	m_pDX19 = g_objAJinAXL.Get_pDX19();
	m_pDX20 = g_objAJinAXL.Get_pDX20();

	m_pDY00 = g_objAJinAXL.Get_pDY00();
	m_pDY01 = g_objAJinAXL.Get_pDY01();
	m_pDY02 = g_objAJinAXL.Get_pDY02();
	m_pDY03 = g_objAJinAXL.Get_pDY03();
	m_pDY04 = g_objAJinAXL.Get_pDY04();
	m_pDY05 = g_objAJinAXL.Get_pDY05();
	m_pDY06 = g_objAJinAXL.Get_pDY06();
	m_pDY07 = g_objAJinAXL.Get_pDY07();
	m_pDY08 = g_objAJinAXL.Get_pDY08();
	m_pDY09 = g_objAJinAXL.Get_pDY09();
	m_pDY10 = g_objAJinAXL.Get_pDY10();
	m_pDY11 = g_objAJinAXL.Get_pDY11();
	m_pDY12 = g_objAJinAXL.Get_pDY12();
	m_pDY13 = g_objAJinAXL.Get_pDY13();
	m_pDY14 = g_objAJinAXL.Get_pDY14();
	m_pDY15 = g_objAJinAXL.Get_pDY15();
	m_pDY16 = g_objAJinAXL.Get_pDY16();
	m_pDY17 = g_objAJinAXL.Get_pDY17();
	m_pDY18 = g_objAJinAXL.Get_pDY18();
	m_pDY19 = g_objAJinAXL.Get_pDY19();
	m_pDY20 = g_objAJinAXL.Get_pDY20();

	m_pEquipData = g_objDataManager.Get_pEquipData();
	m_pMoveData = g_objDataManager.Get_pMoveData();
	m_pRosData = g_objDataManager.Get_pRosData();

	m_bThreadMainRun = FALSE;
	m_pThreadMainRun = NULL;

	Reset_MainRunCase();
}

CSequenceMain::~CSequenceMain()
{
	if (m_pThreadMainRun) {
		m_bThreadMainRun = FALSE;
		WaitForSingleObject(m_pThreadMainRun->m_hThread, INFINITE);
	}
}

void CSequenceMain::Reset_MainRunCase()
{
	m_nElevator1Case	= 0;
	m_nElevator2Case	= 0;
	m_nElevator3Case	= 0;
	m_nElevator4Case	= 0;
	m_nElevator5Case	= 0;
	m_nElevator6Case	= 0;
	m_nElevator7Case	= 0;
	m_nTransfer1Case	= 0;
	m_nTransfer2Case	= 0;
	m_nLoadStage1Case	= 0;
	m_nLoadStage2Case	= 50;
	m_nLoadPicker1Case	= 0;
	m_nLoadPicker2Case	= 70;
	m_nVisionStage1Case = 50;
	m_nVisionStage2Case = 00;
	m_nVisionStage3Case = 60;
	m_nVisionStage4Case = 70;
	m_nUnloadPicker1Case= 0;
	m_nUnloadPicker2Case= 50;
	m_nNGStage1Case		= 0;
	m_nNGStage2Case		= 50;
	m_nGoodStage1Case	= 0;
	m_nGoodStage2Case	= 50;
	m_nShipAlignCase	= 0;

	m_nMZTransferCase	= 0;
	m_nLDCVElevatorCase	= 0;
	m_nULCVElevatorCase	= 0;
	m_nLDMZElevatorCase	= 0;
	m_nNGMZElevatorCase	= 0;
	m_nGDMZElevatorCase	= 0;
	m_nLD1FConveyorCase	= 0;
	m_nLD2FConveyorCase	= 0;
	m_nUnloadConveyorCase = 0;
}

int *CSequenceMain::Get_pMainRunCase()
{
	static int nCase[33] = { 0 };

	nCase[0]  = m_nElevator1Case;	
	nCase[1]  = m_nElevator2Case;	
	nCase[2]  = m_nElevator3Case;	
	nCase[3]  = m_nElevator4Case;	
	nCase[4]  = m_nElevator5Case;	
	nCase[5]  = m_nElevator6Case;	
	nCase[6]  = m_nElevator7Case;	
	nCase[7]  = m_nTransfer1Case;	
	nCase[8]  = m_nTransfer2Case;	
	nCase[9]  = m_nLoadStage1Case;	
	nCase[10] = m_nLoadStage2Case;	
	nCase[11] = m_nLoadPicker1Case;	
	nCase[12] = m_nLoadPicker2Case;	
	nCase[13] = m_nVisionStage1Case;
	nCase[14] = m_nVisionStage2Case;
	nCase[15] = m_nVisionStage3Case;
	nCase[16] = m_nVisionStage4Case;
	nCase[17] = m_nUnloadPicker1Case;
	nCase[18] = m_nUnloadPicker2Case;
	nCase[19] = m_nNGStage1Case;	
	nCase[20] = m_nNGStage2Case;	
	nCase[21] = m_nGoodStage1Case;	
	nCase[22] = m_nGoodStage2Case;	
	nCase[23] = m_nShipAlignCase;	

	nCase[24] = m_nMZTransferCase;
	nCase[25] = m_nLDCVElevatorCase;
	nCase[26] = m_nULCVElevatorCase;
	nCase[27] = m_nLDMZElevatorCase;
	nCase[28] = m_nNGMZElevatorCase;
	nCase[29] = m_nGDMZElevatorCase;
	nCase[30] = m_nLD1FConveyorCase;
	nCase[31] = m_nLD2FConveyorCase;
	nCase[32] = m_nUnloadConveyorCase;

	return nCase;
}

int CSequenceMain::Get_MainRunCase(int nRun)
{
	if (nRun == AUTO_ELEVATOR_1)		return m_nElevator1Case;	
	if (nRun == AUTO_ELEVATOR_2)		return m_nElevator2Case;	
	if (nRun == AUTO_ELEVATOR_3)		return m_nElevator3Case;	
	if (nRun == AUTO_ELEVATOR_4)		return m_nElevator4Case;	
	if (nRun == AUTO_ELEVATOR_5)		return m_nElevator5Case;	
	if (nRun == AUTO_ELEVATOR_6)		return m_nElevator6Case;	
	if (nRun == AUTO_ELEVATOR_7)		return m_nElevator7Case;	
	if (nRun == AUTO_TRANSFER_1)		return m_nTransfer1Case;	
	if (nRun == AUTO_TRANSFER_2)		return m_nTransfer2Case;	
	if (nRun == AUTO_LOAD_STAGE_1)		return m_nLoadStage1Case;	
	if (nRun == AUTO_LOAD_STAGE_2)		return m_nLoadStage2Case;	
	if (nRun == AUTO_LOAD_PICKER_1)		return m_nLoadPicker1Case;	
	if (nRun == AUTO_LOAD_PICKER_2)		return m_nLoadPicker2Case;	
	if (nRun == AUTO_VISION_STAGE_1)	return m_nVisionStage1Case;
	if (nRun == AUTO_VISION_STAGE_2)	return m_nVisionStage2Case;
	if (nRun == AUTO_VISION_STAGE_3)	return m_nVisionStage3Case;
	if (nRun == AUTO_VISION_STAGE_4)	return m_nVisionStage4Case;
	if (nRun == AUTO_UNLOAD_PICKER_1)	return m_nUnloadPicker1Case;
	if (nRun == AUTO_UNLOAD_PICKER_2)	return m_nUnloadPicker2Case;
	if (nRun == AUTO_NG_STAGE_1)		return m_nNGStage1Case;	
	if (nRun == AUTO_NG_STAGE_2)		return m_nNGStage2Case;	
	if (nRun == AUTO_GOOD_STAGE_1)		return m_nGoodStage1Case;	
	if (nRun == AUTO_GOOD_STAGE_2)		return m_nGoodStage2Case;	
	if (nRun == AUTO_SHIP_ALIGN)		return m_nShipAlignCase;	

	if (nRun == AUTO_MZ_TRANSFER)		return m_nMZTransferCase;	
	if (nRun == AUTO_LDCV_ELEVATOR)		return m_nLDCVElevatorCase;	
	if (nRun == AUTO_ULCV_ELEVATOR)		return m_nULCVElevatorCase;	
	if (nRun == AUTO_LDMZ_ELEVATOR)		return m_nLDMZElevatorCase;	
	if (nRun == AUTO_NGMZ_ELEVATOR)		return m_nNGMZElevatorCase;	
	if (nRun == AUTO_GDMZ_ELEVATOR)		return m_nGDMZElevatorCase;	
	if (nRun == AUTO_LDUP_CONVEYOR)		return m_nLD1FConveyorCase;	
	if (nRun == AUTO_LDET_CONVEYOR)		return m_nLD2FConveyorCase;	
	if (nRun == AUTO_UNLOAD_CONVEYOR)	return m_nUnloadConveyorCase;

	return 0;
}

void CSequenceMain::Set_MainRunCase(int nRun, int nCase)
{
	if (nRun == AUTO_ELEVATOR_1)		m_nElevator1Case		= nCase;
	if (nRun == AUTO_ELEVATOR_2)		m_nElevator2Case		= nCase;
	if (nRun == AUTO_ELEVATOR_3)		m_nElevator3Case		= nCase;
	if (nRun == AUTO_ELEVATOR_4)		m_nElevator4Case		= nCase;
	if (nRun == AUTO_ELEVATOR_5)		m_nElevator5Case		= nCase;
	if (nRun == AUTO_ELEVATOR_6)		m_nElevator6Case		= nCase;
	if (nRun == AUTO_ELEVATOR_7)		m_nElevator7Case		= nCase;
	if (nRun == AUTO_TRANSFER_1)		m_nTransfer1Case		= nCase;
	if (nRun == AUTO_TRANSFER_2)		m_nTransfer2Case		= nCase;
	if (nRun == AUTO_LOAD_STAGE_1)		m_nLoadStage1Case		= nCase;
	if (nRun == AUTO_LOAD_STAGE_2)		m_nLoadStage2Case		= nCase;
	if (nRun == AUTO_LOAD_PICKER_1)		m_nLoadPicker1Case		= nCase;
	if (nRun == AUTO_LOAD_PICKER_2)		m_nLoadPicker2Case		= nCase;
	if (nRun == AUTO_VISION_STAGE_1)	m_nVisionStage1Case		= nCase;
	if (nRun == AUTO_VISION_STAGE_2)	m_nVisionStage2Case		= nCase;
	if (nRun == AUTO_VISION_STAGE_3)	m_nVisionStage3Case		= nCase;
	if (nRun == AUTO_VISION_STAGE_4)	m_nVisionStage4Case		= nCase;
	if (nRun == AUTO_UNLOAD_PICKER_1)	m_nUnloadPicker1Case	= nCase;
	if (nRun == AUTO_UNLOAD_PICKER_2)	m_nUnloadPicker2Case	= nCase;
	if (nRun == AUTO_NG_STAGE_1)		m_nNGStage1Case			= nCase;
	if (nRun == AUTO_NG_STAGE_2)		m_nNGStage2Case			= nCase;
	if (nRun == AUTO_GOOD_STAGE_1)		m_nGoodStage1Case		= nCase;
	if (nRun == AUTO_GOOD_STAGE_2)		m_nGoodStage2Case		= nCase;
	if (nRun == AUTO_SHIP_ALIGN)		m_nShipAlignCase		= nCase;

	if (nRun == AUTO_MZ_TRANSFER)		m_nMZTransferCase		= nCase;
	if (nRun == AUTO_LDCV_ELEVATOR)		m_nLDCVElevatorCase		= nCase;
	if (nRun == AUTO_ULCV_ELEVATOR)		m_nULCVElevatorCase		= nCase;
	if (nRun == AUTO_LDMZ_ELEVATOR)		m_nLDMZElevatorCase		= nCase;
	if (nRun == AUTO_NGMZ_ELEVATOR)		m_nNGMZElevatorCase		= nCase;
	if (nRun == AUTO_GDMZ_ELEVATOR)		m_nGDMZElevatorCase		= nCase;
	if (nRun == AUTO_LDUP_CONVEYOR)		m_nLD1FConveyorCase		= nCase;
	if (nRun == AUTO_LDET_CONVEYOR)		m_nLD2FConveyorCase		= nCase;
	if (nRun == AUTO_UNLOAD_CONVEYOR)	m_nUnloadConveyorCase	= nCase;
}

void CSequenceMain::Set_MainRunLoop(int nRun, int nLoop)
{
	if (nRun == AUTO_ELEVATOR_1)		m_tElevator1Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_ELEVATOR_2)		m_tElevator2Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_ELEVATOR_3)		m_tElevator3Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_ELEVATOR_4)		m_tElevator4Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_ELEVATOR_5)		m_tElevator5Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_ELEVATOR_6)		m_tElevator6Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_ELEVATOR_7)		m_tElevator7Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_TRANSFER_1)		m_tTransfer1Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_TRANSFER_2)		m_tTransfer2Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_LOAD_STAGE_1)		m_tLoadStage1Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_LOAD_STAGE_2)		m_tLoadStage2Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_LOAD_PICKER_1)		m_tLoadPicker1Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_LOAD_PICKER_2)		m_tLoadPicker2Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_VISION_STAGE_1)	m_tVisionStage1Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_VISION_STAGE_2)	m_tVisionStage2Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_VISION_STAGE_3)	m_tVisionStage3Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_VISION_STAGE_4)	m_tVisionStage4Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_UNLOAD_PICKER_1)	m_tUnloadPicker1Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_UNLOAD_PICKER_2)	m_tUnloadPicker2Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_NG_STAGE_1)		m_tNGStage1Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_NG_STAGE_2)		m_tNGStage2Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_GOOD_STAGE_1)		m_tGoodStage1Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_GOOD_STAGE_2)		m_tGoodStage2Loop.Set_LoopTime(nLoop);
	if (nRun == AUTO_SHIP_ALIGN)		m_tShipAlignLoop.Set_LoopTime(nLoop);

	if (nRun == AUTO_MZ_TRANSFER)		m_tMZTransferLoop.Set_LoopTime(nLoop);
	if (nRun == AUTO_LDCV_ELEVATOR)		m_tLDCVElevatorLoop.Set_LoopTime(nLoop);
	if (nRun == AUTO_ULCV_ELEVATOR)		m_tULCVElevatorLoop.Set_LoopTime(nLoop);
	if (nRun == AUTO_LDMZ_ELEVATOR)		m_tLDMZElevatorLoop.Set_LoopTime(nLoop);
	if (nRun == AUTO_NGMZ_ELEVATOR)		m_tNGMZElevatorLoop.Set_LoopTime(nLoop);
	if (nRun == AUTO_GDMZ_ELEVATOR)		m_tGDMZElevatorLoop.Set_LoopTime(nLoop);
	if (nRun == AUTO_LDUP_CONVEYOR)		m_tLD1FConveyorLoop.Set_LoopTime(nLoop);
	if (nRun == AUTO_LDET_CONVEYOR)		m_tLD2FConveyorLoop.Set_LoopTime(nLoop);
	if (nRun == AUTO_UNLOAD_CONVEYOR)	m_tUnloadConveyorLoop.Set_LoopTime(nLoop);
}

BOOL CSequenceMain::Get_IsAutoRun()
{
	int *pCase = Get_pMainRunCase();
	for (int i = 0; i < 33; i++) {
		if (i <= AUTO_ELEVATOR_7      && *(pCase + i) == 20) continue;
		if (i <= AUTO_ELEVATOR_7      && *(pCase + i) == 51) continue;

		if (i == AUTO_LOAD_STAGE_1    && m_nLoadStage1Case == 50) continue;
		if (i == AUTO_LOAD_STAGE_2    && m_nLoadStage2Case == 50) continue;
		if (i == AUTO_LOAD_PICKER_1   && m_nLoadPicker1Case == 70) continue;
		if (i == AUTO_LOAD_PICKER_2   && m_nLoadPicker2Case == 70) continue;
		if (i == AUTO_UNLOAD_PICKER_1 && m_nUnloadPicker1Case == 50) continue;
		if (i == AUTO_UNLOAD_PICKER_2 && m_nUnloadPicker2Case == 50) continue;

		if (i == AUTO_NG_STAGE_1      && m_nNGStage1Case == 50) continue;
		if (i == AUTO_NG_STAGE_2      && m_nNGStage2Case == 50) continue;
		if (i == AUTO_GOOD_STAGE_1    && m_nGoodStage1Case == 50) continue;
		if (i == AUTO_GOOD_STAGE_2    && m_nGoodStage2Case == 50) continue;

		if (i == AUTO_VISION_STAGE_1  && (m_nVisionStage1Case == 50 || m_nVisionStage1Case == 60 || m_nVisionStage1Case == 70)) continue;
		if (i == AUTO_VISION_STAGE_2  && (m_nVisionStage2Case == 50 || m_nVisionStage2Case == 60 || m_nVisionStage2Case == 70)) continue;
		if (i == AUTO_VISION_STAGE_3  && (m_nVisionStage3Case == 50 || m_nVisionStage3Case == 60 || m_nVisionStage3Case == 70)) continue;
		if (i == AUTO_VISION_STAGE_4  && (m_nVisionStage4Case == 50 || m_nVisionStage4Case == 60 || m_nVisionStage4Case == 70)) continue;

//		if (i == AUTO_LDUP_CONVEYOR    && m_nGoodStage2Case == 50) continue;
//		if (i == AUTO_LDET_CONVEYOR    && m_nGoodStage2Case == 50) continue;

		if (*(pCase + i) > 0) return TRUE;
	}
//	if (!Check_TrayEmpty()) return TRUE;
	if (!Check_ModuleEmpty()) return TRUE;
	
	return FALSE;
}

void CSequenceMain::Begin_MainRunThread()
{
	Run_Replay();

	if (m_pThreadMainRun) End_MainRunThread();
	m_bThreadMainRun = TRUE;
	m_pThreadMainRun = AfxBeginThread(Thread_MainRun, NULL);
}

void CSequenceMain::End_MainRunThread()
{
	if (m_pThreadMainRun) {
		m_bThreadMainRun = FALSE;
		WaitForSingleObject(m_pThreadMainRun->m_hThread, INFINITE);
	}
}

void CSequenceMain::Run_Replay()
{
	int nLoop = 30000;	//30초
	if (m_nUnloadPicker1Case == 13 || m_nUnloadPicker2Case == 13) nLoop = 300000;	//5분
	if (m_nNGStage1Case == 9)	nLoop = 5000;
	if (m_nNGStage2Case == 9)	nLoop = 5000;
	if (m_nGoodStage1Case == 9) nLoop = 5000;
	if (m_nGoodStage2Case == 9) nLoop = 5000;

	m_tElevator1Loop.Set_LoopTime(nLoop);
	m_tElevator2Loop.Set_LoopTime(nLoop);
	m_tElevator3Loop.Set_LoopTime(nLoop);
	m_tElevator4Loop.Set_LoopTime(nLoop);
	m_tElevator5Loop.Set_LoopTime(nLoop);
	m_tElevator6Loop.Set_LoopTime(nLoop);
	m_tElevator7Loop.Set_LoopTime(nLoop);
	m_tTransfer1Loop.Set_LoopTime(nLoop);
	m_tTransfer2Loop.Set_LoopTime(nLoop);
	m_tLoadStage1Loop.Set_LoopTime(nLoop);
	m_tLoadStage2Loop.Set_LoopTime(nLoop);
	m_tLoadPicker1Loop.Set_LoopTime(nLoop);
	m_tLoadPicker2Loop.Set_LoopTime(nLoop);
	m_tVisionStage1Loop.Set_LoopTime(nLoop);
	m_tVisionStage2Loop.Set_LoopTime(nLoop);
	m_tVisionStage3Loop.Set_LoopTime(nLoop);
	m_tVisionStage4Loop.Set_LoopTime(nLoop);
	m_tUnloadPicker1Loop.Set_LoopTime(nLoop);
	m_tUnloadPicker2Loop.Set_LoopTime(nLoop);
	m_tNGStage1Loop.Set_LoopTime(nLoop);
	m_tNGStage2Loop.Set_LoopTime(nLoop);
	m_tGoodStage1Loop.Set_LoopTime(nLoop);
	m_tGoodStage2Loop.Set_LoopTime(nLoop);
	m_tShipAlignLoop.Set_LoopTime(5000);

	m_tMZTransferLoop.Set_LoopTime(nLoop);
	m_tLDCVElevatorLoop.Set_LoopTime(nLoop);
	m_tULCVElevatorLoop.Set_LoopTime(nLoop);
	m_tLDMZElevatorLoop.Set_LoopTime(nLoop);
	m_tNGMZElevatorLoop.Set_LoopTime(nLoop);
	m_tGDMZElevatorLoop.Set_LoopTime(nLoop);
	m_tLD1FConveyorLoop.Set_LoopTime(nLoop);
	m_tLD2FConveyorLoop.Set_LoopTime(nLoop);
	m_tUnloadConveyorLoop.Set_LoopTime(nLoop);

	if (m_nLD1FConveyorCase == 3) {
		m_pDY16->oLDCV3CCW1F = TRUE; m_pDY16->oLDCV3CW1F = TRUE;
		m_pDY16->oLDCV4CCW1F = TRUE; m_pDY16->oLDCV4CW1F = TRUE;
		g_objAJinAXL.Write_Output(16);
	}
	if (m_nLD1FConveyorCase == 19) {
		m_pDY16->oLDCV3CCW1F = TRUE; m_pDY16->oLDCV3CW1F = TRUE;
		m_pDY16->oLDCV4CCW1F = TRUE; m_pDY16->oLDCV4CW1F = TRUE;
		g_objAJinAXL.Write_Output(16);
		m_pDY17->oLDCVElevatorCVCCW = TRUE; m_pDY17->oLDCVElevatorCVCW = TRUE;
		g_objAJinAXL.Write_Output(17);
	}
	if (m_nLD1FConveyorCase == 21) {
		m_pDY16->oLDCV3CCW1F = FALSE; m_pDY16->oLDCV3CW1F = TRUE;
		m_pDY16->oLDCV4CCW1F = FALSE; m_pDY16->oLDCV4CW1F = TRUE;
		g_objAJinAXL.Write_Output(16);
		m_pDY17->oLDCVElevatorCVCCW = TRUE; m_pDY17->oLDCVElevatorCVCW = TRUE;
		g_objAJinAXL.Write_Output(17);
	}
	if (m_nLD1FConveyorCase == 26) {
		m_pDY16->oLDCV3CCW1F = TRUE; m_pDY16->oLDCV3CW1F = TRUE;
		m_pDY16->oLDCV4CCW1F = TRUE; m_pDY16->oLDCV4CW1F = TRUE;
		g_objAJinAXL.Write_Output(16);
	}

	if (m_nLD2FConveyorCase == 3) {
		m_pDY16->oLDCV1CCW2F = TRUE; m_pDY16->oLDCV1CW2F = TRUE;
		m_pDY16->oLDCV2CCW2F = TRUE; m_pDY16->oLDCV2CW2F = TRUE;
		g_objAJinAXL.Write_Output(16);
	}
	if (m_nLD2FConveyorCase == 19) {
		m_pDY16->oLDCV1CCW2F = TRUE; m_pDY16->oLDCV1CW2F = TRUE;
		m_pDY16->oLDCV2CCW2F = TRUE; m_pDY16->oLDCV2CW2F = TRUE;
		g_objAJinAXL.Write_Output(16);
		m_pDY17->oLDCVElevatorCVCCW = TRUE; m_pDY17->oLDCVElevatorCVCW = TRUE;
		g_objAJinAXL.Write_Output(17);
	}
	if (m_nLD2FConveyorCase == 21) {
		m_pDY16->oLDCV1CCW2F = FALSE; m_pDY16->oLDCV1CW2F = TRUE;
		m_pDY16->oLDCV2CCW2F = FALSE; m_pDY16->oLDCV2CW2F = TRUE;
		g_objAJinAXL.Write_Output(16);
		m_pDY17->oLDCVElevatorCVCCW = TRUE; m_pDY17->oLDCVElevatorCVCW = TRUE;
		g_objAJinAXL.Write_Output(17);
	}

	if (m_nULCVElevatorCase == 13) {
		m_pDY17->oULCVElevatorCVCCW = TRUE; m_pDY17->oULCVElevatorCVCW = TRUE;
		g_objAJinAXL.Write_Output(17);
	}
	if (m_nULCVElevatorCase == 14) {
		m_pDY19->oULCV1CCW2F = TRUE;   m_pDY19->oULCV1CW2F = TRUE;
		m_pDY19->oULCV2CCW2F = TRUE;   m_pDY19->oULCV2CW2F = TRUE;
		g_objAJinAXL.Write_Output(19);
	}
	if (m_nULCVElevatorCase == 23) {
		m_pDY17->oULCVElevatorCVCCW = TRUE; m_pDY17->oULCVElevatorCVCW = TRUE;
		g_objAJinAXL.Write_Output(17);
	}
	if (m_nULCVElevatorCase == 24) {
		m_pDY19->oULCV3CCW1F = TRUE;   m_pDY19->oULCV3CW1F = TRUE;
		m_pDY19->oULCV4CCW1F = TRUE;   m_pDY19->oULCV4CW1F = TRUE;
		g_objAJinAXL.Write_Output(19);
	}
	if (m_nULCVElevatorCase == 31 || m_nULCVElevatorCase == 32) m_nUnloadConveyorCase = 1;

	if (m_nUnloadConveyorCase == 2)		m_nUnloadConveyorCase = 1;
	if (m_nUnloadConveyorCase == 12)	m_nUnloadConveyorCase = 11;
	
	if (!m_pEquipData->bUseMES) {
		if (m_nLoadStage1Case == 5 || m_nLoadStage2Case == 5) gLot.sRecipeName[m_nRcpPortNo] = gData.sRecipeName;
	}

	if (m_nLDMZElevatorCase == 14) g_objCommon.Move_Position(AX_LOAD_FEEDER_Y, 1, 0.5);
	if (m_nLDMZElevatorCase == 17) g_objCommon.Move_Position(AX_LOAD_FEEDER_Y, 2, 0.5);
	if (m_nLDMZElevatorCase == 20) g_objCommon.Move_Position(AX_LOAD_FEEDER_Y, 0, 0.5);
	if (m_nNGMZElevatorCase == 14) g_objCommon.Move_Position(AX_NG_FEEDER_Y, 1, 0.5);
	if (m_nNGMZElevatorCase == 17) g_objCommon.Move_Position(AX_NG_FEEDER_Y, 2, 0.5);
	if (m_nNGMZElevatorCase == 20) g_objCommon.Move_Position(AX_NG_FEEDER_Y, 0, 0.5);
	if (m_nNGMZElevatorCase == 72) g_objCommon.Move_Position(AX_NG_FEEDER_Y, 1, 0.5);
	if (m_nNGMZElevatorCase == 74) g_objCommon.Move_Position(AX_NG_FEEDER_Y, 0, 0.5);
	if (m_nGDMZElevatorCase == 14) g_objCommon.Move_Position(AX_GOOD_FEEDER_Y, 1, 0.5);
	if (m_nGDMZElevatorCase == 17) g_objCommon.Move_Position(AX_GOOD_FEEDER_Y, 2, 0.5);
	if (m_nGDMZElevatorCase == 20) g_objCommon.Move_Position(AX_GOOD_FEEDER_Y, 0, 0.5);
	if (m_nGDMZElevatorCase == 72) g_objCommon.Move_Position(AX_GOOD_FEEDER_Y, 1, 0.5);
	if (m_nGDMZElevatorCase == 74) g_objCommon.Move_Position(AX_GOOD_FEEDER_Y, 0, 0.5);

}

/////////////////////////////////////////////////////////////////////////////
// Main Thread Function 
UINT CSequenceMain::Thread_MainRun(LPVOID lpVoid)
{
	while (g_objSequenceMain.m_bThreadMainRun) {
		if (!g_objCommon.Check_MainEmgAir()) break;
		if (!g_objCommon.Check_MainDoor(TRUE)) break;
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objCommon.Check_DirveAlarm()) break;
		if (!g_objCommon.Check_EndLimit()) break;
		if (!g_objCommon.Check_Elevator()) break;
	
		if (!g_objSequenceMain.Run_Elevator2()) break;		//  2. (Error : 2100)
		if (!g_objSequenceMain.Run_Transfer1()) break;		//  8. (Error : 3000)
		if (!g_objSequenceMain.Run_Transfer2()) break;		//  9. (Error : 3500)
		if (!g_objSequenceMain.Run_LoadStage1()) break;		// 10. (Error : 4000)
		if (!g_objSequenceMain.Run_LoadStage2()) break;		// 11. (Error : 4500)
		if (!g_objSequenceMain.Run_LoadPicker1()) break;	// 12. (Error : 5000)
		if (!g_objSequenceMain.Run_LoadPicker2()) break;	// 13. (Error : 5500)
		if (!g_objSequenceMain.Run_VisionStage1()) break;	// 14. (Error : 6000)
		if (!g_objSequenceMain.Run_VisionStage2()) break;	// 15. (Error : 6200)
		if (!g_objSequenceMain.Run_VisionStage3()) break;	// 16. (Error : 6400)
		if (!g_objSequenceMain.Run_VisionStage4()) break;	// 17. (Error : 6600)
		if (!g_objSequenceMain.Run_UnloadPicker1()) break;	// 18. (Error : 7000)
		if (!g_objSequenceMain.Run_UnloadPicker2()) break;	// 19. (Error : 7500)
		if (!g_objSequenceMain.Run_NGStage1()) break;		// 20. (Error : 8000)
		if (!g_objSequenceMain.Run_NGStage2()) break;		// 21. (Error : 8200)
		if (!g_objSequenceMain.Run_GoodStage1()) break;		// 22. (Error : 8400)
		if (!g_objSequenceMain.Run_GoodStage2()) break;		// 23. (Error : 8600)
		if (!g_objSequenceMain.Run_ShipAlign()) break;		// 24. (Error : 8800)

		if (!g_objSequenceMain.Run_MZTransfer()) break;		// 25. (Error : 2200)
		if (!g_objSequenceMain.Run_LDCVElevator()) break;	// 26. (Error : 2300)
		if (!g_objSequenceMain.Run_ULCVElevator()) break;	// 27. (Error : 2400)
		if (!g_objSequenceMain.Run_LDMZElevator()) break;	// 28. (Error : 2500)
		if (!g_objSequenceMain.Run_NGMZElevator()) break;	// 29. (Error : 2600)
		if (!g_objSequenceMain.Run_GDMZElevator()) break;	// 30. (Error : 2700)
		if (!g_objSequenceMain.Run_LD1FConveyor()) break;	// 31. (Error : 2800)
		if (!g_objSequenceMain.Run_LD2FConveyor()) break;	// 32. (Error : 2900)
		if (!g_objSequenceMain.Run_UnloadConveyor()) break;	// 32. (Error : 2950)

		if (!g_objSequenceMain.Run_Simulation()) break;		//  99.
		if (gLot.nJobStatus == 1 && g_objSequenceMain.Get_IsAutoRun()==FALSE) {
			gLot.nJobStatus = 2;
		}
		if (gLot.nJobStatus == 2 && g_objSequenceMain.LotEnd_Run()) break;

		Sleep(10);
	}

	g_objSequenceMain.m_bThreadMainRun = FALSE;
	g_objSequenceMain.m_pThreadMainRun = NULL;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL CSequenceMain::LotEnd_Run()
{
	if (Get_IsAutoRun()) return FALSE;
//	int nA, nB, nC;
//	if (Check_Transfer2(nA, nB, nC)) return FALSE;

	gLot.nJobStatus = 0;
	g_objSequenceMain.Set_ClearRunData(1);

 	Beep_Post(1000);
	g_dlgWork.PostMessage(UM_JOB_COMPELTE, NULL, NULL);
	if (gData.bCycleStop) g_dlgWork.PostMessage(UM_RESET_CYCLE_STOP, 1, NULL);

	CCMAI2100Dlg *pMainDlg = (CCMAI2100Dlg*)AfxGetMainWnd();
	pMainDlg->Set_MainState(STATE_LOTEND);
//	g_objMES.Set_Status(3);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

void CSequenceMain::Set_ClearRunData(int nType)
{
	for(int i=0; i<10; i++) {
		for(int j=0; j<4; j++) {
			gData.InfoAlignTray[i][j] = gData.InfoLoadTray[i][j] = gData.InfoNgTray[i][j] = gData.InfoGoodTray[i][j] = 0;
		}
	}
	for(int i=0; i<2; i++) {
		for(int j=0; j<10; j++) {
			gData.InfoLoadPick[i][j] = gData.InfoUnloadPick[i][j] = 0;
		}
	}
	for(int i=0; i<4; i++) {
		for(int j=0; j<10; j++) {
			gData.InfoVision[i][j]  = 0;
		}
	}

	gData.sLotID_NGBuffer = ""; gData.nTrayNo_NGBuffer = 0; gData.nPortNo_NGBuffer = 0; gData.nROSReqCount = 0;
	for(int i=0; i<2; i++) {
		gData.sLotID_Tansfer[i] = gData.sLotID_LoadStage[i] = gData.sLotID_LoadPicker[i] = gData.sLotID_UnloadPicker[i] = gData.sLotID_NGTray[i] = gData.sLotID_GoodTray[i] = gData.sCarID_LoadStage[i] = "";
		gData.nTrayNo_Tansfer[i] = gData.nTrayNo_LoadStage[i] = gData.nTrayNo_LoadPicker[i] = gData.nTrayNo_UnloadPicker[i] = gData.nTrayNo_NGTray[i] = gData.nTrayNo_GoodTray[i] = 0;
		gData.nPortNo_Tansfer[i] = gData.nPortNo_LoadStage[i] = gData.nPortNo_LoadPicker[i] = gData.nPortNo_UnloadPicker[i] = gData.nPortNo_NGTray[i] = gData.nPortNo_GoodTray[i] = 0;
	}
	for(int i=0; i<4; i++) {
		gData.sLotID_VisionStage[i] = "";	gData.nTrayNo_VisionStage[i] = 0; gData.nPortNo_VisionStage[i] = 0;
	}
	gData.sLotID_Tansfer[2] = gData.sLotID_LoadStage[2] = gData.sLotID_LoadPicker[2] = gData.sLotID_UnloadPicker[2] = gData.sLotID_NGTray[2] = gData.sLotID_GoodTray[2] = gData.sLotID_VisionStage[4] = ""; 

	gLot.nJobCycle = gLot.nJobStatus = gLot.nUnLastPortID = gData.nCVJobSeq[0] = gData.nCVJobSeq[1] = 0;
	for(int i=0; i<30; i++) {
		gLot.nCmCount[i] = gLot.nLotStatus[i] = gLot.nGoodCount[i] = gLot.nNgCount[i] = gMes.nLotStatus[i] = 0;
		gLot.nUnlGdTrayCount[i] = gLot.nUnlNGTrayCount[i] = gLot.nOutTrayCnt[i][0] = gLot.nOutTrayCnt[i][1] = gLot.nSkipCount[i] = 0;
		gLot.nErrorCount[i] = gLot.dwErrorTime[i] = gLot.dwStopTime[i] = 0;
		gLot.sLotID[i] = gLot.sLLStartTime[i] = gLot.sLLEndTime[i] = "";
		gLot.sCarID_LD[i] = gLot.sCarID_NG[i] = gLot.sCarID_GD[i] = "";
		gLot.sMZID_LD[i] = gLot.sMZID_NG[i] = gLot.sMZID_GD[i] = "";
		gLot.nSlotNo_LD[i] = gLot.nSlotNo_NG[i] = gLot.nSlotNo_GD[i] = gLot.nMZLastCar[i] = 0;
		gLot.bEmptyTray[i] = FALSE;
		for (int j = 0; j < 3; j++) gLot.nTrayPutCnt[i][j] = 0;
	}

	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 10; j++) {
			for (int k = 0; k < 40; k++) {
				gMes.nCmResult[i][j][k] = gLot.nJudge_R[i][j][k] = gLot.nImageCnt[i][j][k][0] = gLot.nImageCnt[i][j][k][1] = gLot.nFOcapExist[i][j][k] = 0; gLot.sBarCode[i][j][k] = "";
				for (int l = 0; l < 6; l++) { gLot.nJudge_I[i][j][k][l] = 0; gLot.sNGCode_I[i][j][k][l] = ""; }
				for (int l = 0; l <10; l++)	  gLot.nHistory[i][j][k][l] = 0;
			}
		}
	}
	for (int i = 0; i < 30; i++) for (int j = 0; j <15; j++) gLot.nRosJugCount[i][j] = 0;
	for (int i = 0; i < 2; i++) for (int j = 0; j < 7; j++) gData.dElevatorZPos[i][j] = 0.0;
	for (int i = 0; i < 30; i++) for (int j = 0; j <20; j++) { gLot.nOcapCount[i][j] = gLot.nFOcapCount[i][j] = 0; }

	gData.sGoodCarrierID = gData.sMZID_Tansfer = ""; gData.nSlotNo_LDMZ = gData.nSlotNo_Tansfer = gData.nCarrierStart = gLot.nMZCountGD = gLot.nMZCountNG = gLot.nJobLog = 0;
	for (int i = 0; i <10; i++) { if(i==5) continue; gData.sMZID[i] = ""; }
	for (int i = 0; i < 2; i++) { gData.sMZID_LoadStage[i] = gData.sMZID_NGElevator[i] = ""; }
	for (int i = 0; i < 2; i++) { gData.nSlotNo_LoadStage[i] = 0; }
	for (int i = 0; i < 2; i++) for (int j = 0; j < 2; j++) gData.dAlignOffset[i][j] = 0.0;

	g_objDispatcher.Reset(0);
	m_bElvCarDown = FALSE;
	for (int i = 0; i < 30; i++) for (int k = 0; k < 40; k++) for (int m = 0; m < 5; m++) for (int n = 0; n < 20; n++) gNG->sNGCode[i][0][k][m][n] = "";
	for (int i = 0; i < 30; i++) for (int k = 0; k < 40; k++) { gNG->sModuleID[i][k] = gNG->sHaimNGCd[i][k] = gNG->sCosmeticNG[i][k] = ""; gLot.nMarginal[i][k] = 0; }
	for(int i=0; i< 5; i++) gData.nNG_MC[i][1] = 0;

	if(nType == 0 )
	{			
		memset(gCap.nDefectCnt_Cosmetic, 0,sizeof(int)*50*50*8);
		memset(gCap.nGoodCnt_Cosmetic, 0,sizeof(int)*50*8);
			
		memset(gCap.nTotalCnt_MZ, 0,sizeof(int)*50);
		memset(gCap.nGoodCnt_MZ, 0,sizeof(int)*50);
		memset(gCap.nNGCnt_MZ, 0,sizeof(int)*50);
		
		gCap.nLotIndex_NG = 0;
		gCap.nLotIndex_Good = 0;

		gCap.nMZIdx_Cosmetic = 0;
		gCap.nOcapCarrierIndex = 0;

		memset(gCap.nDefectCntInCarr_Cosmetic, 0,sizeof(int)*50*28);
		memset(gCap.nGoodCntInCarr_Cosmetic, 0,sizeof(int)*28);
		
		gCap.bErrorShowDone = FALSE;
		gCap.bOCAPDone[0] = FALSE;
		gCap.bOCAPDone[1] = FALSE;
		gCap.bOCAPDone[2] = FALSE;
	}

	



//	CString strLog;
//	strLog.Format("[SequenceMain] Scan Wait(%0.4lf) Count(%d) Inspection Wait(%0.4lf) Count(%d)", gData.dScanTime, gData.nScanTimeOverCnt, gData.dInspTime, gData.nInspectionOverCnt);
//	g_objLogFile.Save_HandlerLog(strLog);
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 0, NULL);
}

///////////////////////////////////////////////////////////////////////////////

BOOL CSequenceMain::Search_Module(CString sLotID, CString sModuleID)
{
	if (gData.bJahwa) return TRUE;
	if (!m_pEquipData->bUseMES) return TRUE;
	if (sModuleID.GetLength() < 20) return TRUE;

	for(int i=0; i<30; i++) {
		if (gLot.sLotID[i] == sLotID) {
			for(int j=0; j<40; j++) {
				if (gNG->sModuleID[i][j] == sModuleID) return TRUE;
			}
			return FALSE;	//NG처리,허수배출
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Search_Lot(int nType)
{
//	if (gData.bCycleStop) return FALSE;
	int nPortNo = 0, nJobNo = 0;
	int nSNo = gLot.nJobCycle;
	for(int i=nSNo; i<30; i++) {
		if (gLot.nCmCount[i] > 0 && gLot.nLotStatus[i] < 1) {
			nJobNo = i; nPortNo = i+1; break;
		}
	}
	if (nPortNo == 0) {
		for(int i=0; i<nSNo; i++) {
			if (gLot.nCmCount[i] > 0 && gLot.nLotStatus[i] < 1) {
				nJobNo = i; nPortNo = i+1; break;
			}
		}
	}
	if (nPortNo == 0 || nPortNo > 30) return FALSE;
	
	gLot.nJobCycle = nPortNo;
	gLot.nLotStatus[gLot.nJobCycle-1] = 1;
	return TRUE;
}

BOOL CSequenceMain::Check_MZTransfer(int &nFrom, int &nToTarget)
{
	//1순위 MZ-Good(5) -> CV-Unload(6)
	//2순위 MZ-NG(4)   -> CV-Unload(6)
	//3순위 MZ-Bufer(3)-> MZ-Good(4,5)
	//4순위 CV-Load(1) -> MZ-Load(2)
	//5순위 CV-Load(1) -> MZ-NG(4)
	//6순위 MZ-Load(2) -> MZ-Good, MZ-Bufer(5,3)

	//1순위 MZ-Good(5) -> CV-Unload(6)
	nFrom = nToTarget = 0;
	if (m_nGDMZElevatorCase == 50) {
		nFrom = 5; nToTarget = 6; return TRUE;
	}

	//2순위 MZ-NG(4)   -> CV-Unload(6)
	if (m_nNGMZElevatorCase == 50) {
		nFrom = 4; nToTarget = 6; return TRUE;
	}

	//3순위 MZ-Bufer(3)-> MZ-Good(5)
	if (m_pDX18->iMZBufferExist && m_pDX18->iLDMZElevatorExist) {
		if (!m_pDX18->iGDMZElevatorExist && m_nGDMZElevatorCase == 0) {
			nFrom = 3; nToTarget = 5; return TRUE;
		}
	}

	//4순위 CV-Load(1) -> MZ-Load(2)
	if (gData.nCVElevatorFrom == 1 && m_nLDCVElevatorCase == 20 && m_nLDMZElevatorCase == 0) {
		nFrom = 1; nToTarget = 2; return TRUE;
	}

	//5순위 CV-Load(1) -> MZ-NG(4)
	if (gData.nCVElevatorFrom == 2 && m_nLDCVElevatorCase == 20) {
		if (!m_pDX18->iNGMZElevatorExist && m_nNGMZElevatorCase == 0) {
			nFrom = 1; nToTarget = 4; return TRUE;
		}
	}

	//6순위 MZ-Load(2) -> MZ-Good, MZ-Bufer(5,3)
	if (m_nLDMZElevatorCase == 50) {
		if (!m_pDX18->iMZBufferExist) {
			nFrom = 2; nToTarget = 3; return TRUE;
		}
		if (!m_pDX18->iGDMZElevatorExist && m_nGDMZElevatorCase == 0) {
			nFrom = 2; nToTarget = 5; return TRUE;
		}
	}

	if (nFrom > 0 && nToTarget > 0) return TRUE;
	else								return FALSE;
}

BOOL CSequenceMain::Check_Transfer1(int &nFrom, int &nTo, int &nPortNo)
{
//	gData.nTransferX1Pos;	//1(LS1),2(LS2),3(Buf),4(LE),5(NE),6(GE),7(NG1),8(NG2),9(Good1),10(Good2)
	//1순위 Stage(1,2) -> Buffer(3)
	//2순위 Load (4)   -> Stage(1,2)
	nFrom = nTo = nPortNo = 0;

	//1순위 Stage -> Buffer
	if (m_nLoadStage1Case == 60 && m_pDX04->iLoadStage1TrayExist) { nFrom = 1; nTo = 3; }
	if (m_nLoadStage2Case == 60 && m_pDX04->iLoadStage2TrayExist) { nFrom = 2; nTo = 3; }
	if (nFrom > 0 && nTo > 0) return TRUE;

	//2순위 Load  -> Stage
	if (m_nLDMZElevatorCase == 30 && m_nLoadStage1Case == 0) { nFrom = 4; nTo = 1; }
	if (m_nLDMZElevatorCase == 30 && m_nLoadStage2Case == 0) { nFrom = 4; nTo = 2; }
	if (nFrom > 0 && nTo > 0) return TRUE;

	return FALSE;
}

BOOL CSequenceMain::Check_Transfer2(int &nFmTarget, int &nToTarget, int &nPortNo)
{
//	gData.nTransferX2Pos;	//1(LS1),2(LS2),3(Buf),4(LE),5(NE),6(GE),7(NG1),8(NG2),9(Good1),10(Good2)
	//1순위 Good-Stage12(9,10)-> Good Elevator(6)
	//2순위 NG-Stage12  (7,8) -> NG Elevator(5)
	//3순위 Good Elevator(6)  -> Good,NG Stage(7~10)
	//4순위 NG Elevator  (5)  -> Good,NG Stage(7~10)
	//5순위 Buffer       (3)  -> Good,NG Stage(7~10)
	//6순위 Good Elevator(6)  -> Buffer(3)
	//7순위 NG Elevator  (5)  -> Buffer(3)
	if (Check_ModuleEmpty()) return FALSE;

	//1순위 Good-Stage12 -> Good Elevator
	nFmTarget = nToTarget = nPortNo = 0;
	if (m_nGoodStage1Case == 60 && m_pDX12->iGoodStage1TrayExist && m_nGDMZElevatorCase == 70) { nFmTarget =  9; nToTarget = 6; }
	if (m_nGoodStage2Case == 60 && m_pDX12->iGoodStage2TrayExist && m_nGDMZElevatorCase == 70) { nFmTarget = 10; nToTarget = 6; }
	if (nFmTarget > 0 && nToTarget > 0) return TRUE;

	//2순위 NG-Stage12   -> NG Elevator (NG-MZ Change)
	if (m_nNGStage1Case == 60 && m_pDX11->iNGStage1TrayExist && m_nNGMZElevatorCase == 70) { if (Check_NGMZMZEnd(gData.nPortNo_NGTray[0])) m_nNGMZElevatorCase = 40; }
	if (m_nNGStage2Case == 60 && m_pDX11->iNGStage2TrayExist && m_nNGMZElevatorCase == 70) { if (Check_NGMZMZEnd(gData.nPortNo_NGTray[1])) m_nNGMZElevatorCase = 40; }
	nFmTarget = nToTarget = nPortNo = 0;
	if (m_nNGStage1Case == 60 && m_pDX11->iNGStage1TrayExist && (m_nNGMZElevatorCase == 23 || m_nNGMZElevatorCase == 70)) { nFmTarget = 7; nToTarget = 5; }
	if (m_nNGStage2Case == 60 && m_pDX11->iNGStage2TrayExist && (m_nNGMZElevatorCase == 23 || m_nNGMZElevatorCase == 70)) { nFmTarget = 8; nToTarget = 5; }
	if (nFmTarget > 0 && nToTarget > 0) return TRUE;

	//3순위 Good Elevator-> Good,NG Stage(E)
	if (m_nGDMZElevatorCase == 30) {
		if (m_nNGStage1Case == 0 && !m_pDX11->iNGStage1TrayExist)   { nFmTarget = 6; nToTarget =  7; }
		if (m_nNGStage2Case == 0 && !m_pDX11->iNGStage2TrayExist)   { nFmTarget = 6; nToTarget =  8; }
		if (m_nGoodStage1Case == 0 && !m_pDX12->iGoodStage1TrayExist) { nFmTarget = 6; nToTarget =  9; }
		if (m_nGoodStage2Case == 0 && !m_pDX12->iGoodStage2TrayExist) { nFmTarget = 6; nToTarget = 10; }
	}
	if (nFmTarget > 0 && nToTarget > 0) return TRUE;

	//4순위 NG Elevator  -> Good,NG Stage(E)
	if (m_nNGMZElevatorCase == 30) {
		if (m_nNGStage1Case == 0 && !m_pDX11->iNGStage1TrayExist)   { nFmTarget = 5; nToTarget =  7; }
		if (m_nNGStage2Case == 0 && !m_pDX11->iNGStage2TrayExist)   { nFmTarget = 5; nToTarget =  8; }
		if (m_nGoodStage1Case == 0 && !m_pDX12->iGoodStage1TrayExist) { nFmTarget = 5; nToTarget =  9; }
		if (m_nGoodStage2Case == 0 && !m_pDX12->iGoodStage2TrayExist) { nFmTarget = 5; nToTarget = 10; }
	}
	if (nFmTarget > 0 && nToTarget > 0) return TRUE;

	//5순위 Buffer       -> Good,NG Stage(E)
	if (m_nElevator2Case == 20 && m_pDX00->iElevator2TrayExist) {
		if (m_nNGStage1Case == 0 && !m_pDX11->iNGStage1TrayExist)   { nFmTarget = 3; nToTarget =  7; }
		if (m_nNGStage2Case == 0 && !m_pDX11->iNGStage2TrayExist)   { nFmTarget = 3; nToTarget =  8; }
		if (m_nGoodStage1Case == 0 && !m_pDX12->iGoodStage1TrayExist) { nFmTarget = 3; nToTarget =  9; }
		if (m_nGoodStage2Case == 0 && !m_pDX12->iGoodStage2TrayExist) { nFmTarget = 3; nToTarget = 10; }
	}
	if (nFmTarget > 0 && nToTarget > 0) return TRUE;

	//6순위 Good Elevator-> Buffer(E)
	if (m_nGDMZElevatorCase == 30 && m_nElevator2Case == 20) { nFmTarget = 6; nToTarget = 3; return TRUE; }

	//7순위 NG Elevator  -> Buffer(E)
	if (m_nNGMZElevatorCase == 30 && m_nElevator2Case == 20) { nFmTarget = 5; nToTarget = 3; return TRUE; }

	//Elevator Carrier유무 Check
	if ((m_nNGStage1Case == 0 && !m_pDX11->iNGStage1TrayExist) ||
		(m_nNGStage2Case == 0 && !m_pDX11->iNGStage2TrayExist) ||
		(m_nGoodStage1Case == 0 && !m_pDX12->iGoodStage1TrayExist) ||
		(m_nGoodStage2Case == 0 && !m_pDX12->iGoodStage2TrayExist)) {
		if (m_nElevator2Case == 20 && !m_pDX00->iElevator2TrayExist) {
			if (m_nNGMZElevatorCase == 70 && m_nGDMZElevatorCase == 70) {
				g_objCommon.Show_Error(2190);
			}
		}
	}

	if (m_nElevator2Case == 0) m_nElevator2Case = 1;
	return FALSE;
}

BOOL CSequenceMain::Check_TrayEmpty()
{
	if (m_pDX03->iTransferLTrayExist) return FALSE;
	if (m_pDX03->iTransferRTrayExist) return FALSE;
	if (m_pDX04->iLoadStage1TrayExist) return FALSE;
	if (m_pDX04->iLoadStage2TrayExist) return FALSE;
	if (m_pDX11->iNGStage1TrayExist) return FALSE;
	if (m_pDX11->iNGStage2TrayExist) return FALSE;
	if (m_pDX12->iGoodStage1TrayExist) return FALSE;
	if (m_pDX12->iGoodStage2TrayExist) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_ModuleEmpty()
{
	for (int i=0; i<2; i++) {
		if (gData.sLotID_Tansfer[i].GetLength()>0) return FALSE;
		if (gData.sLotID_LoadStage[i].GetLength()>0) return FALSE;
		if (gData.sLotID_LoadPicker[i].GetLength()>0) return FALSE;
		if (gData.sLotID_UnloadPicker[i].GetLength()>0) return FALSE;
		if (gData.sLotID_NGTray[i].GetLength()>0) return FALSE;
		if (gData.sLotID_GoodTray[i].GetLength()>0) return FALSE;
		if (gData.sLotID_UnMZ[i].GetLength()>0) return FALSE;
	}
	for (int i=0; i<4; i++) {
		if (gData.sLotID_VisionStage[i].GetLength()>0) return FALSE;
	}

	if (gData.nCarrierStart > 0) return FALSE;
	if (m_nLDMZElevatorCase != 0 && m_nLDMZElevatorCase != 50) return FALSE;
	if (m_nNGMZElevatorCase == 30 && m_nElevator2Case == 20) return FALSE;
	if (m_nTransfer1Case > 0 || m_nTransfer2Case > 0) return FALSE;
	if (m_pDX18->iLDMZElevatorExist) return FALSE;
//	if (m_pDX16->iLDVC1FStop) return FALSE;
	if (m_nMZTransferCase > 0 || m_nLDCVElevatorCase > 0) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_EmptyUnloadPicker()
{
	if (gData.sLotID_Tansfer[0].GetLength()>0) return FALSE;
	for (int i=0; i<2; i++) {
		if (gData.sLotID_LoadStage[i].GetLength()>0) return FALSE;
		if (gData.sLotID_LoadPicker[i].GetLength()>0) return FALSE;
		if (gData.sLotID_UnloadPicker[i].GetLength()>0) return FALSE;
	}
	for (int i=0; i<4; i++) {
		if (gData.sLotID_VisionStage[i].GetLength()>0) return FALSE;
	}	
	return TRUE;
}

BOOL CSequenceMain::Check_LoadEmptyMZ()
{
	for (int i=0; i<2; i++) {
		if (gData.sLotID_LoadStage[i].GetLength()>0) return FALSE;
		if (gData.sLotID_LoadPicker[i].GetLength()>0) return FALSE;
		if (gData.sLotID_UnloadPicker[i].GetLength()>0) return FALSE;
		if (gData.sLotID_NGTray[i].GetLength()>0) return FALSE;
		if (gData.sLotID_GoodTray[i].GetLength()>0) return FALSE;
	}
	for (int i=0; i<4; i++) {
		if (gData.sLotID_VisionStage[i].GetLength()>0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_LotEndTray(CString sLotID, int nPortNo)
{
	if (sLotID == "" || nPortNo < 1) return FALSE;

	for (int i=0; i<2; i++) {
		if (sLotID == gData.sLotID_Tansfer[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadStage[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadPicker[i]) return FALSE;
		if (sLotID == gData.sLotID_UnloadPicker[i]) return FALSE;
		if (sLotID == gData.sLotID_GoodTray[i]) return FALSE;
		if (sLotID == gData.sLotID_NGTray[i]) return FALSE;
		if (sLotID == gData.sLotID_UnMZ[i]) return FALSE;
	}
	for (int i=0; i<4; i++) {
		if (sLotID == gData.sLotID_VisionStage[i]) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_LotEndCarrier(CString sLotID, int nPortNo)
{
	if (sLotID == "" || nPortNo < 1) return FALSE;

	for (int i=0; i<2; i++) {
		if (sLotID == gData.sLotID_Tansfer[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadStage[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadPicker[i]) return FALSE;
		if (sLotID == gData.sLotID_UnloadPicker[i]) return FALSE;
//		if (sLotID == gData.sLotID_GoodTray[i]) return FALSE;
		if (sLotID == gData.sLotID_NGTray[i]) return FALSE;
//		if (sLotID == gData.sLotID_UnMZ[i]) return FALSE;
	}
	for (int i=0; i<4; i++) {
		if (sLotID == gData.sLotID_VisionStage[i]) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_LotEndLast(CString sLotID)
{
	if (sLotID.GetLength() < 2) return TRUE;

	for (int i=0; i<2; i++) {
		if (sLotID == gData.sLotID_Tansfer[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadStage[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadPicker[i]) return FALSE;
		if (sLotID == gData.sLotID_UnloadPicker[i]) return FALSE;
		if (sLotID == gData.sLotID_GoodTray[i]) return FALSE;
		if (sLotID == gData.sLotID_NGTray[i]) return FALSE;
	}
	for (int i=0; i<4; i++) {
		if (sLotID == gData.sLotID_VisionStage[i]) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_LastCarrMZ(CString sMZID)
{
	if (sMZID.GetLength() < 2) return TRUE;
	
	if (sMZID == gTracking.sMZID_LoadMZElev)
	{
		return FALSE;
	}
	if (sMZID == gTracking.sMZID_Trasnfer1)
	{
		return FALSE;
	}
	if (sMZID == gData.sMZID_LoadStage[0])
	{
		return FALSE;
	}
	if (sMZID == gData.sMZID_LoadStage[1])
	{
		return FALSE;
	}
	
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if (sMZID == gTracking.sMZID_LoadStageModule[0][i][j]) return FALSE;
			if (sMZID == gTracking.sMZID_LoadStageModule[1][i][j]) return FALSE;
		}
	}

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < PICK_CNT; j++)
		{
			if (sMZID == gTracking.sMZID_LoadPicker[i][j]) return FALSE;
			if (sMZID == gTracking.sMZID_UnloadPicker[i][j]) return FALSE;
		}
	}

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < PICK_CNT; j++)
		{
			if (sMZID == gTracking.sMZID_InspectStage[i][j]) return FALSE;			
		}
	}
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if (sMZID == gTracking.sMZID_NGTray[i][j])
			{
				return FALSE;
			}
			if (sMZID == gTracking.sMZID_GoodTray[i][j])
			{
				return FALSE;
			}
			if (sMZID == gTracking.sMZID_Transfer2[i][j])
			{
				return FALSE;
			}
		}
	}

	//for(int i = 0; i < 28; i++)
	//{
	//	for(int j = 0; j < 10; j++)
	//	{
	//		for(int k = 0; k < 4; j++)
	//		{
	//			//if (sMZID == gTracking.sMZID_GoodMZElev[i][j][k]) return FALSE;
	//			//if (sMZID == gTracking.sMZID_NGMZElev[i][j][k]) return FALSE;				
	//		}
	//	}
	//}

	return TRUE;
}

BOOL CSequenceMain::Check_MZEndUnload(CString sLotID, int nPortNo, int nPickNo)
{
	if (sLotID.GetLength() < 1) return FALSE;

	if (sLotID == gData.sLotID_Tansfer[0]) return FALSE;
	for (int i=0; i<2; i++) {
		if (sLotID == gData.sLotID_LoadStage[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadPicker[i]) return FALSE;
	}
	for (int i=0; i<4; i++) 
	{
		if (sLotID == gData.sLotID_VisionStage[i]) return FALSE;
	}
	if (nPickNo == 1) { if (sLotID == gData.sLotID_UnloadPicker[1]) return FALSE; }
	else			  { if (sLotID == gData.sLotID_UnloadPicker[0]) return FALSE; }
	if (gLot.nMZLastCar[nPortNo-1] == 0) return FALSE;
	return TRUE;
}

BOOL CSequenceMain::Check_LotEndNGTray(CString sLotID, int nPortNo, int nNo)
{
	if (sLotID == "" || nPortNo < 1) return FALSE;
	if (gData.bCycleStop==FALSE) return FALSE;

	if (sLotID == gData.sLotID_Tansfer[0]) return FALSE;
	for (int i=0; i<2; i++) {
		if (sLotID == gData.sLotID_LoadStage[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadPicker[i]) return FALSE;
		if (sLotID == gData.sLotID_UnloadPicker[i]) return FALSE;
		if (sLotID == gData.sLotID_GoodTray[i]) return FALSE;
	}
	for (int i=0; i<4; i++) {
		if (sLotID == gData.sLotID_VisionStage[i]) return FALSE;
	}
	if (m_pEquipData->bUseCMCheck && sLotID == gData.sLotID_NGBuffer) return FALSE;
	if (m_pEquipData->bUseCMCheck == FALSE) {
		if (nNo == 1 && sLotID == gData.sLotID_NGTray[1]) return FALSE;
		if (nNo == 2 && sLotID == gData.sLotID_NGTray[0]) return FALSE;
	}

	return TRUE;
}

BOOL CSequenceMain::Check_LotEndENGTray(CString sLotID, int nPortNo)
{
	if (sLotID == "") return FALSE;
	if (gData.bCycleStop==FALSE) return FALSE;

	for (int i=0; i<2; i++) {
		if (sLotID == gData.sLotID_Tansfer[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadStage[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadPicker[i]) return FALSE;
		if (sLotID == gData.sLotID_UnloadPicker[i]) return FALSE;
		if (sLotID == gData.sLotID_GoodTray[i]) return FALSE;
		if (m_pEquipData->bUseCMCheck == FALSE && sLotID == gData.sLotID_NGTray[i]) return FALSE;
	}
	for (int i=0; i<4; i++) {
		if (sLotID == gData.sLotID_VisionStage[i]) return FALSE;
	}

	return TRUE;
}

BOOL CSequenceMain::Check_LotEndUnloadPicker(CString sLotID, int nPortNo)
{
	if (sLotID == "") return FALSE;
//	if (gData.bCycleStop==FALSE) return FALSE;

	if (sLotID == gData.sLotID_Tansfer[0]) return FALSE;
	for (int i=0; i<2; i++) {
		if (sLotID == gData.sLotID_LoadStage[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadPicker[i]) return FALSE;
		if (sLotID == gData.sLotID_UnloadPicker[i]) return FALSE;
	}
	for (int i=0; i<4; i++) {
		if (sLotID == gData.sLotID_VisionStage[i]) return FALSE;
	}
//	if (m_pEquipData->bUseCMCheck == FALSE && sLotID == gData.sLotID_NGBuffer) return FALSE;
	return TRUE;
}

BOOL CSequenceMain::Check_LotEndUnloadNG(CString sLotID, int nPortNo, int nPickNo)
{
	if (sLotID == "") return FALSE;
//	if (gData.bCycleStop==FALSE) return FALSE;

	if (sLotID == gData.sLotID_Tansfer[0]) return FALSE;
	for (int i=0; i<2; i++) {
		if (sLotID == gData.sLotID_LoadStage[i]) return FALSE;
		if (sLotID == gData.sLotID_LoadPicker[i]) return FALSE;
	}
	if (nPickNo == 1) { if (sLotID == gData.sLotID_UnloadPicker[1]) return FALSE; }
	else			  { if (sLotID == gData.sLotID_UnloadPicker[0]) return FALSE; }
	for (int i=0; i<4; i++) {
		if (sLotID == gData.sLotID_VisionStage[i]) return FALSE;
	}
	return TRUE;
}

void CSequenceMain::Set_NextLot(CString sLotID, DWORD dwTime)
{
	int nNextPortLot = 0;
	for (int i=0; i<2; i++) {
		if (gData.sLotID_GoodTray[i].GetLength() > 1 && gData.nPortNo_GoodTray[i] > 0 && sLotID != gData.sLotID_GoodTray[i]) {
			nNextPortLot = gData.nPortNo_GoodTray[i]; gLot.dwLLLotStart[nNextPortLot-1] = dwTime;
		}
	}
	for (int i=0; i<2; i++) {
		if (gData.sLotID_NGTray[i].GetLength() > 1 && gData.nPortNo_NGTray[i] > 0 && sLotID != gData.sLotID_NGTray[i]) {
			nNextPortLot = gData.nPortNo_NGTray[i]; gLot.dwLLLotStart[nNextPortLot-1] = dwTime;
		}
	}
	for (int i=0; i<2; i++) {
		if (gData.sLotID_UnloadPicker[i].GetLength() > 1 && gData.nPortNo_UnloadPicker[i] > 0 && sLotID != gData.sLotID_UnloadPicker[i]) {
			nNextPortLot = gData.nPortNo_UnloadPicker[i]; gLot.dwLLLotStart[nNextPortLot-1] = dwTime;
		}
	}
	for (int i=0; i<4; i++) {
		if (gData.sLotID_VisionStage[i].GetLength() > 1 && gData.nPortNo_VisionStage[i] > 0 && sLotID != gData.sLotID_VisionStage[i]) {
			nNextPortLot = gData.nPortNo_VisionStage[i]; gLot.dwLLLotStart[nNextPortLot-1] = dwTime;
		}
	}
	for (int i=0; i<2; i++) {
		if (gData.sLotID_LoadPicker[i].GetLength() > 1 && gData.nPortNo_LoadPicker[i] > 0 && sLotID != gData.sLotID_LoadPicker[i]) {
			nNextPortLot = gData.nPortNo_LoadPicker[i]; gLot.dwLLLotStart[nNextPortLot-1] = dwTime;
		}
	}
	for (int i=0; i<2; i++) {
		if (gData.sLotID_LoadStage[i].GetLength() > 1 && gData.nPortNo_LoadStage[i] > 0 && sLotID != gData.sLotID_LoadStage[i]) {
			nNextPortLot = gData.nPortNo_LoadStage[i]; gLot.dwLLLotStart[nNextPortLot-1] = dwTime;
		}
	}
	if (gData.sLotID_Tansfer[0].GetLength() > 1 && gData.nPortNo_Tansfer[0] > 0 && sLotID != gData.sLotID_Tansfer[0]) {
		nNextPortLot = gData.nPortNo_Tansfer[0]; gLot.dwLLLotStart[nNextPortLot-1] = dwTime;
	}
}

BOOL CSequenceMain::Check_EmptyNGTray()
{
	for(int i=0; i<TRAY_CM_Y; i++) {
		for(int j=0; j<TRAY_CM_X; j++) {
			if (gData.InfoNgTray[i][j] > 0) return FALSE;
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_EmptyGoodTray()
{
	for(int i=0; i<TRAY_CM_Y; i++) {
		for(int j=0; j<TRAY_CM_X; j++) {
			if (gData.InfoGoodTray[i][j] > 0) return FALSE;
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_Load12Empy()
{
	for(int i=0; i<6; i++) {
		if (gLot.nCmCount[i] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_NGCarrierIn(int nSNo)
{
	if (m_nNGStage1Case == 60 && m_pDX11->iNGStage1TrayExist) {
		if (nSNo == 1) return TRUE;
//		if (nSNo == 8) return TRUE;
		if (gData.nPortNo_NGTray[0] > 0) return TRUE;
	}
	if (m_nNGStage2Case == 60 && m_pDX11->iNGStage2TrayExist) {
		if (nSNo == 1) return TRUE;
//		if (nSNo == 8) return TRUE;
		if (gData.nPortNo_NGTray[1] > 0) return TRUE;
	}
	return FALSE;
}

BOOL CSequenceMain::Check_NGMZMZEnd(int nPortNo)
{
	if (nPortNo < 1 || nPortNo > 30) return FALSE;
	if (m_nNGMZElevatorCase != 70) return FALSE;
	if (gData.sMZID_NGElevator[1].GetLength() < 2) return FALSE;
	if (gLot.sMZID_LD[nPortNo-1].GetLength() < 2) return FALSE;
	int nCount = 0;
	for(int i=0; i<8; i++) {
		if (gLot.nCarrierExist[1][i] == 1) return FALSE;
		if (gLot.nCarrierExist[1][i] > 0) nCount++;
	}
	if (nCount < 1) return FALSE;
	if (gData.sMZID_NGElevator[1] == gLot.sMZID_LD[nPortNo-1]) return FALSE;

	m_sLog.Format("Check_NGMZMZEnd MZ Change NG-MZ[%s] Count(%d) PortNo(%d)", gData.sMZID_NGElevator[1], nCount, nPortNo);
	g_objLogFile.Save_HandlerLog(m_sLog);
	return TRUE;
}

BOOL CSequenceMain::Check_LotRun(int nSNo, int nPortNo)
{
	int nPno;
	if (nSNo == 0 && gData.sLotID_LoadStage[1].GetLength() > 1 && gData.nPortNo_LoadStage[1] > 0) {
		nPno = gData.nPortNo_LoadStage[1];
		if (nPno > 0) {
			if (gLot.sRecipeName[nPno-1] == gLot.sRecipeName[nPortNo-1]) return TRUE;
			else														 return FALSE;
		}
	}
	if (nSNo == 1 && gData.sLotID_LoadStage[0].GetLength() > 1 && gData.nPortNo_LoadStage[0] > 0) {
		nPno = gData.nPortNo_LoadStage[0];
		if (nPno > 0) {
			if (gLot.sRecipeName[nPno-1] == gLot.sRecipeName[nPortNo-1]) return TRUE;
			else														 return FALSE;
		}
	}
	for (int i=0; i<2; i++) {
		if (gData.sLotID_LoadPicker[i].GetLength() > 1 && gData.nPortNo_LoadPicker[i] > 0) {
			nPno = gData.nPortNo_LoadPicker[1];
			if (nPno > 0) {
				if (gLot.sRecipeName[nPno-1] == gLot.sRecipeName[nPortNo-1]) return TRUE;
				else														 return FALSE;
			}
		}
	}
	for (int i=0; i<4; i++) {
		if (gData.sLotID_VisionStage[i].GetLength() > 1 && gData.nPortNo_VisionStage[i] > 0) {
			nPno = gData.nPortNo_VisionStage[1];
			if (nPno > 0) {
				if (gLot.sRecipeName[nPno-1] == gLot.sRecipeName[nPortNo-1]) return TRUE;
				else														 return FALSE;
			}
		}
	}
	for (int i=0; i<2; i++) {
		if (gData.sLotID_UnloadPicker[i].GetLength() > 1 && gData.nPortNo_UnloadPicker[i] > 0) {
			nPno = gData.nPortNo_UnloadPicker[1];
			if (nPno > 0) {
				if (gLot.sRecipeName[nPno-1] == gLot.sRecipeName[nPortNo-1]) return TRUE;
				else														 return FALSE;
			}
		}
	}
	return TRUE;
}

void CSequenceMain::Set_AlignData(int nPortNo, int nStageNo)
{
	int nCount = 0;
#ifdef AJIN_BOARD_USE
	for(int i=0; i<10; i++) {
		for(int j=0; j<4; j++) {
			nCount++;
			if (nCount <= gLot.nCmCount[nPortNo-1]) gData.InfoAlignTray[i][j] = 2;
		}
	}
#else
	if (gLot.nCmCount[nPortNo-1] > 20 || !m_pEquipData->bUseGoodSort) {
		for(int i=0; i<10; i++) {
			for(int j=0; j<4; j++) {
				nCount++;
				if (nCount <= gLot.nCmCount[nPortNo-1]) gData.InfoAlignTray[i][j] = 2;
			}
		}
	} else {
		if (gLot.nCmCount[nPortNo-1] <= 12) {
			for(int i=0; i<10; i=i+3) {
				for(int j=1; j<4; j++) {
					nCount++;
					gData.InfoAlignTray[i][j] = 2;
					if (nCount >= gLot.nCmCount[nPortNo-1]) { g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, NULL); return; }
				}
			}
		}
		if (gLot.nCmCount[nPortNo-1] <= 30) {
			for(int i=0; i<10; i=i++) {
				for(int j=1; j<4; j++) {
					nCount++;
					gData.InfoAlignTray[i][j] = 2;
					if (nCount >= gLot.nCmCount[nPortNo-1]) { g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, NULL); return; }
				}
			}
		}
	}
#endif
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, NULL);
}

BOOL CSequenceMain::Check_AlignData(int nPortNo)
{
	int nCMCount = 0;
	for(int i=0; i<10; i++) {
		for(int j=0; j<4; j++) {
			if (gData.InfoAlignTray[i][j] > 0) nCMCount++;
		}
	}
	if (nCMCount < 1) return FALSE;
	if (gLot.nCmCount[nPortNo-1] != nCMCount) return FALSE;

	return TRUE;
}

void CSequenceMain::Set_AlignToStage(int nPortNo, int nStageNo)
{
	gData.dAlignOffset[1][1] = gData.dAlignOffset[0][1];	gData.dAlignOffset[0][1] = 0.0;
	for(int i=0; i<10; i++) 
	{
		for(int j=0; j<4; j++) 
		{
			if (gData.InfoAlignTray[i][j] > 0) 
			{
				gData.InfoLoadTray[i][j] = 1;
				
				gTracking.sMZID_LoadStageModule[nStageNo][i][j] = gTracking.sMZID_LoadStage[nStageNo];
				m_sLog.Format("sMZID_LoadStageModule, x:%d y:%d - MZ ID:%s",j,i, gTracking.sMZID_LoadStageModule[nStageNo][i][j]);
				if (gTracking.sMZID_LoadStageModule[nStageNo][i][j] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);
			}
			else
			{
				gData.InfoLoadTray[i][j] = 0;
			}
		}
	}
	gTracking.sMZID_LoadStage[nStageNo] = "";
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, NULL);
	Init_AlignTray();
}

BOOL CSequenceMain::Check_LoadModule(int &nXNo, int &nYNo)
{
	for(int i=0; i<10; i++) {
		nYNo = nXNo = 0;
		int nType1 = 0, nType2 = 0;
		if (gData.InfoLoadTray[i][0] > 0) nType1 = 1;
		if (gData.InfoLoadTray[i][1] > 0) nType1 = 1;
		if (gData.InfoLoadTray[i][2] > 0) nType2 = 1;
		if (gData.InfoLoadTray[i][3] > 0) nType2 = 1;

		if (nType1 > 0) nXNo = 1;
		if (nType2 > 0) nXNo = 2;
		if (nType1 > 0 && nType2 > 0) nXNo = 3;
		if (nXNo > 0) { nYNo = i+1; return TRUE; }
	}
	return FALSE;
}

BOOL CSequenceMain::Check_EmptyLoadTray()
{
	for(int i=0; i<10; i++) {
		for(int j=0; j<4; j++) {
			if (gData.InfoLoadTray[i][j] > 0) return FALSE; 
		}
	}
	return TRUE;
}

int CSequenceMain::Check_MZCarrierExit(int nMZNo, int nType)	//Type 1:Out-Carrier, 2:In-Carrier, 3:Empty Carrier
{
	for(int i=0; i<8; i++) 
	{
		if (nType == 1 || nType == 2) 
		{
			if (gLot.nCarrierExist[nMZNo-1][i] == nType) return i+1;
		} 
		else if (nType == 3)
		{
			if (gLot.nCarrierExist[nMZNo-1][i] == 0) return i+1;
		} 
		else
		{
			if (gLot.nCarrierExist[nMZNo-1][i] > 0) return i+1;
		}
	}
/*
	if (nMZNo == 1) {
		for(int i=0; i<8; i++) {
			if (nType == 1 || nType == 2) {
				if (gLot.nCarrierExist[nMZNo-1][i] == nType) return i+1;
			} else if (nType == 3) {
				if (gLot.nCarrierExist[nMZNo-1][i] == 0) return i+1;
			} else {
				if (gLot.nCarrierExist[nMZNo-1][i] > 0) return i+1;
			}
		}
	} else {
		for(int i=7; i>=0; i--) {
			if (nType == 1 || nType == 2) {
				if (gLot.nCarrierExist[nMZNo-1][i] == nType) return i+1;
			} else if (nType == 3) {
				if (gLot.nCarrierExist[nMZNo-1][i] == 0) return i+1;
			} else {
				if (gLot.nCarrierExist[nMZNo-1][i] > 0) return i+1;
			}
		}
	}
*/
	return 0;
}

int CSequenceMain::Check_MZCarrierEmpty(int nMZNo)
{
	int nCarrieNo = 0;
//	if (nMZNo == 1) {
	if (nMZNo == 1 || nMZNo == 2) {
		for(int i=0; i<8; i++) {
			if (gLot.nCarrierExist[nMZNo-1][i] == 0) return i+1;
		}
	} else {
		for(int i=7; i>=0; i--) {
			if (gLot.nCarrierExist[nMZNo-1][i] == 0) return i+1;
		}
	}
	return nCarrieNo;
}

int CSequenceMain::Check_LoadMZ()
{
	int nLdMZExit = 0;
	if (m_pDX16->iLDCV1FCnt6) nLdMZExit = 6;
	if (m_pDX16->iLDCV1FCnt5) nLdMZExit = 5;
	if (m_pDX16->iLDCV1FCnt4) nLdMZExit = 4;
	if (m_pDX16->iLDCV1FCnt3) nLdMZExit = 3;
	if (m_pDX16->iLDCV1FCnt2) nLdMZExit = 2;
	if (m_pDX16->iLDCV1FCnt1) nLdMZExit = 1;
	if (gData.nMZDoorOpen[0] > 0) nLdMZExit = 0;
	return nLdMZExit;
}

int CSequenceMain::Check_EmptyMZ()
{
	int nEyMZExit = 0;
#ifndef AJIN_BOARD_USE
	m_pDX16->iLDCV2FCnt1 = TRUE;
#endif
	if (m_pDX16->iLDCV2FCnt6) nEyMZExit = 6;
	if (m_pDX16->iLDCV2FCnt5) nEyMZExit = 5;
	if (m_pDX16->iLDCV2FCnt4) nEyMZExit = 4;
	if (m_pDX16->iLDCV2FCnt3) nEyMZExit = 3;
	if (m_pDX16->iLDCV2FCnt2) nEyMZExit = 2;
	if (m_pDX16->iLDCV2FCnt1) nEyMZExit = 1;
	if (gData.nMZDoorOpen[1] > 0) nEyMZExit = 0;
//	if (m_pDX18->iNGMZElevatorExist && m_pDX18->iGDMZElevatorExist) nEyMZExit = 0;
	return nEyMZExit;
}

void CSequenceMain::Init_AlignTray()
{
	for(int i=0; i<10; i++) for(int j=0; j<4; j++) gData.InfoAlignTray[i][j] = 0;
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, NULL);
}

void CSequenceMain::Init_LoadTray()
{
	for(int i=0; i<10; i++) for(int j=0; j<4; j++) gData.InfoLoadTray[i][j] = 0;
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, NULL);
}

void CSequenceMain::Init_NgTray(int nStageNo)
{
	gData.sLotID_NGTray[nStageNo]  = "";
	gData.nTrayNo_NGTray[nStageNo] = gData.nPortNo_NGTray[nStageNo] = 0;

	for(int i=0; i<10; i++) for(int j=0; j<4; j++) gData.InfoNgTray[i][j] = 0;
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 3, NULL);
}

void CSequenceMain::Init_GoodTray(int nStageNo)
{
	gData.sLotID_GoodTray[nStageNo]  = "";
	gData.nTrayNo_GoodTray[nStageNo] = gData.nPortNo_GoodTray[nStageNo] = 0;

	for(int i=0; i<10; i++) for(int j=0; j<4; j++) gData.InfoGoodTray[i][j] = 0;
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 4, NULL);
}

void CSequenceMain::Init_ShiipAlignData(int nJobNo)
{
	if (nJobNo == 0) {
		for(int i=0; i<4; i++) {
			for(int j=0; j<3; j++) {
				gData.dAlignVisionX[i][j] = gData.dAlignVisionY[i][j] = 0.0;
			}
		}
	} else {
		for(int j=0; j<3; j++) {
			gData.dAlignVisionX[nJobNo-1][j] = gData.dAlignVisionY[nJobNo-1][j] = 0.0;
		}
	}
}

BOOL CSequenceMain::Check_InspectDone(int nPNo, int nPortNo, int nTrayNo)
{
	BOOL bUseVision = TRUE;
	if (nPNo < 1 || nPNo > 2 || nPortNo < 1 || nPortNo > 30 || nTrayNo < 1 || nTrayNo > 10) return FALSE;

	if (!m_pEquipData->bUseBottom && !m_pEquipData->bUseTop1 && !m_pEquipData->bUseTop2 && !m_pEquipData->bUseBotAng && !m_pEquipData->bUseTopAng) bUseVision = FALSE;
	if (bUseVision) 
	{
		Set_InspectDone(nPNo, nPortNo, nTrayNo);
		
		for (int i = 0; i < 8; i++) 
		{
			
			if (gData.InfoUnloadPick[nPNo-1][i] == 1) return FALSE;
		}
	}

	int nNGType[8] = { 3, 4, 5, 6, 11, 12, 15, 15 };
	int nMargin[8] = { 0, 1, 0, 2, 0, 3, 0, 4 };
	if (gData.bUseDryRun ||  gData.bCycleStop || gData.bResultTest || bUseVision == FALSE) 
	{
		int nCno = gData.InfoUnloadPick[nPNo-1][9] - 1;
		for(int i=0; i<8; i++) 
		{
			if (gData.InfoUnloadPick[nPNo-1][i] > 0) 
			{
				int nRand = g_objCommon.Get_Random(0, 99); 
				gData.InfoUnloadPick[nPNo-1][i] = (nRand < m_pEquipData->nDryRunNg ? 3 : 2);

				////gjc-Test (OCAP)
				//if (gData.InfoUnloadPick[nPNo-1][i] != 2)
				//{
				//	gCap.nFAIDefectCnt[0][nPortNo-1]++;

				//	if (i >=0 && i <=1) { gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][3] = 7; gLot.sNGCode_I[nPortNo-1][nTrayNo-1][i+nCno][3] = "DFAI-61"; }
				//	if (i >=2 && i <=3) { gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][3] = 7; gLot.sNGCode_I[nPortNo-1][nTrayNo-1][i+nCno][3] = "DFAI-14A"; }
				//	if (i >=4 && i <=5) { gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][3] = 7; gLot.sNGCode_I[nPortNo-1][nTrayNo-1][i+nCno][3] = "DFAI-44"; }
				//	if (i >=6 && i <=7) { gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][3] = 7; gLot.sNGCode_I[nPortNo-1][nTrayNo-1][i+nCno][3] = "FDFAI-44"; }

				//	if (gLot.sNGCode_I[nPortNo-1][nTrayNo-1][i+nCno][3].Left(5) == "FDFAI") 
				//	{
				//		gLot.sNGCode_I[nPortNo-1][nTrayNo-1][i+nCno][3] = gLot.sNGCode_I[nPortNo-1][nTrayNo-1][i+nCno][3].Right(gLot.sNGCode_I[nPortNo-1][nTrayNo-1][i+nCno][3].GetLength()-1);
				//		gLot.nFOcapExist[nPortNo-1][nTrayNo-1][i+nCno] = 1;						
				//	}						
				//}
				//else
				//{
				//	gCap.nGoodCnt[nPortNo-1]++;
				//}					

				//g_dlgOCAPCosmetic.Set_AddDEFECT();
				//g_dlgOCAPCosmetic.Set_AddMZData(gData.nPortNo_NGTray[n1NSNo-5]);



				if (!bUseVision && !gData.bResultTest)
				{
					if (m_pEquipData->bUseGoodSort) 
					{ 
						if (gData.InfoUnloadPick[nPNo-1][i] == 3) gData.InfoUnloadPick[nPNo-1][i] = 7; 
					}
					/*	if (gData.InfoUnloadPick[nPNo-1][i] != 2) 
					{
					gData.InfoUnloadPick[nPNo-1][i] = nNGType[i];
					if (gData.InfoUnloadPick[nPNo-1][i] == 15) gLot.sNGCode_I[nPortNo-1][0][i+nCno][0] = "VMAC000003";
					else									   gLot.sNGCode_I[nPortNo-1][0][i+nCno][0] = "NG-Code";
					for(int j=0; j<20; j++) 
					{
					gNG->sNGCode[nPortNo-1][0][i+nCno][2][j] = "NG-Code"; 
					}
					} 
					else 
					{
					gLot.nMarginal[nPNo-1][i+nCno] = nMargin[i];
					}*/
									
					

					
				}
			}
		}
	} 
	else 
	{
		for(int i=0; i<8; i++) 
		{
			if (gData.InfoUnloadPick[nPNo-1][i] == 1) return FALSE;
		}
	}
	return TRUE;
}

void CSequenceMain::Set_InspectDone(int nPNo, int nPortNo, int nTrayNo)
{
	if (nPNo < 1 || nPNo > 2 || nPortNo < 1 || nPortNo > 30 || nTrayNo < 1 || nTrayNo > 10 ) return;

	int nCno = gData.InfoUnloadPick[nPNo-1][9] - 1;
	for(int i=0; i<8; i++) {
		if ((i+nCno) >= 40) break;

		if (gData.InfoUnloadPick[nPNo-1][i] > 0) {
			if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][0] > 0) 
			{
				gData.InfoUnloadPick[nPNo-1][i] = gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][0];	

				// FAI-124 인 경우 
				if(gData.InfoUnloadPick[nPNo-1][i] == 19 && gLot.sNGCode_I[nPortNo-1][nTrayNo-1][i+nCno][0].Find("DFAI-124") > -1)
				{
					gData.InfoUnloadPick[nPNo-1][i] = 20; //FAI-124 인 경우, NG Tray Sort "J" = 20
				}
			}
		}
	}
}

void CSequenceMain::Set_ReInspectDone(int nPNo, int nPortNo, int nTrayNo)
{
	if (nPNo < 1 || nPNo > 2 || nPortNo < 1 || nPortNo > 30 || nTrayNo < 1 || nTrayNo > 10 ) return;

	int nCno = gData.InfoUnloadPick[nPNo-1][9] - 1;


#ifdef AJIN_BOARD_USE
	for(int i=0; i<8; i++) {
		if ((i+nCno) >= 40) break;

		if (gData.InfoUnloadPick[nPNo-1][i] == 1) {
			if (gLot.sBarCode[nPortNo-1][nTrayNo-1][i+nCno].GetLength() < 15) {
				gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][0]  = 12;	//BARCODE_NOREAD
				gLot.sNGCode_I[nPortNo-1][nTrayNo-1][i+nCno][0] = "BARCODE_NOREAD";
				gLot.nRosJugCount[nPortNo-1][5]++; gLot.nRosJugCount[nPortNo-1][7]++;
				gData.InfoUnloadPick[nPNo-1][i] = 12;
			} else {
				gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][0]  = 5;	//Vision Result Timeout
				gLot.sNGCode_I[nPortNo-1][nTrayNo-1][i+nCno][0] = "Vision Result Timeout";
				gLot.nRosJugCount[nPortNo-1][5]++; gLot.nRosJugCount[nPortNo-1][9]++;
				gData.InfoUnloadPick[nPNo-1][i] = 5; 
			}
			gData.nNG_MC[3][1]++;

			m_sLog.Format("Set_ReInspectDone MC[Unload Picker:%d LotID[%s] PortNo:%d TrayNo:%d CmNo:%d Bar:%s] NG_Count(%d) NG_Code(%d)",
				nPNo, gData.sLotID_UnloadPicker[nPNo-1], nPortNo, nTrayNo, (i+nCno+1), gLot.sBarCode[nPortNo-1][nTrayNo-1][i+nCno], gData.nNG_MC[3][1], gData.InfoUnloadPick[nPNo-1][i]);
			g_objLogFile.Save_HandlerLog(m_sLog);
		}
	}

	if (gData.nNG_MC[3][0] > 0 && gData.nNG_MC[3][0] <= gData.nNG_MC[3][1]) {
		gAlm.sAlmLotID[0] = gData.sLotID_UnloadPicker[nPNo-1]; gAlm.sAlmLotID[1] = "Inspect"; 
		g_objCommon.Show_Error(9204);
	}
#endif
	
}

void CSequenceMain::Set_InspectRequest(int nPNo, int nPortNo, int nTrayNo)
{
	if (nPNo < 1 || nPNo > 2 || nPortNo < 1 || nPortNo > 30 || nTrayNo < 1 || nTrayNo > 10 ) return;

	//Module 검사완료 Check
	int nCno = gData.InfoUnloadPick[nPNo-1][9] - 1;
	for(int i=0; i<8; i++) {
		if ((i+nCno) >= 40) break;

		if (gData.InfoUnloadPick[nPNo-1][i] == 1) {
			if (m_pEquipData->bUseBottom && gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][1] < 2) g_objInspector.Set_InspectRequest(INSPECTOR_PC1, "B1", gLot.sLotID[nPortNo-1], nPortNo, nTrayNo, i+nCno+1);
			if (m_pEquipData->bUseBotAng && gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][2] < 2) g_objInspector.Set_InspectRequest(INSPECTOR_PC1, "AG", gLot.sLotID[nPortNo-1], nPortNo, nTrayNo, i+nCno+1);
			if (m_pEquipData->bUseTop1   && gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][3] < 2) g_objInspector.Set_InspectRequest(INSPECTOR_PC2, "T1", gLot.sLotID[nPortNo-1], nPortNo, nTrayNo, i+nCno+1);
			if (m_pEquipData->bUseTopAng && gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][4] < 2) g_objInspector.Set_InspectRequest(INSPECTOR_PC2, "TG", gLot.sLotID[nPortNo-1], nPortNo, nTrayNo, i+nCno+1);
			if (m_pEquipData->bUseTop2   && gLot.nJudge_I[nPortNo-1][nTrayNo-1][i+nCno][5] < 2) g_objInspector.Set_InspectRequest(INSPECTOR_PC3, "T2", gLot.sLotID[nPortNo-1], nPortNo, nTrayNo, i+nCno+1);
		}
	}
}

void CSequenceMain::Set_ROSTime(int nPNo, int nPortNo, int nTrayNo)
{
	if (nPNo < 1 || nPNo > 2 || nPortNo < 1 || nPortNo > 30 || nTrayNo < 1 || nTrayNo > 10) return;
	if (gData.bUseDryRun || gData.bCycleStop || !m_pEquipData->bUseROS) return;

	int nCNo = gData.InfoUnloadPick[nPNo-1][9];
	for(int i = 0; i < 8; i++) {
		if (gData.InfoUnloadPick[nPNo-1][i] > 0) {
			g_objDispatcher.Set_TimeOver(nPortNo, nTrayNo, i + nCNo);
		}
	}
}

BOOL CSequenceMain::Check_ROSDone(int nPNo, int nPortNo, int nTrayNo)
{
	if (nPNo < 1 || nPNo > 2 || nPortNo < 1 || nPortNo > 30 || nTrayNo < 1 || nTrayNo > 10) return FALSE;

	if (!m_pEquipData->bUseROS && m_pEquipData->bUsePassGood) {
		for(int i=0; i<8; i++) {
			if (gData.InfoUnloadPick[nPNo-1][i] > 0) gData.InfoUnloadPick[nPNo-1][i] = 2;
		}
		return TRUE;
	}
	if (gData.bUseDryRun || gData.bCycleStop || !m_pEquipData->bUseROS) return TRUE;

	int nCNo = gData.InfoUnloadPick[nPNo-1][9];
	for(int i = 0; i < 8; i++) {
		if (gData.InfoUnloadPick[nPNo-1][i] > 0) {
			if (!g_objDispatcher.Is_JudgeDone(nPortNo, nTrayNo, i + nCNo)) return FALSE;
		}
	}
	Set_ROSDone(nPNo, nPortNo, nTrayNo);

	return TRUE;
}

void CSequenceMain::Set_ROSDone(int nPNo, int nPortNo, int nTrayNo)
{
	if (nPNo < 1 || nPNo > 2 || nPortNo < 1 || nPortNo > 30 || nTrayNo < 1 || nTrayNo > 10 ) return;

	int nCno = gData.InfoUnloadPick[nPNo-1][9] - 1;
	for(int i=0; i<8; i++) {
		if ((i+nCno) >= 40) break;

		if (gData.InfoUnloadPick[nPNo-1][i] > 0) {
			if (gLot.nJudge_R[nPortNo-1][nTrayNo-1][i+nCno] > 0) {
				gData.InfoUnloadPick[nPNo-1][i] = gLot.nJudge_R[nPortNo-1][nTrayNo-1][i+nCno];
			}
			if (m_pEquipData->bUsePassGood) gData.InfoUnloadPick[nPNo-1][i] = 2;
		}
	}
}

BOOL CSequenceMain::Check_JudgeDone(int nPNo, int nPortNo, int nTrayNo)
{
	if (nPNo < 1 || nPNo > 4 || nPortNo < 1 || nPortNo > 30 || nTrayNo < 1 || nTrayNo > 10) return FALSE;

	if (gData.bUseDryRun || gData.bCycleStop) return TRUE;
	if (!m_pEquipData->bUseBottom && !m_pEquipData->bUseTop1 && !m_pEquipData->bUseTop2 && !m_pEquipData->bUseBotAng && !m_pEquipData->bUseTopAng) return TRUE;

	int nCMno = gData.InfoVision[nPNo-1][9] - 1;
	for (int i = 0; i < 8; i++) {
		if (gData.InfoVision[nPNo-1][i] > 0) {
			if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMno+i][0] == 0) return FALSE;
		}
	}

	if (!m_pEquipData->bUseROS) return TRUE;

	int nCNo = gData.InfoVision[nPNo-1][9];
	for(int i = 0; i < 8; i++) {
		if (gData.InfoVision[nPNo-1][i] > 0) {
			if (!g_objDispatcher.Is_JudgeDone(nPortNo, nTrayNo, nCNo+i)) return FALSE;
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_ShipAlign(int &nJobNo)
{
	//1순위 Good Stage
	//2순위 NG Stage
	nJobNo = 0;
	if (m_nNGStage1Case   == 20) nJobNo = 1;
	if (m_nNGStage2Case   == 20) nJobNo = 2;
	if (m_nGoodStage1Case == 20) nJobNo = 3;
	if (m_nGoodStage2Case == 20) nJobNo = 4;

	if (nJobNo > 0) return TRUE;
	else			return FALSE;
}

BOOL CSequenceMain::Get_TrayPosition(int nPNo, int nJobNo, int nPosX, int nPosY, double &dToPosX, double &dToPosY)
{
	double dX[3], dY[3];
	if (nPNo < 1 || nPNo > 2 || nJobNo < 1 || nJobNo > 4) return FALSE;

	m_pEquipData = g_objDataManager.Get_pEquipData();
	m_pMoveData  = g_objDataManager.Get_pMoveData();

	//(0,0) (3,0) (3,9) 점 좌표계산 (30,28)
	if (nJobNo == 1) {	//NG1
		dX[0] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[5] : m_pMoveData->dUnloadPickerX2[5]);
		dY[0] = m_pMoveData->dNGStageY1[3];			//(0,0)
		dX[1] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[9] : m_pMoveData->dUnloadPickerX2[9]);
		dY[1] = m_pMoveData->dNGStageY1[4];			//(3,0)
		dX[2] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[10] : m_pMoveData->dUnloadPickerX2[10]);
		dY[2] = m_pMoveData->dNGStageY1[5];			//(3,9)
	}
	if (nJobNo == 2) {	//NG2
		dX[0] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[6] : m_pMoveData->dUnloadPickerX2[6]);
		dY[0] = m_pMoveData->dNGStageY2[3];			//(0,0)
		dX[1] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[11] : m_pMoveData->dUnloadPickerX2[11]);
		dY[1] = m_pMoveData->dNGStageY2[4];			//(3,0)
		dX[2] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[12] : m_pMoveData->dUnloadPickerX2[12]);
		dY[2] = m_pMoveData->dNGStageY2[5];			//(3,9)
	}
	if (nJobNo == 3) {	//Good1
		dX[0] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[7] : m_pMoveData->dUnloadPickerX2[7]);
		dY[0] = m_pMoveData->dGoodStageY1[3];		//(0,0)
		dX[1] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[13] : m_pMoveData->dUnloadPickerX2[13]);
		dY[1] = m_pMoveData->dGoodStageY1[4];		//(3,0)
		dX[2] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[14] : m_pMoveData->dUnloadPickerX2[14]);
		dY[2] = m_pMoveData->dGoodStageY1[5];		//(3,9)
	}
	if (nJobNo == 4) {	//Good2
		dX[0] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[8] : m_pMoveData->dUnloadPickerX2[8]);
		dY[0] = m_pMoveData->dGoodStageY2[3];		//(0,0)
		dX[1] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[15] : m_pMoveData->dUnloadPickerX2[15]);
		dY[1] = m_pMoveData->dGoodStageY2[4];		//(3,0)
		dX[2] = (nPNo == 1 ? m_pMoveData->dUnloadPickerX1[16] : m_pMoveData->dUnloadPickerX2[16]);
		dY[2] = m_pMoveData->dGoodStageY2[5]	;	//(3,9)
	}

	//(0,0) (3,0) (3,9) 위치에 Vision Align 편차 계산
	if (m_pEquipData->bUseAlign2) {
		dX[0] = dX[0] + gData.dAlignVisionX[nJobNo-1][0];
		dY[0] = dY[0] + gData.dAlignVisionY[nJobNo-1][0];
		dX[1] = dX[1] + gData.dAlignVisionX[nJobNo-1][1];
		dY[1] = dY[1] + gData.dAlignVisionY[nJobNo-1][1];
		dX[2] = dX[2] + gData.dAlignVisionX[nJobNo-1][2];
		dY[2] = dY[2] + gData.dAlignVisionY[nJobNo-1][2];
	}

	//Pitch, Offset 계산
	double dPitchX  = (dX[1] - dX[0]) / 3.0;
	double dPitchY  = (dY[2] - dY[1]) / 9.0;
	double dOffSetX = (dX[2] - dX[1]) / 9.0;
	double dOffSetY = (dY[1] - dY[0]) / 3.0;

	//X = X0 + PiatchX * PosX(0~3) + OffsetX * PosY;
	//Y = Y0 + PiatchY * PosY(0~9) + OffsetY * PosX;
	dToPosX = dX[0] + dPitchX * (double)(nPosX-1) + dOffSetX * (double)(nPosY-1);
	dToPosY = dY[0] + dPitchY * (double)(nPosY-1) + dOffSetY * (double)(nPosX-1);
/*
	m_sLog.Format("A2 Data [Picker:%d Job:%d Option:%d] PosXY(%d,%d) => (%0.3lf,%0.3lf) (%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf,%0.3lf)", nPNo, nJobNo, m_pEquipData->bUseAlign2, nPosX, nPosY, dToPosX, dToPosY,
													dX[0], dX[1], dX[2], dY[0], dY[1], dY[2]);
	g_objLogFile.Save_HandlerLog(m_sLog);
*/
	return TRUE;
}

/*
BOOL CSequenceMain::Get_TrayPosition(int nPNo, int nJobNo, int nPosX, int nPosY, double &dToPosX, double &dToPosY)
{
	int	   nI = nJobNo - 1;
	double dX[3], dY[3];
	if (nPNo < 1 || nPNo > 2 || nJobNo < 1 || nJobNo > 4) return FALSE;

	m_pEquipData = g_objDataManager.Get_pEquipData();
	m_pMoveData  = g_objDataManager.Get_pMoveData();

	//(0,0) (3,0) (3,9) 점 좌표계산
	if (nPNo == 1) {		//Unload Picker1
		if (nJobNo == 1) {	//NG1
			dX[0] = m_pMoveData->dUnloadPickerX1[5];									//(0,0)
			dY[0] = m_pMoveData->dNGStageY1[3];											//(0,0)
			dX[1] = m_pMoveData->dUnloadPickerX1[5] - (m_pEquipData->dTrayPitchX * 3);	//(3,0)
			dY[1] = m_pMoveData->dNGStageY1[3];											//(3,0)
			dX[2] = m_pMoveData->dUnloadPickerX1[5] - (m_pEquipData->dTrayPitchX * 3);	//(3,9)
			dY[2] = m_pMoveData->dNGStageY1[3]      + (m_pEquipData->dTrayPitchY * 9);	//(3,9)
		}
		if (nJobNo == 2) {	//NG2
			dX[0] = m_pMoveData->dUnloadPickerX1[6];									//(0,0)
			dY[0] = m_pMoveData->dNGStageY2[3];											//(0,0)
			dX[1] = m_pMoveData->dUnloadPickerX1[6] - (m_pEquipData->dTrayPitchX * 3);	//(3,0)
			dY[1] = m_pMoveData->dNGStageY2[3];											//(3,0)
			dX[2] = m_pMoveData->dUnloadPickerX1[6] - (m_pEquipData->dTrayPitchX * 3);	//(3,9)
			dY[2] = m_pMoveData->dNGStageY2[3]      + (m_pEquipData->dTrayPitchY * 9);	//(3,9)
		}
		if (nJobNo == 3) {	//Good1
			dX[0] = m_pMoveData->dUnloadPickerX1[7];									//(0,0)
			dY[0] = m_pMoveData->dGoodStageY1[3];										//(0,0)
			dX[1] = m_pMoveData->dUnloadPickerX1[7] - (m_pEquipData->dTrayPitchX * 3);	//(3,0)
			dY[1] = m_pMoveData->dGoodStageY1[3];										//(3,0)
			dX[2] = m_pMoveData->dUnloadPickerX1[7] - (m_pEquipData->dTrayPitchX * 3);	//(3,9)
			dY[2] = m_pMoveData->dGoodStageY1[3]    + (m_pEquipData->dTrayPitchY * 9);	//(3,9)
		}
		if (nJobNo == 4) {	//Good2
			dX[0] = m_pMoveData->dUnloadPickerX1[8];									//(0,0)
			dY[0] = m_pMoveData->dGoodStageY2[3];										//(0,0)
			dX[1] = m_pMoveData->dUnloadPickerX1[8] - (m_pEquipData->dTrayPitchX * 3);	//(3,0)
			dY[1] = m_pMoveData->dGoodStageY2[3];										//(3,0)
			dX[2] = m_pMoveData->dUnloadPickerX1[8] - (m_pEquipData->dTrayPitchX * 3);	//(3,9)
			dY[2] = m_pMoveData->dGoodStageY2[3]    + (m_pEquipData->dTrayPitchY * 9);	//(3,9)
		}
	}
	if (nPNo == 2) {		//Unload Picker2
		if (nJobNo == 1) {	//NG1
			dX[0] = m_pMoveData->dUnloadPickerX2[5];									//(0,0)
			dY[0] = m_pMoveData->dNGStageY1[3];											//(0,0)
			dX[1] = m_pMoveData->dUnloadPickerX2[5] - (m_pEquipData->dTrayPitchX * 3);	//(3,0)
			dY[1] = m_pMoveData->dNGStageY1[3];											//(3,0)
			dX[2] = m_pMoveData->dUnloadPickerX2[5] - (m_pEquipData->dTrayPitchX * 3);	//(3,9)
			dY[2] = m_pMoveData->dNGStageY1[3]      + (m_pEquipData->dTrayPitchY * 9);	//(3,9)
		}
		if (nJobNo == 2) {	//NG2
			dX[0] = m_pMoveData->dUnloadPickerX2[6];									//(0,0)
			dY[0] = m_pMoveData->dNGStageY2[3];											//(0,0)
			dX[1] = m_pMoveData->dUnloadPickerX2[6] - (m_pEquipData->dTrayPitchX * 3);	//(3,0)
			dY[1] = m_pMoveData->dNGStageY2[3];											//(3,0)
			dX[2] = m_pMoveData->dUnloadPickerX2[6] - (m_pEquipData->dTrayPitchX * 3);	//(3,9)
			dY[2] = m_pMoveData->dNGStageY2[3]      + (m_pEquipData->dTrayPitchY * 9);	//(3,9)
		}
		if (nJobNo == 3) {	//Good1
			dX[0] = m_pMoveData->dUnloadPickerX2[7];									//(0,0)
			dY[0] = m_pMoveData->dGoodStageY1[3];										//(0,0)
			dX[1] = m_pMoveData->dUnloadPickerX2[7] - (m_pEquipData->dTrayPitchX * 3);	//(3,0)
			dY[1] = m_pMoveData->dGoodStageY1[3];										//(3,0)
			dX[2] = m_pMoveData->dUnloadPickerX2[7] - (m_pEquipData->dTrayPitchX * 3);	//(3,9)
			dY[2] = m_pMoveData->dGoodStageY1[3]    + (m_pEquipData->dTrayPitchY * 9);	//(3,9)
		}
		if (nJobNo == 4) {	//Good2
			dX[0] = m_pMoveData->dUnloadPickerX2[8];									//(0,0)
			dY[0] = m_pMoveData->dGoodStageY2[3];										//(0,0)
			dX[1] = m_pMoveData->dUnloadPickerX2[8] - (m_pEquipData->dTrayPitchX * 3);	//(3,0)
			dY[1] = m_pMoveData->dGoodStageY2[3];										//(3,0)
			dX[2] = m_pMoveData->dUnloadPickerX2[8] - (m_pEquipData->dTrayPitchX * 3);	//(3,9)
			dY[2] = m_pMoveData->dGoodStageY2[3]    + (m_pEquipData->dTrayPitchY * 9);	//(3,9)
		}
	}

	//(0,0) (3,0) (3,9) 위치에 Vision Align 편차 계산
	if (m_pEquipData->bUseAlign2) {
		dX[0] = dX[0] + gData.dAlignVisionX[nI][0];
		dY[0] = dY[0] + gData.dAlignVisionY[nI][0];
		dX[1] = dX[1] + gData.dAlignVisionX[nI][1];
		dY[1] = dY[1] + gData.dAlignVisionY[nI][1];
		dX[2] = dX[2] + gData.dAlignVisionX[nI][2];
		dY[2] = dY[2] + gData.dAlignVisionY[nI][2];
	}

	//Pitch, Offset 계산
	double dPitchX  = (dX[1] - dX[0]) / 3.0;
	double dPitchY  = (dY[2] - dY[1]) / 9.0;
	double dOffSetX = (dX[2] - dX[1]) / 9.0;
	double dOffSetY = (dY[1] - dY[0]) / 3.0;

	//X = X0 + PiatchX * PosX(0~4) + OffsetX * PosY;
	//Y = Y0 + PiatchY * PosY(0~9) + OffsetY * PosX;
	dToPosX = dX[0] + dPitchX * (double)nPosX + dOffSetX * (double)nPosY;
	dToPosY = dY[0] + dPitchY * (double)nPosY + dOffSetY * (double)nPosX;

	m_sLog.Format("A2 Data [Picker:%d Job:%d Option:%d] PosXY(%d,%d) => (%0.3lf,%0.3lf)", nPNo, nJobNo, m_pEquipData->bUseAlign2, nPosX, nPosY, dToPosX, dToPosY);
	g_objLogFile.Save_HandlerLog(m_sLog);
	return TRUE;
}
*/
int CSequenceMain::Check_NGExist(int nPNo, int &nJNo)
{
	nJNo = 0;
	for(int i=0; i<8; i++) {
		if (gData.InfoUnloadPick[nPNo-1][i] > 2) {
			if (gData.InfoUnloadPick[nPNo-1][i] == 4) gData.InfoUnloadPick[nPNo-1][i] = 3;	//4->3 Convert
			nJNo = gData.InfoUnloadPick[nPNo-1][i];
			return (i+1);
		}
	}
	return 0;
}

BOOL CSequenceMain::Select_NGTrayPoketNo(int nType, int &nPosX, int &nPosY)
{	//Type => 3:NG, 4:ROS재검, 5:Barcode,MES, 6:치수불량123, 7:치수불량1, 8:치수불량2, 9:치수불량3, A:허수불량
	nPosX = nPosY = 0;
	int nTrayBaseTemp = 0;

	for(int i=0; i<10; i++) 
	{
		for(int j=0; j<4; j++) 
		{
			if (gData.InfoNgTray[i][j] == 0) 
			{
				if (!m_pEquipData->bUseNGSort || m_pEquipData->nTrayBase[i][j] == nType) 
				{
					nPosY = i + 1;	//10
					nPosX = j + 1;	//4
					return TRUE;
				}
				if (m_pEquipData->bUseNGSort && m_pEquipData->nTrayBase[i][j] == 6) 
				{
					if (nType == 7 || nType == 8 || nType == 9) {
						nPosY = i + 1;	//10
						nPosX = j + 1;	//4
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

void CSequenceMain::Search_NGFullPoint(int nType, int &nPosX, int &nPosY)
{
	/*int tempX, tempY, tempType;
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 4; j++ )
		{
			tempType = m_pEquipData->nTrayBase[i][j];
		}
	}
	*/
}

int CSequenceMain::Check_GoodExist(int nPNo, int &nPosX, int &nPosY)
{
	for(int i=0; i<8; i++) {
		if (gData.InfoUnloadPick[nPNo-1][i] >= 2) {
			int nNo = gData.InfoUnloadPick[nPNo-1][9] + i;
			nPosX = nNo % TRAY_CM_X;	if (nPosX==0) nPosX = 4;
			nPosY = nNo / TRAY_CM_X;	if (nPosX!=4) nPosY++;
			return (i+1);
		}
	}
	return 0;
}

BOOL CSequenceMain::Select_GoodTrayPoketNo(int nPNo, int nTNo, int &nPosX, int &nPosY)
{
	if (m_pEquipData->bUseGoodSort) {
//		if (Check_EmptyGoodTray() || nTNo+1 == gLot.nOutTrayCnt[nPNo][0]) {
			if (gData.InfoGoodTray[nPosY-1][nPosX-1] == 0) return TRUE;
//		}
	} else {
		nPosX = nPosY = 0;
		if (gData.bJahwa) { //1->40
			for(int i=0; i<10; i++) {
				for(int j=0; j<4; j++) {
					if (gData.InfoGoodTray[i][j] == 0) {
						nPosY = i + 1;	//10
						nPosX = j + 1;	//4
						return TRUE;
					}
				}
			}
		} else {	// 40->1
			for(int i=9; i>=0; i--) {
				for(int j=3; j>=0; j--) {
					if (gData.InfoGoodTray[i][j] == 0) {
						nPosY = i + 1;	//10
						nPosX = j + 1;	//4
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

BOOL CSequenceMain::Check_NGTrayFull()
{
	if(m_pEquipData->bUseNGSort)
	{
		//dahunj 나중에 바꿔야 해 
		for(int i=0; i<10; i++) 
		{
			for(int j=0; j<4; j++) 
			{
				if (gData.InfoNgTray[i][j] == 0) return FALSE; 
			}
		}
	}
	else
	{
		for(int i=0; i<10; i++) 
		{
			for(int j=0; j<4; j++) 
			{
				if (gData.InfoNgTray[i][j] == 0) return FALSE; 
			}
		}
	}	
	return TRUE;
}

BOOL CSequenceMain::Check_GooodTrayFull()
{
	for(int i=0; i<10; i++) 
	{
		for(int j=0; j<4; j++) 
		{
			if (gData.InfoGoodTray[i][j] == 0) return FALSE; 
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_EemptyLoadPicker(int nPNo)
{
	if (nPNo < 1 || nPNo > 2) return FALSE;
	for(int i=0; i<8; i++) {
		if (gData.InfoLoadPick[nPNo-1][i] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_EemptyVisionStage(int nVNo)
{
	if (nVNo < 1 || nVNo > 4) return FALSE;
	for(int i=0; i<8; i++) {
		if (gData.InfoVision[nVNo-1][i] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_ElevatorSearch(int nENo)
{
	if (gData.dElevatorZPos[0][nENo-1] <= 1.0) return TRUE;
	if (nENo == 1 && !m_pDX00->iElevator1TrayExist) return TRUE;
	if (nENo == 2 && !m_pDX00->iElevator2TrayExist) return TRUE;
	if (nENo == 3 && !m_pDX01->iElevator3TrayExist) return TRUE;
	if (nENo == 4 && !m_pDX01->iElevator4TrayExist) return TRUE;
	if (nENo == 5 && !m_pDX02->iElevator5TrayExist) return TRUE;
	if (nENo == 6 && !m_pDX02->iElevator6TrayExist) return TRUE;
	if (nENo == 7 && !m_pDX02->iElevator7TrayExist) return TRUE;

	if (fabs(gData.dElevatorZPos[1][nENo-1] - gData.dElevatorZPos[0][nENo-1]) >= (TRAY_PITCH*3.0)) return TRUE;
	if (fabs(gData.dElevatorZPos[1][nENo-1] - gData.dElevatorZPos[0][nENo-1]) >= (TRAY_PITCH+2.0)) return FALSE;
//	if (fabs(gData.dElevatorZPos[1][nENo-1] - gData.dElevatorZPos[0][nENo-1]) >= (TRAY_PITCH+1.0)) return FALSE;
	return TRUE;
}

void CSequenceMain::Set_JobTack(int nType)
{	
	if (gLot.nJobLog > 0) return;

	CString sLog;
	SYSTEMTIME time;
	GetLocalTime(&time);
	
	if (nType == 1) {	//Start
		gLot.sJobTotStartTime.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		gLot.dwJobStart = GetTickCount();
		return;
	}

	if (nType == 2) {	//End
		gLot.sJobTotEndTime.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		gLot.dwJobtEnd = GetTickCount();
		DWORD dwTime = gLot.dwJobtEnd - gLot.dwJobStart;
		gLot.dJobTotalTack = dwTime / 1000.0 / gLot.nJobCount;

		if (gLot.nJobNo < 0 || gLot.nJobNo >= 100) gLot.nJobNo = 0;
		gLot.nJobPortNo[gLot.nJobNo]	= 99;
		gLot.sJobLotID[gLot.nJobNo]		= "Job Total";
		gLot.nJobCmCount[gLot.nJobNo]	= gLot.nJobCount;
		gLot.nJobGoodCount[gLot.nJobNo]	= 0;
		gLot.nJobNgCount[gLot.nJobNo]	= 0;
		gLot.dJobRate[gLot.nJobNo]		= 0.0;
		gLot.sJobStartTime[gLot.nJobNo] = gLot.sJobTotStartTime;
		gLot.sJobEndTime[gLot.nJobNo]	= gLot.sJobTotEndTime;
		gLot.dJobTack[gLot.nJobNo]		= gLot.dJobTotalTack;
		gLot.nJobNo++; if (gLot.nJobNo < 0 || gLot.nJobNo >= 100) gLot.nJobNo = 0;

		g_dlgWork.PostMessage(UM_UPDATE_UPH, NULL, NULL);

		sLog.Format("%d,%s,%d,%d,%d,%0.1lf,%d,%d,%d,%d,%d,%d,%d,%s,%s,%0.5lf,0",
					0, "Job Total", gLot.nJobCount, 0, 0, 0.0, 0, 0, 0, 0, 0, 0, 0, gLot.sJobTotStartTime, gLot.sJobTotEndTime, gLot.dJobTotalTack);
		g_objLogFile.Save_JobListLog(sLog);
		g_objLogFile.Save_HandlerLog(sLog);
		gLot.nJobLog = nType;
		return;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BOOL CSequenceMain::Set_LotStart(CString sLotID, int nPortNo)
{
	if (nPortNo < 1 || nPortNo > 30) return TRUE;

	int nNo = nPortNo - 1;
	SYSTEMTIME time;
	GetLocalTime(&time);

	gLot.nLotStatus[nNo] = 2;
	gLot.sLLStartTime[nNo].Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	gLot.sMESStartTime[nNo].Format("%04d/%02d/%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	gLot.sLLSHHMMSS[nNo].Format("%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
	gLot.dwLLLotStart[nNo] = GetTickCount();

	gLot.sLLEndTime[nNo] = gLot.sMESEndTime[nNo] = "";
	gLot.nErrorCount[nNo] = gLot.dwErrorTime[nNo] = gLot.dwStopTime[nNo] = gLot.dwLLLotEnd[nNo] = 0;
	gLot.dLLTackTime[nNo] = 0.0;
	gLot.nRunPortNo = nPortNo;

//	g_objInspector.Set_LotStart(INSPECTOR_ALL, gLot.sLotID[nNo], nPortNo, 1, gLot.nCmCount[nNo], sRecipe, m_pEquipData->sPROG_VER, m_pEquipData->sPARA_VER);
	g_objDispatcher.Set_LotStart(nPortNo);
	gData.sLotID_Start = gLot.sLotID[nNo];

	return TRUE;
}

//nGTray 1:GoodTray1, 2:GoodTray2
void CSequenceMain::Set_LotEnd(CString sLotID, int nPortNo, CString sMZID, CString sCarID, CString sType)
{
	if (nPortNo < 1 || nPortNo > 30) return;

	int nNo = nPortNo - 1;
	if (gLot.sLotID[nNo] == "" || gLot.nLotStatus[nNo] == 3) {
		CString sLog;
		sLog.Format("[Set_LotEnd] Skip. Input(%s-%s-%s-%s) PortNo[%d] LotID[%s] Count[%d]", sLotID, sMZID, sCarID, sType, nPortNo, gLot.sLotID[nNo], gLot.nCmCount[nNo]);
		g_objLogFile.Save_HandlerLog(sLog);
		return;
	}
	gLot.nLotStatus[nNo] = 3;

	gLot.sRstLotID[nNo]     = gLot.sLotID[nNo];
	gLot.nRstCmCount[nNo]   = gLot.nCmCount[nNo];
	gLot.nRstGoodCount[nNo] = gLot.nGoodCount[nNo];
	gLot.nRstNgCount[nNo]   = gLot.nNgCount[nNo] + gLot.nSkipCount[nNo];
	gData.nRejectLotCount   = gLot.nNgCount[nNo] + gLot.nSkipCount[nNo];

	SYSTEMTIME time;
	GetLocalTime(&time);

	gLot.dwLLLotEnd[nNo] = GetTickCount();
	gLot.sLLEndTime[nNo].Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	gLot.sMESEndTime[nNo].Format("%04d/%02d/%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	gLot.sLLEHHMMSS[nNo].Format("%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);

	DWORD dwTime = gLot.dwLLLotEnd[nNo] - gLot.dwLLLotStart[nNo];
	gLot.dLLTackTime[nNo] = dwTime / 1000.0 / gLot.nRstCmCount[nNo];
	Set_NextLot(gLot.sLotID[nNo], gLot.dwLLLotEnd[nNo]);

	CString sLog, sGoodCarID, sGoodMZID;
	double dRate = (gLot.nCmCount[nNo] == 0 ? 0.0 : (gLot.nGoodCount[nNo] * 100.0) / gLot.nCmCount[nNo]);

	//"Time,PortNo,LotID,Count,NG_Count,Good_Count,Rate,Skip_Count,Bar_NoRead,RosJudge,RosGood,RosNG,RosRepair,RosTimeOver,Start_Time,End_Time,Tack\r\n");
	sLog.Format("%d,%s,%d,%d,%d,%0.1lf,%d,%d,%d,%d,%d,%d,%d,%s,%s,%0.5lf,%d",
				nPortNo, gLot.sLotID[nNo], gLot.nCmCount[nNo], gLot.nNgCount[nNo], gLot.nGoodCount[nNo], dRate, gLot.nRosJugCount[nNo][10], //gLot.nSkipCount[nNo],
				gLot.nRosJugCount[nNo][5], gLot.nRosJugCount[nNo][0], gLot.nRosJugCount[nNo][1], gLot.nRosJugCount[nNo][3], gLot.nRosJugCount[nNo][4], gLot.nRosJugCount[nNo][2],
				gLot.sLLStartTime[nNo], gLot.sLLEndTime[nNo], gLot.dLLTackTime[nNo], gLot.nRosJugCount[nNo][6]);
	g_objLogFile.Save_JobListLog(sLog);
	g_objLogFile.Save_HandlerLog(sLog);

	gLot.sELotID = gLot.sLotID[nNo];
	sLog.Format("%d,%d,%d,%d,%0.1lf,%d,%d,%d,%d,%d,%d,%d,%s,%s,%0.5lf,%d",
				nPortNo, gLot.nCmCount[nNo], gLot.nNgCount[nNo], gLot.nGoodCount[nNo], dRate, gLot.nRosJugCount[nNo][10], //gLot.nSkipCount[nNo],
				gLot.nRosJugCount[nNo][5], gLot.nRosJugCount[nNo][0], gLot.nRosJugCount[nNo][1], gLot.nRosJugCount[nNo][3], gLot.nRosJugCount[nNo][4], gLot.nRosJugCount[nNo][2],
				gLot.sLLStartTime[nNo], gLot.sLLEndTime[nNo], gLot.dLLTackTime[nNo], gLot.nRosJugCount[nNo][6]);
	g_objLogFile.Save_ECMLog(0, sLog);

	//Time \tLot_ID \tTotal \tGood \tNG \tRate \tSkip \tBar_NoRead \tRosRequest \tRosGood \tRosNg \tRosRepair \tRosTimeOut \tFiltering1 \tFiltering2 \tRateAvi \tRunTime \tTack\r\n");
	double dFiltering1 = (gLot.nRosJugCount[nNo][0] == 0 ? 0.0 : (gLot.nRosJugCount[nNo][1] * 100.0) / gLot.nRosJugCount[nNo][0]);
	double dFiltering2 = (gLot.nCmCount[nNo] == 0 ? 0.0 : (gLot.nRosJugCount[nNo][1] * 100.0) / gLot.nCmCount[nNo]);
	double dRateAvi = (gLot.nCmCount[nNo] == 0 ? 0.0 : ((gLot.nGoodCount[nNo] - gLot.nRosJugCount[nNo][1]) * 100.0) / gLot.nCmCount[nNo]);
	double dRunTime = dwTime / 1000.0;

	sLog.Format("\t%s \t%d \t%d \t%d \t%0.3lf \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%0.2lf \t%0.2lf \t%0.3lf \t%0.3lf \t%0.3lf",
		gLot.sLotID[nNo], gLot.nCmCount[nNo], gLot.nGoodCount[nNo], gLot.nNgCount[nNo], dRate,  gLot.nRosJugCount[nNo][10], gLot.nRosJugCount[nNo][5],
		gLot.nRosJugCount[nNo][0], gLot.nRosJugCount[nNo][1], gLot.nRosJugCount[nNo][3], gLot.nRosJugCount[nNo][4], gLot.nRosJugCount[nNo][2],
		dFiltering1, dFiltering2, dRateAvi, dRunTime, gLot.dLLTackTime[nNo]);
	g_objLogFile.Save_DailyLot(sLog);
	g_objLogFile.Save_LotLog(nNo+1);

	sLog.Format("%s,%d,%d,%d,%0.3lf,%d,%d,%d,%d,%d,%d,%d,%0.2lf,%0.2lf,%0.3lf,%0.3lf,%0.3lf",
		gLot.sLotID[nNo], gLot.nCmCount[nNo], gLot.nGoodCount[nNo], gLot.nNgCount[nNo], dRate,  gLot.nRosJugCount[nNo][10], gLot.nRosJugCount[nNo][5],
		gLot.nRosJugCount[nNo][0], gLot.nRosJugCount[nNo][1], gLot.nRosJugCount[nNo][3], gLot.nRosJugCount[nNo][4], gLot.nRosJugCount[nNo][2],
		dFiltering1, dFiltering2, dRateAvi, dRunTime, gLot.dLLTackTime[nNo]);
	g_objLogFile.Save_ECMDailyLot(sLog);

	//Time,호기,Lot ID,Lot Start,Lot End,Cycle Time,LM(EA),Run Time,Stop Time,Error Time,Error Count,수율,UPH,MTB\r\n");
	sLog.Format(",%s,%s,%s,%s,%0.3lf,%d,%0.3lf,%0.3lf,%0.3lf,%d,%0.2lf,%0.3lf,%0.3lf",
		m_pEquipData->sEquipName, gLot.sLotID[nNo], gLot.sLLSHHMMSS[nNo], gLot.sLLEHHMMSS[nNo], gLot.dLLTackTime[nNo], gLot.nCmCount[nNo], dRunTime,
		(double)(gLot.dwStopTime[nNo]) / 1000, (double)(gLot.dwErrorTime[nNo]) / 1000, gLot.nErrorCount[nNo], dRate, 3600 / gLot.dLLTackTime[nNo],
		(double)(dRunTime / 1000 / (gLot.nErrorCount[nNo] + 1)));
	g_objLogFile.Save_OperatingRatio(sLog);

	if (sType == "G") { sGoodMZID = sMZID;			sGoodCarID = sCarID; }
	else			  { sGoodMZID = m_sLastMZID;	sGoodCarID = m_sLastCarID; }

//	g_objMesAgent.Set_LotEnd(gLot.sLotID[nNo], gLot.sMZID_GD[nNo], gLot.sCarID_GD[nNo], gLot.sRecipeName[nNo], gLot.nCmCount[nNo], gLot.nGoodCount[nNo], gLot.nNgCount[nNo]+gLot.nSkipCount[nNo]);
//	g_objInspector.Set_LotEnd(INSPECTOR_ALL, gLot.sLotID[nNo], nPortNo);
	g_objDispatcher.Set_LotEnd(nPortNo);
	g_dlgOCAPFai.Set_AddMZData(nPortNo);
	g_dlgOCAPCosmetic.Set_AddMZData(nPortNo);

	gData.nDay_TotalCnt += gLot.nCmCount[nNo];
	gData.nDay_NGCnt += gLot.nNgCount[nNo];
	gData.nDay_GoodCnt += gLot.nGoodCount[nNo];

	CIniFileCS INI_EQP(gsCurrentDir + "\\System\\EquipData.ini");
	if (INI_EQP.Check_File()) {
		INI_EQP.Set_Integer("DAY_TOTAL", "COUNT", gData.nDay_TotalCnt);
		INI_EQP.Set_Integer("DAY_TOTAL", "NG_COUNT", gData.nDay_NGCnt);
		INI_EQP.Set_Integer("DAY_TOTAL", "GOOD_COUNT", gData.nDay_GoodCnt);
	}
	
	if (gLot.nJobNo < 0 || gLot.nJobNo >= 100) gLot.nJobNo = 0;
	gLot.nJobPortNo[gLot.nJobNo]	= nPortNo;
	gLot.sJobLotID[gLot.nJobNo]		= gLot.sLotID[nNo];
	gLot.nJobCmCount[gLot.nJobNo]	= gLot.nCmCount[nNo];
	gLot.nJobGoodCount[gLot.nJobNo]	= gLot.nGoodCount[nNo];
	gLot.nJobNgCount[gLot.nJobNo]	= gLot.nNgCount[nNo] + gLot.nSkipCount[nNo];
	gLot.dJobRate[gLot.nJobNo]		= dRate;
	gLot.sJobStartTime[gLot.nJobNo] = gLot.sLLStartTime[nNo];
	gLot.sJobEndTime[gLot.nJobNo]	= gLot.sLLEndTime[nNo];
	gLot.dJobTack[gLot.nJobNo]		= gLot.dLLTackTime[nNo];
	gLot.nJobNo++; if (gLot.nJobNo < 0 || gLot.nJobNo >= 100) gLot.nJobNo = 0;
/*
	sLog.Format("ID =>, %s,%d,%d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
				gLot.sLotID[nNo], nPortNo, gLot.nCmCount[nNo],
				gLot.sCarID_LD[nNo][0], gLot.sCarID_LD[nNo][1], gLot.sCarID_LD[nNo][2], gLot.sCarID_LD[nNo][3], gLot.sCarID_LD[nNo][4], gLot.sCarID_LD[nNo][5], gLot.sCarID_LD[nNo][6], gLot.sCarID_LD[nNo][7],
				gLot.sCarID_NG[nNo][0], gLot.sCarID_NG[nNo][1], gLot.sCarID_NG[nNo][2], gLot.sCarID_NG[nNo][3], gLot.sCarID_NG[nNo][4], gLot.sCarID_NG[nNo][5], gLot.sCarID_NG[nNo][6], gLot.sCarID_NG[nNo][7],
				gLot.sCarID_GD[nNo][0], gLot.sCarID_GD[nNo][1], gLot.sCarID_GD[nNo][2], gLot.sCarID_GD[nNo][3], gLot.sCarID_GD[nNo][4], gLot.sCarID_GD[nNo][5], gLot.sCarID_GD[nNo][6], gLot.sCarID_GD[nNo][7]);
	g_objLogFile.Save_HandlerLog(sLog);
*/
	if (gData.nRejectMaxCount > 0 && gData.nRejectLotCount >= gData.nRejectMaxCount)
			g_dlgWork.PostMessage(UM_LOT_END_MSG, nPortNo, 9);
//	else if (gLot.nCmCount[nNo] != gLot.nRstNgCount[nNo] + gLot.nRstGoodCount[nNo])
//			g_dlgWork.PostMessage(UM_LOT_END_MSG, nPortNo, 2);	//입력수량과 검사수량이 안맞았을때 알람메세지 보여준다.
	else	g_dlgWork.PostMessage(UM_LOT_END_MSG, nPortNo, 0);

	g_dlgWork.PostMessage(UM_UPDATE_UPH, NULL, NULL);
}

void CSequenceMain::Beep_Post(int nTime)
{
	DY_DATA_13 *pDY13 = g_objAJinAXL.Get_pDY13();
	pDY13->oBuzzerBit0 = m_pEquipData->bBuzzer[1][0];
	pDY13->oBuzzerBit1 = m_pEquipData->bBuzzer[1][1];
	pDY13->oBuzzerBit2 = m_pEquipData->bBuzzer[1][2];
	pDY13->oBuzzerBit3 = m_pEquipData->bBuzzer[1][3];
	pDY13->oBuzzerBit4 = m_pEquipData->bBuzzer[1][4];
	g_objAJinAXL.Write_Output(13);

 	Sleep(nTime);

	pDY13->oBuzzerBit0 = FALSE;
	pDY13->oBuzzerBit1 = FALSE;
	pDY13->oBuzzerBit2 = FALSE;
	pDY13->oBuzzerBit3 = FALSE;
	pDY13->oBuzzerBit4 = FALSE;
 	g_objAJinAXL.Write_Output(13);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//  1. (Error : 2100)
BOOL CSequenceMain::Run_Elevator1() //load tray 1
{
	static int	  nElevZAxisNo = AX_ELEVATOR_Z1;
	static double dElev1PrePos, dElev1PostPos, dElelavtorZ1, dElev1Positi;

	dElev1PrePos  = m_pMoveData->dElevatorZ1[1];
	dElev1PostPos = m_pMoveData->dElevatorZ1[2];
	if (gData.nElevatorOpen[1] > 0) return TRUE;
	if (m_nElevator1Case < 50 && !m_pDX00->iElevator1SlideClose) return TRUE;

	switch (m_nElevator1Case) {
	case 0:	// Wait
		m_tElevator1Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (!m_pDX00->iElevator1TrayExist) {
			m_nElevator1Case = 30; m_tElevator1Loop.Set_LoopTime(5000);
		} else {
			m_tElevator1Loop.Takt_Start();
			m_nElevator1Case = 10; m_tElevator1Loop.Set_LoopTime(5000);
		}
		break;

	case 10:
		if (m_pDX00->iElevator1Height1 || m_pDX00->iElevator1Height2) {
			m_tElevator1Loop.Takt_Save(1, 1); m_tElevator1Loop.Takt_Start();
			m_nElevator1Case = 12; m_tElevator1Loop.Set_LoopTime(5000);
		} else {
			dElev1Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_UD;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_UD);
			m_nElevator1Case++; m_tElevator1Loop.Set_LoopTime(30000);
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev1Positi);
			m_nElevator1Case = 10; m_tElevator1Loop.Set_LoopTime(5000);
		}
		break;
	case 12:
		if (m_pDX00->iElevator1Height1 || m_pDX00->iElevator1Height2) {
			dElev1Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_DN*-1.0;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_DN*-1.0);
			m_nElevator1Case++; m_tElevator1Loop.Set_LoopTime(30000);
		} else {
			m_tElevator1Loop.Takt_Save(1, 2); m_tElevator1Loop.Takt_Start();
			m_nElevator1Case = 14; m_tElevator1Loop.Set_LoopTime(30000);
		}
		break;
	case 13:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev1Positi);
			m_nElevator1Case = 12; m_tElevator1Loop.Set_LoopTime(5000);
		}
		break;
	case 14:
		if (m_pDX00->iElevator1Height1 && m_pDX00->iElevator1Height2) {
			m_tElevator1Loop.Takt_Save(1, 3); m_tElevator1Loop.Takt_Start();
			m_nElevator1Case = 16; m_tElevator1Loop.Set_LoopTime(30000);
		} else {
			dElev1Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_SL;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_SL);
			m_nElevator1Case++; m_tElevator1Loop.Set_LoopTime(30000);
		}
		break;
	case 15:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev1Positi);
			m_nElevator1Case = 14; m_tElevator1Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			dElev1Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElev1PostPos;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElev1PostPos);
			m_nElevator1Case++; m_tElevator1Loop.Set_LoopTime(30000);
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev1Positi);
			m_tElevator1Loop.Takt_Save(1, 4);
			gData.dElevatorZPos[1][0] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			m_nElevator1Case++; m_tElevator1Loop.Set_LoopTime(3000);
		}
		break;
	case 18:
		if (Check_ElevatorSearch(1)) {
			m_nElevator1Case = 20; m_tElevator1Loop.Set_LoopTime(30000);
		}
		break;

	case 20:	// Picker Wait
		m_tElevator1Loop.Set_LoopTime(30000);
		return TRUE;

	case 21:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][0] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			if (!m_pDX00->iElevator1TrayExist) gData.dElevatorZPos[0][0] = 0.0;
			m_nElevator1Case = 1; m_tElevator1Loop.Set_LoopTime(30000);
		}
		break;

	case 30:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Move_Position(nElevZAxisNo, 0);
			m_nElevator1Case++; m_tElevator1Loop.Set_LoopTime(30000);
		}
		break;
	case 31:
		if (g_objCommon.Check_Position(nElevZAxisNo, 0)) {
			g_objCommon.Save_Motion(nElevZAxisNo, 0);
			m_nElevator1Case = 20; m_tElevator1Loop.Set_LoopTime(5000);
		}
		break;

	case 51:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][0] = 0.0;
			m_nElevator1Case++; m_tElevator1Loop.Set_LoopTime(3000);
		}
		break;
	case 52:
		if (m_pDX00->iElevator1SlideClose) {
			m_nElevator1Case = 1; m_tElevator1Loop.Set_LoopTime(30000);
		}
		break;

	}

	// 1. (Error : 2100)
	if (m_tElevator1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(2100 + m_nElevator1Case);
		return FALSE;
	}
	return TRUE;
}

//  2. (Error : 2100)
BOOL CSequenceMain::Run_Elevator2() // load tray 2
{
	static int	  nElevZAxisNo = AX_ELEVATOR_Z2;
	static double dElev2PrePos, dElev2PostPos, dElelavtorZ2, dElev2Positi, dEvtSPos2, dEvtEPos2;

	dElev2PrePos  = m_pMoveData->dElevatorZ2[1];
	dElev2PostPos = m_pMoveData->dElevatorZ2[2];
//	if (gData.nElevatorOpen[2] > 0) return TRUE;
	if (m_nElevator2Case < 50 && !m_pDX00->iElevator2SlideClose) return TRUE;

	switch (m_nElevator2Case) 
	{
	case 0:	// Wait
		m_tElevator2Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (!m_pDX00->iElevator2TrayExist) 
		{
			m_nElevator2Case = 30; m_tElevator2Loop.Set_LoopTime(5000);
		} 
		else
		{
			m_tElevator2Loop.Takt_Start();
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(5000);
		}
		break;

	case 2:
			m_pDY00->oElevator2Clamp1On = FALSE; m_pDY00->oElevator2Clamp1Off = TRUE;
			m_pDY00->oElevator2Clamp2On = FALSE; m_pDY00->oElevator2Clamp2Off = TRUE;
			g_objAJinAXL.Write_Output(0);
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(5000);
		break;
	case 3:
		if (!m_pDX00->iElevator2Clamp1On && m_pDX00->iElevator2Clamp1Off && !m_pDX00->iElevator2Clamp2On && m_pDX00->iElevator2Clamp2Off)
		{
			if (m_pEquipData->bElevatorAlign) 
			{
				if (m_bElvCarDown) g_objAJinAXL.Move_Relative(nElevZAxisNo, m_pEquipData->dEleCarDown*-1.0);
				m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(5000);
			} 
			else
			{
				m_nElevator2Case = 10; m_tElevator2Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 4:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(5000);
		}
		break;
	case 5:
		if (!m_pDX00->iElevator2Clamp1On && m_pDX00->iElevator2Clamp1Off && !m_pDX00->iElevator2Clamp2On && m_pDX00->iElevator2Clamp2Off) {
			m_pDY00->oElevator2Clamp1On = TRUE; m_pDY00->oElevator2Clamp1Off = FALSE;
			m_pDY00->oElevator2Clamp2On = TRUE; m_pDY00->oElevator2Clamp2Off = FALSE;
			g_objAJinAXL.Write_Output(0);
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(5000);
		}
		break;
	case 6:
		if (m_pDX00->iElevator2Clamp1On && !m_pDX00->iElevator2Clamp1Off && m_pDX00->iElevator2Clamp2On && !m_pDX00->iElevator2Clamp2Off) {
			if (!m_tElevator2Loop.Waiting_Time(500)) break;
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(5000);
		}
		break;
	case 7:
			m_pDY00->oElevator2Clamp1On = FALSE; m_pDY00->oElevator2Clamp1Off = TRUE;
			m_pDY00->oElevator2Clamp2On = FALSE; m_pDY00->oElevator2Clamp2Off = TRUE;
			g_objAJinAXL.Write_Output(0);
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(5000);
		break;
	case 8:
		if (!m_pDX00->iElevator2Clamp1On && m_pDX00->iElevator2Clamp1Off && !m_pDX00->iElevator2Clamp2On && m_pDX00->iElevator2Clamp2Off) 
		{
			m_nElevator2Case = 10; m_tElevator2Loop.Set_LoopTime(5000);
		}
		break;

	case 10:
		if (m_pDX00->iElevator2Height1 || m_pDX00->iElevator2Height2) 
		{
			m_tElevator2Loop.Takt_Save(2, 1); m_tElevator2Loop.Takt_Start();
			m_nElevator2Case = 12; m_tElevator2Loop.Set_LoopTime(5000);
		} 
		else 
		{
			dElev2Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_UD;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_UD);
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(30000);
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) 
		{
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev2Positi);
			m_nElevator2Case = 10; m_tElevator2Loop.Set_LoopTime(5000);
		}
		break;
	case 12:
		if (m_pDX00->iElevator2Height1 || m_pDX00->iElevator2Height2)
		{
			dElev2Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_DN*-1.0;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_DN*-1.0);
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(30000);
		} 
		else
		{
			dEvtSPos2 = 0.0;
			m_tElevator2Loop.Takt_Save(2, 2); m_tElevator2Loop.Takt_Start();
			m_nElevator2Case = 14; m_tElevator2Loop.Set_LoopTime(30000);
		}
		break;
	case 13:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) 
		{
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev2Positi);
			m_nElevator2Case = 12; m_tElevator2Loop.Set_LoopTime(5000);
		}
		break;
	case 14:
		if (m_pDX00->iElevator2Height1 && m_pDX00->iElevator2Height2)
		{
			m_tElevator2Loop.Takt_Save(2, 3); m_tElevator2Loop.Takt_Start();
			m_nElevator2Case = 16; m_tElevator2Loop.Set_LoopTime(30000);
		} 
		else
		{
			dEvtEPos2 = g_objAJinAXL.Get_Position(nElevZAxisNo);
			if (dEvtSPos2 > 1.0 && m_pEquipData->dEevatOffset > 0.0 && (dEvtEPos2 - dEvtSPos2) > m_pEquipData->dEevatOffset)
			{
				m_nElevator2Case = 19; m_tElevator2Loop.Set_LoopTime(3000);
			} 
			else
			{
				dElev2Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_SL;
				g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_SL);
				m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 15:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) 
		{
			if (dEvtSPos2 < 1.0)
			{
				if (m_pDX00->iElevator2Height1 || m_pDX00->iElevator2Height2)
				{
					dEvtSPos2 = g_objAJinAXL.Get_Position(nElevZAxisNo);
				}
			}
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev2Positi);
			m_nElevator2Case = 14; m_tElevator2Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			dElev2Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElev2PostPos;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElev2PostPos);
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(30000);
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev2Positi);
			m_tElevator2Loop.Takt_Save(2, 4);
			gData.dElevatorZPos[1][1] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(3000);
		}
		break;
	case 18:
		if (Check_ElevatorSearch(2)) {
			m_nElevator2Case = 20; m_tElevator2Loop.Set_LoopTime(30000);
		}
		break;
	case 19:
		g_objCommon.Show_Error(2100 + m_nElevator2Case);
		return FALSE;

	case 20:	// Picker Wait
		m_tElevator2Loop.Set_LoopTime(30000);
		return TRUE;

	case 21:	//Carrier Down
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) 
		{
			gData.dElevatorZPos[0][1] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			if (!m_pDX00->iElevator2TrayExist) gData.dElevatorZPos[0][1] = 0.0;
			m_bElvCarDown = TRUE;
			m_nElevator2Case = 1; m_tElevator2Loop.Set_LoopTime(30000);
		}
		break;
	case 22:	//Carrier Up
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][1] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			if (!m_pDX00->iElevator2TrayExist) gData.dElevatorZPos[0][1] = 0.0;
			m_bElvCarDown = FALSE;
			m_nElevator2Case = 1; m_tElevator2Loop.Set_LoopTime(30000);
		}
		break;

	case 30:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) 
		{
			g_objCommon.Move_Position(nElevZAxisNo, 0);
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(30000);
		}
		break;
	case 31:
		if (g_objCommon.Check_Position(nElevZAxisNo, 0)) {
			g_objCommon.Save_Motion(nElevZAxisNo, 0);
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(5000);
		}
		break;
	case 32:
			m_nElevator2Case = 51; m_tElevator2Loop.Set_LoopTime(5000);
			g_objCommon.Show_Error(2190);
			return FALSE;

	case 40:
			m_pDY00->oElevator2Clamp1On = TRUE; m_pDY00->oElevator2Clamp1Off = FALSE;
			m_pDY00->oElevator2Clamp2On = TRUE; m_pDY00->oElevator2Clamp2Off = FALSE;
			g_objAJinAXL.Write_Output(0);
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(5000);
		break;
	case 41:
		if (m_pDX00->iElevator2Clamp1On && !m_pDX00->iElevator2Clamp1Off && m_pDX00->iElevator2Clamp2On && !m_pDX00->iElevator2Clamp2Off) {
			if (!m_tElevator2Loop.Waiting_Time(500)) break;
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(5000);
		}
		break;
	case 42:
			m_pDY00->oElevator2Clamp1On = FALSE; m_pDY00->oElevator2Clamp1Off = TRUE;
			m_pDY00->oElevator2Clamp2On = FALSE; m_pDY00->oElevator2Clamp2Off = TRUE;
			g_objAJinAXL.Write_Output(0);
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(5000);
		break;
	case 43:
		if (!m_pDX00->iElevator2Clamp1On && m_pDX00->iElevator2Clamp1Off && !m_pDX00->iElevator2Clamp2On && m_pDX00->iElevator2Clamp2Off) {
			m_nElevator2Case = 20; m_tElevator2Loop.Set_LoopTime(5000);
		}
		break;

	case 51:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][1] = 0.0;
			m_nElevator2Case++; m_tElevator2Loop.Set_LoopTime(3000);
		}
		break;
	case 52:
		if (m_pDX00->iElevator2SlideClose) {
			m_nElevator2Case = 1; m_tElevator2Loop.Set_LoopTime(30000);
		}
		break;

	}

	// 2. (Error : 2100)
	if (m_tElevator2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(2100 + m_nElevator2Case);
		return FALSE;
	}
	return TRUE;
}

//  3. (Error : 2300)
BOOL CSequenceMain::Run_Elevator3()// ng empty tray 
{
	static int	  nElevZAxisNo = AX_ELEVATOR_Z3;
	static double dElev3PrePos, dElev3PostPos, dElelavtorZ3, dElev3Positi;

	dElev3PrePos  = m_pMoveData->dElevatorZ3[1];
	dElev3PostPos = m_pMoveData->dElevatorZ3[2];
	if (gData.nElevatorOpen[3] > 0) return TRUE;
	if (m_nElevator3Case < 50 && !m_pDX01->iElevator3SlideClose) return TRUE;

	switch (m_nElevator3Case) {
	case 0:	// Wait
		m_tElevator3Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (!m_pDX01->iElevator3TrayExist) {
			m_nElevator3Case = 30; m_tElevator3Loop.Set_LoopTime(5000);
		} else {
			m_tElevator3Loop.Takt_Start();
			m_nElevator3Case = 10; m_tElevator3Loop.Set_LoopTime(5000);
		}
		break;

	case 10:
		if (m_pDX01->iElevator3Height1 || m_pDX01->iElevator3Height2) {
			m_tElevator3Loop.Takt_Save(3, 1); m_tElevator3Loop.Takt_Start();
			m_nElevator3Case = 12; m_tElevator3Loop.Set_LoopTime(5000);
		} else {
			dElev3Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_UD;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_UD);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev3Positi);
			m_nElevator3Case = 10; m_tElevator3Loop.Set_LoopTime(5000);
		}
		break;
	case 12:
		if (m_pDX01->iElevator3Height1 || m_pDX01->iElevator3Height2) {
			dElev3Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_DN*-1.0;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_DN*-1.0);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		} else {
			m_tElevator3Loop.Takt_Save(3, 2); m_tElevator3Loop.Takt_Start();
			m_nElevator3Case = 14; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 13:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev3Positi);
			m_nElevator3Case = 12; m_tElevator3Loop.Set_LoopTime(5000);
		}
		break;
	case 14:
		if (m_pDX01->iElevator3Height1 && m_pDX01->iElevator3Height2) {
			m_tElevator3Loop.Takt_Save(3, 3); m_tElevator3Loop.Takt_Start();
			m_nElevator3Case = 16; m_tElevator3Loop.Set_LoopTime(30000);
		} else {
			dElev3Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_SL;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_SL);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 15:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev3Positi);
			m_nElevator3Case = 14; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			dElev3Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElev3PostPos;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElev3PostPos);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev3Positi);
			m_tElevator3Loop.Takt_Save(3, 4);
			gData.dElevatorZPos[1][2] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(3000);
		}
		break;
	case 18:
		if (Check_ElevatorSearch(3)) {
			m_nElevator3Case = 20; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;

	case 20:	// Picker Wait
		m_tElevator3Loop.Set_LoopTime(30000);
		return TRUE;

	case 21:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][2] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			if (!m_pDX01->iElevator3TrayExist) gData.dElevatorZPos[0][2] = 0.0;
			m_nElevator3Case = 1; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;

	case 30:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Move_Position(nElevZAxisNo, 0);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 31:
		if (g_objCommon.Check_Position(nElevZAxisNo, 0)) {
			g_objCommon.Save_Motion(nElevZAxisNo, 0);
			m_nElevator3Case = 20; m_tElevator3Loop.Set_LoopTime(5000);
		}
		break;

	case 51:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][2] = 0.0;
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(3000);
		}
		break;
	case 52:
		if (m_pDX01->iElevator3SlideClose) {
			m_nElevator3Case = 1; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;

	}

	// 3. (Error : 2300)
	if (m_tElevator3Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(2300 + m_nElevator3Case);
		return FALSE;
	}
	return TRUE;
}

/*
BOOL CSequenceMain::Run_Elevator3()
{
	static int	  nElevZAxisNo = AX_ELEVATOR_Z3;
	static double dElev3PrePos, dElev3PostPos, dElelavtorZ3, dElev3Positi;

	dElev3PrePos  = m_pMoveData->dElevatorZ3[1];
	dElev3PostPos = m_pMoveData->dElevatorZ3[2];
	if (gData.nElevatorOpen[3] > 0) return TRUE;

	switch (m_nElevator3Case) {
	case 0:	// Wait
		m_tElevator3Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (!m_pDX01->iElevator3TrayExist) {
			m_nElevator3Case = 30; m_tElevator3Loop.Set_LoopTime(5000);
		} else {
			m_tElevator3Loop.Takt_Start();
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(5000);
		}
		break;
	case 2:
		if (m_pDX01->iElevator3Height1 || m_pDX01->iElevator3Height2) {
			dElelavtorZ3 = dElev3PrePos * -1;
			dElev3Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElelavtorZ3;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElelavtorZ3);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		} else {
			m_nElevator3Case = 10; m_tElevator3Loop.Set_LoopTime(5000);
		}
		break;
	case 3:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev3Positi);
			m_nElevator3Case = 2; m_tElevator3Loop.Set_LoopTime(5000);
		}
		break;

	case 10:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objAJinAXL.Jog_Positive(nElevZAxisNo);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 11:
		if (m_pDX01->iElevator3Height1 || m_pDX01->iElevator3Height2) {
			g_objAJinAXL.Stop_Motion(nElevZAxisNo);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 12:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			dElelavtorZ3 = dElev3PrePos * -1;
			dElev3Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElelavtorZ3;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElelavtorZ3);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 13:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev3Positi);
			if (m_pDX01->iElevator3Height1 || m_pDX01->iElevator3Height2) {
				m_nElevator3Case = 12; m_tElevator3Loop.Set_LoopTime(30000);
			} else {
				m_tElevator3Loop.Takt_Save(3, 1); m_tElevator3Loop.Takt_Start();
				g_objAJinAXL.Jog_Positive(nElevZAxisNo, ELEVATOR_SL);
				m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 14:
		if (m_pDX01->iElevator3Height1 && m_pDX01->iElevator3Height2) {
			g_objAJinAXL.Stop_Motion(nElevZAxisNo);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 15:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			m_tElevator3Loop.Takt_Save(3, 2); m_tElevator3Loop.Takt_Start();
			dElev3Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElev3PostPos;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElev3PostPos);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev3Positi);
			m_tElevator3Loop.Takt_Save(3, 3);
			m_nElevator3Case = 20; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;

	case 20:	// Picker Wait
		m_tElevator3Loop.Set_LoopTime(30000);
		return TRUE;

	case 21:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			if (!m_tElevator3Loop.Waiting_Time(500)) break;
			m_nElevator3Case = 1; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;

	case 30:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Move_Position(nElevZAxisNo, 0);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		}
		break;
	case 31:
		if (g_objCommon.Check_Position(nElevZAxisNo, 0)) {
			g_objCommon.Save_Motion(nElevZAxisNo, 0);
			m_nElevator3Case = 20; m_tElevator3Loop.Set_LoopTime(5000);
		}
		break;

	case 51:
		if (!m_pDX01->iElevator3TrayExist) {
			m_nElevator3Case = 1; m_tElevator3Loop.Set_LoopTime(5000);
		} else {
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(5000);
		}
		break;
	case 52:
		if (m_pDX01->iElevator3Height1 || m_pDX01->iElevator3Height2) {
			dElelavtorZ3 = dElev3PrePos + 1.0;
			g_objAJinAXL.Move_Absolute(nElevZAxisNo, dElelavtorZ3);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(30000);
		} else {
			m_nElevator3Case = 1; m_tElevator3Loop.Set_LoopTime(5000);
		}
		break;
	case 53:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElelavtorZ3);
			m_nElevator3Case++; m_tElevator3Loop.Set_LoopTime(3000);
		}
		break;
	case 54:
		if (!m_pDX01->iElevator3Height1 && !m_pDX01->iElevator3Height2) {
			m_nElevator3Case = 1; m_tElevator3Loop.Set_LoopTime(5000);
		}
		break;

	}

	// 3. (Error : 2300)
	if (m_tElevator3Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(2300 + m_nElevator3Case);
		return FALSE;
	}
	return TRUE;
}
*/
//  4. (Error : 2400)
BOOL CSequenceMain::Run_Elevator4()
{
	static int	  nElevZAxisNo = AX_ELEVATOR_Z4;
	static double dElev4PrePos, dElev4PostPos, dElelavtorZ4, dElev4Positi;

	dElev4PrePos  = m_pMoveData->dElevatorZ4[1];
	dElev4PostPos = m_pMoveData->dElevatorZ4[2];
	if (gData.nElevatorOpen[4] > 0) return TRUE;
	if (m_nElevator4Case < 50 && !m_pDX01->iElevator4SlideClose) return TRUE;

	switch (m_nElevator4Case) {
	case 0:	// Wait
		m_tElevator4Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (!m_pDX01->iElevator4TrayExist) {
			m_nElevator4Case = 30; m_tElevator4Loop.Set_LoopTime(5000);
		} else {
			m_tElevator4Loop.Takt_Start();
			m_nElevator4Case = 10; m_tElevator4Loop.Set_LoopTime(5000);
		}
		break;

	case 10:
		if (m_pDX01->iElevator4Height1 || m_pDX01->iElevator4Height2) {
			m_tElevator4Loop.Takt_Save(4, 1); m_tElevator4Loop.Takt_Start();
			m_nElevator4Case = 12; m_tElevator4Loop.Set_LoopTime(5000);
		} else {
			dElev4Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_UD;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_UD);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev4Positi);
			m_nElevator4Case = 10; m_tElevator4Loop.Set_LoopTime(5000);
		}
		break;
	case 12:
		if (m_pDX01->iElevator4Height1 || m_pDX01->iElevator4Height2) {
			dElev4Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_DN*-1.0;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_DN*-1.0);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		} else {
			m_tElevator4Loop.Takt_Save(4, 2); m_tElevator4Loop.Takt_Start();
			m_nElevator4Case = 14; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 13:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev4Positi);
			m_nElevator4Case = 12; m_tElevator4Loop.Set_LoopTime(5000);
		}
		break;
	case 14:
		if (m_pDX01->iElevator4Height1 && m_pDX01->iElevator4Height2) {
			m_tElevator4Loop.Takt_Save(4, 3); m_tElevator4Loop.Takt_Start();
			m_nElevator4Case = 16; m_tElevator4Loop.Set_LoopTime(30000);
		} else {
			dElev4Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_SL;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_SL);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 15:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev4Positi);
			m_nElevator4Case = 14; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			dElev4Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElev4PostPos;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElev4PostPos);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev4Positi);
			m_tElevator4Loop.Takt_Save(4, 4);
			gData.dElevatorZPos[1][3] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(3000);
		}
		break;
	case 18:
		if (Check_ElevatorSearch(4)) {
			m_nElevator4Case = 20; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;

	case 20:	// Picker Wait
		m_tElevator4Loop.Set_LoopTime(30000);
		return TRUE;

	case 21:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][3] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			if (!m_pDX01->iElevator4TrayExist) gData.dElevatorZPos[0][3] = 0.0;
			m_nElevator4Case = 1; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;

	case 30:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Move_Position(nElevZAxisNo, 0);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 31:
		if (g_objCommon.Check_Position(nElevZAxisNo, 0)) {
			g_objCommon.Save_Motion(nElevZAxisNo, 0);
			m_nElevator4Case = 20; m_tElevator4Loop.Set_LoopTime(5000);
		}
		break;

	case 51:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][3] = 0.0;
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(3000);
		}
		break;
	case 52:
		if (m_pDX01->iElevator4SlideClose) {
			m_nElevator4Case = 1; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;

	}

	// 4. (Error : 2400)
	if (m_tElevator4Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(2400 + m_nElevator4Case);
		return FALSE;
	}
	return TRUE;
}

/*
BOOL CSequenceMain::Run_Elevator4()
{
	static int	  nElevZAxisNo = AX_ELEVATOR_Z4;
	static double dElev4PrePos, dElev4PostPos, dElelavtorZ4, dElev4Positi;

	dElev4PrePos  = m_pMoveData->dElevatorZ4[1];
	dElev4PostPos = m_pMoveData->dElevatorZ4[2];
	if (gData.nElevatorOpen[4] > 0) return TRUE;

	switch (m_nElevator4Case) {
	case 0:	// Wait
		m_tElevator4Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (!m_pDX01->iElevator4TrayExist) {
			m_nElevator4Case = 30; m_tElevator4Loop.Set_LoopTime(5000);
		} else {
			m_tElevator4Loop.Takt_Start();
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(5000);
		}
		break;
	case 2:
		if (m_pDX01->iElevator4Height1 || m_pDX01->iElevator4Height2) {
			dElelavtorZ4 = dElev4PrePos * -1;
			dElev4Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElelavtorZ4;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElelavtorZ4);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		} else {
			m_nElevator4Case = 10; m_tElevator4Loop.Set_LoopTime(5000);
		}
		break;
	case 3:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev4Positi);
			m_nElevator4Case = 2; m_tElevator4Loop.Set_LoopTime(5000);
		}
		break;

	case 10:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objAJinAXL.Jog_Positive(nElevZAxisNo);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 11:
		if (m_pDX01->iElevator4Height1 || m_pDX01->iElevator4Height2) {
			g_objAJinAXL.Stop_Motion(nElevZAxisNo);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 12:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			dElelavtorZ4 = dElev4PrePos * -1;
			dElev4Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElelavtorZ4;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElelavtorZ4);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 13:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev4Positi);
			if (m_pDX01->iElevator4Height1 || m_pDX01->iElevator4Height2) {
				m_nElevator4Case = 12; m_tElevator4Loop.Set_LoopTime(30000);
			} else {
				m_tElevator4Loop.Takt_Save(4, 1); m_tElevator4Loop.Takt_Start();
				g_objAJinAXL.Jog_Positive(nElevZAxisNo, ELEVATOR_SL);
				m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 14:
		if (m_pDX01->iElevator4Height1 && m_pDX01->iElevator4Height2) {
			g_objAJinAXL.Stop_Motion(nElevZAxisNo);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 15:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			m_tElevator4Loop.Takt_Save(4, 2); m_tElevator4Loop.Takt_Start();
			dElev4Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElev4PostPos;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElev4PostPos);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev4Positi);
			m_tElevator4Loop.Takt_Save(4, 3);
			m_nElevator4Case = 20; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;

	case 20:	// Picker Wait
		m_tElevator4Loop.Set_LoopTime(30000);
		return TRUE;

	case 21:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			if (!m_tElevator4Loop.Waiting_Time(500)) break;
			m_nElevator4Case = 1; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;

	case 30:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Move_Position(nElevZAxisNo, 0);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		}
		break;
	case 31:
		if (g_objCommon.Check_Position(nElevZAxisNo, 0)) {
			g_objCommon.Save_Motion(nElevZAxisNo, 0);
			m_nElevator4Case = 20; m_tElevator4Loop.Set_LoopTime(5000);
		}
		break;

	case 51:
		if (!m_pDX01->iElevator4TrayExist) {
			m_nElevator4Case = 1; m_tElevator4Loop.Set_LoopTime(5000);
		} else {
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(5000);
		}
		break;
	case 52:
		if (m_pDX01->iElevator4Height1 || m_pDX01->iElevator4Height2) {
			dElelavtorZ4 = dElev4PrePos + 1.0;
			g_objAJinAXL.Move_Absolute(nElevZAxisNo, dElelavtorZ4);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(30000);
		} else {
			m_nElevator4Case = 1; m_tElevator4Loop.Set_LoopTime(5000);
		}
		break;
	case 53:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElelavtorZ4);
			m_nElevator4Case++; m_tElevator4Loop.Set_LoopTime(3000);
		}
		break;
	case 54:
		if (!m_pDX01->iElevator4Height1 && !m_pDX01->iElevator4Height2) {
			m_nElevator4Case = 1; m_tElevator4Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 4. (Error : 2400)
	if (m_tElevator4Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(2400 + m_nElevator4Case);
		return FALSE;
	}
	return TRUE;
}
*/
//  5. (Error : 2500)
BOOL CSequenceMain::Run_Elevator5()
{
	static int	  nElevZAxisNo = AX_ELEVATOR_Z5;
	static double dElev5PrePos, dElev5PostPos, dElelavtorZ5, dElev5Positi;

	dElev5PrePos  = m_pMoveData->dElevatorZ5[1];
	dElev5PostPos = m_pMoveData->dElevatorZ5[2];
	if (gData.nElevatorOpen[5] > 0) return TRUE;
	if (m_nElevator5Case < 50 && !m_pDX02->iElevator5SlideClose) return TRUE;

	switch (m_nElevator5Case) {
	case 0:	// Wait
		m_tElevator5Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (!m_pDX02->iElevator5TrayExist) {
			m_nElevator5Case = 30; m_tElevator5Loop.Set_LoopTime(5000);
		} else {
			m_tElevator5Loop.Takt_Start();
			m_nElevator5Case = 10; m_tElevator5Loop.Set_LoopTime(5000);
		}
		break;

	case 10:
		if (m_pDX02->iElevator5Height1 || m_pDX02->iElevator5Height2) {
			m_tElevator5Loop.Takt_Save(5, 1); m_tElevator5Loop.Takt_Start();
			m_nElevator5Case = 12; m_tElevator5Loop.Set_LoopTime(5000);
		} else {
			dElev5Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_UD;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_UD);
			m_nElevator5Case++; m_tElevator5Loop.Set_LoopTime(30000);
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev5Positi);
			m_nElevator5Case = 10; m_tElevator5Loop.Set_LoopTime(5000);
		}
		break;
	case 12:
		if (m_pDX02->iElevator5Height1 || m_pDX02->iElevator5Height2) {
			dElev5Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_DN*-1.0;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_DN*-1.0);
			m_nElevator5Case++; m_tElevator5Loop.Set_LoopTime(30000);
		} else {
			m_tElevator5Loop.Takt_Save(5, 2); m_tElevator5Loop.Takt_Start();
			m_nElevator5Case = 14; m_tElevator5Loop.Set_LoopTime(30000);
		}
		break;
	case 13:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev5Positi);
			m_nElevator5Case = 12; m_tElevator5Loop.Set_LoopTime(5000);
		}
		break;
	case 14:
		if (m_pDX02->iElevator5Height1 && m_pDX02->iElevator5Height2) {
			m_tElevator5Loop.Takt_Save(5, 3); m_tElevator5Loop.Takt_Start();
			m_nElevator5Case = 16; m_tElevator5Loop.Set_LoopTime(30000);
		} else {
			dElev5Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_SL;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_SL);
			m_nElevator5Case++; m_tElevator5Loop.Set_LoopTime(30000);
		}
		break;
	case 15:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev5Positi);
			m_nElevator5Case = 14; m_tElevator5Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			dElev5Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElev5PostPos;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElev5PostPos);
			m_nElevator5Case++; m_tElevator5Loop.Set_LoopTime(30000);
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev5Positi);
			m_tElevator5Loop.Takt_Save(5, 4);
			gData.dElevatorZPos[1][4] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			m_nElevator5Case++; m_tElevator5Loop.Set_LoopTime(3000);
		}
		break;
	case 18:
		if (Check_ElevatorSearch(5)) {
			m_nElevator5Case = 20; m_tElevator5Loop.Set_LoopTime(30000);
		}
		break;

	case 20:	// Picker Wait
		m_tElevator5Loop.Set_LoopTime(30000);
		return TRUE;

	case 21:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][4] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			if (!m_pDX02->iElevator5TrayExist) gData.dElevatorZPos[0][4] = 0.0;
			m_nElevator5Case = 1; m_tElevator5Loop.Set_LoopTime(30000);
		}
		break;

	case 30:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Move_Position(nElevZAxisNo, 0);
			m_nElevator5Case++; m_tElevator5Loop.Set_LoopTime(30000);
		}
		break;
	case 31:
		if (g_objCommon.Check_Position(nElevZAxisNo, 0)) {
			g_objCommon.Save_Motion(nElevZAxisNo, 0);
			m_nElevator5Case = 20; m_tElevator5Loop.Set_LoopTime(5000);
		}
		break;

	case 51:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][4] = 0.0;
			m_nElevator5Case++; m_tElevator5Loop.Set_LoopTime(3000);
		}
		break;
	case 52:
		if (m_pDX02->iElevator5SlideClose) {
			m_nElevator5Case = 1; m_tElevator5Loop.Set_LoopTime(30000);
		}
		break;

	}

	//  5. (Error : 2500)
	if (m_tElevator5Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(2500 + m_nElevator5Case);
		return FALSE;
	}
	return TRUE;
}

//  6. (Error : 2600)
BOOL CSequenceMain::Run_Elevator6()
{
	static int	  nElevZAxisNo = AX_ELEVATOR_Z6;
	static double dElev6PrePos, dElev6PostPos, dElelavtorZ6, dElev6Positi;

	dElev6PrePos  = m_pMoveData->dElevatorZ6[1];
	dElev6PostPos = m_pMoveData->dElevatorZ6[2];
	if (gData.nElevatorOpen[6] > 0) return TRUE;
	if (m_nElevator6Case < 50 && !m_pDX02->iElevator6SlideClose) return TRUE;

	switch (m_nElevator6Case) {
	case 0:	// Wait
		m_tElevator6Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (!m_pDX02->iElevator6TrayExist) {
			m_nElevator6Case = 30; m_tElevator6Loop.Set_LoopTime(5000);
		} else {
			m_tElevator6Loop.Takt_Start();
			m_nElevator6Case = 10; m_tElevator6Loop.Set_LoopTime(5000);
		}
		break;

	case 10:
		if (m_pDX02->iElevator6Height1) {
			m_tElevator6Loop.Takt_Save(6, 1); m_tElevator6Loop.Takt_Start();
			m_nElevator6Case = 12; m_tElevator6Loop.Set_LoopTime(5000);
		} else {
			dElev6Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_UD;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_UD);
			m_nElevator6Case++; m_tElevator6Loop.Set_LoopTime(30000);
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev6Positi);
			m_nElevator6Case = 10; m_tElevator6Loop.Set_LoopTime(5000);
		}
		break;
	case 12:
		if (m_pDX02->iElevator6Height1) {
			dElev6Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_DN*-1.0;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_DN*-1.0);
			m_nElevator6Case++; m_tElevator6Loop.Set_LoopTime(30000);
		} else {
			m_tElevator6Loop.Takt_Save(6, 2); m_tElevator6Loop.Takt_Start();
			m_nElevator6Case = 14; m_tElevator6Loop.Set_LoopTime(30000);
		}
		break;
	case 13:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev6Positi);
			m_nElevator6Case = 12; m_tElevator6Loop.Set_LoopTime(5000);
		}
		break;
	case 14:
		if (m_pDX02->iElevator6Height1) {
			m_tElevator6Loop.Takt_Save(6, 3); m_tElevator6Loop.Takt_Start();
			m_nElevator6Case = 16; m_tElevator6Loop.Set_LoopTime(30000);
		} else {
			dElev6Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_SL;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_SL);
			m_nElevator6Case++; m_tElevator6Loop.Set_LoopTime(30000);
		}
		break;
	case 15:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev6Positi);
			m_nElevator6Case = 14; m_tElevator6Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			dElev6Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElev6PostPos;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElev6PostPos);
			m_nElevator6Case++; m_tElevator6Loop.Set_LoopTime(30000);
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev6Positi);
			m_tElevator6Loop.Takt_Save(6, 4);
			gData.dElevatorZPos[1][5] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			m_nElevator6Case++; m_tElevator6Loop.Set_LoopTime(3000);
		}
		break;
	case 18:
		if (Check_ElevatorSearch(6)) {
			m_nElevator6Case = 20; m_tElevator6Loop.Set_LoopTime(30000);
		}
		break;

	case 20:	// Picker Wait
		m_tElevator6Loop.Set_LoopTime(30000);
		return TRUE;

	case 21:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][5] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			if (!m_pDX02->iElevator6TrayExist) gData.dElevatorZPos[0][5] = 0.0;
			m_nElevator6Case = 1; m_tElevator6Loop.Set_LoopTime(30000);
		}
		break;

	case 30:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Move_Position(nElevZAxisNo, 0);
			m_nElevator6Case++; m_tElevator6Loop.Set_LoopTime(30000);
		}
		break;
	case 31:
		if (g_objCommon.Check_Position(nElevZAxisNo, 0)) {
			g_objCommon.Save_Motion(nElevZAxisNo, 0);
			m_nElevator6Case = 20; m_tElevator6Loop.Set_LoopTime(5000);
		}
		break;

	case 51:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][5] = 0.0;
			m_nElevator6Case++; m_tElevator6Loop.Set_LoopTime(3000);
		}
		break;
	case 52:
		if (m_pDX02->iElevator6SlideClose) {
			m_nElevator6Case = 1; m_tElevator6Loop.Set_LoopTime(30000);
		}
		break;

	}

	//  6. (Error : 2600)
	if (m_tElevator6Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(2600 + m_nElevator6Case);
		return FALSE;
	}
	return TRUE;
}

//  7. (Error : 2700)
BOOL CSequenceMain::Run_Elevator7()
{
	static int	  nElevZAxisNo = AX_ELEVATOR_Z7;
	static double dElev7PrePos, dElev7PostPos, dElelavtorZ7, dElev7Positi;

	dElev7PrePos  = m_pMoveData->dElevatorZ7[1];
	dElev7PostPos = m_pMoveData->dElevatorZ7[2];
	if (gData.nElevatorOpen[7] > 0) return TRUE;
	if (m_nElevator7Case < 50 && !m_pDX02->iElevator7SlideClose) return TRUE;

	switch (m_nElevator7Case) {
	case 0:	// Wait
		m_tElevator7Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (!m_pDX02->iElevator7TrayExist) {
			m_nElevator7Case = 30; m_tElevator7Loop.Set_LoopTime(5000);
		} else {
			m_tElevator7Loop.Takt_Start();
			m_nElevator7Case = 10; m_tElevator7Loop.Set_LoopTime(5000);
		}
		break;

	case 10:
		if (m_pDX02->iElevator7Height1) {
			m_tElevator7Loop.Takt_Save(7, 1); m_tElevator7Loop.Takt_Start();
			m_nElevator7Case = 12; m_tElevator7Loop.Set_LoopTime(5000);
		} else {
			dElev7Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_UD;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_UD);
			m_nElevator7Case++; m_tElevator7Loop.Set_LoopTime(30000);
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev7Positi);
			m_nElevator7Case = 10; m_tElevator7Loop.Set_LoopTime(5000);
		}
		break;
	case 12:
		if (m_pDX02->iElevator7Height1) {
			dElev7Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_DN*-1.0;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_DN*-1.0);
			m_nElevator7Case++; m_tElevator7Loop.Set_LoopTime(30000);
		} else {
			m_tElevator7Loop.Takt_Save(7, 2); m_tElevator7Loop.Takt_Start();
			m_nElevator7Case = 14; m_tElevator7Loop.Set_LoopTime(30000);
		}
		break;
	case 13:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev7Positi);
			m_nElevator7Case = 12; m_tElevator7Loop.Set_LoopTime(5000);
		}
		break;
	case 14:
		if (m_pDX02->iElevator7Height1) {
			m_tElevator7Loop.Takt_Save(7, 3); m_tElevator7Loop.Takt_Start();
			m_nElevator7Case = 16; m_tElevator7Loop.Set_LoopTime(30000);
		} else {
			dElev7Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + ELEVATOR_SL;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, ELEVATOR_SL);
			m_nElevator7Case++; m_tElevator7Loop.Set_LoopTime(30000);
		}
		break;
	case 15:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev7Positi);
			m_nElevator7Case = 14; m_tElevator7Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			dElev7Positi = g_objAJinAXL.Get_Position(nElevZAxisNo) + dElev7PostPos;
			g_objAJinAXL.Move_Relative(nElevZAxisNo, dElev7PostPos);
			m_nElevator7Case++; m_tElevator7Loop.Set_LoopTime(30000);
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Save_Motion(nElevZAxisNo, -1, dElev7Positi);
			m_tElevator7Loop.Takt_Save(7, 4);
			gData.dElevatorZPos[1][6] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			m_nElevator7Case++; m_tElevator7Loop.Set_LoopTime(3000);
		}
		break;
	case 18:
		if (Check_ElevatorSearch(7)) {
			m_nElevator7Case = 20; m_tElevator7Loop.Set_LoopTime(30000);
		}
		break;

	case 20:	// Picker Wait
		m_tElevator7Loop.Set_LoopTime(30000);
		return TRUE;

	case 21:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][6] = g_objAJinAXL.Get_Position(nElevZAxisNo);
			if (!m_pDX02->iElevator7TrayExist) gData.dElevatorZPos[0][6] = 0.0;
			m_nElevator7Case = 1; m_tElevator7Loop.Set_LoopTime(30000);
		}
		break;

	case 30:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			g_objCommon.Move_Position(nElevZAxisNo, 0);
			m_nElevator7Case++; m_tElevator7Loop.Set_LoopTime(30000);
		}
		break;
	case 31:
		if (g_objCommon.Check_Position(nElevZAxisNo, 0)) {
			g_objCommon.Save_Motion(nElevZAxisNo, 0);
			m_nElevator7Case = 20; m_tElevator7Loop.Set_LoopTime(5000);
		}
		break;

	case 51:
		if (g_objAJinAXL.Is_Done(nElevZAxisNo)) {
			gData.dElevatorZPos[0][6] = 0.0;
			m_nElevator7Case++; m_tElevator7Loop.Set_LoopTime(3000);
		}
		break;
	case 52:
		if (m_pDX02->iElevator7SlideClose) {
			m_nElevator7Case = 1; m_tElevator7Loop.Set_LoopTime(30000);
		}
		break;

	}

	//  7. (Error : 2700)
	if (m_tElevator7Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(2700 + m_nElevator7Case);
		return FALSE;
	}
	return TRUE;
}

//  8. (Error : 3000)
BOOL CSequenceMain::Run_Transfer1()
{
//	gData.nTransferX1Pos;	//1(LS1),2(LS2),3(Buf),4(LE),5(NE),6(GE),7(NG1),8(NG2),9(Good1),10(Good2)
	static int nFromTran1Pos = 0;
	static int nToTran1Pos = 0;
	static int nPort1No = 0;

	switch (m_nTransfer1Case) {
	case 0:	//작업판단
		if (Check_Transfer1(nFromTran1Pos, nToTran1Pos, nPort1No)) 
		{
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(60000);
			m_sLog.Format("MCC,8,Transfer1,%d,  Check Transfer from:%d  To:%d  PortNo:%d", m_nTransfer1Case, nFromTran1Pos, nToTran1Pos, nPort1No);
			g_objLogFile.Save_SeqLog(m_sLog);
		} else {
//			if (Check_Load12Empy()) {
			if (!m_pDX18->iLDMZElevatorExist) 
			{
				if (m_nLoadStage1Case == 0 && m_nLoadStage2Case == 50 && m_pDX04->iLoadStage2TrayExist) 
				{ 
					m_nLoadStage1Case = 22;  gNG->nTrayOX[0][0] = 0;
					m_sLog.Format("MCC,8,Transfer1,%d,  Load Stage 1 Start ", m_nTransfer1Case);
					g_objLogFile.Save_SeqLog(m_sLog);
				}
				if (m_nLoadStage2Case == 0 && m_nLoadStage1Case == 50 && m_pDX04->iLoadStage1TrayExist)
				{ 
					m_nLoadStage2Case = 22;  gNG->nTrayOX[0][1] = 0; 
					m_sLog.Format("MCC,8,Transfer1,%d,  Load Stage 2 start ", m_nTransfer1Case);
					g_objLogFile.Save_SeqLog(m_sLog);
				}
			}
		}
		return TRUE;

	case 1:	//Up
		if ((nFromTran1Pos == 1) ||	//LS1
			(nFromTran1Pos == 2) ||	//LS2
			(nFromTran1Pos == 4) ) 
		{	//Load-MZ
			m_tTransfer1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANSFER_Z1, 0);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 move to Ready Up  From:%d", m_nTransfer1Case, nFromTran1Pos);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 2:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, 0)) 
		{
			if ((nFromTran1Pos != 4) || (nFromTran1Pos == 4 && gData.nTransferXLock == 0))
			{
				if (nFromTran1Pos == 4) gData.nTransferXLock = 1;
				gData.nTransferX1Pos = nFromTran1Pos;
				g_objCommon.Move_Position(AX_TRANSFER_X1, gData.nTransferX1Pos);
				m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(30000);


				m_sLog.Format("MCC,8,Transfer1,%d,  Info Update From:%d  XLock:%d ", m_nTransfer1Case, gData.nTransferX1Pos, gData.nTransferXLock);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		return TRUE;

	case 3:
		if (g_objCommon.Check_Position(AX_TRANSFER_X1, gData.nTransferX1Pos)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_X1, gData.nTransferX1Pos);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d,  AX_TRANSFER_X1 move to %d ", m_nTransfer1Case, gData.nTransferX1Pos);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 4:
		if (!m_pDX03->iTransferLTrayExist) 
		{
			m_pDY03->oTransferLGrabOpen = TRUE; m_pDY03->oTransferLGrabClose = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d,  TransferLeftGrab Open ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 5:
		if (m_pDX03->iTransferLGrab1Open && !m_pDX03->iTransferLGrab1Close) 
		{
			if (m_pDX03->iTransferLGrab2Open && !m_pDX03->iTransferLGrab2Close) 
			{
				m_nTransfer1Case = 8; m_tTransfer1Loop.Set_LoopTime(5000);
				m_tTransfer1Loop.Takt_Save(8, 1); m_tTransfer1Loop.Takt_Start();

				m_sLog.Format("MCC,8,Transfer1,%d,  TransferLeftGrab Open Check ", m_nTransfer1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;

	case 8:	//Tray Up
		if (nFromTran1Pos == 1 || nFromTran1Pos == 2) m_nTransfer1Case = 10;	//Up Stage 12(1,2)
		if (nFromTran1Pos == 4)					  m_nTransfer1Case = 30;	//Up Load-MZ(4)
		m_tTransfer1Loop.Set_LoopTime(60000);
		break;

	case 10:	//Up Stage 12(1,2)
		if ((nFromTran1Pos == 1 && m_nLoadStage1Case == 60) ||
			(nFromTran1Pos == 2 && m_nLoadStage2Case == 60) ) 
		{
			m_tTransfer1Loop.Takt_Save(8, 2); m_tTransfer1Loop.Takt_Start();
			if (nFromTran1Pos == 1) g_objCommon.Move_Position(AX_TRANSFER_Z1, 6);
			if (nFromTran1Pos == 2) g_objCommon.Move_Position(AX_TRANSFER_Z1, 7);
			m_nTransfer1Case++;	m_tTransfer1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 move to %d ", m_nTransfer1Case, nFromTran1Pos);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;

	case 11:
		if ((nFromTran1Pos == 1 && g_objCommon.Check_Position(AX_TRANSFER_Z1, 6)) ||
			(nFromTran1Pos == 2 && g_objCommon.Check_Position(AX_TRANSFER_Z1, 7)) ) 
		{
			if (nFromTran1Pos == 1) g_objCommon.Save_Motion(AX_TRANSFER_Z1, 6);
			if (nFromTran1Pos == 2) g_objCommon.Save_Motion(AX_TRANSFER_Z1, 7);
			m_tTransfer1Loop.Takt_Save(8, 3); m_tTransfer1Loop.Takt_Start();
			m_pDY03->oTransferLGrabOpen = FALSE; m_pDY03->oTransferLGrabClose = TRUE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, oTransferLeftGrab Close ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 12:
		if (!m_pDX03->iTransferLGrab1Open && m_pDX03->iTransferLGrab1Close) 
		{
			if (!m_pDX03->iTransferLGrab2Open && m_pDX03->iTransferLGrab2Close) 
			{
				if (!m_tTransfer1Loop.Waiting_Time(300)) break;
				m_tTransfer1Loop.Takt_Save(8, 4); m_tTransfer1Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z1, 0);
				m_nTransfer1Case++;	m_tTransfer1Loop.Set_LoopTime(30000);

				m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 move to Ready Up", m_nTransfer1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 13:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, 0)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_Z1, 0);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 save motion ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 14:
		if (m_pDX03->iTransferLTrayExist) 
		{
			gData.sLotID_Tansfer[0]  = gData.sLotID_LoadStage[nFromTran1Pos-1]; 
			gData.nTrayNo_Tansfer[0] = gData.nTrayNo_LoadStage[nFromTran1Pos-1];
			gData.nPortNo_Tansfer[0] = gData.nPortNo_LoadStage[nFromTran1Pos-1];
			gData.sLotID_Tansfer[2]  = gData.sLotID_Tansfer[0];

			gData.sLotID_LoadStage[nFromTran1Pos-1]  = "";
			gData.nTrayNo_LoadStage[nFromTran1Pos-1] = gData.nPortNo_LoadStage[nFromTran1Pos-1] = 0;

			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("[Transfer1: Up Load-Stage%d] Lot(%s) TNo(%d) PNo(%d)", (nFromTran1Pos), gData.sLotID_Tansfer[0], gData.nTrayNo_Tansfer[0], gData.nPortNo_Tansfer[0]);
			g_objLogFile.Save_HandlerLog(m_sLog);g_objLogFile.Save_SeqLog(m_sLog);

			m_sLog.Format("MCC,8,Transfer1,%d, Info Update ", m_nTransfer1Case);
		}
		break;
	case 15:
		if (m_pDX03->iTransferLTrayExist) 
		{
			m_tTransfer1Loop.Takt_Save(8, 5); m_tTransfer1Loop.Takt_Start();
			if (nFromTran1Pos == 1 && m_nLoadStage1Case == 60) m_nLoadStage1Case = 61;
			if (nFromTran1Pos == 2 && m_nLoadStage2Case == 60) m_nLoadStage2Case = 61;

			m_nTransfer1Case = 40; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, make load stage back to work & From:%d ", m_nTransfer1Case, nFromTran1Pos);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 30:	//Up Load-MZ(4)
		if (m_pDX03->iTransferLGrab1Open && !m_pDX03->iTransferLGrab1Close) 
		{
			if (m_pDX03->iTransferLGrab2Open && !m_pDX03->iTransferLGrab2Close) 
			{
				m_tTransfer1Loop.Takt_Save(8, 12); m_tTransfer1Loop.Takt_Start();
				m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

				m_sLog.Format("MCC,8,Transfer1,%d, iTransferLGrabOpen Check", m_nTransfer1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 31:
		if (m_pDX15->iLDRailCarrierChk2) 
		{
			if (g_objCommon.Check_Position(AX_LOAD_FEEDER_Y, 0)) 
			{
				m_tTransfer1Loop.Takt_Save(8, 13); m_tTransfer1Loop.Takt_Start();
				m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(60000);

				m_sLog.Format("MCC,8,Transfer1,%d, AX_LOAD_FEEDER_Y pos check ", m_nTransfer1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 32:
		if (m_nLDMZElevatorCase == 30) 
		{
			m_tTransfer1Loop.Takt_Save(8, 14); m_tTransfer1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANSFER_Z1, 4);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(60000);

			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 move to (Load Carrier Up) ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;
	case 33:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, 4)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_Z1, 4);
			m_tTransfer1Loop.Takt_Save(8, 15); m_tTransfer1Loop.Takt_Start();
			m_pDY03->oTransferLGrabOpen = FALSE; m_pDY03->oTransferLGrabClose = TRUE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, oTransferLGrabClose ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 34:
		if (!m_pDX03->iTransferLGrab1Open && m_pDX03->iTransferLGrab1Close) 
		{
			if (!m_pDX03->iTransferLGrab2Open && m_pDX03->iTransferLGrab2Close) 
			{
				if (g_objCommon.Check_Position(AX_LOAD_FEEDER_Y, 0)) 
				{
					if (!m_tTransfer1Loop.Waiting_Time(300)) break;
					m_tTransfer1Loop.Takt_Save(8, 16); m_tTransfer1Loop.Takt_Start();
					g_objCommon.Move_Position(AX_TRANSFER_Z1, 0);
					m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

					m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 Ready Up ", m_nTransfer1Case);
					g_objLogFile.Save_SeqLog(m_sLog);
				}
			}
		}
		break;
	case 35:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, 0)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_Z1, 0);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, Save Motion ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 36:
		if (m_pDX03->iTransferLTrayExist) 
		{
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, Save Motion ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 37:
			gData.sMZID_Tansfer  = gData.sMZID[4];
			gData.nSlotNo_Tansfer = gData.nSlotNo_LDMZ; gData.nSlotNo_LDMZ = 0;
			gData.sLotID_Tansfer[0]  = "";
			gData.nTrayNo_Tansfer[0] = gData.nPortNo_Tansfer[0] = 0;
			gData.nLDMZ_LastCarrier[1] = gData.nLDMZ_LastCarrier[0];

			gTracking.sMZID_Trasnfer1 = gTracking.sMZID_LoadMZElev; gTracking.sMZID_LoadMZElev = "";
			m_sLog.Format("Load Mz Elev To Transfer1, : %s", gTracking.sMZID_Trasnfer1);
			g_objLogFile.Save_HomeTrackingLog(m_sLog);

			if (m_nLDMZElevatorCase == 30) 
			{
				m_nLDMZElevatorCase = 31; m_tLDMZElevatorLoop.Set_LoopTime(5000);
			}
			m_nTransfer1Case = 40; m_tTransfer1Loop.Set_LoopTime(5000);
			m_tTransfer1Loop.Takt_Save(8, 17); m_tTransfer1Loop.Takt_Start();

			m_sLog.Format("[Transfer1: Up Load-Lot] Lot(%s) TNo(%d) PNo(%d)", gData.sLotID_Tansfer[0], gData.nTrayNo_Tansfer[0], gData.nPortNo_Tansfer[0]);
			g_objLogFile.Save_HandlerLog(m_sLog);g_objLogFile.Save_SeqLog(m_sLog);

			m_sLog.Format("MCC,8,Transfer1,%d, Let Load Mz Elev continue to work ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		break;

//	gData.nTransferX1Pos;	//1(LS1),2(LS2),3(Buf),4(LE),5(NE),6(GE),7(NG1),8(NG2),9(Good1),10(Good2)
	case 40:	//Tray Down
		if (nToTran1Pos == 1 || nToTran1Pos == 2) m_nTransfer1Case = 41;	//Down Stage 12(1,2)
		if (nToTran1Pos == 3)					  m_nTransfer1Case = 51;	//Down Buffer(3)
		m_tTransfer1Loop.Set_LoopTime(5000);

		m_sLog.Format("MCC,8,Transfer1,%d, To:%d ", m_nTransfer1Case, nToTran1Pos);
		g_objLogFile.Save_SeqLog(m_sLog);
		break;

	case 41:	//Down Stage 12 (1,2)
		nToTran1Pos = 0;
		if (m_nLoadStage1Case == 0) nToTran1Pos = 1;
		if (m_nLoadStage2Case == 0) nToTran1Pos = 2;
		if (nToTran1Pos > 0) 
		{
			m_nTransfer1Case++;	m_tTransfer1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,8,Transfer1,%d, To : %d ", m_nTransfer1Case, nToTran1Pos);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;
	case 42:	
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, 0)) 
		{
			m_tTransfer1Loop.Takt_Save(8, 18); m_tTransfer1Loop.Takt_Start();
			gData.nTransferX1Pos = nToTran1Pos;
			g_objCommon.Move_Position(AX_TRANSFER_X1, gData.nTransferX1Pos);
			if (nFromTran1Pos == 4) gData.nTransferXLock = 0;
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_X1  move to %d and TransferXLock:%d", m_nTransfer1Case, gData.nTransferX1Pos, gData.nTransferXLock);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 43:
		if (g_objCommon.Check_Position(AX_TRANSFER_X1, nToTran1Pos)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_X1, gData.nTransferX1Pos);
			m_tTransfer1Loop.Takt_Save(8, 19); m_tTransfer1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANSFER_Z1, nToTran1Pos);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 move to %d ", m_nTransfer1Case, nToTran1Pos);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 44:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, gData.nTransferX1Pos)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_Z1, gData.nTransferX1Pos);
			m_tTransfer1Loop.Takt_Save(8, 20); m_tTransfer1Loop.Takt_Start();
			m_pDY03->oTransferLGrabOpen = TRUE; m_pDY03->oTransferLGrabClose = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, oTransferLGrabOpen ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 45:
		if (m_pDX03->iTransferLGrab1Open && !m_pDX03->iTransferLGrab1Close) 
		{
			if (m_pDX03->iTransferLGrab2Open && !m_pDX03->iTransferLGrab2Close) 
			{
				if (!m_tTransfer1Loop.Waiting_Time(300)) break;
				m_tTransfer1Loop.Takt_Save(8, 21); m_tTransfer1Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z1, 0);
				m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

				m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 Ready Up ", m_nTransfer1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 46:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, 0)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_Z1, 0);
			m_sLog.Format("[Transfer1: Down Load-Stage%d] Lot(%s) TNo(%d) PNo(%d)", nToTran1Pos, gData.sLotID_Tansfer[0], gData.nTrayNo_Tansfer[0], gData.nPortNo_Tansfer[0]);
			g_objLogFile.Save_HandlerLog(m_sLog);g_objLogFile.Save_SeqLog(m_sLog);

			m_sLog.Format("MCC,8,Transfer1,%d, empty ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);


			gTracking.sMZID_LoadStage[nToTran1Pos-1] = gTracking.sMZID_Trasnfer1; gTracking.sMZID_Trasnfer1 = "";

			m_sLog.Format("Transfer1 To LoadStage No %d, MZ ID:%s",nToTran1Pos, gTracking.sMZID_LoadStage[nToTran1Pos-1]);
			g_objLogFile.Save_HomeTrackingLog(m_sLog);

			gData.sMZID_LoadStage[nToTran1Pos-1] = gData.sMZID_Tansfer; gData.sMZID_Tansfer = "";
			gData.nSlotNo_LoadStage[nToTran1Pos-1] = gData.nSlotNo_Tansfer; gData.nSlotNo_Tansfer = 0;
			gData.nLastCar_LoadStage[nToTran1Pos-1] = gData.nLDMZ_LastCarrier[1];

			gData.sLotID_Tansfer[0]  = "";
			gData.nTrayNo_Tansfer[0] = gData.nPortNo_Tansfer[0] = 0;
			m_tTransfer1Loop.Takt_Save(8, 22); m_tTransfer1Loop.Takt_Start();

			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);
		}
		break;
	case 47:
		if (!m_pDX03->iTransferLTrayExist)
		{
			if ((nToTran1Pos == 1 && m_pDX04->iLoadStage1TrayExist) ||
				(nToTran1Pos == 2 && m_pDX04->iLoadStage2TrayExist) ) 
			{
				m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

				m_sLog.Format("MCC,8,Transfer1,%d, To:%d ", m_nTransfer1Case, nToTran1Pos);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 48:
			if (m_nLoadStage1Case == 0) 
			{
				m_pDY04->oLoadStage1MasterIn = TRUE; m_pDY04->oLoadStage1MasterOut = FALSE;
				m_nLoadStage1Case = 1;
			}
			if (m_nLoadStage2Case == 0) 
			{
				m_pDY04->oLoadStage2MasterIn = TRUE; m_pDY04->oLoadStage2MasterOut = FALSE;
				m_nLoadStage2Case = 1;
			}
			g_objAJinAXL.Write_Output(4);
			m_nTransfer1Case = 70; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, Let LoadStage work continuously", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		break;

	case 51:	//Down Buffer(3)
			m_tTransfer1Loop.Takt_Save(8, 23); m_tTransfer1Loop.Takt_Start();
			gData.nTransferX1Pos = nToTran1Pos;
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,8,Transfer1,%d, To:%d ", m_nTransfer1Case, gData.nTransferX1Pos);
			g_objLogFile.Save_SeqLog(m_sLog);
		break;
	case 52:
		if (gData.nTransferX2Pos > 3) 
		{
			if (gData.nTransferXLock == 0) 
			{
				m_tTransfer1Loop.Takt_Save(8, 25); m_tTransfer1Loop.Takt_Start();
				gData.nTransferXLock = 1;
				gData.nTransferX1Pos = nToTran1Pos;
				g_objCommon.Move_Position(AX_TRANSFER_X1, gData.nTransferX1Pos);
				m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(30000);

				m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_X1 move to X1Pos:%d and X2Pos:%d", m_nTransfer1Case, gData.nTransferX1Pos, gData.nTransferX2Pos);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		return TRUE;
	case 53:
		if (g_objCommon.Check_Position(AX_TRANSFER_X1, gData.nTransferX1Pos)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_X1, gData.nTransferX1Pos);
			m_tTransfer1Loop.Takt_Save(8, 26); m_tTransfer1Loop.Takt_Start();
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, empty ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 54:
		if (m_nElevator2Case == 0) m_nElevator2Case = 1;
		if (m_nElevator2Case == 20) 
		{
			m_tTransfer1Loop.Takt_Save(8, 27); m_tTransfer1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANSFER_Z1, nToTran1Pos);
			m_nTransfer1Case++;	m_tTransfer1Loop.Set_LoopTime(30000);


			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 move to %d ", m_nTransfer1Case, nToTran1Pos);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;
	case 55:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, nToTran1Pos)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_Z1, nToTran1Pos);
			m_tTransfer1Loop.Takt_Save(8, 28); m_tTransfer1Loop.Takt_Start();
			m_pDY03->oTransferLGrabOpen = TRUE; m_pDY03->oTransferLGrabClose = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, oTransferLGrabOpen ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 56:
		if (m_pDX03->iTransferLGrab1Open && !m_pDX03->iTransferLGrab1Close) 
		{
			if (m_pDX03->iTransferLGrab2Open && !m_pDX03->iTransferLGrab2Close) 
			{
				if (!m_tTransfer1Loop.Waiting_Time(300)) break;
				m_tTransfer1Loop.Takt_Save(8, 29); m_tTransfer1Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z1, 0);
				m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);


				m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 Ready Up ", m_nTransfer1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 57:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, 0)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_Z1, 0);
			m_sLog.Format("[Transfer1: Down Buffer] Lot(%s) TNo(%d) PNo(%d)", gData.sLotID_Tansfer[0], gData.nTrayNo_Tansfer[0], gData.nPortNo_Tansfer[0]);
			g_objLogFile.Save_HandlerLog(m_sLog);g_objLogFile.Save_SeqLog(m_sLog);

			gData.sLotID_Tansfer[0]  = "";
			gData.nTrayNo_Tansfer[0] = gData.nPortNo_Tansfer[0] = 0;
			gData.nCarrierCount++;

			m_tTransfer1Loop.Takt_Save(8, 30); m_tTransfer1Loop.Takt_Start();
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, Info Update ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 58:
		if (!m_pDX03->iTransferLTrayExist)
		{
			if (m_nElevator2Case == 20) m_nElevator2Case = 21;
			m_nTransfer1Case = 70; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, Let Elev continue to work ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 61:	//Down Empty-Good (6)
		if (gData.nTransferX2Pos > 6) 
		{
			m_tTransfer1Loop.Takt_Save(8, 31); m_tTransfer1Loop.Takt_Start();
			gData.nTransferX1Pos = nToTran1Pos;
			g_objCommon.Move_Position(AX_TRANSFER_X1, gData.nTransferX1Pos);
			m_nTransfer1Case = 63; m_tTransfer1Loop.Set_LoopTime(30000);


			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_X1 move to %d ", m_nTransfer1Case, gData.nTransferX1Pos);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		else 
		{
			m_tTransfer1Loop.Takt_Save(8, 32); m_tTransfer1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANSFER_X1, 4);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_X1 move to 4(Empty NG) ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 62:
		if (g_objCommon.Check_Position(AX_TRANSFER_X1, 4)) 
		{
			double dT2Pos = g_objAJinAXL.Get_Position(AX_TRANSFER_X2);
			if (gData.nTransferX2Pos > 6 && dT2Pos < (m_pMoveData->dTransferX2[2]-10)) 
			{
				g_objCommon.Save_Motion(AX_TRANSFER_X1, 4);
				m_tTransfer1Loop.Takt_Save(8, 33); m_tTransfer1Loop.Takt_Start();
				gData.nTransferX1Pos = nToTran1Pos;
				g_objCommon.Move_Position(AX_TRANSFER_X1, gData.nTransferX1Pos);
				m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(30000);

				m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_X1 move to %d ", m_nTransfer1Case, gData.nTransferX1Pos);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		return TRUE;
	case 63:
		if (g_objCommon.Check_Position(AX_TRANSFER_X1, gData.nTransferX1Pos))
		{
			g_objCommon.Save_Motion(AX_TRANSFER_X1, gData.nTransferX1Pos);
			m_tTransfer1Loop.Takt_Save(8, 34); m_tTransfer1Loop.Takt_Start();
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_X1 move done ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 64:
		if (m_nElevator4Case == 20 && gData.nElevatorOpen[4] == 0) 
		{
			m_tTransfer1Loop.Takt_Save(8, 35); m_tTransfer1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANSFER_Z1, nToTran1Pos);
			m_nTransfer1Case++;	m_tTransfer1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 move to %d ", m_nTransfer1Case, nToTran1Pos);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;
	case 65:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, nToTran1Pos)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_Z1, nToTran1Pos);
			m_tTransfer1Loop.Takt_Save(8, 36); m_tTransfer1Loop.Takt_Start();
			m_pDY03->oTransferLGrabOpen = TRUE; m_pDY03->oTransferLGrabClose = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, oTransferLGrabOpen ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 66:
		if (m_pDX03->iTransferLGrab1Open && !m_pDX03->iTransferLGrab1Close) 
		{
			if (m_pDX03->iTransferLGrab2Open && !m_pDX03->iTransferLGrab2Close) 
			{
				if (!m_tTransfer1Loop.Waiting_Time(300)) break;
				m_tTransfer1Loop.Takt_Save(8, 37); m_tTransfer1Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z1, 0);
				m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

				m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_Z1 Ready Up ", m_nTransfer1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 67:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, 0)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_Z1, 0);
			m_sLog.Format("[Transfer1: Down Empty-Good] Lot(%s) TNo(%d) PNo(%d)", gData.sLotID_Tansfer[0], gData.nTrayNo_Tansfer[0], gData.nPortNo_Tansfer[0]);
			g_objLogFile.Save_HandlerLog(m_sLog);g_objLogFile.Save_SeqLog(m_sLog);

			gData.sLotID_Tansfer[0]  = "";
			gData.nTrayNo_Tansfer[0] = gData.nPortNo_Tansfer[0] = 0;

			m_tTransfer1Loop.Takt_Save(8, 38); m_tTransfer1Loop.Takt_Start();
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, Info Update ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 68:
		if (!m_pDX03->iTransferLTrayExist)
		{
			if (m_nElevator4Case == 20) m_nElevator4Case = 21;
			m_nTransfer1Case = 70; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, Let Good Elev continue to work ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 70:	//대기위치로
		if (g_objCommon.Check_Position(AX_TRANSFER_Z1, 0)) 
		{
			gData.nTransferX1Pos = 1;
			g_objCommon.Move_Position(AX_TRANSFER_X1, gData.nTransferX1Pos);
			if (nToTran1Pos == 3) gData.nTransferXLock = 0;
			m_nTransfer1Case++; m_tTransfer1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_X1 move to %d ", m_nTransfer1Case, gData.nTransferX1Pos);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 71:
		if (g_objCommon.Check_Position(AX_TRANSFER_X1, gData.nTransferX1Pos)) 
		{
			g_objCommon.Save_Motion(AX_TRANSFER_X1, gData.nTransferX1Pos);
			m_tTransfer1Loop.Takt_Save(8, 39);
			m_nTransfer1Case = 0; m_tTransfer1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,8,Transfer1,%d, AX_TRANSFER_X1 move done ", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	}

	//  8. (Error : 3000)
	if (m_tTransfer1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(3000 + m_nTransfer1Case);
		return FALSE;
	}
	return TRUE;
}

//  9. (Error : 3500)
BOOL CSequenceMain::Run_Transfer2()
{
//	gData.nTransferX2Pos;	//1(LS1),2(LS2),3(Buf),4(LE),5(NE),6(GE),7(NG1),8(NG2),9(Good1),10(Good2)
	static int nFmTran2Pos = 0;
	static int nToTran2Pos = 0;
	static int nPort2No = 0, nPosX = 0, nPosZ = 0;

	switch (m_nTransfer2Case) 
	{
	case 0:	//작업판단
		if (Check_EmptyUnloadPicker()==FALSE) 
		{
			if (m_nNGStage1Case == 0 && m_pDX11->iNGStage1TrayExist) m_nNGStage1Case = 1;
			if (m_nNGStage2Case == 0 && m_pDX11->iNGStage2TrayExist) m_nNGStage2Case = 1;
			if (m_nGoodStage1Case == 0 && m_pDX12->iGoodStage1TrayExist) m_nGoodStage1Case = 1;
			if (m_nGoodStage2Case == 0 && m_pDX12->iGoodStage2TrayExist) m_nGoodStage2Case = 1;
		}
		if (Check_Transfer2(nFmTran2Pos, nToTran2Pos, nPort2No)) 
		{
			m_sLog.Format("SEQ,9,Transfer2,%d, comments", m_nTransfer2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(60000);
		}
		return TRUE;

	case 1:
		m_sLog.Format("SEQ,9,Transfer2,%d, comments", m_nTransfer2Case);
		g_objLogFile.Save_SeqLog(m_sLog);

		g_objCommon.Move_Position(AX_TRANSFER_Z2, 0);
		m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		break;
	case 2:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0))
		{
			m_sLog.Format("SEQ,9,Transfer2,%d, comments", m_nTransfer2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY03->oTransferRGrabOpen = TRUE; m_pDY03->oTransferRGrabClose = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 3:
		if (m_pDX03->iTransferRGrab1Open && !m_pDX03->iTransferRGrab1Close) 
		{
			if (m_pDX03->iTransferRGrab2Open && !m_pDX03->iTransferRGrab2Close)
			{
				m_sLog.Format("SEQ,9,Transfer2,%d, comments", m_nTransfer2Case);
				g_objLogFile.Save_SeqLog(m_sLog);

				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 4:
		if (nFmTran2Pos == 3) 
		{
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		} 
		else 
		{
			gData.nTransferX2Pos = nFmTran2Pos;
			m_nTransfer2Case = 6; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 5:
		if (nFmTran2Pos == 3 && gData.nTransferXLock == 0) {
			gData.nTransferXLock = 1;
			gData.nTransferX2Pos = nFmTran2Pos;
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		return TRUE;

//	gData.nTransferX2Pos;	//1(LS1),2(LS2),3(Buf),4(LE),5(NE),6(GE),7(NG1),8(NG2),9(Good1),10(Good2)
	//1순위 Good-Stage12(9,10)-> Good Elevator(6)
	//2순위 NG-Stage12  (7,8) -> NG Elevator(5)
	//3순위 Good Elevator(6)  -> Good,NG Stage(7~10)
	//4순위 NG Elevator  (5)  -> Good,NG Stage(7~10)
	//5순위 Buffer       (3)  -> Good,NG Stage(7~10)
	//6순위 Good Elevator(6)  -> Buffer(3)
	//7순위 NG Elevator  (5)  -> Buffer(3)

	case 6:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			m_tTransfer2Loop.Takt_Start();
			if (gData.nTransferX2Pos ==  3) { nPosX = 1; nPosZ =  2; }	//Up Buffer(3)
			if (gData.nTransferX2Pos ==  5) { nPosX = 3; nPosZ =  5; }	//Up NG Elevator(5)
			if (gData.nTransferX2Pos ==  6) { nPosX = 4; nPosZ =  7; }	//Up Good Elevator(6)
			if (gData.nTransferX2Pos ==  7) { nPosX = 6; nPosZ = 12; }	//Up NG-Stage12(7)
			if (gData.nTransferX2Pos ==  8) { nPosX = 7; nPosZ = 13; }	//Up NG-Stage12(8)
			if (gData.nTransferX2Pos ==  9) { nPosX = 8; nPosZ = 14; }	//Up Good-Stage12(9)
			if (gData.nTransferX2Pos == 10) { nPosX = 9; nPosZ = 15; }	//Up Good-Stage12(10)

			g_objCommon.Move_Position(AX_TRANSFER_X2, nPosX);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(30000);
		}
	case 7:
		if (g_objCommon.Check_Position(AX_TRANSFER_X2, nPosX)) {
			g_objCommon.Save_Motion(AX_TRANSFER_X2, nPosX);
			m_tTransfer2Loop.Takt_Save(9, 1); m_tTransfer2Loop.Takt_Start();
			m_nTransfer2Case = 9; m_tTransfer2Loop.Set_LoopTime(5000);
		}
		break;
	
	case 9:
		if (gData.nTransferX2Pos ==  3) m_nTransfer2Case = 10;	//Up Buffer(3)
		if (gData.nTransferX2Pos ==  5) m_nTransfer2Case = 20;	//Up NG Elevator(5)
		if (gData.nTransferX2Pos ==  6) m_nTransfer2Case = 30;	//Up Good Elevator(6)
		if (gData.nTransferX2Pos ==  7) m_nTransfer2Case = 40;	//Up NG-Stage12(7)
		if (gData.nTransferX2Pos ==  8) m_nTransfer2Case = 40;	//Up NG-Stage12(8)
		if (gData.nTransferX2Pos ==  9) m_nTransfer2Case = 50;	//Up Good-Stage12(9)
		if (gData.nTransferX2Pos == 10) m_nTransfer2Case = 50;	//Up Good-Stage12(10)
		m_tTransfer2Loop.Set_LoopTime(5000);
		break;

	case 10:	//Up Buffer(3)
		if (m_nElevator2Case == 0) m_nElevator2Case = 1;
		if (m_nElevator2Case == 20) {
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 11:
		if (m_nElevator2Case == 20) {
			if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
				m_tTransfer2Loop.Takt_Save(9, 2); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, nPosZ);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 12:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, nPosZ)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, nPosZ);
			m_tTransfer2Loop.Takt_Save(9, 3); m_tTransfer2Loop.Takt_Start();
			m_pDY03->oTransferRGrabOpen = FALSE; m_pDY03->oTransferRGrabClose = TRUE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 13:
		if (!m_pDX03->iTransferRGrab1Open && m_pDX03->iTransferRGrab1Close) {
			if (!m_pDX03->iTransferRGrab2Open && m_pDX03->iTransferRGrab2Close) {
				if (!m_tTransfer2Loop.Waiting_Time(500)) break;
				m_tTransfer2Loop.Takt_Save(9, 4); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, 0);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 14:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 15:
		if (m_pDX03->iTransferRTrayExist) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, 0);
			m_tTransfer2Loop.Takt_Save(9, 5); m_tTransfer2Loop.Takt_Start();
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 16:
			m_sLog.Format("[Transfer2: Up Buffer] Lot(%s) TNo(%d) PNo(%d)", gData.sLotID_Tansfer[1], gData.nTrayNo_Tansfer[1], gData.nPortNo_Tansfer[1]);
			g_objLogFile.Save_HandlerLog(m_sLog);

			gData.sLotID_Tansfer[1] = "";
			gData.nTrayNo_Tansfer[1] = gData.nPortNo_Tansfer[1] = 0;
			gData.nCarrierCount--;

			if (m_nElevator2Case == 20) m_nElevator2Case = 22;
			m_nTransfer2Case = 60; m_tTransfer2Loop.Set_LoopTime(5000);
			m_tTransfer2Loop.Takt_Save(9, 6); m_tTransfer2Loop.Takt_Start();
		break;

	case 20:	//Up NG Elevator(5)
		if (m_nNGMZElevatorCase == 30) {
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 21:
		if (m_nNGMZElevatorCase == 30) {
			if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
				if (g_objCommon.Check_Position(AX_NG_FEEDER_Y, 0)) {
					m_tTransfer2Loop.Takt_Save(9, 7); m_tTransfer2Loop.Takt_Start();
					g_objCommon.Move_Position(AX_TRANSFER_Z2, nPosZ);
					m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 22:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, nPosZ)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, nPosZ);
			m_tTransfer2Loop.Takt_Save(9, 8); m_tTransfer2Loop.Takt_Start();
			m_pDY03->oTransferRGrabOpen = FALSE; m_pDY03->oTransferRGrabClose = TRUE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 23:
		if (!m_pDX03->iTransferRGrab1Open && m_pDX03->iTransferRGrab1Close) {
			if (!m_pDX03->iTransferRGrab2Open && m_pDX03->iTransferRGrab2Close) {
				if (g_objCommon.Check_Position(AX_NG_FEEDER_Y, 0)) {
					m_tTransfer2Loop.Takt_Save(9, 9); m_tTransfer2Loop.Takt_Start();
					g_objCommon.Move_Position(AX_TRANSFER_Z2, 0);
					m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 24:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, 0);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 25:
		if (m_pDX03->iTransferRTrayExist) {
			m_tTransfer2Loop.Takt_Save(9, 10); m_tTransfer2Loop.Takt_Start();
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 26:
			m_sLog.Format("[Transfer2: MZ Up from NG-Elevator] Lot(%s) TNo(%d) PNo(%d)", gData.sLotID_Tansfer[1], gData.nTrayNo_Tansfer[1], gData.nPortNo_Tansfer[1]);
			g_objLogFile.Save_HandlerLog(m_sLog);

			gData.sLotID_Tansfer[1] = "";
			gData.nTrayNo_Tansfer[1] = gData.nPortNo_Tansfer[1] = 0;

			if (m_nNGMZElevatorCase == 30) m_nNGMZElevatorCase = 31;
			m_nTransfer2Case = 60; m_tTransfer2Loop.Set_LoopTime(5000);
			m_tTransfer2Loop.Takt_Save(9, 11); m_tTransfer2Loop.Takt_Start();
		break;

	case 30:	//Up Good Elevator(6)
		if (m_nGDMZElevatorCase == 30) {
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 31:
		if (m_nGDMZElevatorCase == 30) {
			if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
				if (g_objCommon.Check_Position(AX_GOOD_FEEDER_Y, 0)) {
					m_tTransfer2Loop.Takt_Save(9, 12); m_tTransfer2Loop.Takt_Start();
					g_objCommon.Move_Position(AX_TRANSFER_Z2, nPosZ);
					m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 32:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, nPosZ)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, nPosZ);
			m_tTransfer2Loop.Takt_Save(9, 13); m_tTransfer2Loop.Takt_Start();
			m_pDY03->oTransferRGrabOpen = FALSE; m_pDY03->oTransferRGrabClose = TRUE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 33:
		if (!m_pDX03->iTransferRGrab1Open && m_pDX03->iTransferRGrab1Close) {
			if (!m_pDX03->iTransferRGrab2Open && m_pDX03->iTransferRGrab2Close) {
				if (g_objCommon.Check_Position(AX_GOOD_FEEDER_Y, 0)) {
					m_tTransfer2Loop.Takt_Save(9, 14); m_tTransfer2Loop.Takt_Start();
					g_objCommon.Move_Position(AX_TRANSFER_Z2, 0);
					m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 34:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, 0);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 35:
		if (m_pDX03->iTransferRTrayExist) {
			m_tTransfer2Loop.Takt_Save(9, 15); m_tTransfer2Loop.Takt_Start();
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 36:
			m_sLog.Format("[Transfer2: Up Good-Elevator] Lot(%s) TNo(%d) PNo(%d)", gData.sLotID_Tansfer[1], gData.nTrayNo_Tansfer[1], gData.nPortNo_Tansfer[1]);
			g_objLogFile.Save_HandlerLog(m_sLog);

			gData.sLotID_Tansfer[1] = "";
			gData.nTrayNo_Tansfer[1] = gData.nPortNo_Tansfer[1] = 0;

			if (m_nGDMZElevatorCase == 30) m_nGDMZElevatorCase = 31;
			m_nTransfer2Case = 60; m_tTransfer2Loop.Set_LoopTime(5000);
			m_tTransfer2Loop.Takt_Save(9, 16); m_tTransfer2Loop.Takt_Start();
		break;

	case 40:	//Up NG-Stage12(7,8)
		if ((gData.nTransferX2Pos == 7 && m_nNGStage1Case == 60) ||
			(gData.nTransferX2Pos == 8 && m_nNGStage2Case == 60) ) {
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 41:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			m_tTransfer2Loop.Takt_Save(9, 17); m_tTransfer2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANSFER_Z2, nPosZ);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, nPosZ)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, nPosZ);
			m_tTransfer2Loop.Takt_Save(9, 18); m_tTransfer2Loop.Takt_Start();
			m_pDY03->oTransferRGrabOpen = FALSE; m_pDY03->oTransferRGrabClose = TRUE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 43:
		if (!m_pDX03->iTransferRGrab1Open && m_pDX03->iTransferRGrab1Close) {
			if (!m_pDX03->iTransferRGrab2Open && m_pDX03->iTransferRGrab2Close) {
				if (!m_tTransfer2Loop.Waiting_Time(300)) break;
				m_tTransfer2Loop.Takt_Save(9, 19); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, 0);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 44:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, 0);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 45:
		if (m_pDX03->iTransferRTrayExist) 
		{
			gCap.nTransferMZIndex[1] = gCap.nCarrierMZIndex[1]; gCap.nCarrierMZIndex[1] = 0;
			
			gData.sLotID_Tansfer[1] = gData.sLotID_NGTray[gData.nTransferX2Pos-7];
			gData.nTrayNo_Tansfer[1] = gData.nTrayNo_NGTray[gData.nTransferX2Pos-7];
			gData.nPortNo_Tansfer[1] = gData.nPortNo_NGTray[gData.nTransferX2Pos-7];
			gData.sCarID_Tansfer     = gData.sCarID_NGTray[gData.nTransferX2Pos-7];
			gData.sLotID_Tansfer[2]  = gData.sLotID_Tansfer[1];

			gData.sLotID_NGTray[gData.nTransferX2Pos-7] = gData.sCarID_NGTray[gData.nTransferX2Pos-7] = "";
			gData.nTrayNo_NGTray[gData.nTransferX2Pos-7] = gData.nPortNo_NGTray[gData.nTransferX2Pos-7] = 0;

			for(int y = 0; y < 10; y++)
			{
				for(int x = 0; x < 4; x++)
				{
					gTracking.sMZID_Transfer2[y][x] = gTracking.sMZID_NGTray[y][x];
					gTracking.sMZID_NGTray[y][x] = "";

					m_sLog.Format("NGTray To Transfer2, x:%d Y:%d - MZ ID:%s", x, y , gTracking.sMZID_Transfer2[y][x]);
					if(gTracking.sMZID_Transfer2[y][x] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);
				}
			}
			
			if (m_nNGStage1Case == 60) m_nNGStage1Case = 61;
			if (m_nNGStage2Case == 60) m_nNGStage2Case = 61;
			m_nTransfer2Case = 60; m_tTransfer2Loop.Set_LoopTime(5000);

			m_sLog.Format("[Transfer2: Up NG-Stage%d] Lot(%s) TNo(%d) PNo(%d)", (gData.nTransferX2Pos-6), gData.sLotID_Tansfer[1], gData.nTrayNo_Tansfer[1], gData.nPortNo_Tansfer[1]);
			g_objLogFile.Save_HandlerLog(m_sLog);
			m_tTransfer2Loop.Takt_Save(9, 20); m_tTransfer2Loop.Takt_Start();
		}
		break;

	case 50:	//Up Good-Stage12(9,10)
		if ((gData.nTransferX2Pos ==  9 && m_nGoodStage1Case == 60) ||
			(gData.nTransferX2Pos == 10 && m_nGoodStage2Case == 60) ) {
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 51:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			m_tTransfer2Loop.Takt_Save(9, 21); m_tTransfer2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_TRANSFER_Z2, nPosZ);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 52:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, nPosZ)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, nPosZ);
			m_tTransfer2Loop.Takt_Save(9, 22); m_tTransfer2Loop.Takt_Start();
			m_pDY03->oTransferRGrabOpen = FALSE; m_pDY03->oTransferRGrabClose = TRUE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 53:
		if (!m_pDX03->iTransferRGrab1Open && m_pDX03->iTransferRGrab1Close) {
			if (!m_pDX03->iTransferRGrab2Open && m_pDX03->iTransferRGrab2Close) {
				if (!m_tTransfer2Loop.Waiting_Time(300)) break;
				m_tTransfer2Loop.Takt_Save(9, 23); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, 0);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 54:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, 0);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 55:
		if (m_pDX03->iTransferRTrayExist) 
		{
			
			gCap.nTransferMZIndex[0] = gCap.nCarrierMZIndex[0];gCap.nCarrierMZIndex[0] = 0;

			gData.sLotID_Tansfer[1] = gData.sLotID_GoodTray[gData.nTransferX2Pos-9];
			gData.nTrayNo_Tansfer[1] = gData.nTrayNo_GoodTray[gData.nTransferX2Pos-9];
			gData.nPortNo_Tansfer[1] = gData.nPortNo_GoodTray[gData.nTransferX2Pos-9];
			gData.sCarID_Tansfer     = gData.sCarID_GoodTray[gData.nTransferX2Pos-9];
			gData.sLotID_Tansfer[2]  = gData.sLotID_Tansfer[1];

			gData.sLotID_GoodTray[gData.nTransferX2Pos-9] = gData.sCarID_GoodTray[gData.nTransferX2Pos-9] = "";
			gData.nTrayNo_GoodTray[gData.nTransferX2Pos-9] = gData.nPortNo_GoodTray[gData.nTransferX2Pos-9] = 0;

			for(int y = 0; y < 10; y++)
			{
				for(int x = 0; x < 4; x++)
				{
					gTracking.sMZID_Transfer2[y][x] = gTracking.sMZID_GoodTray[y][x];
					gTracking.sMZID_GoodTray[y][x] = "";

					m_sLog.Format("GoodTray To Transfer2, x:%d Y:%d - MZ ID:%s", x, y , gTracking.sMZID_Transfer2[y][x]);
					if(gTracking.sMZID_Transfer2[y][x] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);
				}
			}
			if (m_nGoodStage1Case == 60) m_nGoodStage1Case = 61;
			if (m_nGoodStage2Case == 60) m_nGoodStage2Case = 61;
			m_nTransfer2Case = 60; m_tTransfer2Loop.Set_LoopTime(5000);

			m_sLog.Format("[Transfer2: Up Good-Stage%d] Lot(%s) TNo(%d) PNo(%d)", (gData.nTransferX2Pos-8), gData.sLotID_Tansfer[1], gData.nTrayNo_Tansfer[1], gData.nPortNo_Tansfer[1]);
			g_objLogFile.Save_HandlerLog(m_sLog);
			m_tTransfer2Loop.Takt_Save(9, 24); m_tTransfer2Loop.Takt_Start();
		}
		break;

//	gData.nTransferX2Pos;	//1(LS1),2(LS2),3(Buf),4(LE),5(NE),6(GE),7(NG1),8(NG2),9(Good1),10(Good2)
	//1순위 Good-Stage12(9,10)-> Good Elevator(6)
	//2순위 NG-Stage12  (7,8) -> NG Elevator(5)
	//3순위 Good Elevator(6)  -> Good,NG Stage(7~10)
	//4순위 NG Elevator  (5)  -> Good,NG Stage(7~10)
	//5순위 Buffer       (3)  -> Good,NG Stage(7~10)
	//6순위 Good Elevator(6)  -> Buffer(3)
	//7순위 NG Elevator  (5)  -> Buffer(3)

	case 60:	//Tray Down
		if (nToTran2Pos == 3) {
			if (gData.nTransferXLock == 0) {
				gData.nTransferXLock = 1;
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		} else {
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 61:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			m_tTransfer2Loop.Takt_Save(9, 25); m_tTransfer2Loop.Takt_Start();
			gData.nTransferX2Pos = nToTran2Pos;
			if (gData.nTransferX2Pos ==  3) { nPosX = 1; nPosZ =  1; }	//Down Buffer(3)
			if (gData.nTransferX2Pos ==  5) { nPosX = 3; nPosZ =  4; }	//Down NG Elevator(5)
			if (gData.nTransferX2Pos ==  6) { nPosX = 4; nPosZ =  6; }	//Down Good Elevator(6)
			if (gData.nTransferX2Pos ==  7) { nPosX = 6; nPosZ =  8; }	//Down NG-Stage 1(7)
			if (gData.nTransferX2Pos ==  8) { nPosX = 7; nPosZ =  9; }	//Down NG-Stage 2(8)
			if (gData.nTransferX2Pos ==  9) { nPosX = 8; nPosZ = 10; }	//Down Good-Stage 1(9)
			if (gData.nTransferX2Pos == 10) { nPosX = 9; nPosZ = 11; }	//Down Good-Stage 2(10)
			g_objCommon.Move_Position(AX_TRANSFER_X2, nPosX);
			if (nFmTran2Pos == 3) gData.nTransferXLock = 0;
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(30000);
		}
	case 62:
		if (g_objCommon.Check_Position(AX_TRANSFER_X2, nPosX)) {
			g_objCommon.Save_Motion(AX_TRANSFER_X2, nPosX);
			m_tTransfer2Loop.Takt_Save(9, 26); m_tTransfer2Loop.Takt_Start();
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(30000);
		}
		break;
	case 63:
		if (gData.nTransferX2Pos ==  3) m_nTransfer2Case = 70;	//Down Buffer(3)
		if (gData.nTransferX2Pos ==  5) m_nTransfer2Case = 75;	//Down NG Elevator(5)
		if (gData.nTransferX2Pos ==  6) m_nTransfer2Case = 75;	//Down Good Elevator(6)
		if (gData.nTransferX2Pos ==  7) m_nTransfer2Case = 80;	//Down NG-Stage 1(7)
		if (gData.nTransferX2Pos ==  8) m_nTransfer2Case = 80;	//Down NG-Stage 2(8)
		if (gData.nTransferX2Pos ==  9) m_nTransfer2Case = 85;	//Down Good-Stage 1(9)
		if (gData.nTransferX2Pos == 10) m_nTransfer2Case = 85;	//Down Good-Stage 2(10)
		m_tTransfer2Loop.Set_LoopTime(5000);
		break;

	case 70:	//Down Buffer(3)
		if (m_nElevator2Case == 0) m_nElevator2Case = 1;
		if (m_nElevator2Case == 20) {
			if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
				m_tTransfer2Loop.Takt_Save(9, 27); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, nPosZ);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;
	case 71:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, nPosZ)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, nPosZ);
			m_tTransfer2Loop.Takt_Save(9, 28); m_tTransfer2Loop.Takt_Start();
			m_pDY03->oTransferRGrabOpen = TRUE; m_pDY03->oTransferRGrabClose = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 72:
		if (m_pDX03->iTransferRGrab1Open && !m_pDX03->iTransferRGrab1Close) {
			if (m_pDX03->iTransferRGrab2Open && !m_pDX03->iTransferRGrab2Close) {
				if (!m_tTransfer2Loop.Waiting_Time(300)) break;
				m_tTransfer2Loop.Takt_Save(9, 29); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, 0);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 73:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			if (!m_pDX03->iTransferRTrayExist) {
				g_objCommon.Save_Motion(AX_TRANSFER_Z2, 0);
				m_sLog.Format("[Transfer2: Down Buffer] Lot(%s) TNo(%d) PNo(%d)", gData.sLotID_Tansfer[1], gData.nTrayNo_Tansfer[1], gData.nPortNo_Tansfer[1]);
				g_objLogFile.Save_HandlerLog(m_sLog);

				gData.sLotID_Tansfer[1] = "";
				gData.nTrayNo_Tansfer[1] = gData.nPortNo_Tansfer[1] = 0;
				gData.nCarrierCount++;

				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(5000);
				m_tTransfer2Loop.Takt_Save(9, 30); m_tTransfer2Loop.Takt_Start();
			}
		}
		break;
	case 74:
			if (m_nElevator2Case == 20) m_nElevator2Case = 21;
			m_nTransfer2Case = 95; m_tTransfer2Loop.Set_LoopTime(5000);
		break;


	case 75:	//Down NG-Good Elevator(5,6)
		if ((gData.nTransferX2Pos == 5 && (m_nNGMZElevatorCase == 23 || m_nNGMZElevatorCase == 70)) ||
			(gData.nTransferX2Pos == 6 && (m_nNGMZElevatorCase == 23 || m_nGDMZElevatorCase == 70)) ) {
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 76:
		if ((gData.nTransferX2Pos == 5 && (m_nNGMZElevatorCase == 23 || m_nNGMZElevatorCase == 70)) ||
			(gData.nTransferX2Pos == 6 && (m_nNGMZElevatorCase == 23 || m_nGDMZElevatorCase == 70)) ) {
			if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
				m_tTransfer2Loop.Takt_Save(9, 31); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, nPosZ);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 77:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, nPosZ)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, nPosZ);
			m_tTransfer2Loop.Takt_Save(9, 32); m_tTransfer2Loop.Takt_Start();
			m_pDY03->oTransferRGrabOpen = TRUE; m_pDY03->oTransferRGrabClose = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 78:
		if (m_pDX03->iTransferRGrab1Open && !m_pDX03->iTransferRGrab1Close) {
			if (m_pDX03->iTransferRGrab2Open && !m_pDX03->iTransferRGrab2Close) {
				if (!m_tTransfer2Loop.Waiting_Time(300)) break;
				m_tTransfer2Loop.Takt_Save(9, 33); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, 0);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 79:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) 
		{
			if (!m_pDX03->iTransferRTrayExist) 
			{
				g_objCommon.Save_Motion(AX_TRANSFER_Z2, 0);
				nPort2No = gData.nPortNo_Tansfer[1];
				if (gData.nTransferX2Pos == 5) {	//NG-Elevator Down
					gData.sMZID_NGElevator[0] = gLot.sMZID_LD[nPort2No-1];
					m_sLog.Format("[Transfer2: Down NG-Elevator-Fm%d-Lot%d-G] Lot(%s) TNo(%d) PNo(%d-%d) LoadMZ(%s)", nFmTran2Pos, gData.nTransferX2Pos, gData.sLotID_Tansfer[1], gData.nTrayNo_Tansfer[1], gData.nPortNo_Tansfer[1], nPort2No, gData.sMZID_NGElevator[0]);
				} else {							//Good-Elevator Down
					m_sLog.Format("[Transfer2: Down Good-Elevator-Fm%d-Lot%d-N] Lot(%s) TNo(%d) PNo(%d-%d)", nFmTran2Pos, gData.nTransferX2Pos, gData.sLotID_Tansfer[1], gData.nTrayNo_Tansfer[1], gData.nPortNo_Tansfer[1], nPort2No);
//					g_objCommon.Set_LotDataCopy(nPort2No, 30);
				}
				g_objLogFile.Save_HandlerLog(m_sLog);

				if (nPort2No == 0) nPort2No = gLot.nUnLastPortID;
				if (gData.nTransferX2Pos == 6) gLot.nUnlGdTrayCount[nPort2No-1]++;
				else						   gLot.nUnlNGTrayCount[nPort2No-1]++;

				if (gData.sLotID_Tansfer[1].GetLength() > 0) gLot.sUnLastLotID = gData.sLotID_Tansfer[1];
				if (gData.nPortNo_Tansfer[1] > 0)			 gLot.nUnLastPortID = gData.nPortNo_Tansfer[1];

				if (gData.nTransferX2Pos == 5) {	//NG-Elevator Down
					gData.sCarID_Elevator[0] = gData.sCarID_Tansfer; gData.sCarID_Tansfer = "";
					gData.sLotID_UnMZ[0]     = gData.sLotID_Tansfer[1];
					gData.nPortNo_UnMZ[0]    = gData.nPortNo_Tansfer[1];

					for(int y = 0; y < 10; y++)
					{
						for(int x = 0; x < 4; x++)
						{
							gTracking.sMZID_NGMZElev[gData.nPortNo_UnMZ[0] -1][y][x] = gTracking.sMZID_Transfer2[y][x];
							gTracking.sMZID_Transfer2[y][x] = "";

							m_sLog.Format("Transfer2 To NG MZ Elev, SlotNo:%d, x:%d, Y:%d - MZ ID:%s",gData.nPortNo_UnMZ[0], x, y , gTracking.sMZID_NGMZElev[gData.nPortNo_UnMZ[0] -1][y][x]);
							if(gTracking.sMZID_NGMZElev[gData.nPortNo_UnMZ[0] -1][y][x] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);
						}
					}


					if (m_nNGMZElevatorCase == 23) m_nNGMZElevatorCase = 24;
					if (m_nNGMZElevatorCase == 70) m_nNGMZElevatorCase = 71;
				}
				if (gData.nTransferX2Pos == 6) 
				{	//Good-Elevator Down
					gData.sCarID_Elevator[1] = gData.sCarID_Tansfer; gData.sCarID_Tansfer = "";
					gData.sLotID_UnMZ[1]     = gData.sLotID_Tansfer[1];
					gData.nPortNo_UnMZ[1]    = gData.nPortNo_Tansfer[1];

					for(int y = 0; y < 10; y++)
					{
						for(int x = 0; x < 4; x++)
						{
							gTracking.sMZID_GoodMZElev[gData.nPortNo_UnMZ[1] - 1][y][x] = gTracking.sMZID_Transfer2[y][x];
							gTracking.sMZID_Transfer2[y][x] = "";

							m_sLog.Format("Transfer2 To Good MZ Elev, SlotNo:%d, x:%d, Y:%d - MZ ID:%s",gData.nPortNo_UnMZ[1] - 1, x, y , gTracking.sMZID_GoodMZElev[gData.nPortNo_UnMZ[1] - 1][y][x]);
							if(gTracking.sMZID_GoodMZElev[gData.nPortNo_UnMZ[1] - 1][y][x] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);
						}
					}
					if (m_nGDMZElevatorCase == 70) m_nGDMZElevatorCase = 71;
				}

				gData.sLotID_Tansfer[1] = "";
				gData.nTrayNo_Tansfer[1] = gData.nPortNo_Tansfer[1] = 0;

				m_nTransfer2Case = 95; m_tTransfer2Loop.Set_LoopTime(5000);
				m_tTransfer2Loop.Takt_Save(9, 34); m_tTransfer2Loop.Takt_Start();
			}
		}
		break;

	case 80:	//Down NG-Stage 12 (7,8)
		if ((gData.nTransferX2Pos == 7 && m_nNGStage1Case == 0) ||
			(gData.nTransferX2Pos == 8 && m_nNGStage2Case == 0) ) {
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 81:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			if ((gData.nTransferX2Pos == 7 && m_pDX11->iNGStage1MasterOut && m_pDX11->iNGStage1SlaveOut) ||
				(gData.nTransferX2Pos == 8 && m_pDX11->iNGStage2MasterOut && m_pDX11->iNGStage2SlaveOut) ) {
				m_tTransfer2Loop.Takt_Save(9, 35); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, nPosZ);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 82:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, nPosZ)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, nPosZ);
			m_tTransfer2Loop.Takt_Save(9, 36); m_tTransfer2Loop.Takt_Start();
			m_pDY03->oTransferRGrabOpen = TRUE; m_pDY03->oTransferRGrabClose = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 83:
		if (m_pDX03->iTransferRGrab1Open && !m_pDX03->iTransferRGrab1Close) {
			if (m_pDX03->iTransferRGrab2Open && !m_pDX03->iTransferRGrab2Close) {
				if (!m_tTransfer2Loop.Waiting_Time(300)) break;
				m_tTransfer2Loop.Takt_Save(9, 37); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, 0);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 84:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			if (!m_pDX03->iTransferRTrayExist) {
				g_objCommon.Save_Motion(AX_TRANSFER_Z2, 0);
				m_sLog.Format("[Transfer2: Down NG-Stage%d] Lot(%s) TNo(%d) PNo(%d)", (gData.nTransferX2Pos-6), gData.sLotID_Tansfer[1], gData.nTrayNo_Tansfer[1], gData.nPortNo_Tansfer[1]);
				g_objLogFile.Save_HandlerLog(m_sLog);
				
				gData.sLotID_NGTray[gData.nTransferX2Pos-7] = gData.sLotID_Tansfer[1];
				gData.nTrayNo_NGTray[gData.nTransferX2Pos-7] = gData.nTrayNo_Tansfer[1];
				gData.nPortNo_NGTray[gData.nTransferX2Pos-7] = gData.nPortNo_Tansfer[1];
				gData.sLotID_NGTray[2] = gData.sLotID_NGTray[gData.nTransferX2Pos-7];

				gData.sLotID_Tansfer[1] = "";
				gData.nTrayNo_Tansfer[1] = gData.nPortNo_Tansfer[1] = 0;

				if (!Check_ModuleEmpty()) {
					if (m_nNGStage1Case == 0) m_nNGStage1Case = 1;
					if (m_nNGStage2Case == 0) m_nNGStage2Case = 1;
				}
				m_nTransfer2Case = 95; m_tTransfer2Loop.Set_LoopTime(5000);
				m_tTransfer2Loop.Takt_Save(9, 38); m_tTransfer2Loop.Takt_Start();
			}
		}
		break;

	case 85:	//Down Good-Stage 12 (9,10)
		if ((gData.nTransferX2Pos ==  9 && m_nGoodStage1Case == 0) ||
			(gData.nTransferX2Pos == 10 && m_nGoodStage2Case == 0) ) {
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 86:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			if ((gData.nTransferX2Pos ==  9 && m_pDX12->iGoodStage1MasterOut && m_pDX12->iGoodStage1SlaveOut) ||
				(gData.nTransferX2Pos == 10 && m_pDX12->iGoodStage2MasterOut && m_pDX12->iGoodStage2SlaveOut) ) {
				m_tTransfer2Loop.Takt_Save(9, 39); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, nPosZ);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 87:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, nPosZ)) {
			g_objCommon.Save_Motion(AX_TRANSFER_Z2, nPosZ);
			m_tTransfer2Loop.Takt_Save(9, 40); m_tTransfer2Loop.Takt_Start();
			m_pDY03->oTransferRGrabOpen = TRUE; m_pDY03->oTransferRGrabClose = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
		}
		break;
	case 88:
		if (m_pDX03->iTransferRGrab1Open && !m_pDX03->iTransferRGrab1Close) {
			if (m_pDX03->iTransferRGrab2Open && !m_pDX03->iTransferRGrab2Close) {
				if (!m_tTransfer2Loop.Waiting_Time(300)) break;
				m_tTransfer2Loop.Takt_Save(9, 41); m_tTransfer2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_TRANSFER_Z2, 0);
				m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 89:
		if (g_objCommon.Check_Position(AX_TRANSFER_Z2, 0)) {
			if (!m_pDX03->iTransferRTrayExist) {
				g_objCommon.Save_Motion(AX_TRANSFER_Z2, 0);
				m_sLog.Format("[Transfer2: Down Good-Stage%d] Lot(%s) TNo(%d) PNo(%d)", (gData.nTransferX2Pos-8), gData.sLotID_Tansfer[1], gData.nTrayNo_Tansfer[1], gData.nPortNo_Tansfer[1]);
				g_objLogFile.Save_HandlerLog(m_sLog);
	
				gData.sLotID_GoodTray[gData.nTransferX2Pos-9] = gData.sLotID_Tansfer[1];
				gData.nTrayNo_GoodTray[gData.nTransferX2Pos-9] = gData.nTrayNo_Tansfer[1];
				gData.nPortNo_GoodTray[gData.nTransferX2Pos-9] = gData.nPortNo_Tansfer[1];
				gData.sLotID_GoodTray[2] = gData.sLotID_GoodTray[gData.nTransferX2Pos-9];

				gData.sLotID_Tansfer[1] = "";
				gData.nTrayNo_Tansfer[1] = gData.nPortNo_Tansfer[1] = 0;

				if (!Check_ModuleEmpty()) {
					if (m_nGoodStage1Case == 0) m_nGoodStage1Case = 1;
					if (m_nGoodStage2Case == 0) m_nGoodStage2Case = 1;
				}
				m_nTransfer2Case = 95; m_tTransfer2Loop.Set_LoopTime(5000);
				m_tTransfer2Loop.Takt_Save(9, 42); m_tTransfer2Loop.Takt_Start();
			}
		}
		break;

//	gData.nTransferX2Pos;	//1(LS1),2(LS2),3(L1),4(L2),5(EN),6(EG),7(NB),8(U1),9(U2),10(NG1),11(NG2),12(Good1),13(Good2)
	case 95:	//대기위치로
		if (nToTran2Pos != 3 && Check_Transfer2(nFmTran2Pos, nToTran2Pos, nPort2No)) {
			m_nTransfer2Case = 0; m_tTransfer2Loop.Set_LoopTime(5000);
		} else {
			if (gData.nTransferX2Pos == 3)											 { nPosX = 3; }
			else if (m_pDX12->iGoodStage1TrayExist && m_pDX12->iGoodStage2TrayExist) { nPosX = gData.nTransferX2Pos = 7; }
			else if (m_pDX11->iNGStage1TrayExist   && m_pDX11->iNGStage2TrayExist)	 { nPosX = gData.nTransferX2Pos = 7; }
			else																	 { nPosX = gData.nTransferX2Pos = 5; }
			g_objCommon.Move_Position(AX_TRANSFER_X2, nPosX);
			if (nToTran2Pos == 3) gData.nTransferXLock = 0;
			m_nTransfer2Case++; m_tTransfer2Loop.Set_LoopTime(30000);
		}
		break;
	case 96:
		if (g_objCommon.Check_Position(AX_TRANSFER_X2, nPosX)) {
			g_objCommon.Save_Motion(AX_TRANSFER_X2, nPosX);
			m_tTransfer2Loop.Takt_Save(9, 43);
			m_nTransfer2Case = 0; m_tTransfer2Loop.Set_LoopTime(5000);
		}
		break;

	}

	//  9. (Error : 3500)
	if (m_tTransfer2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(3500 + m_nTransfer2Case);
		return FALSE;
	}
	return TRUE;
}

// 10. (Error : 4000)
BOOL CSequenceMain::Run_LoadStage1()
{
	static int nStageNo1 = 0, nSNo1Vision, nPortNo1;	//0고정

	switch (m_nLoadStage1Case) {
	case 0:	// Wait
		m_tLoadStage1Loop.Set_LoopTime(5000);
		return TRUE;

	//Load Stage 2 case 48에서 "m_nLoadStage1Case = 1"
	case 1:
		if (m_pDX04->iLoadStage1TrayExist) 
		{
			if (m_pDX04->iLoadStage1MasterIn && !m_pDX04->iLoadStage1MasterOut) 
			{
				if (!m_tLoadStage1Loop.Waiting_Time(200)) break;
				gNG->nTrayOX[0][0] = 1;
				m_tLoadStage1Loop.Takt_Start();
				m_pDY04->oLoadStage1SlaveIn = TRUE; m_pDY04->oLoadStage1SlaveOut = FALSE;
				g_objAJinAXL.Write_Output(4);
				m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

				m_sLog.Format("MCC,10,LoadStage1,%d, oLoadStage1SlaveIn ", m_nLoadStage1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 2:
		if (m_pDX04->iLoadStage1MasterIn && !m_pDX04->iLoadStage1MasterOut)
		{
			if (m_pDX04->iLoadStage1SlaveIn && !m_pDX04->iLoadStage1SlaveOut)
			{
				if (m_pDX04->iLoadStage1Up && !m_pDX04->iLoadStage1Down) 
				{
					m_tLoadStage1Loop.Takt_Save(10, 1); m_tLoadStage1Loop.Takt_Start();
					m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

					m_sLog.Format("MCC,10,LoadStage1,%d, iLoadStage1Up Check ", m_nLoadStage1Case);
					g_objLogFile.Save_SeqLog(m_sLog);
				}
			}
		}
		break;
	case 3:	//RFID Wait
		if (m_nLoadStage2Case >= 30)
		{
			m_tLoadStage1Loop.Takt_Save(10, 2); m_tLoadStage1Loop.Takt_Start();
			m_nRFRetray[0][0] = 0;
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y1, 6);	//RFID
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);

			m_sLog.Format("MCC,10,LoadStage1,%d, AX_LOAD_STAGE_Y1 move to RFID Pos ", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;

	case 4:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y1, 6)) 
		{
			if (m_pEquipData->bUseRFIDLoad)
			{
				g_objCarrierRFID_Load.Send_RFIDRead();
				m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

				m_sLog.Format("MCC,10,LoadStage1,%d, Send_RFIDRead ", m_nLoadStage1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			} 
			else 
			{
				if (Search_Lot()) 
				{
					m_nLoadStage1Case = 10; m_tLoadStage1Loop.Set_LoopTime(5000);
					m_sLog.Format("MCC,10,LoadStage1,%d, Search_Lot ", m_nLoadStage1Case);
					g_objLogFile.Save_SeqLog(m_sLog);
				}
			}
		}
		break;
	case 5:
		if (g_objCarrierRFID_Load.Is_RecvComplete())
		{
			gData.sCarID_LoadStage[nStageNo1] = g_objCarrierRFID_Load.Get_CarrierID();
			if (gData.sCarID_LoadStage[nStageNo1].GetLength() > 2) 
			{
				g_objMesAgent.Set_CarrierOutMGZ(gData.sMZID_LoadStage[nStageNo1], gData.sCarID_LoadStage[nStageNo1], gData.nSlotNo_LoadStage[nStageNo1]);
				m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

				m_sLog.Format("MCC,10,LoadStage1,%d, RFID recv done and Set_CarrierOutMGZ ", m_nLoadStage1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 6:
		if (m_pEquipData->bUseMES) 
		{
			if (gData.sCarID_LoadStage[nStageNo1].GetLength() > 2) 
			{
				gMes.nCarConfirm[LOAD_STAGE] = 1;
				g_objMesAgent.Set_CarrierIDReport("L", gData.sMZID_LoadStage[nStageNo1], gData.sCarID_LoadStage[nStageNo1]);
				m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

				m_sLog.Format("MCC,10,LoadStage1,%d, Set_CarrierIDReport MzID:%s  CarrierID:%s ", m_nLoadStage1Case, gData.sMZID_LoadStage[nStageNo1], gData.sCarID_LoadStage[nStageNo1]);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		} 
		else 
		{
			if (Search_Lot()) 
			{	
				g_objLogFile.Save_RFBarData(4, gData.sCarID_LoadStage[nStageNo1], gLot.nCmCount[gLot.nJobCycle-1]);
				m_nLoadStage1Case = 10; m_tLoadStage1Loop.Set_LoopTime(5000);

				m_sLog.Format("MCC,10,LoadStage1,%d, Save_RFBarData ", m_nLoadStage1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 7:
		if (gMes.nCarConfirm[LOAD_STAGE] > 1) {
			if (gMes.nCarConfirm[LOAD_STAGE] == 2) 
			{
				gMes.bRcpBodyDone = FALSE;	nSNo1Vision = 0;
				g_objInspector.Set_RecipeRequest(0, gMes.sHostLotID, gMes.sHostRecipe);
				m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

				m_sLog.Format("MCC,10,LoadStage1,%d-1, Set_RecipeRequest ", m_nLoadStage1Case);
				g_objLogFile.Save_SeqLog(m_sLog);

			} 
			else 
			{
				m_nLoadStage1Case = 9; m_tLoadStage1Loop.Set_LoopTime(10000);
				m_sLog.Format("MCC,10,LoadStage1,%d-2, nCarConfirm ? ", m_nLoadStage1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 8:	
		if (gMes.bRcpBodyDone) {
			g_objMesAgent.Set_PPSelectReport(gMes.sHostLotID, gMes.sRcpVersion);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(60000);

			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nTransfer1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 9:
		if (gMes.nCarConfirm[LOAD_STAGE] >= 4)
		{
			if (!m_tLoadStage1Loop.Waiting_Time(500)) break;

			nPortNo1 = gMes.nLotPortNo - 1;
			g_objMesAgent.Set_LotStart(gLot.sLotID[nPortNo1], gData.sMZID_LoadStage[nStageNo1], gData.nSlotNo_LoadStage[nStageNo1], gData.sCarID_LoadStage[nStageNo1], gLot.sRecipeName[nPortNo1]);
			g_objLogFile.Save_RFBarData(4, gData.sCarID_LoadStage[nStageNo1], gLot.nCmCount[gLot.nJobCycle-1]);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,g_objMesAgent.Set_LotStart and Save_RFBarData", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 10:	//Inspection Start
		if (gLot.nJobCycle > 0) 
		{
			nPortNo1 = gLot.nStartPno = gLot.nJobCycle - 1;
			if (m_pEquipData->bUseMES) 
			{
				gLot.sRecipeName[nPortNo1] = gMes.sHostRecipe;
				if (gLot.sLotID[nPortNo1].GetLength() < 5) 
				{
					gLot.sLotID[nPortNo1] = gMes.sHostLotID;
					gLot.nCmCount[nPortNo1] = gMes.nHostCmCount;
				}
			} 
			else	gLot.sRecipeName[nPortNo1] = gData.sRecipeName;
			
			gData.sReadyRecipe = gLot.sRecipeName[nPortNo1];
			gLot.nJobCount = gLot.nJobCount + gLot.nCmCount[nPortNo1];
			g_objInspector.Set_LotReady(FALSE);
			g_objInspector.Set_LotStart(INSPECTOR_ALL, gLot.sLotID[nPortNo1], nPortNo1+1, 1, gLot.nCmCount[nPortNo1], gLot.sRecipeName[nPortNo1], m_pEquipData->sPROG_VER, m_pEquipData->sPARA_VER, m_pEquipData->bUseCosmeticNG);

			gData.nLoadAlignRetry = 0;
			gLot.sMZID_LD[nPortNo1] = gData.sMZID_LoadStage[nStageNo1];
			gLot.sCarID_LD[nPortNo1] = gData.sCarID_LoadStage[nStageNo1];
			gLot.nSlotNo_LD[nPortNo1] = gData.nSlotNo_LoadStage[nStageNo1];
			gLot.nMZLastCar[nPortNo1] = gData.nLastCar_LoadStage[nStageNo1];

			gData.sLotID_LoadStage[nStageNo1] = gLot.sLotID[nPortNo1];
			gData.nTrayNo_LoadStage[nStageNo1] = 1;
			gData.nPortNo_LoadStage[nStageNo1] = gLot.nJobCycle;
			g_dlgWork.PostMessage(UM_UPDATE_MODEL, 1, NULL);

			Set_LotStart(gLot.sLotID[nPortNo1],  gLot.nJobCycle);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(60000);

			m_sLog.Format("MCC,10,LoadStage1,%d,g_objInspector.Set_LotStart and Set_LotStart", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 11:
		if (g_objInspector.Check_LotReady()) 
		{
			if (m_pDX04->iLoadStage1Up && !m_pDX04->iLoadStage1Down) 
			{
				if (m_pEquipData->bUseMES)
				{
					if (gData.sLotID_LoadStage[nStageNo1].GetLength() > 2 && gLot.sLotID[gMes.nLotPortNo-1].GetLength() < 2) 
					{
						gLot.nJobCycle = gMes.nLotPortNo;
						gLot.sLotID[gLot.nJobCycle-1] = gMes.sHostLotID;
						gLot.nCmCount[gLot.nJobCycle-1] = gMes.nHostCmCount;
						gLot.sRecipeName[gLot.nJobCycle-1] = gMes.sHostRecipe;
						g_dlgWork.PostMessage(UM_UPDATE_MODEL, 1, NULL);
					}
				}
				m_tLoadStage1Loop.Takt_Save(10, 3); m_tLoadStage1Loop.Takt_Start();
				g_objCommon.Move_Position(AX_LOAD_STAGE_Y1, 3);	//Align 1 pos
				m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

				m_sLog.Format("MCC,10,LoadStage1,%d,AX_LOAD_STAGE_Y1 move to Align 1 Pos", m_nLoadStage1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;

	case 12:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y1, 3)) 
		{
			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, 3); 
			m_tLoadStage1Loop.Takt_Save(10, 4); m_tLoadStage1Loop.Takt_Start();
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,Save_Motion", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 13:
		if (!m_pEquipData->bUseAlign1)
		{
			if (gNG->nTrayOX[0][0] == 1) Set_AlignData(gData.nPortNo_LoadStage[nStageNo1], nStageNo1);
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y1, 5);	//Aling3
			m_nLoadStage1Case = 19; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("[LoadStage1: Skip] AlingUse(%d) Exist(%d)", m_pEquipData->bUseAlign1, m_pDX04->iLoadStage1TrayExist);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_sLog.Format("MCC,10,LoadStage1,%d,AX_LOAD_STAGE_Y1 move to Align 3 Pos", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		else
		{
			Init_AlignTray();
			gData.bAlignScanDone = FALSE;
			g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A1", gData.sLotID_LoadStage[nStageNo1], gData.nPortNo_LoadStage[nStageNo1], gData.nTrayNo_LoadStage[nStageNo1], 1);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,Set_AlignRequest 1", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 14:
		if (gData.bAlignScanDone)
		{
			if (!m_tLoadStage1Loop.Waiting_Time(m_pEquipData->nDelayTime[5])) break;
			m_tLoadStage1Loop.Takt_Save(10, 5); m_tLoadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y1, 4);	//Align 2 
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);

			m_sLog.Format("MCC,10,LoadStage1,%d,bAlignScanDone and (AX_LOAD_STAGE_Y1) move to Align 2 Pos", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 15:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y1, 4)) 
		{
			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, 4);
			m_tLoadStage1Loop.Takt_Save(10, 6); m_tLoadStage1Loop.Takt_Start();
			gData.bAlignScanDone = FALSE;
			g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A1", gData.sLotID_LoadStage[nStageNo1], gData.nPortNo_LoadStage[nStageNo1], gData.nTrayNo_LoadStage[nStageNo1], 2);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,Set_AlignRequest 2", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 16:
		if (gData.bAlignScanDone) 
		{
			if (!m_tLoadStage1Loop.Waiting_Time(m_pEquipData->nDelayTime[5])) break;
			m_tLoadStage1Loop.Takt_Save(10, 7); m_tLoadStage1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y1, 5);	//Aling3
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);

			m_sLog.Format("MCC,10,LoadStage1,%d,bAlignScanDone and AX_LOAD_STAGE_Y1 move to Align 3 Pos", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 17:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y1, 5)) 
		{
			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, 5);
			m_tLoadStage1Loop.Takt_Save(10, 8); m_tLoadStage1Loop.Takt_Start();
			gData.bAlignScanDone = gData.bAlignA1Done = FALSE;
			g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A1", gData.sLotID_LoadStage[nStageNo1], gData.nPortNo_LoadStage[nStageNo1], gData.nTrayNo_LoadStage[nStageNo1], 3);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,Set_AlignRequest 3", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 18:
		if (gData.bAlignA1Done) 
		{
			if (!m_tLoadStage1Loop.Waiting_Time(m_pEquipData->nDelayTime[5])) break;
			if (m_pEquipData->bUseAlignOffset) 
			{
				if (m_pEquipData->dAlignOffset < fabs(gData.dAlignOffset[0][1])) 
				{
					gAlm.sAlmLID[0].Format("%0.3lf", m_pEquipData->dAlignOffset);
					gAlm.sAlmLID[1].Format("%0.3lf", fabs(gData.dAlignOffset[0][1]));
					g_objCommon.Show_Error(4118);
					return FALSE;
				}
			}
			m_tLoadStage1Loop.Takt_Save(10, 9); m_tLoadStage1Loop.Takt_Start();
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
			m_sLog.Format("MCC,10,LoadStage1,%d,bAlignA1Done", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 19:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y1, 5)) {
			if (gNG->nTrayOX[0][0] == 1) 
			{
				if (Check_AlignData(gData.nPortNo_LoadStage[nStageNo1]))
				{
					g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, 5);
					m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
				}
			} 
			else 
			{
				g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, 5);
				m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
			}
			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 20:	//Align Wait
		if (m_nLoadStage2Case > 20 && m_nLoadStage2Case < 40) 
		{
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y1, WaitPos_11);	//Wait
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,AX_LOAD_STAGE_Y1 wait", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		} 
		else if (m_nLoadStage2Case < 20 || (m_nLoadStage2Case >= 25 && m_nLoadStage2Case <= 29)  || m_nLoadStage2Case >= 40) 
		{
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y1, UnloadPos_11);	//Unload
			m_nLoadStage1Case = 23; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,AX_LOAD_STAGE_Y1 move to Unload Pos", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;
	case 21:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y1, 1)) 
		{
			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, 1);
			m_tLoadStage1Loop.Takt_Save(10, 10); m_tLoadStage1Loop.Takt_Start();
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 22:	//Align Wait
		if (m_nLoadStage2Case < 20 || m_nLoadStage2Case >= 40) 
		{
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y1, 2);	//Unload
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,AX_LOAD_STAGE_Y1 move to Unload Pos", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;
	case 23:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y1, 2)) 
		{
			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, 2);
			m_tLoadStage1Loop.Takt_Save(10, 11); m_tLoadStage1Loop.Takt_Start();
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 24:
		if (gNG->nTrayOX[0][0] == 0 || (gNG->nTrayOX[0][0] == 1 && m_pDX04->iLoadStage1TrayExist)) 
		{
			Set_AlignToStage(gData.nPortNo_LoadStage[nStageNo1], nStageNo1);
			m_nLoadStage1Case = 30; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 25:	//Lot취소
		if (m_pDX04->iLoadStage1Up && !m_pDX04->iLoadStage1Down) 
		{
			if (gData.nLastCar_LoadStage[nStageNo1] == 1) {
				int nPNo = gLot.nJobCycle - 1; if (nPNo < 0) nPNo = 27;
				gLot.nMZLastCar[nPNo] = gData.nLastCar_LoadStage[nStageNo1];
			}
			m_sLog.Format("[LoadStage1: Lot Cancel] LotID(%s) MZ(%s) Car(%s) Last(%d) Cycle(%d)", gData.sLotID_LoadStage[nStageNo1], gData.sMZID_LoadStage[nStageNo1], gData.sCarID_LoadStage[nStageNo1], gData.nLastCar_LoadStage[nStageNo1], gLot.nJobCycle);
			g_objLogFile.Save_HandlerLog(m_sLog);

			gData.sMZID_LoadStage[nStageNo1] = "";
			gData.sCarID_LoadStage[nStageNo1] = "";
			gData.nSlotNo_LoadStage[nStageNo1] = 0;
			gData.sLotID_LoadStage[nStageNo1] = "";
			gData.nTrayNo_LoadStage[nStageNo1] = 0;
			gData.nPortNo_LoadStage[nStageNo1] = 0;

			m_pDY04->oLoadStage1SlaveIn = FALSE; m_pDY04->oLoadStage1SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,10,LoadStage1,%d,Lot Cancel", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 26:
		if (!m_pDX04->iLoadStage1SlaveIn && m_pDX04->iLoadStage1SlaveOut) 
		{
			m_pDY04->oLoadStage1MasterIn = FALSE; m_pDY04->oLoadStage1MasterOut = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,10,LoadStage1,%d,Master Out", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 27:
		if (!m_pDX04->iLoadStage1MasterIn && m_pDX04->iLoadStage1MasterOut) 
		{
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,10,LoadStage1,%d,iLoadStage1MasterOut Check", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 28:
		if (!m_pDX04->iLoadStage1TrayExist)
		{
			gNG->nTrayOX[0][0] = 0;
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y1, 0); //load pos
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,10,LoadStage1,%d,AX_LOAD_STAGE_Y1 move to Load Pos", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 29:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y1, 0)) 
		{
			gData.nCarrierStart = 0;
			m_nLoadStage1Case = 0; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,10,LoadStage1,%d,AX_LOAD_STAGE_Y1 move to Load Pos Done", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 30:	// Load Picker Wait
		m_tLoadStage1Loop.Set_LoopTime(5000);
		return TRUE;
	case 31:
		if (m_nLoadStage2Case < 30 || m_nLoadStage2Case >= 50) 
		{
			m_tLoadStage1Loop.Takt_Save(10, 12); m_tLoadStage1Loop.Takt_Start();
			m_pDY04->oLoadStage1Up = FALSE; m_pDY04->oLoadStage1Down = TRUE;
			g_objAJinAXL.Write_Output(4);
			gData.sCarID_LoadStage[nStageNo1] = "";	gData.dAlignOffset[1][1] = 0.0;
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);

			m_sLog.Format("MCC,10,LoadStage1,%d,Load Stage Down", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;
	case 32:
		if (!m_pDX04->iLoadStage1Up && m_pDX04->iLoadStage1Down) 
		{
			gData.nCarrierStart = 0;
			m_tLoadStage1Loop.Takt_Save(10, 13); m_tLoadStage1Loop.Takt_Start();
			m_nLoadStage1Case = 40; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 40:	//Up Wait
		if (m_nLoadStage2Case < 40 || m_nLoadStage2Case >= 60)
		{
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;
	case 41:
		if (!m_pDX04->iLoadStage1Up && m_pDX04->iLoadStage1Down) {
			if (m_pDX04->iLoadStage2Up && !m_pDX04->iLoadStage2Down) 
			{
				g_objCommon.Move_Position(AX_LOAD_STAGE_Y1, 0);	//load
				m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

				m_sLog.Format("MCC,10,LoadStage1,%d,AX_LOAD_STAGE_Y1 move to Load Pos", m_nLoadStage1Case);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y1, 0)) 
		{
			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, 0);
			m_tLoadStage1Loop.Takt_Save(10, 14); m_tLoadStage1Loop.Takt_Start();
			m_nLoadStage1Case = 50; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 50:	//Align후
		if (m_nLoadStage2Case >= 30 && m_nLoadStage2Case < 50) 
		{
			m_tLoadStage1Loop.Takt_Save(10, 15); m_tLoadStage1Loop.Takt_Start();
			m_pDY04->oLoadStage1Up = TRUE; m_pDY04->oLoadStage1Down = FALSE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,Stage Up", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;
	case 51:
		if (m_pDX04->iLoadStage1Up && !m_pDX04->iLoadStage1Down) {
			m_tLoadStage1Loop.Takt_Save(10, 16); m_tLoadStage1Loop.Takt_Start();
			m_pDY04->oLoadStage1SlaveIn = FALSE; m_pDY04->oLoadStage1SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,10,LoadStage1,%d,Slave Out", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 52:
		if (!m_pDX04->iLoadStage1SlaveIn && m_pDX04->iLoadStage1SlaveOut) {
			m_tLoadStage1Loop.Takt_Save(10, 17); m_tLoadStage1Loop.Takt_Start();
			m_pDY04->oLoadStage1MasterIn = FALSE; m_pDY04->oLoadStage1MasterOut = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,10,LoadStage1,%d,Master Out", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 53:
		if (!m_pDX04->iLoadStage1MasterIn && m_pDX04->iLoadStage1MasterOut)
		{
			m_tLoadStage1Loop.Takt_Save(10, 18); m_tLoadStage1Loop.Takt_Start();
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);

			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 54:
		if (gNG->nTrayOX[0][0] == 1) 
		{
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d-1,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		} 
		else 
		{
			m_tLoadStage1Loop.Takt_Save(10, 19);
			m_nLoadStage1Case = 0; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d-2,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 55:
		if (m_pDX04->iLoadStage1TrayExist) 
		{
			m_nLoadStage1Case = 60; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 60:	// Transfer Picker Wait
		m_tLoadStage1Loop.Set_LoopTime(5000);
		return TRUE;

	case 61:
		if (!m_pDX04->iLoadStage1TrayExist) 
		{
			gNG->nTrayOX[0][0] = 0;
			m_tLoadStage1Loop.Takt_Save(10, 19);
			m_nLoadStage1Case = 0; m_tLoadStage1Loop.Set_LoopTime(30000);

			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;


	case 81:
			m_pDY04->oLoadStage1MasterIn = FALSE; m_pDY04->oLoadStage1MasterOut = TRUE;
			m_pDY04->oLoadStage1SlaveIn = FALSE;  m_pDY04->oLoadStage1SlaveOut = TRUE;
			m_pDY04->oLoadStage2MasterIn = FALSE; m_pDY04->oLoadStage2MasterOut = TRUE;
			m_pDY04->oLoadStage2SlaveIn = FALSE;  m_pDY04->oLoadStage2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);

			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		break;
	case 82:
		if (!m_pDX04->iLoadStage1MasterIn && m_pDX04->iLoadStage1MasterOut) {
			if (!m_pDX04->iLoadStage1SlaveIn && m_pDX04->iLoadStage1SlaveOut) {
				if (!m_pDX04->iLoadStage2MasterIn && m_pDX04->iLoadStage2MasterOut) {
					if (!m_pDX04->iLoadStage2SlaveIn && m_pDX04->iLoadStage2SlaveOut) {
						if (!m_tLoadStage1Loop.Waiting_Time(500)) break;
						m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
						m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
						g_objLogFile.Save_SeqLog(m_sLog);
					}
				}
			}
		}
		break;
	case 83:
		gData.nLoadStageSeqNo[3]++;
		if (gData.nLoadStageSeqNo[3] > 5 || m_nTransfer1Case > 0 || m_nLDMZElevatorCase > 0) {
			m_sLog.Format("[LoadStage] Align Seq(%d-%d-%d) Count(%d)", gData.nLoadStageSeqNo[0], m_nTransfer1Case, m_nLDMZElevatorCase, gData.nLoadStageSeqNo[3]);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_nLoadStage1Case = gData.nLoadStageSeqNo[0];
			m_tLoadStage1Loop.Set_LoopTime(60000);
			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		} else {
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 84:
			m_pDY04->oLoadStage1MasterIn = TRUE; m_pDY04->oLoadStage1MasterOut = FALSE;
			m_pDY04->oLoadStage1SlaveIn = FALSE; m_pDY04->oLoadStage1SlaveOut = TRUE;
			m_pDY04->oLoadStage2MasterIn = TRUE; m_pDY04->oLoadStage2MasterOut = FALSE;
			m_pDY04->oLoadStage2SlaveIn = FALSE; m_pDY04->oLoadStage2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		break;
	case 85:
		if (m_pDX04->iLoadStage1MasterIn && !m_pDX04->iLoadStage1MasterOut) {
			if (!m_pDX04->iLoadStage1SlaveIn && m_pDX04->iLoadStage1SlaveOut) {
				if (m_pDX04->iLoadStage2MasterIn && !m_pDX04->iLoadStage2MasterOut) {
					if (!m_pDX04->iLoadStage2SlaveIn && m_pDX04->iLoadStage2SlaveOut) {
						if (!m_tLoadStage1Loop.Waiting_Time(500)) break;
						m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
						m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
						g_objLogFile.Save_SeqLog(m_sLog);
					}
				}
			}
		}
		break;
	case 86:
			m_pDY04->oLoadStage1MasterIn = TRUE; m_pDY04->oLoadStage1MasterOut = FALSE;
			m_pDY04->oLoadStage1SlaveIn = TRUE;  m_pDY04->oLoadStage1SlaveOut = FALSE;
			m_pDY04->oLoadStage2MasterIn = TRUE; m_pDY04->oLoadStage2MasterOut = FALSE;
			m_pDY04->oLoadStage2SlaveIn = TRUE;  m_pDY04->oLoadStage2SlaveOut = FALSE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		break;
	case 87:
		if (m_pDX04->iLoadStage1MasterIn && !m_pDX04->iLoadStage1MasterOut) {
			if (m_pDX04->iLoadStage1SlaveIn && !m_pDX04->iLoadStage1SlaveOut) {
				if (m_pDX04->iLoadStage2MasterIn && !m_pDX04->iLoadStage2MasterOut) {
					if (m_pDX04->iLoadStage2SlaveIn && !m_pDX04->iLoadStage2SlaveOut) {
						if (!m_tLoadStage1Loop.Waiting_Time(500)) break;
						m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(5000);
						m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
						g_objLogFile.Save_SeqLog(m_sLog);
					}
				}
			}
		}
		break;
	case 88:
			m_pDY04->oLoadStage1MasterIn = TRUE; m_pDY04->oLoadStage1MasterOut = FALSE;
			m_pDY04->oLoadStage1SlaveIn = FALSE; m_pDY04->oLoadStage1SlaveOut = TRUE;
			m_pDY04->oLoadStage2MasterIn = TRUE; m_pDY04->oLoadStage2MasterOut = FALSE;
			m_pDY04->oLoadStage2SlaveIn = FALSE; m_pDY04->oLoadStage2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage1Case++; m_tLoadStage1Loop.Set_LoopTime(10000);
			m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		break;
	case 89:
		if (m_pDX04->iLoadStage1MasterIn && !m_pDX04->iLoadStage1MasterOut) {
			if (!m_pDX04->iLoadStage1SlaveIn && m_pDX04->iLoadStage1SlaveOut) {
				if (m_pDX04->iLoadStage2MasterIn && !m_pDX04->iLoadStage2MasterOut) {
					if (!m_pDX04->iLoadStage2SlaveIn && m_pDX04->iLoadStage2SlaveOut) {
						if (!m_tLoadStage1Loop.Waiting_Time(500)) break;
						m_nLoadStage1Case = 81; m_tLoadStage1Loop.Set_LoopTime(5000);
						m_sLog.Format("MCC,10,LoadStage1,%d,empty", m_nLoadStage1Case);
						g_objLogFile.Save_SeqLog(m_sLog);
					}
				}
			}
		}
		break;

	}

	// 10. (Error : 4000)
	if (m_tLoadStage1Loop.Over_LoopTime()) {
		if (m_nLoadStage1Case == 5) {
			m_nRFRetray[0][0]++;
			if (m_nRFRetray[0][0] < 3) { m_nLoadStage1Case = 4; return TRUE; }
		}
		if (m_nLoadStage1Case == 8) {
			nSNo1Vision++;
			if (nSNo1Vision <= 2) {
				g_objInspector.Set_RecipeRequest(1, gMes.sHostLotID, gMes.sHostRecipe);
				m_tLoadStage1Loop.Set_LoopTime(10000);	return TRUE;
			}
		}
		g_objCommon.Show_Error(4000 + m_nLoadStage1Case);
		return FALSE;
	}
	return TRUE;
}

// 11. (Error : 4500)
BOOL CSequenceMain::Run_LoadStage2()
{
	static int nStageNo2 = 1, nSNo2Vision, nPortNo2;	//1고정

	switch (m_nLoadStage2Case) 
	{
	case 0:	// Wait
		m_tLoadStage2Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (m_pDX04->iLoadStage2TrayExist) 
		{
			if (m_pDX04->iLoadStage2MasterIn && !m_pDX04->iLoadStage2MasterOut) 
			{
				m_sLog.Format("MCC,11,LoadStage2,%d,Slave In", m_nLoadStage2Case);
				g_objLogFile.Save_SeqLog(m_sLog);

				if (!m_tLoadStage2Loop.Waiting_Time(200)) break;
				gNG->nTrayOX[0][1] = 1;
				m_tLoadStage2Loop.Takt_Start();
				m_pDY04->oLoadStage2SlaveIn = TRUE; m_pDY04->oLoadStage2SlaveOut = FALSE;
				g_objAJinAXL.Write_Output(4);
				m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);

			}
		}
		break;
	case 2:
		if (m_pDX04->iLoadStage2MasterIn && !m_pDX04->iLoadStage2MasterOut) {
			if (m_pDX04->iLoadStage2SlaveIn && !m_pDX04->iLoadStage2SlaveOut) 
			{
				if (m_pDX04->iLoadStage2Up && !m_pDX04->iLoadStage2Down) 
				{
					m_sLog.Format("MCC,11,LoadStage2,%d,Up Check", m_nLoadStage2Case);
					g_objLogFile.Save_SeqLog(m_sLog);

					m_tLoadStage2Loop.Takt_Save(11, 1); m_tLoadStage2Loop.Takt_Start();
					m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);					
				}
			}
		}
		break;
	case 3:	//RFID Wait
		if (m_nLoadStage1Case >= 30)
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,AX_LOAD_STAGE_Y2 move to RFID pos", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_tLoadStage2Loop.Takt_Save(11, 2); m_tLoadStage2Loop.Takt_Start();
			m_nRFRetray[0][1] = 0;
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y2, 6);	//RFID
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);			
		}
		return TRUE;

	case 4:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y2, 6)) {
			if (m_pEquipData->bUseRFIDLoad) 
			{
				m_sLog.Format("MCC,11,LoadStage2,%d,g_objCarrierRFID_Load.Send_RFIDRead()", m_nLoadStage2Case);
				g_objLogFile.Save_SeqLog(m_sLog);

				g_objCarrierRFID_Load.Send_RFIDRead();
				m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);				
			} 
			else 
			{
				if (Search_Lot()) 
				{ 
					m_sLog.Format("MCC,11,LoadStage2,%d,Search_Lot", m_nLoadStage2Case);
					g_objLogFile.Save_SeqLog(m_sLog);

					m_nLoadStage2Case = 10; m_tLoadStage2Loop.Set_LoopTime(5000);					
				}
			}
		}
		break;
	case 5:
		if (g_objCarrierRFID_Load.Is_RecvComplete()) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,g_objMesAgent.Set_CarrierOutMGZ", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			gData.sCarID_LoadStage[nStageNo2] = g_objCarrierRFID_Load.Get_CarrierID();
			if (gData.sCarID_LoadStage[nStageNo2].GetLength() > 2) 
			{
				g_objMesAgent.Set_CarrierOutMGZ(gData.sMZID_LoadStage[nStageNo2], gData.sCarID_LoadStage[nStageNo2], gData.nSlotNo_LoadStage[nStageNo2]);
				m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);			
			}
		}
		break;
	case 6:
		if (m_pEquipData->bUseMES) 
		{
			if (gData.sCarID_LoadStage[nStageNo2].GetLength() > 2) 
			{
				m_sLog.Format("MCC,11,LoadStage2,%d,g_objMesAgent.Set_CarrierIDReport", m_nLoadStage2Case);
				g_objLogFile.Save_SeqLog(m_sLog);

				gMes.nCarConfirm[LOAD_STAGE] = 1;
				g_objMesAgent.Set_CarrierIDReport("L", gData.sMZID_LoadStage[nStageNo2], gData.sCarID_LoadStage[nStageNo2]);
				m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);				
			}
		} 
		else 
		{
			if (Search_Lot()) 
			{	
				m_sLog.Format("MCC,11,LoadStage2,%d,g_objLogFile.Save_RFBarData", m_nLoadStage2Case);
				g_objLogFile.Save_SeqLog(m_sLog);

				g_objLogFile.Save_RFBarData(4, gData.sCarID_LoadStage[nStageNo2], gLot.nCmCount[gLot.nJobCycle-1]);
				m_nLoadStage2Case = 10; m_tLoadStage2Loop.Set_LoopTime(5000);			
			}
		}
		break;
	case 7:
		if (gMes.nCarConfirm[LOAD_STAGE] > 1) 
		{
			if (gMes.nCarConfirm[LOAD_STAGE] == 2) 
			{
				m_sLog.Format("MCC,11,LoadStage2,%d,g_objInspector.Set_RecipeRequest", m_nLoadStage2Case);
				g_objLogFile.Save_SeqLog(m_sLog);

				gMes.bRcpBodyDone = FALSE;	nSNo2Vision = 0;
				g_objInspector.Set_RecipeRequest(0, gMes.sHostLotID, gMes.sHostRecipe);
				m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
			
			} else {
				m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
				g_objLogFile.Save_SeqLog(m_sLog);

				m_nLoadStage2Case = 9; m_tLoadStage2Loop.Set_LoopTime(10000);
				
			}
		}
		break;
	case 8:	
		if (gMes.bRcpBodyDone) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,g_objMesAgent.Set_PPSelectReport", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objMesAgent.Set_PPSelectReport(gMes.sHostLotID, gMes.sRcpVersion);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(60000);				
		}
		break;
	case 9:
		if (gMes.nCarConfirm[LOAD_STAGE] >= 4) 
		{
			if (!m_tLoadStage2Loop.Waiting_Time(500)) break;


			m_sLog.Format("MCC,11,LoadStage2,%d,g_objMesAgent.Set_LotStart", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			nPortNo2 = gMes.nLotPortNo - 1;
			g_objMesAgent.Set_LotStart(gLot.sLotID[nPortNo2], gData.sMZID_LoadStage[nStageNo2], gData.nSlotNo_LoadStage[nStageNo2], gData.sCarID_LoadStage[nStageNo2], gLot.sRecipeName[nPortNo2]);
			g_objLogFile.Save_RFBarData(4, gData.sCarID_LoadStage[nStageNo2], gLot.nCmCount[gLot.nJobCycle-1]);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);

		}
		break;

	case 10:	//Inspection Start
		if (gLot.nJobCycle > 0) {
			nPortNo2 = gLot.nStartPno = gLot.nJobCycle - 1;
			if (m_pEquipData->bUseMES) {
				gLot.sRecipeName[nPortNo2] = gMes.sHostRecipe;
				if (gLot.sLotID[nPortNo2].GetLength() < 5) {
					gLot.sLotID[nPortNo2] = gMes.sHostLotID;
					gLot.nCmCount[nPortNo2] = gMes.nHostCmCount;
				}
			} else	gLot.sRecipeName[nPortNo2] = gData.sRecipeName;
			gData.sReadyRecipe = gLot.sRecipeName[nPortNo2];
			gLot.nJobCount = gLot.nJobCount + gLot.nCmCount[nPortNo2];
			g_objInspector.Set_LotReady(FALSE);
			g_objInspector.Set_LotStart(INSPECTOR_ALL, gLot.sLotID[nPortNo2], nPortNo2+1, 1, gLot.nCmCount[nPortNo2], gLot.sRecipeName[nPortNo2], m_pEquipData->sPROG_VER, m_pEquipData->sPARA_VER, m_pEquipData->bUseCosmeticNG);

			gData.nLoadAlignRetry = 0;
			gLot.sMZID_LD[nPortNo2] = gData.sMZID_LoadStage[nStageNo2];
			gLot.sCarID_LD[nPortNo2] = gData.sCarID_LoadStage[nStageNo2];
			gLot.nSlotNo_LD[nPortNo2] = gData.nSlotNo_LoadStage[nStageNo2];
			gLot.nMZLastCar[nPortNo2] = gData.nLastCar_LoadStage[nStageNo2];

			gData.sLotID_LoadStage[nStageNo2] = gLot.sLotID[nPortNo2];
			gData.nTrayNo_LoadStage[nStageNo2] = 1;
			gData.nPortNo_LoadStage[nStageNo2] = gLot.nJobCycle;
			g_dlgWork.PostMessage(UM_UPDATE_MODEL, 1, NULL);

			Set_LotStart(gLot.sLotID[nPortNo2],  gLot.nJobCycle);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(60000);

			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 11:
		if (g_objInspector.Check_LotReady()) {
			if (m_pDX04->iLoadStage2Up && !m_pDX04->iLoadStage2Down) {
				if (m_pEquipData->bUseMES) {
					if (gData.sLotID_LoadStage[nStageNo2].GetLength() > 2 && gLot.sLotID[gMes.nLotPortNo-1].GetLength() < 2) {
						gLot.nJobCycle = gMes.nLotPortNo;
						gLot.sLotID[gLot.nJobCycle-1] = gMes.sHostLotID;
						gLot.nCmCount[gLot.nJobCycle-1] = gMes.nHostCmCount;
						gLot.sRecipeName[gLot.nJobCycle-1] = gMes.sHostRecipe;
						g_dlgWork.PostMessage(UM_UPDATE_MODEL, 1, NULL);
					}
				}
				m_sLog.Format("MCC,11,LoadStage2,%d,g_objInspector.Check_LotReady AX_LOAD_STAGE_Y2 move to Align 3 Pos", m_nLoadStage2Case);
				g_objLogFile.Save_SeqLog(m_sLog);

				m_tLoadStage2Loop.Takt_Save(11, 3); m_tLoadStage2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_LOAD_STAGE_Y2, 3);	//Aling1
				m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
			}
		}
		break;

	case 12:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y2, 3)) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,Save_Motion", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, 3);
			m_tLoadStage2Loop.Takt_Save(11, 4); m_tLoadStage2Loop.Takt_Start();
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		break;

	case 13:
		if (!m_pEquipData->bUseAlign1) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,AX_LOAD_STAGE_Y2 move to Align 3", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			if (gNG->nTrayOX[0][1] == 1) Set_AlignData(gData.nPortNo_LoadStage[nStageNo2], nStageNo2);
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y2, 5);	//Aling3
			m_nLoadStage2Case = 19; m_tLoadStage2Loop.Set_LoopTime(30000);

			m_sLog.Format("[LoadStage2: Skip] AlingUse(%d) Exist(%d)", m_pEquipData->bUseAlign1, m_pDX04->iLoadStage2TrayExist);
			g_objLogFile.Save_HandlerLog(m_sLog);
		}
		else
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,g_objInspector.Set_AlignRequest", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			Init_AlignTray();
			gData.bAlignScanDone = FALSE;
			g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A1", gData.sLotID_LoadStage[nStageNo2], gData.nPortNo_LoadStage[nStageNo2], gData.nTrayNo_LoadStage[nStageNo2], 1);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 14:
		if (gData.bAlignScanDone) 
		{
			if (!m_tLoadStage2Loop.Waiting_Time(m_pEquipData->nDelayTime[5])) break;

			m_sLog.Format("MCC,11,LoadStage2,%d,AX_LOAD_STAGE_Y2 move to Align 2", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_tLoadStage2Loop.Takt_Save(11, 5); m_tLoadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y2, 4);	//Aling2
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 15:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y2, 4)) 
		{
			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, 4);
			m_tLoadStage2Loop.Takt_Save(11, 6); m_tLoadStage2Loop.Takt_Start();
			gData.bAlignScanDone = FALSE;

			m_sLog.Format("MCC,11,LoadStage2,%d,g_objInspector.Set_AlignRequest", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A1", gData.sLotID_LoadStage[nStageNo2], gData.nPortNo_LoadStage[nStageNo2], gData.nTrayNo_LoadStage[nStageNo2], 2);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (gData.bAlignScanDone) 
		{
			if (!m_tLoadStage2Loop.Waiting_Time(m_pEquipData->nDelayTime[5])) break;
			m_sLog.Format("MCC,11,LoadStage2,%d,AX_LOAD_STAGE_Y2 move to Align 3 Pos", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_tLoadStage2Loop.Takt_Save(11, 7); m_tLoadStage2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y2, 5);	//Aling3
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 17:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y2, 5)) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,g_objInspector.Set_AlignRequest 3", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, 5);
			m_tLoadStage2Loop.Takt_Save(11, 8); m_tLoadStage2Loop.Takt_Start();
			gData.bAlignScanDone = gData.bAlignA1Done = FALSE;
			g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A1", gData.sLotID_LoadStage[nStageNo2], gData.nPortNo_LoadStage[nStageNo2], gData.nTrayNo_LoadStage[nStageNo2], 3);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 18:
		if (gData.bAlignA1Done) 
		{
			if (!m_tLoadStage2Loop.Waiting_Time(m_pEquipData->nDelayTime[5])) break;
			if (m_pEquipData->bUseAlignOffset) {
				if (m_pEquipData->dAlignOffset < fabs(gData.dAlignOffset[0][1])) {
					gAlm.sAlmLID[0].Format("%0.3lf", m_pEquipData->dAlignOffset);
					gAlm.sAlmLID[1].Format("%0.3lf", fabs(gData.dAlignOffset[0][1]));
					g_objCommon.Show_Error(4618);
					return FALSE;
				}
			}
			m_sLog.Format("MCC,11,LoadStage2,%d,bAlignA1Done", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_tLoadStage2Loop.Takt_Save(11, 9); m_tLoadStage2Loop.Takt_Start();
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 19:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y2, 5)) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,Save_Motion", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);
			if (gNG->nTrayOX[0][1] == 1) {
				if (Check_AlignData(gData.nPortNo_LoadStage[nStageNo2]))
				{
					g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, 5);
					m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
				}
			} else {
				g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, 5);
				m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 20:	//Align Wait
		if (m_nLoadStage1Case > 20 && m_nLoadStage1Case < 40) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,AX_LOAD_STAGE_Y2 move to Wait Pos", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_LOAD_STAGE_Y2, 1);	//Wait
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
		} 
		else if (m_nLoadStage1Case < 20 || (m_nLoadStage1Case >= 25 && m_nLoadStage1Case <= 29) || m_nLoadStage1Case >= 40) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,AX_LOAD_STAGE_Y2 move to Unload Pos", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_LOAD_STAGE_Y2, 2);	//Unload
			m_nLoadStage2Case = 23; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 21:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y2, 1)) 
		{
			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, 1);
			m_tLoadStage2Loop.Takt_Save(11, 10); m_tLoadStage2Loop.Takt_Start();
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 22:	//Align Wait
		if (m_nLoadStage1Case < 20 || m_nLoadStage1Case >= 40) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,AX_LOAD_STAGE_Y2 move to Unload", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_LOAD_STAGE_Y2, 2);	//Unload
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 23:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y2, 2)) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,Save_Motion", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, 2);
			m_tLoadStage2Loop.Takt_Save(11, 11); m_tLoadStage2Loop.Takt_Start();
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 24:
		if (gNG->nTrayOX[0][1] == 0 || (gNG->nTrayOX[0][1] == 1 && m_pDX04->iLoadStage2TrayExist)) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,Set_AlignToStage", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			Set_AlignToStage(gData.nPortNo_LoadStage[nStageNo2], nStageNo2);
			m_nLoadStage2Case = 30; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 25:	//Lot취소
		if (m_pDX04->iLoadStage2Up && !m_pDX04->iLoadStage2Down) 
		{
			if (gData.nLastCar_LoadStage[nStageNo2] == 1) {
				int nPNo = gLot.nJobCycle - 1; if (nPNo < 0) nPNo = 27;
				gLot.nMZLastCar[nPNo] = gData.nLastCar_LoadStage[nStageNo2];
			}
			m_sLog.Format("[LoadStage2: Lot Cancel] LotID(%s) MZ(%s) Car(%s) Last(%d) Cycle(%d)", gData.sLotID_LoadStage[nStageNo2], gData.sMZID_LoadStage[nStageNo2], gData.sCarID_LoadStage[nStageNo2], gData.nLastCar_LoadStage[nStageNo2], gLot.nJobCycle);
			g_objLogFile.Save_HandlerLog(m_sLog);

			gData.sMZID_LoadStage[nStageNo2] = "";
			gData.sCarID_LoadStage[nStageNo2] = "";
			gData.nSlotNo_LoadStage[nStageNo2] = 0;
			gData.sLotID_LoadStage[nStageNo2] = "";
			gData.nTrayNo_LoadStage[nStageNo2] = 0;
			gData.nPortNo_LoadStage[nStageNo2] = 0;

			m_sLog.Format("MCC,11,LoadStage2,%d,Lot Cancel", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY04->oLoadStage2SlaveIn = FALSE; m_pDY04->oLoadStage2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 26:
		if (!m_pDX04->iLoadStage2SlaveIn && m_pDX04->iLoadStage2SlaveOut) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY04->oLoadStage2MasterIn = FALSE; m_pDY04->oLoadStage2MasterOut = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 27:
		if (!m_pDX04->iLoadStage2MasterIn && m_pDX04->iLoadStage2MasterOut) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 28:
		if (!m_pDX04->iLoadStage2TrayExist) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,AX_LOAD_STAGE_Y2 move to Load", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			gNG->nTrayOX[0][1] = 0;
			g_objCommon.Move_Position(AX_LOAD_STAGE_Y2, 0);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 29:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y2, 0))
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			gData.nCarrierStart = 0;
			m_nLoadStage2Case = 0; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 30:	// Load Picker Wait
		m_tLoadStage2Loop.Set_LoopTime(5000);
		return TRUE;
	case 31:
		if (m_nLoadStage1Case < 30 || m_nLoadStage1Case >= 50) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,Stage Down", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_tLoadStage2Loop.Takt_Save(11, 12); m_tLoadStage2Loop.Takt_Start();
			m_pDY04->oLoadStage2Up = FALSE; m_pDY04->oLoadStage2Down = TRUE;
			g_objAJinAXL.Write_Output(4);
			gData.sCarID_LoadStage[nStageNo2] = "";	gData.dAlignOffset[1][1] = 0.0;
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 32:
		if (!m_pDX04->iLoadStage2Up && m_pDX04->iLoadStage2Down) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			gData.nCarrierStart = 0;
			m_tLoadStage2Loop.Takt_Save(11, 13); m_tLoadStage2Loop.Takt_Start();
			m_nLoadStage2Case = 40; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		break;

	case 40:	//Up Wait
		if (m_nLoadStage1Case < 40 || m_nLoadStage1Case >= 60) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 41:
		if (!m_pDX04->iLoadStage2Up && m_pDX04->iLoadStage2Down) 
		{
			if (m_pDX04->iLoadStage1Up && !m_pDX04->iLoadStage1Down) 
			{
				m_sLog.Format("MCC,11,LoadStage2,%d,AX_LOAD_STAGE_Y2 move to Load", m_nLoadStage2Case);
				g_objLogFile.Save_SeqLog(m_sLog);

				g_objCommon.Move_Position(AX_LOAD_STAGE_Y2, 0);	//load
				m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Y2, 0)) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,Save_Motion", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, 0);
			m_tLoadStage2Loop.Takt_Save(11, 14); m_tLoadStage2Loop.Takt_Start();
			m_nLoadStage2Case = 50; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 50:	//Align후
		if (m_nLoadStage1Case >= 30 && m_nLoadStage1Case < 50) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,Stage Up", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_tLoadStage2Loop.Takt_Save(11, 15); m_tLoadStage2Loop.Takt_Start();
			m_pDY04->oLoadStage2Up = TRUE; m_pDY04->oLoadStage2Down = FALSE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 51:
		if (m_pDX04->iLoadStage2Up && !m_pDX04->iLoadStage2Down) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_tLoadStage2Loop.Takt_Save(11, 16); m_tLoadStage2Loop.Takt_Start();
			m_pDY04->oLoadStage2SlaveIn = FALSE; m_pDY04->oLoadStage2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 52:
		if (!m_pDX04->iLoadStage2SlaveIn && m_pDX04->iLoadStage2SlaveOut) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_tLoadStage2Loop.Takt_Save(11, 17); m_tLoadStage2Loop.Takt_Start();
			m_pDY04->oLoadStage2MasterIn = FALSE; m_pDY04->oLoadStage2MasterOut = TRUE;
			g_objAJinAXL.Write_Output(4);
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 53:
		if (!m_pDX04->iLoadStage2MasterIn && m_pDX04->iLoadStage2MasterOut)
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_tLoadStage2Loop.Takt_Save(11, 18); m_tLoadStage2Loop.Takt_Start();
			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 54:
		if (gNG->nTrayOX[0][1] == 1) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nLoadStage2Case++; m_tLoadStage2Loop.Set_LoopTime(30000);
		} 
		else
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_tLoadStage2Loop.Takt_Save(11, 19);
			m_nLoadStage2Case = 0; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 55:
		if (m_pDX04->iLoadStage2TrayExist) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nLoadStage2Case = 60; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		break;

	case 60:	// Transfer Picker Wait
		m_tLoadStage2Loop.Set_LoopTime(5000);
		return TRUE;

	case 61:
		if (!m_pDX04->iLoadStage2TrayExist) 
		{
			m_sLog.Format("MCC,11,LoadStage2,%d,empty", m_nLoadStage2Case);
			g_objLogFile.Save_SeqLog(m_sLog);
			gNG->nTrayOX[0][1] = 0;
			m_tLoadStage2Loop.Takt_Save(11, 19);
			m_nLoadStage2Case = 0; m_tLoadStage2Loop.Set_LoopTime(30000);
		}
		break;

	}

	// 11. (Error : 4500)
	if (m_tLoadStage2Loop.Over_LoopTime()) {
		if (m_nLoadStage2Case == 5) {
			m_nRFRetray[0][1]++;
			if (m_nRFRetray[0][1] < 3) { m_nLoadStage2Case = 4; return TRUE; }
		}
		if (m_nLoadStage2Case == 8) {
			nSNo2Vision++;
			if (nSNo2Vision <= 2) {
				g_objInspector.Set_RecipeRequest(1, gMes.sHostLotID, gMes.sHostRecipe);
				m_tLoadStage2Loop.Set_LoopTime(10000);	return TRUE;
			}
		}
		g_objCommon.Show_Error(4500 + m_nLoadStage2Case);
		return FALSE;
	}
	return TRUE;
}

// 12. (Error : 5000)
BOOL CSequenceMain::Run_LoadPicker1()
{
	static int n1No = 1;			//1(고정)
	static int nLPStage1No = 0;	//1,2
	static int nLoad1PosX = 0, nLoad1PosY = 0, nStartY1 = 0;
	static int n1BTMNo, n1CMno1, n1CMno5, n1LPVisionNo, n1LP, n1LT, n1LM, n1Type, n1S, n1E, nL1Retry;
	static double dLPStage1PosY = 0, dLPStage1PosX = 0, dLP1PosY = 0;

	switch (m_nLoadPicker1Case) 
	{
	case 0:	// Wait
		if (Check_EmptyLoadTray()) 
		{
			// 30은 Load Picker 작업을 기다리는 스텝, Tray가 없으면 그냥 넘기기
			if (m_nLoadStage1Case == 30 && !m_pDX04->iLoadStage1TrayExist) m_nLoadStage1Case = 31;
			if (m_nLoadStage2Case == 30 && !m_pDX04->iLoadStage2TrayExist) m_nLoadStage2Case = 31;
		}
		if (m_nLoadStage1Case == 31 || m_nLoadStage2Case == 31) 
		{
			m_sLog.Format("[LoadPicker1: Start1] Empty - Seq(%d-%d)", m_nLoadStage1Case, m_nLoadStage2Case);
			g_objLogFile.Save_HandlerLog(m_sLog);
		}
		if (m_nLoadStage1Case == 30 || m_nLoadStage2Case == 30) {
			if (m_nLoadStage1Case == 30) nLPStage1No = 1;
			if (m_nLoadStage2Case == 30) nLPStage1No = 2;
			g_objCommon.Set_LoadPickerUp(n1No);	gData.nLPCount1 = 0;
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, 0);

			m_sLog.Format("MCC,12,LoadPicker1,%d,AX_LOAD_PICKER_Z1 move to Ready up", m_nLoadPicker1Case);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(5000);

			m_sLog.Format("[LoadPicker1: Start] StageNo(%d) LoadAlignOffset(%d/%0.3lf)", nLPStage1No, m_pEquipData->bUseAlignOffset, gData.dAlignOffset[1][1]);
			g_objLogFile.Save_HandlerLog(m_sLog);
		}
		return TRUE;

	case 1:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z1, 0)) 
		{
			if (g_objCommon.Get_LoadPickerUp(n1No)) 
			{
				g_objCommon.Move_Position(AX_LOAD_PICKER_X1, nLPStage1No);
				g_objCommon.Move_Position(AX_LOAD_PICKER_Y1, nLPStage1No);
//				if(m_pEquipData->bUseAlignOffset) dLP1PosY = m_pMoveData->dLoadPickerY1[nLPStage1No] + (gData.dAlignOffset[1][1] * -1.0);
//				else							  dLP1PosY = m_pMoveData->dLoadPickerY1[nLPStage1No];
//				g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_Y1, dLP1PosY);
				m_sLog.Format("MCC,12,LoadPicker1,%d,AX_LOAD_PICKER_X1 and Y1 move to Stage No:%d", m_nLoadPicker1Case, nLPStage1No);
				g_objLogFile.Save_SeqLog(m_sLog);

				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 2:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_X1, nLPStage1No) && g_objCommon.Check_Position(AX_LOAD_PICKER_Y1, nLPStage1No)) 
		{
//		if (g_objCommon.Check_Position(AX_LOAD_PICKER_X1, nLPStage1No) && g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_Y1, dLP1PosY)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_X1, nLPStage1No);	g_objCommon.Save_Motion(AX_LOAD_PICKER_Y1, nLPStage1No);
			g_objCommon.Set_LoadPickerOpen(n1No);
			if (Check_LoadModule(nLoad1PosX, nLoad1PosY)) 
			{
				if (nLoad1PosY == 1 || nLoad1PosY == 3 || nLoad1PosY == 5 || nLoad1PosY == 7 || nLoad1PosY == 9) 
				{
					m_sLog.Format("MCC,12,LoadPicker1,%d,Check_LoadModule", m_nLoadPicker1Case);
					g_objLogFile.Save_SeqLog(m_sLog);

					gData.InfoLoadPick[n1No-1][9] = ((nLoad1PosY-1) * 4) + 1;	nStartY1 = nLoad1PosY;
					m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(5000);
				} 
				else 
				{
					m_sLog.Format("MCC,12,LoadPicker1,%d,Check_LoadModule", m_nLoadPicker1Case);
					g_objLogFile.Save_SeqLog(m_sLog);

					gData.InfoLoadPick[n1No-1][9] = ((nLoad1PosY-2) * 4) + 1;	nLoad1PosX = nStartY1 = 0;
					m_nLoadPicker1Case = 10; m_tLoadPicker1Loop.Set_LoopTime(5000);
				}
				m_tLoadPicker1Loop.Takt_Start();
			} 
			else 
			{
				gData.sLotID_LoadStage[nLPStage1No-1]  = "";
				gData.nTrayNo_LoadStage[nLPStage1No-1] = gData.nPortNo_LoadStage[nLPStage1No-1] = 0;

				if (m_nLoadStage1Case == 30) m_nLoadStage1Case = 31;
				if (m_nLoadStage2Case == 30) m_nLoadStage2Case = 31;
				if (m_nLoadStage1Case == 31 || m_nLoadStage2Case == 31) 
				{
					m_sLog.Format("[LoadPicker1: Start2] Empty - Seq(%d-%d)", m_nLoadStage1Case, m_nLoadStage2Case);
					g_objLogFile.Save_HandlerLog(m_sLog);
				}
				m_nLoadPicker1Case = 0; m_tLoadPicker1Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 3:
		if (nLoad1PosY == 1 || nLoad1PosY == 3 || nLoad1PosY == 5 || nLoad1PosY == 7 || nLoad1PosY == 9) 
		{
			double dOffset1 = (nLoad1PosY <= 5 ? 0.0 : CARRIER_OFFSETY);
			double dOffset2 = (!m_pEquipData->bUseAlignOffset ? 0.0 : gData.dAlignOffset[1][1]);
			if (nLPStage1No == 1)
			{
				dLPStage1PosY = m_pMoveData->dLoadStageY1[2] + (m_pEquipData->dTrayPitchY * (nLoad1PosY - 1)) + dOffset1 + dOffset2;
				g_objAJinAXL.Move_Absolute(AX_LOAD_STAGE_Y1, dLPStage1PosY);
			}
			if (nLPStage1No == 2)
			{
				dLPStage1PosY = m_pMoveData->dLoadStageY2[2] + (m_pEquipData->dTrayPitchY * (nLoad1PosY - 1)) + dOffset1 + dOffset2;
				g_objAJinAXL.Move_Absolute(AX_LOAD_STAGE_Y2, dLPStage1PosY);
			}
			n1Type = (nLoad1PosX == 2 ? 2 : 1);
			if (n1Type == 2) 
			{
				g_objAJinAXL.Move_Relative(AX_LOAD_PICKER_X1, CARRIER_OFFSETX*-1.0);
			}
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		} 
		else
		{
			m_nLoadPicker1Case = 10; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objAJinAXL.Is_Done(AX_LOAD_PICKER_X1)) 
		{
			if ((nLPStage1No == 1 && g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE_Y1, dLPStage1PosY)) ||
				(nLPStage1No == 2 && g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE_Y2, dLPStage1PosY)) ) 
			{
				if (nLPStage1No == 1) g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, -1, dLPStage1PosY);
				if (nLPStage1No == 2) g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, -1, dLPStage1PosY);
				m_tLoadPicker1Loop.Takt_Save(12, 1); m_tLoadPicker1Loop.Takt_Start();
				g_objCommon.Set_LoadPickerDown(n1No, n1Type);
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 5:
		if (g_objCommon.Get_LoadPickerDown(n1No, n1Type))
		{
			m_tLoadPicker1Loop.Takt_Save(12, 2); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, nLPStage1No);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 6:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z1, nLPStage1No)) 
		{
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z1, nLPStage1No);
			m_tLoadPicker1Loop.Takt_Save(12, 3); m_tLoadPicker1Loop.Takt_Start();
			gData.sLotID_LoadPicker[n1No-1]  = gData.sLotID_LoadStage[nLPStage1No-1];
			gData.nTrayNo_LoadPicker[n1No-1] = gData.nTrayNo_LoadStage[nLPStage1No-1];
			gData.nPortNo_LoadPicker[n1No-1] = gData.nPortNo_LoadStage[nLPStage1No-1];
			gData.sLotID_LoadPicker[2]		 = gData.sLotID_LoadPicker[n1No-1];

			int nS, nE;
			if (n1Type == 1) { nS = 0; nE = 2; }
			else			 { nS = 2; nE = 4; }
			for(int i=nS; i<nE; i++) 
			{
				gData.InfoLoadPick[n1No-1][i] = gData.InfoLoadTray[nLoad1PosY-1][i];	gData.InfoLoadTray[nLoad1PosY-1][i] = 0;
				
				gTracking.sMZID_LoadPicker[n1No-1][i] = gTracking.sMZID_LoadStageModule[nLPStage1No-1][nLoad1PosY-1][i];
				gTracking.sMZID_LoadStageModule[nLPStage1No-1][nLoad1PosY-1][i] = "";
				m_sLog.Format("LoadStage To LoadPicker No:%d, x:%d y:%d - MZ ID:%s",n1No, i, nLoad1PosY-1, gTracking.sMZID_LoadPicker[n1No-1][i]);
				if(gTracking.sMZID_LoadPicker[n1No-1][i] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);
			}

			n1LP = gData.nPortNo_LoadPicker[n1No-1] - 1;
			n1LT = gData.nTrayNo_LoadPicker[n1No-1] - 1;
			n1LM = gData.InfoLoadPick[n1No-1][9] - 1;
			for(int i=nS; i<nE; i++) {
				if (gData.InfoLoadPick[n1No-1][i] > 0) gLot.nTrayPutCnt[n1LP][2]++;
				gLot.nHistory[n1LP][n1LT][n1LM+i][0] = nLPStage1No;
				gLot.nHistory[n1LP][n1LT][n1LM+i][1] = 1;
				gLot.nHistory[n1LP][n1LT][n1LM+i][2] = i + 1;
			}

			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 7:
		if (g_objCommon.Get_LoadPickerDown(n1No, n1Type)) {
			g_objCommon.Set_LoadPickerClose(n1No, n1Type);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 8:
		if (g_objCommon.Get_LoadPickerClose(n1No, n1Type)) {
			if (!m_tLoadPicker1Loop.Waiting_Time(m_pEquipData->nDelayTime[1])) break;
			m_tLoadPicker1Loop.Takt_Save(12, 4); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Set_LoadPickerUp(n1No);
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, 0);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 9:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z1, 0)) {
			if (g_objCommon.Get_LoadPickerUp(n1No)) {
				g_objCommon.Save_Motion(AX_LOAD_PICKER_Z1, 0);
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(m_pEquipData->nDelayTime[10]);
			}
		}
		break;
	case 10:
		if (g_objCommon.Get_LoadPickerCheck(n1No, 9, 0, gData.InfoLoadPick[n1No-1])) {
			if (nLoad1PosX == 3) {
				nLoad1PosX = 2;
				m_nLoadPicker1Case = 3; m_tLoadPicker1Loop.Set_LoopTime(30000);
			} else {
				if (Check_LoadModule(nLoad1PosX, nLoad1PosY)) 
				{
					if (nLoad1PosY == 1 || nLoad1PosY == 3 || nLoad1PosY == 5 || nLoad1PosY == 7 || nLoad1PosY == 9 ||
						(nStartY1 > 0 && (nLoad1PosY > nStartY1+1))) 
					{
						m_nLoadPicker1Case = 18; m_tLoadPicker1Loop.Set_LoopTime(30000);
					}
					else
					{
						m_tLoadPicker1Loop.Takt_Save(12, 5); m_tLoadPicker1Loop.Takt_Start();
						double dOffset1 = (nLoad1PosY <= 5 ? 0.0 : CARRIER_OFFSETY);
						double dOffset2 = (!m_pEquipData->bUseAlignOffset ? 0.0 : gData.dAlignOffset[1][1]);
						if (nLPStage1No == 1) {
							dLPStage1PosY = m_pMoveData->dLoadStageY1[2] + (m_pEquipData->dTrayPitchY * (nLoad1PosY - 1)) + dOffset1 + dOffset2;
							g_objAJinAXL.Move_Absolute(AX_LOAD_STAGE_Y1, dLPStage1PosY);
						}
						if (nLPStage1No == 2) {
							dLPStage1PosY = m_pMoveData->dLoadStageY2[2] + (m_pEquipData->dTrayPitchY * (nLoad1PosY - 1)) + dOffset1 + dOffset2;
							g_objAJinAXL.Move_Absolute(AX_LOAD_STAGE_Y2, dLPStage1PosY);
						}
						m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
					}
				} else {
					m_nLoadPicker1Case = 18; m_tLoadPicker1Loop.Set_LoopTime(30000);
				}
			}
		}
		break;

	case 11:
		if ((m_nLoadPicker2Case > 41 && m_nLoadPicker2Case < 80) ||
			(m_nLoadPicker2Case > 33 && m_nLoadPicker2Case < 80 && gData.nLPCount2 >= 2) ) {
			m_tLoadPicker1Loop.Takt_Save(12, 6); m_tLoadPicker1Loop.Takt_Start();
			n1Type = (nLoad1PosX == 2 ? 4 : 3);
			if (n1Type == 4) dLPStage1PosX = m_pMoveData->dLoadPickerX1[nLPStage1No] + (m_pEquipData->dTrayPitchX * 4) - CARRIER_OFFSETX;
			else			 dLPStage1PosX = m_pMoveData->dLoadPickerX1[nLPStage1No] + (m_pEquipData->dTrayPitchX * 4);
			g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_X1, dLPStage1PosX);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		return TRUE;

	case 12:
		if ((nLPStage1No == 1 && g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE_Y1, dLPStage1PosY)) ||
			(nLPStage1No == 2 && g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE_Y2, dLPStage1PosY)) ) {
			if (g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_X1, dLPStage1PosX)) {
				if (nLPStage1No == 1) g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, -1, dLPStage1PosY);
				if (nLPStage1No == 2) g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, -1, dLPStage1PosY);
				g_objCommon.Save_Motion(AX_LOAD_PICKER_X1, -1, dLPStage1PosX);
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 13:
		if (g_objCommon.Get_LoadPickerOpen(n1No, n1Type)) {
			g_objCommon.Set_LoadPickerDown(n1No, n1Type);
			m_tLoadPicker1Loop.Takt_Save(12, 7); m_tLoadPicker1Loop.Takt_Start();

			gData.sLotID_LoadPicker[n1No-1]  = gData.sLotID_LoadStage[nLPStage1No-1];
			gData.nTrayNo_LoadPicker[n1No-1] = gData.nTrayNo_LoadStage[nLPStage1No-1];
			gData.nPortNo_LoadPicker[n1No-1] = gData.nPortNo_LoadStage[nLPStage1No-1];
			gData.sLotID_LoadPicker[2]		 = gData.sLotID_LoadPicker[n1No-1];

			int nS, nE;
			if (n1Type == 3) { nS = 0; nE = 2; }
			else			 { nS = 2; nE = 4; }

			for(int i=nS; i<nE; i++) {
				gData.InfoLoadPick[n1No-1][i+4] = gData.InfoLoadTray[nLoad1PosY-1][i]; gData.InfoLoadTray[nLoad1PosY-1][i] = 0;

				gTracking.sMZID_LoadPicker[n1No-1][i+4] = gTracking.sMZID_LoadStageModule[nLPStage1No-1][nLoad1PosY-1][i];
				gTracking.sMZID_LoadStageModule[nLPStage1No-1][nLoad1PosY-1][i] = "";
				m_sLog.Format("LoadStage To LoadPicker No:%d, x:%d y:%d - MZ ID:%s",n1No, i, nLoad1PosY-1, gTracking.sMZID_LoadPicker[n1No-1][i]);
				if(gTracking.sMZID_LoadPicker[n1No-1][i+4] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);
			}

			n1LP = gData.nPortNo_LoadPicker[n1No-1] - 1;
			n1LT = gData.nTrayNo_LoadPicker[n1No-1] - 1;
			n1LM = gData.InfoLoadPick[n1No-1][9] - 1;
			for(int i=nS; i<nE; i++) {
				if (gData.InfoLoadPick[n1No-1][i+4] > 0) gLot.nTrayPutCnt[n1LP][2]++;
				gLot.nHistory[n1LP][n1LT][n1LM+i+4][0] = nLPStage1No;
				gLot.nHistory[n1LP][n1LT][n1LM+i+4][1] = 1;
				gLot.nHistory[n1LP][n1LT][n1LM+i+4][2] = i + 1 + 4;
			}

			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 14:
		if (g_objCommon.Get_LoadPickerDown(n1No, n1Type)) {
			if (g_objAJinAXL.Is_Done(AX_LOAD_PICKER_Z1)) {
				g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, nLPStage1No);
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 15:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z1, nLPStage1No)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z1, nLPStage1No);
			m_tLoadPicker1Loop.Takt_Save(12, 8); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Set_LoadPickerClose(n1No, n1Type);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objCommon.Get_LoadPickerClose(n1No, n1Type)) {
			if (!m_tLoadPicker1Loop.Waiting_Time(m_pEquipData->nDelayTime[1])) break;
			m_tLoadPicker1Loop.Takt_Save(12, 9); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Set_LoadPickerUp(n1No);
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, 0);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(m_pEquipData->nDelayTime[10]);
		}
		break;
	case 17:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z1, 0)) {
			if (g_objCommon.Get_LoadPickerCheck(n1No, 9, 0, gData.InfoLoadPick[n1No-1])) {
				if (nLoad1PosX == 3) {
					nLoad1PosX = 2;
					m_nLoadPicker1Case = 11; m_tLoadPicker1Loop.Set_LoopTime(30000);
				} else {
					m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
				}
			}
		}
		break;
	case 18:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z1, 0)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z1, 0);
			m_tLoadPicker1Loop.Takt_Save(12, 10); m_tLoadPicker1Loop.Takt_Start();
			if (Check_EmptyLoadTray()) {
				gData.sLotID_LoadStage[nLPStage1No-1]  = "";
				gData.nTrayNo_LoadStage[nLPStage1No-1] = gData.nPortNo_LoadStage[nLPStage1No-1] = 0;

				if (m_nLoadStage1Case == 30) m_nLoadStage1Case = 31;
				if (m_nLoadStage2Case == 30) m_nLoadStage2Case = 31;
				if (m_nLoadStage1Case == 31 || m_nLoadStage2Case == 31) {
					m_sLog.Format("[LoadPicker1: Start3] Empty - Seq(%d-%d)", m_nLoadStage1Case, m_nLoadStage2Case);
					g_objLogFile.Save_HandlerLog(m_sLog);
				}
			}
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(180000);
		}
		break;
	case 19:
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		break;

	case 20:
		if (m_nLoadPicker2Case < 20 || (m_nLoadPicker2Case >= 41 && m_nLoadPicker2Case < 80)) {
			m_tLoadPicker1Loop.Takt_Save(12, 11); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_BTM_ANGLE_Y, 1);
			g_objCommon.Move_Position(AX_BTM_FOCUS_Z, 1);
			g_objCommon.Move_Position(AX_BTM_LIGHT_Z, 1);

			g_objCommon.Move_Position(AX_LOAD_PICKER_X1, 3);	//BTM
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y1, 3);
			g_objCommon.Move_Position(AX_LOAD_PICKER_P1, 1);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		return TRUE;

	case 21:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_X1, 3) && g_objCommon.Check_Position(AX_LOAD_PICKER_Y1, 3) && g_objCommon.Check_Position(AX_LOAD_PICKER_P1, 1)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_X1, 3);	g_objCommon.Save_Motion(AX_LOAD_PICKER_Y1, 3);	g_objCommon.Save_Motion(AX_LOAD_PICKER_P1, 1);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(m_pEquipData->nDelayTime[10]);
		}
		break;
	case 22:
		if (g_objCommon.Get_LoadPickerCheck(n1No, 9, 0, gData.InfoLoadPick[n1No-1])) {
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(5000);
		}
		break;
	case 23:
		if (m_pEquipData->bUseBottom) {
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, 3);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		} else {
			m_nLoadPicker1Case = 29; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 24:
		if (g_objCommon.Check_Position(AX_BTM_ANGLE_Y, 1) ) {
			g_objCommon.Save_Motion(AX_BTM_ANGLE_Y, 1);
			m_tLoadPicker1Loop.Takt_Save(12, 12); m_tLoadPicker1Loop.Takt_Start();
//			g_objCommon.Move_Position(AX_BTM_FOCUS_Z, 1);
//			g_objCommon.Move_Position(AX_BTM_LIGHT_Z, 1);
			g_objCommon.Set_LoadPickerDown(n1No);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 25:
		if (g_objCommon.Get_LoadPickerDown(n1No)) {
			n1BTMNo = gData.nLPCount1 = gData.nScanRetry[0] = 0;
			n1CMno1 = gData.InfoLoadPick[n1No-1][9];
			n1CMno5 = gData.InfoLoadPick[n1No-1][9] + 4;
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 26:
		if (g_objCommon.Get_LoadPickerDown(n1No)) {
			if (g_objAJinAXL.Is_Done(AX_BTM_FOCUS_Z)) {
				m_tLoadPicker1Loop.Takt_Save(12, 13); m_tLoadPicker1Loop.Takt_Start();
				dLPStage1PosX = m_pMoveData->dLoadPickerX1[3] + (m_pEquipData->dPitchBtm * n1BTMNo);
				g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_X1, dLPStage1PosX);
				g_objCommon.Move_Position(AX_BTM_FOCUS_Z, 1);	nL1Retry = 0;
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 27:
		if (g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_X1, dLPStage1PosX)) {
			if (g_objCommon.Check_Position(AX_BTM_LIGHT_Z, 1) && g_objAJinAXL.Is_Done(AX_BTM_FOCUS_Z)) {
				if (!m_tLoadPicker1Loop.Waiting_Time(m_pEquipData->nDelayTime[2])) break;
				g_objCommon.Save_Motion(AX_LOAD_PICKER_X1, -1, dLPStage1PosX);	g_objCommon.Save_Motion(AX_BTM_LIGHT_Z, 1);
				m_tLoadPicker1Loop.Takt_Save(12, 14); m_tLoadPicker1Loop.Takt_Start();
				gData.bBTMScanDone = FALSE;
				int nModule1 = n1CMno1; if (gData.InfoLoadPick[n1No-1][n1BTMNo] < 1)   nModule1 = -1;
				int nModule5 = n1CMno5; if (gData.InfoLoadPick[n1No-1][n1BTMNo+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC1, "B1", gData.sLotID_LoadPicker[n1No-1], gData.nPortNo_LoadPicker[n1No-1], gData.nTrayNo_LoadPicker[n1No-1], nModule1, gData.nTrayNo_LoadPicker[n1No-1], nModule5);
				gAlm.sAlmLID[1] = gData.sLotID_LoadPicker[n1No-1]; gAlm.nAlmTNo[1] = gData.nTrayNo_LoadPicker[n1No-1]; gAlm.nAlmPNo[1] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[1] = nModule5;
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 28:
		if (gData.bBTMScanDone || !m_pEquipData->bUseBottom) {
//			if (!m_tLoadPicker1Loop.Waiting_Time(850)) break;
			m_tLoadPicker1Loop.Takt_Save(12, 15); m_tLoadPicker1Loop.Takt_Start();
			n1BTMNo++; n1CMno1++; n1CMno5++;
			if (n1BTMNo >= 4) {
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
			} else {
				m_nLoadPicker1Case = 26; m_tLoadPicker1Loop.Set_LoopTime(30000);
			}
		}
		break;

	case 29:
		if (m_pEquipData->bUseBotAng) {
			g_objCommon.Move_Position(AX_BTM_ANGLE_Y, 2);
			g_objCommon.Move_Position(AX_BTM_FOCUS_Z, 2);
			g_objCommon.Move_Position(AX_BTM_LIGHT_Z, 2);
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, 3);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		} else {
			m_nLoadPicker1Case = 40; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 30:
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		break;
	case 31:
		if (g_objCommon.Check_Position(AX_BTM_FOCUS_Z, 2) && g_objCommon.Check_Position(AX_BTM_LIGHT_Z, 2) ) {
			g_objCommon.Save_Motion(AX_BTM_FOCUS_Z, 2);	g_objCommon.Save_Motion(AX_BTM_LIGHT_Z, 2);
			m_tLoadPicker1Loop.Takt_Save(12, 16); m_tLoadPicker1Loop.Takt_Start();
//			g_objCommon.Move_Position(AX_BTM_ANGLE_Y, 2);
			n1BTMNo = gData.nLPCount1 = gData.nScanRetry[0] = 0;
			n1CMno1 = gData.InfoLoadPick[n1No-1][9];
			n1CMno5 = gData.InfoLoadPick[n1No-1][9] + 4;
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 32:
		if (g_objCommon.Get_LoadPickerDown(n1No)) {
			if (g_objAJinAXL.Is_Done(AX_BTM_FOCUS_Z)) {
				m_tLoadPicker1Loop.Takt_Save(12, 17); m_tLoadPicker1Loop.Takt_Start();
				dLPStage1PosX = m_pMoveData->dLoadPickerX1[3] + (m_pEquipData->dPitchBtm * n1BTMNo);
				g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_X1, dLPStage1PosX);
				g_objCommon.Move_Position(AX_BTM_FOCUS_Z, 2);	nL1Retry = 0;
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 33:
		if (g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_X1, dLPStage1PosX)) {
			if (g_objCommon.Check_Position(AX_BTM_ANGLE_Y, 2) && g_objAJinAXL.Is_Done(AX_BTM_FOCUS_Z)) {
				if (!m_tLoadPicker1Loop.Waiting_Time(m_pEquipData->nDelayTime[2])) break;
				g_objCommon.Save_Motion(AX_LOAD_PICKER_X1, -1, dLPStage1PosX);	g_objCommon.Save_Motion(AX_BTM_ANGLE_Y, 2);
				m_tLoadPicker1Loop.Takt_Save(12, 18); m_tLoadPicker1Loop.Takt_Start();
				gData.bBTMScanDone = FALSE;
				int nModule1 = n1CMno1; if (gData.InfoLoadPick[n1No-1][n1BTMNo] < 1)   nModule1 = -1;
				int nModule5 = n1CMno5; if (gData.InfoLoadPick[n1No-1][n1BTMNo+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC1, "AG", gData.sLotID_LoadPicker[n1No-1], gData.nPortNo_LoadPicker[n1No-1], gData.nTrayNo_LoadPicker[n1No-1], nModule1, gData.nTrayNo_LoadPicker[n1No-1], nModule5);
				gAlm.sAlmLID[1] = gData.sLotID_LoadPicker[n1No-1]; gAlm.nAlmTNo[1] = gData.nTrayNo_LoadPicker[n1No-1]; gAlm.nAlmPNo[1] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[1] = nModule5;
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 34:
		if (gData.bBTMScanDone || !m_pEquipData->bUseBotAng) {
//			if (!m_tLoadPicker1Loop.Waiting_Time(450)) break;
			m_tLoadPicker1Loop.Takt_Save(12, 19); m_tLoadPicker1Loop.Takt_Start();
			n1BTMNo++; n1CMno1++; n1CMno5++;
			gData.nLPCount1 = n1BTMNo;
			if (n1BTMNo >= 4) {
//				g_objCommon.Set_LoadPickerUp(n1No);
				g_objCommon.Move_Position(AX_BTM_ANGLE_Y, 1);
				g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, 0);
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
			} else {
				m_nLoadPicker1Case = 32; m_tLoadPicker1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 35:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z1, 0) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z1, 0);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 36:
		if (Check_EemptyLoadPicker(n1No)) {
			g_objCommon.Set_LoadPickerUp(n1No);
			g_objCommon.Move_Position(AX_LOAD_PICKER_X1, 4);

			gData.sLotID_LoadPicker[n1No-1]  = "";
			gData.nTrayNo_LoadPicker[n1No-1] = gData.nPortNo_LoadPicker[n1No-1] = 0;
			for(int i=0; i<10; i++) gData.InfoLoadPick[n1No-1][i] = 0;
			
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		} else {
			m_nLoadPicker1Case = 40; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 37:
		if (g_objCommon.Get_LoadPickerUp(n1No)) {
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_X1, 4) ) {
				g_objCommon.Move_Position(AX_LOAD_PICKER_Y1, 0);
				g_objCommon.Move_Position(AX_LOAD_PICKER_P1, 0);
				m_nLoadPicker1Case = 60; m_tLoadPicker1Loop.Set_LoopTime(30000);
			}
		}
		break;

	case 40:
		if (m_nLoadPicker2Case < 40 || (m_nLoadPicker2Case > 61 && m_nLoadPicker2Case < 80)) {
//			if (g_objCommon.Get_LoadPickerUp(n1No)) {
				m_tLoadPicker1Loop.Takt_Save(12, 20); m_tLoadPicker1Loop.Takt_Start();
				g_objCommon.Move_Position(AX_LOAD_PICKER_X1, 4);
				g_objCommon.Move_Position(AX_LOAD_PICKER_Y1, 4);
				g_objCommon.Move_Position(AX_LOAD_PICKER_P1, 2);
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
//			}
		}
		return TRUE;

	case 41:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_X1, 4) && g_objCommon.Check_Position(AX_LOAD_PICKER_Y1, 4) && g_objCommon.Check_Position(AX_LOAD_PICKER_P1, 2) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_X1, 4);	g_objCommon.Save_Motion(AX_LOAD_PICKER_Y1, 4);	g_objCommon.Save_Motion(AX_LOAD_PICKER_P1, 2);
			m_tLoadPicker1Loop.Takt_Save(12, 21); m_tLoadPicker1Loop.Takt_Start();
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 42:
		n1LPVisionNo = 0;
		if (m_nVisionStage1Case == 0) n1LPVisionNo = 1;
		if (m_nVisionStage2Case == 0) n1LPVisionNo = 2;
		if (m_nVisionStage3Case == 0) n1LPVisionNo = 3;
		if (m_nVisionStage4Case == 0) n1LPVisionNo = 4;

		if (n1LPVisionNo > 0) {
			m_tLoadPicker1Loop.Takt_Save(12, 22); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Set_VisionVacuumOn(n1LPVisionNo);
			g_objCommon.Set_LoadPickerDown(n1No);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 43:
		if (g_objCommon.Get_LoadPickerDown(n1No)) {
			m_tLoadPicker1Loop.Takt_Save(12, 23); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, 4);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 44:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z1, 4) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z1, 4);
			m_tLoadPicker1Loop.Takt_Save(12, 24); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Set_LoadPickerOpen(n1No);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 45:
		if (g_objCommon.Get_LoadPickerOpen(n1No)) {
			if (!m_tLoadPicker1Loop.Waiting_Time(m_pEquipData->nDelayTime[0])) break;
			m_tLoadPicker1Loop.Takt_Save(12, 25); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Set_LoadPickerUp(n1No);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 46:
		if (g_objCommon.Get_LoadPickerUp(n1No)) {
			m_tLoadPicker1Loop.Takt_Save(12, 26); m_tLoadPicker1Loop.Takt_Start();

			gData.sLotID_VisionStage[n1LPVisionNo-1]  = gData.sLotID_LoadPicker[n1No-1];
			gData.nTrayNo_VisionStage[n1LPVisionNo-1] = gData.nTrayNo_LoadPicker[n1No-1];
			gData.nPortNo_VisionStage[n1LPVisionNo-1] = gData.nPortNo_LoadPicker[n1No-1];
			gData.sLotID_VisionStage[4]  = gData.sLotID_VisionStage[n1LPVisionNo-1];

			gData.sLotID_LoadPicker[n1No-1]  = "";
			gData.nTrayNo_LoadPicker[n1No-1] = gData.nPortNo_LoadPicker[n1No-1] = 0;

			for(int i=0; i<10; i++) 
			{
				 gData.InfoVision[n1LPVisionNo-1][i] = gData.InfoLoadPick[n1No-1][i]; gData.InfoLoadPick[n1No-1][i] = 0;				
			}
			for(int i=0; i< PICK_CNT; i++) 
			{ 
				gTracking.sMZID_InspectStage[n1LPVisionNo-1][i] = gTracking.sMZID_LoadPicker[n1No-1][i]; gTracking.sMZID_LoadPicker[n1No-1][i] = "";
				m_sLog.Format("LoadPicker %d To InspectStage %d, pocket:%d - MZ ID:%s",n1No, n1LPVisionNo, i , gTracking.sMZID_InspectStage[n1LPVisionNo-1][i]);
				if(gTracking.sMZID_InspectStage[n1LPVisionNo-1][i] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);

				gLot.nHistory[n1LP][n1LT][n1LM+i][3] = gData.nVisionNo = n1LPVisionNo; 
			}
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(3000+m_pEquipData->nDelayTime[4]);
		}
		break;
	case 47:
		if (m_pEquipData->bNotUseVacCheck || (g_objCommon.Get_VisionVacuumOn(n1LPVisionNo, 9, gData.InfoVision[n1LPVisionNo-1]))) {
			g_objCommon.Set_VisionVacuumOff(n1LPVisionNo);
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, 0);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 48:
		if (g_objCommon.Get_LoadPickerUp(n1No)) {
			m_pDY03->oAlignMasterIn = TRUE; m_pDY03->oAlignMasterOut = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 49:
		if (m_pDX03->iAlignMasterIn && !m_pDX03->iAlignMasterOut) {
			m_tLoadPicker1Loop.Takt_Save(12, 27); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y1, 0);
			g_objCommon.Move_Position(AX_LOAD_PICKER_P1, 0);
			m_pDY03->oAlignSlave1In = TRUE; m_pDY03->oAlignSlave1Out = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 50:
		if (m_pDX03->iAlignSlave1In && !m_pDX03->iAlignSlave1Out) {
			m_pDY03->oAlignSlave2In = TRUE; m_pDY03->oAlignSlave2Out = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 51:
		if (m_pDX03->iAlignSlave2In && !m_pDX03->iAlignSlave2Out) {
			m_tLoadPicker1Loop.Takt_Save(12, 28); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Set_VisionVacuumOn(n1LPVisionNo);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 52:
		if (m_pDX03->iAlignSlave1In && !m_pDX03->iAlignSlave1Out && m_pDX03->iAlignSlave2In && !m_pDX03->iAlignSlave2Out) {
			if (!m_tLoadPicker1Loop.Waiting_Time(300)) break;
			m_pDY03->oAlignSlave2In = FALSE; m_pDY03->oAlignSlave2Out = TRUE;
			g_objAJinAXL.Write_Output(3);
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y1, 0)) {
				g_objCommon.Move_Position(AX_LOAD_PICKER_X1, 1);
			}
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 53:
		if (!m_pDX03->iAlignSlave2In && m_pDX03->iAlignSlave2Out) {
			m_pDY03->oAlignSlave1In = FALSE; m_pDY03->oAlignSlave1Out = TRUE;
			g_objAJinAXL.Write_Output(3);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 54:
		if (!m_pDX03->iAlignSlave1In && m_pDX03->iAlignSlave1Out) {
			m_tLoadPicker1Loop.Takt_Save(12, 29); m_tLoadPicker1Loop.Takt_Start();
			m_pDY03->oAlignMasterIn = FALSE; m_pDY03->oAlignMasterOut = TRUE;
			g_objAJinAXL.Write_Output(3);
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y1, 0)) {
				g_objCommon.Move_Position(AX_LOAD_PICKER_X1, 1);
			}
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 55:
		if (!m_pDX03->iAlignMasterIn && m_pDX03->iAlignMasterOut) {
			m_tLoadPicker1Loop.Takt_Save(12, 30); m_tLoadPicker1Loop.Takt_Start();
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y1, 0)) {
				g_objCommon.Move_Position(AX_LOAD_PICKER_X1, 1);
			}
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(3000+m_pEquipData->nDelayTime[4]);
		}
		break;
	case 56:
		if (g_objCommon.Get_VisionVacuumOn(n1LPVisionNo, 9, gData.InfoVision[n1LPVisionNo-1])) {
			if (Check_EemptyVisionStage(n1LPVisionNo)) {
				gData.sLotID_VisionStage[n1LPVisionNo-1] = "";
				gData.nTrayNo_VisionStage[n1LPVisionNo-1] = gData.nPortNo_VisionStage[n1LPVisionNo-1] = 0;

				for(int i=0; i<10; i++) gData.InfoVision[n1LPVisionNo-1][i] = 0;

			} else {
				if (n1LPVisionNo == 1) m_nVisionStage1Case = 1;
				if (n1LPVisionNo == 2) m_nVisionStage2Case = 1;
				if (n1LPVisionNo == 3) m_nVisionStage3Case = 1;
				if (n1LPVisionNo == 4) m_nVisionStage4Case = 1;
			}
			m_nLoadPicker1Case = 60; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;

	case 60:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z1, 0) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z1, 0);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 61:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y1, 0) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Y1, 0);
			m_tLoadPicker1Loop.Takt_Save(12, 31); m_tLoadPicker1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_PICKER_X1, 1);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 62:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_X1, 1) && g_objCommon.Check_Position(AX_LOAD_PICKER_P1, 0) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_X1, 1);
			m_tLoadPicker1Loop.Takt_Save(12, 32);
			m_nLoadPicker1Case = 70; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;

	case 70:
		if (m_nLoadPicker2Case > 21 && m_nLoadPicker2Case < 70) {
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y1, 1);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 71:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y1, 1) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Y1, 1);
			m_nLoadPicker1Case = 0; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;

	case 81:
		if (g_objCommon.Get_LoadPickerUp(n1No)) {
			if (g_objCommon.Get_LoadPickerCheck(n1No, 9, 0, gData.InfoLoadPick[n1No-1])) {
				m_nLoadPicker1Case = 10; m_tLoadPicker1Loop.Set_LoopTime(30000);
			} else {
				if (n1Type == 1) { n1S = 0; n1E = 1; }
				else			 { n1S = 2; n1E = 3; }
				if (gData.nLoadPicker1ErrNo[n1S] == 1) g_objCommon.Set_LoadPickerOpen(n1No, 9, n1S+1);
				if (gData.nLoadPicker1ErrNo[n1E] == 1) g_objCommon.Set_LoadPickerOpen(n1No, 9, n1E+1);
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 82:
		if (g_objCommon.Get_LoadPickerUp(n1No)) {
			if (!m_tLoadPicker1Loop.Waiting_Time(500)) break;
			g_objCommon.Set_LoadPickerDown(n1No, n1Type);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 83:
		if (g_objCommon.Get_LoadPickerDown(n1No, n1Type)) {
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, nLPStage1No);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 84:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z1, nLPStage1No)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z1, nLPStage1No);
			m_nLoadPicker1Case = 7; m_tLoadPicker1Loop.Set_LoopTime(5000);
		}
		break;

	case 85:
		if (g_objCommon.Get_LoadPickerUp(n1No)) {
			if (g_objCommon.Get_LoadPickerCheck(n1No, 9, 0, gData.InfoLoadPick[n1No-1])) {
				m_nLoadPicker1Case = 17; m_tLoadPicker1Loop.Set_LoopTime(30000);
			} else {
				if (n1Type == 3) { n1S = 4; n1E = 5; }
				else			 { n1S = 6; n1E = 7; }
				if (gData.nLoadPicker1ErrNo[n1S] == 1) g_objCommon.Set_LoadPickerOpen(n1No, 9, n1S+1);
				if (gData.nLoadPicker1ErrNo[n1E] == 1) g_objCommon.Set_LoadPickerOpen(n1No, 9, n1E+1);
				m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 86:
		if (g_objCommon.Get_LoadPickerUp(n1No)) {
			if (!m_tLoadPicker1Loop.Waiting_Time(500)) break;
			g_objCommon.Set_LoadPickerDown(n1No, n1Type);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 87:
		if (g_objCommon.Get_LoadPickerDown(n1No, n1Type)) {
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z1, nLPStage1No);
			m_nLoadPicker1Case++; m_tLoadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 88:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z1, nLPStage1No)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z1, nLPStage1No);
			m_nLoadPicker1Case = 14; m_tLoadPicker1Loop.Set_LoopTime(5000);
		}
		break;

	}

	// 12. (Error : 5000)
	if (m_tLoadPicker1Loop.Over_LoopTime()) {
//		if (m_nLoadPicker1Case == 28 || m_nLoadPicker1Case == 34) {
//			nL1Retry++;
//			if (nL1Retry == 1) { m_nLoadPicker1Case--; return TRUE; }
//		}
//		if (m_nLoadPicker1Case == 27 || m_nLoadPicker1Case == 33) m_nLoadPicker1Case++;
		g_objCommon.Show_Error(5000 + m_nLoadPicker1Case);
		return FALSE;
	}
	return TRUE;
}

// 13. (Error : 5500)
BOOL CSequenceMain::Run_LoadPicker2()
{
	static int n2No = 2;			//2(고정)
	static int nLPStage2No = 0;	//1,2
	static int nLoad2PosX = 0, nLoad2PosY = 0, nStartY2 = 0;
	static int n2BTMNo, n2CMno1, n2CMno5, n2LPVisionNo, n2LP, n2LT, n2LM, n2Type, n2S, n2E, nL2Retry;
	static double dLPStage2PosY = 0, dLPStage2PosX = 0, dLP2PosY = 0;

	switch (m_nLoadPicker2Case) {
	case 0:	// Wait
		if (Check_EmptyLoadTray()) {
			if (m_nLoadStage1Case == 30 && !m_pDX04->iLoadStage1TrayExist) m_nLoadStage1Case = 31;
			if (m_nLoadStage2Case == 30 && !m_pDX04->iLoadStage2TrayExist) m_nLoadStage2Case = 31;
		}
		if (m_nLoadStage1Case == 31 || m_nLoadStage2Case == 31) {
			m_sLog.Format("[LoadPicker2: Start1] Empty - Seq(%d-%d)", m_nLoadStage1Case, m_nLoadStage2Case);
			g_objLogFile.Save_HandlerLog(m_sLog);
		}
		if (m_nLoadStage1Case == 30 || m_nLoadStage2Case == 30) {
			if (m_nLoadStage1Case == 30) nLPStage2No = 1;
			if (m_nLoadStage2Case == 30) nLPStage2No = 2;
			g_objCommon.Set_LoadPickerUp(n2No);	gData.nLPCount2 = 0;
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, 0);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(5000);

			m_sLog.Format("[LoadPicker2: Start] StageNo(%d) LoadAlignOffset(%d/%0.3lf)", nLPStage2No, m_pEquipData->bUseAlignOffset, gData.dAlignOffset[1][1]);
			g_objLogFile.Save_HandlerLog(m_sLog);
		}
		return TRUE;

	case 1:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z2, 0)) {
			if (g_objCommon.Get_LoadPickerUp(n2No)) {
				g_objCommon.Move_Position(AX_LOAD_PICKER_X2, nLPStage2No);
				g_objCommon.Move_Position(AX_LOAD_PICKER_Y2, nLPStage2No);
//				if(m_pEquipData->bUseAlignOffset) dLP2PosY = m_pMoveData->dLoadPickerY1[nLPStage2No] + gData.dAlignOffset[1][1];
//				else							  dLP2PosY = m_pMoveData->dLoadPickerY1[nLPStage2No];
//				g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_Y2, dLP2PosY);
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 2:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_X2, nLPStage2No) && g_objCommon.Check_Position(AX_LOAD_PICKER_Y2, nLPStage2No)) {
//		if (g_objCommon.Check_Position(AX_LOAD_PICKER_X2, nLPStage2No) && g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_Y2, dLP2PosY)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_X2, nLPStage2No);	g_objCommon.Save_Motion(AX_LOAD_PICKER_Y2, nLPStage2No);
			g_objCommon.Set_LoadPickerOpen(n2No);
			if (Check_LoadModule(nLoad2PosX, nLoad2PosY)) {
				if (nLoad2PosY == 1 || nLoad2PosY == 3 || nLoad2PosY == 5 || nLoad2PosY == 7 || nLoad2PosY == 9) {
					gData.InfoLoadPick[n2No-1][9] = ((nLoad2PosY-1) * 4) + 1;	nStartY2 = nLoad2PosY;
					m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
				} else {
					gData.InfoLoadPick[n2No-1][9] = ((nLoad2PosY-2) * 4) + 1;	nLoad2PosX = nStartY2 = 0;
					m_nLoadPicker2Case = 10; m_tLoadPicker2Loop.Set_LoopTime(30000);
				}
				m_tLoadPicker2Loop.Takt_Start();
			} else {
				gData.sLotID_LoadStage[nLPStage2No-1]  = "";
				gData.nTrayNo_LoadStage[nLPStage2No-1] = gData.nPortNo_LoadStage[nLPStage2No-1] = 0;

				if (m_nLoadStage1Case == 30) m_nLoadStage1Case = 31;
				if (m_nLoadStage2Case == 30) m_nLoadStage2Case = 31;
				if (m_nLoadStage1Case == 31 || m_nLoadStage2Case == 31) {
					m_sLog.Format("[LoadPicker2: Start2] Empty - Seq(%d-%d)", m_nLoadStage1Case, m_nLoadStage2Case);
					g_objLogFile.Save_HandlerLog(m_sLog);
				}
				m_nLoadPicker2Case = 0; m_tLoadPicker2Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 3:
		if (nLoad2PosY == 1 || nLoad2PosY == 3 || nLoad2PosY == 5 || nLoad2PosY == 7 || nLoad2PosY == 9) {
			double dOffset1 = (nLoad2PosY <= 5 ? 0.0 : CARRIER_OFFSETY);
			double dOffset2 = (!m_pEquipData->bUseAlignOffset ? 0.0 : gData.dAlignOffset[1][1]);
			if (nLPStage2No == 1) {
				dLPStage2PosY = m_pMoveData->dLoadStageY1[2] + (m_pEquipData->dTrayPitchY * (nLoad2PosY - 1)) + dOffset1 + dOffset2;
				g_objAJinAXL.Move_Absolute(AX_LOAD_STAGE_Y1, dLPStage2PosY);
			}
			if (nLPStage2No == 2) {
				dLPStage2PosY = m_pMoveData->dLoadStageY2[2] + (m_pEquipData->dTrayPitchY * (nLoad2PosY - 1)) + dOffset1 + dOffset2;
				g_objAJinAXL.Move_Absolute(AX_LOAD_STAGE_Y2, dLPStage2PosY);
			}
			n2Type = (nLoad2PosX == 2 ? 2 : 1);
			if (nLoad2PosX == 2) {
				g_objAJinAXL.Move_Relative(AX_LOAD_PICKER_X2, CARRIER_OFFSETX*-1.0);
			}
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		} else {
			m_nLoadPicker2Case = 10; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objAJinAXL.Is_Done(AX_LOAD_PICKER_X2)) {
			if ((nLPStage2No == 1 && g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE_Y1, dLPStage2PosY)) ||
				(nLPStage2No == 2 && g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE_Y2, dLPStage2PosY)) ) {
				if (nLPStage2No == 1) g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, -1, dLPStage2PosY);
				if (nLPStage2No == 2) g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, -1, dLPStage2PosY);
				m_tLoadPicker2Loop.Takt_Save(13, 1); m_tLoadPicker2Loop.Takt_Start();
				g_objCommon.Set_LoadPickerDown(n2No, n2Type);
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 5:
		if (g_objCommon.Get_LoadPickerDown(n2No, n2Type)) {
			m_tLoadPicker2Loop.Takt_Save(13, 2); m_tLoadPicker2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, nLPStage2No);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 6:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z2, nLPStage2No)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z2, nLPStage2No);
			m_tLoadPicker2Loop.Takt_Save(13, 3); m_tLoadPicker2Loop.Takt_Start();
			gData.sLotID_LoadPicker[n2No-1]  = gData.sLotID_LoadStage[nLPStage2No-1];
			gData.nTrayNo_LoadPicker[n2No-1] = gData.nTrayNo_LoadStage[nLPStage2No-1];
			gData.nPortNo_LoadPicker[n2No-1] = gData.nPortNo_LoadStage[nLPStage2No-1];
			gData.sLotID_LoadPicker[2]		 = gData.sLotID_LoadPicker[n2No-1];

			int nS, nE;
			if (n2Type == 1) { nS = 0; nE = 2; }
			else			 { nS = 2; nE = 4; }
			for(int i=nS; i<nE; i++)
			{
				gData.InfoLoadPick[n2No-1][i] = gData.InfoLoadTray[nLoad2PosY-1][i];	gData.InfoLoadTray[nLoad2PosY-1][i] = 0;
				
				gTracking.sMZID_LoadPicker[n2No-1][i] = gTracking.sMZID_LoadStageModule[nLPStage2No-1][nLoad2PosY-1][i];
				gTracking.sMZID_LoadStageModule[nLPStage2No-1][nLoad2PosY-1][i] = "";
				m_sLog.Format("Load Stage To LoadPicker No:%d, x:%d y:%d - MZ ID:%s",n2No, i, nLoad2PosY-1, gTracking.sMZID_LoadPicker[n2No-1][i]);
				if (gTracking.sMZID_LoadPicker[n2No-1][i] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);
			}
			
			n2LP = gData.nPortNo_LoadPicker[n2No-1] - 1;
			n2LT = gData.nTrayNo_LoadPicker[n2No-1] - 1;
			n2LM = gData.InfoLoadPick[n2No-1][9] - 1;
			for(int i=nS; i<nE; i++) {
				if (gData.InfoLoadPick[n2No-1][i] > 0) gLot.nTrayPutCnt[n2LP][2]++;
				gLot.nHistory[n2LP][n2LT][n2LM+i][0] = nLPStage2No;
				gLot.nHistory[n2LP][n2LT][n2LM+i][1] = 2;
				gLot.nHistory[n2LP][n2LT][n2LM+i][2] = i + 1;
			}

			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 7:
		if (g_objCommon.Get_LoadPickerDown(n2No, n2Type)) {
			g_objCommon.Set_LoadPickerClose(n2No, n2Type);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 8:
		if (g_objCommon.Get_LoadPickerClose(n2No, n2Type)) {
			if (!m_tLoadPicker2Loop.Waiting_Time(m_pEquipData->nDelayTime[1])) break;
			m_tLoadPicker2Loop.Takt_Save(13, 4); m_tLoadPicker2Loop.Takt_Start();
			g_objCommon.Set_LoadPickerUp(n2No);
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, 0);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 9:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z2, 0)) {
			if (g_objCommon.Get_LoadPickerUp(n2No)) {
				g_objCommon.Save_Motion(AX_LOAD_PICKER_Z2, 0);
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(m_pEquipData->nDelayTime[10]);
			}
		}
		break;
	case 10:
		if (g_objCommon.Get_LoadPickerCheck(n2No, 9, 0, gData.InfoLoadPick[n2No-1])) {
			if (nLoad2PosX == 3) {
				nLoad2PosX = 2;
				m_nLoadPicker2Case = 3; m_tLoadPicker2Loop.Set_LoopTime(30000);
			} else {
				if (Check_LoadModule(nLoad2PosX, nLoad2PosY)) {
					if (nLoad2PosY == 1 || nLoad2PosY == 3 || nLoad2PosY == 5 || nLoad2PosY == 7 || nLoad2PosY == 9 ||
						(nStartY2 > 0 && (nLoad2PosY > nStartY2+1))) {
						m_nLoadPicker2Case = 18; m_tLoadPicker2Loop.Set_LoopTime(30000);
					} else {
						m_tLoadPicker2Loop.Takt_Save(13, 5); m_tLoadPicker2Loop.Takt_Start();
						double dOffset1 = (nLoad2PosY <= 5 ? 0.0 : CARRIER_OFFSETY);
						double dOffset2 = (!m_pEquipData->bUseAlignOffset ? 0.0 : gData.dAlignOffset[1][1]);
						if (nLPStage2No == 1) {
							dLPStage2PosY = m_pMoveData->dLoadStageY1[2] + (m_pEquipData->dTrayPitchY * (nLoad2PosY - 1)) + dOffset1 + dOffset2;
							g_objAJinAXL.Move_Absolute(AX_LOAD_STAGE_Y1, dLPStage2PosY);
						}
						if (nLPStage2No == 2) {
							dLPStage2PosY = m_pMoveData->dLoadStageY2[2] + (m_pEquipData->dTrayPitchY * (nLoad2PosY - 1)) + dOffset1 + dOffset2;
							g_objAJinAXL.Move_Absolute(AX_LOAD_STAGE_Y2, dLPStage2PosY);
						}
						m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
					}
				} else {
					m_nLoadPicker2Case = 18; m_tLoadPicker2Loop.Set_LoopTime(30000);
				}
			}
		}
		break;

	case 11:
		if ((m_nLoadPicker1Case > 41 && m_nLoadPicker1Case < 80) ||
			(m_nLoadPicker1Case > 33 && m_nLoadPicker1Case < 80 && gData.nLPCount1 >= 2) ) {
			m_tLoadPicker2Loop.Takt_Save(13, 6); m_tLoadPicker2Loop.Takt_Start();
			n2Type = (nLoad2PosX == 2 ? 4 : 3);
			if (n2Type == 4) dLPStage2PosX = m_pMoveData->dLoadPickerX2[nLPStage2No] + (m_pEquipData->dTrayPitchX * 4) - CARRIER_OFFSETX;
			else			 dLPStage2PosX = m_pMoveData->dLoadPickerX2[nLPStage2No] + (m_pEquipData->dTrayPitchX * 4);
			g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_X2, dLPStage2PosX);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		return TRUE;

	case 12:
		if ((nLPStage2No == 1 && g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE_Y1, dLPStage2PosY)) ||
			(nLPStage2No == 2 && g_objAJinAXL.Is_MoveDone(AX_LOAD_STAGE_Y2, dLPStage2PosY)) ) {
			if (g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_X2, dLPStage2PosX)) {
				if (nLPStage2No == 1) g_objCommon.Save_Motion(AX_LOAD_STAGE_Y1, -1, dLPStage2PosY);
				if (nLPStage2No == 2) g_objCommon.Save_Motion(AX_LOAD_STAGE_Y2, -1, dLPStage2PosY);
				g_objCommon.Save_Motion(AX_LOAD_PICKER_X2, -1, dLPStage2PosX);
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 13:
		if (g_objCommon.Get_LoadPickerOpen(n2No, n2Type)) {
			g_objCommon.Set_LoadPickerDown(n2No, n2Type);
			m_tLoadPicker2Loop.Takt_Save(13, 7); m_tLoadPicker2Loop.Takt_Start();

			gData.sLotID_LoadPicker[n2No-1]  = gData.sLotID_LoadStage[nLPStage2No-1];
			gData.nTrayNo_LoadPicker[n2No-1] = gData.nTrayNo_LoadStage[nLPStage2No-1];
			gData.nPortNo_LoadPicker[n2No-1] = gData.nPortNo_LoadStage[nLPStage2No-1];
			gData.sLotID_LoadPicker[2]		 = gData.sLotID_LoadPicker[n2No-1];

			int nS, nE;
			if (n2Type == 3) { nS = 0; nE = 2; }
			else			 { nS = 2; nE = 4; }
			for(int i=nS; i<nE; i++) {
				gData.InfoLoadPick[n2No-1][i+4] = gData.InfoLoadTray[nLoad2PosY-1][i]; gData.InfoLoadTray[nLoad2PosY-1][i] = 0;
				
				gTracking.sMZID_LoadPicker[n2No-1][i+4] = gTracking.sMZID_LoadStageModule[nLPStage2No-1][nLoad2PosY-1][i];
				gTracking.sMZID_LoadStageModule[nLPStage2No-1][nLoad2PosY-1][i] = "";
				m_sLog.Format("Load Stage To LoadPicker No:%d, x:%d y:%d - MZ ID:%s",n2No, i, nLoad2PosY-1, gTracking.sMZID_LoadPicker[n2No-1][i]);
				if (gTracking.sMZID_LoadPicker[n2No-1][i+4] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);
			}

			n2LP = gData.nPortNo_LoadPicker[n2No-1] - 1;
			n2LT = gData.nTrayNo_LoadPicker[n2No-1] - 1;
			n2LM = gData.InfoLoadPick[n2No-1][9] - 1;
			for(int i=nS; i<nE; i++) {
				if (gData.InfoLoadPick[n2No-1][i+4] > 0) gLot.nTrayPutCnt[n2LP][2]++;
				gLot.nHistory[n2LP][n2LT][n2LM+i+4][0] = nLPStage2No;
				gLot.nHistory[n2LP][n2LT][n2LM+i+4][1] = 2;
				gLot.nHistory[n2LP][n2LT][n2LM+i+4][2] = i + 1 + 4;
			}

			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 14:
		if (g_objCommon.Get_LoadPickerDown(n2No, n2Type)) {
			if (g_objAJinAXL.Is_Done(AX_LOAD_PICKER_Z2)) {
				g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, nLPStage2No);
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 15:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z2, nLPStage2No)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z2, nLPStage2No);
			m_tLoadPicker2Loop.Takt_Save(13, 8); m_tLoadPicker2Loop.Takt_Start();
			g_objCommon.Set_LoadPickerClose(n2No, n2Type);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objCommon.Get_LoadPickerClose(n2No, n2Type)) {
			if (!m_tLoadPicker2Loop.Waiting_Time(m_pEquipData->nDelayTime[1])) break;
			m_tLoadPicker2Loop.Takt_Save(13, 9); m_tLoadPicker2Loop.Takt_Start();
			g_objCommon.Set_LoadPickerUp(n2No);
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, 0);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(m_pEquipData->nDelayTime[10]);
		}
		break;
	case 17:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z2, 0)) {
			if (g_objCommon.Get_LoadPickerCheck(n2No, 9, 0, gData.InfoLoadPick[n2No-1])) {
				if (nLoad2PosX == 3) {
					nLoad2PosX = 2;
					m_nLoadPicker2Case = 11; m_tLoadPicker2Loop.Set_LoopTime(30000);
				} else {
					m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
				}
			}
		}
		break;
	case 18:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z2, 0)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z2, 0);
			m_tLoadPicker2Loop.Takt_Save(13, 10); m_tLoadPicker2Loop.Takt_Start();
			if (Check_EmptyLoadTray()) {
				gData.sLotID_LoadStage[nLPStage2No-1]  = "";
				gData.nTrayNo_LoadStage[nLPStage2No-1] = gData.nPortNo_LoadStage[nLPStage2No-1] = 0;

				if (m_nLoadStage1Case == 30) m_nLoadStage1Case = 31;
				if (m_nLoadStage2Case == 30) m_nLoadStage2Case = 31;
				if (m_nLoadStage1Case == 31 || m_nLoadStage2Case == 31) {
					m_sLog.Format("[LoadPicker2: Start3] Empty - Seq(%d-%d)", m_nLoadStage1Case, m_nLoadStage2Case);
					g_objLogFile.Save_HandlerLog(m_sLog);
				}
			}
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(180000);
		}
		break;
	case 19:
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		break;

	case 20:
		if (m_nLoadPicker1Case < 20 || (m_nLoadPicker1Case >= 41 && m_nLoadPicker1Case < 80)) {
			m_tLoadPicker2Loop.Takt_Save(13, 11); m_tLoadPicker2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_BTM_ANGLE_Y, 1);
			g_objCommon.Move_Position(AX_BTM_FOCUS_Z, 1);
			g_objCommon.Move_Position(AX_BTM_LIGHT_Z, 1);

			g_objCommon.Move_Position(AX_LOAD_PICKER_X2, 3);
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y2, 3);
			g_objCommon.Move_Position(AX_LOAD_PICKER_P2, 1);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		return TRUE;

	case 21:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_X2, 3) && g_objCommon.Check_Position(AX_LOAD_PICKER_Y2, 3) && g_objCommon.Check_Position(AX_LOAD_PICKER_P2, 1)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_X2, 3);	g_objCommon.Save_Motion(AX_LOAD_PICKER_Y2, 3);	g_objCommon.Save_Motion(AX_LOAD_PICKER_P2, 1);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(m_pEquipData->nDelayTime[10]);
		}
		break;
	case 22:
		if (g_objCommon.Get_LoadPickerCheck(n2No, 9, 0, gData.InfoLoadPick[n2No-1])) {
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(5000);
		}
		break;
	case 23:
		if (m_pEquipData->bUseBottom) {
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, 3);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		} else {
			m_nLoadPicker2Case = 29; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 24:
		if (g_objCommon.Check_Position(AX_BTM_ANGLE_Y, 1) ) {
			g_objCommon.Save_Motion(AX_BTM_ANGLE_Y, 1);
			m_tLoadPicker2Loop.Takt_Save(13, 12); m_tLoadPicker2Loop.Takt_Start();
//			g_objCommon.Move_Position(AX_BTM_FOCUS_Z, 1);
//			g_objCommon.Move_Position(AX_BTM_LIGHT_Z, 1);
			g_objCommon.Set_LoadPickerDown(n2No);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 25:
		if (g_objCommon.Get_LoadPickerDown(n2No)) {
			n2BTMNo = gData.nScanRetry[0] = 0;
			n2CMno1 = gData.InfoLoadPick[n2No-1][9];
			n2CMno5 = gData.InfoLoadPick[n2No-1][9] + 4;
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 26:
		if (g_objCommon.Get_LoadPickerDown(n2No)) {
			if (g_objAJinAXL.Is_Done(AX_BTM_FOCUS_Z)) {
				m_tLoadPicker2Loop.Takt_Save(13, 13); m_tLoadPicker2Loop.Takt_Start();
				dLPStage2PosX = m_pMoveData->dLoadPickerX2[3] + (m_pEquipData->dPitchBtm * n2BTMNo);
				g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_X2, dLPStage2PosX);
				g_objCommon.Move_Position(AX_BTM_FOCUS_Z, 1);	nL2Retry = 0;
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 27:
		if (g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_X2, dLPStage2PosX)) {
			if (g_objCommon.Check_Position(AX_BTM_LIGHT_Z, 1) && g_objAJinAXL.Is_Done(AX_BTM_FOCUS_Z)) {
				if (!m_tLoadPicker2Loop.Waiting_Time(m_pEquipData->nDelayTime[2])) break;
				g_objCommon.Save_Motion(AX_LOAD_PICKER_X2, -1, dLPStage2PosX);	g_objCommon.Save_Motion(AX_BTM_LIGHT_Z, 1);
				m_tLoadPicker2Loop.Takt_Save(13, 14); m_tLoadPicker2Loop.Takt_Start();
				gData.bBTMScanDone = FALSE;
				int nModule1 = n2CMno1; if (gData.InfoLoadPick[n2No-1][n2BTMNo] < 1)   nModule1 = -1;
				int nModule5 = n2CMno5; if (gData.InfoLoadPick[n2No-1][n2BTMNo+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC1, "B1", gData.sLotID_LoadPicker[n2No-1], gData.nPortNo_LoadPicker[n2No-1], gData.nTrayNo_LoadPicker[n2No-1], nModule1, gData.nTrayNo_LoadPicker[n2No-1], nModule5);
				gAlm.sAlmLID[1] = gData.sLotID_LoadPicker[n2No-1]; gAlm.nAlmTNo[1] = gData.nTrayNo_LoadPicker[n2No-1]; gAlm.nAlmPNo[1] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[1] = nModule5;
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 28:
		if (gData.bBTMScanDone || !m_pEquipData->bUseBottom) {
//			if (!m_tLoadPicker2Loop.Waiting_Time(850)) break;
			m_tLoadPicker2Loop.Takt_Save(13, 15); m_tLoadPicker2Loop.Takt_Start();
			n2BTMNo++; n2CMno1++; n2CMno5++;
			if (n2BTMNo >= 4) {
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
			} else {
				m_nLoadPicker2Case = 26; m_tLoadPicker2Loop.Set_LoopTime(30000);
			}
		}
		break;

	case 29:
		if (m_pEquipData->bUseBotAng) {
			g_objCommon.Move_Position(AX_BTM_ANGLE_Y, 2);
			g_objCommon.Move_Position(AX_BTM_FOCUS_Z, 2);
			g_objCommon.Move_Position(AX_BTM_LIGHT_Z, 2);
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, 3);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		} else {
			m_nLoadPicker2Case = 40; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 30:
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		break;
	case 31:
		if (g_objCommon.Check_Position(AX_BTM_FOCUS_Z, 2) && g_objCommon.Check_Position(AX_BTM_LIGHT_Z, 2) ) {
			g_objCommon.Save_Motion(AX_BTM_FOCUS_Z, 2);	g_objCommon.Save_Motion(AX_BTM_LIGHT_Z, 2);
			m_tLoadPicker2Loop.Takt_Save(13, 16); m_tLoadPicker2Loop.Takt_Start();
			n2BTMNo = gData.nLPCount2 = gData.nScanRetry[0] = 0;
			n2CMno1 = gData.InfoLoadPick[n2No-1][9];
			n2CMno5 = gData.InfoLoadPick[n2No-1][9] + 4;
//			g_objCommon.Move_Position(AX_BTM_ANGLE_Y, 2);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 32:
		if (g_objCommon.Get_LoadPickerDown(n2No)) {
			if (g_objAJinAXL.Is_Done(AX_BTM_FOCUS_Z)) {
				m_tLoadPicker2Loop.Takt_Save(13, 17); m_tLoadPicker2Loop.Takt_Start();
				dLPStage2PosX = m_pMoveData->dLoadPickerX2[3] + (m_pEquipData->dPitchBtm * n2BTMNo);
				g_objAJinAXL.Move_Absolute(AX_LOAD_PICKER_X2, dLPStage2PosX);
				g_objCommon.Move_Position(AX_BTM_FOCUS_Z, 2);	nL2Retry = 0;
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 33:
		if (g_objAJinAXL.Is_MoveDone(AX_LOAD_PICKER_X2, dLPStage2PosX)) {
			if (g_objCommon.Check_Position(AX_BTM_ANGLE_Y, 2) && g_objAJinAXL.Is_Done(AX_BTM_FOCUS_Z)) {
				if (!m_tLoadPicker2Loop.Waiting_Time(m_pEquipData->nDelayTime[2])) break;
				g_objCommon.Save_Motion(AX_LOAD_PICKER_X2, -1, dLPStage2PosX);	g_objCommon.Save_Motion(AX_BTM_ANGLE_Y, 2);
				m_tLoadPicker2Loop.Takt_Save(13, 18); m_tLoadPicker2Loop.Takt_Start();
				gData.bBTMScanDone = FALSE;
				int nModule1 = n2CMno1; if (gData.InfoLoadPick[n2No-1][n2BTMNo] < 1)   nModule1 = -1;
				int nModule5 = n2CMno5; if (gData.InfoLoadPick[n2No-1][n2BTMNo+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC1, "AG", gData.sLotID_LoadPicker[n2No-1], gData.nPortNo_LoadPicker[n2No-1], gData.nTrayNo_LoadPicker[n2No-1], nModule1, gData.nTrayNo_LoadPicker[n2No-1], nModule5);
				gAlm.sAlmLID[1] = gData.sLotID_LoadPicker[n2No-1]; gAlm.nAlmTNo[1] = gData.nTrayNo_LoadPicker[n2No-1]; gAlm.nAlmPNo[1] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[1] = nModule5;
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 34:
		if (gData.bBTMScanDone || !m_pEquipData->bUseBotAng) {
//			if (!m_tLoadPicker2Loop.Waiting_Time(450)) break;
			m_tLoadPicker2Loop.Takt_Save(13, 19); m_tLoadPicker2Loop.Takt_Start();
			n2BTMNo++; n2CMno1++; n2CMno5++;
			gData.nLPCount2 = n2BTMNo;
			if (n2BTMNo >= 4) {
//				g_objCommon.Set_LoadPickerUp(n2No);
				g_objCommon.Move_Position(AX_BTM_ANGLE_Y, 1);
				g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, 0);
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
			} else {
				m_nLoadPicker2Case = 32; m_tLoadPicker2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 35:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z2, 0) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z2, 0);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 36:
		if (Check_EemptyLoadPicker(n2No)) {
			g_objCommon.Set_LoadPickerUp(n2No);
			g_objCommon.Move_Position(AX_LOAD_PICKER_X2, 4);

			gData.sLotID_LoadPicker[n2No-1]  = "";
			gData.nTrayNo_LoadPicker[n2No-1] = gData.nPortNo_LoadPicker[n2No-1] = 0;
			for(int i=0; i<10; i++) gData.InfoLoadPick[n2No-1][i] = 0;
			
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		} else {
			m_nLoadPicker2Case = 40; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 37:
		if (g_objCommon.Get_LoadPickerUp(n2No)) {
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_X2, 4) ) {
				g_objCommon.Move_Position(AX_LOAD_PICKER_Y2, 0);
				g_objCommon.Move_Position(AX_LOAD_PICKER_P2, 0);
				m_nLoadPicker2Case = 60; m_tLoadPicker2Loop.Set_LoopTime(30000);
			}
		}
		break;

	case 40:
		if (m_nLoadPicker1Case < 40 || (m_nLoadPicker1Case > 61 && m_nLoadPicker1Case < 80)) {
//			if (g_objCommon.Get_LoadPickerUp(n2No)) {
				m_tLoadPicker2Loop.Takt_Save(13, 20); m_tLoadPicker2Loop.Takt_Start(); 
				g_objCommon.Move_Position(AX_LOAD_PICKER_X2, 4);	//Align
				g_objCommon.Move_Position(AX_LOAD_PICKER_Y2, 4);
				g_objCommon.Move_Position(AX_LOAD_PICKER_P2, 2);
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
//			}
		}
		return TRUE;

	case 41:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_X2, 4) && g_objCommon.Check_Position(AX_LOAD_PICKER_Y2, 4) && g_objCommon.Check_Position(AX_LOAD_PICKER_P2, 2) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_X2, 4);	g_objCommon.Save_Motion(AX_LOAD_PICKER_Y2, 4);	g_objCommon.Save_Motion(AX_LOAD_PICKER_P2, 2);
			m_tLoadPicker2Loop.Takt_Save(13, 21); m_tLoadPicker2Loop.Takt_Start(); 
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 42:
		n2LPVisionNo = 0;
		if (m_nVisionStage1Case == 0) n2LPVisionNo = 1;
		if (m_nVisionStage2Case == 0) n2LPVisionNo = 2;
		if (m_nVisionStage3Case == 0) n2LPVisionNo = 3;
		if (m_nVisionStage4Case == 0) n2LPVisionNo = 4;

		if (n2LPVisionNo > 0) {
			m_tLoadPicker2Loop.Takt_Save(13, 22); m_tLoadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_VisionVacuumOn(n2LPVisionNo);
			g_objCommon.Set_LoadPickerDown(n2No);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 43:
		if (g_objCommon.Get_LoadPickerDown(n2No)) {
			m_tLoadPicker2Loop.Takt_Save(13, 23); m_tLoadPicker2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, 4);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 44:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z2, 4) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z2, 4);
			m_tLoadPicker2Loop.Takt_Save(13, 24); m_tLoadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_LoadPickerOpen(n2No);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 45:
		if (g_objCommon.Get_LoadPickerOpen(n2No)) {
			if (!m_tLoadPicker2Loop.Waiting_Time(m_pEquipData->nDelayTime[0])) break;
			m_tLoadPicker2Loop.Takt_Save(13, 25); m_tLoadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_LoadPickerUp(n2No);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 46:
		if (g_objCommon.Get_LoadPickerUp(n2No)) {
			m_tLoadPicker2Loop.Takt_Save(13, 26); m_tLoadPicker2Loop.Takt_Start(); 

			gData.sLotID_VisionStage[n2LPVisionNo-1]  = gData.sLotID_LoadPicker[n2No-1];
			gData.nTrayNo_VisionStage[n2LPVisionNo-1] = gData.nTrayNo_LoadPicker[n2No-1];
			gData.nPortNo_VisionStage[n2LPVisionNo-1] = gData.nPortNo_LoadPicker[n2No-1];
			gData.sLotID_VisionStage[4]  = gData.sLotID_VisionStage[n2LPVisionNo-1];

			gData.sLotID_LoadPicker[n2No-1]  = "";
			gData.nTrayNo_LoadPicker[n2No-1] = gData.nPortNo_LoadPicker[n2No-1] = 0;

			for(int i=0; i<10; i++) {
				 gData.InfoVision[n2LPVisionNo-1][i] = gData.InfoLoadPick[n2No-1][i]; gData.InfoLoadPick[n2No-1][i] = 0;
				
			}
			for(int i=0; i < PICK_CNT; i++) 
			{ 
				gTracking.sMZID_InspectStage[n2LPVisionNo-1][i] = gTracking.sMZID_LoadPicker[n2No-1][i]; gTracking.sMZID_LoadPicker[n2No-1][i] = "";
				m_sLog.Format("LoadPicker %d To InspectStage %d, pocket:%d - MZ ID:%s",n2No, n2LPVisionNo, i , gTracking.sMZID_InspectStage[n2LPVisionNo-1][i]);
				if(gTracking.sMZID_InspectStage[n2LPVisionNo-1][i] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);
				gLot.nHistory[n2LP][n2LT][n2LM+i][3] = gData.nVisionNo = n2LPVisionNo;
			}
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(3000+m_pEquipData->nDelayTime[4]);
		}
		break;
	case 47:
		if (m_pEquipData->bNotUseVacCheck || (g_objCommon.Get_VisionVacuumOn(n2LPVisionNo, 9, gData.InfoVision[n2LPVisionNo-1]))) {
			g_objCommon.Set_VisionVacuumOff(n2LPVisionNo);
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, 0);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 48:
		if (g_objCommon.Get_LoadPickerUp(n2No)) {
			m_pDY03->oAlignMasterIn = TRUE; m_pDY03->oAlignMasterOut = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 49:
		if (m_pDX03->iAlignMasterIn && !m_pDX03->iAlignMasterOut) {
			m_tLoadPicker2Loop.Takt_Save(13, 27); m_tLoadPicker2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y2, 0);
			g_objCommon.Move_Position(AX_LOAD_PICKER_P2, 0);
			m_pDY03->oAlignSlave1In = TRUE; m_pDY03->oAlignSlave1Out = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 50:
		if (m_pDX03->iAlignSlave1In && !m_pDX03->iAlignSlave1Out) {
			m_pDY03->oAlignSlave2In = TRUE; m_pDY03->oAlignSlave2Out = FALSE;
			g_objAJinAXL.Write_Output(3);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 51:
		if (m_pDX03->iAlignSlave2In && !m_pDX03->iAlignSlave2Out) {
			m_tLoadPicker2Loop.Takt_Save(13, 28); m_tLoadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_VisionVacuumOn(n2LPVisionNo);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 52:
		if (m_pDX03->iAlignSlave1In && !m_pDX03->iAlignSlave1Out && m_pDX03->iAlignSlave2In && !m_pDX03->iAlignSlave2Out) {
			if (!m_tLoadPicker2Loop.Waiting_Time(300)) break;
			m_pDY03->oAlignSlave2In = FALSE; m_pDY03->oAlignSlave2Out = TRUE;
			g_objAJinAXL.Write_Output(3);
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y2, 0)) {
				g_objCommon.Move_Position(AX_LOAD_PICKER_X2, 1);
			}
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 53:
		if (!m_pDX03->iAlignSlave2In && m_pDX03->iAlignSlave2Out) {
			m_pDY03->oAlignSlave1In = FALSE; m_pDY03->oAlignSlave1Out = TRUE;
			g_objAJinAXL.Write_Output(3);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 54:
		if (!m_pDX03->iAlignSlave1In && m_pDX03->iAlignSlave1Out) {
			m_tLoadPicker2Loop.Takt_Save(13, 29); m_tLoadPicker2Loop.Takt_Start(); 
			m_pDY03->oAlignMasterIn = FALSE; m_pDY03->oAlignMasterOut = TRUE;
			g_objAJinAXL.Write_Output(3);
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y2, 0)) {
				g_objCommon.Move_Position(AX_LOAD_PICKER_X2, 1);
			}
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 55:
		if (!m_pDX03->iAlignMasterIn && m_pDX03->iAlignMasterOut) {
			m_tLoadPicker2Loop.Takt_Save(13, 30); m_tLoadPicker2Loop.Takt_Start(); 
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y2, 0)) {
				g_objCommon.Move_Position(AX_LOAD_PICKER_X2, 1);
			}
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(3000+m_pEquipData->nDelayTime[4]);
		}
		break;
	case 56:
		if (g_objCommon.Get_VisionVacuumOn(n2LPVisionNo, 9, gData.InfoVision[n2LPVisionNo-1])) {
			if (Check_EemptyVisionStage(n2LPVisionNo)) {
				gData.sLotID_VisionStage[n2LPVisionNo-1] = "";
				gData.nTrayNo_VisionStage[n2LPVisionNo-1] = gData.nPortNo_VisionStage[n2LPVisionNo-1] = 0;

				for(int i=0; i<10; i++) gData.InfoVision[n2LPVisionNo-1][i] = 0;
			} else {
				if (n2LPVisionNo == 1) m_nVisionStage1Case = 1;
				if (n2LPVisionNo == 2) m_nVisionStage2Case = 1;
				if (n2LPVisionNo == 3) m_nVisionStage3Case = 1;
				if (n2LPVisionNo == 4) m_nVisionStage4Case = 1;
			}
			m_nLoadPicker2Case = 60; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;

	case 60:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z2, 0) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z2, 0);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 61:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y2, 0) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Y2, 0);
			m_tLoadPicker2Loop.Takt_Save(13, 31); m_tLoadPicker2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_LOAD_PICKER_X2, 1);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 62:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_X2, 1) && g_objCommon.Check_Position(AX_LOAD_PICKER_P2, 0) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_X2, 1);
			m_tLoadPicker2Loop.Takt_Save(13, 32);
			m_nLoadPicker2Case = 70; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;

	case 70:
		if (m_nLoadPicker1Case > 21 && m_nLoadPicker1Case < 70) {
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y2, 1);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 71:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y2, 1) ) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Y2, 1);
			m_nLoadPicker2Case = 0; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;

	case 81:
		if (g_objCommon.Get_LoadPickerUp(n2No)) {
			if (g_objCommon.Get_LoadPickerCheck(n2No, 9, 0, gData.InfoLoadPick[n2No-1])) {
				m_nLoadPicker2Case = 10; m_tLoadPicker2Loop.Set_LoopTime(30000);
			} else {
				int nS, nE;
				if (n2Type == 1) { nS = 0; nE = 1; }
				else			 { nS = 2; nE = 3; }
				if (gData.nLoadPicker2ErrNo[nS] == 1) g_objCommon.Set_LoadPickerOpen(n2No, 9, nS+1);
				if (gData.nLoadPicker2ErrNo[nE] == 1) g_objCommon.Set_LoadPickerOpen(n2No, 9, nE+1);
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 82:
		if (g_objCommon.Get_LoadPickerUp(n2No)) {
			if (!m_tLoadPicker2Loop.Waiting_Time(500)) break;
			g_objCommon.Set_LoadPickerDown(n2No, n2Type);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 83:
		if (g_objCommon.Get_LoadPickerDown(n2No, n2Type)) {
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, nLPStage2No);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 84:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z2, nLPStage2No)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z2, nLPStage2No);
			m_nLoadPicker2Case = 7; m_tLoadPicker2Loop.Set_LoopTime(5000);
		}
		break;

	case 85:
		if (g_objCommon.Get_LoadPickerUp(n2No)) {
			if (g_objCommon.Get_LoadPickerCheck(n2No, 9, 0, gData.InfoLoadPick[n2No-1])) {
				m_nLoadPicker2Case = 17; m_tLoadPicker2Loop.Set_LoopTime(30000);
			} else {
				int nS, nE;
				if (n2Type == 3) { nS = 4; nE = 5; }
				else			 { nS = 6; nE = 7; }
				if (gData.nLoadPicker2ErrNo[nS] == 1) g_objCommon.Set_LoadPickerOpen(n2No, 9, nS+1);
				if (gData.nLoadPicker2ErrNo[nE] == 1) g_objCommon.Set_LoadPickerOpen(n2No, 9, nE+1);
				m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 86:
		if (g_objCommon.Get_LoadPickerUp(n2No)) {
			if (!m_tLoadPicker2Loop.Waiting_Time(500)) break;
			g_objCommon.Set_LoadPickerDown(n2No, n2Type);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 87:
		if (g_objCommon.Get_LoadPickerDown(n2No, n2Type)) {
			g_objCommon.Move_Position(AX_LOAD_PICKER_Z2, nLPStage2No);
			m_nLoadPicker2Case++; m_tLoadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 88:
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Z2, nLPStage2No)) {
			g_objCommon.Save_Motion(AX_LOAD_PICKER_Z2, nLPStage2No);
			m_nLoadPicker2Case = 14; m_tLoadPicker2Loop.Set_LoopTime(5000);
		}
		break;

	}

	// 13. (Error : 5500)
	if (m_tLoadPicker2Loop.Over_LoopTime()) {
//		if (m_nLoadPicker2Case == 28 || m_nLoadPicker2Case == 34) {
//			nL2Retry++;
//			if (nL2Retry == 1) { m_nLoadPicker2Case--; return TRUE; }
//		}
//		if (m_nLoadPicker2Case == 27 || m_nLoadPicker2Case == 33) m_nLoadPicker2Case++;
		g_objCommon.Show_Error(5500 + m_nLoadPicker2Case);
		return FALSE;
	}
	return TRUE;
}

// 14. (Error : 6000)
BOOL CSequenceMain::Run_VisionStage1()
{
	static int nS1No = 1;		//1,2,3,4(고정)
	static int n1Tno1, n1Tno5, n1FrontVisionCase, nV1Retry;
	static double dStage1PosX = 0;
	static DWORD dwV1RosTime[3];	//Top2,Buffer,Unload

	n1FrontVisionCase = m_nVisionStage3Case;
	switch (m_nVisionStage1Case) {
	case 0:	// Wait
		m_tVisionStage1Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (n1FrontVisionCase > 14) {
//		if (n1FrontVisionCase > 15) {
			m_sLog.Format("[Inspection Stage1: Start] Inspection Stage3 Case(%d)", n1FrontVisionCase);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 2:
		if (!m_pDX03->iAlignSlave1In && m_pDX03->iAlignSlave1Out && !m_pDX03->iAlignSlave2In && m_pDX03->iAlignSlave2Out && !m_pDX03->iAlignMasterIn && m_pDX03->iAlignMasterOut) {
			if (m_pDX07->iInspectStage1Up && !m_pDX07->iInspectStage1Down) {
				if (m_pDX07->iInspectStage1Yin && !m_pDX07->iInspectStage1YOut) {
					m_tVisionStage1Loop.Takt_Start(); 
					m_pDY07->oInspectStage1Deg0 = TRUE; m_pDY07->oInspectStage1Deg90 = FALSE;
					g_objAJinAXL.Write_Output(7);
					g_objCommon.Move_Position(AX_INSPECT_STAGE_X1, 1);
					m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 3:	//Top1-Angle
		if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 1)) {
			g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);
			g_objCommon.Move_Position(AX_TOP1_LIGHT_Z, 1);
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X1, 1)) {
			if (m_pDX07->iInspectStage1Deg0) {
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X1, 1);
				m_tVisionStage1Loop.Takt_Save(14, 1); m_tVisionStage1Loop.Takt_Start(); 
				gData.nVisionPos[1][0] = 0; gData.nVisionPos[1][1] = nS1No;
				gData.nTop1Cnt = gData.nScanRetry[1] = 0;
				n1Tno1 = gData.InfoVision[nS1No-1][9];
				n1Tno5 = gData.InfoVision[nS1No-1][9] + 4;
				m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 5:	//Top2 Scan 대기
		if (n1FrontVisionCase >= 22 || (n1FrontVisionCase >= 17 && gData.nTop1Cnt <= 1) || (n1FrontVisionCase >= 17 && gData.nTop2Cnt >= gData.nTop1Cnt)) {
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(20000);
		}
		return TRUE;

	case 6:
		if (m_pDX07->iInspectStage1Up && !m_pDX07->iInspectStage1Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				m_tVisionStage1Loop.Takt_Save(14, 2); m_tVisionStage1Loop.Takt_Start(); 
				dStage1PosX = m_pMoveData->dVisionStageX1[1] + (m_pEquipData->dPitchTop * gData.nTop1Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X1, dStage1PosX);
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);	nV1Retry = 0;
				m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 7:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X1, dStage1PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 1) && g_objCommon.Check_Position(AX_TOP1_VISION_Z, 1)) {
			if (g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 1) && g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				if (!m_tVisionStage1Loop.Waiting_Time(m_pEquipData->nDelayTime[3])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X1, -1, dStage1PosX);	g_objCommon.Save_Motion(AX_TOP1_LIGHT_Z, 1);
				m_tVisionStage1Loop.Takt_Save(14, 3); m_tVisionStage1Loop.Takt_Start(); 
				gData.bTop1ScanDone = FALSE;	gData.nVsNo[0] = nS1No; gData.nVsIx[0] = gData.nTop1Cnt+1;
				int nModule1 = n1Tno1; if (gData.InfoVision[nS1No-1][gData.nTop1Cnt] < 1)   nModule1 = -1;
				int nModule5 = n1Tno5; if (gData.InfoVision[nS1No-1][gData.nTop1Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "T1", gData.sLotID_VisionStage[nS1No-1], gData.nPortNo_VisionStage[nS1No-1], gData.nTrayNo_VisionStage[nS1No-1], nModule1, gData.nTrayNo_VisionStage[nS1No-1], nModule5);
				gAlm.sAlmLID[2] = gData.sLotID_VisionStage[nS1No-1]; gAlm.nAlmTNo[2] = gData.nTrayNo_VisionStage[nS1No-1]; gAlm.nAlmPNo[2] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[2] = nModule5;
				m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 8:
		if (gData.bTop1ScanDone || !m_pEquipData->bUseTop1) {
			m_tVisionStage1Loop.Takt_Save(14, 4); m_tVisionStage1Loop.Takt_Start(); 
			gData.nTop1Cnt++; n1Tno1++; n1Tno5++;
			if (gData.nTop1Cnt >= 4) {
				if (m_pEquipData->bUseTopAng) {
					gData.nTop1Cnt = 0;
					n1Tno1 = gData.InfoVision[nS1No-1][9];
					n1Tno5 = gData.InfoVision[nS1No-1][9] + 4;

					g_objCommon.Move_Position(AX_TOP1_VISION_Z, 2);
					g_objCommon.Move_Position(AX_TOP1_LIGHT_Z, 2);
					g_objCommon.Move_Position(AX_INSPECT_STAGE_X1, 1);

					m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
				} else {
					m_nVisionStage1Case = 12; m_tVisionStage1Loop.Set_LoopTime(5000);
				}
			} else {
				m_nVisionStage1Case = 5; m_tVisionStage1Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 9:	//Top1-Angle
		if (g_objCommon.Check_Position(AX_TOP1_VISION_Z, 2) && g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 2) ) {
			g_objCommon.Save_Motion(AX_TOP1_VISION_Z, 2);	g_objCommon.Save_Motion(AX_TOP1_LIGHT_Z, 2);
			g_objCommon.Move_Position(AX_TOP1_ANGLE_Y, 2); gData.nScanRetry[1] = 0;
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(30000);
		}
		break;
	case 10:	//Top1-Angle
		if (m_pDX07->iInspectStage1Up && !m_pDX07->iInspectStage1Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				m_tVisionStage1Loop.Takt_Save(14, 5); m_tVisionStage1Loop.Takt_Start(); 
				dStage1PosX = m_pMoveData->dVisionStageX1[1] + (m_pEquipData->dPitchTop * gData.nTop1Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X1, dStage1PosX);
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 2);	nV1Retry = 0;
				m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X1, dStage1PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 2) && g_objCommon.Check_Position(AX_TOP1_VISION_Z, 2)) {
			if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 2) && g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				if (!m_tVisionStage1Loop.Waiting_Time(m_pEquipData->nDelayTime[9])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X1, -1, dStage1PosX);	g_objCommon.Save_Motion(AX_TOP1_ANGLE_Y, 2);
				m_tVisionStage1Loop.Takt_Save(14, 6); m_tVisionStage1Loop.Takt_Start(); 
				gData.bTop1ScanDone = FALSE;	gData.nVsNo[0] = nS1No; gData.nVsIx[0] = gData.nTop1Cnt+1;
				int nModule1 = n1Tno1; if (gData.InfoVision[nS1No-1][gData.nTop1Cnt] < 1)   nModule1 = -1;
				int nModule5 = n1Tno5; if (gData.InfoVision[nS1No-1][gData.nTop1Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "TG", gData.sLotID_VisionStage[nS1No-1], gData.nPortNo_VisionStage[nS1No-1], gData.nTrayNo_VisionStage[nS1No-1], nModule1, gData.nTrayNo_VisionStage[nS1No-1], nModule5);
				gAlm.sAlmLID[2] = gData.sLotID_VisionStage[nS1No-1]; gAlm.nAlmTNo[2] = gData.nTrayNo_VisionStage[nS1No-1]; gAlm.nAlmPNo[2] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[2] = nModule5;
				m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 12:
		if (gData.bTop1ScanDone || !m_pEquipData->bUseTopAng) {
			m_tVisionStage1Loop.Takt_Save(14, 7); m_tVisionStage1Loop.Takt_Start(); 
			gData.nTop1Cnt++; n1Tno1++; n1Tno5++;
			if (gData.nTop1Cnt >= 4) {
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);
				g_objCommon.Move_Position(AX_TOP1_ANGLE_Y, 1);
				m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
			} else {
				m_nVisionStage1Case = 10; m_tVisionStage1Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 13:
		if (n1FrontVisionCase > 22) {
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 14:
		if (m_pDX07->iInspectStage1Up && !m_pDX07->iInspectStage1Down) {
			if (m_pDX07->iInspectStage1Yin && !m_pDX07->iInspectStage1YOut) {
				m_tVisionStage1Loop.Takt_Save(14, 8); m_tVisionStage1Loop.Takt_Start(); 
				m_pDY07->oInspectStage1Deg0 = FALSE; m_pDY07->oInspectStage1Deg90 = TRUE;
				g_objAJinAXL.Write_Output(7);
				g_objCommon.Move_Position(AX_TOP2_VISION_Z, 1);
				g_objCommon.Move_Position(AX_INSPECT_STAGE_X1, 2);
				m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 15:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X1, 2)) {
			if (g_objCommon.Check_Position(AX_TOP2_VISION_Z, 1)) {
				if (m_pDX07->iInspectStage1Deg90) {
					g_objCommon.Save_Motion(AX_INSPECT_STAGE_X1, 2);	g_objCommon.Save_Motion(AX_TOP2_VISION_Z, 1);
					m_tVisionStage1Loop.Takt_Save(14, 9); m_tVisionStage1Loop.Takt_Start(); 
					gData.nVisionPos[1][1] = 0; gData.nVisionPos[1][2] = nS1No;
					gData.nTop2Cnt = gData.nScanRetry[2] = 0;
					n1Tno1 = gData.InfoVision[nS1No-1][9];
					n1Tno5 = gData.InfoVision[nS1No-1][9] + 4;
					m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(20000);
				}
			}
		}
		break;
	case 16:
		if (m_pDX07->iInspectStage1Up && !m_pDX07->iInspectStage1Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP2_VISION_Z)) {
				m_tVisionStage1Loop.Takt_Save(14, 10); m_tVisionStage1Loop.Takt_Start(); 
				dStage1PosX = m_pMoveData->dVisionStageX1[2] + (m_pEquipData->dPitchTop * gData.nTop2Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X1, dStage1PosX);
				g_objCommon.Move_Position(AX_TOP2_VISION_Z, 1);	nV1Retry = 0;
				m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X1, dStage1PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP2_VISION_Z, 1)) {
			if (g_objAJinAXL.Is_Done(AX_TOP2_VISION_Z)) {
				if (!m_tVisionStage1Loop.Waiting_Time(m_pEquipData->nDelayTime[9])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X1, -1, dStage1PosX);
				m_tVisionStage1Loop.Takt_Save(14, 11); m_tVisionStage1Loop.Takt_Start(); 
				gData.bTop2ScanDone = FALSE;	gData.nVsNo[1] = nS1No; gData.nVsIx[1] = gData.nTop2Cnt+1;
				int nModule1 = n1Tno1; if (gData.InfoVision[nS1No-1][gData.nTop2Cnt] < 1)   nModule1 = -1;
				int nModule5 = n1Tno5; if (gData.InfoVision[nS1No-1][gData.nTop2Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC3, "T2", gData.sLotID_VisionStage[nS1No-1], gData.nPortNo_VisionStage[nS1No-1], gData.nTrayNo_VisionStage[nS1No-1], nModule1, gData.nTrayNo_VisionStage[nS1No-1], nModule5);
				gAlm.sAlmLID[3] = gData.sLotID_VisionStage[nS1No-1]; gAlm.nAlmTNo[3] = gData.nTrayNo_VisionStage[nS1No-1]; gAlm.nAlmPNo[3] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[3] = nModule5;
				m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 18:
		if (gData.bTop2ScanDone || !m_pEquipData->bUseTop2) {
			m_tVisionStage1Loop.Takt_Save(14, 12); m_tVisionStage1Loop.Takt_Start(); 
			gData.nTop2Cnt++; n1Tno1++; n1Tno5++;
			if (gData.nTop2Cnt >= 4) {
				dwV1RosTime[0] = GetTickCount();
				m_nVisionStage1Case = 20; m_tVisionStage1Loop.Set_LoopTime(5000);
			} else {
				m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 19:	//Buffer에 있으면 Scan 1번하고 대기
		if (n1FrontVisionCase < 19 || (n1FrontVisionCase >= 23 && gData.nTop2Cnt <= 1) || n1FrontVisionCase >= 25) {
			m_nVisionStage1Case = 16; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 20:
		if (n1FrontVisionCase < 20 || n1FrontVisionCase >= 25) {
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 21:
		if (m_pDX07->iInspectStage1Up && !m_pDX07->iInspectStage1Down) {
			if (m_pDX07->iInspectStage1Yin && !m_pDX07->iInspectStage1YOut) {
				if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X1, dStage1PosX)) {
					m_tVisionStage1Loop.Takt_Save(14, 13); m_tVisionStage1Loop.Takt_Start(); 
					g_objCommon.Move_Position(AX_INSPECT_STAGE_X1, 4);
					m_pDY07->oInspectStage1Deg0 = TRUE; m_pDY07->oInspectStage1Deg90 = FALSE;
					g_objAJinAXL.Write_Output(7);
					m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 22:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X1, 4)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X1, 4);
			m_tVisionStage1Loop.Takt_Save(14, 14); m_tVisionStage1Loop.Takt_Start(); 
			dwV1RosTime[1] = dwV1RosTime[0];
			gData.nVisionPos[1][2] = 0; gData.nVisionPos[1][4] = nS1No;
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 23:
		if (n1FrontVisionCase < 22 || n1FrontVisionCase > 34) {
			m_tVisionStage1Loop.Takt_Save(14, 15); m_tVisionStage1Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X1, 3);
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 24:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X1, 3)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X1, 3);
			m_tVisionStage1Loop.Takt_Save(14, 16); m_tVisionStage1Loop.Takt_Start(); 
			dwV1RosTime[2] = dwV1RosTime[1];
			gData.nVisionPos[1][4] = 0; gData.nVisionPos[1][3] = nS1No;
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(30000);
		}
		break;
	case 25:
			m_nVisionStage1Case = 30; m_tVisionStage1Loop.Set_LoopTime(30000);
		break;
	case 26:	//RosTime 6초대기
		if (GetTickCount() - dwV1RosTime[2] >= 8000) {
			m_nVisionStage1Case = 30; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 30:	//Unload Picker wait
		m_tVisionStage1Loop.Set_LoopTime(5000);
		return TRUE;

	case 31:
		if (m_pDX07->iInspectStage1Up && !m_pDX07->iInspectStage1Down) {
			if (m_pDX07->iInspectStage1Yin && !m_pDX07->iInspectStage1YOut) {
				m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 32:
		if (n1FrontVisionCase < 30 || n1FrontVisionCase >= 50) {
			m_tVisionStage1Loop.Takt_Save(14, 17); m_tVisionStage1Loop.Takt_Start(); 
			m_pDY07->oInspectStage1Up = FALSE; m_pDY07->oInspectStage1Down = TRUE;
			g_objAJinAXL.Write_Output(7);
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 33:
		if (!m_pDX07->iInspectStage1Up && m_pDX07->iInspectStage1Down) {
			m_tVisionStage1Loop.Takt_Save(14, 18); m_tVisionStage1Loop.Takt_Start(); 
			gData.nVisionPos[1][3] = 0; gData.nVisionPos[0][3] = nS1No;
			if (nS1No == 1) {
				m_pDY07->oInspectStage1Yin = FALSE; m_pDY07->oInspectStage1YOut = TRUE;
				g_objAJinAXL.Write_Output(7);
			}
			if (nS1No == 4) {
				m_pDY08->oInspectStage4Yin = FALSE; m_pDY08->oInspectStage4YOut = TRUE;
				g_objAJinAXL.Write_Output(8);
			}
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 34:
		if ((nS1No == 1 && !m_pDX07->iInspectStage1Yin && m_pDX07->iInspectStage1YOut) ||
			(nS1No == 2) || (nS1No == 3) ||
			(nS1No == 4 && !m_pDX08->iInspectStage4Yin && m_pDX08->iInspectStage4YOut) ) {
			m_tVisionStage1Loop.Takt_Save(14, 19); m_tVisionStage1Loop.Takt_Start(); 
			m_nVisionStage1Case = 40; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 40:
		if (n1FrontVisionCase < 40 || n1FrontVisionCase >= 60) {
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 41:
		if (!m_pDX07->iInspectStage1Up && m_pDX07->iInspectStage1Down) {
			m_tVisionStage1Loop.Takt_Save(14, 20); m_tVisionStage1Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X1, 2);
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X1, 2)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X1, 2);
			m_tVisionStage1Loop.Takt_Save(14, 21); m_tVisionStage1Loop.Takt_Start(); 
			gData.nVisionPos[0][3] = 0; gData.nVisionPos[0][2] = nS1No;
			m_nVisionStage1Case = 50; m_tVisionStage1Loop.Set_LoopTime(60000);
		}
		break;

	case 50:
		if (n1FrontVisionCase < 50 || n1FrontVisionCase >= 70) {
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 51:
		if (!m_pDX07->iInspectStage1Up && m_pDX07->iInspectStage1Down) {
			m_tVisionStage1Loop.Takt_Save(14, 22); m_tVisionStage1Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X1, 1);
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 52:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X1, 1)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X1, 1);
			m_tVisionStage1Loop.Takt_Save(14, 23); m_tVisionStage1Loop.Takt_Start(); 
			gData.nVisionPos[0][2] = 0; gData.nVisionPos[0][1] = nS1No;
			m_nVisionStage1Case = 60; m_tVisionStage1Loop.Set_LoopTime(60000);
		}
		break;

	case 60:
		if (n1FrontVisionCase >= 0 && n1FrontVisionCase < 50) {
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 61:
		if (!m_pDX07->iInspectStage1Up && m_pDX07->iInspectStage1Down) {
			m_tVisionStage1Loop.Takt_Save(14, 24); m_tVisionStage1Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X1, 0);
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 62:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X1, 0)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X1, 0);
			m_tVisionStage1Loop.Takt_Save(14, 25); m_tVisionStage1Loop.Takt_Start(); 
			gData.nVisionPos[0][1] = 0; gData.nVisionPos[0][0] = nS1No;
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(10000);
		}
		break;
	case 63:
		if (!m_pDX07->iInspectStage1Up && m_pDX07->iInspectStage1Down) {
/*			if (nS1No == 1) {
				m_pDY07->oInspectStage1Yin = TRUE; m_pDY07->oInspectStage1YOut = FALSE;
				g_objAJinAXL.Write_Output(7);
			}
			if (nS1No == 4) {
				m_pDY08->oInspectStage4Yin = TRUE; m_pDY08->oInspectStage4YOut = FALSE;
				g_objAJinAXL.Write_Output(8);
			}*/
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 64:
//		if ((nS1No == 1 && m_pDX07->iInspectStage1Yin && !m_pDX07->iInspectStage1YOut) ||
//			(nS1No == 2) || (nS1No == 3) ||
//			(nS1No == 4 && m_pDX08->iInspectStage4Yin && !m_pDX08->iInspectStage4YOut) ) {
			m_nVisionStage1Case = 70; m_tVisionStage1Loop.Set_LoopTime(5000);
//		}
		break;

	case 70:
		if (n1FrontVisionCase > 4 && n1FrontVisionCase < 60) {
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 71:
		if (!m_pDX07->iInspectStage1Up && m_pDX07->iInspectStage1Down) {
			m_tVisionStage1Loop.Takt_Save(14, 26); m_tVisionStage1Loop.Takt_Start(); 
			if (nS1No == 1) {
				m_pDY07->oInspectStage1Yin = TRUE; m_pDY07->oInspectStage1YOut = FALSE;
				g_objAJinAXL.Write_Output(7);
			}
			if (nS1No == 4) {
				m_pDY08->oInspectStage4Yin = TRUE; m_pDY08->oInspectStage4YOut = FALSE;
				g_objAJinAXL.Write_Output(8);
			}
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 72:
		if (m_pDX07->iInspectStage1Yin && !m_pDX07->iInspectStage1YOut) {
			m_tVisionStage1Loop.Takt_Save(14, 27); m_tVisionStage1Loop.Takt_Start(); 
			m_pDY07->oInspectStage1Up = TRUE; m_pDY07->oInspectStage1Down = FALSE;
			g_objAJinAXL.Write_Output(7);
			m_nVisionStage1Case++; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 73:
		if (m_pDX07->iInspectStage1Up && !m_pDX07->iInspectStage1Down) {
			m_tVisionStage1Loop.Takt_Save(14, 28);
			gData.nVisionPos[0][0] = 0; gData.nVisionPos[1][0] = nS1No;
			m_nVisionStage1Case = 0; m_tVisionStage1Loop.Set_LoopTime(5000);
		}
		break;

	}

	// 14. (Error : 6000)
	if (m_tVisionStage1Loop.Over_LoopTime()) {
//		if (m_nVisionStage1Case == 8 || m_nVisionStage1Case == 12 || m_nVisionStage1Case == 18) {
//			nV1Retry++;
//			if (nV1Retry == 1) { m_nVisionStage1Case--; return TRUE; }
//		}
//		if (m_nVisionStage1Case == 7 || m_nVisionStage1Case == 11 || m_nVisionStage1Case == 17) m_nVisionStage1Case++;
		if (m_nVisionStage1Case == 26) { m_nVisionStage1Case = 30; return TRUE; }
		g_objCommon.Show_Error(6000 + m_nVisionStage1Case);
		return FALSE;
	}
	return TRUE;
}

// 15. (Error : 6200)
BOOL CSequenceMain::Run_VisionStage2()
{
	static int nS2No = 2;		//1,2,3,4(고정)
	static int n2Tno1, n2Tno5, n2FrontVisionCase, nV2Retry;
	static double dStage2PosX = 0;
	static DWORD dwV2RosTime[3];	//Top2,Buffer,Unload

	n2FrontVisionCase = m_nVisionStage1Case;
	switch (m_nVisionStage2Case) {
	case 0:	// Wait
		m_tVisionStage2Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (n2FrontVisionCase > 14) {
//		if (n2FrontVisionCase > 15) {
			m_sLog.Format("[Inspection Stage2: Start] Inspection Stage1 Case(%d)", n2FrontVisionCase);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 2:
		if (!m_pDX03->iAlignSlave1In && m_pDX03->iAlignSlave1Out && !m_pDX03->iAlignSlave2In && m_pDX03->iAlignSlave2Out && !m_pDX03->iAlignMasterIn && m_pDX03->iAlignMasterOut) {
			if (m_pDX07->iInspectStage2Up && !m_pDX07->iInspectStage2Down) {
				m_tVisionStage2Loop.Takt_Start(); 
				m_pDY07->oInspectStage2Deg0 = TRUE; m_pDY07->oInspectStage2Deg90 = FALSE;
				g_objAJinAXL.Write_Output(7);
				g_objCommon.Move_Position(AX_INSPECT_STAGE_X2, 1);
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 3:	//Top1-Angle
		if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 1)) {
			g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);
			g_objCommon.Move_Position(AX_TOP1_LIGHT_Z, 1);
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X2, 1)) {
			if (m_pDX07->iInspectStage2Deg0) {
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X2, 1);
				m_tVisionStage2Loop.Takt_Save(15, 1); m_tVisionStage2Loop.Takt_Start(); 
				gData.nVisionPos[1][0] = 0; gData.nVisionPos[1][1] = nS2No;
				gData.nTop1Cnt = gData.nScanRetry[1] = 0;
				n2Tno1 = gData.InfoVision[nS2No-1][9];
				n2Tno5 = gData.InfoVision[nS2No-1][9] + 4;
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 5:
		if (n2FrontVisionCase >= 22 || (n2FrontVisionCase >= 17 && gData.nTop1Cnt <= 1) || (n2FrontVisionCase >= 17 && gData.nTop2Cnt >= gData.nTop1Cnt)) {
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(20000);
		}
		return TRUE;
	case 6:
		if (m_pDX07->iInspectStage2Up && !m_pDX07->iInspectStage2Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				m_tVisionStage2Loop.Takt_Save(15, 2); m_tVisionStage2Loop.Takt_Start(); 
				dStage2PosX = m_pMoveData->dVisionStageX2[1] + (m_pEquipData->dPitchTop * gData.nTop1Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X2, dStage2PosX);
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);	nV2Retry = 0;
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 7:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X2, dStage2PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 1) && g_objCommon.Check_Position(AX_TOP1_VISION_Z, 1)) {
			if (g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 1) && g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				if (!m_tVisionStage2Loop.Waiting_Time(m_pEquipData->nDelayTime[3])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X2, -1, dStage2PosX);	g_objCommon.Save_Motion(AX_TOP1_LIGHT_Z, 1);
				m_tVisionStage2Loop.Takt_Save(15, 3); m_tVisionStage2Loop.Takt_Start(); 
				gData.bTop1ScanDone = FALSE;	gData.nVsNo[0] = nS2No; gData.nVsIx[0] = gData.nTop1Cnt+1;
				int nModule1 = n2Tno1; if (gData.InfoVision[nS2No-1][gData.nTop1Cnt] < 1)   nModule1 = -1;
				int nModule5 = n2Tno5; if (gData.InfoVision[nS2No-1][gData.nTop1Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "T1", gData.sLotID_VisionStage[nS2No-1], gData.nPortNo_VisionStage[nS2No-1], gData.nTrayNo_VisionStage[nS2No-1], nModule1, gData.nTrayNo_VisionStage[nS2No-1], nModule5);
				gAlm.sAlmLID[2] = gData.sLotID_VisionStage[nS2No-1]; gAlm.nAlmTNo[2] = gData.nTrayNo_VisionStage[nS2No-1]; gAlm.nAlmPNo[2] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[2] = nModule5;
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 8:
		if (gData.bTop1ScanDone || !m_pEquipData->bUseTop1) {
			m_tVisionStage2Loop.Takt_Save(15, 4); m_tVisionStage2Loop.Takt_Start(); 
			gData.nTop1Cnt++; n2Tno1++; n2Tno5++;
			if (gData.nTop1Cnt >= 4) {
				if (m_pEquipData->bUseTopAng) {
					gData.nTop1Cnt = 0;
					n2Tno1 = gData.InfoVision[nS2No-1][9];
					n2Tno5 = gData.InfoVision[nS2No-1][9] + 4;

					g_objCommon.Move_Position(AX_TOP1_VISION_Z, 2);
					g_objCommon.Move_Position(AX_TOP1_LIGHT_Z, 2);
					g_objCommon.Move_Position(AX_INSPECT_STAGE_X2, 1);

					m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(30000);
				} else {
					m_nVisionStage2Case = 12; m_tVisionStage2Loop.Set_LoopTime(5000);
				}
			} else {
				m_nVisionStage2Case = 5; m_tVisionStage2Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 9:	//Top1-Angle
		if (g_objCommon.Check_Position(AX_TOP1_VISION_Z, 2) && g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 2) ) {
			g_objCommon.Save_Motion(AX_TOP1_VISION_Z, 2);	g_objCommon.Save_Motion(AX_TOP1_LIGHT_Z, 2);
			g_objCommon.Move_Position(AX_TOP1_ANGLE_Y, 2); gData.nScanRetry[1] = 0;
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 10:
		if (m_pDX07->iInspectStage2Up && !m_pDX07->iInspectStage2Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				m_tVisionStage2Loop.Takt_Save(15, 5); m_tVisionStage2Loop.Takt_Start(); 
				dStage2PosX = m_pMoveData->dVisionStageX2[1] + (m_pEquipData->dPitchTop * gData.nTop1Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X2, dStage2PosX);
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 2);	nV2Retry = 0;
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X2, dStage2PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 2) && g_objCommon.Check_Position(AX_TOP1_VISION_Z, 2)) {
			if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 2) && g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				if (!m_tVisionStage2Loop.Waiting_Time(m_pEquipData->nDelayTime[9])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X2, -1, dStage2PosX);	g_objCommon.Save_Motion(AX_TOP1_ANGLE_Y, 2);
				m_tVisionStage2Loop.Takt_Save(15, 6); m_tVisionStage2Loop.Takt_Start(); 
				gData.bTop1ScanDone = FALSE;	gData.nVsNo[0] = nS2No; gData.nVsIx[0] = gData.nTop1Cnt+1;
				int nModule1 = n2Tno1; if (gData.InfoVision[nS2No-1][gData.nTop1Cnt] < 1)   nModule1 = -1;
				int nModule5 = n2Tno5; if (gData.InfoVision[nS2No-1][gData.nTop1Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "TG", gData.sLotID_VisionStage[nS2No-1], gData.nPortNo_VisionStage[nS2No-1], gData.nTrayNo_VisionStage[nS2No-1], nModule1, gData.nTrayNo_VisionStage[nS2No-1], nModule5);
				gAlm.sAlmLID[2] = gData.sLotID_VisionStage[nS2No-1]; gAlm.nAlmTNo[2] = gData.nTrayNo_VisionStage[nS2No-1]; gAlm.nAlmPNo[2] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[2] = nModule5;
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 12:
		if (gData.bTop1ScanDone || !m_pEquipData->bUseTopAng) {
			m_tVisionStage2Loop.Takt_Save(15, 7); m_tVisionStage2Loop.Takt_Start(); 
			gData.nTop1Cnt++; n2Tno1++; n2Tno5++;
			if (gData.nTop1Cnt >= 4) {
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);
				g_objCommon.Move_Position(AX_TOP1_ANGLE_Y, 1);
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(30000);
			} else {
				m_nVisionStage2Case = 10; m_tVisionStage2Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 13:
		if (n2FrontVisionCase > 22) {
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 14:
		if (m_pDX07->iInspectStage2Up && !m_pDX07->iInspectStage2Down) {
			m_tVisionStage2Loop.Takt_Save(15, 8); m_tVisionStage2Loop.Takt_Start(); 
			m_pDY07->oInspectStage2Deg0 = FALSE; m_pDY07->oInspectStage2Deg90 = TRUE;
			g_objAJinAXL.Write_Output(7);
			g_objCommon.Move_Position(AX_TOP2_VISION_Z, 1);
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X2, 2);
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 15:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X2, 2)) {
			if (g_objCommon.Check_Position(AX_TOP2_VISION_Z, 1)) {
				if (m_pDX07->iInspectStage2Deg90) {
					g_objCommon.Save_Motion(AX_INSPECT_STAGE_X2, 2);	g_objCommon.Save_Motion(AX_TOP2_VISION_Z, 1);
					m_tVisionStage2Loop.Takt_Save(15, 9); m_tVisionStage2Loop.Takt_Start(); 
					gData.nVisionPos[1][1] = 0; gData.nVisionPos[1][2] = nS2No;
					gData.nTop2Cnt = gData.nScanRetry[2] = 0;
					n2Tno1 = gData.InfoVision[nS2No-1][9];
					n2Tno5 = gData.InfoVision[nS2No-1][9] + 4;
					m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(20000);
				}
			}
		}
		break;
	case 16:
		if (m_pDX07->iInspectStage2Up && !m_pDX07->iInspectStage2Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP2_VISION_Z)) {
				m_tVisionStage2Loop.Takt_Save(15, 10); m_tVisionStage2Loop.Takt_Start(); 
				dStage2PosX = m_pMoveData->dVisionStageX2[2] + (m_pEquipData->dPitchTop * gData.nTop2Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X2, dStage2PosX);
				g_objCommon.Move_Position(AX_TOP2_VISION_Z, 1);	nV2Retry = 0;
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X2, dStage2PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP2_VISION_Z, 1)) {
			if (g_objAJinAXL.Is_Done(AX_TOP2_VISION_Z)) {
				if (!m_tVisionStage2Loop.Waiting_Time(m_pEquipData->nDelayTime[9])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X2, -1, dStage2PosX);
				m_tVisionStage2Loop.Takt_Save(15, 11); m_tVisionStage2Loop.Takt_Start(); 
				gData.bTop2ScanDone = FALSE;	gData.nVsNo[1] = nS2No; gData.nVsIx[1] = gData.nTop2Cnt+1;
				int nModule1 = n2Tno1; if (gData.InfoVision[nS2No-1][gData.nTop2Cnt] < 1)   nModule1 = -1;
				int nModule5 = n2Tno5; if (gData.InfoVision[nS2No-1][gData.nTop2Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC3, "T2", gData.sLotID_VisionStage[nS2No-1], gData.nPortNo_VisionStage[nS2No-1], gData.nTrayNo_VisionStage[nS2No-1], nModule1, gData.nTrayNo_VisionStage[nS2No-1], nModule5);
				gAlm.sAlmLID[3] = gData.sLotID_VisionStage[nS2No-1]; gAlm.nAlmTNo[3] = gData.nTrayNo_VisionStage[nS2No-1]; gAlm.nAlmPNo[3] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[3] = nModule5;
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 18:
		if (gData.bTop2ScanDone || !m_pEquipData->bUseTop2) {
			m_tVisionStage2Loop.Takt_Save(15, 12); m_tVisionStage2Loop.Takt_Start(); 
			gData.nTop2Cnt++; n2Tno1++; n2Tno5++;
			if (gData.nTop2Cnt >= 4) {
				dwV2RosTime[0] = GetTickCount();
				m_nVisionStage2Case = 20; m_tVisionStage2Loop.Set_LoopTime(5000);
			} else {
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 19:
		if (n2FrontVisionCase < 19 || (n2FrontVisionCase >= 23 && gData.nTop2Cnt <= 1) || n2FrontVisionCase >= 25) {
			m_nVisionStage2Case = 16; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 20:
		if (n2FrontVisionCase < 20 || n2FrontVisionCase >= 25) {
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 21:
		if (m_pDX07->iInspectStage2Up && !m_pDX07->iInspectStage2Down) {
			if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X2, dStage2PosX)) {
				m_tVisionStage2Loop.Takt_Save(15, 13); m_tVisionStage2Loop.Takt_Start(); 
				g_objCommon.Move_Position(AX_INSPECT_STAGE_X2, 4);
				m_pDY07->oInspectStage2Deg0 = TRUE; m_pDY07->oInspectStage2Deg90 = FALSE;
				g_objAJinAXL.Write_Output(7);
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 22:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X2, 4)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X2, 4);
			m_tVisionStage2Loop.Takt_Save(15, 14); m_tVisionStage2Loop.Takt_Start(); 
			dwV2RosTime[1] = dwV2RosTime[0];
			gData.nVisionPos[1][2] = 0; gData.nVisionPos[1][4] = nS2No;
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 23:
		if (n2FrontVisionCase < 22 || n2FrontVisionCase > 34) {
			m_tVisionStage2Loop.Takt_Save(15, 15); m_tVisionStage2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X2, 3);
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 24:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X2, 3)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X2, 3);
			m_tVisionStage2Loop.Takt_Save(15, 16); m_tVisionStage2Loop.Takt_Start(); 
			dwV2RosTime[2] = dwV2RosTime[1];
			gData.nVisionPos[1][4] = 0; gData.nVisionPos[1][3] = nS2No;
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 25:
			m_nVisionStage2Case = 30; m_tVisionStage2Loop.Set_LoopTime(30000);
		break;
	case 26:	//RosTime 6초대기
		if (GetTickCount() - dwV2RosTime[2] >= 8000) {
			m_nVisionStage2Case = 30; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 30:	//Unload Picker wait
		m_tVisionStage2Loop.Set_LoopTime(5000);
		return TRUE;

	case 31:
		if (m_pDX07->iInspectStage2Up && !m_pDX07->iInspectStage2Down) {
//			if (m_pDX07->iInspectStage2Yin && !m_pDX07->iInspectStage2YOut) {
				m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(10000);
//			}
		}
		break;
	case 32:
		if (n2FrontVisionCase < 30 || n2FrontVisionCase >= 50) {
			m_tVisionStage2Loop.Takt_Save(15, 17); m_tVisionStage2Loop.Takt_Start(); 
			m_pDY07->oInspectStage2Up = FALSE; m_pDY07->oInspectStage2Down = TRUE;
			g_objAJinAXL.Write_Output(7);
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 33:
		if (!m_pDX07->iInspectStage2Up && m_pDX07->iInspectStage2Down) {
			m_tVisionStage2Loop.Takt_Save(15, 18); m_tVisionStage2Loop.Takt_Start(); 
			gData.nVisionPos[1][3] = 0; gData.nVisionPos[0][3] = nS2No;
//			if (nS2No == 1) {
//				m_pDY07->oInspectStage2Yin = FALSE; m_pDY07->oInspectStage2YOut = TRUE;
//				g_objAJinAXL.Write_Output(7);
//			}
//			if (nS2No == 4) {
//				m_pDY08->oInspectStage4Yin = FALSE; m_pDY08->oInspectStage4YOut = TRUE;
//				g_objAJinAXL.Write_Output(8);
//			}
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 34:
		if (
			(nS2No == 2) || (nS2No == 3) ||
			(nS2No == 4 && !m_pDX08->iInspectStage4Yin && m_pDX08->iInspectStage4YOut) ) {
			m_tVisionStage2Loop.Takt_Save(15, 19); m_tVisionStage2Loop.Takt_Start(); 
			m_nVisionStage2Case = 40; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 40:
		if (n2FrontVisionCase < 40 || n2FrontVisionCase >= 60) {
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 41:
		if (!m_pDX07->iInspectStage2Up && m_pDX07->iInspectStage2Down) {
			m_tVisionStage2Loop.Takt_Save(15, 20); m_tVisionStage2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X2, 2);
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X2, 2)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X2, 2);
			m_tVisionStage2Loop.Takt_Save(15, 21); m_tVisionStage2Loop.Takt_Start(); 
			gData.nVisionPos[0][3] = 0; gData.nVisionPos[0][2] = nS2No;
			m_nVisionStage2Case = 50; m_tVisionStage2Loop.Set_LoopTime(60000);
		}
		break;

	case 50:
		if (n2FrontVisionCase < 50 || n2FrontVisionCase >= 70) {
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 51:
		if (!m_pDX07->iInspectStage2Up && m_pDX07->iInspectStage2Down) {
			m_tVisionStage2Loop.Takt_Save(15, 22); m_tVisionStage2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X2, 1);
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 52:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X2, 1)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X2, 1);
			m_tVisionStage2Loop.Takt_Save(15, 23); m_tVisionStage2Loop.Takt_Start(); 
			gData.nVisionPos[0][2] = 0; gData.nVisionPos[0][1] = nS2No;
			m_nVisionStage2Case = 60; m_tVisionStage2Loop.Set_LoopTime(60000);
		}
		break;

	case 60:
		if (n2FrontVisionCase >= 0 && n2FrontVisionCase < 50) {
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 61:
		if (!m_pDX07->iInspectStage2Up && m_pDX07->iInspectStage2Down) {
			m_tVisionStage2Loop.Takt_Save(15, 24); m_tVisionStage2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X2, 0);
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 62:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X2, 0)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X2, 0);
			m_tVisionStage2Loop.Takt_Save(15, 25); m_tVisionStage2Loop.Takt_Start(); 
			gData.nVisionPos[0][1] = 0; gData.nVisionPos[0][0] = nS2No;
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(10000);
		}
		break;
	case 63:
		if (!m_pDX07->iInspectStage2Up && m_pDX07->iInspectStage2Down) {
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 64:
//		if (
//			(nS2No == 2) || (nS2No == 3) ||
//			(nS2No == 4 && m_pDX08->iInspectStage4Yin && !m_pDX08->iInspectStage4YOut) ) {
			m_nVisionStage2Case = 70; m_tVisionStage2Loop.Set_LoopTime(5000);
//		}
		break;

	case 70:
		if (n2FrontVisionCase > 4 && n2FrontVisionCase < 60) {
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 71:
		if (!m_pDX07->iInspectStage2Up && m_pDX07->iInspectStage2Down) {
			m_tVisionStage2Loop.Takt_Save(15, 26); m_tVisionStage2Loop.Takt_Start(); 
			m_pDY07->oInspectStage2Up = TRUE; m_pDY07->oInspectStage2Down = FALSE;
			g_objAJinAXL.Write_Output(7);
			m_nVisionStage2Case++; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 72:
		if (m_pDX07->iInspectStage2Up && !m_pDX07->iInspectStage2Down) {
			m_tVisionStage2Loop.Takt_Save(15, 27);
			gData.nVisionPos[0][0] = 0; gData.nVisionPos[1][0] = nS2No;
			m_nVisionStage2Case = 0; m_tVisionStage2Loop.Set_LoopTime(5000);
		}
		break;

	}

	// 15. (Error : 6200)
	if (m_tVisionStage2Loop.Over_LoopTime()) {
//		if (m_nVisionStage2Case == 8 || m_nVisionStage2Case == 12 || m_nVisionStage2Case == 18) {
//			nV2Retry++;
//			if (nV2Retry == 1) { m_nVisionStage2Case--; return TRUE; }
//		}
//		if (m_nVisionStage2Case == 7 || m_nVisionStage2Case == 11 || m_nVisionStage2Case == 17) m_nVisionStage2Case++;
		if (m_nVisionStage2Case == 26) { m_nVisionStage2Case = 30; return TRUE; }
		g_objCommon.Show_Error(6200 + m_nVisionStage2Case);
		return FALSE;
	}
	return TRUE;
}

// 16. (Error : 6400)
BOOL CSequenceMain::Run_VisionStage3()
{
	static int nS3No = 3;		//1,2,3,4(고정)
	static int n3Tno1, n3Tno5, n3FrontVisionCase, nV3Retry;
	static double dStage3PosX = 0;
	static DWORD dwV3RosTime[3];	//Top2,Buffer,Unload

	n3FrontVisionCase = m_nVisionStage4Case;
	switch (m_nVisionStage3Case) {
	case 0:	// Wait
		m_tVisionStage3Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (n3FrontVisionCase > 14) {
//		if (n3FrontVisionCase > 15) {
			m_sLog.Format("[Inspection Stage3: Start] Inspection Stage4 Case(%d)", n3FrontVisionCase);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 2:
		if (!m_pDX03->iAlignSlave1In && m_pDX03->iAlignSlave1Out && !m_pDX03->iAlignSlave2In && m_pDX03->iAlignSlave2Out && !m_pDX03->iAlignMasterIn && m_pDX03->iAlignMasterOut) {
			if (m_pDX08->iInspectStage3Up && !m_pDX08->iInspectStage3Down) {
				m_tVisionStage3Loop.Takt_Start(); 
				m_pDY08->oInspectStage3Deg0 = TRUE; m_pDY08->oInspectStage3Deg90 = FALSE;
				g_objAJinAXL.Write_Output(8);
				g_objCommon.Move_Position(AX_INSPECT_STAGE_X3, 1);
				m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 3:	//Top1-Angle
		if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 1)) {
			g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);
			g_objCommon.Move_Position(AX_TOP1_LIGHT_Z, 1);
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X3, 1)) {
			if (m_pDX08->iInspectStage3Deg0) {
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X3, 1);
				m_tVisionStage3Loop.Takt_Save(16, 1); m_tVisionStage3Loop.Takt_Start(); 
				gData.nVisionPos[1][0] = 0; gData.nVisionPos[1][1] = nS3No;
				gData.nTop1Cnt = gData.nScanRetry[1] = 0;
				n3Tno1 = gData.InfoVision[nS3No-1][9];
				n3Tno5 = gData.InfoVision[nS3No-1][9] + 4;
				m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 5:
		if (n3FrontVisionCase >= 22 || (n3FrontVisionCase >= 17 && gData.nTop1Cnt <= 1) || (n3FrontVisionCase >= 17 && gData.nTop2Cnt >= gData.nTop1Cnt)) {
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(20000);
		}
		return TRUE;
	case 6:
		if (m_pDX08->iInspectStage3Up && !m_pDX08->iInspectStage3Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				m_tVisionStage3Loop.Takt_Save(16, 2); m_tVisionStage3Loop.Takt_Start(); 
				dStage3PosX = m_pMoveData->dVisionStageX3[1] + (m_pEquipData->dPitchTop * gData.nTop1Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X3, dStage3PosX);
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);	nV3Retry = 0;
				m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 7:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X3, dStage3PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 1) && g_objCommon.Check_Position(AX_TOP1_VISION_Z, 1)) {
			if (g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 1) && g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				if (!m_tVisionStage3Loop.Waiting_Time(m_pEquipData->nDelayTime[3])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X3, -1, dStage3PosX);	g_objCommon.Save_Motion(AX_TOP1_LIGHT_Z, 1);
				m_tVisionStage3Loop.Takt_Save(16, 3); m_tVisionStage3Loop.Takt_Start(); 
				gData.bTop1ScanDone = FALSE;	gData.nVsNo[0] = nS3No; gData.nVsIx[0] = gData.nTop1Cnt+1;
				int nModule1 = n3Tno1; if (gData.InfoVision[nS3No-1][gData.nTop1Cnt] < 1)   nModule1 = -1;
				int nModule5 = n3Tno5; if (gData.InfoVision[nS3No-1][gData.nTop1Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "T1", gData.sLotID_VisionStage[nS3No-1], gData.nPortNo_VisionStage[nS3No-1], gData.nTrayNo_VisionStage[nS3No-1], nModule1, gData.nTrayNo_VisionStage[nS3No-1], nModule5);
				gAlm.sAlmLID[2] = gData.sLotID_VisionStage[nS3No-1]; gAlm.nAlmTNo[2] = gData.nTrayNo_VisionStage[nS3No-1]; gAlm.nAlmPNo[2] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[2] = nModule5;
				m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 8:
		if (gData.bTop1ScanDone || !m_pEquipData->bUseTop1) {
			m_tVisionStage3Loop.Takt_Save(16, 4); m_tVisionStage3Loop.Takt_Start(); 
			gData.nTop1Cnt++; n3Tno1++; n3Tno5++;
			if (gData.nTop1Cnt >= 4) {
				if (m_pEquipData->bUseTopAng) {
					gData.nTop1Cnt = 0;
					n3Tno1 = gData.InfoVision[nS3No-1][9];
					n3Tno5 = gData.InfoVision[nS3No-1][9] + 4;

					g_objCommon.Move_Position(AX_TOP1_VISION_Z, 2);
					g_objCommon.Move_Position(AX_TOP1_LIGHT_Z, 2);
					g_objCommon.Move_Position(AX_INSPECT_STAGE_X3, 1);

					m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(30000);
				} else {
					m_nVisionStage3Case = 12; m_tVisionStage3Loop.Set_LoopTime(5000);
				}
			} else {
				m_nVisionStage3Case = 5; m_tVisionStage3Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 9:	//Top1-Angle
		if (g_objCommon.Check_Position(AX_TOP1_VISION_Z, 2) && g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 2) ) {
			g_objCommon.Save_Motion(AX_TOP1_VISION_Z, 2);	g_objCommon.Save_Motion(AX_TOP1_LIGHT_Z, 2);
			g_objCommon.Move_Position(AX_TOP1_ANGLE_Y, 2); gData.nScanRetry[1] = 0;
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(30000);
		}
		break;
	case 10:
		if (m_pDX08->iInspectStage3Up && !m_pDX08->iInspectStage3Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				m_tVisionStage3Loop.Takt_Save(16, 5); m_tVisionStage3Loop.Takt_Start(); 
				dStage3PosX = m_pMoveData->dVisionStageX3[1] + (m_pEquipData->dPitchTop * gData.nTop1Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X3, dStage3PosX);
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 2);	nV3Retry = 0;
				m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X3, dStage3PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 2) && g_objCommon.Check_Position(AX_TOP1_VISION_Z, 2)) {
			if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 2) && g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				if (!m_tVisionStage3Loop.Waiting_Time(m_pEquipData->nDelayTime[9])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X3, -1, dStage3PosX);	g_objCommon.Save_Motion(AX_TOP1_ANGLE_Y, 2);
				m_tVisionStage3Loop.Takt_Save(16, 6); m_tVisionStage3Loop.Takt_Start(); 
				gData.bTop1ScanDone = FALSE;	gData.nVsNo[0] = nS3No; gData.nVsIx[0] = gData.nTop1Cnt+1;
				int nModule1 = n3Tno1; if (gData.InfoVision[nS3No-1][gData.nTop1Cnt] < 1)   nModule1 = -1;
				int nModule5 = n3Tno5; if (gData.InfoVision[nS3No-1][gData.nTop1Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "TG", gData.sLotID_VisionStage[nS3No-1], gData.nPortNo_VisionStage[nS3No-1], gData.nTrayNo_VisionStage[nS3No-1], nModule1, gData.nTrayNo_VisionStage[nS3No-1], nModule5);
				gAlm.sAlmLID[2] = gData.sLotID_VisionStage[nS3No-1]; gAlm.nAlmTNo[2] = gData.nTrayNo_VisionStage[nS3No-1]; gAlm.nAlmPNo[2] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[2] = nModule5;
				m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 12:
		if (gData.bTop1ScanDone || !m_pEquipData->bUseTopAng) {
			m_tVisionStage3Loop.Takt_Save(16, 7); m_tVisionStage3Loop.Takt_Start(); 
			gData.nTop1Cnt++; n3Tno1++; n3Tno5++;
			if (gData.nTop1Cnt >= 4) {
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);
				g_objCommon.Move_Position(AX_TOP1_ANGLE_Y, 1);
				m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(30000);
			} else {
				m_nVisionStage3Case = 10; m_tVisionStage3Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 13:
		if (n3FrontVisionCase > 22) {
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 14:
		if (m_pDX08->iInspectStage3Up && !m_pDX08->iInspectStage3Down) {
			m_tVisionStage3Loop.Takt_Save(16, 8); m_tVisionStage3Loop.Takt_Start(); 
			m_pDY08->oInspectStage3Deg0 = FALSE; m_pDY08->oInspectStage3Deg90 = TRUE;
			g_objAJinAXL.Write_Output(8);
			g_objCommon.Move_Position(AX_TOP2_VISION_Z, 1);
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X3, 2);
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(10000);
		}
		break;
	case 15:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X3, 2)) {
			if (g_objCommon.Check_Position(AX_TOP2_VISION_Z, 1)) {
				if (m_pDX08->iInspectStage3Deg90) {
					g_objCommon.Save_Motion(AX_INSPECT_STAGE_X3, 2);	g_objCommon.Save_Motion(AX_TOP2_VISION_Z, 1);
					m_tVisionStage3Loop.Takt_Save(16, 9); m_tVisionStage3Loop.Takt_Start(); 
					gData.nVisionPos[1][1] = 0; gData.nVisionPos[1][2] = nS3No;
					gData.nTop2Cnt = gData.nScanRetry[2] = 0;
					n3Tno1 = gData.InfoVision[nS3No-1][9];
					n3Tno5 = gData.InfoVision[nS3No-1][9] + 4;
					m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(20000);
				}
			}
		}
		break;
	case 16:
		if (m_pDX08->iInspectStage3Up && !m_pDX08->iInspectStage3Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP2_VISION_Z)) {
				m_tVisionStage3Loop.Takt_Save(16, 10); m_tVisionStage3Loop.Takt_Start(); 
				dStage3PosX = m_pMoveData->dVisionStageX3[2] + (m_pEquipData->dPitchTop * gData.nTop2Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X3, dStage3PosX);
				g_objCommon.Move_Position(AX_TOP2_VISION_Z, 1);	nV3Retry = 0;
				m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X3, dStage3PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP2_VISION_Z, 1)) {
			if (g_objAJinAXL.Is_Done(AX_TOP2_VISION_Z)) {
				if (!m_tVisionStage3Loop.Waiting_Time(m_pEquipData->nDelayTime[9])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X3, -1, dStage3PosX);
				m_tVisionStage3Loop.Takt_Save(16, 11); m_tVisionStage3Loop.Takt_Start(); 
				gData.bTop2ScanDone = FALSE;	gData.nVsNo[1] = nS3No; gData.nVsIx[1] = gData.nTop2Cnt+1;
				int nModule1 = n3Tno1; if (gData.InfoVision[nS3No-1][gData.nTop2Cnt] < 1)   nModule1 = -1;
				int nModule5 = n3Tno5; if (gData.InfoVision[nS3No-1][gData.nTop2Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC3, "T2", gData.sLotID_VisionStage[nS3No-1], gData.nPortNo_VisionStage[nS3No-1], gData.nTrayNo_VisionStage[nS3No-1], nModule1, gData.nTrayNo_VisionStage[nS3No-1], nModule5);
				gAlm.sAlmLID[3] = gData.sLotID_VisionStage[nS3No-1]; gAlm.nAlmTNo[3] = gData.nTrayNo_VisionStage[nS3No-1]; gAlm.nAlmPNo[3] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[3] = nModule5;
				m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 18:
		if (gData.bTop2ScanDone || !m_pEquipData->bUseTop2) {
			m_tVisionStage3Loop.Takt_Save(16, 12); m_tVisionStage3Loop.Takt_Start(); 
			gData.nTop2Cnt++; n3Tno1++; n3Tno5++;
			if (gData.nTop2Cnt >= 4) {
				dwV3RosTime[0] = GetTickCount();
				m_nVisionStage3Case = 20; m_tVisionStage3Loop.Set_LoopTime(5000);
			} else {
				m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 19:
		if (n3FrontVisionCase < 19 || (n3FrontVisionCase >= 23 && gData.nTop2Cnt <= 1) || n3FrontVisionCase >= 25) {
			m_nVisionStage3Case = 16; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 20:
		if (n3FrontVisionCase < 20 || n3FrontVisionCase >= 25) {
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 21:
		if (m_pDX08->iInspectStage3Up && !m_pDX08->iInspectStage3Down) {
			if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X3, dStage3PosX)) {
				m_tVisionStage3Loop.Takt_Save(16, 13); m_tVisionStage3Loop.Takt_Start(); 
				g_objCommon.Move_Position(AX_INSPECT_STAGE_X3, 4);
				m_pDY08->oInspectStage3Deg0 = TRUE; m_pDY08->oInspectStage3Deg90 = FALSE;
				g_objAJinAXL.Write_Output(8);
				m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 22:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X3, 4)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X3, 4);
			m_tVisionStage3Loop.Takt_Save(16, 14); m_tVisionStage3Loop.Takt_Start(); 
			dwV3RosTime[1] = dwV3RosTime[0];
			gData.nVisionPos[1][2] = 0; gData.nVisionPos[1][4] = nS3No;
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(10000);
		}
		break;
	case 23:
		if (n3FrontVisionCase < 22 || n3FrontVisionCase > 34) {
			m_tVisionStage3Loop.Takt_Save(16, 15); m_tVisionStage3Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X3, 3);
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 24:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X3, 3)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X3, 3);
			m_tVisionStage3Loop.Takt_Save(16, 16); m_tVisionStage3Loop.Takt_Start(); 
			dwV3RosTime[2] = dwV3RosTime[1];
			gData.nVisionPos[1][4] = 0; gData.nVisionPos[1][3] = nS3No;
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(30000);
		}
		break;
	case 25:
			m_nVisionStage3Case = 30; m_tVisionStage3Loop.Set_LoopTime(30000);
		break;
	case 26:	//RosTime 6초대기
		if (GetTickCount() - dwV3RosTime[2] >= 8000) {
			m_nVisionStage3Case = 30; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		break;

	case 30:	//Unload Picker wait
		m_tVisionStage3Loop.Set_LoopTime(5000);
		return TRUE;

	case 31:
		if (m_pDX08->iInspectStage3Up && !m_pDX08->iInspectStage3Down) {
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(10000);
		}
		break;
	case 32:
		if (n3FrontVisionCase < 30 || n3FrontVisionCase >= 50) {
			m_tVisionStage3Loop.Takt_Save(16, 17); m_tVisionStage3Loop.Takt_Start(); 
			m_pDY08->oInspectStage3Up = FALSE; m_pDY08->oInspectStage3Down = TRUE;
			g_objAJinAXL.Write_Output(8);
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 33:
		if (!m_pDX08->iInspectStage3Up && m_pDX08->iInspectStage3Down) {
			m_tVisionStage3Loop.Takt_Save(16, 18); m_tVisionStage3Loop.Takt_Start(); 
			gData.nVisionPos[1][3] = 0; gData.nVisionPos[0][3] = nS3No;
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		break;
	case 34:
		if (
			(nS3No == 2) || (nS3No == 3) ||
			(nS3No == 4 && !m_pDX08->iInspectStage4Yin && m_pDX08->iInspectStage4YOut) ) {
			m_tVisionStage3Loop.Takt_Save(16, 19); m_tVisionStage3Loop.Takt_Start(); 
			m_nVisionStage3Case = 40; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		break;

	case 40:
		if (n3FrontVisionCase < 40 || n3FrontVisionCase >= 60) {
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 41:
		if (!m_pDX08->iInspectStage3Up && m_pDX08->iInspectStage3Down) {
			m_tVisionStage3Loop.Takt_Save(16, 20); m_tVisionStage3Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X3, 2);
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(10000);
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X3, 2)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X3, 2);
			m_tVisionStage3Loop.Takt_Save(16, 21); m_tVisionStage3Loop.Takt_Start(); 
			gData.nVisionPos[0][3] = 0; gData.nVisionPos[0][2] = nS3No;
			m_nVisionStage3Case = 50; m_tVisionStage3Loop.Set_LoopTime(60000);
		}
		break;

	case 50:
		if (n3FrontVisionCase < 50 || n3FrontVisionCase >= 70) {
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 51:
		if (!m_pDX08->iInspectStage3Up && m_pDX08->iInspectStage3Down) {
			m_tVisionStage3Loop.Takt_Save(16, 22); m_tVisionStage3Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X3, 1);
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(10000);
		}
		break;
	case 52:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X3, 1)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X3, 1);
			m_tVisionStage3Loop.Takt_Save(16, 23); m_tVisionStage3Loop.Takt_Start(); 
			gData.nVisionPos[0][2] = 0; gData.nVisionPos[0][1] = nS3No;
			m_nVisionStage3Case = 60; m_tVisionStage3Loop.Set_LoopTime(60000);
		}
		break;

	case 60:
		if (n3FrontVisionCase >= 0 && n3FrontVisionCase < 50) {
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 61:
		if (!m_pDX08->iInspectStage3Up && m_pDX08->iInspectStage3Down) {
			m_tVisionStage3Loop.Takt_Save(16, 24); m_tVisionStage3Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X3, 0);
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(10000);
		}
		break;
	case 62:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X3, 0)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X3, 0);
			m_tVisionStage3Loop.Takt_Save(16, 25); m_tVisionStage3Loop.Takt_Start(); 
			gData.nVisionPos[0][1] = 0; gData.nVisionPos[0][0] = nS3No;
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(10000);
		}
		break;
	case 63:
		if (!m_pDX08->iInspectStage3Up && m_pDX08->iInspectStage3Down) {
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		break;
	case 64:
///		if (
//			(nS3No == 2) || (nS3No == 3) ||
//			(nS3No == 4 && m_pDX08->iInspectStage4Yin && !m_pDX08->iInspectStage4YOut) ) {
			m_nVisionStage3Case = 70; m_tVisionStage3Loop.Set_LoopTime(5000);
//		}
		break;

	case 70:
		if (n3FrontVisionCase > 4 && n3FrontVisionCase < 60) {
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 71:
		if (!m_pDX08->iInspectStage3Up && m_pDX08->iInspectStage3Down) {
			m_tVisionStage3Loop.Takt_Save(16, 26); m_tVisionStage3Loop.Takt_Start(); 
			m_pDY08->oInspectStage3Up = TRUE; m_pDY08->oInspectStage3Down = FALSE;
			g_objAJinAXL.Write_Output(8);
			m_nVisionStage3Case++; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		break;
	case 72:
		if (m_pDX08->iInspectStage3Up && !m_pDX08->iInspectStage3Down) {
			m_tVisionStage3Loop.Takt_Save(16, 27);
			gData.nVisionPos[0][0] = 0; gData.nVisionPos[1][0] = nS3No;
			m_nVisionStage3Case = 0; m_tVisionStage3Loop.Set_LoopTime(5000);
		}
		break;

	}

	// 16. (Error : 6400)
	if (m_tVisionStage3Loop.Over_LoopTime()) {
//		if (m_nVisionStage3Case == 8 || m_nVisionStage3Case == 12 || m_nVisionStage3Case == 18) {
//			nV3Retry++;
//			if (nV3Retry == 1) { m_nVisionStage3Case--; return TRUE; }
//		}
//		if (m_nVisionStage3Case == 7 || m_nVisionStage3Case == 11 || m_nVisionStage3Case == 17) m_nVisionStage3Case++;
		if (m_nVisionStage3Case == 26) { m_nVisionStage3Case = 30; return TRUE; }
		g_objCommon.Show_Error(6400 + m_nVisionStage3Case);
		return FALSE;
	}
	return TRUE;
}

// 17. (Error : 6600)
BOOL CSequenceMain::Run_VisionStage4()
{
	static int nS4No = 4;		//1,2,3,4(고정)
	static int n4Tno1, n4Tno5, n4FrontVisionCase, nV4Retry;
	static double dStage4PosX = 0;
	static DWORD dwV4RosTime[3];	//Top2,Buffer,Unload

	n4FrontVisionCase = m_nVisionStage2Case;
	switch (m_nVisionStage4Case) {
	case 0:	// Wait
		m_tVisionStage4Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (n4FrontVisionCase > 14) {
//		if (n4FrontVisionCase > 15) {
			m_sLog.Format("[Inspection Stage4: Start] Inspection Stage2 Case(%d)", n4FrontVisionCase);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 2:
		if (!m_pDX03->iAlignSlave1In && m_pDX03->iAlignSlave1Out && !m_pDX03->iAlignSlave2In && m_pDX03->iAlignSlave2Out && !m_pDX03->iAlignMasterIn && m_pDX03->iAlignMasterOut) {
			if (m_pDX08->iInspectStage4Up && !m_pDX08->iInspectStage4Down) {
				if (m_pDX08->iInspectStage4Yin && !m_pDX08->iInspectStage4YOut) {
					m_tVisionStage4Loop.Takt_Start(); 
					m_pDY08->oInspectStage4Deg0 = TRUE; m_pDY08->oInspectStage4Deg90 = FALSE;
					g_objAJinAXL.Write_Output(8);
					g_objCommon.Move_Position(AX_INSPECT_STAGE_X4, 1);
					m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 3:	//Top1-Angle
		if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 1)) {
			g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);
			g_objCommon.Move_Position(AX_TOP1_LIGHT_Z, 1);
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X4, 1)) {
			if (m_pDX08->iInspectStage4Deg0) {
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X4, 1);
				m_tVisionStage4Loop.Takt_Save(17, 1); m_tVisionStage4Loop.Takt_Start(); 
				gData.nVisionPos[1][0] = 0; gData.nVisionPos[1][1] = nS4No;
				gData.nTop1Cnt = gData.nScanRetry[1] = 0;
				n4Tno1 = gData.InfoVision[nS4No-1][9];
				n4Tno5 = gData.InfoVision[nS4No-1][9] + 4;
				m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 5:
		if (n4FrontVisionCase >= 22 || (n4FrontVisionCase >= 17 && gData.nTop1Cnt <= 1) || (n4FrontVisionCase >= 17 && gData.nTop2Cnt >= gData.nTop1Cnt)) {
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(20000);
		}
		return TRUE;
	case 6:
		if (m_pDX08->iInspectStage4Up && !m_pDX08->iInspectStage4Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				m_tVisionStage4Loop.Takt_Save(17, 2); m_tVisionStage4Loop.Takt_Start(); 
				dStage4PosX = m_pMoveData->dVisionStageX4[1] + (m_pEquipData->dPitchTop * gData.nTop1Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X4, dStage4PosX);
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);	nV4Retry = 0;
				m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 7:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X4, dStage4PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 1) && g_objCommon.Check_Position(AX_TOP1_VISION_Z, 1)) {
			if (g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 1) && g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				if (!m_tVisionStage4Loop.Waiting_Time(m_pEquipData->nDelayTime[3])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X4, -1, dStage4PosX);	g_objCommon.Save_Motion(AX_TOP1_LIGHT_Z, 1);
				m_tVisionStage4Loop.Takt_Save(17, 3); m_tVisionStage4Loop.Takt_Start(); 
				gData.bTop1ScanDone = FALSE;	gData.nVsNo[0] = nS4No; gData.nVsIx[0] = gData.nTop1Cnt+1;
				int nModule1 = n4Tno1; if (gData.InfoVision[nS4No-1][gData.nTop1Cnt] < 1)   nModule1 = -1;
				int nModule5 = n4Tno5; if (gData.InfoVision[nS4No-1][gData.nTop1Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "T1", gData.sLotID_VisionStage[nS4No-1], gData.nPortNo_VisionStage[nS4No-1], gData.nTrayNo_VisionStage[nS4No-1], nModule1, gData.nTrayNo_VisionStage[nS4No-1], nModule5);
				gAlm.sAlmLID[2] = gData.sLotID_VisionStage[nS4No-1]; gAlm.nAlmTNo[2] = gData.nTrayNo_VisionStage[nS4No-1]; gAlm.nAlmPNo[2] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[2] = nModule5;
				m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 8:
		if (gData.bTop1ScanDone || !m_pEquipData->bUseTop1) {
			m_tVisionStage4Loop.Takt_Save(17, 4); m_tVisionStage4Loop.Takt_Start(); 
			gData.nTop1Cnt++; n4Tno1++; n4Tno5++;
			if (gData.nTop1Cnt >= 4) {
				if (m_pEquipData->bUseTopAng) {
					gData.nTop1Cnt = 0;
					n4Tno1 = gData.InfoVision[nS4No-1][9];
					n4Tno5 = gData.InfoVision[nS4No-1][9] + 4;

					g_objCommon.Move_Position(AX_TOP1_VISION_Z, 2);
					g_objCommon.Move_Position(AX_TOP1_LIGHT_Z, 2);
					g_objCommon.Move_Position(AX_INSPECT_STAGE_X4, 1);

					m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(30000);
				} else {
					m_nVisionStage4Case = 12; m_tVisionStage4Loop.Set_LoopTime(5000);
				}
			} else {
				m_nVisionStage4Case = 5; m_tVisionStage4Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 9:	//Top1-Angle
		if (g_objCommon.Check_Position(AX_TOP1_VISION_Z, 2) && g_objCommon.Check_Position(AX_TOP1_LIGHT_Z, 2) ) {
			g_objCommon.Save_Motion(AX_TOP1_VISION_Z, 2);	g_objCommon.Save_Motion(AX_TOP1_LIGHT_Z, 2);
			g_objCommon.Move_Position(AX_TOP1_ANGLE_Y, 2); gData.nScanRetry[1] = 0;
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(30000);
		}
		break;
	case 10:
		if (m_pDX08->iInspectStage4Up && !m_pDX08->iInspectStage4Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				m_tVisionStage4Loop.Takt_Save(17, 5); m_tVisionStage4Loop.Takt_Start(); 
				dStage4PosX = m_pMoveData->dVisionStageX4[1] + (m_pEquipData->dPitchTop * gData.nTop1Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X4, dStage4PosX);
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 2);	nV4Retry = 0;
				m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X4, dStage4PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 2) && g_objCommon.Check_Position(AX_TOP1_VISION_Z, 2)) {
			if (g_objCommon.Check_Position(AX_TOP1_ANGLE_Y, 2) && g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
				if (!m_tVisionStage4Loop.Waiting_Time(m_pEquipData->nDelayTime[9])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X4, -1, dStage4PosX);	g_objCommon.Save_Motion(AX_TOP1_ANGLE_Y, 2);
				m_tVisionStage4Loop.Takt_Save(17, 6); m_tVisionStage4Loop.Takt_Start(); 
				gData.bTop1ScanDone = FALSE;	gData.nVsNo[0] = nS4No; gData.nVsIx[0] = gData.nTop1Cnt+1;
				int nModule1 = n4Tno1; if (gData.InfoVision[nS4No-1][gData.nTop1Cnt] < 1)   nModule1 = -1;
				int nModule5 = n4Tno5; if (gData.InfoVision[nS4No-1][gData.nTop1Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "TG", gData.sLotID_VisionStage[nS4No-1], gData.nPortNo_VisionStage[nS4No-1], gData.nTrayNo_VisionStage[nS4No-1], nModule1, gData.nTrayNo_VisionStage[nS4No-1], nModule5);
				gAlm.sAlmLID[2] = gData.sLotID_VisionStage[nS4No-1]; gAlm.nAlmTNo[2] = gData.nTrayNo_VisionStage[nS4No-1]; gAlm.nAlmPNo[2] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[2] = nModule5;
				m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 12:
		if (gData.bTop1ScanDone || !m_pEquipData->bUseTopAng) {
			m_tVisionStage4Loop.Takt_Save(17, 7); m_tVisionStage4Loop.Takt_Start(); 
			gData.nTop1Cnt++; n4Tno1++; n4Tno5++;
			if (gData.nTop1Cnt >= 4) {
				g_objCommon.Move_Position(AX_TOP1_VISION_Z, 1);
				g_objCommon.Move_Position(AX_TOP1_ANGLE_Y, 1);
				m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(30000);
			} else {
				m_nVisionStage4Case = 10; m_tVisionStage4Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 13:
		if (n4FrontVisionCase > 22) {
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 14:
		if (m_pDX08->iInspectStage4Up && !m_pDX08->iInspectStage4Down) {
			if (m_pDX08->iInspectStage4Yin && !m_pDX08->iInspectStage4YOut) {
				m_tVisionStage4Loop.Takt_Save(17, 8); m_tVisionStage4Loop.Takt_Start(); 
				m_pDY08->oInspectStage4Deg0 = FALSE; m_pDY08->oInspectStage4Deg90 = TRUE;
				g_objAJinAXL.Write_Output(8);
				g_objCommon.Move_Position(AX_TOP2_VISION_Z, 1);
				g_objCommon.Move_Position(AX_INSPECT_STAGE_X4, 2);
				m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 15:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X4, 2)) {
			if (g_objCommon.Check_Position(AX_TOP2_VISION_Z, 1)) {
				if (m_pDX08->iInspectStage4Deg90) {
					g_objCommon.Save_Motion(AX_INSPECT_STAGE_X4, 2);	g_objCommon.Save_Motion(AX_TOP2_VISION_Z, 1);
					m_tVisionStage4Loop.Takt_Save(17, 9); m_tVisionStage4Loop.Takt_Start(); 
					gData.nVisionPos[1][1] = 0; gData.nVisionPos[1][2] = nS4No;
					gData.nTop2Cnt = gData.nScanRetry[2] = 0;
					n4Tno1 = gData.InfoVision[nS4No-1][9];
					n4Tno5 = gData.InfoVision[nS4No-1][9] + 4;
					m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(20000);
				}
			}
		}
		break;
	case 16:
		if (m_pDX08->iInspectStage4Up && !m_pDX08->iInspectStage4Down) {
			if (g_objAJinAXL.Is_Done(AX_TOP2_VISION_Z)) {
				m_tVisionStage4Loop.Takt_Save(17, 10); m_tVisionStage4Loop.Takt_Start(); 
				dStage4PosX = m_pMoveData->dVisionStageX4[2] + (m_pEquipData->dPitchTop * gData.nTop2Cnt);
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE_X4, dStage4PosX);
				g_objCommon.Move_Position(AX_TOP2_VISION_Z, 1);	nV4Retry = 0;
				m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(20000);
			}
		}
		break;
	case 17:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X4, dStage4PosX)) {
//			if (g_objCommon.Check_Position(AX_TOP2_VISION_Z, 1)) {
			if (g_objAJinAXL.Is_Done(AX_TOP2_VISION_Z)) {
				if (!m_tVisionStage4Loop.Waiting_Time(m_pEquipData->nDelayTime[9])) break;
				g_objCommon.Save_Motion(AX_INSPECT_STAGE_X4, -1, dStage4PosX);
				m_tVisionStage4Loop.Takt_Save(17, 11); m_tVisionStage4Loop.Takt_Start(); 
				gData.bTop2ScanDone = FALSE;	gData.nVsNo[1] = nS4No; gData.nVsIx[1] = gData.nTop2Cnt+1;
				int nModule1 = n4Tno1; if (gData.InfoVision[nS4No-1][gData.nTop2Cnt] < 1)   nModule1 = -1;
				int nModule5 = n4Tno5; if (gData.InfoVision[nS4No-1][gData.nTop2Cnt+4] < 1) nModule5 = -1;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC3, "T2", gData.sLotID_VisionStage[nS4No-1], gData.nPortNo_VisionStage[nS4No-1], gData.nTrayNo_VisionStage[nS4No-1], nModule1, gData.nTrayNo_VisionStage[nS4No-1], nModule5);
				gAlm.sAlmLID[3] = gData.sLotID_VisionStage[nS4No-1]; gAlm.nAlmTNo[3] = gData.nTrayNo_VisionStage[nS4No-1]; gAlm.nAlmPNo[3] = nModule1; if (nModule1 == -1) gAlm.nAlmPNo[3] = nModule5;
				m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]);
			}
		}
		break;
	case 18:
		if (gData.bTop2ScanDone || !m_pEquipData->bUseTop2) {
			m_tVisionStage4Loop.Takt_Save(17, 12); m_tVisionStage4Loop.Takt_Start(); 
			gData.nTop2Cnt++; n4Tno1++; n4Tno5++;
			if (gData.nTop2Cnt >= 4) {
				dwV4RosTime[0] = GetTickCount();
				m_nVisionStage4Case = 20; m_tVisionStage4Loop.Set_LoopTime(5000);
			} else {
				m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 19:
		if (n4FrontVisionCase < 19 || (n4FrontVisionCase >= 23 && gData.nTop2Cnt <= 1) || n4FrontVisionCase >= 25) {
			m_nVisionStage4Case = 16; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 20:
		if (n4FrontVisionCase < 20 || n4FrontVisionCase >= 25) {
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 21:
		if (m_pDX08->iInspectStage4Up && !m_pDX08->iInspectStage4Down) {
			if (m_pDX08->iInspectStage4Yin && !m_pDX08->iInspectStage4YOut) {
				if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE_X4, dStage4PosX)) {
					m_tVisionStage4Loop.Takt_Save(17, 13); m_tVisionStage4Loop.Takt_Start(); 
					g_objCommon.Move_Position(AX_INSPECT_STAGE_X4, 4);
					m_pDY08->oInspectStage4Deg0 = TRUE; m_pDY08->oInspectStage4Deg90 = FALSE;
					g_objAJinAXL.Write_Output(8);
					m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 22:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X4, 4)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X4, 4);
			m_tVisionStage4Loop.Takt_Save(17, 14); m_tVisionStage4Loop.Takt_Start(); 
			dwV4RosTime[1] = dwV4RosTime[0];
			gData.nVisionPos[1][2] = 0; gData.nVisionPos[1][4] = nS4No;
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(10000);
		}
		break;
	case 23:
		if (n4FrontVisionCase < 22 || n4FrontVisionCase > 34) {
			m_tVisionStage4Loop.Takt_Save(17, 15); m_tVisionStage4Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X4, 3);
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 24:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X4, 3)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X4, 3);
			m_tVisionStage4Loop.Takt_Save(17, 16); m_tVisionStage4Loop.Takt_Start(); 
			dwV4RosTime[2] = dwV4RosTime[1];
			gData.nVisionPos[1][4] = 0; gData.nVisionPos[1][3] = nS4No;
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(30000);
		}
		break;
	case 25:
			m_nVisionStage4Case = 30; m_tVisionStage4Loop.Set_LoopTime(30000);
		break;
	case 26:	//RosTime 6초대기
		if (GetTickCount() - dwV4RosTime[2] >= 8000) {
			m_nVisionStage4Case = 30; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		break;

	case 30:	//Unload Picker wait
		m_tVisionStage4Loop.Set_LoopTime(5000);
		return TRUE;

	case 31:
		if (m_pDX08->iInspectStage4Up && !m_pDX08->iInspectStage4Down) {
			if (m_pDX08->iInspectStage4Yin && !m_pDX08->iInspectStage4YOut) {
				m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 32:
		if (n4FrontVisionCase < 30 || n4FrontVisionCase >= 50) {
			m_tVisionStage4Loop.Takt_Save(17, 17); m_tVisionStage4Loop.Takt_Start(); 
			m_pDY08->oInspectStage4Up = FALSE; m_pDY08->oInspectStage4Down = TRUE;
			g_objAJinAXL.Write_Output(8);
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 33:
		if (!m_pDX08->iInspectStage4Up && m_pDX08->iInspectStage4Down) {
			m_tVisionStage4Loop.Takt_Save(17, 18); m_tVisionStage4Loop.Takt_Start(); 
			gData.nVisionPos[1][3] = 0; gData.nVisionPos[0][3] = nS4No;
			if (nS4No == 1) {
				m_pDY08->oInspectStage4Yin = FALSE; m_pDY08->oInspectStage4YOut = TRUE;
				g_objAJinAXL.Write_Output(8);
			}
			if (nS4No == 4) {
				m_pDY08->oInspectStage4Yin = FALSE; m_pDY08->oInspectStage4YOut = TRUE;
				g_objAJinAXL.Write_Output(8);
			}
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		break;
	case 34:
		if ((nS4No == 1 && !m_pDX08->iInspectStage4Yin && m_pDX08->iInspectStage4YOut) ||
			(nS4No == 2) || (nS4No == 3) ||
			(nS4No == 4 && !m_pDX08->iInspectStage4Yin && m_pDX08->iInspectStage4YOut) ) {
			m_tVisionStage4Loop.Takt_Save(17, 19); m_tVisionStage4Loop.Takt_Start(); 
			m_nVisionStage4Case = 40; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		break;

	case 40:
		if (n4FrontVisionCase < 40 || n4FrontVisionCase >= 60) {
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 41:
		if (!m_pDX08->iInspectStage4Up && m_pDX08->iInspectStage4Down) {
			m_tVisionStage4Loop.Takt_Save(17, 20); m_tVisionStage4Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X4, 2);
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(10000);
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X4, 2)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X4, 2);
			m_tVisionStage4Loop.Takt_Save(17, 21); m_tVisionStage4Loop.Takt_Start(); 
			gData.nVisionPos[0][3] = 0; gData.nVisionPos[0][2] = nS4No;
			m_nVisionStage4Case = 50; m_tVisionStage4Loop.Set_LoopTime(60000);
		}
		break;

	case 50:
		if (n4FrontVisionCase < 50 || n4FrontVisionCase >= 70) {
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 51:
		if (!m_pDX08->iInspectStage4Up && m_pDX08->iInspectStage4Down) {
			m_tVisionStage4Loop.Takt_Save(17, 22); m_tVisionStage4Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X4, 1);
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(10000);
		}
		break;
	case 52:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X4, 1)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X4, 1);
			m_tVisionStage4Loop.Takt_Save(17, 23); m_tVisionStage4Loop.Takt_Start(); 
			gData.nVisionPos[0][2] = 0; gData.nVisionPos[0][1] = nS4No;
			m_nVisionStage4Case = 60; m_tVisionStage4Loop.Set_LoopTime(60000);
		}
		break;

	case 60:
		if (n4FrontVisionCase >= 0 && n4FrontVisionCase < 50) {
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 61:
		if (!m_pDX08->iInspectStage4Up && m_pDX08->iInspectStage4Down) {
			m_tVisionStage4Loop.Takt_Save(17, 24); m_tVisionStage4Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_INSPECT_STAGE_X4, 0);
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(10000);
		}
		break;
	case 62:
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE_X4, 0)) {
			g_objCommon.Save_Motion(AX_INSPECT_STAGE_X4, 0);
			m_tVisionStage4Loop.Takt_Save(17, 25); m_tVisionStage4Loop.Takt_Start(); 
			gData.nVisionPos[0][1] = 0; gData.nVisionPos[0][0] = nS4No;
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(10000);
		}
		break;
	case 63:
		if (!m_pDX08->iInspectStage4Up && m_pDX08->iInspectStage4Down) {
/*			if (nS4No == 1) {
				m_pDY08->oInspectStage4Yin = TRUE; m_pDY08->oInspectStage4YOut = FALSE;
				g_objAJinAXL.Write_Output(8);
			}
			if (nS4No == 4) {
				m_pDY08->oInspectStage4Yin = TRUE; m_pDY08->oInspectStage4YOut = FALSE;
				g_objAJinAXL.Write_Output(8);
			}*/
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		break;
	case 64:
//		if ((nS4No == 1 && m_pDX08->iInspectStage4Yin && !m_pDX08->iInspectStage4YOut) ||
//			(nS4No == 2) || (nS4No == 3) ||
//			(nS4No == 4 && m_pDX08->iInspectStage4Yin && !m_pDX08->iInspectStage4YOut) ) {
			m_nVisionStage4Case = 70; m_tVisionStage4Loop.Set_LoopTime(5000);
//		}
		break;

	case 70:
		if (n4FrontVisionCase > 4 && n4FrontVisionCase < 60) {
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 71:
		if (!m_pDX08->iInspectStage4Up && m_pDX08->iInspectStage4Down) {
			m_tVisionStage4Loop.Takt_Save(17, 26); m_tVisionStage4Loop.Takt_Start(); 
			m_pDY08->oInspectStage4Yin = TRUE; m_pDY08->oInspectStage4YOut = FALSE;
			g_objAJinAXL.Write_Output(8);
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		break;	
	case 72:
		if (m_pDX08->iInspectStage4Yin && !m_pDX08->iInspectStage4YOut) {
			m_tVisionStage4Loop.Takt_Save(17, 27); m_tVisionStage4Loop.Takt_Start(); 
			m_pDY08->oInspectStage4Up = TRUE; m_pDY08->oInspectStage4Down = FALSE;
			g_objAJinAXL.Write_Output(8);
			m_nVisionStage4Case++; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		break;
	case 73:
		if (m_pDX08->iInspectStage4Up && !m_pDX08->iInspectStage4Down) {
			m_tVisionStage4Loop.Takt_Save(17, 28);
			gData.nVisionPos[0][0] = 0; gData.nVisionPos[1][0] = nS4No;
			m_nVisionStage4Case = 0; m_tVisionStage4Loop.Set_LoopTime(5000);
		}
		break;

	}

	// 17. (Error : 6600)
	if (m_tVisionStage4Loop.Over_LoopTime()) {
//		if (m_nVisionStage4Case == 8 || m_nVisionStage4Case == 12 || m_nVisionStage4Case == 18) {
//			nV4Retry++;
//			if (nV4Retry == 1) { m_nVisionStage4Case--; return TRUE; }
//		}
//		if (m_nVisionStage4Case == 7 || m_nVisionStage4Case == 11 || m_nVisionStage4Case == 17) m_nVisionStage4Case++;
		if (m_nVisionStage4Case == 26) { m_nVisionStage4Case = 30; return TRUE; }
		g_objCommon.Show_Error(6600 + m_nVisionStage4Case);
		return FALSE;
	}
	return TRUE;
}

// 18. (Error : 7000)
BOOL CSequenceMain::Run_UnloadPicker1()
{
	static int n1No = 1;	//1고정
	static int n1VNo, n1NSNo, n1ModuleNo, n1PosX, n1PosY, n1UP, n1UT, n1UM, nJudgeNo1;
	static double d1PosX, d1PosY;

	switch (m_nUnloadPicker1Case) {
	case 0:	// Wait
		n1VNo = 0;
		if (m_nVisionStage1Case == 30) n1VNo = 1;
		if (m_nVisionStage2Case == 30) n1VNo = 2;
		if (m_nVisionStage3Case == 30) n1VNo = 3;
		if (m_nVisionStage4Case == 30) n1VNo = 4;
		if (n1VNo > 0) 
		{
			if (gData.nTrayNo_VisionStage[n1VNo-1] > 0) 
			{
				m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(5000);

				m_sLog.Format("[UnloadPicker1: Start] Inspection StageNo(%d)", n1VNo);
				g_objLogFile.Save_HandlerLog(m_sLog);
			}
			else
			{
				if (m_nVisionStage1Case == 30) m_nVisionStage1Case = 31;
				if (m_nVisionStage2Case == 30) m_nVisionStage2Case = 31;
				if (m_nVisionStage3Case == 30) m_nVisionStage3Case = 31;
				if (m_nVisionStage4Case == 30) m_nVisionStage4Case = 31;
			}
		}
		if (Check_EmptyUnloadPicker()) 
		{
			if (m_nLDCVElevatorCase == 0 && m_nLDMZElevatorCase == 0)
			{
				if ((m_nLoadStage1Case == 0 && m_nLoadStage2Case == 50) || (m_nLoadStage1Case == 50 && m_nLoadStage2Case == 0)) {
					if (m_nNGStage1Case == 30) m_nNGStage1Case = 31;
					if (m_nNGStage2Case == 30) m_nNGStage2Case = 31;
				}
			}
			if (m_nGoodStage1Case == 30) m_nGoodStage1Case = 31;
			if (m_nGoodStage2Case == 30) m_nGoodStage2Case = 31;
		}
		return TRUE;

	case 1:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z1, 0)) 
		{
			if (g_objCommon.Get_UnloadPickerUp(n1No))
			{
				g_objCommon.Move_Position(AX_UNLOAD_PICKER_X1, n1VNo);
				g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y1, n1VNo);
				g_objCommon.Move_Position(AX_UNLOAD_PICKER_P1, 0);

				m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 2:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_X1, n1VNo) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y1, n1VNo) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P1, 0)) 
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_X1, n1VNo);	g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y1, n1VNo);	g_objCommon.Save_Motion(AX_UNLOAD_PICKER_P1, 0);
			m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerOpen(n1No);
			g_objCommon.Set_UnloadPickerDown(n1No);

			gData.sLotID_UnloadPicker[n1No-1]  = gData.sLotID_VisionStage[n1VNo-1];
			gData.nTrayNo_UnloadPicker[n1No-1] = gData.nTrayNo_VisionStage[n1VNo-1];
			gData.nPortNo_UnloadPicker[n1No-1] = gData.nPortNo_VisionStage[n1VNo-1];
			gData.sLotID_UnloadPicker[2]	   = gData.sLotID_UnloadPicker[n1No-1];

			for(int i=0; i<10; i++) 
			{
				gData.InfoUnloadPick[n1No-1][i] = gData.InfoVision[n1VNo-1][i];	gData.InfoVision[n1VNo-1][i] = 0;				
			}

			for(int i=0; i<PICK_CNT; i++) 
			{
				gTracking.sMZID_UnloadPicker[n1No-1][i] = gTracking.sMZID_InspectStage[n1VNo-1][i];	gTracking.sMZID_InspectStage[n1VNo-1][i] = "";
				m_sLog.Format("InspectStage %d To UnloadPicker %d, pocket:%d - MZ ID:%s",n1VNo, n1No, i , gTracking.sMZID_UnloadPicker[n1No-1][i]);
				if(gTracking.sMZID_UnloadPicker[n1No-1][i] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);				
			}
			
			gData.sLotID_VisionStage[n1VNo-1] = "";
			gData.nTrayNo_VisionStage[n1VNo-1] = gData.nPortNo_VisionStage[n1VNo-1] = 0;

			n1UP = m_nData[0][0] = gData.nPortNo_UnloadPicker[n1No-1] - 1;
			n1UT = m_nData[0][1] = gData.nTrayNo_UnloadPicker[n1No-1] - 1;
			n1UM = m_nData[0][2] = gData.InfoUnloadPick[n1No-1][9] - 1;
			for(int i=n1UM; (i<40 || i<n1UM+8); i++) gLot.nHistory[n1UP][n1UT][i][4] = 1;

			m_sLog.Format("UnloadPicker Data [%d-%d-%d-%d-%s-%d]", n1No, n1UP+1, n1UT+1, n1UM+1, gLot.sLotID[n1UP], n1VNo);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 3:
		if (g_objCommon.Get_UnloadPickerDown(n1No))
		{
			m_tUnloadPicker1Loop.Takt_Save(18, 1); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z1, n1VNo);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z1, n1VNo))
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Z1, n1VNo);
			m_tUnloadPicker1Loop.Takt_Save(18, 2); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Set_VisionVacuumOff(n1VNo);
			g_objCommon.Set_UnloadPickerClose(n1No);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 5:
		if (g_objCommon.Get_UnloadPickerClose(n1No))
		{
			if (!m_tUnloadPicker1Loop.Waiting_Time(m_pEquipData->nDelayTime[8])) break;
			m_tUnloadPicker1Loop.Takt_Save(18, 3); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z1, 0);
			g_objCommon.Set_UnloadPickerUp(n1No);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 6:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z1, 0)) 
		{
			if (g_objCommon.Get_UnloadPickerUp(n1No)) 
			{
				g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Z1, 0);
				m_tUnloadPicker1Loop.Takt_Save(18, 4); m_tUnloadPicker1Loop.Takt_Start(); 

				m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 7:
		if (g_objCommon.Get_UnloadPickerCheck(n1No, 9, gData.InfoUnloadPick[n1No-1])) 
		{
			m_tUnloadPicker1Loop.Takt_Save(18, 5); m_tUnloadPicker1Loop.Takt_Start(); 
			if (m_nVisionStage1Case == 30) m_nVisionStage1Case = 31;
			if (m_nVisionStage2Case == 30) m_nVisionStage2Case = 31;
			if (m_nVisionStage3Case == 30) m_nVisionStage3Case = 31;
			if (m_nVisionStage4Case == 30) m_nVisionStage4Case = 31;

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 10; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;

	case 10:
		if (m_nUnloadPicker2Case > 33 && m_nUnloadPicker2Case < 70) 
		{
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_X1, 5);
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_P1, 1);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 11:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_X1, 5) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P1, 1)) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_X1, 5);	g_objCommon.Save_Motion(AX_UNLOAD_PICKER_P1, 1);
			gAlm.sAlmLID[0] = gData.sLotID_UnloadPicker[n1No-1]; gAlm.nAlmTNo[0] = gData.nTrayNo_UnloadPicker[n1No-1]; gAlm.nAlmPNo[0] = gData.InfoUnloadPick[n1No-1][9];

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]-10000);
		}
		break;
	case 12:
		if (Check_InspectDone(n1No, gData.nPortNo_UnloadPicker[n1No-1], gData.nTrayNo_UnloadPicker[n1No-1])) 
		{
			m_tUnloadPicker1Loop.Takt_Save(18, 6); m_tUnloadPicker1Loop.Takt_Start(); 

			Set_ROSTime(n1No, gData.nPortNo_UnloadPicker[n1No-1], gData.nTrayNo_UnloadPicker[n1No-1]);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 15; m_tUnloadPicker1Loop.Set_LoopTime(300000);	//5분
		}
		break;
	case 13:
		if (Check_InspectDone(n1No, gData.nPortNo_UnloadPicker[n1No-1], gData.nTrayNo_UnloadPicker[n1No-1])) {
			m_tUnloadPicker1Loop.Takt_Save(18, 6); m_tUnloadPicker1Loop.Takt_Start(); 
			Set_ROSTime(n1No, gData.nPortNo_UnloadPicker[n1No-1], gData.nTrayNo_UnloadPicker[n1No-1]);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 15; m_tUnloadPicker1Loop.Set_LoopTime(300000);	//5분
		}
		break;

	case 15:
		if (Check_ROSDone(n1No, gData.nPortNo_UnloadPicker[n1No-1], gData.nTrayNo_UnloadPicker[n1No-1])) {
			m_tUnloadPicker1Loop.Takt_Save(18, 7); m_tUnloadPicker1Loop.Takt_Start(); 

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 19; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;

	case 19:
		int nType;
		n1ModuleNo = Check_NGExist(n1No, nType);
		nJudgeNo1 = nType;
		if (n1ModuleNo > 0) 
		{
			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 21; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		} 
		else
		{
			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 20:
		if (m_nUnloadPicker2Case < 20 || (m_nUnloadPicker2Case > 42 && m_nUnloadPicker2Case < 70))
		{
			if (Check_MZEndUnload(gData.sLotID_UnloadPicker[n1No-1], gData.nPortNo_UnloadPicker[n1No-1], n1No)) 
			{
			
				if (m_nNGStage1Case == 30) m_nNGStage1Case = 31;
				if (m_nNGStage2Case == 30) m_nNGStage2Case = 31;
				m_sLog.Format("Unload Picker:%d MZ-End Pno[%d] LotID[%s] Seq[%d-%d]", n1No, gData.nPortNo_UnloadPicker[n1No-1], gData.sLotID_UnloadPicker[n1No-1], m_nNGStage1Case, m_nNGStage2Case);
				g_objLogFile.Save_HandlerLog(m_sLog);
			}
			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 30; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 21:
		if (m_nUnloadPicker2Case < 20 || (m_nUnloadPicker2Case > 42 && m_nUnloadPicker2Case < 70)) 
		{
			n1NSNo = 0;
//			if (m_nNGStage1Case == 30 && !m_pDX11->iNGStage1TrayExist && gData.nPortNo_NGTray[0] < 1) m_nNGStage1Case = 31;
//			if (m_nNGStage2Case == 30 && !m_pDX11->iNGStage2TrayExist && gData.nPortNo_NGTray[1] < 1) m_nNGStage2Case = 31;
			if (m_nNGStage1Case == 30 && gNG->nTrayOX[1][0] == 0) m_nNGStage1Case = 31;
			if (m_nNGStage2Case == 30 && gNG->nTrayOX[1][1] == 0) m_nNGStage2Case = 31;
			if (m_nNGStage1Case == 30) n1NSNo = 5;
			if (m_nNGStage2Case == 30) n1NSNo = 6;
			if (n1NSNo > 0) 
			{
				if (m_nGoodStage1Case == 30 || m_nGoodStage2Case == 30) 
				{
					if (Select_NGTrayPoketNo(nJudgeNo1, n1PosX, n1PosY)) 
					{
						g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y1, n1NSNo);
						m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
					}
					else
					{
						if (Check_EmptyNGTray()) 
						{
							gAlm.sAlmLotID[0].Format("%d", nJudgeNo1);
							g_objCommon.Show_Error(7091);
							return FALSE;
						}
						if (m_nNGStage1Case == 30) m_nNGStage1Case = 31;
						if (m_nNGStage2Case == 30) m_nNGStage2Case = 31;
					}
				}
			}
		}
		return TRUE;
	case 22:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y1, n1NSNo))
		{
			if (Get_TrayPosition(n1No, (n1NSNo-4), n1PosX, n1PosY, d1PosX, d1PosY)) 
			{
				g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y1, n1NSNo);
				m_tUnloadPicker1Loop.Takt_Save(18, 8); m_tUnloadPicker1Loop.Takt_Start(); 
				if (n1PosX < 3) d1PosX = d1PosX + (m_pEquipData->dTrayPitchX * (n1ModuleNo - 1));
				else			d1PosX = d1PosX + (m_pEquipData->dTrayPitchX * (n1ModuleNo - 1)) - CARRIER_OFFSETX;
				if (n1PosY > 5) d1PosY = d1PosY + CARRIER_OFFSETY;
				g_objAJinAXL.Move_Absolute(AX_UNLOAD_PICKER_X1, d1PosX);
				if (n1NSNo==5) g_objAJinAXL.Move_Absolute(AX_NG_STAGE_Y1, d1PosY);
				if (n1NSNo==6) g_objAJinAXL.Move_Absolute(AX_NG_STAGE_Y2, d1PosY);

				m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);

				m_sLog.Format("A2 Data-NG [Picker:%d Job:%d Opt:%d] D[%d-%d-%d-%s] Type(%d) MNo(%d) XY(%d-%d) PosXY(%0.3lf-%0.3lf)", n1No, (n1NSNo-4), m_pEquipData->bUseNGSort, n1UP, n1UT, n1UM, gLot.sLotID[n1UP], nJudgeNo1, n1ModuleNo, n1PosX, n1PosY, d1PosX, d1PosY);
				g_objLogFile.Save_HandlerLog(m_sLog);
			}
		}
		break;
	case 23:
		if (g_objAJinAXL.Is_MoveDone(AX_UNLOAD_PICKER_X1, d1PosX)) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_X1, -1, d1PosX);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break; 
	case 24:
		if ((n1NSNo==5 && g_objAJinAXL.Is_MoveDone(AX_NG_STAGE_Y1, d1PosY)) ||
			(n1NSNo==6 && g_objAJinAXL.Is_MoveDone(AX_NG_STAGE_Y2, d1PosY)) ) 
		{
			if (n1NSNo==5) g_objCommon.Save_Motion(AX_NG_STAGE_Y1, -1, d1PosY);
			if (n1NSNo==6) g_objCommon.Save_Motion(AX_NG_STAGE_Y2, -1, d1PosY);
			m_tUnloadPicker1Loop.Takt_Save(18, 9); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z1, n1NSNo);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 25:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z1, n1NSNo)) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Z1, n1NSNo);
			m_tUnloadPicker1Loop.Takt_Save(18, 10); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerDown(n1No, n1ModuleNo);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 26:
		if (g_objCommon.Get_UnloadPickerDown(n1No, n1ModuleNo)) {
			m_tUnloadPicker1Loop.Takt_Save(18, 11); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerOpen(n1No, n1ModuleNo);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 27:
		if (g_objCommon.Get_UnloadPickerOpen(n1No, n1ModuleNo)) {
			if (!m_tUnloadPicker1Loop.Waiting_Time(m_pEquipData->nDelayTime[7])) break;
			m_tUnloadPicker1Loop.Takt_Save(18, 12); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerUp(n1No);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 28:
		if (g_objCommon.Get_UnloadPickerUp(n1No)) 
		{
			m_tUnloadPicker1Loop.Takt_Save(18, 13); m_tUnloadPicker1Loop.Takt_Start(); 
			if (n1UP < 0 || n1UP > 29) {
				n1UP = m_nData[0][0];	n1UT = m_nData[0][1];	n1UM = m_nData[0][2];
			}
			if (gLot.sLotID[n1UP].GetLength() < 1) {
				gLot.sLotID[n1UP] = gNG->sBackData[n1UP][0];
				gLot.sRecipeName[n1UP] = gNG->sBackData[n1UP][1];
				gLot.nCmCount[n1UP] = gNG->nBackCnt[n1UP];
				m_sLog.Format("[UnloadPicker:%d] DN[%d-%s]", n1No, n1UP+1, gLot.sLotID[n1UP]);
				g_objLogFile.Save_HandlerLog(m_sLog);
			}
			gLot.nNgCount[n1UP]++;	gLot.nTrayPutCnt[n1UP][1]++;
			if (Check_EmptyNGTray()) gLot.nOutTrayCnt[n1UP][1]++;

			gData.sLotID_NGTray[n1NSNo-5]  = gData.sLotID_UnloadPicker[n1No-1];
			gData.nTrayNo_NGTray[n1NSNo-5] = gData.nTrayNo_UnloadPicker[n1No-1];
			gData.nPortNo_NGTray[n1NSNo-5] = gData.nPortNo_UnloadPicker[n1No-1];
			gData.sLotID_NGTray[2]  = gData.sLotID_NGTray[n1NSNo-5];

			gData.InfoNgTray[n1PosY-1][n1PosX-1] = gData.InfoUnloadPick[n1No-1][n1ModuleNo-1];
			gData.InfoUnloadPick[n1No-1][n1ModuleNo-1] = 0;

			gTracking.sMZID_NGTray[n1PosY-1][n1PosX-1] = gTracking.sMZID_UnloadPicker[n1No-1][n1ModuleNo-1];
			gTracking.sMZID_UnloadPicker[n1No-1][n1ModuleNo-1] = "";

			m_sLog.Format("UnloadPicker %d To NGTray, x:%d Y:%d - MZ ID:%s",n1No, n1PosY, n1PosX , gTracking.sMZID_NGTray[n1PosY-1][n1PosX-1]);
			if(gTracking.sMZID_NGTray[n1PosY-1][n1PosX-1] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);
			
			g_dlgOCAPFai.Set_AddDEFECT(gData.sMZID[7], gLot.sNGCode_I[n1UP][n1UT][n1UM+n1ModuleNo-1][0]);
			g_dlgOCAPCosmetic.AddModuleToCarrier(gData.nPortNo_NGTray[n1NSNo-5]-1, "NG",gData.InfoNgTray[n1PosY-1][n1PosX-1], gLot.sNGCode_I[n1UP][n1UT][n1UM+n1ModuleNo-1][0], gLot.sNGCode_Vision[n1UP][n1UT][n1UM+n1ModuleNo-1][0], gCap.nCarrierMZIndex[1]);
			
			gLot.nHistory[n1UP][n1UT][n1UM+n1ModuleNo-1][6] = 1;
			gLot.nHistory[n1UP][n1UT][n1UM+n1ModuleNo-1][7] = (n1PosY-1) * 4 + n1PosX;
			gLot.nHistory[n1UP][n1UT][n1UM+n1ModuleNo-1][8] = n1NSNo-4;
			g_objMesAgent.Set_CmEnd(1, n1UP+1, n1UT+1, n1UM+n1ModuleNo, (n1PosY-1) * 4 + n1PosX, gData.sCarID_NGTray[n1NSNo-5]);
			g_objLogFile.Save_OutTrayNLog(n1UP+1, n1UT+1, n1UM+n1ModuleNo, n1NSNo-5); 

			if (m_nGoodStage1Case == 30 && gData.nPortNo_GoodTray[0] == 0)			 gData.nPortNo_GoodTray[0] = gData.nPortNo_NGTray[n1NSNo-5];
			if (m_nGoodStage1Case == 30 && gData.sLotID_GoodTray[0].GetLength() < 2) gData.sLotID_GoodTray[0] = gData.sLotID_UnloadPicker[n1No-1];
			if (m_nGoodStage2Case == 30 && gData.nPortNo_GoodTray[1] == 0)			 gData.nPortNo_GoodTray[1] = gData.nPortNo_NGTray[n1NSNo-5];
			if (m_nGoodStage2Case == 30 && gData.sLotID_GoodTray[1].GetLength() < 2) gData.sLotID_GoodTray[1] = gData.sLotID_UnloadPicker[n1No-1];

			if (Check_NGTrayFull()) 
			{				
				if (m_nNGStage1Case == 30) m_nNGStage1Case = 31;
				if (m_nNGStage2Case == 30) m_nNGStage2Case = 31;
			}

			m_sLog.Format("[UnloadPicker:%d Job:%d Option:%d] D[%d-%d-%d-%s] P(%d-1) Type(%d) => (%d,%d-%0.3lf,%0.3lf)",
							n1No, (n1NSNo-4), m_pEquipData->bUseAlign2, n1UP, n1UT, n1UM, gLot.sLotID[n1UP], n1ModuleNo, gData.InfoNgTray[n1PosY-1][n1PosX-1], n1PosX, n1PosY, d1PosX, d1PosY);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 19; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;

	case 29:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y1, 0)) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y1, 0);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 40; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 30:
		n1ModuleNo = Check_GoodExist(n1No, n1PosX, n1PosY);	//Unload Pikcer에 작업 Module이 있는지 Check
		if (n1ModuleNo > 0) {
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		} 
		else
		{
			if (Check_LotEndUnloadNG(gData.sLotID_UnloadPicker[n1No-1], gData.nPortNo_UnloadPicker[n1No-1], n1No)) 
			{				
//				if (m_nNGStage1Case == 30)   m_nNGStage1Case = 31;
//				if (m_nNGStage2Case == 30)   m_nNGStage2Case = 31;
				if (m_nGoodStage1Case == 30) m_nGoodStage1Case = 31;
				if (m_nGoodStage2Case == 30) m_nGoodStage2Case = 31;
			}
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y1, 0);
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z1, 0);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 29; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 31:
		if (m_nUnloadPicker2Case < 30 ||  (m_nUnloadPicker2Case > 42 && m_nUnloadPicker2Case < 70)) {
			if (g_objAJinAXL.Is_Done(AX_UNLOAD_PICKER_X1)) {
				n1NSNo = 0;
//				if (m_nGoodStage1Case == 30 && !m_pDX12->iGoodStage1TrayExist && gData.nTrayNo_GoodTray[0] < 1) m_nGoodStage1Case = 31;
//				if (m_nGoodStage2Case == 30 && !m_pDX12->iGoodStage2TrayExist && gData.nTrayNo_GoodTray[1] < 1) m_nGoodStage2Case = 31;
				if (m_nGoodStage1Case == 30 && gNG->nTrayOX[2][0] == 0) m_nGoodStage1Case = 31;
				if (m_nGoodStage2Case == 30 && gNG->nTrayOX[2][1] == 0) m_nGoodStage2Case = 31;
				if (m_nGoodStage1Case == 30) n1NSNo = 7;
				if (m_nGoodStage2Case == 30) n1NSNo = 8;
				if (n1NSNo > 0) {
					if (Select_GoodTrayPoketNo(n1UP, n1UT, n1PosX, n1PosY)) {	//GoodTray에서 빈위치(작업위치) Check
						g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y1, n1NSNo);
						g_objCommon.Move_Position(AX_UNLOAD_PICKER_P1, 2);
						m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
					} else {
						if (m_nGoodStage1Case == 30) m_nGoodStage1Case = 31;
						if (m_nGoodStage2Case == 30) m_nGoodStage2Case = 31;
					}
				} else {
					if (!g_objCommon.Check_Position(AX_UNLOAD_PICKER_X1, 7)) g_objCommon.Move_Position(AX_UNLOAD_PICKER_X1, 7);
				}
			}
		}
		return TRUE;
	case 32:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y1, n1NSNo) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P1, 2)) {
			if (Get_TrayPosition(n1No, (n1NSNo-4), n1PosX, n1PosY, d1PosX, d1PosY)) {
				g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y1, n1NSNo);	g_objCommon.Save_Motion(AX_UNLOAD_PICKER_P1, 2);
				m_tUnloadPicker1Loop.Takt_Save(18, 14); m_tUnloadPicker1Loop.Takt_Start(); 
				if (n1PosX < 3) d1PosX = d1PosX + (m_pEquipData->dTrayPitchX * (n1ModuleNo - 1));
				else			d1PosX = d1PosX + (m_pEquipData->dTrayPitchX * (n1ModuleNo - 1)) - CARRIER_OFFSETX;
				if (n1PosY > 5) d1PosY = d1PosY + CARRIER_OFFSETY;
				g_objAJinAXL.Move_Absolute(AX_UNLOAD_PICKER_X1, d1PosX);
				if (n1NSNo==7) g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE_Y1, d1PosY);
				if (n1NSNo==8) g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE_Y2, d1PosY);

				m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);

				m_sLog.Format("A2 Data-GD D[%d-%d-%d-%s] [Picker:%d Job:%d] XY(%d,%d) PosXY(%0.3lf,%0.3lf) PNo(%d)", n1UP, n1UT, n1UM, gLot.sLotID[n1UP], n1No, (n1NSNo-4), n1PosX, n1PosY, d1PosX, d1PosY, n1ModuleNo);
				g_objLogFile.Save_HandlerLog(m_sLog);
			}
		}
		break;
	case 33:
		if (g_objAJinAXL.Is_MoveDone(AX_UNLOAD_PICKER_X1, d1PosX)) 
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_X1, -1, d1PosX);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 34:
		if ((n1NSNo==7 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE_Y1, d1PosY)) ||
			(n1NSNo==8 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE_Y2, d1PosY)) ) {
			if (n1NSNo==7) g_objCommon.Save_Motion(AX_GOOD_STAGE_Y1, -1, d1PosY);
			if (n1NSNo==8) g_objCommon.Save_Motion(AX_GOOD_STAGE_Y2, -1, d1PosY);
			m_tUnloadPicker1Loop.Takt_Save(18, 15); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z1, n1NSNo);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 35:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z1, n1NSNo)) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Z1, n1NSNo);
			m_tUnloadPicker1Loop.Takt_Save(18, 16); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerDown(n1No, 9, n1ModuleNo, n1PosX, gData.InfoUnloadPick[n1No-1]);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 36:
		if (g_objCommon.Get_UnloadPickerDown(n1No, 9, n1ModuleNo, n1PosX, gData.InfoUnloadPick[n1No-1])) {
			m_tUnloadPicker1Loop.Takt_Save(18, 17); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerOpen(n1No, 9, n1ModuleNo, n1PosX, gData.InfoUnloadPick[n1No-1]);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 37:
		if (g_objCommon.Get_UnloadPickerOpen(n1No, 9, n1ModuleNo, n1PosX, gData.InfoUnloadPick[n1No-1])) {
			if (!m_tUnloadPicker1Loop.Waiting_Time(m_pEquipData->nDelayTime[7])) break;
			m_tUnloadPicker1Loop.Takt_Save(18, 18); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerUp(n1No);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 38:
		if (g_objCommon.Get_UnloadPickerUp(n1No)) 
		{
			m_tUnloadPicker1Loop.Takt_Save(18, 19); m_tUnloadPicker1Loop.Takt_Start(); 
			if (n1UP < 0 || n1UP > 29) {
				n1UP = m_nData[0][0];	n1UT = m_nData[0][1];	n1UM = m_nData[0][2];
			}
			if (gLot.sLotID[n1UP].GetLength() < 1) {
				gLot.sLotID[n1UP] = gNG->sBackData[n1UP][0];
				gLot.sRecipeName[n1UP] = gNG->sBackData[n1UP][1];
				gLot.nCmCount[n1UP] = gNG->nBackCnt[n1UP];
				m_sLog.Format("[UnloadPicker:%d] DG[%d-%s]", n1No, n1UP+1, gLot.sLotID[n1UP]);
				g_objLogFile.Save_HandlerLog(m_sLog);
			}

			if (Check_EmptyGoodTray()) {
				if (m_pEquipData->bUseGoodSort) gLot.nOutTrayCnt[n1UP][0] = n1UT+1;
				else							gLot.nOutTrayCnt[n1UP][0]++;
			}
			gData.sLotID_GoodTray[n1NSNo-7]  = gData.sLotID_UnloadPicker[n1No-1];
			gData.nTrayNo_GoodTray[n1NSNo-7] = gData.nTrayNo_UnloadPicker[n1No-1];
			gData.nPortNo_GoodTray[n1NSNo-7] = gData.nPortNo_UnloadPicker[n1No-1];
			gData.sLotID_GoodTray[2]  = gData.sLotID_GoodTray[n1NSNo-7];

			for(int i=0; i<gData.nUnloadPick1DownCnt; i++) {
				gLot.nGoodCount[n1UP]++;	gLot.nTrayPutCnt[n1UP][0]++;
				gData.InfoGoodTray[n1PosY-1][n1PosX-1+i] = gData.InfoUnloadPick[n1No-1][n1ModuleNo-1+i];
				gData.InfoUnloadPick[n1No-1][n1ModuleNo-1+i] = 0;

				gTracking.sMZID_GoodTray[n1PosY-1][n1PosX-1+i] = gTracking.sMZID_UnloadPicker[n1No-1][n1ModuleNo-1+i];
				gTracking.sMZID_UnloadPicker[n1No-1][n1ModuleNo-1+i] = "";

				m_sLog.Format("UnloadPicker %d To GoodTray, x:%d Y:%d - MZ ID:%s",n1No, n1PosY, n1PosX , gTracking.sMZID_GoodTray[n1PosY-1][n1PosX-1]);
				if(gTracking.sMZID_GoodTray[n1PosY-1][n1PosX-1+i] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);

				m_sLog = gLot.sNGCode_Vision[n1UP][n1UT][n1UM+n1ModuleNo-1+i][0];

				g_dlgOCAPFai.Set_AddDEFECT(gData.sMZID[8], "");
				g_dlgOCAPCosmetic.AddModuleToCarrier(gData.nPortNo_GoodTray[n1NSNo-7]-1, "GOOD", 0, "", gLot.sNGCode_Vision[n1UP][n1UT][n1UM+n1ModuleNo-1+i][0], gCap.nCarrierMZIndex[0]);

				if (gData.InfoGoodTray[n1PosY-1][n1PosX-1+i] > 0) 
				{
					if (gLot.nMarginal[n1UP][n1UM+n1ModuleNo-1+i] > 0) gData.InfoGoodTray[n1PosY-1][n1PosX-1+i] = 9;	//Marginal Set
				}			

				gLot.nHistory[n1UP][n1UT][n1UM+n1ModuleNo-1+i][5] = 1;
				gLot.nHistory[n1UP][n1UT][n1UM+n1ModuleNo-1+i][7] = (n1PosY-1) * 4 + n1PosX + i;
				gLot.nHistory[n1UP][n1UT][n1UM+n1ModuleNo-1+i][8] = n1NSNo-6;
				g_objMesAgent.Set_CmEnd(0, n1UP+1, n1UT+1, n1UM+n1ModuleNo+i, (n1PosY-1) * 4 + n1PosX + i, gData.sCarID_GoodTray[n1NSNo-7]);
				g_objLogFile.Save_OutTrayGLog(n1UP+1, n1UT+1, n1UM+n1ModuleNo+i, n1NSNo-7);
			}

			m_sLog.Format("[UnloadPicker:%d Job:%d Option:%d] D[%d-%d-%d-%s] P(%d-%d) => (%d,%d-%0.3lf,%0.3lf)", n1No, (n1NSNo-4), m_pEquipData->bUseAlign2, n1UP, n1UT, n1UM, gLot.sLotID[n1UP], n1ModuleNo, gData.nUnloadPick1DownCnt, n1PosX, n1PosY, d1PosX, d1PosY);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 30; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;

	case 40:
		if (g_objCommon.Get_UnloadPickerUp(n1No)) {

			if (Check_GooodTrayFull()) 
			{
				
				if (m_nGoodStage1Case == 30) m_nGoodStage1Case = 31;
				if (m_nGoodStage2Case == 30) m_nGoodStage2Case = 31;
			}
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z1, 0);
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_P1, 0);

			CString sLotID  = gData.sLotID_UnloadPicker[n1No-1];
			int		nPortNo = gData.nPortNo_UnloadPicker[n1No-1];
			gData.sLotID_UnloadPicker[n1No-1] = "";
			gData.nTrayNo_UnloadPicker[n1No-1] = gData.nPortNo_UnloadPicker[n1No-1] = 0;
			for(int i=0; i<10; i++) gData.InfoUnloadPick[n1No-1][i] = 0;

			if (Check_LotEndUnloadPicker(sLotID, nPortNo))
			{				
				if (m_nGoodStage1Case == 30) m_nGoodStage1Case = 31;
				if (m_nGoodStage2Case == 30) m_nGoodStage2Case = 31;
			}

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 41:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z1, 0) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P1, 0) ) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Z1, 0);	g_objCommon.Save_Motion(AX_UNLOAD_PICKER_P1, 0); 
			m_tUnloadPicker1Loop.Takt_Save(18, 20); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y1, 0);
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z1, 0);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y1, 0)) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y1, 0);
			m_tUnloadPicker1Loop.Takt_Save(18, 21); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_X1, 1);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;
	case 43:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_X1, 1) ) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_X1, 1);
			m_tUnloadPicker1Loop.Takt_Save(18, 22); m_tUnloadPicker1Loop.Takt_Start(); 

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 50; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;

	case 50:
		if (m_nUnloadPicker2Case > 11 && m_nUnloadPicker2Case < 50) {
			m_tUnloadPicker1Loop.Takt_Save(18, 23); m_tUnloadPicker1Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y1, 1);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 51:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y1, 1)) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y1, 1);
			m_tUnloadPicker1Loop.Takt_Save(18, 24);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 0; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;

	case 81:
		if (g_objCommon.Get_UnloadPickerUp(n1No)) {
			if (g_objCommon.Get_UnloadPickerCheck(n1No, 9, gData.InfoUnloadPick[n1No-1])) {
				m_nUnloadPicker1Case = 7; m_tUnloadPicker1Loop.Set_LoopTime(30000);
			} else {
				for(int i=0; i<8; i++) {
					if (gData.nUnloadPicker1ErrNo[i] == 1) {
						g_objCommon.Set_UnloadPickerOpen(n1No, i+1);
					}
				}
				m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 82:
		if (g_objCommon.Get_UnloadPickerUp(n1No)) 
		{
			if (!m_tUnloadPicker1Loop.Waiting_Time(500)) break;
			g_objCommon.Set_UnloadPickerDown(n1No);

			m_sLog.Format("Seq,18,UnloadPicker1,%d,explanation",m_nUnloadPicker1Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker1Case = 4; m_tUnloadPicker1Loop.Set_LoopTime(30000);
		}
		break;

	}

	// 18. (Error : 7000)
	if (m_tUnloadPicker1Loop.Over_LoopTime()) 
	{
		if (m_nUnloadPicker1Case == 12) {
			Set_InspectRequest(1, gData.nPortNo_UnloadPicker[0], gData.nTrayNo_UnloadPicker[0]);
			m_nUnloadPicker1Case++; m_tUnloadPicker1Loop.Set_LoopTime(10000);
			return TRUE;
		}
		if (m_nUnloadPicker1Case == 13 && m_pEquipData->bUseAutoSkip) 
		{
			Set_ReInspectDone(1, gData.nPortNo_UnloadPicker[0], gData.nTrayNo_UnloadPicker[0]);
			m_tUnloadPicker1Loop.Set_LoopTime(30000);
			return TRUE;
		}
		g_objCommon.Show_Error(7000 + m_nUnloadPicker1Case);
		return FALSE;
	}
	return TRUE;
}

// 19. (Error : 7500)
BOOL CSequenceMain::Run_UnloadPicker2()
{
	static int n2No = 2;	//2고정
	static int n2VNo, n2NSNo, n2ModuleNo, n2PosX, n2PosY, n2UP, n2UT, n2UM, nJudgeNo2;
	static double d2PosX, d2PosY;

	switch (m_nUnloadPicker2Case) {
	case 0:	// Wait
		n2VNo = 0;
		if (m_nVisionStage1Case == 30) n2VNo = 1;
		if (m_nVisionStage2Case == 30) n2VNo = 2;
		if (m_nVisionStage3Case == 30) n2VNo = 3;
		if (m_nVisionStage4Case == 30) n2VNo = 4;
		if (n2VNo > 0) {
			if (gData.nTrayNo_VisionStage[n2VNo-1] > 0) 
			{
				m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(5000);

				m_sLog.Format("[UnloadPicker2: Start] Inspection StageNo(%d)", n2VNo);
				g_objLogFile.Save_HandlerLog(m_sLog);
			} else {
				if (m_nVisionStage1Case == 30) m_nVisionStage1Case = 31;
				if (m_nVisionStage2Case == 30) m_nVisionStage2Case = 31;
				if (m_nVisionStage3Case == 30) m_nVisionStage3Case = 31;
				if (m_nVisionStage4Case == 30) m_nVisionStage4Case = 31;
			}
		}
		if (Check_EmptyUnloadPicker()) {
			if (m_nLDCVElevatorCase == 0 && m_nLDMZElevatorCase == 0) {
				if ((m_nLoadStage1Case == 0 && m_nLoadStage2Case == 50) || (m_nLoadStage1Case == 50 && m_nLoadStage2Case == 0)) {
					if (m_nNGStage1Case == 30) m_nNGStage1Case = 31;
					if (m_nNGStage2Case == 30) m_nNGStage2Case = 31;
				}
			}
			if (m_nGoodStage1Case == 30) m_nGoodStage1Case = 31;
			if (m_nGoodStage2Case == 30) m_nGoodStage2Case = 31;
		}
		return TRUE;

	case 1:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z2, 0)) 
		{
			if (g_objCommon.Get_UnloadPickerUp(n2No)) 
			{
				g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Z2, 0);
				g_objCommon.Move_Position(AX_UNLOAD_PICKER_X2, n2VNo);
				g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y2, n2VNo);
				g_objCommon.Move_Position(AX_UNLOAD_PICKER_P2, 0);

				m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 2:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_X2, n2VNo) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y2, n2VNo) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P2, 0)) 
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_X2, n2VNo);	g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y2, n2VNo);	g_objCommon.Save_Motion(AX_UNLOAD_PICKER_P2, 0);
			m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerOpen(n2No);
			g_objCommon.Set_UnloadPickerDown(n2No);

			gData.sLotID_UnloadPicker[n2No-1]  = gData.sLotID_VisionStage[n2VNo-1];
			gData.nTrayNo_UnloadPicker[n2No-1] = gData.nTrayNo_VisionStage[n2VNo-1];
			gData.nPortNo_UnloadPicker[n2No-1] = gData.nPortNo_VisionStage[n2VNo-1];
			gData.sLotID_UnloadPicker[2]	   = gData.sLotID_UnloadPicker[n2No-1];

			for(int i=0; i<10; i++) 
			{
				gData.InfoUnloadPick[n2No-1][i] = gData.InfoVision[n2VNo-1][i];	gData.InfoVision[n2VNo-1][i] = 0;
				
			}

			for(int i=0; i<PICK_CNT; i++) 
			{
				gTracking.sMZID_UnloadPicker[n2No-1][i] = gTracking.sMZID_InspectStage[n2VNo-1][i];	gTracking.sMZID_InspectStage[n2VNo-1][i] = "";
				m_sLog.Format("InspectStage %d To UnloadPicker %d, pocket:%d - MZ ID:%s",n2VNo, n2No, i , gTracking.sMZID_UnloadPicker[n2No-1][i]);
				if(gTracking.sMZID_UnloadPicker[n2No-1][i] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);			
			}

			gData.sLotID_VisionStage[n2VNo-1] = "";
			gData.nTrayNo_VisionStage[n2VNo-1] = gData.nPortNo_VisionStage[n2VNo-1] = 0;

			n2UP = m_nData[1][0] = gData.nPortNo_UnloadPicker[n2No-1] - 1;
			n2UT = m_nData[1][1] = gData.nTrayNo_UnloadPicker[n2No-1] - 1;
			n2UM = m_nData[1][2] = gData.InfoUnloadPick[n2No-1][9] - 1;
			for(int i=n2UM; (i<40 || i<n2UM+8); i++) gLot.nHistory[n2UP][n2UT][i][4] = 2;
			
			m_sLog.Format("UnloadPicker Data [%d-%d-%d-%d-%s-%d]", n2No, n2UP+1, n2UT+1, n2UM+1, gLot.sLotID[n2UP], n2VNo);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 3:
		if (g_objCommon.Get_UnloadPickerDown(n2No)) 
		{
			m_tUnloadPicker2Loop.Takt_Save(19, 1); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z2, n2VNo);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 4:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z2, n2VNo)) 
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Z2, n2VNo);
			m_tUnloadPicker2Loop.Takt_Save(19, 2); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_VisionVacuumOff(n2VNo);
			g_objCommon.Set_UnloadPickerClose(n2No);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 5:
		if (g_objCommon.Get_UnloadPickerClose(n2No)) 
		{
			if (!m_tUnloadPicker2Loop.Waiting_Time(m_pEquipData->nDelayTime[8])) break;
			m_tUnloadPicker2Loop.Takt_Save(19, 3); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z2, 0);
			g_objCommon.Set_UnloadPickerUp(n2No);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 6:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z2, 0)) 
		{
			if (g_objCommon.Get_UnloadPickerUp(n2No)) {
				g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Z2, 0);
				m_tUnloadPicker2Loop.Takt_Save(19, 4); m_tUnloadPicker2Loop.Takt_Start(); 

				m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 7:
		if (g_objCommon.Get_UnloadPickerCheck(n2No, 9, gData.InfoUnloadPick[n2No-1])) {
			m_tUnloadPicker2Loop.Takt_Save(19, 5); m_tUnloadPicker2Loop.Takt_Start(); 
			if (m_nVisionStage1Case == 30) m_nVisionStage1Case = 31;
			if (m_nVisionStage2Case == 30) m_nVisionStage2Case = 31;
			if (m_nVisionStage3Case == 30) m_nVisionStage3Case = 31;
			if (m_nVisionStage4Case == 30) m_nVisionStage4Case = 31;

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 10; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;

	case 10:
		if (m_nUnloadPicker1Case > 33 && m_nUnloadPicker1Case < 70) 
		{
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_X2, 5);
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_P2, 1);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 11:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_X2, 5) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P2, 1)) 
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_X2, 5);
			gAlm.sAlmLID[0] = gData.sLotID_UnloadPicker[n2No-1]; gAlm.nAlmTNo[0] = gData.nTrayNo_UnloadPicker[n2No-1]; gAlm.nAlmPNo[0] = gData.InfoUnloadPick[n2No-1][9];

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(m_pEquipData->nTimeOver[0]-10000);
		}
		break;
	case 12:
		if (Check_InspectDone(n2No, gData.nPortNo_UnloadPicker[n2No-1], gData.nTrayNo_UnloadPicker[n2No-1])) 
		{
			m_tUnloadPicker2Loop.Takt_Save(19, 6); m_tUnloadPicker2Loop.Takt_Start(); 
			Set_ROSTime(n2No, gData.nPortNo_UnloadPicker[n2No-1], gData.nTrayNo_UnloadPicker[n2No-1]);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 15; m_tUnloadPicker2Loop.Set_LoopTime(300000);	//5분
		}
		break;
	case 13:
		if (Check_InspectDone(n2No, gData.nPortNo_UnloadPicker[n2No-1], gData.nTrayNo_UnloadPicker[n2No-1])) {
			m_tUnloadPicker2Loop.Takt_Save(19, 6); m_tUnloadPicker2Loop.Takt_Start(); 
			Set_ROSTime(n2No, gData.nPortNo_UnloadPicker[n2No-1], gData.nTrayNo_UnloadPicker[n2No-1]);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 15; m_tUnloadPicker2Loop.Set_LoopTime(300000);	//5분
		}
		break;

	case 15:
		if (Check_ROSDone(n2No, gData.nPortNo_UnloadPicker[n2No-1], gData.nTrayNo_UnloadPicker[n2No-1])) {
			m_tUnloadPicker2Loop.Takt_Save(19, 7); m_tUnloadPicker2Loop.Takt_Start(); 

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 19; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;

	case 19:
		int nType;
		n2ModuleNo = Check_NGExist(n2No, nType);
		nJudgeNo2 = nType;
		if (n2ModuleNo > 0) 
		{
			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 21; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		} 
		else 
		{
			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 20:
		if (m_nUnloadPicker1Case < 20 || (m_nUnloadPicker1Case > 42 && m_nUnloadPicker1Case < 70)) {
			if (Check_MZEndUnload(gData.sLotID_UnloadPicker[n2No-1], gData.nPortNo_UnloadPicker[n2No-1], n2No)) 
			{
				
				if (m_nNGStage1Case == 30) m_nNGStage1Case = 31;
				if (m_nNGStage2Case == 30) m_nNGStage2Case = 31;
				m_sLog.Format("Unload Picker:%d MZ-End Pno[%d] LotID[%s] Seq[%d-%d]", n2No, gData.nPortNo_UnloadPicker[n2No-1], gData.sLotID_UnloadPicker[n2No-1], m_nNGStage1Case, m_nNGStage2Case);
				g_objLogFile.Save_HandlerLog(m_sLog);
			}
			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 30; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 21:
		if (m_nUnloadPicker1Case < 20 || (m_nUnloadPicker1Case > 42 && m_nUnloadPicker1Case < 70)) 
		{
			n2NSNo = 0;
//			if (m_nNGStage1Case == 30 && !m_pDX11->iNGStage1TrayExist && gData.nPortNo_NGTray[0] < 1) m_nNGStage1Case = 31;
//			if (m_nNGStage2Case == 30 && !m_pDX11->iNGStage2TrayExist && gData.nPortNo_NGTray[1] < 1) m_nNGStage2Case = 31;
			if (m_nNGStage1Case == 30 && gNG->nTrayOX[1][0] == 0) m_nNGStage1Case = 31;
			if (m_nNGStage2Case == 30 && gNG->nTrayOX[1][1] == 0) m_nNGStage2Case = 31;
			if (m_nNGStage1Case == 30) n2NSNo = 5;
			if (m_nNGStage2Case == 30) n2NSNo = 6;
			if (n2NSNo > 0) 
			{
				if (m_nGoodStage1Case == 30 || m_nGoodStage2Case == 30) 
				{
					if (Select_NGTrayPoketNo(nJudgeNo2, n2PosX, n2PosY))
					{
						g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y2, n2NSNo);

						m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
						m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
					} 
					else 
					{
						if (Check_EmptyNGTray()) 
						{
							gAlm.sAlmLotID[0].Format("%d", nJudgeNo2);
							g_objCommon.Show_Error(7591);
							return FALSE;
						}
						if (m_nNGStage1Case == 30) m_nNGStage1Case = 31;
						if (m_nNGStage2Case == 30) m_nNGStage2Case = 31;
					}
				}
			}
		}
		return TRUE;
	case 22:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y2, n2NSNo)) {
			if (Get_TrayPosition(n2No, (n2NSNo-4), n2PosX, n2PosY, d2PosX, d2PosY)) {
				g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y2, n2NSNo);
				m_tUnloadPicker2Loop.Takt_Save(19, 8); m_tUnloadPicker2Loop.Takt_Start(); 
				if (n2PosX < 3) d2PosX = d2PosX + (m_pEquipData->dTrayPitchX * (n2ModuleNo - 1));
				else			d2PosX = d2PosX + (m_pEquipData->dTrayPitchX * (n2ModuleNo - 1)) - CARRIER_OFFSETX;
				if (n2PosY > 5) d2PosY = d2PosY + CARRIER_OFFSETY;
				g_objAJinAXL.Move_Absolute(AX_UNLOAD_PICKER_X2, d2PosX);
				if (n2NSNo==5) g_objAJinAXL.Move_Absolute(AX_NG_STAGE_Y1, d2PosY);
				if (n2NSNo==6) g_objAJinAXL.Move_Absolute(AX_NG_STAGE_Y2, d2PosY);

				m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);

				m_sLog.Format("A2 Data-NG [Picker:%d Job:%d Opt:%d] D[%d-%d-%d-%s] Type(%d) MNo(%d) XY(%d-%d) PosXY(%0.3lf-%0.3lf)", n2No, (n2NSNo-4), m_pEquipData->bUseNGSort, n2UP, n2UT, n2UM, gLot.sLotID[n2UP], nJudgeNo2, n2ModuleNo, n2PosX, n2PosY, d2PosX, d2PosY);
				g_objLogFile.Save_HandlerLog(m_sLog);

			}
		}
		break;
	case 23:
		if (g_objAJinAXL.Is_MoveDone(AX_UNLOAD_PICKER_X2, d2PosX))
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_X2, -1, d2PosX);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 24:
		if ((n2NSNo==5 && g_objAJinAXL.Is_MoveDone(AX_NG_STAGE_Y1, d2PosY)) ||
			(n2NSNo==6 && g_objAJinAXL.Is_MoveDone(AX_NG_STAGE_Y2, d2PosY)) ) 
		{
			if (n2NSNo==5) g_objCommon.Save_Motion(AX_NG_STAGE_Y1, -1, d2PosY);
			if (n2NSNo==6) g_objCommon.Save_Motion(AX_NG_STAGE_Y2, -1, d2PosY);
			m_tUnloadPicker2Loop.Takt_Save(19, 9); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z2, n2NSNo);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 25:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z2, n2NSNo))
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Z2, n2NSNo);
			m_tUnloadPicker2Loop.Takt_Save(19, 10); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerDown(n2No, n2ModuleNo);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 26:
		if (g_objCommon.Get_UnloadPickerDown(n2No, n2ModuleNo))
		{
			m_tUnloadPicker2Loop.Takt_Save(19, 11); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerOpen(n2No, n2ModuleNo);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 27:
		if (g_objCommon.Get_UnloadPickerOpen(n2No, n2ModuleNo)) 
		{
			if (!m_tUnloadPicker2Loop.Waiting_Time(m_pEquipData->nDelayTime[7])) break;
			m_tUnloadPicker2Loop.Takt_Save(19, 12); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerUp(n2No);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 28:
		if (g_objCommon.Get_UnloadPickerUp(n2No)) 
		{
			m_tUnloadPicker2Loop.Takt_Save(19, 13); m_tUnloadPicker2Loop.Takt_Start(); 
			if (n2UP < 0 || n2UP > 29) {
				n2UP = m_nData[1][0];	n2UT = m_nData[1][1];	n2UM = m_nData[1][2];
			}
			if (gLot.sLotID[n2UP].GetLength() < 1) {
				gLot.sLotID[n2UP] = gNG->sBackData[n2UP][0];
				gLot.sRecipeName[n2UP] = gNG->sBackData[n2UP][1];
				gLot.nCmCount[n2UP] = gNG->nBackCnt[n2UP];
				m_sLog.Format("[UnloadPicker:%d] DN[%d-%s]", n2No, n2UP+1, gLot.sLotID[n2UP]);
				g_objLogFile.Save_HandlerLog(m_sLog);
			}

			gLot.nNgCount[n2UP]++;	gLot.nTrayPutCnt[n2UP][1]++;
			if (Check_EmptyNGTray()) gLot.nOutTrayCnt[n2UP][1]++;

			gData.sLotID_NGTray[n2NSNo-5]  = gData.sLotID_UnloadPicker[n2No-1];
			gData.nTrayNo_NGTray[n2NSNo-5] = gData.nTrayNo_UnloadPicker[n2No-1];
			gData.nPortNo_NGTray[n2NSNo-5] = gData.nPortNo_UnloadPicker[n2No-1];
			gData.sLotID_NGTray[2]  = gData.sLotID_NGTray[n2NSNo-5];

			gData.InfoNgTray[n2PosY-1][n2PosX-1] = gData.InfoUnloadPick[n2No-1][n2ModuleNo-1];
			gData.InfoUnloadPick[n2No-1][n2ModuleNo-1] = 0;

			gTracking.sMZID_NGTray[n2PosY-1][n2PosX-1] = gTracking.sMZID_UnloadPicker[n2No-1][n2ModuleNo-1];
			gTracking.sMZID_UnloadPicker[n2No-1][n2ModuleNo-1] = "";
			m_sLog.Format("UnloadPicker %d To NGTray, Y:%d X:%d - MZ ID:%s",n2No, n2PosY, n2PosX , gTracking.sMZID_NGTray[n2PosY-1][n2PosX-1]);
			if(gTracking.sMZID_NGTray[n2PosY-1][n2PosX-1] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);

			g_dlgOCAPFai.Set_AddDEFECT(gData.sMZID[7], gLot.sNGCode_I[n2UP][n2UT][n2UM+n2ModuleNo-1][0]);
			g_dlgOCAPCosmetic.AddModuleToCarrier(gData.nPortNo_NGTray[n2NSNo-5]-1, "NG",gData.InfoNgTray[n2PosY-1][n2PosX-1], gLot.sNGCode_I[n2UP][n2UT][n2UM+n2ModuleNo-1][0], gLot.sNGCode_Vision[n2UP][n2UT][n2UM+n2ModuleNo-1][0], gCap.nCarrierMZIndex[1]);
		
			gLot.nHistory[n2UP][n2UT][n2UM+n2ModuleNo-1][6] = 1;
			gLot.nHistory[n2UP][n2UT][n2UM+n2ModuleNo-1][7] = (n2PosY-1) * 4 + n2PosX;
			gLot.nHistory[n2UP][n2UT][n2UM+n2ModuleNo-1][8] = n2NSNo-4;
			g_objMesAgent.Set_CmEnd(1, n2UP+1, n2UT+1, n2UM+n2ModuleNo, (n2PosY-1) * 4 + n2PosX, gData.sCarID_NGTray[n2NSNo-5]);
			g_objLogFile.Save_OutTrayNLog(n2UP+1, n2UT+1, n2UM+n2ModuleNo, n2NSNo-5); 

			if (m_nGoodStage1Case == 30 && gData.nPortNo_GoodTray[0] == 0)			 gData.nPortNo_GoodTray[0] = gData.nPortNo_NGTray[n2NSNo-5];
			if (m_nGoodStage1Case == 30 && gData.sLotID_GoodTray[0].GetLength() < 2) gData.sLotID_GoodTray[0] = gData.sLotID_UnloadPicker[n2No-1];
			if (m_nGoodStage2Case == 30 && gData.nPortNo_GoodTray[1] == 0)			 gData.nPortNo_GoodTray[1] = gData.nPortNo_NGTray[n2NSNo-5];
			if (m_nGoodStage2Case == 30 && gData.sLotID_GoodTray[1].GetLength() < 2) gData.sLotID_GoodTray[1] = gData.sLotID_UnloadPicker[n2No-1];

			if (Check_NGTrayFull()) 
			{				
				if (m_nNGStage1Case == 30) m_nNGStage1Case = 31;
				if (m_nNGStage2Case == 30) m_nNGStage2Case = 31;
			}

			m_sLog.Format("[UnloadPicker:%d Job:%d Option:%d] D[%d-%d-%d-%s] P(%d-1) Type(%d) => (%d,%d-%0.3lf,%0.3lf)", 
							n2No, (n2NSNo-4), m_pEquipData->bUseAlign2, n2UP, n2UT, n2UM, gLot.sLotID[n2UP], n2ModuleNo, gData.InfoNgTray[n2PosY-1][n2PosX-1], n2PosX, n2PosY, d2PosX, d2PosY);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 19; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;

	case 29:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y2, 0))
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y2, 0);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 40; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 30:
		n2ModuleNo = Check_GoodExist(n2No, n2PosX, n2PosY);
		if (n2ModuleNo > 0) 
		{
			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		else
		{
			if (Check_LotEndUnloadNG(gData.sLotID_UnloadPicker[n2No-1], gData.nPortNo_UnloadPicker[n2No-1], n2No)) 
			{				
//				if (m_nNGStage1Case == 30)   m_nNGStage1Case = 31;
//				if (m_nNGStage2Case == 30)   m_nNGStage2Case = 31;
				if (m_nGoodStage1Case == 30) m_nGoodStage1Case = 31;
				if (m_nGoodStage2Case == 30) m_nGoodStage2Case = 31;
			}
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z2, 0);
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y2, 0);
			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 29; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 31:
		if (m_nUnloadPicker1Case < 30 || (m_nUnloadPicker1Case > 42 && m_nUnloadPicker1Case < 70)) {
			if (g_objAJinAXL.Is_Done(AX_UNLOAD_PICKER_X2)) {
				n2NSNo = 0;
//				if (m_nGoodStage1Case == 30 && !m_pDX12->iGoodStage1TrayExist && gData.nTrayNo_GoodTray[0] < 1) m_nGoodStage1Case = 31;
//				if (m_nGoodStage2Case == 30 && !m_pDX12->iGoodStage2TrayExist && gData.nTrayNo_GoodTray[1] < 1) m_nGoodStage2Case = 31;
				if (m_nGoodStage1Case == 30 && gNG->nTrayOX[2][0] == 0) m_nGoodStage1Case = 31;
				if (m_nGoodStage2Case == 30 && gNG->nTrayOX[2][1] == 0) m_nGoodStage2Case = 31;
				if (m_nGoodStage1Case == 30) n2NSNo = 7;
				if (m_nGoodStage2Case == 30) n2NSNo = 8;
				if (n2NSNo > 0) {
					if (Select_GoodTrayPoketNo(n2UP, n2UT, n2PosX, n2PosY)) {
						g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y2, n2NSNo);
						g_objCommon.Move_Position(AX_UNLOAD_PICKER_P2, 2);
						m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
					} else {
						if (m_nGoodStage1Case == 30) m_nGoodStage1Case = 31;
						if (m_nGoodStage2Case == 30) m_nGoodStage2Case = 31;
					}
				} else {
					if (!g_objCommon.Check_Position(AX_UNLOAD_PICKER_X2, 7)) g_objCommon.Move_Position(AX_UNLOAD_PICKER_X2, 7);
				}
			}
		}
		return TRUE;
	case 32:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y2, n2NSNo) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P2, 2)) {
			if (Get_TrayPosition(n2No, (n2NSNo-4), n2PosX, n2PosY, d2PosX, d2PosY)) {
				g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y2, n2NSNo);
				m_tUnloadPicker2Loop.Takt_Save(19, 14); m_tUnloadPicker2Loop.Takt_Start(); 
				if (n2PosX < 3) d2PosX = d2PosX + (m_pEquipData->dTrayPitchX * (n2ModuleNo - 1));
				else			d2PosX = d2PosX + (m_pEquipData->dTrayPitchX * (n2ModuleNo - 1)) - CARRIER_OFFSETX;
				if (n2PosY > 5) d2PosY = d2PosY + CARRIER_OFFSETY;
				g_objAJinAXL.Move_Absolute(AX_UNLOAD_PICKER_X2, d2PosX);
				if (n2NSNo==7) g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE_Y1, d2PosY);
				if (n2NSNo==8) g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE_Y2, d2PosY);

				m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);

				m_sLog.Format("A2 Data-GD D[%d-%d-%d-%s] [Picker:%d Job:%d] XY(%d,%d) PosXY(%0.3lf,%0.3lf) PNo(%d)", n2UP, n2UT, n2UM, gLot.sLotID[n2UP], n2No, (n2NSNo-4), n2PosX, n2PosY, d2PosX, d2PosY, n2ModuleNo);
				g_objLogFile.Save_HandlerLog(m_sLog);
			}
		}
		break;
	case 33:
		if (g_objAJinAXL.Is_MoveDone(AX_UNLOAD_PICKER_X2, d2PosX)) 
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_X2, -1, d2PosX);
			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 34:
		if ((n2NSNo==7 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE_Y1, d2PosY)) ||
			(n2NSNo==8 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE_Y2, d2PosY)) ) 
		{
			if (n2NSNo==7) g_objCommon.Save_Motion(AX_GOOD_STAGE_Y1, -1, d2PosY);
			if (n2NSNo==8) g_objCommon.Save_Motion(AX_GOOD_STAGE_Y2, -1, d2PosY);
			m_tUnloadPicker2Loop.Takt_Save(19, 15); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z2, n2NSNo);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 35:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z2, n2NSNo)) 
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Z2, n2NSNo);
			m_tUnloadPicker2Loop.Takt_Save(19, 16); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerDown(n2No, 9, n2ModuleNo, n2PosX, gData.InfoUnloadPick[n2No-1]);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 36:
		if (g_objCommon.Get_UnloadPickerDown(n2No, 9, n2ModuleNo, n2PosX, gData.InfoUnloadPick[n2No-1])) 
		{
			m_tUnloadPicker2Loop.Takt_Save(19, 17); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerOpen(n2No, 9, n2ModuleNo, n2PosX, gData.InfoUnloadPick[n2No-1]);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 37:
		if (g_objCommon.Get_UnloadPickerOpen(n2No, 9, n2ModuleNo, n2PosX, gData.InfoUnloadPick[n2No-1])) 
		{
			if (!m_tUnloadPicker2Loop.Waiting_Time(m_pEquipData->nDelayTime[7])) break;
			m_tUnloadPicker2Loop.Takt_Save(19, 18); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Set_UnloadPickerUp(n2No);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 38:
		if (g_objCommon.Get_UnloadPickerUp(n2No)) 
		{
			m_tUnloadPicker2Loop.Takt_Save(19, 19); m_tUnloadPicker2Loop.Takt_Start(); 
			if (n2UP < 0 || n2UP > 29) {
				n2UP = m_nData[1][0];	n2UT = m_nData[1][1];	n2UM = m_nData[1][2];
			}
			if (gLot.sLotID[n2UP].GetLength() < 1) {
				gLot.sLotID[n2UP] = gNG->sBackData[n2UP][0];
				gLot.sRecipeName[n2UP] = gNG->sBackData[n2UP][1];
				gLot.nCmCount[n2UP] = gNG->nBackCnt[n2UP];
				m_sLog.Format("[UnloadPicker:%d] DG[%d-%s]", n2No, n2UP+1, gLot.sLotID[n2UP]);
				g_objLogFile.Save_HandlerLog(m_sLog);
			}

			if (Check_EmptyGoodTray()) {
				if (m_pEquipData->bUseGoodSort) gLot.nOutTrayCnt[n2UP][0] = n2UT+1;
				else							gLot.nOutTrayCnt[n2UP][0]++;
			}

			gData.sLotID_GoodTray[n2NSNo-7]  = gData.sLotID_UnloadPicker[n2No-1];
			gData.nTrayNo_GoodTray[n2NSNo-7] = gData.nTrayNo_UnloadPicker[n2No-1];
			gData.nPortNo_GoodTray[n2NSNo-7] = gData.nPortNo_UnloadPicker[n2No-1];
			gData.sLotID_GoodTray[2]  = gData.sLotID_GoodTray[n2NSNo-7];

			for(int i=0; i<gData.nUnloadPick2DownCnt; i++) 
			{
				gLot.nGoodCount[n2UP]++;	gLot.nTrayPutCnt[n2UP][0]++;
				gData.InfoGoodTray[n2PosY-1][n2PosX-1+i] = gData.InfoUnloadPick[n2No-1][n2ModuleNo-1+i];
				gData.InfoUnloadPick[n2No-1][n2ModuleNo-1+i] = 0;

				gTracking.sMZID_GoodTray[n2PosY-1][n2PosX-1+i] = gTracking.sMZID_UnloadPicker[n2No-1][n2ModuleNo-1+i];
				gTracking.sMZID_UnloadPicker[n2No-1][n2ModuleNo-1+i] = "";

				m_sLog.Format("UnloadPicker %d To GoodTray, Y:%d X:%d - MZ ID:%s",n2No, n2PosY, n2PosX , gTracking.sMZID_GoodTray[n2PosY-1][n2PosX-1]);
				if(gTracking.sMZID_GoodTray[n2PosY-1][n2PosX-1+i] != "") g_objLogFile.Save_HomeTrackingLog(m_sLog);

				m_sLog = gLot.sNGCode_Vision[n2UP][n2UT][n2UM+n2ModuleNo-1+i][0];
				g_dlgOCAPFai.Set_AddDEFECT(gData.sMZID[8], "");
				g_dlgOCAPCosmetic.AddModuleToCarrier(gData.nPortNo_GoodTray[n2NSNo-7]-1, "GOOD",0, "", gLot.sNGCode_Vision[n2UP][n2UT][n2UM+n2ModuleNo-1+i][0], gCap.nCarrierMZIndex[0]);
				
				if (gData.InfoGoodTray[n2PosY-1][n2PosX-1+i] > 0) 
				{
					if (gLot.nMarginal[n2UP][n2UM+n2ModuleNo-1+i] > 0) gData.InfoGoodTray[n2PosY-1][n2PosX-1+i] = 9;	//Marginal Set
				}
				

				gLot.nHistory[n2UP][n2UT][n2UM+n2ModuleNo-1+i][5] = 1;
				gLot.nHistory[n2UP][n2UT][n2UM+n2ModuleNo-1+i][7] = (n2PosY-1) * 4 + n2PosX + i;
				gLot.nHistory[n2UP][n2UT][n2UM+n2ModuleNo-1+i][8] = n2NSNo-6;
				g_objMesAgent.Set_CmEnd(0, n2UP+1, n2UT+1, n2UM+n2ModuleNo+i, (n2PosY-1) * 4 + n2PosX + i, gData.sCarID_GoodTray[n2NSNo-7]);
				g_objLogFile.Save_OutTrayGLog(n2UP+1, n2UT+1, n2UM+n2ModuleNo+i, n2NSNo-7);
			}

			m_sLog.Format("[UnloadPicker:%d Job:%d Option:%d] D[%d-%d-%d-%s] P(%d-%d) => (%d,%d-%0.3lf,%0.3lf)", n2No, (n2NSNo-4), m_pEquipData->bUseAlign2, n2UP, n2UT, n2UM, gLot.sLotID[n2UP], n2ModuleNo, gData.nUnloadPick2DownCnt, n2PosX, n2PosY, d2PosX, d2PosY);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 30; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;

	case 40:
		if (g_objCommon.Get_UnloadPickerUp(n2No))
		{
			if (Check_GooodTrayFull())
			{
				if (m_nGoodStage1Case == 30) m_nGoodStage1Case = 31;
				if (m_nGoodStage2Case == 30) m_nGoodStage2Case = 31;
			}
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z2, 0);
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_P2, 0);

			CString sLotID  = gData.sLotID_UnloadPicker[n2No-1];
			int		nPortNo = gData.nPortNo_UnloadPicker[n2No-1];
			gData.sLotID_UnloadPicker[n2No-1] = "";
			gData.nTrayNo_UnloadPicker[n2No-1] = gData.nPortNo_UnloadPicker[n2No-1] = 0;
			for(int i=0; i<10; i++) gData.InfoUnloadPick[n2No-1][i] = 0;

			if (Check_LotEndUnloadPicker(sLotID, nPortNo)) 
			{
				if (m_nGoodStage1Case == 30) m_nGoodStage1Case = 31;
				if (m_nGoodStage2Case == 30) m_nGoodStage2Case = 31;
			}

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 41:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Z2, 0) && g_objCommon.Check_Position(AX_UNLOAD_PICKER_P2, 0) ) 
		{
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Z2, 0);
			m_tUnloadPicker2Loop.Takt_Save(19, 20); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Z2, 0);
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y2, 0);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y2, 0)) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y2, 0);
			m_tUnloadPicker2Loop.Takt_Save(19, 21); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_X2, 1);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;
	case 43:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_X2, 1) ) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_X2, 1);
			m_tUnloadPicker2Loop.Takt_Save(19, 22); m_tUnloadPicker2Loop.Takt_Start(); 

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 50; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;

	case 50:
		if (m_nUnloadPicker1Case > 11 && m_nUnloadPicker1Case < 50) {
			m_tUnloadPicker2Loop.Takt_Save(19, 23); m_tUnloadPicker2Loop.Takt_Start(); 
			g_objCommon.Move_Position(AX_UNLOAD_PICKER_Y2, 1);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 51:
		if (g_objCommon.Check_Position(AX_UNLOAD_PICKER_Y2, 1)) {
			g_objCommon.Save_Motion(AX_UNLOAD_PICKER_Y2, 1);
			m_tUnloadPicker2Loop.Takt_Save(19, 24);

			m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
			m_nUnloadPicker2Case = 0; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;

	case 81:
		if (g_objCommon.Get_UnloadPickerUp(n2No)) {
			if (g_objCommon.Get_UnloadPickerCheck(n2No, 9, gData.InfoUnloadPick[n2No-1])) {
				m_nUnloadPicker2Case = 7; m_tUnloadPicker2Loop.Set_LoopTime(30000);
			} else {
				for(int i=0; i<8; i++) {
					if (gData.nUnloadPicker2ErrNo[i] == 1) {
						g_objCommon.Set_UnloadPickerOpen(n2No, i+1);
					}
				}

				m_sLog.Format("Seq,19,UnloadPicker2,%d,explanation",m_nUnloadPicker2Case); g_objLogFile.Save_SeqLog(m_sLog);
				m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 82:
		if (g_objCommon.Get_UnloadPickerUp(n2No)) {
			if (!m_tUnloadPicker2Loop.Waiting_Time(500)) break;
			g_objCommon.Set_UnloadPickerDown(n2No);
			m_nUnloadPicker2Case = 4; m_tUnloadPicker2Loop.Set_LoopTime(30000);
		}
		break;

	}

	// 19. (Error : 7500)
	if (m_tUnloadPicker2Loop.Over_LoopTime()) {
		if (m_nUnloadPicker2Case == 12) {
			Set_InspectRequest(2, gData.nPortNo_UnloadPicker[1], gData.nTrayNo_UnloadPicker[1]);
			m_nUnloadPicker2Case++; m_tUnloadPicker2Loop.Set_LoopTime(10000);
			return TRUE;
		}
		if (m_nUnloadPicker2Case == 13 && m_pEquipData->bUseAutoSkip) {
			Set_ReInspectDone(2, gData.nPortNo_UnloadPicker[1], gData.nTrayNo_UnloadPicker[1]);
			m_tUnloadPicker2Loop.Set_LoopTime(30000);
			return TRUE;
		}
		g_objCommon.Show_Error(7500 + m_nUnloadPicker2Case);
		return FALSE;
	}
	return TRUE;
}

// 20. (Error : 8000)
BOOL CSequenceMain::Run_NGStage1()
{
	static int	  nNStage1No      = 0;	//0고정
	static int	  nNStage1YAxisNo = AX_NG_STAGE_Y1;

	switch (m_nNGStage1Case) {
	case 0:	// Wait
		m_tNGStage1Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (m_pDX11->iNGStage1TrayExist) {
			gNG->nTrayOX[1][0] = 1;
			m_tNGStage1Loop.Takt_Start(); 
			m_pDY11->oNGStage1MasterIn = TRUE; m_pDY11->oNGStage1MasterOut = FALSE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 2:
		if (m_pDX11->iNGStage1MasterIn && !m_pDX11->iNGStage1MasterOut) {
			m_tNGStage1Loop.Takt_Save(20, 1); m_tNGStage1Loop.Takt_Start(); 
			m_pDY11->oNGStage1SlaveIn = TRUE; m_pDY11->oNGStage1SlaveOut = FALSE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 3:
		if (m_pDX11->iNGStage1SlaveIn && !m_pDX11->iNGStage1SlaveOut) {
			m_tNGStage1Loop.Takt_Save(20, 2); m_tNGStage1Loop.Takt_Start(); 
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 4:
		if (m_pDX11->iNGStage1Up && !m_pDX11->iNGStage1Down) {
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 5:	//Align Wait
		if (m_nNGStage2Case > 22) {
			if (m_pEquipData->bUseRFIDNG) m_nNGStage1Case++; 
			else						  m_nNGStage1Case = 11;
			m_tNGStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 6:
		if (m_pDX11->iNGStage1Up && !m_pDX11->iNGStage1Down) {
			m_nRFRetray[1][0] = 0;
			g_objCommon.Move_Position(nNStage1YAxisNo, 6);	//RFID
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(30000);
		}
		break;
	case 7:
		if (g_objCommon.Check_Position(nNStage1YAxisNo, 6)) 
		{
			if (m_pEquipData->bUseRFIDNG) m_nNGStage1Case++; 
			else						  m_nNGStage1Case = 11;
			g_objCarrierRFID_NG.Send_RFIDRead();
			m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 8:
		if (g_objCarrierRFID_NG.Is_RecvComplete()) {
			gData.sCarID_NGTray[nNStage1No] = g_objCarrierRFID_NG.Get_CarrierID();
			if (gData.sCarID_NGTray[nNStage1No].GetLength() > 2) 
			{
				gMes.nCarConfirm[2] = 1;
#ifndef AJIN_BOARD_USE
				gMes.nCarConfirm[2] = 2;
#endif
//				g_objMesAgent.Set_CarrierIDReport("N", "", gData.sCarID_NGTray[nNStage1No]);
				g_objLogFile.Save_RFBarData(6, gData.sCarID_NGTray[nNStage1No]);
				m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 9:
//		if (!m_pEquipData->bUseMES || gMes.nCarConfirm[2] == 2) {
			m_nNGStage1Case = 11; m_tNGStage1Loop.Set_LoopTime(30000);
//		}
		break;

	case 11:
		if (m_pDX11->iNGStage1Up && !m_pDX11->iNGStage1Down) {
			m_tNGStage1Loop.Takt_Save(20, 3); m_tNGStage1Loop.Takt_Start(); 
			g_objCommon.Move_Position(nNStage1YAxisNo, 1);	//Aling1
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(30000);
		}
		break;
	case 12:
		if (g_objCommon.Check_Position(nNStage1YAxisNo, 1))
		{
			g_objCommon.Save_Motion(nNStage1YAxisNo, 1);
			m_tNGStage1Loop.Takt_Save(20, 4); m_tNGStage1Loop.Takt_Start(); 
			m_nNGStage1Case = 20; m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	//Align Wait
		return TRUE;
	case 21:
		if (m_nNGStage2Case > 20 && m_nNGStage2Case < 40) {
			m_tNGStage1Loop.Takt_Save(20, 5); m_tNGStage1Loop.Takt_Start(); 
			g_objCommon.Move_Position(nNStage1YAxisNo, 2);	//Wait
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(30000);
		} else if (m_nNGStage2Case <= 20 || m_nNGStage2Case >= 40) {
			m_tNGStage1Loop.Takt_Save(20, 5); m_tNGStage1Loop.Takt_Start(); 
			g_objCommon.Move_Position(nNStage1YAxisNo, 3);	//Work
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 22:
		if (g_objCommon.Check_Position(nNStage1YAxisNo, 2) || g_objCommon.Check_Position(nNStage1YAxisNo, 3)) {
			if (g_objCommon.Check_Position(nNStage1YAxisNo, 2)) g_objCommon.Save_Motion(nNStage1YAxisNo, 2);
			m_tNGStage1Loop.Takt_Save(20, 6); m_tNGStage1Loop.Takt_Start(); 
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(30000);
		}
		break;
	case 23:
		if (m_nNGStage2Case <= 20 || m_nNGStage2Case >= 40) {
			g_objCommon.Move_Position(nNStage1YAxisNo, 3);	//Work
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 24:
		if (g_objCommon.Check_Position(nNStage1YAxisNo, 3)) {
			g_objCommon.Save_Motion(nNStage1YAxisNo, 3);
			m_tNGStage1Loop.Takt_Save(20, 7); m_tNGStage1Loop.Takt_Start(); 
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 25:
		if (gNG->nTrayOX[1][0] == 0 || (gNG->nTrayOX[1][0] == 1 && m_pDX11->iNGStage1TrayExist)) {
			Init_NgTray(nNStage1No);
			m_nNGStage1Case = 30; m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 30:	// Unload Picker Wait
		m_tNGStage1Loop.Set_LoopTime(5000);
		return TRUE;

	case 31:
		if (m_nNGStage2Case < 30 || m_nNGStage2Case >= 50) {
			m_tNGStage1Loop.Takt_Save(20, 8); m_tNGStage1Loop.Takt_Start(); 
			m_pDY11->oNGStage1Up = FALSE; m_pDY11->oNGStage1Down = TRUE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 32:
		if (!m_pDX11->iNGStage1Up && m_pDX11->iNGStage1Down) {
			m_tNGStage1Loop.Takt_Save(20, 9); m_tNGStage1Loop.Takt_Start(); 
			m_nNGStage1Case = 40; m_tNGStage1Loop.Set_LoopTime(30000);
		}
		break;

	case 40:	//Up Wait
		if (m_nNGStage2Case < 40 || m_nNGStage2Case >= 60) {
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 41:
		if (!m_pDX11->iNGStage1Up && m_pDX11->iNGStage1Down) {
			if (m_pDX11->iNGStage2Up && !m_pDX11->iNGStage2Down) {
				m_tNGStage1Loop.Takt_Save(20, 10); m_tNGStage1Loop.Takt_Start(); 
				g_objCommon.Move_Position(nNStage1YAxisNo, 0);	//load
				m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(nNStage1YAxisNo, 0)) {
			g_objCommon.Save_Motion(nNStage1YAxisNo, 0);
			m_tNGStage1Loop.Takt_Save(20, 11); m_tNGStage1Loop.Takt_Start(); 
			m_nNGStage1Case = 50; m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 50:
		if (m_nNGStage2Case > 22 && m_nNGStage2Case < 50) {
			m_tNGStage1Loop.Takt_Save(20, 12); m_tNGStage1Loop.Takt_Start(); 
			m_pDY11->oNGStage1Up = TRUE; m_pDY11->oNGStage1Down = FALSE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 51:
		if (m_pDX11->iNGStage1Up && !m_pDX11->iNGStage1Down) {
			m_tNGStage1Loop.Takt_Save(20, 13); m_tNGStage1Loop.Takt_Start(); 
			m_pDY11->oNGStage1SlaveIn = FALSE; m_pDY11->oNGStage1SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 52:
		if (!m_pDX11->iNGStage1SlaveIn && m_pDX11->iNGStage1SlaveOut) {
			m_tNGStage1Loop.Takt_Save(20, 14); m_tNGStage1Loop.Takt_Start(); 
			m_pDY11->oNGStage1MasterIn = FALSE; m_pDY11->oNGStage1MasterOut = TRUE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 53:
		if (!m_pDX11->iNGStage1MasterIn && m_pDX11->iNGStage1MasterOut) {
			m_tNGStage1Loop.Takt_Save(20, 15); m_tNGStage1Loop.Takt_Start(); 
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 54:
		if (gNG->nTrayOX[1][0] == 1) {
			m_nNGStage1Case++; m_tNGStage1Loop.Set_LoopTime(30000);
		} else {
			m_tNGStage1Loop.Takt_Save(20, 16);
			m_nNGStage1Case = 0; m_tNGStage1Loop.Set_LoopTime(30000);
		}
		break;
	case 55:
		if (m_pDX11->iNGStage1TrayExist) {
			if (gData.nPortNo_NGTray[0] > 0) m_nNGStage1Case = 60;
			else							 m_nNGStage1Case = 0;
			m_tNGStage1Loop.Set_LoopTime(30000);
		}
		break;

	case 60:	// Transfer Picker Wait
		m_tNGStage1Loop.Set_LoopTime(5000);
		return TRUE;

	case 61:
		if (!m_pDX11->iNGStage1TrayExist) 
		{
			gNG->nTrayOX[1][0] = 0;
			m_tNGStage1Loop.Takt_Save(20, 16);
			m_nNGStage1Case = 0; m_tNGStage1Loop.Set_LoopTime(30000);
		}
		break;

	}

	// 20. (Error : 8000)
	if (m_tNGStage1Loop.Over_LoopTime()) {
		if (m_nNGStage1Case == 8) {
			m_nNGStage1Case = 7; m_nRFRetray[1][0]++;
			if (m_nRFRetray[1][0] < 3) return TRUE;
		}
		g_objCommon.Show_Error(8000 + m_nNGStage1Case);
		return FALSE;
	}
	return TRUE;
}

// 21. (Error : 8200)
BOOL CSequenceMain::Run_NGStage2()
{
	static int	  nNStage2No	  = 1;	//1고정
	static int	  nNStage2YAxisNo = AX_NG_STAGE_Y2;

	switch (m_nNGStage2Case) {
	case 0:	// Wait
		m_tNGStage2Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (m_pDX11->iNGStage2TrayExist) {
			gNG->nTrayOX[1][1] = 1;
			m_tNGStage2Loop.Takt_Start(); 
			m_pDY11->oNGStage2MasterIn = TRUE; m_pDY11->oNGStage2MasterOut = FALSE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 2:
		if (m_pDX11->iNGStage2MasterIn && !m_pDX11->iNGStage2MasterOut) {
			m_tNGStage2Loop.Takt_Save(21, 1); m_tNGStage2Loop.Takt_Start(); 
			m_pDY11->oNGStage2SlaveIn = TRUE; m_pDY11->oNGStage2SlaveOut = FALSE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 3:
		if (m_pDX11->iNGStage2SlaveIn && !m_pDX11->iNGStage2SlaveOut) {
			m_tNGStage2Loop.Takt_Save(21, 2); m_tNGStage2Loop.Takt_Start(); 
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 4:
		if (m_pDX11->iNGStage2Up && !m_pDX11->iNGStage2Down) {
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 5:	//Align Wait
		if (m_nNGStage1Case > 22) {
			if (m_pEquipData->bUseRFIDNG) m_nNGStage2Case++; 
			else						  m_nNGStage2Case = 11;
			m_tNGStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 6:
		if (m_pDX11->iNGStage2Up && !m_pDX11->iNGStage2Down) {
			m_nRFRetray[1][1] = 0;
			g_objCommon.Move_Position(nNStage2YAxisNo, 6);	//RFID
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 7:
		if (g_objCommon.Check_Position(nNStage2YAxisNo, 6)) {
			if (m_pEquipData->bUseRFIDNG) m_nNGStage2Case++; 
			else						  m_nNGStage2Case = 11;
			g_objCarrierRFID_NG.Send_RFIDRead();
			m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 8:
		if (g_objCarrierRFID_NG.Is_RecvComplete()) {
			gData.sCarID_NGTray[nNStage2No] = g_objCarrierRFID_NG.Get_CarrierID();
			if (gData.sCarID_NGTray[nNStage2No].GetLength() > 2) {
				gMes.nCarConfirm[2] = 1;
#ifndef AJIN_BOARD_USE
				gMes.nCarConfirm[2] = 2;
#endif
//				g_objMesAgent.Set_CarrierIDReport("N", "", gData.sCarID_NGTray[nNStage2No]);
				g_objLogFile.Save_RFBarData(6, gData.sCarID_NGTray[nNStage2No]);
				m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 9:
//		if (!m_pEquipData->bUseMES || gMes.nCarConfirm[2] == 2) {
			m_nNGStage2Case = 11; m_tNGStage2Loop.Set_LoopTime(30000);
//		}
		break;

	case 11:
		if (m_pDX11->iNGStage2Up && !m_pDX11->iNGStage2Down) {
			m_tNGStage2Loop.Takt_Save(21, 3); m_tNGStage2Loop.Takt_Start(); 
			g_objCommon.Move_Position(nNStage2YAxisNo, 1);	//Aling1
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 12:
		if (g_objCommon.Check_Position(nNStage2YAxisNo, 1)) {
			g_objCommon.Save_Motion(nNStage2YAxisNo, 1);
			m_tNGStage2Loop.Takt_Save(21, 4); m_tNGStage2Loop.Takt_Start(); 
			m_nNGStage2Case = 20; m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	//Align Wait
		return TRUE;
	case 21:
		if (m_nNGStage1Case > 20 && m_nNGStage1Case < 40) {
			m_tNGStage2Loop.Takt_Save(21, 5); m_tNGStage2Loop.Takt_Start(); 
			g_objCommon.Move_Position(nNStage2YAxisNo, 2);	//Wait
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(30000);
		} else if (m_nNGStage1Case <= 20 || m_nNGStage1Case >= 40) {
			m_tNGStage2Loop.Takt_Save(21, 5); m_tNGStage2Loop.Takt_Start(); 
			g_objCommon.Move_Position(nNStage2YAxisNo, 3);	//Work
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 22:
		if (g_objCommon.Check_Position(nNStage2YAxisNo, 2) || g_objCommon.Check_Position(nNStage2YAxisNo, 3)) {
			if (g_objCommon.Check_Position(nNStage2YAxisNo, 2)) g_objCommon.Save_Motion(nNStage2YAxisNo, 2);
			m_tNGStage2Loop.Takt_Save(21, 6); m_tNGStage2Loop.Takt_Start(); 
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 23:
		if (m_nNGStage1Case <= 20 || m_nNGStage1Case >= 40) {
			g_objCommon.Move_Position(nNStage2YAxisNo, 3);	//Work
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 24:
		if (g_objCommon.Check_Position(nNStage2YAxisNo, 3)) {
			g_objCommon.Save_Motion(nNStage2YAxisNo, 3);
			m_tNGStage2Loop.Takt_Save(21, 7); m_tNGStage2Loop.Takt_Start(); 
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 25:
		if (gNG->nTrayOX[1][1] == 0 || (gNG->nTrayOX[1][1] == 1 && m_pDX11->iNGStage2TrayExist)) {
			Init_NgTray(nNStage2No);
			m_nNGStage2Case = 30; m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 30:	// Unload Picker Wait
		m_tNGStage2Loop.Set_LoopTime(5000);
		return TRUE;

	case 31:
		if (m_nNGStage1Case < 30 || m_nNGStage1Case >= 50) {
			m_tNGStage2Loop.Takt_Save(21, 8); m_tNGStage2Loop.Takt_Start(); 
			m_pDY11->oNGStage2Up = FALSE; m_pDY11->oNGStage2Down = TRUE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 32:
		if (!m_pDX11->iNGStage2Up && m_pDX11->iNGStage2Down) {
			m_tNGStage2Loop.Takt_Save(21, 9); m_tNGStage2Loop.Takt_Start(); 
			m_nNGStage2Case = 40; m_tNGStage2Loop.Set_LoopTime(30000);
		}
		break;

	case 40:	//Up Wait
		if (m_nNGStage1Case < 40 || m_nNGStage1Case >= 60) {
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 41:
		if (!m_pDX11->iNGStage2Up && m_pDX11->iNGStage2Down) {
			if (m_pDX11->iNGStage1Up && !m_pDX11->iNGStage1Down) {
				m_tNGStage2Loop.Takt_Save(21, 10); m_tNGStage2Loop.Takt_Start(); 
				g_objCommon.Move_Position(nNStage2YAxisNo, 0);	//load
				m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(nNStage2YAxisNo, 0)) {
			g_objCommon.Save_Motion(nNStage2YAxisNo, 0);
			m_tNGStage2Loop.Takt_Save(21, 11); m_tNGStage2Loop.Takt_Start(); 
			m_nNGStage2Case = 50; m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 50:
		if (m_nNGStage1Case > 22 && m_nNGStage1Case < 50) {
			m_tNGStage2Loop.Takt_Save(21, 12); m_tNGStage2Loop.Takt_Start(); 
			m_pDY11->oNGStage2Up = TRUE; m_pDY11->oNGStage2Down = FALSE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 51:
		if (m_pDX11->iNGStage2Up && !m_pDX11->iNGStage2Down) {
			m_tNGStage2Loop.Takt_Save(21, 13); m_tNGStage2Loop.Takt_Start(); 
			m_pDY11->oNGStage2SlaveIn = FALSE; m_pDY11->oNGStage2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 52:
		if (!m_pDX11->iNGStage2SlaveIn && m_pDX11->iNGStage2SlaveOut) {
			m_tNGStage2Loop.Takt_Save(21, 14); m_tNGStage2Loop.Takt_Start(); 
			m_pDY11->oNGStage2MasterIn = FALSE; m_pDY11->oNGStage2MasterOut = TRUE;
			g_objAJinAXL.Write_Output(11);
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 53:
		if (!m_pDX11->iNGStage2MasterIn && m_pDX11->iNGStage2MasterOut) {
			m_tNGStage2Loop.Takt_Save(21, 15); m_tNGStage2Loop.Takt_Start(); 
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 54:
		if (gNG->nTrayOX[1][1] == 1) {
			m_nNGStage2Case++; m_tNGStage2Loop.Set_LoopTime(30000);
		} else {
			m_tNGStage2Loop.Takt_Save(21, 16);
			m_nNGStage2Case = 0; m_tNGStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 55:
		if (m_pDX11->iNGStage2TrayExist) {
			if (gData.nPortNo_NGTray[1] > 0) m_nNGStage2Case = 60;
			else							 m_nNGStage2Case = 0;
			m_tNGStage2Loop.Set_LoopTime(30000);
		}
		break;

	case 60:	// Transfer Picker Wait
		m_tNGStage2Loop.Set_LoopTime(5000);
		return TRUE;

	case 61:
		if (!m_pDX11->iNGStage2TrayExist) {
			gNG->nTrayOX[1][1] = 0;
			m_tNGStage2Loop.Takt_Save(21, 16);
			m_nNGStage2Case = 0; m_tNGStage2Loop.Set_LoopTime(30000);
		}
		break;

	}

	// 21. (Error : 8200)
	if (m_tNGStage2Loop.Over_LoopTime()) {
		if (m_nNGStage2Case == 8) {
			m_nNGStage2Case = 7; m_nRFRetray[1][1]++;
			if (m_nRFRetray[1][1] < 3) return TRUE;
		}
		g_objCommon.Show_Error(8200 + m_nNGStage2Case);
		return FALSE;
	}
	return TRUE;
}

// 22. (Error : 8400)
BOOL CSequenceMain::Run_GoodStage1()
{
	static int	  nGStage1No		= 0;	//0고정
	static int	  nGStage1YAxisNo = AX_GOOD_STAGE_Y1;

	switch (m_nGoodStage1Case) {
	case 0:	// Wait
		m_tGoodStage1Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (m_pDX12->iGoodStage1TrayExist) {
			gNG->nTrayOX[2][0] = 1;
			m_tGoodStage1Loop.Takt_Start();
			m_pDY12->oGoodStage1MasterIn = TRUE; m_pDY12->oGoodStage1MasterOut = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 2:
		if (m_pDX12->iGoodStage1MasterIn && !m_pDX12->iGoodStage1MasterOut) {
			m_tGoodStage1Loop.Takt_Save(22, 1); m_tGoodStage1Loop.Takt_Start(); 
			m_pDY12->oGoodStage1SlaveIn = TRUE; m_pDY12->oGoodStage1SlaveOut = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 3:
		if (m_pDX12->iGoodStage1SlaveIn && !m_pDX12->iGoodStage1SlaveOut) {
			m_tGoodStage1Loop.Takt_Save(22, 2); m_tGoodStage1Loop.Takt_Start(); 
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 4:
		if (m_pDX12->iGoodStage1Up && !m_pDX12->iGoodStage1Down) {
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 5:	//Align Wait
		if (m_nGoodStage2Case > 22) {
			if (m_pEquipData->bUseRFIDGood) m_nGoodStage1Case++; 
			else							m_nGoodStage1Case = 11;
			m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 6:
		if (m_pDX12->iGoodStage1Up && !m_pDX12->iGoodStage1Down) {
			m_nRFRetray[2][0] = 0;
			g_objCommon.Move_Position(nGStage1YAxisNo, 6);	//RFID
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(30000);
		}
		break;
	case 7:
		if (g_objCommon.Check_Position(nGStage1YAxisNo, 6)) {
			if (m_pEquipData->bUseRFIDGood) m_nGoodStage1Case++; 
			else							m_nGoodStage1Case = 11;
			g_objCarrierRFID_Good.Send_RFIDRead();
			m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 8:
		if (g_objCarrierRFID_Good.Is_RecvComplete()) {
			gData.sCarID_GoodTray[nGStage1No] = g_objCarrierRFID_Good.Get_CarrierID();
			if (gData.sCarID_GoodTray[nGStage1No].GetLength() > 2) {
				gMes.nCarConfirm[1] = 1;
#ifndef AJIN_BOARD_USE
				gMes.nCarConfirm[1] = 2;
#endif
				g_objMesAgent.Set_CarrierIDReport("G", "", gData.sCarID_GoodTray[nGStage1No]);
				g_objLogFile.Save_RFBarData(5, gData.sCarID_GoodTray[nGStage1No]);
				m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 9:
		if (!m_pEquipData->bUseMES || gMes.nCarConfirm[1] == 2) {
			m_nGoodStage1Case = 11; m_tGoodStage1Loop.Set_LoopTime(30000);
		}
		break;

	case 11:
		if (m_pDX12->iGoodStage1Up && !m_pDX12->iGoodStage1Down) {
			m_tGoodStage1Loop.Takt_Save(22, 3); m_tGoodStage1Loop.Takt_Start(); 
			g_objCommon.Move_Position(nGStage1YAxisNo, 1);	//Aling1
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(30000);
		}
		break;
	case 12:
		if (g_objCommon.Check_Position(nGStage1YAxisNo, 1)) {
			g_objCommon.Save_Motion(nGStage1YAxisNo, 1);
			m_tGoodStage1Loop.Takt_Save(22, 4); m_tGoodStage1Loop.Takt_Start(); 
			m_nGoodStage1Case = 20; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	//Align Wait
		return TRUE;
	case 21:
		if (m_nGoodStage2Case > 20 && m_nGoodStage2Case < 40) {
			m_tGoodStage1Loop.Takt_Save(22, 5); m_tGoodStage1Loop.Takt_Start(); 
			g_objCommon.Move_Position(nGStage1YAxisNo, 2);	//Wait
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(30000);
		} else if (m_nGoodStage2Case <= 20 || m_nGoodStage2Case >= 40) {
			m_tGoodStage1Loop.Takt_Save(22, 5); m_tGoodStage1Loop.Takt_Start(); 
			g_objCommon.Move_Position(nGStage1YAxisNo, 3);	//Work
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 22:
		if (g_objCommon.Check_Position(nGStage1YAxisNo, 2) || g_objCommon.Check_Position(nGStage1YAxisNo, 3)) {
			if (g_objCommon.Check_Position(nGStage1YAxisNo, 2)) g_objCommon.Save_Motion(nGStage1YAxisNo, 2);
			m_tGoodStage1Loop.Takt_Save(22, 6); m_tGoodStage1Loop.Takt_Start(); 
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 23:
		if (m_nGoodStage2Case <= 20 || m_nGoodStage2Case >= 40) {
			if (gData.bJahwa) {
				g_objCommon.Move_Position(nGStage1YAxisNo, 3);	//Work
			} else {
				double d1PosY = m_pMoveData->dGoodStageY1[3] + (m_pEquipData->dTrayPitchY * 9) + CARRIER_OFFSETY;
				g_objAJinAXL.Move_Absolute(nGStage1YAxisNo, d1PosY);
			}
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 24:
//		if (g_objCommon.Check_Position(nGStage1YAxisNo, 3)) {
		if (g_objAJinAXL.Is_Done(nGStage1YAxisNo)) {
			g_objCommon.Save_Motion(nGStage1YAxisNo, 3);
			m_tGoodStage1Loop.Takt_Save(22, 7); m_tGoodStage1Loop.Takt_Start(); 
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 25:
		if (gNG->nTrayOX[2][0] == 0 || (gNG->nTrayOX[2][0] == 1 && m_pDX12->iGoodStage1TrayExist)) {
			Init_GoodTray(nGStage1No);
			gData.sGoodCarrierID = gData.sCarID_GoodTray[nGStage1No];
			m_nGoodStage1Case = 30; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 30:	// Unload Picker Wait
		m_tGoodStage1Loop.Set_LoopTime(5000);
		return TRUE;

	case 31:
		if (m_nGoodStage2Case < 30 || m_nGoodStage2Case >= 50) {
			m_tGoodStage1Loop.Takt_Save(22, 8); m_tGoodStage1Loop.Takt_Start(); 
			m_pDY12->oGoodStage1Up = FALSE; m_pDY12->oGoodStage1Down = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 32:
		if (!m_pDX12->iGoodStage1Up && m_pDX12->iGoodStage1Down) {
			m_tGoodStage1Loop.Takt_Save(22, 9); m_tGoodStage1Loop.Takt_Start(); 
			m_nGoodStage1Case = 40; m_tGoodStage1Loop.Set_LoopTime(30000);
		}
		break;

	case 40:	//Up Wait
		if (m_nGoodStage2Case < 40 || m_nGoodStage2Case >= 60) {
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 41:
		if (!m_pDX12->iGoodStage1Up && m_pDX12->iGoodStage1Down) {
			if (m_pDX12->iGoodStage2Up && !m_pDX12->iGoodStage2Down) {
				m_tGoodStage1Loop.Takt_Save(22, 10); m_tGoodStage1Loop.Takt_Start(); 
				g_objCommon.Move_Position(nGStage1YAxisNo, 0);	//load
				m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(nGStage1YAxisNo, 0)) {
			g_objCommon.Save_Motion(nGStage1YAxisNo, 0);
			m_tGoodStage1Loop.Takt_Save(22, 11); m_tGoodStage1Loop.Takt_Start(); 
			m_nGoodStage1Case = 50; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;

	case 50:
		if (m_nGoodStage2Case > 22 && m_nGoodStage2Case < 50) {
			m_tGoodStage1Loop.Takt_Save(22, 12); m_tGoodStage1Loop.Takt_Start(); 
			m_pDY12->oGoodStage1Up = TRUE; m_pDY12->oGoodStage1Down = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 51:
		if (m_pDX12->iGoodStage1Up && !m_pDX12->iGoodStage1Down) {
			m_tGoodStage1Loop.Takt_Save(22, 13); m_tGoodStage1Loop.Takt_Start(); 
			m_pDY12->oGoodStage1SlaveIn = FALSE; m_pDY12->oGoodStage1SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 52:
		if (!m_pDX12->iGoodStage1SlaveIn && m_pDX12->iGoodStage1SlaveOut) {
			m_tGoodStage1Loop.Takt_Save(22, 14); m_tGoodStage1Loop.Takt_Start(); 
			m_pDY12->oGoodStage1MasterIn = FALSE; m_pDY12->oGoodStage1MasterOut = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 53:
		if (!m_pDX12->iGoodStage1MasterIn && m_pDX12->iGoodStage1MasterOut) {
			m_tGoodStage1Loop.Takt_Save(22, 15); m_tGoodStage1Loop.Takt_Start(); 
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(5000);
		}
		break;
	case 54:
		if (gNG->nTrayOX[2][0] == 1) {
			m_nGoodStage1Case++; m_tGoodStage1Loop.Set_LoopTime(30000);
		} else {
			m_tGoodStage1Loop.Takt_Save(22, 16);
			m_nGoodStage1Case = 0; m_tGoodStage1Loop.Set_LoopTime(30000);
		}
		break;
	case 55:
		if (m_pDX12->iGoodStage1TrayExist) {
			if (gData.nPortNo_GoodTray[0] > 0) m_nGoodStage1Case = 60;
			else							   m_nGoodStage1Case = 0;
			m_tGoodStage1Loop.Set_LoopTime(30000);
		}
		break;

	case 60:	// Transfer Picker Wait
		m_tGoodStage1Loop.Set_LoopTime(5000);
		return TRUE;

	case 61:
		if (!m_pDX12->iGoodStage1TrayExist) {
			gNG->nTrayOX[2][0] = 0;
			m_tGoodStage1Loop.Takt_Save(22, 16);
			m_nGoodStage1Case = 0; m_tGoodStage1Loop.Set_LoopTime(30000);
		}
		break;

	}

	// 22. (Error : 8400)
	if (m_tGoodStage1Loop.Over_LoopTime()) {
		if (m_nGoodStage1Case == 8) {
			m_nGoodStage1Case = 7; m_nRFRetray[2][0]++;
			if (m_nRFRetray[2][0] < 3) return TRUE;
		}
		g_objCommon.Show_Error(8400 + m_nGoodStage1Case);
		return FALSE;
	}
	return TRUE;
}

// 23. (Error : 8600)
BOOL CSequenceMain::Run_GoodStage2()
{
	static int	  nGStage2No		= 1;	//1고정
	static int	  nGStage2YAxisNo = AX_GOOD_STAGE_Y2;

	switch (m_nGoodStage2Case) {
	case 0:	// Wait
		m_tGoodStage2Loop.Set_LoopTime(5000);
		return TRUE;

	case 1:
		if (m_pDX12->iGoodStage2TrayExist) {
			gNG->nTrayOX[2][1] = 1;
			m_tGoodStage2Loop.Takt_Start();
			m_pDY12->oGoodStage2MasterIn = TRUE; m_pDY12->oGoodStage2MasterOut = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 2:
		if (m_pDX12->iGoodStage2MasterIn && !m_pDX12->iGoodStage2MasterOut) {
			m_tGoodStage2Loop.Takt_Save(23, 1); m_tGoodStage2Loop.Takt_Start(); 
			m_pDY12->oGoodStage2SlaveIn = TRUE; m_pDY12->oGoodStage2SlaveOut = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 3:
		if (m_pDX12->iGoodStage2SlaveIn && !m_pDX12->iGoodStage2SlaveOut) {
			m_tGoodStage2Loop.Takt_Save(23, 2); m_tGoodStage2Loop.Takt_Start(); 
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 4:
		if (m_pDX12->iGoodStage2Up && !m_pDX12->iGoodStage2Down) {
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 5:	//Align Wait
		if (m_nGoodStage1Case > 22) {
			if (m_pEquipData->bUseRFIDGood) m_nGoodStage2Case++; 
			else							m_nGoodStage2Case = 11;
			m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 6:
		if (m_pDX12->iGoodStage2Up && !m_pDX12->iGoodStage2Down) {
			m_nRFRetray[2][1] = 0;
			g_objCommon.Move_Position(nGStage2YAxisNo, 6);	//RFID
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 7:
		if (g_objCommon.Check_Position(nGStage2YAxisNo, 6)) {
			if (m_pEquipData->bUseRFIDGood) m_nGoodStage2Case++; 
			else							m_nGoodStage2Case = 11;
			g_objCarrierRFID_Good.Send_RFIDRead();
			m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 8:
		if (g_objCarrierRFID_Good.Is_RecvComplete()) {
			gData.sCarID_GoodTray[nGStage2No] = g_objCarrierRFID_Good.Get_CarrierID();
			if (gData.sCarID_GoodTray[nGStage2No].GetLength() > 2) {
				gMes.nCarConfirm[1] = 1;
#ifndef AJIN_BOARD_USE
				gMes.nCarConfirm[1] = 2;
#endif
				g_objMesAgent.Set_CarrierIDReport("G", "", gData.sCarID_GoodTray[nGStage2No]);
				g_objLogFile.Save_RFBarData(5, gData.sCarID_GoodTray[nGStage2No]);
				m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 9:
		if (!m_pEquipData->bUseMES || gMes.nCarConfirm[1] == 2) {
			m_nGoodStage2Case = 11; m_tGoodStage2Loop.Set_LoopTime(30000);
		}
		break;

	case 11:
		if (m_pDX12->iGoodStage2Up && !m_pDX12->iGoodStage2Down) {
			m_tGoodStage2Loop.Takt_Save(23, 3); m_tGoodStage2Loop.Takt_Start(); 
			g_objCommon.Move_Position(nGStage2YAxisNo, 1);	//Aling1
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 12:
		if (g_objCommon.Check_Position(nGStage2YAxisNo, 1)) {
			g_objCommon.Save_Motion(nGStage2YAxisNo, 1);
			m_tGoodStage2Loop.Takt_Save(23, 4); m_tGoodStage2Loop.Takt_Start(); 
			m_nGoodStage2Case = 20; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	//Align Wait
		return TRUE;
	case 21:
		if (m_nGoodStage1Case > 20 && m_nGoodStage1Case < 40) {
			m_tGoodStage2Loop.Takt_Save(23, 5); m_tGoodStage2Loop.Takt_Start(); 
			g_objCommon.Move_Position(nGStage2YAxisNo, 2);	//Wait
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(30000);
		} else if (m_nGoodStage1Case <= 20 || m_nGoodStage1Case >= 40) {
			m_tGoodStage2Loop.Takt_Save(23, 5); m_tGoodStage2Loop.Takt_Start(); 
			g_objCommon.Move_Position(nGStage2YAxisNo, 3);	//Work
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 22:
		if (g_objCommon.Check_Position(nGStage2YAxisNo, 2) || g_objCommon.Check_Position(nGStage2YAxisNo, 3)) {
			if (g_objCommon.Check_Position(nGStage2YAxisNo, 2)) g_objCommon.Save_Motion(nGStage2YAxisNo, 2);
			m_tGoodStage2Loop.Takt_Save(23, 6); m_tGoodStage2Loop.Takt_Start(); 
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 23:
		if (m_nGoodStage1Case <= 20 || m_nGoodStage1Case >= 40) {
			if (gData.bJahwa) {
				g_objCommon.Move_Position(nGStage2YAxisNo, 3);	//Work
			} else {
				double d2PosY = m_pMoveData->dGoodStageY2[3] + (m_pEquipData->dTrayPitchY * 9) + CARRIER_OFFSETY;
				g_objAJinAXL.Move_Absolute(nGStage2YAxisNo, d2PosY);
			}
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 24:
//		if (g_objCommon.Check_Position(nGStage2YAxisNo, 3)) {
		if (g_objAJinAXL.Is_Done(nGStage2YAxisNo)) {
			g_objCommon.Save_Motion(nGStage2YAxisNo, 3);
			m_tGoodStage2Loop.Takt_Save(23, 7); m_tGoodStage2Loop.Takt_Start(); 
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 25:
		if (gNG->nTrayOX[2][1] == 0 || (gNG->nTrayOX[2][1] == 1 && m_pDX12->iGoodStage2TrayExist)) {
			Init_GoodTray(nGStage2No);
			gData.sGoodCarrierID = gData.sCarID_GoodTray[nGStage2No];
			m_nGoodStage2Case = 30; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 30:	// Unload Picker Wait
		m_tGoodStage2Loop.Set_LoopTime(5000);
		return TRUE;

	case 31:
		if (m_nGoodStage1Case < 30 || m_nGoodStage1Case >= 50) {
			m_tGoodStage2Loop.Takt_Save(23, 8); m_tGoodStage2Loop.Takt_Start(); 
			m_pDY12->oGoodStage2Up = FALSE; m_pDY12->oGoodStage2Down = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(10000);
		}
		return TRUE;
	case 32:
		if (!m_pDX12->iGoodStage2Up && m_pDX12->iGoodStage2Down) {
			m_tGoodStage2Loop.Takt_Save(23, 9); m_tGoodStage2Loop.Takt_Start(); 
			m_nGoodStage2Case = 40; m_tGoodStage2Loop.Set_LoopTime(30000);
		}
		break;

	case 40:	//Up Wait
		if (m_nGoodStage1Case < 40 || m_nGoodStage1Case >= 60) {
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 41:
		if (!m_pDX12->iGoodStage2Up && m_pDX12->iGoodStage2Down) {
			if (m_pDX12->iGoodStage1Up && !m_pDX12->iGoodStage1Down) {
				m_tGoodStage2Loop.Takt_Save(23, 10); m_tGoodStage2Loop.Takt_Start(); 
				g_objCommon.Move_Position(nGStage2YAxisNo, 0);	//load
				m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(30000);
			}
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(nGStage2YAxisNo, 0)) {
			g_objCommon.Save_Motion(nGStage2YAxisNo, 0);
			m_tGoodStage2Loop.Takt_Save(23, 11); m_tGoodStage2Loop.Takt_Start(); 
			m_nGoodStage2Case = 50; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;

	case 50:
		if (m_nGoodStage1Case > 22 && m_nGoodStage1Case < 50) {
			m_tGoodStage2Loop.Takt_Save(23, 12); m_tGoodStage2Loop.Takt_Start(); 
			m_pDY12->oGoodStage2Up = TRUE; m_pDY12->oGoodStage2Down = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(30000);
		}
		return TRUE;
	case 51:
		if (m_pDX12->iGoodStage2Up && !m_pDX12->iGoodStage2Down) {
			m_tGoodStage2Loop.Takt_Save(23, 13); m_tGoodStage2Loop.Takt_Start(); 
			m_pDY12->oGoodStage2SlaveIn = FALSE; m_pDY12->oGoodStage2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 52:
		if (!m_pDX12->iGoodStage2SlaveIn && m_pDX12->iGoodStage2SlaveOut) {
			m_tGoodStage2Loop.Takt_Save(23, 14); m_tGoodStage2Loop.Takt_Start(); 
			m_pDY12->oGoodStage2MasterIn = FALSE; m_pDY12->oGoodStage2MasterOut = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 53:
		if (!m_pDX12->iGoodStage2MasterIn && m_pDX12->iGoodStage2MasterOut) {
			m_tGoodStage2Loop.Takt_Save(23, 15); m_tGoodStage2Loop.Takt_Start(); 
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(5000);
		}
		break;
	case 54:
		if (gNG->nTrayOX[2][1] == 1) {
			m_nGoodStage2Case++; m_tGoodStage2Loop.Set_LoopTime(30000);
		} else {
			m_tGoodStage2Loop.Takt_Save(23, 16);
			m_nGoodStage2Case = 0; m_tGoodStage2Loop.Set_LoopTime(30000);
		}
		break;
	case 55:
		if (m_pDX12->iGoodStage2TrayExist) {
			if (gData.nPortNo_GoodTray[1] > 0) m_nGoodStage2Case = 60;
			else							   m_nGoodStage2Case = 0;
			m_tGoodStage2Loop.Set_LoopTime(30000);
		}
		break;

	case 60:	// Transfer Picker Wait
		m_tGoodStage2Loop.Set_LoopTime(5000);
		return TRUE;

	case 61:
		if (!m_pDX12->iGoodStage2TrayExist) {
			gNG->nTrayOX[2][1] = 0;
			m_tGoodStage2Loop.Takt_Save(23, 16);
			m_nGoodStage2Case = 0; m_tGoodStage2Loop.Set_LoopTime(30000);
		}
		break;

	}

	// 23. (Error : 8600)
	if (m_tGoodStage2Loop.Over_LoopTime()) {
		if (m_nGoodStage2Case == 8) {
			m_nGoodStage2Case = 7; m_nRFRetray[2][1]++;
			if (m_nRFRetray[2][1] < 3) return TRUE;
		}
		g_objCommon.Show_Error(8600 + m_nGoodStage2Case);
		return FALSE;
	}
	return TRUE;
}

// 24. (Error : 8800)
BOOL CSequenceMain::Run_ShipAlign()
{
	static int nSJobNo;	//1,2,3,4
	static double dSAlignX, dSAlignY;
	if (m_nShipAlignCase > 1 && !m_pEquipData->bUseAlign2) m_nShipAlignCase = 8;

	switch (m_nShipAlignCase) {
	case 0:	// Wait
		if (Check_ShipAlign(nSJobNo)) {
			m_nShipAlignCase++; m_tShipAlignLoop.Set_LoopTime(5000);
		}
		return TRUE;

	case 1:
		if (Check_ShipAlign(nSJobNo)) {
			if (!m_tShipAlignLoop.Waiting_Time(500)) break;
			Init_ShiipAlignData(nSJobNo);

			BOOL bTrayExist = FALSE;
			if (nSJobNo == 1 && m_pDX11->iNGStage1TrayExist)   bTrayExist = TRUE;
			if (nSJobNo == 2 && m_pDX11->iNGStage2TrayExist)   bTrayExist = TRUE;
			if (nSJobNo == 3 && m_pDX12->iGoodStage1TrayExist) bTrayExist = TRUE;
			if (nSJobNo == 4 && m_pDX12->iGoodStage2TrayExist) bTrayExist = TRUE;

			if (bTrayExist == FALSE || !m_pEquipData->bUseAlign2) {
				if (nSJobNo == 1) m_nNGStage1Case   = 21;
				if (nSJobNo == 2) m_nNGStage2Case   = 21;	
				if (nSJobNo == 3) m_nGoodStage1Case = 21;
				if (nSJobNo == 4) m_nGoodStage2Case = 21;
				m_nShipAlignCase = 0; m_tShipAlignLoop.Set_LoopTime(5000);
			} else {
				m_nShipAlignCase++; m_tShipAlignLoop.Set_LoopTime(5000);
			}
		} else {
			m_nShipAlignCase = 0; m_tShipAlignLoop.Set_LoopTime(5000);
		}
		break;
	case 2:
		if (nSJobNo > 0 && nSJobNo < 5) {
			m_tShipAlignLoop.Takt_Start();
			g_objCommon.Move_Position(AX_SHIP_ALIGN_X, (nSJobNo-1));

			if (nSJobNo == 1) g_objCommon.Move_Position(AX_NG_STAGE_Y1, 1);
			if (nSJobNo == 2) g_objCommon.Move_Position(AX_NG_STAGE_Y2, 1);
			if (nSJobNo == 3) g_objCommon.Move_Position(AX_GOOD_STAGE_Y1, 1);
			if (nSJobNo == 4) g_objCommon.Move_Position(AX_GOOD_STAGE_Y2, 1);

			m_nShipAlignCase++; m_tShipAlignLoop.Set_LoopTime(10000);
		}
		break;
	case 3:
		if (g_objCommon.Check_Position(AX_SHIP_ALIGN_X, (nSJobNo-1))) {
			if ((nSJobNo == 1 && g_objCommon.Check_Position(AX_NG_STAGE_Y1,   1)) ||
				(nSJobNo == 2 && g_objCommon.Check_Position(AX_NG_STAGE_Y2,   1)) ||
				(nSJobNo == 3 && g_objCommon.Check_Position(AX_GOOD_STAGE_Y1, 1)) ||
				(nSJobNo == 4 && g_objCommon.Check_Position(AX_GOOD_STAGE_Y2, 1)) ) {

				g_objCommon.Save_Motion(AX_SHIP_ALIGN_X, (nSJobNo-1));
				gData.bAlignShipDone = FALSE;
				if (nSJobNo < 3) g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A2", "NG_STAGE",   nSJobNo, nSJobNo, 1);
				else			 g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A2", "GOOD_STAGE", nSJobNo, nSJobNo, 1);
				m_tShipAlignLoop.Takt_Save(24, 1); m_tShipAlignLoop.Takt_Start();
				m_nShipAlignCase++; m_tShipAlignLoop.Set_LoopTime(30000);
			}
		}
		break;
	case 4:
		if (gData.bAlignShipDone) {
			if (!m_tShipAlignLoop.Waiting_Time(m_pEquipData->nDelayTime[6])) break;
			dSAlignX = m_pMoveData->dShiipAling[nSJobNo-1] - (m_pEquipData->dTrayPitchX * 3) - CARRIER_OFFSETX;
			g_objAJinAXL.Move_Absolute(AX_SHIP_ALIGN_X, dSAlignX);
			m_tShipAlignLoop.Takt_Save(24, 2); m_tShipAlignLoop.Takt_Start();
			m_nShipAlignCase++; m_tShipAlignLoop.Set_LoopTime(10000);
		}
		break;
	case 5:
		if (g_objAJinAXL.Is_MoveDone(AX_SHIP_ALIGN_X, dSAlignX)) {
			g_objCommon.Save_Motion(AX_SHIP_ALIGN_X, -1, dSAlignX);
			gData.bAlignShipDone = FALSE;
			if (nSJobNo < 3) g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A2", "NG_STAGE",   nSJobNo, nSJobNo, 2);
			else			 g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A2", "GOOD_STAGE", nSJobNo, nSJobNo, 2);
			m_tShipAlignLoop.Takt_Save(24, 3); m_tShipAlignLoop.Takt_Start();
			m_nShipAlignCase++; m_tShipAlignLoop.Set_LoopTime(30000);
		}
		break;
	case 6:
		if (gData.bAlignShipDone) {
			if (!m_tShipAlignLoop.Waiting_Time(m_pEquipData->nDelayTime[6])) break;
			if (nSJobNo == 1) dSAlignY = m_pMoveData->dNGStageY1[1]   + (m_pEquipData->dTrayPitchY * 9) + CARRIER_OFFSETY;
			if (nSJobNo == 2) dSAlignY = m_pMoveData->dNGStageY2[1]   + (m_pEquipData->dTrayPitchY * 9) + CARRIER_OFFSETY;
			if (nSJobNo == 3) dSAlignY = m_pMoveData->dGoodStageY1[1] + (m_pEquipData->dTrayPitchY * 9) + CARRIER_OFFSETY;
			if (nSJobNo == 4) dSAlignY = m_pMoveData->dGoodStageY2[1] + (m_pEquipData->dTrayPitchY * 9) + CARRIER_OFFSETY;

			if (nSJobNo == 1) g_objAJinAXL.Move_Absolute(AX_NG_STAGE_Y1,   dSAlignY);
			if (nSJobNo == 2) g_objAJinAXL.Move_Absolute(AX_NG_STAGE_Y2,   dSAlignY);
			if (nSJobNo == 3) g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE_Y1, dSAlignY);
			if (nSJobNo == 4) g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE_Y2, dSAlignY);
			
			m_tShipAlignLoop.Takt_Save(24, 4); m_tShipAlignLoop.Takt_Start();
			m_nShipAlignCase++; m_tShipAlignLoop.Set_LoopTime(10000);
		}
		break;
	case 7:
		if ((nSJobNo == 1 && g_objAJinAXL.Is_MoveDone(AX_NG_STAGE_Y1,   dSAlignY)) ||
			(nSJobNo == 2 && g_objAJinAXL.Is_MoveDone(AX_NG_STAGE_Y2,   dSAlignY)) ||
			(nSJobNo == 3 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE_Y1, dSAlignY)) ||
			(nSJobNo == 4 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE_Y2, dSAlignY)) ) {
			if (nSJobNo == 1) g_objCommon.Save_Motion(AX_NG_STAGE_Y1,   -1, dSAlignY);
			if (nSJobNo == 2) g_objCommon.Save_Motion(AX_NG_STAGE_Y2,   -1, dSAlignY);
			if (nSJobNo == 3) g_objCommon.Save_Motion(AX_GOOD_STAGE_Y1, -1, dSAlignY);
			if (nSJobNo == 4) g_objCommon.Save_Motion(AX_GOOD_STAGE_Y2, -1, dSAlignY);
			gData.bAlignShipDone = FALSE;
			if (nSJobNo < 3) g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A2", "NG_STAGE",   nSJobNo, nSJobNo, 3);
			else			 g_objInspector.Set_AlignRequest(INSPECTOR_PC1, "A2", "GOOD_STAGE", nSJobNo, nSJobNo, 3);
			m_tShipAlignLoop.Takt_Save(24, 5); m_tShipAlignLoop.Takt_Start();
			m_nShipAlignCase++; m_tShipAlignLoop.Set_LoopTime(30000);
		}
		break;
	case 8:
		if (gData.bAlignShipDone || !m_pEquipData->bUseAlign2) {
			if (!m_tShipAlignLoop.Waiting_Time(m_pEquipData->nDelayTime[6])) break;
			if (nSJobNo == 1) m_nNGStage1Case   = 21;
			if (nSJobNo == 2) m_nNGStage2Case   = 21;	
			if (nSJobNo == 3) m_nGoodStage1Case = 21;
			if (nSJobNo == 4) m_nGoodStage2Case = 21;

//			g_objCommon.Move_Position(AX_SHIP_ALIGN_X, 4);	//Ready
			m_tShipAlignLoop.Takt_Save(24, 6);
			m_nShipAlignCase++; m_tShipAlignLoop.Set_LoopTime(10000);
		}
		break;
	case 9:
//		if (g_objCommon.Check_Position(AX_SHIP_ALIGN_X, 4)) {
			m_nShipAlignCase = 0; m_tShipAlignLoop.Set_LoopTime(10000);
//		}
		break;

	}

	// 24. (Error : 8800)
	if (m_tShipAlignLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(8800 + m_nShipAlignCase);
		return FALSE;
	}
	return TRUE;
}

// 25. (Error : 2200)
BOOL CSequenceMain::Run_MZTransfer()
{
	static int nPosX, nFrom, nTo; 

	switch (m_nMZTransferCase) {
	case 0:	// Wait
		if (Check_MZTransfer(nFrom, nTo)) // 어디에서 어디로 가는지 우선순위 체크     From, To 
		{
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d, From To Checked From:%d  To:%d", m_nMZTransferCase, nFrom, nTo);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;

	case 1:
		if (nFrom > 0) 
		{
			nPosX = nFrom;
			if (nFrom == 3) 
			{	//Buffer-MZ
				m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(5000);
			} 
			else 
			{
				m_nMZTransferCase = 11; m_tMZTransferLoop.Set_LoopTime(5000);
			}

			m_sLog.Format("MCC,25,MZTransfer,%d, nFrom:%d ", m_nMZTransferCase, nFrom);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;

	case 2:
		if (m_pDX18->iMZBufferExist && gData.sMZID[5].GetLength() > 4)
		{
			gMes.nMZConfirm[1] = 1;
			g_objMesAgent.Set_MGZIDReport("G", gData.sMZID[5]);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(30000);

			m_sLog.Format("MCC,25,MZTransfer,%d, iMZBufferExist and MZ ID report ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 3:	//MES 완료
		if (!m_pEquipData->bUseMES || gMes.nMZConfirm[1] == 2) 
		{
			m_nMZTransferCase = 11; m_tMZTransferLoop.Set_LoopTime(5000);

			m_sLog.Format("MCC,25,MZTransfer,%d, mes done", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 5:
		if ((nTo == 2 && m_nLDMZElevatorCase == 0) ||
			(nTo == 3 && !m_pDX18->iMZBufferExist) ||
			(nTo == 4 && m_nNGMZElevatorCase == 0) ||
			(nTo == 5 && m_nGDMZElevatorCase == 0) ||
			(nTo == 6 && m_nULCVElevatorCase == 0) ) 
		{
			nPosX = nTo;
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(5000);


			m_sLog.Format("MCC,25,MZTransfer,%d, nTo:%d ", m_nMZTransferCase, nTo);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		return TRUE;
	case 6:
		if ((nTo == 2 && !m_pDX18->iLDMZElevatorExist) ||
			(nTo == 3 && !m_pDX18->iMZBufferExist)     ||
			(nTo == 4 && !m_pDX18->iNGMZElevatorExist) ||
			(nTo == 5 && !m_pDX18->iGDMZElevatorExist) ||
			(nTo == 6 && !m_pDX17->iULCVElevatorMZExist) ) 
		{
			m_nMZTransferCase = 31; m_tMZTransferLoop.Set_LoopTime(30000);

			m_sLog.Format("MCC,25,MZTransfer,%d, empty", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 11:	//MZ-Ready Up Check 
		if (g_objCommon.Check_Position(AX_MZ_TRANSFER_Z, 0)) 
		{
			m_pDY20->oMZTransGrip12Open = TRUE; m_pDY20->oMZTransGrip12Close = FALSE;
			m_pDY20->oMZTransGrip34Open = TRUE; m_pDY20->oMZTransGrip34Close = FALSE;
			m_pDY20->oMZTransLockBarUp  = TRUE; m_pDY20->oMZTransLockBarDn = FALSE;
			m_pDY20->oMZTransLockBarOpen = TRUE; m_pDY20->oMZTransLockBarClose = FALSE;
			g_objAJinAXL.Write_Output(20);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);
			
			m_sLog.Format("MCC,25,MZTransfer,%d, Grip1234 Open Bar Up and Open", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 12:
		if (g_objCommon.Check_Position(AX_MZ_TRANSFER_Z, 0))
		{
			g_objCommon.Move_Position(AX_MZ_TRANSFER_X, nPosX);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(20000);

			m_sLog.Format("MCC,25,MZTransfer,%d, MZ_TRANSFER_X move to Pos:%d", m_nMZTransferCase, nPosX);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 13:
		if (g_objCommon.Check_Position(AX_MZ_TRANSFER_X, nPosX)) 
		{
			g_objCommon.Move_Override(AX_MZ_TRANSFER_Z, nPosX, 10.0);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d, AX_MZ_TRANSFER_Z move to Pos:%d", m_nMZTransferCase, nPosX);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 14:
		if (g_objCommon.Check_Position(AX_MZ_TRANSFER_Z, nPosX)) 
		{
			m_pDY20->oMZTransGrip12Open = FALSE; m_pDY20->oMZTransGrip12Close = TRUE;
			g_objAJinAXL.Write_Output(20);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  Grip12 Close ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 15:
		if (!m_pDX20->iMZTransGrip12Open && m_pDX20->iMZTransGrip12Close)
		{
			m_pDY20->oMZTransGrip34Open = FALSE; m_pDY20->oMZTransGrip34Close = TRUE;
			g_objAJinAXL.Write_Output(20);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  Grip34 Close ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 16:
		if (!m_pDX20->iMZTransGrip34Open && m_pDX20->iMZTransGrip34Close)
		{
			if (nPosX == 4 || nPosX == 5) {	//NG-MZ, Good-MZ
				m_pDY20->oMZTransLockBarOpen = TRUE; m_pDY20->oMZTransLockBarClose = FALSE;
				g_objAJinAXL.Write_Output(20);
				m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

				m_sLog.Format("MCC,25,MZTransfer,%d,  Lock Bar Open ", m_nMZTransferCase);
				g_objLogFile.Save_SeqLog(m_sLog);
			} 
			else
			{
				m_nMZTransferCase = 20; m_tMZTransferLoop.Set_LoopTime(10000);

				m_sLog.Format("MCC,25,MZTransfer,%d, jump to 20 ", m_nMZTransferCase);
				g_objLogFile.Save_SeqLog(m_sLog);
			}

			
		}
		break;
	case 17:
		if (m_pDX20->iMZTransLockBarOpen && !m_pDX20->iMZTransLockBarClose)
		{
			m_pDY20->oMZTransLockBarUp = FALSE; m_pDY20->oMZTransLockBarDn = TRUE;
			g_objAJinAXL.Write_Output(20);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  Lock Bar Down ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 18:
		if (!m_pDX20->iMZTransLockBarUp && m_pDX20->iMZTransLockBarDn) 
		{
			if (!m_tMZTransferLoop.Waiting_Time(500)) break;
			m_pDY20->oMZTransLockBarOpen = FALSE; m_pDY20->oMZTransLockBarClose = TRUE;
			g_objAJinAXL.Write_Output(20);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  Lock Bar Close ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 19:
		if (!m_pDX20->iMZTransLockBarOpen && m_pDX20->iMZTransLockBarClose) 
		{
			if (!m_tMZTransferLoop.Waiting_Time(500)) break;
			m_pDY20->oMZTransLockBarUp  = TRUE; m_pDY20->oMZTransLockBarDn = FALSE;
			g_objAJinAXL.Write_Output(20);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  Lock Bar Up ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 20:
		if (m_pDX20->iMZTransLockBarUp && !m_pDX20->iMZTransLockBarDn) 
		{
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(3000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  Lock Bar Up done ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 21:
		if (!m_pEquipData->bUseLockOpenChk || m_pDX20->iMZLoackBarCheck || nFrom < 4)
		{	//CloseCheck
			if (!m_pDX20->iMZTransGrip12Open && m_pDX20->iMZTransGrip12Close)
			{
				if (!m_pDX20->iMZTransGrip34Open && m_pDX20->iMZTransGrip34Close)
				{
					g_objCommon.Move_Position(AX_MZ_TRANSFER_Z, 0);
					m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

					m_sLog.Format("MCC,25,MZTransfer,%d, AX_MZ_TRANSFER_Z move to Ready Up", m_nMZTransferCase);
					g_objLogFile.Save_SeqLog(m_sLog);
				}
			}
		}
		break;
	case 22:
		if (g_objCommon.Check_Position(AX_MZ_TRANSFER_Z, 0))
		{
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d, AX_MZ_TRANSFER_Z move to Ready Up done", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 23:
		if (m_pDX20->iMZTransExist) 
		{
			if (nFrom == 1 && m_nLDCVElevatorCase == 20) m_nLDCVElevatorCase = 21;
			if (nFrom == 2 && m_nLDMZElevatorCase == 50) m_nLDMZElevatorCase = 51;
			if (nFrom == 4 && m_nNGMZElevatorCase == 50) m_nNGMZElevatorCase = 51;
			if (nFrom == 5 && m_nGDMZElevatorCase == 50) m_nGDMZElevatorCase = 51;

			m_nMZTransferCase = 5; m_tMZTransferLoop.Set_LoopTime(10000);
			if (nFrom == 1) 
			{	//Load-Elevator
				gData.sMZID[3] = gData.sMZID[2]; gData.sMZID[2] = "";
				
				if (gData.nCVElevatorFrom == 1) {
					m_sLog.Format("[MZTransfer: Up] Load-Elevator(G) => MZID(%s)", gData.sMZID[3]);
				} else {
					m_sLog.Format("[MZTransfer: Up] Load-Elevator(N) => MZID(%s)", gData.sMZID[3]);
				}
			}
			if (nFrom == 2 || nFrom == 3) 
			{	//Load-MZ,Buffer-MZ
				if (nFrom == 2) {
					gData.sMZID[3] = gData.sMZID[4]; gData.sMZID[4] = "";
					m_sLog.Format("[MZTransfer: Up] Load-MZ => MZID(%s)", gData.sMZID[3]);
					m_nMZTransferCase = 24;
				}
				if (nFrom == 3) {
					gData.sMZID[3] = gData.sMZID[5]; gData.sMZID[5] = "";
					m_sLog.Format("[MZTransfer: Up] Buffer-MZ => MZID(%s)", gData.sMZID[3]);
					m_nMZTransferCase = 25;
				}
			}
			if (nFrom == 4) {	//NG-MZ
				for(int i=0; i<8; i++) gLot.nCarrierExist[1][i] = 0;
				gData.sMZID[3] = gData.sMZID[6]; gData.sMZID[6] = "";
				m_sLog.Format("[MZTransfer: Up] NG-MZ => MZID(%s)", gData.sMZID[3]);
				m_nMZTransferCase = 26;
			}
			if (nFrom == 5) {	//Good-MZ
				gData.sMZID[3] = gData.sMZID[7]; gData.sMZID[7] = "";
				m_sLog.Format("[MZTransfer: Up] Good-MZ => MZID(%s)", gData.sMZID[3]);
				m_nMZTransferCase = 27;
			}
			g_objLogFile.Save_HandlerLog(m_sLog); g_objLogFile.Save_SeqLog(m_sLog);

			m_sLog.Format("MCC,25,MZTransfer,%d, Info Check ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 24:
			m_nMZTransferCase = 5; m_tMZTransferLoop.Set_LoopTime(10000);
		break;
	case 25:
			m_nMZTransferCase = 5; m_tMZTransferLoop.Set_LoopTime(10000);
		break;
	case 26:
			m_nMZTransferCase = 5; m_tMZTransferLoop.Set_LoopTime(10000);
		break;
	case 27:
			m_nMZTransferCase = 5; m_tMZTransferLoop.Set_LoopTime(10000);
		break;

	case 31:	//MZ-Down
		if (g_objCommon.Check_Position(AX_MZ_TRANSFER_Z, 0)) {
			if (nTo != 6 || (nTo == 6 && g_objCommon.Check_Position(AX_ULCV_ELEVATOR_Z, 2))) {
				if (nTo == 2) {	//Load-MZ
					m_pDY18->oLDMZElevatorClamp12Out = TRUE; m_pDY18->oLDMZElevatorClamp12In = FALSE;
					m_pDY18->oLDMZElevatorClamp34Out = TRUE; m_pDY18->oLDMZElevatorClamp34In = FALSE;
					g_objAJinAXL.Write_Output(18);
				}
				if (nTo == 4) {	//NG-MZ
					m_pDY18->oNGMZElevatorClamp12Out = TRUE; m_pDY18->oNGMZElevatorClamp12In = FALSE;
					m_pDY18->oNGMZElevatorClamp34Out = TRUE; m_pDY18->oNGMZElevatorClamp34In = FALSE;
					g_objAJinAXL.Write_Output(18);
				}
				if (nTo == 5) {	//Good-MZ
					m_pDY18->oGDMZElevatorClamp12Out = TRUE; m_pDY18->oGDMZElevatorClamp12In = FALSE;
					m_pDY18->oGDMZElevatorClamp34Out = TRUE; m_pDY18->oGDMZElevatorClamp34In = FALSE;
					g_objAJinAXL.Write_Output(18);
				}

				m_pDY20->oMZTransLockBarUp  = TRUE; m_pDY20->oMZTransLockBarDn = FALSE;
				m_pDY20->oMZTransLockBarOpen = TRUE; m_pDY20->oMZTransLockBarClose = FALSE;
				g_objAJinAXL.Write_Output(20);
				m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

				m_sLog.Format("MCC,25,MZTransfer,%d,  Clamp1234 Out ", m_nMZTransferCase);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 32:
		if (g_objCommon.Check_Position(AX_MZ_TRANSFER_Z, 0))
		{
			g_objCommon.Move_Position(AX_MZ_TRANSFER_X, nPosX);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(20000);

			m_sLog.Format("MCC,25,MZTransfer,%d, AX_MZ_TRANSFER_X move to nPosX:%d ", m_nMZTransferCase, nPosX);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 33:
		if (g_objCommon.Check_Position(AX_MZ_TRANSFER_X, nPosX)) 
		{
			if ((nTo == 2 && m_pDX18->iLDMZElevatorClamp12Out && m_pDX18->iLDMZElevatorClamp34Out) ||
				(nTo == 4 && m_pDX18->iNGMZElevatorClamp12Out && m_pDX18->iNGMZElevatorClamp34Out) ||
				(nTo == 5 && m_pDX18->iGDMZElevatorClamp12Out && m_pDX18->iGDMZElevatorClamp34Out) ||
				(nTo == 1) || (nTo == 3) || (nTo > 5) ) 
			{
				g_objCommon.Move_Override(AX_MZ_TRANSFER_Z, nPosX, 10.0);
				m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

				m_sLog.Format("MCC,25,MZTransfer,%d, AX_MZ_TRANSFER_Z move nPosx:%d", m_nMZTransferCase, nPosX);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		}
		break;
	case 34:
		if (g_objCommon.Check_Position(AX_MZ_TRANSFER_Z, nPosX)) 
		{
			if (nTo == 2 || nTo == 4) //Load-MZ,NG-MZ 
			{	
				m_pDY20->oMZTransLockBarOpen = FALSE; m_pDY20->oMZTransLockBarClose = TRUE;
				g_objAJinAXL.Write_Output(20);
				m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

				m_sLog.Format("MCC,25,MZTransfer,%d,  Lock Bar Close nTo:%d ", m_nMZTransferCase, nTo);
				g_objLogFile.Save_SeqLog(m_sLog);

			} 
			else
			{
				m_nMZTransferCase = 38; m_tMZTransferLoop.Set_LoopTime(10000);

				m_sLog.Format("MCC,25,MZTransfer,%d,  empty ", m_nMZTransferCase);
				g_objLogFile.Save_SeqLog(m_sLog);
			}

		
		}
		break;
	case 35:
		if (!m_pDX20->iMZTransLockBarOpen && m_pDX20->iMZTransLockBarClose) 
		{
			if (!m_tMZTransferLoop.Waiting_Time(500)) break;
			m_pDY20->oMZTransLockBarUp  = FALSE; m_pDY20->oMZTransLockBarDn = TRUE;
			g_objAJinAXL.Write_Output(20);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  Lock Bar Down ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 36:
		if (!m_pDX20->iMZTransLockBarUp && m_pDX20->iMZTransLockBarDn) 
		{
			if (!m_tMZTransferLoop.Waiting_Time(500)) break;
			m_pDY20->oMZTransLockBarOpen = TRUE; m_pDY20->oMZTransLockBarClose = FALSE;
			g_objAJinAXL.Write_Output(20);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);


			m_sLog.Format("MCC,25,MZTransfer,%d,  Lock Bar Open ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 37:
		if (m_pDX20->iMZTransLockBarOpen && !m_pDX20->iMZTransLockBarClose) 
		{
			if (!m_tMZTransferLoop.Waiting_Time(500)) break;
			m_pDY20->oMZTransLockBarUp  = TRUE; m_pDY20->oMZTransLockBarDn = FALSE;
			g_objAJinAXL.Write_Output(20);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  Lock Bar Up ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 38:
		if (m_pDX20->iMZTransLockBarUp && !m_pDX20->iMZTransLockBarDn)
		{
			m_pDY20->oMZTransGrip34Open = TRUE; m_pDY20->oMZTransGrip34Close = FALSE;
			g_objAJinAXL.Write_Output(20);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);


			m_sLog.Format("MCC,25,MZTransfer,%d,  Grip34 Open ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 39:
		if (m_pDX20->iMZTransGrip34Open && !m_pDX20->iMZTransGrip34Close) 
		{
			m_pDY20->oMZTransGrip12Open = TRUE; m_pDY20->oMZTransGrip12Close = FALSE;
			g_objAJinAXL.Write_Output(20);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  Grip12 Open ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 40:
		if (m_pDX20->iMZTransGrip12Open && !m_pDX20->iMZTransGrip12Close)
		{
			if (!m_tMZTransferLoop.Waiting_Time(500)) break;
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(3000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  Grip12 Open Checked", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 41:
		if (!m_pEquipData->bUseLockOpenChk || m_pDX20->iMZLoackBarCheck || nTo == 6) {	//OpenCheck, 6:Unload-Elevator
			g_objCommon.Move_Position(AX_MZ_TRANSFER_Z, 0);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);


			m_sLog.Format("MCC,25,MZTransfer,%d, AX_MZ_TRANSFER_Z move to Ready %d", m_nMZTransferCase, 0);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 42:
		if (g_objCommon.Check_Position(AX_MZ_TRANSFER_Z, 0)) 
		{
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);
			m_sLog.Format("MCC,25,MZTransfer,%d,  Ready done ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 43:
			if (nTo == 2 && m_nLDMZElevatorCase == 0) { m_nLDMZElevatorCase = 1; m_tLDMZElevatorLoop.Set_LoopTime(5000); }
			if (nTo == 4 && m_nNGMZElevatorCase == 0) { m_nNGMZElevatorCase = 1; m_tNGMZElevatorLoop.Set_LoopTime(5000); }
			if (nTo == 5 && m_nGDMZElevatorCase == 0) { m_nGDMZElevatorCase = 1; m_tGDMZElevatorLoop.Set_LoopTime(5000); }
			if (nTo == 6 && m_nULCVElevatorCase == 0) {
				if (nFrom == 4 && m_nULCVElevatorCase == 0) { m_nULCVElevatorCase = 11; m_tULCVElevatorLoop.Set_LoopTime(5000); }
				if (nFrom == 5 && m_nULCVElevatorCase == 0) { m_nULCVElevatorCase = 21; m_tULCVElevatorLoop.Set_LoopTime(5000); }
			}

			m_nMZTransferCase = 50; m_tMZTransferLoop.Set_LoopTime(5000);
			if (nTo == 2) {	//Load-MZ
				gData.nMZSlotNo[0] = gData.nMZSlotNo[3] = 0;
				gData.sMZID[4] = gData.sMZID[3]; gData.sMZID[3] = "";
				m_sLog.Format("[MZTransfer: Down] Load-MZ => MZID(%s)", gData.sMZID[4]);
				m_nMZTransferCase = 44;
			}
			if (nTo == 3) {	//Buffer
				gData.sMZID[5] = gData.sMZID[3]; gData.sMZID[3] = "";
				m_sLog.Format("[MZTransfer: Down] Buffer-MZ => MZID(%s)", gData.sMZID[5]);
				m_nMZTransferCase = 45;
			}
			if (nTo == 6) {	//Unload-Elevator
				gData.sMZID[8] = gData.sMZID[3]; gData.sMZID[3] = "";
				m_sLog.Format("[MZTransfer: Down] Unload-Elevator => MZID(%s,%s)", gData.sMZID[8], gData.sMZID[9]);
			}
			if (nTo == 4) {	//NG-MZ
				gData.nMZSlotNo[1] = 0;
				gData.sMZID[6] = gData.sMZID[3]; gData.sMZID[3] = "";
				m_sLog.Format("[MZTransfer: Down] NG-MZ => MZID(%s)", gData.sMZID[6]);
				m_nMZTransferCase = 46;
			}
			if (nTo == 5) {	//Good-MZ
				gData.nMZSlotNo[2] = 0;
				gData.sMZID[7] = gData.sMZID[3]; gData.sMZID[3] = "";
				m_sLog.Format("[MZTransfer: Down] Good-MZ => MZID(%s)", gData.sMZID[7]);
				m_nMZTransferCase = 47;
			}
			g_objLogFile.Save_HandlerLog(m_sLog); g_objLogFile.Save_SeqLog(m_sLog);

			m_sLog.Format("MCC,25,MZTransfer,%d, Info Update ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		break;
	case 44:	//Load-MZ
		if (m_pDX18->iLDMZElevatorExist) {
			m_nMZTransferCase = 50; m_tMZTransferLoop.Set_LoopTime(5000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  iLDMZElevatorExist ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 45:	//Buffer-MZ
		if (m_pDX18->iMZBufferExist) {
			m_nMZTransferCase = 50; m_tMZTransferLoop.Set_LoopTime(5000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  iMZBufferExist ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 46:	//NG-MZ
		if (m_pDX18->iNGMZElevatorExist) {
			m_nMZTransferCase = 50; m_tMZTransferLoop.Set_LoopTime(5000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  iNGMZElevatorExist ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 47:	//Good-MZ
		if (m_pDX18->iGDMZElevatorExist) {
			m_nMZTransferCase = 50; m_tMZTransferLoop.Set_LoopTime(5000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  iGDMZElevatorExist ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 50:
		if (m_pDX18->iLDMZElevatorExist || m_pDX18->iNGMZElevatorExist || m_pDX18->iGDMZElevatorExist) 
		{
			m_nMZTransferCase = 0; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  Mz Exist Checked ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		} 
		else
		{
			g_objCommon.Move_Position(AX_MZ_TRANSFER_X, 1);
			m_nMZTransferCase++; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  AX_MZ_TRANSFER_X move to 1 ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 51:
		if (g_objCommon.Check_Position(AX_MZ_TRANSFER_X, 1)) 
		{
			m_nMZTransferCase = 0; m_tMZTransferLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,25,MZTransfer,%d,  AX_MZ_TRANSFER_X move done ", m_nMZTransferCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	}

	// 25. (Error : 2200)
	if (m_tMZTransferLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(2200 + m_nMZTransferCase);
		return FALSE;
	}
	return TRUE;
}

// 26. (Error : 2300)
BOOL CSequenceMain::Run_LDCVElevator()
{
	switch (m_nLDCVElevatorCase) {
	case 0:	// Wait
		return TRUE;

	case 10:	// Wait
		return TRUE;

	case 11:
		if (m_pDX17->iLDCVElevatorStopperUp && !m_pDX17->iLDCVElevatorStopperDn) 
		{
			g_objLogFile.Save_SeqLog("MCC,26,LoadConvayorElevator,11, Elevator Stopper Up Check and Align Up&Out");

			m_pDY17->oLDCVElevatorAlignUp = TRUE;   m_pDY17->oLDCVElevatorAlignDn = FALSE;
			m_pDY17->oLDCVElevatorAlignIn = FALSE;   m_pDY17->oLDCVElevatorAlignOut = TRUE;
			g_objAJinAXL.Write_Output(17);
			m_nLDCVElevatorCase++; m_tLDCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 12:
		if (m_pDX17->iLDCVElevatorAlignUp && !m_pDX17->iLDCVElevatorAlignDn) 
		{
			g_objLogFile.Save_SeqLog("MCC,26,LoadConvayorElevator,12,Align In and Align Up Check");

			m_pDY17->oLDCVElevatorAlignIn = TRUE;   m_pDY17->oLDCVElevatorAlignOut = FALSE;
			g_objAJinAXL.Write_Output(17);
			m_nLDCVElevatorCase++; m_tLDCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 13:
		if (m_pDX17->iLDCVElevatorAlignIn && !m_pDX17->iLDCVElevatorAlignOut) 
		{
			if (!m_tLDCVElevatorLoop.Waiting_Time(500)) break;

			g_objLogFile.Save_SeqLog("MCC,26,LoadConvayorElevator,13,ElevatorAlignIn Check and Align Out");

			m_pDY17->oLDCVElevatorAlignIn = FALSE;   m_pDY17->oLDCVElevatorAlignOut = TRUE;
			g_objAJinAXL.Write_Output(17);
			m_nLDCVElevatorCase++; m_tLDCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 14:
		if (!m_pDX17->iLDCVElevatorAlignIn && m_pDX17->iLDCVElevatorAlignOut) 
		{
			g_objLogFile.Save_SeqLog("MCC,26,LoadConvayorElevator,14,Elevator Align Out Check and Align Down");

			m_pDY17->oLDCVElevatorAlignUp = FALSE;   m_pDY17->oLDCVElevatorAlignDn = TRUE;
			g_objAJinAXL.Write_Output(17);
			m_nLDCVElevatorCase++; m_tLDCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 15:
		if (!m_pDX17->iLDCVElevatorAlignUp && m_pDX17->iLDCVElevatorAlignDn)
		{
			g_objLogFile.Save_SeqLog("MCC,26,LoadConvayorElevator,15,Elevator Align Down Check and Transfer Up");

			g_objCommon.Move_Position(AX_LDCV_ELEVATOR_Z, 2);	//TransferUp
			m_nLDCVElevatorCase++; m_tLDCVElevatorLoop.Set_LoopTime(30000);
		}
		break;
	case 16:
		if (g_objCommon.Check_Position(AX_LDCV_ELEVATOR_Z, 2)) 
		{
			g_objLogFile.Save_SeqLog("MCC,26,LoadConvayorElevator,15,Transfer Up Move Done");
			m_nLDCVElevatorCase = 20; m_tLDCVElevatorLoop.Set_LoopTime(10000);
		}
		break;

	case 20:	// Wait-GoodMZ
		return TRUE;
	case 21:
			m_nLDCVElevatorCase = 0; m_tLDCVElevatorLoop.Set_LoopTime(10000);
		break;
	}

	// 26. (Error : 2300)
	if (m_tLDCVElevatorLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(2300 + m_nLDCVElevatorCase);
		return FALSE;
	}
	return TRUE;
}

// 27. (Error : 2400)
BOOL CSequenceMain::Run_ULCVElevator()
{
	static CString sULMZID;

	switch (m_nULCVElevatorCase) {
	case 0:	// Wait
		return TRUE;

	case 11:	//NG Conveyor-Out(2F)
		if (m_pDX17->iULCVElevatorMZExist) {
			g_objCommon.Move_Position(AX_ULCV_ELEVATOR_Z, 0);	//2F-NG
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 12:
		if (g_objCommon.Check_Position(AX_ULCV_ELEVATOR_Z, 0)) {
			m_pDY17->oULCVElevatorCVCCW = TRUE; m_pDY17->oULCVElevatorCVCW = TRUE;
			g_objAJinAXL.Write_Output(17);
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 13:
		if (m_pDX19->iULCV2FStop) {
			m_pDY19->oULCV1CCW2F = TRUE;   m_pDY19->oULCV1CW2F = TRUE;
			m_pDY19->oULCV2CCW2F = TRUE;   m_pDY19->oULCV2CW2F = TRUE;
			g_objAJinAXL.Write_Output(19);
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 14:
		if (!m_pDX17->iULCVElevatorMZExist && !m_pDX17->iULCVElevatorStop && !m_pDX19->iULCV2FStop) {
			m_pDY17->oULCVElevatorCVCCW = FALSE; m_pDY17->oULCVElevatorCVCW = FALSE;
			g_objAJinAXL.Write_Output(17);
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 15:
		if (!m_pDX19->iULCV2FStop) {
			m_pDY19->oULCV1CCW2F = FALSE;   m_pDY19->oULCV1CW2F = FALSE;
			m_pDY19->oULCV2CCW2F = FALSE;   m_pDY19->oULCV2CW2F = FALSE;
			g_objAJinAXL.Write_Output(19);
			m_nBarRetry3 = 0;
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 16:
		if (m_pEquipData->bUseMZIDUnload) {
			if (!m_tULCVElevatorLoop.Waiting_Time(500)) break;
			g_objBarcodeLot_Cognex.Set_Trigger(3, TRUE);	//2F
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(5000);
		} else {
			m_nULCVElevatorCase = 19; m_tULCVElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 17:
			sULMZID = g_objBarcodeLot_Cognex.Get_BarcodeLot(3);	//U-2F
			if (sULMZID.GetLength() > 0) {
				g_objLogFile.Save_RFBarData(3, sULMZID);
				gData.sMZID[9] = sULMZID;
				m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(10000);
			}
		break;
	case 18:	//MES 요청
#ifndef AJIN_BOARD_USE
			g_objMesAgent.Set_MGZIDRemove("N", gData.sMZID[9], gData.sRecipeName);
#endif
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(5000);
		break;
	case 19:
			gCap.nCVMZIndex[1] = gCap.nTransferMZIndex[1];
			g_dlgOCAPCosmetic.AddMZOut(gTracking.sMZID_NGCV, "NG");
			g_objCommon.Move_Position(AX_ULCV_ELEVATOR_Z, 2);	//TransferDown
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(5000);
		break;
	case 20:
		if (m_nUnloadConveyorCase == 0) {
			m_sLog.Format("[MZTransfer: Unload] NG-Conveyor(2F) => MZID(%s)", gData.sMZID[9]);
			g_objLogFile.Save_HandlerLog(m_sLog);
			gData.sMZID[9] = "";
			m_nUnloadConveyorCase = 11;	//2F구동
			m_nULCVElevatorCase = 0; m_tULCVElevatorLoop.Set_LoopTime(10000);
		}
		return TRUE;

	case 21:	//Good Conveyor-Out(1F)
		if (m_pDX17->iULCVElevatorMZExist) {
			g_objCommon.Move_Position(AX_ULCV_ELEVATOR_Z, 1);	//1F-Good
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(30000);
		}
		break;
	case 22:
		if (g_objCommon.Check_Position(AX_ULCV_ELEVATOR_Z, 1)) {
			m_pDY17->oULCVElevatorCVCCW = TRUE; m_pDY17->oULCVElevatorCVCW = TRUE;
			g_objAJinAXL.Write_Output(17);
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 23:
		if (m_pDX19->iULCV1FStop) {
			m_pDY19->oULCV3CCW1F = TRUE;   m_pDY19->oULCV3CW1F = TRUE;
			m_pDY19->oULCV4CCW1F = TRUE;   m_pDY19->oULCV4CW1F = TRUE;
			g_objAJinAXL.Write_Output(19);
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 24:
		if (!m_pDX17->iULCVElevatorMZExist && !m_pDX17->iULCVElevatorStop && !m_pDX19->iULCV1FStop) {
			m_pDY17->oULCVElevatorCVCCW = FALSE; m_pDY17->oULCVElevatorCVCW = FALSE;
			g_objAJinAXL.Write_Output(17);
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 25:
		if (!m_pDX19->iULCV1FStop) {
			m_pDY19->oULCV3CCW1F = FALSE;   m_pDY19->oULCV3CW1F = FALSE;
			m_pDY19->oULCV4CCW1F = FALSE;   m_pDY19->oULCV4CW1F = FALSE;
			g_objAJinAXL.Write_Output(19);
			m_nBarRetry4 = 0;
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 26:
		if (m_pEquipData->bUseMZIDUnload) {
			if (!m_tULCVElevatorLoop.Waiting_Time(500)) break;
			g_objBarcodeLot_Cognex.Set_Trigger(4, TRUE);	//1F
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(5000);
		} 
		else 
		{
			g_dlgOCAPFai.AddMZOut(gData.sMZID[8]);
			g_dlgOCAPCosmetic.AddMZOut(gData.sMZID[8], "GOOD");
			m_nULCVElevatorCase = 29; m_tULCVElevatorLoop.Set_LoopTime(15000);
		}
		break;
	case 27:
			sULMZID = g_objBarcodeLot_Cognex.Get_BarcodeLot(4);	//U-1F
			if (sULMZID.GetLength() > 0) {
				g_objLogFile.Save_RFBarData(2, sULMZID);
				gData.sMZID[8] = sULMZID;
				m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(10000);
			}
		break;
	case 28:	//MES 요청
		if (m_pEquipData->bUseMZIDUnload) 
		{
			g_objMesAgent.Set_MGZIDRemove("G", gData.sMZID[8], gData.sRecipeName);
			g_dlgOCAPFai.AddMZOut(gData.sMZID[8]);
			g_dlgOCAPCosmetic.AddMZOut(gData.sMZID[8], "GOOD");
		}		
		m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(15000);
		break;
	case 29:
		if (!m_pDX19->iULCV1FStop) {
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(15000);
		}
		break;
	case 30:
		if (m_nUnloadConveyorCase == 0) {
			m_sLog.Format("[MZTransfer: Unload] Good-Conveyor(1F) => MZID(%s)", gData.sMZID[8]);
			g_objLogFile.Save_HandlerLog(m_sLog);
			
			m_nUnloadConveyorCase = 1;	//1F구동
			if (Check_ModuleEmpty()) Set_JobTack(2);
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(3000);
		}
		return TRUE;

	case 31:
		if (m_pDX19->iULCV1FCnt1) {


			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 32:
		if (!m_pDX19->iULCV1FCnt1) {
			gCap.nCVMZIndex[0] = gCap.nTransferMZIndex[0];
			
			gData.sMZID[8] = "";
			g_objCommon.Move_Position(AX_ULCV_ELEVATOR_Z, 2);	//TransferDown
			m_nULCVElevatorCase = 0; m_tULCVElevatorLoop.Set_LoopTime(10000);

		}
		break;

	}

	// 27. (Error : 2400)
	if (m_tULCVElevatorLoop.Over_LoopTime()) {
		if (m_nULCVElevatorCase == 17) {
			m_nBarRetry3++; m_nULCVElevatorCase = 16;
			if (m_nBarRetry3 < 3) {
				g_objBarcodeLot_Cognex.Set_Trigger(3, FALSE); return TRUE;
			}
		}
		if (m_nULCVElevatorCase == 27) {
			m_nBarRetry4++; m_nULCVElevatorCase = 26;
			if (m_nBarRetry4 < 3) {
				g_objBarcodeLot_Cognex.Set_Trigger(4, FALSE); return TRUE;
			}
		}
		if (m_nULCVElevatorCase == 31) {
			m_nULCVElevatorCase++; m_tULCVElevatorLoop.Set_LoopTime(3000);
			return TRUE;
		}
		g_objCommon.Show_Error(2400 + m_nULCVElevatorCase);
		return FALSE;
	}
	return TRUE;
}

// 28. (Error : 2500)
BOOL CSequenceMain::Run_LDMZElevator()
{
	static int nSlotNo = 0, nLDPort = 0, nLastSlotNo = 0;
	static double dSlotPosZ = 0.0;

	switch (m_nLDMZElevatorCase) {
	case 0:	// Wait
		return TRUE;

	case 1:
		if (m_pDX18->iLDMZElevatorExist)
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d,iLDMZElevatorExist Checked and Clamp12 In", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY18->oLDMZElevatorClamp12In = TRUE; m_pDY18->oLDMZElevatorClamp12Out = FALSE;
			g_objAJinAXL.Write_Output(18);
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 2:
		if (m_pDX18->iLDMZElevatorClamp12In && !m_pDX18->iLDMZElevatorClamp12Out) 
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d,Clamp12 In Done and Clamp34 In", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY18->oLDMZElevatorClamp34In = TRUE; m_pDY18->oLDMZElevatorClamp34Out = FALSE;
			g_objAJinAXL.Write_Output(18);
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 3:
		if (m_pDX18->iLDMZElevatorClamp34In && !m_pDX18->iLDMZElevatorClamp34Out) 
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d,Clamp 34 In Done", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 4:
		if (m_pDX18->iLDMZElevatorExist) 
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d,iLDMZElevatorExist Checked", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 5:
		if (!m_pDX15->iLDRailCarrierChk1 && !m_pDX15->iLDRailCarrierChk2) 
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d,NoCarrier Sensed LDMZ_ELEVATOR_Z Move to Sensing Pre Pos", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_LDMZ_ELEVATOR_Z, 1);	//Mapping
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 6:
		if (g_objCommon.Check_Position(AX_LDMZ_ELEVATOR_Z, 1)) 
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d, LDMZ_ELEVATOR_Z Move Done and Info Initial", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			nSlotNo = nLastSlotNo = 0;
			for(int i=0; i<8; i++) gLot.nCarrierExist[0][i] = 0;
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 7:
		if (g_objAJinAXL.Is_Done(AX_LDMZ_ELEVATOR_Z)) 
		{
			if (!m_tLDMZElevatorLoop.Waiting_Time(500)) break;
			if (m_pDX15->iLDMZCarrierExist) gLot.nCarrierExist[0][nSlotNo] = 1;

			if (nSlotNo >= 7) m_nLDMZElevatorCase = 9;
			else			  m_nLDMZElevatorCase++;
			m_tLDMZElevatorLoop.Set_LoopTime(5000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, Carrier Exist Slot No :%d and Exist : %d", m_nLDMZElevatorCase, nSlotNo, gLot.nCarrierExist[0][nSlotNo]);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 8:
		if (g_objAJinAXL.Is_Done(AX_LDMZ_ELEVATOR_Z)) {
			nSlotNo++;
			g_objAJinAXL.Move_Relative(AX_LDMZ_ELEVATOR_Z, m_pEquipData->dMZPitchZ*-1.0);
			m_nLDMZElevatorCase--; m_tLDMZElevatorLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, LDMZ_ELEVATOR_Z Pitch Move Slot No: %d", m_nLDMZElevatorCase, nSlotNo);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 9:
		if (Check_MZCarrierExit(1) > 0) {
			gData.nMZSlotNo[3] = nLastSlotNo = 0;
			for(int i=7; i>=0; i--) 
			{
//			for(int i=0; i<8; i++) {
				if (gLot.nCarrierExist[0][i] == 1) {
					nLastSlotNo = i + 1; break;
				}
			}
			if (nLastSlotNo > 0) {
				gData.nMZSlotNo[3] = nLastSlotNo;

				m_sLog.Format("Load-MZ Carrier Check MZ[%s] [%d-%d-%d-%d - %d-%d-%d-%d] End[%d]",
								gData.sMZID[4], gLot.nCarrierExist[0][0], gLot.nCarrierExist[0][1], gLot.nCarrierExist[0][2], gLot.nCarrierExist[0][3], gLot.nCarrierExist[0][4], gLot.nCarrierExist[0][5], gLot.nCarrierExist[0][6], gLot.nCarrierExist[0][7], gData.nMZSlotNo[3]);
				g_objLogFile.Save_HandlerLog(m_sLog);

				m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);

				m_sLog.Format("MCC,28,LDMZElevator,%d, Slot Check", m_nLDMZElevatorCase);
				g_objLogFile.Save_SeqLog(m_sLog);

			}
		}
		break;

	case 10:
		nSlotNo = Check_MZCarrierExit(1);
		if (nSlotNo > 0) 
		{
			if (gData.bCycleStop)
			{
				m_nLDMZElevatorCase = 40; m_tLDMZElevatorLoop.Set_LoopTime(5000);

				m_sLog.Format("MCC,28,LDMZElevator,%d, Cycle Stop", m_nLDMZElevatorCase);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
			else
			{
				gData.nMZSlotNo[0] = nSlotNo;//MZ의 가장 아래 Slot 부터 채움 
				dSlotPosZ = m_pMoveData->dLDMZElevatorZ[2] - (m_pEquipData->dMZPitchZ * (nSlotNo-1));
				g_objAJinAXL.Move_Absolute(AX_LDMZ_ELEVATOR_Z, dSlotPosZ);
				m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);

				m_sLog.Format("MCC,28,LDMZElevator,%d, Load MZ Elev Z Pitch move : %lf & Slot No:%d", m_nLDMZElevatorCase, dSlotPosZ, nSlotNo);
				g_objLogFile.Save_SeqLog(m_sLog);
			}
		} 
		else 
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d, Load MZ Elev Z Pitch move", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nLDMZElevatorCase = 40; m_tLDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 11:
		if (g_objAJinAXL.Is_MoveDone(AX_LDMZ_ELEVATOR_Z, dSlotPosZ))
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d, Load MZ Elev Z move done", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 12:
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(20000);
		break;
	case 13:
		if (!m_pDX15->iLDRailCarrierChk1 && !m_pDX15->iLDRailCarrierChk2) 
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d, Load Feeder Y Move to Grip", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Override(AX_LOAD_FEEDER_Y, 1, 20.0);	//Grip
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(30000);
		}
		break;
	case 14:
		if (g_objCommon.Check_Position(AX_LOAD_FEEDER_Y, 1)) 
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d, Load Feeder Y Move to Grip done and Grip Close", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY15->oLDGripOpen = FALSE; m_pDY15->oLDGripClose = TRUE;
			g_objAJinAXL.Write_Output(15);
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 15:
		if (!m_pDX15->iLDGripOpen && m_pDX15->iLDGripClose)
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d, Load Grip Close done", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 16:
		if (m_pDX15->iLDGripCarrierChk) 
		{
			m_sLog.Format("MCC,28,LDMZElevator,%d, Load Feeder Y move to pull", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_LOAD_FEEDER_Y, 2);	//Pull
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(30000);
		}
		break;
	case 17:
		if (g_objCommon.Check_Position(AX_LOAD_FEEDER_Y, 2)) 
		{
			if (!m_tLDMZElevatorLoop.Waiting_Time(300)) break;
			m_pDY15->oLDGripOpen = TRUE; m_pDY15->oLDGripClose = FALSE;
			g_objAJinAXL.Write_Output(15);
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, Load Feeder Y move to pull done and Grip Open", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 18:
		if (m_pDX15->iLDGripOpen && !m_pDX15->iLDGripClose) 
		{
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, Grip Open done", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 19:
		if (!m_pDX15->iLDRailCarrierChk1 && m_pDX15->iLDRailCarrierChk2) 
		{
			g_objCommon.Move_Position(AX_LOAD_FEEDER_Y, 0);	//Pull-Next
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, No Carrier checked", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 20:
		if (g_objCommon.Check_Position(AX_LOAD_FEEDER_Y, 0))
		{			
			gLot.nCarrierExist[0][nSlotNo-1] = 0;
			gData.nSlotNo_LDMZ = nSlotNo;
			if (nSlotNo == nLastSlotNo) gData.nLDMZ_LastCarrier[0] = 1;
			else						gData.nLDMZ_LastCarrier[0] = 0;
						
			gTracking.sMZID_LoadMZElev = gData.sMZID[4];
			m_sLog.Format("sMZID_LoadMZElev, %s", gTracking.sMZID_LoadMZElev);
			g_objLogFile.Save_HomeTrackingLog(m_sLog);
		
			m_nLDMZElevatorCase = 30; m_tLDMZElevatorLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, Slot NO : %d & Last Carrier Check : %d", m_nLDMZElevatorCase, nSlotNo, gData.nLDMZ_LastCarrier[0]);
			g_objLogFile.Save_SeqLog(m_sLog);

		}
		break;

	case 30:	// Carrier-Out Wait
		return TRUE;
	case 31:
		if (!m_pDX15->iLDRailCarrierChk1 && !m_pDX15->iLDRailCarrierChk2) 
		{
			if (Check_MZCarrierExit(1) > 0)
			{
				m_nLDMZElevatorCase = 10;
			}
			else
			{
				m_nLDMZElevatorCase = 40;
			}
			m_tLDMZElevatorLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, No Carrier Checked", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 40:
		if (!m_pDX15->iLDRailCarrierChk1)
		{
			g_objCommon.Move_Position(AX_LDMZ_ELEVATOR_Z, 0);	//Ready
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, Load MZ Elev Z Back to Ready", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 41:
		if (g_objCommon.Check_Position(AX_LDMZ_ELEVATOR_Z, 0)) 
		{
			m_pDY18->oLDMZElevatorClamp34In = FALSE; m_pDY18->oLDMZElevatorClamp34Out = TRUE;
			g_objAJinAXL.Write_Output(18);
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, Clamp34 Out", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 42:
		if (!m_pDX18->iLDMZElevatorClamp34In && m_pDX18->iLDMZElevatorClamp34Out) {
			m_pDY18->oLDMZElevatorClamp12In = FALSE; m_pDY18->oLDMZElevatorClamp12Out = TRUE;
			g_objAJinAXL.Write_Output(18);
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, Clamp12 Out", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 43:
		if (!m_pDX18->iLDMZElevatorClamp12In && m_pDX18->iLDMZElevatorClamp12Out) {
			m_nLDMZElevatorCase++; m_tLDMZElevatorLoop.Set_LoopTime(5000);
			
			m_sLog.Format("MCC,28,LDMZElevator,%d, Clamp12 Out done", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	case 44:
		if (m_pDX18->iLDMZElevatorExist) {
			m_nLDMZElevatorCase = 50; m_tLDMZElevatorLoop.Set_LoopTime(5000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, iLDMZElevatorExist", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;

	case 50:	// MZ-Out Pick Wait
		m_tLDMZElevatorLoop.Set_LoopTime(10000);
		return TRUE;
	case 51:
		if (!m_pDX18->iLDMZElevatorExist) {
//			gData.sLotID_Magazine[0] = "";
//			gData.nTrayNo_Magazine[0] = gData.nPortNo_Magazine[0] = 0;
			m_nLDMZElevatorCase = 0; m_tLDMZElevatorLoop.Set_LoopTime(10000);

			m_sLog.Format("MCC,28,LDMZElevator,%d, iLDMZElevatorExist False", m_nLDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
		}
		break;
	}

	// 28. (Error : 2500)
	if (m_tLDMZElevatorLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(2500 + m_nLDMZElevatorCase);
		return FALSE;
	}
	return TRUE;
}

// 29. (Error : 2600)
BOOL CSequenceMain::Run_NGMZElevator()
{
	static int nSlotNo = 0, nLDPNoNG = 0, nMZNo1 = 0, nMZNo2 = 0, nMZNo3 = 0;
	static double dSlotPosZ = 0.0;

	switch (m_nNGMZElevatorCase) {
	case 0:	// Wait
		return TRUE;

	case 1:
		if (m_pDX18->iNGMZElevatorExist) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY18->oNGMZElevatorClamp12In = TRUE; m_pDY18->oNGMZElevatorClamp12Out = FALSE;
			g_objAJinAXL.Write_Output(18);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 2:
		if (m_pDX18->iNGMZElevatorClamp12In && !m_pDX18->iNGMZElevatorClamp12Out) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY18->oNGMZElevatorClamp34In = TRUE; m_pDY18->oNGMZElevatorClamp34Out = FALSE;
			g_objAJinAXL.Write_Output(18);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 3:
		if (m_pDX18->iNGMZElevatorClamp34In && !m_pDX18->iNGMZElevatorClamp34Out) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 4:
		if (m_pDX18->iNGMZElevatorExist) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			nSlotNo = 0;
			gData.sMZID_NGElevator[1] = "";
			for(int i=0; i<8; i++) {
				gLot.nCarrierExist[1][i] = 0;
//				gLot.sCarrierID[gLot.nRunPortNo-1][i] = "";
			}
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 5:
		if (!m_pDX15->iNGRailCarrierChk1 && !m_pDX15->iNGRailCarrierChk2) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_NGMZ_ELEVATOR_Z, 1);	//Mapping
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 6:
		if (g_objCommon.Check_Position(AX_NGMZ_ELEVATOR_Z, 1)) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 7:
		if (g_objAJinAXL.Is_Done(AX_NGMZ_ELEVATOR_Z))
		{
			if (!m_tNGMZElevatorLoop.Waiting_Time(500)) break;

			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			
			if (m_pDX15->iNGMZCarrierExist) gLot.nCarrierExist[1][nSlotNo] = 1;
			else							gLot.nCarrierExist[1][nSlotNo] = 0;

			if (nSlotNo >= 7) m_nNGMZElevatorCase = 9;
			else			  m_nNGMZElevatorCase++;
			m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 8:
		if (g_objAJinAXL.Is_Done(AX_NGMZ_ELEVATOR_Z)) 
		{			
			nSlotNo++;

			m_sLog.Format("SEQ,29,NGMZElevator,%d, nSlotNo:%d", m_nNGMZElevatorCase, nSlotNo);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objAJinAXL.Move_Relative(AX_NGMZ_ELEVATOR_Z, m_pEquipData->dMZPitchZ*-1.0);
			m_nNGMZElevatorCase--; m_tNGMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 9:
		if (m_pDX18->iNGMZElevatorExist)
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_sLog.Format("NG-MZ Carrier Check MZ[%s] [%d-%d-%d-%d - %d-%d-%d-%d]",
							gData.sMZID[6], gLot.nCarrierExist[1][0], gLot.nCarrierExist[1][1], gLot.nCarrierExist[1][2], gLot.nCarrierExist[1][3], gLot.nCarrierExist[1][4], gLot.nCarrierExist[1][5], gLot.nCarrierExist[1][6], gLot.nCarrierExist[1][7]);
			g_objLogFile.Save_HandlerLog(m_sLog);

			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(10000);
		}		
		break;

	case 10:
		nMZNo1 = Check_MZCarrierExit(2, 1);	//Search Out Carrier No
		m_sLog.Format("SEQ,29,NGMZElevator,%d, nMZNo1:%d", m_nNGMZElevatorCase,nMZNo1);
		g_objLogFile.Save_SeqLog(m_sLog);
		if (nMZNo1 <= 0) 
		{
			m_nNGMZElevatorCase = 60; m_tNGMZElevatorLoop.Set_LoopTime(5000);
			gData.nNGMgzOut = 0;
		}
		else if (nMZNo1 == 1)
		{
//		} else if (nMZNo1 == 8) {
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		} 
		else
		{
			nMZNo2 = Check_MZCarrierEmpty(2);
			m_sLog.Format("SEQ,29,NGMZElevator,%d, nMZNo2:%d", m_nNGMZElevatorCase, nMZNo2);
			g_objLogFile.Save_SeqLog(m_sLog);
			if (nMZNo1 > nMZNo2 && nMZNo2 > 0 && Check_NGCarrierIn(nMZNo2)) 
			{
//			if (nMZNo1 < nMZNo2 && nMZNo2 > 0 && Check_NGCarrierIn(nMZNo2)) {
				m_nNGMZElevatorCase = 21; m_tNGMZElevatorLoop.Set_LoopTime(5000);
			}
			else
			{
				m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
			}
		}
		break;
	case 11:
		if (nMZNo1 > 0) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			dSlotPosZ = m_pMoveData->dNGMZElevatorZ[2] - (m_pEquipData->dMZPitchZ * (nMZNo1-1));
			g_objAJinAXL.Move_Absolute(AX_NGMZ_ELEVATOR_Z, dSlotPosZ);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 12:
		if (g_objAJinAXL.Is_MoveDone(AX_NGMZ_ELEVATOR_Z, dSlotPosZ)) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 13:
		if (!m_pDX15->iNGRailCarrierChk1 && !m_pDX15->iNGRailCarrierChk2) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			g_objCommon.Move_Override(AX_NG_FEEDER_Y, 1, 20.0);	//Grip
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(30000);	//10000
		}
		break;
	case 14:
		if (g_objCommon.Check_Position(AX_NG_FEEDER_Y, 1)) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_pDY15->oNGGripOpen = FALSE; m_pDY15->oNGGripClose = TRUE;
			g_objAJinAXL.Write_Output(15);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 15:
		if (!m_pDX15->iNGGripOpen && m_pDX15->iNGGripClose) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 16:
		//Pull Carrier from Magazine to Rail 
		if (m_pDX15->iNGGripCarrierChk) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			g_objCommon.Move_Position(AX_NG_FEEDER_Y, 2);	//Pull
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(30000);	//5000
		}
		break;
	case 17:
		if (g_objCommon.Check_Position(AX_NG_FEEDER_Y, 2)) 
		{
			if (!m_tNGMZElevatorLoop.Waiting_Time(300)) break;

			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY15->oNGGripOpen = TRUE; m_pDY15->oNGGripClose = FALSE;
			g_objAJinAXL.Write_Output(15);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 18:
		if (m_pDX15->iNGGripOpen && !m_pDX15->iNGGripClose) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 19:
		if (!m_pDX15->iNGRailCarrierChk1 && m_pDX15->iNGRailCarrierChk2) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_NG_FEEDER_Y, 0);	//Pull-Next
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 20:
		if (g_objCommon.Check_Position(AX_NG_FEEDER_Y, 0)) 
		{						
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			gLot.nCarrierExist[1][nMZNo1-1] = 0;
			m_nNGMZElevatorCase = 30; m_tNGMZElevatorLoop.Set_LoopTime(10000);
		}
		break;

	case 21:	//Carrier in-MZ
		nMZNo2 = Check_MZCarrierEmpty(2);
		m_sLog.Format("SEQ,29,NGMZElevator,%d, nMZNo2:%d", m_nNGMZElevatorCase, nMZNo2);
		g_objLogFile.Save_SeqLog(m_sLog);
		if (nMZNo2 > 0 && Check_NGCarrierIn(nMZNo2)) 
		{
			dSlotPosZ = m_pMoveData->dNGMZElevatorZ[2] - (m_pEquipData->dMZPitchZ * (nMZNo2-1));
			g_objAJinAXL.Move_Absolute(AX_NGMZ_ELEVATOR_Z, dSlotPosZ);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		else
		{
			m_nNGMZElevatorCase = 10; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 22:
		if (g_objAJinAXL.Is_MoveDone(AX_NGMZ_ELEVATOR_Z, dSlotPosZ)) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 23:
		m_tNGMZElevatorLoop.Set_LoopTime(5000);
		return TRUE;

	case 24:
		if (m_pDX18->iNGMZElevatorExist) 
		{
			if (!m_pDX15->iNGRailCarrierChk1 && m_pDX15->iNGRailCarrierChk2) 
			{
				m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
				g_objLogFile.Save_SeqLog(m_sLog);

				g_objCommon.Move_Position(AX_NG_FEEDER_Y, 1, 0.5);	//Push
				m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 25:
		if (g_objCommon.Check_Position(AX_NG_FEEDER_Y, 1)) 
		{
			if (!m_tNGMZElevatorLoop.Waiting_Time(500)) break;
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY15->oNGGripOpen = TRUE; m_pDY15->oNGGripClose = FALSE;
			g_objAJinAXL.Write_Output(15);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 26:
		if (m_pDX15->iNGGripOpen && !m_pDX15->iNGGripClose) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_NG_FEEDER_Y, 0);	//Ready
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(30000);
		}
		break;
	case 27:
		if (g_objCommon.Check_Position(AX_NG_FEEDER_Y, 0)) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 28:
		if (!m_pDX15->iNGRailCarrierChk1 && !m_pDX15->iNGRailCarrierChk2) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			gLot.nCarrierExist[1][nMZNo2-1] = 2;
			int nPNo = nLDPNoNG = gData.nPortNo_UnMZ[0] - 1;

			gLot.sMZID_NG[nPNo] = gData.sMZID[6];
			gLot.sCarID_NG[nPNo] = gData.sCarID_Elevator[0];
			gLot.nSlotNo_NG[nPNo] = gData.nMZSlotNo[1] = nMZNo2;
			gData.sMZID_NGElevator[1] = gData.sMZID_NGElevator[0];


			for(int y = 0; y < 10; y++)
			{
				for(int x = 0; x < 4; x++)
				{
					gTracking.sMZID_NGMZ[nMZNo2-1][y][x] = gTracking.sMZID_NGMZElev[nLDPNoNG][y][x];
					gTracking.sMZID_NGMZElev[nLDPNoNG][y][x] = "";

					m_sLog.Format("NG Elev To NG MZ, No:%d x:%d Y:%d - MZ ID:%s",nMZNo2-1, x, y , gTracking.sMZID_NGMZ[nMZNo2-1][y][x]);
					if(gTracking.sMZID_NGMZ[nMZNo2-1][y][x] != "")
					{
						 
						gTracking.sMZID_NGCV = gTracking.sMZID_NGMZ[nMZNo2-1][y][x]; 
						g_objLogFile.Save_HomeTrackingLog(m_sLog);
					}
				}
			}	
			//gCap.sMZID_Later[nPNo] = gLot.sMZID_NG[nPNo];
			gData.sCarID_Elevator[0] = gData.sLotID_UnMZ[0] = gData.sMZID_NGElevator[0] = "";
			gData.nPortNo_UnMZ[0] = 0;
			
			CString sCaInLotID = gLot.sLotID[nPNo];
			int     nMZLastCar = gLot.nMZLastCar[nPNo];
			if (sCaInLotID.GetLength() < 2) { sCaInLotID = gData.sLastLotID; nMZLastCar = gData.nMZLastCar; }
			g_objMesAgent.Set_CarrierInMGZ("N", sCaInLotID, gLot.sMZID_NG[nPNo], gLot.sCarID_NG[nPNo], gLot.nSlotNo_NG[nPNo]);
			
			m_nNGMZElevatorCase = 10; m_tNGMZElevatorLoop.Set_LoopTime(5000);

			m_sLog.Format("NG Carrier In MZ1[%s] Car[%s] SlotNo[%d] LotID[%s] PNo[%d] LastCar[%d] Seq[%d] NGMgzOut[%d] LoadMZ[%s]",
							gLot.sMZID_NG[nPNo], gLot.sCarID_NG[nPNo], gLot.nSlotNo_NG[nPNo], sCaInLotID, nPNo+1, nMZLastCar, m_nNGMZElevatorCase, gData.nNGMgzOut, gData.sMZID_NGElevator[1]);
			g_objLogFile.Save_HandlerLog(m_sLog);
		}
		break;

	case 30:	// Carrier-Out Wait
		return TRUE;
	case 31:
		if (!m_pDX15->iNGRailCarrierChk1 && !m_pDX15->iNGRailCarrierChk2) 
		{
			if (g_objCommon.Check_Position(AX_NG_FEEDER_Y, 0)) 
			{
				m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
				g_objLogFile.Save_SeqLog(m_sLog);

				m_sLog.Format("NG-MZ Carrier Out MZ[%s] ==> [%d]", gData.sMZID[6], nSlotNo);	g_objLogFile.Save_HandlerLog(m_sLog);
				m_nNGMZElevatorCase = 10; m_tNGMZElevatorLoop.Set_LoopTime(10000);
			}
		}
		break;

	case 40:	//MZ Out
		if (!m_pDX15->iNGRailCarrierChk1)
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_NGMZ_ELEVATOR_Z, 0);	//Ready
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 41:
		if (g_objCommon.Check_Position(AX_NGMZ_ELEVATOR_Z, 0))
		{
			m_pDY18->oNGMZElevatorClamp34In = FALSE; m_pDY18->oNGMZElevatorClamp34Out = TRUE;
			g_objAJinAXL.Write_Output(18);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 42:
		if (!m_pDX18->iNGMZElevatorClamp34In && m_pDX18->iNGMZElevatorClamp34Out) 
		{
			m_pDY18->oNGMZElevatorClamp12In = FALSE; m_pDY18->oNGMZElevatorClamp12Out = TRUE;
			g_objAJinAXL.Write_Output(18);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 43:
		if (!m_pDX18->iNGMZElevatorClamp12In && m_pDX18->iNGMZElevatorClamp12Out) 
		{
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 44:
		if (m_pDX18->iNGMZElevatorExist)
		{
//			if (Check_MZCarrierExit(2) > 0) m_nNGMZElevatorCase = 50;
//			else							m_nNGMZElevatorCase = 0;
			
			m_nNGMZElevatorCase = 50; m_tNGMZElevatorLoop.Set_LoopTime(5000);
			if (m_nNGMZElevatorCase == 50)
			{
				m_sLog.Format("NG MZ Out MZ[%s] Car[%s] SlotNo[%d] LotID[%s] PNo[%d] LastCar[%d] GSeq[%d]",
								gLot.sMZID_NG[nLDPNoNG], gLot.sCarID_NG[nLDPNoNG], gLot.nSlotNo_NG[nLDPNoNG], gLot.sLotID[nLDPNoNG], nLDPNoNG+1, gLot.nMZLastCar[nLDPNoNG], m_nGDMZElevatorCase);
				g_objLogFile.Save_HandlerLog(m_sLog);
			}
		}
		break;

	case 50:	// MZ-Out Pick Wait
		m_tNGMZElevatorLoop.Set_LoopTime(10000);
		return TRUE;
	case 51:
		if (!m_pDX18->iNGMZElevatorExist) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			gData.sMZID_NGElevator[1] = "";
			gData.nNGMgzOut++;
			m_nNGMZElevatorCase = 0; m_tNGMZElevatorLoop.Set_LoopTime(10000);
		} 
		break;

	case 60:
		nMZNo3 = Check_MZCarrierEmpty(2);
		if (nMZNo3 > 0)
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			dSlotPosZ = m_pMoveData->dNGMZElevatorZ[2] - (m_pEquipData->dMZPitchZ * (nMZNo3-1));
			g_objAJinAXL.Move_Absolute(AX_NGMZ_ELEVATOR_Z, dSlotPosZ);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		} 
		else
		{
			m_nNGMZElevatorCase = 40; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 61:
		if (g_objAJinAXL.Is_MoveDone(AX_NGMZ_ELEVATOR_Z, dSlotPosZ)) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			
			m_nNGMZElevatorCase = 70; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;

	case 70:	// Carrier-In Wait
		if (Check_ModuleEmpty()) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, Check_ModuleEmpty", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nNGMZElevatorCase = 40; gData.nNGMgzOut = 1; 
		}
		m_tNGMZElevatorLoop.Set_LoopTime(5000);
		return TRUE;

	case 71:
		if (m_pDX18->iNGMZElevatorExist) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			if (!m_pDX15->iNGRailCarrierChk1 && m_pDX15->iNGRailCarrierChk2) 
			{
	//			g_objCommon.Move_Override(AX_NG_FEEDER_Y, 1, 10.0);	//Push
				g_objCommon.Move_Position(AX_NG_FEEDER_Y, 1, 0.5);	//Push
				m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 72:
		if (g_objCommon.Check_Position(AX_NG_FEEDER_Y, 1)) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			if (!m_tNGMZElevatorLoop.Waiting_Time(500)) break;
			m_pDY15->oNGGripOpen = TRUE; m_pDY15->oNGGripClose = FALSE;
			g_objAJinAXL.Write_Output(15);
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 73:
		if (m_pDX15->iNGGripOpen && !m_pDX15->iNGGripClose) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_NG_FEEDER_Y, 0);	//Ready
			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(30000);
		}
		break;
	case 74:
		if (g_objCommon.Check_Position(AX_NG_FEEDER_Y, 0)) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nNGMZElevatorCase++; m_tNGMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 75:
		if (!m_pDX15->iNGRailCarrierChk1 && !m_pDX15->iNGRailCarrierChk2) 
		{
			m_sLog.Format("SEQ,29,NGMZElevator,%d, comments", m_nNGMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);


			gLot.nCarrierExist[1][nMZNo3-1] = 2;
			int nPNo = nLDPNoNG = gData.nPortNo_UnMZ[0] - 1;

			gLot.sMZID_NG[nPNo] = gData.sMZID[6];
			gLot.sCarID_NG[nPNo] = gData.sCarID_Elevator[0];
			gLot.nSlotNo_NG[nPNo] = gData.nMZSlotNo[1] = nMZNo3;
			gData.sMZID_NGElevator[1] = gData.sMZID_NGElevator[0];

			gData.sCarID_Elevator[0] = gData.sLotID_UnMZ[0] = gData.sMZID_NGElevator[0] = "";
			gData.nPortNo_UnMZ[0] = 0;

			CString sCaInLotID = gLot.sLotID[nPNo];
			int     nMZLastCar = gLot.nMZLastCar[nPNo];
			if (sCaInLotID.GetLength() < 2) { sCaInLotID = gData.sLastLotID; nMZLastCar = gData.nMZLastCar; }
			g_objMesAgent.Set_CarrierInMGZ("N", sCaInLotID, gLot.sMZID_NG[nPNo], gLot.sCarID_NG[nPNo], gLot.nSlotNo_NG[nPNo]);
			
			if (nMZLastCar == 1 && Check_LotEndCarrier(sCaInLotID, nPNo+1)) { m_nNGMZElevatorCase = 40; gData.nNGMgzOut = 1; }
			else														    { m_nNGMZElevatorCase = 60; }
			if (gLot.nSlotNo_NG[nPNo] == 8) m_nNGMZElevatorCase = 40;
//			if (gLot.nSlotNo_NG[nPNo] == 1) m_nNGMZElevatorCase = 40;
			m_tNGMZElevatorLoop.Set_LoopTime(5000);

			m_sLog.Format("NG Carrier In MZ2[%s] Car[%s] SlotNo[%d] LotID[%s] PNo[%d] LastCar[%d] Seq[%d] NGMgzOut[%d] LoadMZ[%s]",
							gLot.sMZID_NG[nPNo], gLot.sCarID_NG[nPNo], gLot.nSlotNo_NG[nPNo], sCaInLotID, nPNo+1, nMZLastCar, m_nNGMZElevatorCase, gData.nNGMgzOut, gData.sMZID_NGElevator[1]);
			g_objLogFile.Save_HandlerLog(m_sLog);

//			if (Check_LotEndTray(gLot.sLotID[nPNo], nPNo+1)) Set_LotEnd(gLot.sLotID[nPNo], nPNo+1, gLot.sMZID_NG[nPNo], gLot.sCarID_NG[nPNo], "N");
		}
		break;

	}

	// 29. (Error : 2600)
	if (m_tNGMZElevatorLoop.Over_LoopTime()) {
		if (m_nNGMZElevatorCase == 0 || m_nNGMZElevatorCase == 60 || m_nNGMZElevatorCase == 70) return TRUE;
		g_objCommon.Show_Error(2600 + m_nNGMZElevatorCase);
		return FALSE;
	}
	return TRUE;
}

// 30. (Error : 2700)
BOOL CSequenceMain::Run_GDMZElevator()
{
	static int nSlotNo = 0, nLDPort = 0, nInPNo = 0;
	static double dSlotPosZ = 0.0;

	switch (m_nGDMZElevatorCase) {
	case 0:	// Wait
		if (m_pDX18->iGDMZElevatorExist) {
			if (g_objCommon.Check_Position(AX_MZ_TRANSFER_Z, 0)) 
			{
//				if (!m_tGDMZElevatorLoop.Waiting_Time(5000)) break;
				m_sLog.Format("MCC,30,GDMZElevator,%d, Check MZ Exist,iGDMZElevatorExist:%d", m_nGDMZElevatorCase, m_pDX18->iGDMZElevatorExist);
				g_objLogFile.Save_SeqLog(m_sLog);
				m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
			}
		}
		return TRUE;
	//Load MZ 에서 Carrier가 모두 배출 된 이후에 GOOD MZ Elev로 비어진 MZ이 이동됨 
	case 1:
		if (m_pDX18->iGDMZElevatorExist) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments, m_pDX18->iGDMZElevatorExist %d", m_nGDMZElevatorCase, m_pDX18->iGDMZElevatorExist);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY18->oGDMZElevatorClamp12In = TRUE; m_pDY18->oGDMZElevatorClamp12Out = FALSE;
			g_objAJinAXL.Write_Output(18);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);

			
		}
		break;
	case 2:
		if (m_pDX18->iGDMZElevatorClamp12In && !m_pDX18->iGDMZElevatorClamp12Out) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, Clamp In, m_pDX18->iGDMZElevatorExist %d", m_nGDMZElevatorCase, m_pDX18->iGDMZElevatorExist);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY18->oGDMZElevatorClamp34In = TRUE; m_pDY18->oGDMZElevatorClamp34Out = FALSE;
			g_objAJinAXL.Write_Output(18);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 3:
		if (m_pDX18->iGDMZElevatorClamp34In && !m_pDX18->iGDMZElevatorClamp34Out) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 4:
		if (m_pDX18->iGDMZElevatorExist) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 5:
		if (!m_pDX15->iGDRailCarrierChk1 && !m_pDX15->iGDRailCarrierChk2)
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, Rail Check", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_GDMZ_ELEVATOR_Z, 1);	//Mapping
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 6:
		if (g_objCommon.Check_Position(AX_GDMZ_ELEVATOR_Z, 1)) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			nSlotNo = 0;
			for(int i=0; i<8; i++) {
				gLot.nCarrierExist[2][i] = 0;
				m_sGdMZInLot[i] = "";
			}
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 7:
		if (g_objAJinAXL.Is_Done(AX_GDMZ_ELEVATOR_Z)) 
		{
			if (!m_tGDMZElevatorLoop.Waiting_Time(500)) break;
			gLot.nCarrierExist[2][nSlotNo] = 0;
			if (m_pDX15->iGDMZCarrierExist) {
				gLot.nCarrierExist[2][nSlotNo] = 1;
			}
			if (nSlotNo >= 7) m_nGDMZElevatorCase = 10;
			else			  m_nGDMZElevatorCase++;
			m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 8:
		if (g_objAJinAXL.Is_Done(AX_GDMZ_ELEVATOR_Z))
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments, nSlotNo:%d", m_nGDMZElevatorCase, nSlotNo);
			g_objLogFile.Save_SeqLog(m_sLog);

			nSlotNo++;
			g_objAJinAXL.Move_Relative(AX_GDMZ_ELEVATOR_Z, m_pEquipData->dMZPitchZ*-1.0);
			m_nGDMZElevatorCase--; m_tGDMZElevatorLoop.Set_LoopTime(10000);
		}
		break;

	case 10:
		nSlotNo = Check_MZCarrierExit(3);
		m_sLog.Format("MCC,30,GDMZElevator,%d, comments, nSlotNo:%d", m_nGDMZElevatorCase, nSlotNo);
		g_objLogFile.Save_SeqLog(m_sLog);
		if (nSlotNo > 0) 
		{
			m_nGDMZElevatorCase = 1; m_tGDMZElevatorLoop.Set_LoopTime(5000);
			g_objCommon.Show_Error(2710);
			return FALSE;
		} 
		else
		{
			m_nGDMZElevatorCase = 60; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
/*
		if (nSlotNo > 0) {
			dSlotPosZ = m_pMoveData->dGDMZElevatorZ[2] - (m_pEquipData->dMZPitchZ * (nSlotNo-1));
			g_objAJinAXL.Move_Absolute(AX_GDMZ_ELEVATOR_Z, dSlotPosZ);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		} else {
			m_nGDMZElevatorCase = 60; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
*/
		break;
	case 11:
		if (g_objAJinAXL.Is_MoveDone(AX_GDMZ_ELEVATOR_Z, dSlotPosZ))
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 12:
		m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
		g_objLogFile.Save_SeqLog(m_sLog);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(20000);
		break;
	case 13:
		if (!m_pDX15->iGDRailCarrierChk1 && !m_pDX15->iGDRailCarrierChk2) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Override(AX_GOOD_FEEDER_Y, 1, 20.0);	//Grip
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(30000);	//10000
		}
		break;
	case 14:
		if (g_objCommon.Check_Position(AX_GOOD_FEEDER_Y, 1))
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY15->oGDGripOpen = FALSE; m_pDY15->oGDGripClose = TRUE;
			g_objAJinAXL.Write_Output(15);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 15:
		if (!m_pDX15->iGDGripOpen && m_pDX15->iGDGripClose) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 16:
		if (m_pDX15->iGDGripCarrierChk)
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_GOOD_FEEDER_Y, 2);	//Pull
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(30000);	//5000
		}
		break;
	case 17:
		if (g_objCommon.Check_Position(AX_GOOD_FEEDER_Y, 2)) 
		{
			if (!m_tGDMZElevatorLoop.Waiting_Time(300)) break;
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
	
			m_pDY15->oGDGripOpen = TRUE; m_pDY15->oGDGripClose = FALSE;
			g_objAJinAXL.Write_Output(15);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 18:
		if (m_pDX15->iGDGripOpen && !m_pDX15->iGDGripClose) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 19:
		if (!m_pDX15->iGDRailCarrierChk1 && m_pDX15->iGDRailCarrierChk2) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_GOOD_FEEDER_Y, 0);	//Pull-Next
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 20:
		if (g_objCommon.Check_Position(AX_GOOD_FEEDER_Y, 0)) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			gLot.nCarrierExist[2][nSlotNo-1] = 0;
//			gData.nTrayNo_Magazine[2] = nSlotNo;
			m_nGDMZElevatorCase = 30; m_tGDMZElevatorLoop.Set_LoopTime(10000);
		}
		break;

	case 30:	// Carrier-Out Wait
		return TRUE;
	case 31:
		if (!m_pDX15->iGDRailCarrierChk1 && !m_pDX15->iGDRailCarrierChk2) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_nGDMZElevatorCase = 10; m_tGDMZElevatorLoop.Set_LoopTime(10000);
		}
		break;

	case 40:
		if (!m_pDX15->iGDRailCarrierChk1) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_GDMZ_ELEVATOR_Z, 0);	//Ready
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 41:
		if (g_objCommon.Check_Position(AX_GDMZ_ELEVATOR_Z, 0)) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY18->oGDMZElevatorClamp34In = FALSE; m_pDY18->oGDMZElevatorClamp34Out = TRUE;
			g_objAJinAXL.Write_Output(18);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(10000);
		}
		break;
	case 42:
		if (!m_pDX18->iGDMZElevatorClamp34In && m_pDX18->iGDMZElevatorClamp34Out)
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY18->oGDMZElevatorClamp12In = FALSE; m_pDY18->oGDMZElevatorClamp12Out = TRUE;
			g_objAJinAXL.Write_Output(18);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 43:
		if (!m_pDX18->iGDMZElevatorClamp12In && m_pDX18->iGDMZElevatorClamp12Out) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 44:
		if (m_pDX18->iGDMZElevatorExist) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 45:
		if (gData.nNGMgzOut > 1)
		{
			m_nGDMZElevatorCase = 50;
		}
		if (Check_LotEndTray(gData.sLastLotID, nInPNo+1)) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, Check_LotEndTray", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			if (m_nNGMZElevatorCase == 0)  m_nGDMZElevatorCase = 50;
		}
		if (m_nGDMZElevatorCase == 45 && m_nNGMZElevatorCase == 70 && m_nTransfer2Case == 0) 
		{
			int nFound = 0;
//			for(int i=0; i<8; i++) {
			for(int i=7; i>=0; i--) {
				if (Check_LotEndLast(m_sGdMZInLot[i]) == FALSE) nFound++;
			}
			if (nFound == 0) 
			{	
				m_sLog.Format("MCC,30,GDMZElevator,%d, Check_LotEndLast", m_nGDMZElevatorCase);
				g_objLogFile.Save_SeqLog(m_sLog);
				m_nNGMZElevatorCase = 40;				
			}
		}
		m_tGDMZElevatorLoop.Set_LoopTime(5000);
			
		if (m_nGDMZElevatorCase == 50) {
			m_sLog.Format("GD MZ Out MZ[%s] Car[%s] SlotNo[%d] LotID[%s] PNo[%d] LastCar[%d] NSeq[%d] NGMgzOut[%d]",
							gLot.sMZID_GD[nInPNo], gLot.sCarID_GD[nInPNo], gLot.nSlotNo_GD[nInPNo], gLot.sLotID[nInPNo], nInPNo+1, gLot.nMZLastCar[nInPNo], m_nNGMZElevatorCase, gData.nNGMgzOut);
			g_objLogFile.Save_HandlerLog(m_sLog);
		}
		return TRUE;

	case 50:	// MZ-Out Pick Wait
		m_tGDMZElevatorLoop.Set_LoopTime(10000);
		return TRUE;
	case 51:
		if (!m_pDX18->iGDMZElevatorExist) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, gData.nNGMgzOut:%d", m_nGDMZElevatorCase, gData.nNGMgzOut);
			g_objLogFile.Save_SeqLog(m_sLog);

			if (gData.nNGMgzOut > 1) gData.nNGMgzOut = 0;
			m_nGDMZElevatorCase = 0; m_tGDMZElevatorLoop.Set_LoopTime(10000);
		}
		break;

	case 60:
		nSlotNo = Check_MZCarrierEmpty(3);
		m_sLog.Format("MCC,30,GDMZElevator,%d, nSlotNo:%d", m_nGDMZElevatorCase,nSlotNo);
		g_objLogFile.Save_SeqLog(m_sLog);
		if (nSlotNo > 0) 
		{
			dSlotPosZ = m_pMoveData->dGDMZElevatorZ[2] - (m_pEquipData->dMZPitchZ * (nSlotNo-1));
			g_objAJinAXL.Move_Absolute(AX_GDMZ_ELEVATOR_Z, dSlotPosZ);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		else
		{
			m_nGDMZElevatorCase = 40; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 61:
		if (g_objAJinAXL.Is_MoveDone(AX_GDMZ_ELEVATOR_Z, dSlotPosZ)) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nGDMZElevatorCase = 70; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;

	case 70:	// Push Wait, Carrier from Rail To Magazine 
		if (Check_ModuleEmpty()) m_nGDMZElevatorCase = 40;
//		if (m_nLDMZElevatorCase == 50 && m_pDX18->iMZBufferExist && m_pDX16->iLDVC1FStop && nSlotNo <= 1) m_nGDMZElevatorCase = 40;
		m_tGDMZElevatorLoop.Set_LoopTime(10000);
		return TRUE;

	case 71:
		if (m_pDX18->iGDMZElevatorExist) 
		{
			if (!m_pDX15->iGDRailCarrierChk1 && m_pDX15->iGDRailCarrierChk2) 
			{
				m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
				g_objLogFile.Save_SeqLog(m_sLog);
	//			g_objCommon.Move_Override(AX_GOOD_FEEDER_Y, 1, 10.0);	//Push
				g_objCommon.Move_Position(AX_GOOD_FEEDER_Y, 1, 0.5);	//Push
				m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 72:
		if (g_objCommon.Check_Position(AX_GOOD_FEEDER_Y, 1)) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			m_pDY15->oGDGripOpen = TRUE; m_pDY15->oGDGripClose = FALSE;
			g_objAJinAXL.Write_Output(15);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 73:
		if (m_pDX15->iGDGripOpen && !m_pDX15->iGDGripClose) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			g_objCommon.Move_Position(AX_GOOD_FEEDER_Y, 0);	//Ready
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(30000);
		}
		break;
	case 74:
		if (g_objCommon.Check_Position(AX_GOOD_FEEDER_Y, 0)) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);
			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(5000);
		}
		break;
	case 75:
		if (!m_pDX15->iGDRailCarrierChk1 && !m_pDX15->iGDRailCarrierChk2) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			gLot.nCarrierExist[2][nSlotNo-1] = 1;
			int nPNo = nInPNo = gData.nPortNo_UnMZ[1] - 1;

			gLot.sMZID_GD[nPNo] = gData.sMZID[7];
			gLot.sCarID_GD[nPNo] = gData.sCarID_Elevator[1];
			gLot.nSlotNo_GD[nPNo] = gData.nMZSlotNo[2] = nSlotNo;

			
			for(int y = 0; y < 10; y++)
			{
				for(int x = 0; x < 4; x++)
				{
					gTracking.sMZID_GoodMZ[nSlotNo-1][y][x] = gTracking.sMZID_GoodMZElev[nInPNo][y][x];
					gTracking.sMZID_GoodMZElev[nInPNo][y][x] = "";

					m_sLog.Format("Good Elev To Good MZ No:%d x:%d Y:%d - MZ ID:%s",nSlotNo-1, x, y , gTracking.sMZID_GoodMZ[nSlotNo-1][y][x]);
					if(gTracking.sMZID_GoodMZ[nSlotNo-1][y][x] != "")
					{
						gTracking.sMZID_GOODCV = gTracking.sMZID_GoodMZ[nSlotNo-1][y][x];
						g_objLogFile.Save_HomeTrackingLog(m_sLog);
					}
				}
			}
			gData.sCarID_Elevator[1] = gData.sLotID_UnMZ[1] = "";
			gData.nPortNo_UnMZ[1] = 0;

//			if (gLot.nSlotNo_GD[nPNo] == 1) {	//Joblist
			if (gLot.nSlotNo_GD[nPNo] == 8) {	//Joblist
				CString sLog;

				if (gLot.nJobNo < 0 || gLot.nJobNo >= 100) gLot.nJobNo = 0;
				gLot.nJobPortNo[gLot.nJobNo]	= 90;
				gLot.sJobLotID[gLot.nJobNo]		= gLot.sMZID_GD[nPNo];
				gLot.nJobCmCount[gLot.nJobNo]	= 0;
				gLot.nJobGoodCount[gLot.nJobNo]	= 0;
				gLot.nJobNgCount[gLot.nJobNo]	= 0;
				gLot.dJobRate[gLot.nJobNo]		= 0.0;
				gLot.sJobStartTime[gLot.nJobNo] = "Start";
				gLot.sJobEndTime[gLot.nJobNo]	= "";
				gLot.dJobTack[gLot.nJobNo]		= 0.0;
				gLot.nJobNo++; if (gLot.nJobNo < 0 || gLot.nJobNo >= 100) gLot.nJobNo = 0;
				
				sLog.Format("%d,%s,%d,%d,%d,%0.1lf,%d,%d,%d,%d,%d,%d,%d,%s,%s,%0.5lf,0", 0, gLot.sMZID_GD[nPNo], 0, 0, 0, 0.0, 0, 0, 0, 0, 0, 0, 0, "Start", "", 0.0);
				g_objLogFile.Save_JobListLog(sLog);
			}
//			if (gLot.sLotID[nPNo].GetLength() < 1) g_objCommon.Set_LotDataCopy(30, nPNo+1);

			g_objMesAgent.Set_CarrierInMGZ("G", gLot.sLotID[nPNo], gLot.sMZID_GD[nPNo], gLot.sCarID_GD[nPNo], gLot.nSlotNo_GD[nPNo]);
			m_sLastMZID = gLot.sMZID_GD[nPNo]; m_sLastCarID = gLot.sCarID_GD[nPNo];
			m_sGdMZInLot[nSlotNo-1] = gLot.sLotID[nPNo];

			gData.sLastLotID = gLot.sLotID[nPNo];
			gData.nLastPorNo = nPNo + 1;
			gData.nMZLastCar = gLot.nMZLastCar[nPNo];

			
//			if ((gLot.nMZLastCar[nPNo] == 1 || gLot.nSlotNo_GD[nPNo] == 8) && Check_LotEndTray(gLot.sLotID[nPNo], nPNo+1)) {
			if ((gLot.nMZLastCar[nPNo] == 1 || gLot.nSlotNo_GD[nPNo] == 1) && Check_LotEndTray(gLot.sLotID[nPNo], nPNo+1)) {
				if (m_nNGMZElevatorCase == 70) { m_nNGMZElevatorCase = 40; gData.nNGMgzOut = 1; }
			}

			gData.bInpectLotEnd = FALSE;
			g_objInspector.Set_LotEnd(INSPECTOR_ALL, gLot.sLotID[nPNo], gData.nLastPorNo);

			if (!m_pEquipData->bUseBottom && !m_pEquipData->bUseBotAng && !m_pEquipData->bUseTop1 && !m_pEquipData->bUseTopAng && !m_pEquipData->bUseTop2) {
				gData.bInpectLotEnd = TRUE;
			}
			if (!m_pEquipData->bUseMES) gData.bInpectLotEnd = TRUE;

			m_nGDMZElevatorCase++; m_tGDMZElevatorLoop.Set_LoopTime(60000);
		}
		break;
	case 76:
		if (gData.bInpectLotEnd || gData.bUseDryRun || gData.bCycleStop) 
		{
			m_sLog.Format("MCC,30,GDMZElevator,%d, comments", m_nGDMZElevatorCase);
			g_objLogFile.Save_SeqLog(m_sLog);

			int nPNo = gData.nLastPorNo - 1;
			if (gLot.nMZLastCar[nPNo] == 1 || gLot.nSlotNo_GD[nPNo] == 1) m_nGDMZElevatorCase = 40; 
			else														  m_nGDMZElevatorCase = 60;
			m_tGDMZElevatorLoop.Set_LoopTime(5000);

			m_sLog.Format("GD Carrier In MZ[%s] Car[%s] SlotNo[%d] LotID[%s] PNo[%d] LastCar[%d] SeqGd[%d] SeqNG[%d] NGMgzOut[%d]",
							gLot.sMZID_GD[nPNo], gLot.sCarID_GD[nPNo], gLot.nSlotNo_GD[nPNo], gLot.sLotID[nPNo], nPNo+1, gLot.nMZLastCar[nPNo], m_nGDMZElevatorCase, m_nNGMZElevatorCase, gData.nNGMgzOut);
			g_objLogFile.Save_HandlerLog(m_sLog);

			Set_LotEnd(gLot.sLotID[nPNo], nPNo+1, gLot.sMZID_GD[nPNo], gLot.sCarID_GD[nPNo], "G");
		}
		break;

	}

	// 30. (Error : 2700)
	if (m_tGDMZElevatorLoop.Over_LoopTime()) {
		if (m_nGDMZElevatorCase == 0 || m_nGDMZElevatorCase == 60 || m_nGDMZElevatorCase == 70) return TRUE;
		g_objCommon.Show_Error(2700 + m_nGDMZElevatorCase);
		return FALSE;
	}
	return TRUE;
}

// 31. (Error : 2800)
BOOL CSequenceMain::Run_LD1FConveyor()
{
	static int nMZExit = 0;
	static CString sMZID;
	static int nLDMZNum;

	switch (m_nLD1FConveyorCase)
	{
	case 0:	// Wait
		nMZExit = Check_LoadMZ();
		if (nMZExit > 0 && !gData.bCycleStop) 
		{
			if (!m_tLD1FConveyorLoop.Waiting_Time(2000)) break;

			g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,0,LoadMZ Checked");
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);

#ifndef AJIN_BOARD_USE
			m_pDX17->iLDCVElevatorCVStop = m_pDX17->iLDCVElevatorCVStart = FALSE;
#endif

		}
		return TRUE;

	case 1:
		if (nMZExit > 0)
		{
			if (!m_tLD1FConveyorLoop.Waiting_Time(2000)) break;   
			m_pDY16->oLDCVStopper1FUp = TRUE; m_pDY16->oLDCVStopper1FDn = FALSE;
			g_objAJinAXL.Write_Output(16);

			g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,1,Load Conveyor Stopper 1F Up");
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		 }
		break;
	case 2:
		if (m_pDX16->iLDCVStopper1FUp && !m_pDX16->iLDCVStopper1FDn) 
		{
			if (m_pDX16->iLDVC1FStop)
			{
				g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,1,Load Conveyor Stopper 1F Up check and 1F Stop checked");
				m_nLD1FConveyorCase = 4;
			}
			else  
			{
				g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,2,Load Conveyor 1F All Move");
				
				m_pDY16->oLDCV3CCW1F = TRUE; m_pDY16->oLDCV3CW1F = TRUE;
				m_pDY16->oLDCV4CCW1F = TRUE; m_pDY16->oLDCV4CW1F = TRUE;
				g_objAJinAXL.Write_Output(16);
				m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(15000);


			}
		}
		break;
	case 3:
#ifndef AJIN_BOARD_USE
		m_pDX16->iLDVC1FStop = TRUE;
#endif
		if (m_pDX16->iLDVC1FStop)
		{
			g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,3,Load Conveyor 1F All Stop");
//			if (!m_tLD1FConveyorLoop.Waiting_Time(1000)) break;   
			m_pDY16->oLDCV3CCW1F = FALSE; m_pDY16->oLDCV3CW1F = FALSE;
			m_pDY16->oLDCV4CCW1F = FALSE; m_pDY16->oLDCV4CW1F = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);

			
		}
		break;
	case 4:
		if (m_pDX16->iLDVC1FStop && m_nLDMZElevatorCase == 0)
		{
			g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,4-1,Load MZ Elevator have no MZ");
			m_nBarRetry2 = 0;
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		} 
		else
		{
			g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,4-1,Load MZ Elevator doing something");
			m_nLD1FConveyorCase = 0; m_tLD1FConveyorLoop.Set_LoopTime(50000);
		}
		break;
	case 5:	//Barcode Read
			if (m_pEquipData->bUseMZIDLoad) 
			{
				g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,5-1,Barcode Trigger On");

				g_objBarcodeLot_Cognex.Set_Trigger(2, TRUE);	//1F
				m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
			} 
			else
			{
				g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,5-2,Barcode No use so MZ Count add");
				/*gLot.nMZCountGD++;
				sMZID.Format("MZID%02d-LOAD", gLot.nMZCountGD);*/

				int nTemp = g_objCommon.Get_Random(1, 9999);
				if(nTemp == nLDMZNum) nTemp++;
				sMZID.Format("MZID%d-LOAD", nTemp );
				nLDMZNum = nTemp;

				m_nLD1FConveyorCase = 7; m_tLD1FConveyorLoop.Set_LoopTime(5000);
			}
		break;
	case 6:	//Barcode Read End
			sMZID = g_objBarcodeLot_Cognex.Get_BarcodeLot(2);	//1F
			if (sMZID.GetLength() > 2) 
			{
				g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,6,Barcode Read End");
				g_objLogFile.Save_RFBarData(0, sMZID);
				m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);

			}

		break;
	case 7:
//		if (gData.bCycleStop == FALSE && gData.nCVJobSeq[0] == gData.nCVJobSeq[1] m_nMZTransferCase == 0) {

#ifndef AJIN_BOARD_USE
		m_pDX20->iMZTransExist = FALSE;
#endif				
		if (gData.bCycleStop == FALSE && m_nLDMZElevatorCase == 0 
			&& m_nMZTransferCase == 0 && !m_pDX15->iLDMZCarrierExist && !m_pDX20->iMZTransExist 
			&& (gData.nCVJobSeq[0] == 0 || gData.nCVJobSeq[1] > 0))
		{
			if (sMZID.GetLength() > 2) 
			{
				g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,7-1,empty");
				gData.sMZID[0] = sMZID;
				m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
			}
		}
		else 
		{
			g_objLogFile.Save_SeqLog("MCC,31,LD1FConveyor,7-2,empty");
			m_nLD1FConveyorCase = 0; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		break;

	case 8:	//MZ 투입 Check
		if (m_nLDMZElevatorCase == 0 && m_nMZTransferCase == 0 
			&& !m_pDX15->iLDMZCarrierExist && !m_pDX20->iMZTransExist 
			&& (gData.nCVJobSeq[0] == 0 || gData.nCVJobSeq[1] > 0))
		{
			if (gData.bCycleStop) 
			{
				m_nLD1FConveyorCase = 0; m_tLD1FConveyorLoop.Set_LoopTime(5000);
			} 
			else
			{
				if (m_nLDCVElevatorCase == 0 && m_nLDMZElevatorCase == 0 && !m_pDX18->iLDMZElevatorExist)
				{
					m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
				}
			}
		} 
		else 
		{
			m_nLD1FConveyorCase = 0; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		return TRUE;

	case 9:	//MES 요청(MZ투입)
		if (m_pEquipData->bUseMES) {
			if (gData.sMZID[0].GetLength() < 2) { m_nLD1FConveyorCase = 4; return TRUE; }
			gMes.nMZConfirm[0] = 1;
			g_objMesAgent.Set_MGZIDReport("L", gData.sMZID[0]);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(30000);
		} else {
			m_nLD1FConveyorCase = 11; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 10:	//MES 완료
		if (!m_pEquipData->bUseMES || gMes.nMZConfirm[0] == 2) {
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	//Elevator Wait
		if (m_nLDCVElevatorCase == 0) {
			m_nLDCVElevatorCase = 10; gData.nCVJobSeq[0]++;
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		return TRUE;
	case 12:
		if (m_nLDCVElevatorCase == 10) 
		{
			if (gLot.dwJobStart < 1) Set_JobTack(1);
			g_objCommon.Move_Position(AX_LDCV_ELEVATOR_Z, 1);	//1F
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(10000);
		}
		return TRUE;

	case 13:
		if (g_objCommon.Check_Position(AX_LDCV_ELEVATOR_Z, 1)) {
			m_pDY17->oLDCVElevatorStopperUp = TRUE; m_pDY17->oLDCVElevatorStopperDn = FALSE;
			m_pDY17->oLDCVElevatorAlignUp = FALSE;   m_pDY17->oLDCVElevatorAlignDn = TRUE;
			m_pDY17->oLDCVElevatorAlignIn = FALSE;   m_pDY17->oLDCVElevatorAlignOut = TRUE;
			g_objAJinAXL.Write_Output(17);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 14:
		if (m_pDX17->iLDCVElevatorStopperUp && !m_pDX17->iLDCVElevatorStopperDn) {
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 15:
		if (!m_pDX17->iLDCVElevatorAlignUp && m_pDX17->iLDCVElevatorAlignDn) {
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 16:
		if (!m_pDX17->iLDCVElevatorAlignIn && m_pDX17->iLDCVElevatorAlignOut) {
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 17:
			m_pDY16->oLDCVStopper1FUp = FALSE; m_pDY16->oLDCVStopper1FDn = TRUE;
			g_objAJinAXL.Write_Output(16);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		break;
	case 18:
		if (!m_pDX16->iLDCVStopper1FUp && m_pDX16->iLDCVStopper1FDn) {
			if (!m_tLD1FConveyorLoop.Waiting_Time(500)) break;
			m_pDY16->oLDCV3CCW1F = TRUE; m_pDY16->oLDCV3CW1F = TRUE;
			m_pDY16->oLDCV4CCW1F = TRUE; m_pDY16->oLDCV4CW1F = TRUE;
			g_objAJinAXL.Write_Output(16);
			m_pDY17->oLDCVElevatorCVCCW = TRUE; m_pDY17->oLDCVElevatorCVCW = TRUE;
			g_objAJinAXL.Write_Output(17);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(10000);
#ifndef AJIN_BOARD_USE
			m_pDX17->iLDCVElevatorCVStart = TRUE;
#endif
		}
		break;
	case 19:
		if (m_pDX17->iLDCVElevatorCVStart || m_pDX17->iLDCVElevatorCVStop) {
			m_pDY16->oLDCV3CCW1F = FALSE; m_pDY16->oLDCV3CW1F = FALSE;
			m_pDY16->oLDCV4CCW1F = FALSE; m_pDY16->oLDCV4CW1F = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_pDY17->oLDCVElevatorCVCCW = FALSE; m_pDY17->oLDCVElevatorCVCW = FALSE;
			g_objAJinAXL.Write_Output(17);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(10000);


		}
		break;
	case 20:
		if (m_pDX17->iLDCVElevatorCVStart || m_pDX17->iLDCVElevatorCVStop) {
			if (!m_tLD1FConveyorLoop.Waiting_Time(1000)) break;
			m_pDY16->oLDCV3CCW1F = FALSE; m_pDY16->oLDCV3CW1F = TRUE;
			m_pDY16->oLDCV4CCW1F = FALSE; m_pDY16->oLDCV4CW1F = TRUE;
			g_objAJinAXL.Write_Output(16);
			m_pDY17->oLDCVElevatorCVCCW = TRUE; m_pDY17->oLDCVElevatorCVCW = TRUE;
			g_objAJinAXL.Write_Output(17);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 21:
		if (m_pDX17->iLDCVElevatorCVStart || m_pDX17->iLDCVElevatorCVStop) {
			if (!m_tLD1FConveyorLoop.Waiting_Time(3000)) break;
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);

#ifndef AJIN_BOARD_USE
			m_pDX16->iLDVC1FStop = FALSE;
#endif
			
		}
		break;
	case 22:
		if (!m_pDX16->iLDVC1FStop && !m_pDX17->iLDCVElevatorAlarm) 
		{
			if (!m_tLD1FConveyorLoop.Waiting_Time(1000)) break;
			m_pDY16->oLDCV3CCW1F = FALSE; m_pDY16->oLDCV3CW1F = FALSE;
			m_pDY16->oLDCV4CCW1F = FALSE; m_pDY16->oLDCV4CW1F = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);

#ifndef AJIN_BOARD_USE
			m_pDX17->iLDCVElevatorCVStop = TRUE; m_pDX17->iLDCVElevatorCVStart = FALSE;
#endif

		}
		break;
	case 23:
		if (m_pDX17->iLDCVElevatorCVStop) {
			m_pDY17->oLDCVElevatorCVCCW = FALSE; m_pDY17->oLDCVElevatorCVCW = FALSE;
			g_objAJinAXL.Write_Output(17);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 24:
		if (m_pDX17->iLDCVElevatorCVStop) {
			gData.sMZID[2] = gData.sMZID[0]; gData.sMZID[0] = "";
			m_nLDCVElevatorCase = 11;	gData.nCVElevatorFrom = 1;

			m_pDY16->oLDCVStopper1FUp = TRUE; m_pDY16->oLDCVStopper1FDn = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 25:
			m_pDY16->oLDCVStopper1FUp = TRUE; m_pDY16->oLDCVStopper1FDn = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);


		break;
	case 26:
		if (m_pDX16->iLDCVStopper1FUp && !m_pDX16->iLDCVStopper1FDn)
		{
			m_pDY16->oLDCV3CCW1F = TRUE; m_pDY16->oLDCV3CW1F = TRUE;
			m_pDY16->oLDCV4CCW1F = TRUE; m_pDY16->oLDCV4CW1F = TRUE;
			g_objAJinAXL.Write_Output(16);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);

#ifndef AJIN_BOARD_USE
			if(gData.nSimMzCntLoaded == 0)
			{
				m_pDX16->iLDVC1FStop = TRUE; m_pDX16->iLDCV1FCnt1 = FALSE;
				gData.nSimMzCntLoaded++;
			}
			else if(gData.nSimMzCntLoaded == 1)
			{
				m_pDX16->iLDVC1FStop = TRUE; m_pDX16->iLDCV1FCnt2 = FALSE;
				gData.nSimMzCntLoaded++;
			}
			else if(gData.nSimMzCntLoaded == 2)
			{
				m_pDX16->iLDVC1FStop = TRUE; m_pDX16->iLDCV1FCnt3 = FALSE;
				gData.nSimMzCntLoaded++;
			}			
#endif
		}
		break;
	case 27:
		if (m_pDX16->iLDVC1FStop) {
			if (!m_tLD1FConveyorLoop.Waiting_Time(1000)) break;   
			m_pDY16->oLDCV3CCW1F = FALSE; m_pDY16->oLDCV3CW1F = FALSE;
			m_pDY16->oLDCV4CCW1F = FALSE; m_pDY16->oLDCV4CW1F = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 28:
		if (m_pDX16->iLDCVStopper1FUp && !m_pDX16->iLDCVStopper1FDn) {
			m_pDY16->oLDCV3CCW1F = FALSE; m_pDY16->oLDCV3CW1F = FALSE;
			m_pDY16->oLDCV4CCW1F = FALSE; m_pDY16->oLDCV4CW1F = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_nLD1FConveyorCase = 0; m_tLD1FConveyorLoop.Set_LoopTime(50000);
		}
		break;

	case 31:
		nMZExit = 1;
		m_nLD1FConveyorCase = 1; m_tLD1FConveyorLoop.Set_LoopTime(5000);
		return TRUE;
	}

	// 31. (Error : 2800)
	if (m_tLD1FConveyorLoop.Over_LoopTime()) {
		if (m_nLD1FConveyorCase == 0) { m_tLD1FConveyorLoop.Set_LoopTime(30000); return TRUE; }
		if (m_nLD1FConveyorCase == 1) { m_nLD1FConveyorCase = 0; return TRUE; }
		if (m_nLD1FConveyorCase == 6 || m_nLD1FConveyorCase == 7) {
			m_nBarRetry2++; m_nLD1FConveyorCase = 5;
			if (m_nBarRetry2 < 3) {
				g_objBarcodeLot_Cognex.Set_Trigger(2, FALSE); return TRUE;
			}
		}
		if (m_nLD1FConveyorCase == 27) { m_nLD1FConveyorCase++; m_tLD1FConveyorLoop.Set_LoopTime(5000); return TRUE; }
		g_objCommon.Show_Error(2800 + m_nLD1FConveyorCase);
		return FALSE;
	}
	return TRUE;
}

// 32. (Error : 2900)
BOOL CSequenceMain::Run_LD2FConveyor()
{
	static int nETMZExit = 0;
	static CString sETMZID;

	switch (m_nLD2FConveyorCase) {
	case 0:	// Wait
#ifndef AJIN_BOARD_USE
		m_pDX16->iLDCV2FCnt1 = TRUE;
#endif
		nETMZExit = Check_EmptyMZ();
		if (nETMZExit > 0) {
			if (!m_tLD2FConveyorLoop.Waiting_Time(3000)) break;
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		} else {
			 if (gData.nMZDoorOpen[1] == 0 && m_nNGMZElevatorCase == 0 && m_nMZTransferCase == 0 && m_nLDCVElevatorCase == 0 && (m_nNGStage1Case == 60 || m_nNGStage2Case == 60)) {
				g_objCommon.Show_Error(2940);
				return FALSE;
			}			
		}
		return TRUE;

	case 1:
		if (nETMZExit > 0) 
		{
			m_nBarRetry1 = 0;
			m_pDY16->oLDCVStopper2FUp = TRUE; m_pDY16->oLDCVStopper2FDn = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		 }
		break;
	case 2:
		if (m_pDX16->iLDCVStopper2FUp && !m_pDX16->iLDCVStopper2FDn) {
			if (m_pDX16->iLDVC2FStop) m_nLD2FConveyorCase = 5;
			else {
				m_pDY16->oLDCV1CCW2F = TRUE; m_pDY16->oLDCV1CW2F = TRUE;
				m_pDY16->oLDCV2CCW2F = TRUE; m_pDY16->oLDCV2CW2F = TRUE;
				g_objAJinAXL.Write_Output(16);
				m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(15000);
			}
		}
		break;
	case 3:
		if (m_pDX16->iLDVC2FStop) {
//			if (!m_tLD2FConveyorLoop.Waiting_Time(1000)) break;   
			m_pDY16->oLDCV1CCW2F = FALSE; m_pDY16->oLDCV1CW2F = FALSE;
			m_pDY16->oLDCV2CCW2F = FALSE; m_pDY16->oLDCV2CW2F = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(10000);
		}
		break;
	case 4:
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		break;

	case 5:	//MZ 투입 Check
		if (gData.nCVJobSeq[0] > 0 && gData.nCVJobSeq[1] == 0) {
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		} else if (m_nNGMZElevatorCase == 0 && m_nMZTransferCase == 0 && (m_nNGStage1Case == 60 || m_nNGStage2Case == 60)) {
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		} else if (m_nNGMZElevatorCase == 0  && m_nMZTransferCase == 0 && gData.nCVJobSeq[1] > 0 && Check_EmptyUnloadPicker() == FALSE) {
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		} else {
			m_nLD2FConveyorCase = 0; m_tLD2FConveyorLoop.Set_LoopTime(50000);
		}
		return TRUE;

	case 6:	//Barcode Read
		if (m_pEquipData->bUseMZIDNG) {
			g_objBarcodeLot_Cognex.Set_Trigger(1, TRUE);	//2F
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		} else {
			gLot.nMZCountNG++;
			sETMZID.Format("MZID%02d-NG", gLot.nMZCountNG);
			m_nLD2FConveyorCase = 8; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 7:	//Barcode Read End
			sETMZID = g_objBarcodeLot_Cognex.Get_BarcodeLot(1);	//2F
			if (sETMZID.GetLength() > 0) {
				g_objLogFile.Save_RFBarData(1, sETMZID);
				m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
			}
		break;
	case 8:
			gData.sMZID[1] = sETMZID;
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		break;
	case 9:
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		break;
	case 10:
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		return TRUE;

	case 11:	//Elevator Wati
		if (m_nLDCVElevatorCase == 0) {
			m_nLDCVElevatorCase = 10;	gData.nCVJobSeq[1]++;
			g_objCommon.Move_Position(AX_LDCV_ELEVATOR_Z, 0);	//2F
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(30000);	//10000
		}
		return TRUE;

	case 12:
		if (g_objCommon.Check_Position(AX_LDCV_ELEVATOR_Z, 0)) {
			m_pDY17->oLDCVElevatorStopperUp = TRUE; m_pDY17->oLDCVElevatorStopperDn = FALSE;
			m_pDY17->oLDCVElevatorAlignUp = FALSE;   m_pDY17->oLDCVElevatorAlignDn = TRUE;
			m_pDY17->oLDCVElevatorAlignIn = FALSE;   m_pDY17->oLDCVElevatorAlignOut = TRUE;
			g_objAJinAXL.Write_Output(17);
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 13:
		if (m_pDX17->iLDCVElevatorStopperUp && !m_pDX17->iLDCVElevatorStopperDn) {
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 14:
		if (!m_pDX17->iLDCVElevatorAlignUp && m_pDX17->iLDCVElevatorAlignDn) {
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 15:
		if (!m_pDX17->iLDCVElevatorAlignIn && m_pDX17->iLDCVElevatorAlignOut) {
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 16:
			m_pDY16->oLDCVStopper2FUp = FALSE; m_pDY16->oLDCVStopper2FDn = TRUE;
			g_objAJinAXL.Write_Output(16);
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		break;
	case 17:
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		break;
	case 18:
		if (!m_pDX16->iLDCVStopper2FUp && m_pDX16->iLDCVStopper2FDn) {
			if (!m_tLD2FConveyorLoop.Waiting_Time(500)) break;
			m_pDY16->oLDCV1CCW2F = TRUE; m_pDY16->oLDCV1CW2F = TRUE;
			m_pDY16->oLDCV2CCW2F = TRUE; m_pDY16->oLDCV2CW2F = TRUE;
			g_objAJinAXL.Write_Output(16);
			m_pDY17->oLDCVElevatorCVCCW = TRUE; m_pDY17->oLDCVElevatorCVCW = TRUE;
			g_objAJinAXL.Write_Output(17);
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(10000);
		}
		break;
	case 19:
#ifndef AJIN_BOARD_USE
		m_pDX17->iLDCVElevatorCVStart = TRUE; 
#endif
		if (m_pDX17->iLDCVElevatorCVStart || m_pDX17->iLDCVElevatorCVStop) {
			m_pDY16->oLDCV1CCW2F = FALSE; m_pDY16->oLDCV1CW2F = FALSE;
			m_pDY16->oLDCV2CCW2F = FALSE; m_pDY16->oLDCV2CW2F = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_pDY17->oLDCVElevatorCVCCW = FALSE; m_pDY17->oLDCVElevatorCVCW = FALSE;
			g_objAJinAXL.Write_Output(17);
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(10000);
		}
		break;
	case 20:
#ifndef AJIN_BOARD_USE
		m_pDX17->iLDCVElevatorCVStart = TRUE; 
#endif
		if (m_pDX17->iLDCVElevatorCVStart || m_pDX17->iLDCVElevatorCVStop) {
			if (!m_tLD2FConveyorLoop.Waiting_Time(1000)) break;
			m_pDY16->oLDCV1CCW2F = FALSE; m_pDY16->oLDCV1CW2F = TRUE;
			m_pDY16->oLDCV2CCW2F = FALSE; m_pDY16->oLDCV2CW2F = TRUE;
			g_objAJinAXL.Write_Output(16);
			m_pDY17->oLDCVElevatorCVCCW = TRUE; m_pDY17->oLDCVElevatorCVCW = TRUE;
			g_objAJinAXL.Write_Output(17);
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 21:
		if (m_pDX17->iLDCVElevatorCVStart || m_pDX17->iLDCVElevatorCVStop) {
			if (!m_tLD2FConveyorLoop.Waiting_Time(3000)) break;
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 22:
		if (!m_pDX16->iLDVC2FStop && !m_pDX17->iLDCVElevatorAlarm) {
			if (!m_tLD2FConveyorLoop.Waiting_Time(1000)) break;
			m_pDY16->oLDCV1CCW2F = FALSE; m_pDY16->oLDCV1CW2F = FALSE;
			m_pDY16->oLDCV2CCW2F = FALSE; m_pDY16->oLDCV2CW2F = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 23:
#ifndef AJIN_BOARD_USE
		m_pDX17->iLDCVElevatorCVStop = TRUE;
#endif
		if (m_pDX17->iLDCVElevatorCVStop) {
			m_pDY17->oLDCVElevatorCVCCW = FALSE; m_pDY17->oLDCVElevatorCVCW = FALSE;
			g_objAJinAXL.Write_Output(17);
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 24:
#ifndef AJIN_BOARD_USE
		m_pDX17->iLDCVElevatorCVStop = TRUE;
#endif
		if (m_pDX17->iLDCVElevatorCVStop) {
			gData.sMZID[2] = gData.sMZID[1]; gData.sMZID[1] = "";
			m_nLDCVElevatorCase = 11;	gData.nCVElevatorFrom = 2;

			m_pDY16->oLDCVStopper2FUp = TRUE; m_pDY16->oLDCVStopper2FDn = FALSE;
			g_objAJinAXL.Write_Output(16);
			m_nLD2FConveyorCase++; m_tLD2FConveyorLoop.Set_LoopTime(5000);
		}
		break;
	case 25:
		if (m_pDX16->iLDCVStopper2FUp && !m_pDX16->iLDCVStopper2FDn) {
			m_nLD2FConveyorCase = 0; m_tLD2FConveyorLoop.Set_LoopTime(50000);
		}
		break;

	}

	// 32. (Error : 2900)
	if (m_tLD2FConveyorLoop.Over_LoopTime()) {
		if (m_nLD2FConveyorCase == 0 || m_nLD2FConveyorCase == 5) { m_tLD2FConveyorLoop.Set_LoopTime(30000); return TRUE; }
		if (m_nLD2FConveyorCase == 7) {
			m_nBarRetry1++; m_nLD2FConveyorCase = 6;
			if (m_nBarRetry1 < 3) {
				g_objBarcodeLot_Cognex.Set_Trigger(1, FALSE); return TRUE;
			}
		}
		g_objCommon.Show_Error(2900 + m_nLD2FConveyorCase);
		return FALSE;
	}
	return TRUE;
}

// 33. (Error : 2950)
BOOL CSequenceMain::Run_UnloadConveyor()
{
	switch (m_nUnloadConveyorCase) {
	case 0:	// Wait
		m_tUnloadConveyorLoop.Set_LoopTime(10000);
		return TRUE;

	case 1:	//1층구동
			m_pDY19->oULCV3CCW1F = TRUE;   m_pDY19->oULCV3CW1F = TRUE;
			m_pDY19->oULCV4CCW1F = TRUE;   m_pDY19->oULCV4CW1F = TRUE;
			g_objAJinAXL.Write_Output(19);
			m_nUnloadConveyorCase++; m_tUnloadConveyorLoop.Set_LoopTime(20000);
		break;
	case 2:
		if (m_pDY19->oULCV3CCW1F) {
			if (!m_tUnloadConveyorLoop.Waiting_Time(m_pEquipData->nConveyorTime)) break;
			m_nUnloadConveyorCase++; m_tUnloadConveyorLoop.Set_LoopTime(10000);
		}
		break;
	case 3:
			m_pDY19->oULCV3CCW1F = FALSE;   m_pDY19->oULCV3CW1F = FALSE;
			m_pDY19->oULCV4CCW1F = FALSE;   m_pDY19->oULCV4CW1F = FALSE;
			g_objAJinAXL.Write_Output(19);
			m_nUnloadConveyorCase = 0; m_tUnloadConveyorLoop.Set_LoopTime(10000);
		break;


	case 11:	//2층구동
			m_pDY19->oULCV1CCW2F = TRUE;   m_pDY19->oULCV1CW2F = TRUE;
			m_pDY19->oULCV2CCW2F = TRUE;   m_pDY19->oULCV2CW2F = TRUE;
			g_objAJinAXL.Write_Output(19);
			m_nUnloadConveyorCase++; m_tUnloadConveyorLoop.Set_LoopTime(20000);
		break;
	case 12:
		if (m_pDY19->oULCV1CCW2F) {
			if (!m_tUnloadConveyorLoop.Waiting_Time(m_pEquipData->nConveyorTime)) break;
			m_nUnloadConveyorCase++; m_tUnloadConveyorLoop.Set_LoopTime(10000);
		}
		break;
	case 13:
			m_pDY19->oULCV1CCW2F = FALSE;   m_pDY19->oULCV1CW2F = FALSE;
			m_pDY19->oULCV2CCW2F = FALSE;   m_pDY19->oULCV2CW2F = FALSE;
			g_objAJinAXL.Write_Output(19);
			m_nUnloadConveyorCase = 0; m_tUnloadConveyorLoop.Set_LoopTime(10000);
		break;

	}

	// 33. (Error : 2950)
	if (m_tUnloadConveyorLoop.Over_LoopTime()) {
		if (m_nUnloadConveyorCase ==  2) m_nUnloadConveyorCase--;
		if (m_nUnloadConveyorCase == 12) m_nUnloadConveyorCase--;
		m_tUnloadConveyorLoop.Set_LoopTime(20000);
		return TRUE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSequenceMain::Run_Simulation()
{
#ifdef AJIN_BOARD_USE
	return TRUE;
#endif
	static int nLotMZCount = 0;
	static int nRen, nCarCount, nMZCarCount;
	m_pDX00->iElevator2TrayExist = TRUE;
	if (m_nElevator2Case == 10) { Sleep(SIM_WAITTIMES); m_pDX00->iElevator2Height1 = TRUE; m_pDX00->iElevator2Height2 = TRUE; }
	if (m_nElevator2Case == 12) { Sleep(SIM_WAITTIMES); m_pDX00->iElevator2Height1 = FALSE; m_pDX00->iElevator2Height2 = FALSE; }
	if (m_nElevator2Case == 14) { Sleep(SIM_WAITTIMES); m_pDX00->iElevator2Height1 = TRUE; m_pDX00->iElevator2Height2 = TRUE; }

	if (m_nTransfer1Case == 14) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferLTrayExist = TRUE; }
	if (m_nTransfer1Case == 26) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferLTrayExist = TRUE; }
	if (m_nTransfer1Case == 36) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferLTrayExist = TRUE; }
	if (m_nTransfer1Case == 47) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferLTrayExist = FALSE; }
	if (m_nTransfer1Case == 47) { if (m_nLoadStage1Case == 0) m_pDX04->iLoadStage1TrayExist = TRUE; }
	if (m_nTransfer1Case == 47) { if (m_nLoadStage2Case == 0) m_pDX04->iLoadStage2TrayExist = TRUE; }
	if (m_nTransfer1Case == 58) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferLTrayExist = FALSE; }
	if (m_nTransfer1Case == 68) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferLTrayExist = FALSE; }

	if (m_nTransfer2Case == 15) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferRTrayExist = TRUE; }
	if (m_nTransfer2Case == 25) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferRTrayExist = TRUE; }
	if (m_nTransfer2Case == 26) { Sleep(SIM_WAITTIMES); m_pDX15->iNGRailCarrierChk1 = FALSE; m_pDX15->iNGRailCarrierChk2 = FALSE; }
	if (m_nTransfer2Case == 35) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferRTrayExist = TRUE; }
	if (m_nTransfer2Case == 36) { Sleep(SIM_WAITTIMES); m_pDX02->iElevator5TrayExist = FALSE; }
	if (m_nTransfer2Case == 45) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferRTrayExist = TRUE; }
	if (m_nTransfer2Case == 55) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferRTrayExist = TRUE; }
	if (m_nTransfer2Case == 73) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferRTrayExist = FALSE; }
	if (m_nTransfer2Case == 74) { Sleep(SIM_WAITTIMES); m_pDX02->iElevator5TrayExist = TRUE; }
	if (m_nTransfer2Case == 79) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferRTrayExist = FALSE; }
	if (m_nTransfer2Case == 84) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferRTrayExist = FALSE; }
	if (m_nTransfer2Case == 89) { Sleep(SIM_WAITTIMES); m_pDX03->iTransferRTrayExist = FALSE;
								if ((gData.nTransferX2Pos-11)==1) m_pDX12->iGoodStage1TrayExist = TRUE;
								if ((gData.nTransferX2Pos-11)==2) m_pDX12->iGoodStage2TrayExist = TRUE; }

	if (m_nLoadStage1Case ==  1) { Sleep(SIM_WAITTIMES); m_pDX04->iLoadStage1TrayExist = TRUE; }
	if (m_nLoadStage1Case == 61) { Sleep(SIM_WAITTIMES); m_pDX04->iLoadStage1TrayExist = FALSE; }
	if (m_nLoadStage2Case ==  1) { Sleep(SIM_WAITTIMES); m_pDX04->iLoadStage2TrayExist = TRUE; }
	if (m_nLoadStage2Case == 61) { Sleep(SIM_WAITTIMES); m_pDX04->iLoadStage2TrayExist = FALSE; }
	if (m_nLoadStage1Case == 26) { Sleep(SIM_WAITTIMES); m_pDX04->iLoadStage1SlaveIn = FALSE; m_pDX04->iLoadStage1SlaveOut = TRUE; }
	if (m_nLoadStage1Case == 27) { Sleep(SIM_WAITTIMES); m_pDX04->iLoadStage1MasterIn = FALSE; m_pDX04->iLoadStage1MasterOut = TRUE; }
	if (m_nLoadStage1Case == 28) { Sleep(SIM_WAITTIMES); m_pDX04->iLoadStage1TrayExist = FALSE; }
	if (m_nLoadStage2Case == 26) { Sleep(SIM_WAITTIMES); m_pDX04->iLoadStage2SlaveIn = FALSE; m_pDX04->iLoadStage2SlaveOut = TRUE; }
	if (m_nLoadStage2Case == 27) { Sleep(SIM_WAITTIMES); m_pDX04->iLoadStage2MasterIn = FALSE; m_pDX04->iLoadStage2MasterOut = TRUE; }
	if (m_nLoadStage2Case == 28) { Sleep(SIM_WAITTIMES); m_pDX04->iLoadStage2TrayExist = FALSE; }

	if (m_nNGStage1Case ==  1) { Sleep(SIM_WAITTIMES); m_pDX11->iNGStage1TrayExist = TRUE; }
	if (m_nNGStage1Case == 54 && gData.nPortNo_NGTray[0] > 0) { Sleep(SIM_WAITTIMES); m_pDX11->iNGStage1TrayExist = TRUE; }
	if (m_nNGStage1Case == 61) { Sleep(SIM_WAITTIMES); m_pDX11->iNGStage1TrayExist = FALSE; }
	if (m_nNGStage2Case ==  1) { Sleep(SIM_WAITTIMES); m_pDX11->iNGStage2TrayExist = TRUE; }
	if (m_nNGStage2Case == 54 && gData.nPortNo_NGTray[1] > 0) { Sleep(SIM_WAITTIMES); m_pDX11->iNGStage2TrayExist = TRUE; }
	if (m_nNGStage2Case == 61) { Sleep(SIM_WAITTIMES); m_pDX11->iNGStage2TrayExist = FALSE; }

	if (m_nGoodStage1Case ==  1) { Sleep(SIM_WAITTIMES); m_pDX12->iGoodStage1TrayExist = TRUE; }
	if (m_nGoodStage1Case == 54 && gData.nPortNo_GoodTray[0] > 0) { Sleep(SIM_WAITTIMES); m_pDX12->iGoodStage1TrayExist = TRUE; }
	if (m_nGoodStage1Case == 61) { Sleep(SIM_WAITTIMES); m_pDX12->iGoodStage1TrayExist = FALSE; }
	if (m_nGoodStage2Case ==  1) { Sleep(SIM_WAITTIMES); m_pDX12->iGoodStage2TrayExist = TRUE; }
	if (m_nGoodStage2Case == 54 && gData.nPortNo_GoodTray[1] > 0) { Sleep(SIM_WAITTIMES); m_pDX12->iGoodStage2TrayExist = TRUE; }
	if (m_nGoodStage2Case == 61) { Sleep(SIM_WAITTIMES); m_pDX12->iGoodStage2TrayExist = FALSE; }

	if (m_nLoadPicker1Case == 19) m_nLoadPicker1Case++;
	if (m_nLoadPicker2Case == 19) m_nLoadPicker2Case++;

	if (m_nVisionStage1Case ==  8 || m_nVisionStage2Case ==  8 || m_nVisionStage3Case ==  8 || m_nVisionStage4Case ==  8) gData.bTop1ScanDone = TRUE;
	if (m_nVisionStage1Case == 12 || m_nVisionStage2Case == 12 || m_nVisionStage3Case == 12 || m_nVisionStage4Case == 12) gData.bTop1ScanDone = TRUE;
	if (m_nVisionStage1Case == 18 || m_nVisionStage2Case == 18 || m_nVisionStage3Case == 18 || m_nVisionStage4Case == 18) gData.bTop2ScanDone = TRUE;
	
	
	if (m_nLD2FConveyorCase == 3) { Sleep(SIM_WAITTIMES); m_pDX16->iLDVC2FStop = TRUE; }
	if (m_nLD2FConveyorCase == 19) { Sleep(SIM_WAITTIMES); m_pDX17->iLDCVElevatorCVStart = TRUE; }
	if (m_nLD2FConveyorCase == 21) { Sleep(SIM_WAITTIMES); m_pDX17->iLDCVElevatorCVStart = TRUE; m_pDX16->iLDVC2FStop = FALSE; }
	if (m_nLD2FConveyorCase == 22) { Sleep(SIM_WAITTIMES); m_pDX17->iLDCVElevatorCVStop = TRUE; m_pDX17->iLDCVElevatorCVStart = FALSE; }

	if (m_nULCVElevatorCase == 11) { Sleep(SIM_WAITTIMES); m_pDX17->iULCVElevatorMZExist = TRUE; }
	if (m_nULCVElevatorCase == 13) { Sleep(SIM_WAITTIMES); m_pDX19->iULCV2FStop = TRUE; }
	if (m_nULCVElevatorCase == 14) { Sleep(SIM_WAITTIMES); m_pDX17->iULCVElevatorMZExist = FALSE; m_pDX17->iULCVElevatorStop = FALSE; m_pDX19->iULCV2FStop = FALSE; }
	if (m_nULCVElevatorCase == 21) { Sleep(SIM_WAITTIMES); m_pDX17->iULCVElevatorMZExist = TRUE; }
	if (m_nULCVElevatorCase == 23) { Sleep(SIM_WAITTIMES); m_pDX19->iULCV1FStop = TRUE; }
	if (m_nULCVElevatorCase == 24) { Sleep(SIM_WAITTIMES); m_pDX17->iULCVElevatorMZExist = FALSE; m_pDX17->iULCVElevatorStop = FALSE; m_pDX19->iULCV1FStop = FALSE; }
	if (m_nULCVElevatorCase == 31) { Sleep(SIM_WAITTIMES); m_pDX19->iULCV1FCnt1 = TRUE; }
	if (m_nULCVElevatorCase == 32) { Sleep(SIM_WAITTIMES); m_pDX19->iULCV1FCnt1 = FALSE; }

	if (m_nMZTransferCase == 23) { Sleep(SIM_WAITTIMES); m_pDX20->iMZTransExist = TRUE; }
	if (m_nMZTransferCase == 24) { Sleep(SIM_WAITTIMES); m_pDX18->iLDMZElevatorExist = FALSE; }
	if (m_nMZTransferCase == 25) { Sleep(SIM_WAITTIMES); m_pDX18->iMZBufferExist = FALSE; }
	if (m_nMZTransferCase == 26) { Sleep(SIM_WAITTIMES); m_pDX18->iNGMZElevatorExist = FALSE; }
	if (m_nMZTransferCase == 27) { Sleep(SIM_WAITTIMES); m_pDX18->iGDMZElevatorExist = FALSE; }
	if (m_nMZTransferCase == 44) { Sleep(SIM_WAITTIMES); m_pDX18->iLDMZElevatorExist = TRUE; }
	if (m_nMZTransferCase == 45) { Sleep(SIM_WAITTIMES); m_pDX18->iMZBufferExist = TRUE; }
	if (m_nMZTransferCase == 46) { Sleep(SIM_WAITTIMES); m_pDX18->iNGMZElevatorExist = TRUE; }
	if (m_nMZTransferCase == 47) { Sleep(SIM_WAITTIMES); m_pDX18->iGDMZElevatorExist = TRUE; }

	if (m_pEquipData->bUseMES) 
	{
		if (m_nLDMZElevatorCase == 9) {	//MES-Test시 Carrrier수량설정
			gLot.nCarrierExist[0][0] = 1; //gLot.nCarrierExist[0][1] = 1;
			gData.nSimCount++;
		}
	} 
	else
	{
		if (m_nLDMZElevatorCase == 9) {
			int nS, nE, nX;
			if (gData.nSimCount == 0 || gData.nSimCount == 4) { nS =  0; nE =  8; }
			if (gData.nSimCount == 1 || gData.nSimCount == 5) { nS =  8; nE = 16; }
			if (gData.nSimCount == 2 || gData.nSimCount == 6) { nS = 16; nE = 24; }
			if (gData.nSimCount == 3 || gData.nSimCount == 7) { nS = 24; nE = 28; }
			if (gData.nSimCount > 7) 
			{
				gData.nSimCount = 0;
				nS =  0; nE =  8;
			}
			nX = 0;
			for(int i=nS; i<nE; i++) {
				if (gLot.nCmCount[i] > 0) gLot.nCarrierExist[0][nX] = 1;
				else					  gLot.nCarrierExist[0][nX] = 0;
				nX++;
			}
			gData.nSimCount++;
		}
	}

	if (m_nLDMZElevatorCase ==  0) { Sleep(SIM_WAITTIMES); m_pDX18->iLDMZElevatorExist = FALSE; }
	if (m_nLDMZElevatorCase ==  1) { Sleep(SIM_WAITTIMES); m_pDX18->iLDMZElevatorExist = TRUE; }
	if (m_nLDMZElevatorCase == 16) { Sleep(SIM_WAITTIMES); m_pDX15->iLDGripCarrierChk = TRUE; }
	if (m_nLDMZElevatorCase == 19) { Sleep(SIM_WAITTIMES); m_pDX15->iLDRailCarrierChk1 = FALSE; m_pDX15->iLDRailCarrierChk2 = TRUE; }
	if (m_nLDMZElevatorCase == 31) { Sleep(SIM_WAITTIMES); m_pDX15->iLDRailCarrierChk1 = FALSE; m_pDX15->iLDRailCarrierChk2 = FALSE; }
	if (m_nLDMZElevatorCase == 40) { Sleep(SIM_WAITTIMES); m_pDX15->iLDGripCarrierChk = FALSE; }
	if (m_nLDMZElevatorCase == 51) { Sleep(SIM_WAITTIMES); m_pDX18->iLDMZElevatorExist = FALSE; }

	if (m_nNGMZElevatorCase ==  0) { Sleep(SIM_WAITTIMES); m_pDX18->iNGMZElevatorExist = FALSE; }
	if (m_nNGMZElevatorCase ==  1) { Sleep(SIM_WAITTIMES); m_pDX18->iNGMZElevatorExist = TRUE; }
	if (m_nNGMZElevatorCase ==  9) { for(int i=0; i<8; i++) gLot.nCarrierExist[1][i] = 1; }
	if (m_nNGMZElevatorCase == 16) { Sleep(SIM_WAITTIMES); m_pDX15->iNGGripCarrierChk = TRUE; }
	if (m_nNGMZElevatorCase == 19) { Sleep(SIM_WAITTIMES); m_pDX15->iNGRailCarrierChk1 = FALSE; m_pDX15->iNGRailCarrierChk2 = TRUE; }
	if (m_nNGMZElevatorCase == 24) { Sleep(SIM_WAITTIMES); m_pDX15->iNGRailCarrierChk1 = FALSE; m_pDX15->iNGRailCarrierChk2 = TRUE; }
	if (m_nNGMZElevatorCase == 28) { Sleep(SIM_WAITTIMES); m_pDX15->iNGRailCarrierChk1 = FALSE; m_pDX15->iNGRailCarrierChk2 = FALSE; }
	if (m_nNGMZElevatorCase == 31) { Sleep(SIM_WAITTIMES); m_pDX15->iNGRailCarrierChk1 = FALSE; m_pDX15->iNGRailCarrierChk2 = FALSE; }
	if (m_nNGMZElevatorCase == 51) { Sleep(SIM_WAITTIMES); m_pDX18->iNGMZElevatorExist = FALSE; }
	if (m_nNGMZElevatorCase == 71) { Sleep(SIM_WAITTIMES); m_pDX15->iNGRailCarrierChk1 = FALSE; m_pDX15->iNGRailCarrierChk2 = TRUE; }
	if (m_nNGMZElevatorCase == 75) { Sleep(SIM_WAITTIMES); m_pDX15->iNGRailCarrierChk1 = FALSE; m_pDX15->iNGRailCarrierChk2 = FALSE; }

	if (m_nGDMZElevatorCase ==  0) { Sleep(SIM_WAITTIMES); m_pDX18->iGDMZElevatorExist = FALSE; }
	if (m_nGDMZElevatorCase ==  1) { Sleep(SIM_WAITTIMES); m_pDX18->iGDMZElevatorExist = TRUE; }
	if (m_nGDMZElevatorCase == 51) { Sleep(SIM_WAITTIMES); m_pDX18->iGDMZElevatorExist = FALSE; }
	if (m_nGDMZElevatorCase == 71) { Sleep(SIM_WAITTIMES); m_pDX15->iGDRailCarrierChk1 = FALSE; m_pDX15->iGDRailCarrierChk2 = TRUE; }
	if (m_nGDMZElevatorCase == 75) { Sleep(SIM_WAITTIMES); m_pDX15->iGDRailCarrierChk1 = FALSE; m_pDX15->iGDRailCarrierChk2 = FALSE; }

	return TRUE;
}