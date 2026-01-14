// MesAgent.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Inspector.h"
#include "LogFile.h"
#include "Common.h"
#include "SequenceMain.h"
//#include "WorkDlg.h"
//#include "OperatorDlg.h"
#include "MesAgent.h"

#define	MES_AGENT_IP	"127.0.0.1"
#define MES_AGENT_PORT	10000		// MesAgent Handler Port

IMPLEMENT_DYNAMIC(CMesAgent, CWnd)

CMesAgent g_objMesAgent;

// CMesAgent

CMesAgent::CMesAgent()
{
	m_bConnected = FALSE;
	m_bHostOnline = FALSE;
	m_strRecvCmd = "";
	m_sLotID = "";
	for(int i=0; i<100; i++) m_sSizeNG[i] = "";
}

CMesAgent::~CMesAgent()
{
}

BEGIN_MESSAGE_MAP(CMesAgent, CWnd)
	ON_WM_TIMER()
	ON_MESSAGE(UM_CLIENT_CONNECT, OnClientConnect)
	ON_MESSAGE(UM_CLIENT_RECEIVE, OnClientReceive)
	ON_MESSAGE(UM_CLIENT_CLOSE, OnClientClose)
END_MESSAGE_MAP()

// CMesAgent 메시지 처리기입니다.

void CMesAgent::Initialize()
{
	if (m_bConnected) return;

	m_Client.Open_Socket(MES_AGENT_IP, MES_AGENT_PORT, this);	Sleep(3000);

	CString sLog, sKey;
	CIniFileCS INI(gsCurrentDir + "\\System\\SizeNGOrder.ini");
	if (!INI.Check_File()) {
		sLog.Format("MesAgent-Initialize => SizeNGOrder.ini File Not Found!!!!");
		g_objLogFile.Save_MesAgentLog(sLog);
		AfxMessageBox(sLog);
		return;
	}

	m_nSizeCount = INI.Get_Integer("SIZENG", "COUNT", 0);
	if (m_nSizeCount > 100) m_nSizeCount = 100;
	for(int i=0; i<m_nSizeCount; i++) {
		sKey.Format("%02d", i+1);
		m_sSizeNG[i] = INI.Get_String("SIZENG", sKey, "");
	}		

	sLog.Format("MesAgent Initialize. SizeNG Count=%d", m_nSizeCount);
	g_objLogFile.Save_MesAgentLog(sLog);
}

void CMesAgent::Terminate()
{
	m_bConnected = FALSE;
	m_bHostOnline = FALSE;
	m_Client.Close_Socket();

	g_objLogFile.Save_MesAgentLog("MesAgent Terminate.");	Sleep(500);
}

/////////////////////////////////////////////////////////////////////////////

LRESULT CMesAgent::OnClientConnect(WPARAM wConnect, LPARAM lParam)
{
	m_bConnected = (BOOL)wConnect;
	if (!m_bConnected) return 0;

	Set_OperUpdate(gData.sOperID);
	Set_EquipState(2);	//Idle
	g_objLogFile.Save_MesAgentLog("MesAgent Connected");
	return 0;
}

LRESULT CMesAgent::OnClientClose(WPARAM wParam, LPARAM lParam)
{
	m_bConnected = FALSE;
	m_bHostOnline = FALSE;
	m_Client.Close_Socket();
	g_objLogFile.Save_MesAgentLog("MesAgent Disconnected");
	return 0;
}

LRESULT CMesAgent::OnClientReceive(WPARAM wParam, LPARAM lParam)
{
	BYTE byRecv[1025] = { 0 };	// Buffer 1024, Last 0x00
	int nLen = m_Client.Read_Socket(byRecv);

	CString strRecvSocket, strLog;
	strRecvSocket.Format("%s", byRecv);
	m_strRecvCmd += strRecvSocket;

	while (!m_strRecvCmd.IsEmpty()) {
		int nStart = m_strRecvCmd.Find("@");
		int nEnd = m_strRecvCmd.Find("\n");

		if (nEnd < 0) break;	// 버퍼에 들어오는 중...

		if (nStart < 0 || nStart > nEnd) {
			strLog.Format("[<-] : <<Error>> %s : Start(%d), End(%d)", m_strRecvCmd, nStart, nEnd);
			g_objLogFile.Save_MesAgentLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}

		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		// Inspector Log ////////////////////////////////////////////////////////////
		strLog.Format("[<-] : %s", strRecv);
		g_objLogFile.Save_MesAgentLog(strLog);
		/////////////////////////////////////////////////////////////////////////////

		char chSep = ',';
		CString strCmd, strOp;

		AfxExtractSubString(strCmd, strRecv, 0, chSep);
		AfxExtractSubString(strOp, strRecv, 1, chSep);

		CString strArg[10];
		for (int i = 0; i < 5; i++) AfxExtractSubString(strArg[i], strRecv, i + 2, chSep);

		if (strCmd == "CONTROL") {
			if (strOp == "STATE") Get_ControlState(strArg[0]);

		} else if (strCmd == "LOT") {
			if (strOp == "START")  Get_LotStart(strArg[0], strArg[1], strArg[2], strArg[3], strArg[4]);
			if (strOp == "CANCEL") Get_LotCancel(strArg[0], strArg[1],  strArg[2]);

		} else if (strCmd == "TIME") {
			if (strOp == "UPDATE") Get_TimeSync();

		} else if (strCmd == "RECIPE") {
			if (strOp == "REQUEST") Get_RecipeList(strArg[0]);
			if (strOp == "SELECT")	Get_PPSelect(strArg[0], strArg[1]);
			if (strOp == "FAIL")	Get_PPSelectFail(strArg[0], strArg[1], strArg[2], strArg[3]);

		} else if (strCmd == "CM") {
			if (strOp == "RESULT") Get_CmResult(strArg[0], strArg[1],  strArg[2], strArg[3], strArg[4]);
			if (strOp == "FAIL")   Get_CmFail(strArg[0], strArg[1],  strArg[2], strArg[3]);

		} else if (strCmd == "HOST") {
			if (strOp == "MESSAGE") Get_HostMessage(strArg[0]);

		} else if (strCmd == "MGZ") {
			if (strOp == "CONFIRM") Get_MGZConfirm(strArg[0]);
			if (strOp == "CANCEL")  Get_MGZCancel(strArg[0], strArg[1],  strArg[2]);

		} else if (strCmd == "CARRIER") {
			if (strOp == "CONFIRM") Get_CarrierConfirm(strArg[0]);
			if (strOp == "CANCEL")  Get_CarrierCancel(strArg[0], strArg[1],  strArg[2]);

		} 
		else if (strCmd == "MODULE")
		{
			if (strOp == "DATA") Get_ModuleData(strRecv);

		}
		else if (strCmd == "RMS")
		{
			if (strOp == "ALREADYDONE") Get_RMSAlreadyDone();
			if (strOp == "LOADDONE") Get_RMSAlreadyDone();

		}
	}

	return 0;
}

void CMesAgent::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(0);
	if (GetTickCount() - gMes.dwMesTime > MES_WAITTIME) {
		g_objCommon.Show_Error(9000);
		KillTimer(0);
		return;
	}
	SetTimer(0, 100, NULL);
	CWnd::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////
// Get Command

void CMesAgent::Get_ControlState(CString sFlag)
{
	int nOnline = atoi(sFlag);	// 1:Online, 2:Offline
// 	if (m_bHostOnline && nOnline == 2) g_objCommon.Show_Error(9006);	// Agent 에서 Offline 변경
	if (nOnline == 1 && m_bHostOnline == FALSE) Set_OperUpdate(gData.sOperID);
	m_bHostOnline = (nOnline == 1 ? TRUE : FALSE);
}

void CMesAgent::Get_LotStart(CString sLotId, CString sRecipe, CString sCmCount, CString sVendor, CString sConfig)
{
	int nCmCount = atoi(sCmCount);

#ifndef AJIN_BOARD_USE
	CString strTemp;
	strTemp.Format("%d", g_objCommon.Get_Random(0,9999));
	sLotId = sLotId + strTemp;
#endif

	gMes.sHostLotID = m_sLotStartID = sLotId;
	gMes.sHostRecipe = sRecipe;
	gMes.nHostCmCount = nCmCount;
	gMes.sHostVendor = sVendor;
	gMes.sHostConfig = sConfig;

	for(int i=0; i<30; i++) {
		if (sLotId.GetLength() < 2 || gLot.sLotID[i] == sLotId) {
			g_objCommon.Show_Error(9009);	return;
		}
	}
	if (sRecipe.GetLength() < 1 || nCmCount < 1 || nCmCount > 40) {
		g_objCommon.Show_Error(9001);	return;
	}
	if (Exist_Recipe(sRecipe) == FALSE) {
		g_objCommon.Show_Error(9007);	return;
	}
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (sRecipe != pEquipData->sModelName) {
		if (pEquipData->bUseMESRcpCheck) {
			g_objCommon.Show_Error(9008);	return;
		}
		g_objCommon.Display_MESRecipe(sRecipe);
	}

	int nPortNo = 0;
	gLot.nJobCycle++;
	if (gLot.nJobCycle > 28) gLot.nJobCycle = 1;

	nPortNo = gLot.nJobCycle - 1;
	gMes.nLotPortNo = gLot.nJobCycle;
	gLot.sLotID[nPortNo] = gNG->sBackData[nPortNo][0] = gMes.sHostLotID;
	gLot.sRecipeName[nPortNo] = gNG->sBackData[nPortNo][1] = gMes.sHostRecipe;
	gLot.nCmCount[nPortNo] = gNG->nBackCnt[nPortNo] = gMes.nHostCmCount;

	CString	strLog;
	strLog.Format("[Get_LotStart] Lot Data Port(%d) LotID(%s) Count(%d) Recipe(%s)", nPortNo+1, gMes.sHostLotID, gMes.nHostCmCount, gMes.sHostRecipe);
	g_objLogFile.Save_MesAgentLog(strLog);

	gMes.nCarConfirm[LOAD_STAGE] = 4; //원래 3인데 "4"로 횡전개 해버림... 나중에 다시 원복해야 할 수도 있다...
#ifndef AJIN_BOARD_USE
	gMes.nCarConfirm[LOAD_STAGE]++;
#endif
}

void CMesAgent::Get_LotCancel(CString sLotId, CString sCode, CString sText)
{
	gMes.sHostCancelLotId = sLotId;
	gMes.sHostCancelCode = sCode;
	gMes.sHostCancelText = sText;
	g_objCommon.Show_Error(9002);
}

void CMesAgent::Get_TimeSync()
{
	g_objInspector.Set_TimeUpdate(INSPECTOR_ALL);
}

void CMesAgent::Get_RecipeList(CString sFlag)		// 0:All, 1:Current Recipe
{
	int nType = atoi(sFlag);
	Set_RecipeList(nType);
}

void CMesAgent::Get_CmResult(CString sLotId, CString sCmId, CString sJudge, CString sNgCode, CString sNgText)
{
	CString	strLog;

	int nPortNo = 99;
	for(int i=0; i<30; i++) {
		if (gLot.sLotID[i] == sLotId) { nPortNo = i; break; }
	}
	if (nPortNo > 30 || sLotId.GetLength() < 2) {
		strLog.Format("MESAgent ReciveData LotID error => (%s) (%s) (%s) (%s) (%s)", sLotId, sCmId, sJudge, sNgCode, sNgText);
		g_objLogFile.Save_MesAgentLog(strLog);
		return;
	}
	if (sCmId.GetLength() < 10) {
		strLog.Format("MESAgent ReciveData ModuleID error => (%s) (%s) (%s) (%s) (%s)", sLotId, sCmId, sJudge, sNgCode, sNgText);
		g_objLogFile.Save_MesAgentLog(strLog);
		return;
	}
	
	int nTray = -1, nCmNo = -1;
	for(int i = 0; i < 10; i++) {
		for (int j = 0; j < 40; j++) {
			if (gLot.sBarCode[nPortNo][i][j] == sCmId) {
				nTray = i, nCmNo = j;	break;
			}
		}
		if (nTray > -1) break;
	}
	if (nTray == -1 || nCmNo == -1) {
		strLog.Format("MESAgent ReciveData ModuleID Not Found => (%s) (%s) (%s) (%s) (%s)", sLotId, sCmId, sJudge, sNgCode, sNgText);
		g_objLogFile.Save_MesAgentLog(strLog);
		return;
	}

//	gMes.sCmCode[nPortNo][nTray][nCmNo] = sNgCode;
//	gMes.sCmText[nPortNo][nTray][nCmNo] = sNgText;	// Result Done
	if (sJudge == "NG") {
		gMes.nCmResult[nPortNo][nTray][nCmNo] = 2;
	} else {
		if (gMes.nCmResult[nPortNo][nTray][nCmNo] != 2) gMes.nCmResult[nPortNo][nTray][nCmNo] = 1;
	}

	DWORD dwTerm = GetTickCount() - m_dwReqStart[nPortNo][nTray][nCmNo];
	strLog.Format("[Get_CmResult] MES CM Result(%s) Judge(%s) Code(%s) Text(%s) PortNo(%d) TrayNo(%d) CmNo(%d) Time,%d", sCmId, sJudge, sNgCode, sNgText, nPortNo+1, nTray+1, nCmNo+1, dwTerm);
	g_objLogFile.Save_DispatcherLog(strLog);
}

void CMesAgent::Get_CmFail(CString sLotId, CString sCmId, CString sCode, CString sText)
{
	CString	strLog;
	gMes.sHostCancelLotId = sLotId;
	gMes.sHostCancelModule = sCmId;
	gMes.sHostCancelCode = sCode;
	gMes.sHostCancelText = sText;

	if (sCmId.GetLength() < 2) {
		strLog.Format("MESAgent CmFail ReciveData ModuleID error => (%s) (%s) (%s) (%s)", sLotId, sCmId, sCode, sText);
		g_objLogFile.Save_MesAgentLog(strLog);
		g_objCommon.Show_Error(9005);
		return;
	}

	int nPort = -1, nTray = -1, nCmNo = -1;
	for(int i=0; i<30; i++) {
		for(int j=0; j<10; j++) {
			for(int k=0; k<40; k++) {
				if (gLot.sBarCode[i][j][k] == sCmId) {
					nPort = i; nTray = j, nCmNo = k; break;
				}
			}
			if (nTray > -1) break;
		}
		if (nTray > -1) break;
	}
	if (nTray == -1 || nCmNo == -1) {
		strLog.Format("MESAgent CmFail ReciveData ModuleID Not Found => (%s) (%s) (%s) (%s)", sLotId, sCmId, sCode, sText);
		g_objLogFile.Save_MesAgentLog(strLog);
		g_objCommon.Show_Error(9005);
		return;
	}

	gMes.sHostCancelLotId = gLot.sLotID[nPort];
	gMes.nCmResult[nPort][nTray][nCmNo] = 2;
//	gMes.sCmCode[nPort][nTray][nCmNo] = gMes.sHostCancelCode;
//	gMes.sCmText[nPort][nTray][nCmNo] = gMes.sHostCancelText;

	g_objCommon.Show_Error(9005);

	strLog.Format("[Get_CmFail] MES CM Fail(%s) LotID(%s) Code(%s) Text(%s) PortNo(%d) TrayNo(%d) CmNo(%d)", sCmId, sLotId, sCode, sText, nPort+1, nTray+1, nCmNo+1);
	g_objLogFile.Save_DispatcherLog(strLog);
}

void CMesAgent::Get_HostMessage(CString sMsg)
{
	g_objCommon.Show_Alarm(sMsg, 1);
}

void CMesAgent::Get_MGZConfirm(CString sMGZID)
{
	gMes.sMZConfirmID = sMGZID;
#ifndef AJIN_BOARD_USE
	gMes.nMZConfirm[0] = gMes.nMZConfirm[1] = gMes.nMZConfirm[2] = 2;
#endif

	if (gData.sMZID[0] == sMGZID) { gMes.nMZConfirm[0] = 2; return; }
	if (gData.sMZID[5] == sMGZID) { gMes.nMZConfirm[1] = 2; return; }

	if		(gMes.nMZConfirm[0] == 1) gMes.nMZConfirm[0] = 2;
	else if (gMes.nMZConfirm[1] == 1) gMes.nMZConfirm[1] = 2;
	else if (gMes.nMZConfirm[2] == 1) gMes.nMZConfirm[2] = 2;
}

void CMesAgent::Get_MGZCancel(CString sMGZID, CString sCode, CString sText)
{
	gMes.sHostCancelMGZID = sMGZID;
	gMes.sHostCancelCode = sCode;
	gMes.sHostCancelText = sText;
	if (gData.sMZID[0] == sMGZID) { g_objCommon.Show_Error(9011); return; }
	if (gData.sMZID[5] == sMGZID) { g_objCommon.Show_Error(9012); return; }

	if		(gMes.nMZConfirm[0] == 1) g_objCommon.Show_Error(9011);
	else if (gMes.nMZConfirm[1] == 1) g_objCommon.Show_Error(9012);
	else if (gMes.nMZConfirm[2] == 1) g_objCommon.Show_Error(9013);
}

void CMesAgent::Get_CarrierConfirm (CString sCarrierID)
{
	gMes.sCarConfirmID = sCarrierID;
	if (m_sCarrier[1] == sCarrierID) { gMes.nCarConfirm[1] = 2; return; }
	if (m_sCarrier[2] == sCarrierID) { gMes.nCarConfirm[2] = 2; return; }

	if		(gMes.nCarConfirm[1] == 1) gMes.nCarConfirm[1] = 2;
	else if (gMes.nCarConfirm[2] == 1) gMes.nCarConfirm[2] = 2;
}

void CMesAgent::Get_CarrierCancel(CString sCarrierId, CString sCode, CString sText)
{
	gMes.sHostCancelTrayID = sCarrierId;
	gMes.sHostCancelCode = sCode;
	gMes.sHostCancelText = sText;
	if (m_sCarrier[0] == sCarrierId) { g_objCommon.Show_Error(9015); return; }
	if (m_sCarrier[1] == sCarrierId) { g_objCommon.Show_Error(9016); return; }
	if (m_sCarrier[2] == sCarrierId) { g_objCommon.Show_Error(9017); return; }

	if		(gMes.nCarConfirm[LOAD_STAGE] == 1) g_objCommon.Show_Error(9015);
	else if (gMes.nCarConfirm[GOOD_STAGE] == 1) g_objCommon.Show_Error(9016);
	else if (gMes.nCarConfirm[NG_STAGE] == 1) g_objCommon.Show_Error(9017);
}

void CMesAgent::Get_ModuleData(CString sData)
{
	CString strLog, sTemp, sCnt, sRcvData[40][13];

	AfxExtractSubString(sCnt, sData, 2, ',');
	int nLen = sData.GetLength();
	int nCnt = atoi(sCnt);
	if (nCnt < 1 || nCnt > 40) {
		strLog.Format("MESAgent Module Data Error => Count(%d) Length(%d)", nCnt, nLen);
		g_objLogFile.Save_MesAgentLog(strLog);
		g_objCommon.Show_Error(9021);
		return;
	}

	int nCosNG, k=3;
	for(int i=0; i<nCnt; i++) {
		for(int j=0; j<13; j++) {
			AfxExtractSubString(sRcvData[i][j], sData, k++, ',');
		}
	}

	gMes.sHostLotID = sRcvData[0][0];
	if (sRcvData[0][0].GetLength() < 2) {
		strLog.Format("MESAgent Module LotID Error => (%s)", sRcvData[0][0]);
		g_objLogFile.Save_MesAgentLog(strLog);
		g_objCommon.Show_Error(9022);
		return;
	}

//	gLot.sLotID[2] = sRcvData[0][0];	//GJC_Test
	for(int i=0; i<30; i++)
	{
		if (gLot.sLotID[i] == sRcvData[0][0]) 
		{
			for(int j=0; j<nCnt; j++)
			{
				gNG->sModuleID[i][j]   = sRcvData[j][1];
				gNG->sHaimNGCd[i][j]   = sRcvData[j][11];
				gNG->sCosmeticNG[i][j] = sRcvData[j][12];
			}
			sTemp = ""; k=0; nCosNG=0;
			for(int j=0; j<nCnt; j++) {
				sTemp += "," + gNG->sModuleID[i][j];
				sTemp += "," + gNG->sHaimNGCd[i][j];
				sTemp += "," + gNG->sCosmeticNG[i][j];
				if (gNG->sHaimNGCd[i][j].GetLength() > 0) k++;
				if (gNG->sCosmeticNG[i][j] == "NG") nCosNG++;
			}
			gMes.nCarConfirm[LOAD_STAGE]++;	//4

			//0:LOTID,1:MODULEID,2:SITE,3:EQPID,4:EQPNAME,5:TOOL_CAVITY,6:PARA,7:DATE,8:ROS_JUDGE,9:DFA_LOTID,10:POCKETNO,11:HaimNGCode
			for(int j=0; j<nCnt; j++)
			{
				g_objInspector.Set_ModuleData(INSPECTOR_ALL, sRcvData[0][0], j+1, sRcvData[j][1], sRcvData[j][2], sRcvData[j][3], sRcvData[j][4], sRcvData[j][6], sRcvData[j][5], nCnt);
			}

			strLog.Format("[Get_ModuleData] Module Data LotID(%s) Port(%d) Count(%d) Haim(%d) CosNG(%d) %s", gMes.sHostLotID, i+1, nCnt, k, nCosNG, sTemp);
			g_objLogFile.Save_MesAgentLog(strLog);
			return;
		}
	}

	strLog.Format("MESAgent Module Data LotID Not Found Error => LotID(%s) MesNo(%d) CycleNo(%d)", sRcvData[0][0], gMes.nCarConfirm[LOAD_STAGE], gLot.nJobCycle);
	g_objLogFile.Save_MesAgentLog(strLog);
	g_objCommon.Show_Error(9023);
}

void CMesAgent::Get_PPSelect(CString sLotId, CString sRecipe)
{
	gMes.sHostLotID = sLotId;
	gMes.sHostRecipe = sRecipe;
	if (gMes.sHostLotID.GetLength() < 5 || gMes.sHostRecipe.GetLength() < 2) {
		g_objCommon.Show_Error(9004); return;
	}
	gMes.nCarConfirm[LOAD_STAGE] = 2;
}

void CMesAgent::Get_PPSelectFail(CString sLotId, CString sRecipe, CString sCode, CString sText)
{
	gMes.sHostLotID = sLotId;
	gMes.sHostRecipe = sRecipe;
	gMes.sHostCancelCode = sCode;
	gMes.sHostCancelText = sText;
	g_objCommon.Show_Error(9030);
}

void CMesAgent::Get_RMSAlreadyDone()
{
	gData.bRMSDone = TRUE;
}

void CMesAgent::Get_RMSDone()
{
	gData.bRMSDone = TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Set Command

void CMesAgent::Set_EquipState(int nFlag)
{
	// MES : Init, idle, Setup, Ready, Executing(=Run), Paused(=Down)
	CString strSend;
	strSend.Format("EQUIP,STATE,%d,%s", nFlag, gData.sOperID);	// 1:Init, 2:Idle, 3:Setup, 4:Ready, 5:Run(=Executing), 6;Pause(=Down)
	Send_Command(strSend);
}

void CMesAgent::Set_ErrorUpdate(int nFlag, CString sErrNo)
{
	CString strSend;
	strSend.Format("ERROR,UPDATE,%d,%s", nFlag, sErrNo);
	Send_Command(strSend);
}

void CMesAgent::Set_ControlState(int nFlag, CString sOperId)
{
	CString strSend;
	strSend.Format("CONTROL,STATE,%d,%s", nFlag, sOperId);
	Send_Command(strSend);
	if (nFlag == 2) m_bHostOnline = FALSE;	// 사용자 Offline
}

void CMesAgent::Set_LotStart(CString sLotId, CString sMGZId, int nSlot, CString sTrayID, CString sRecipe)
{
//	if (nType == 0) gMes.nLotStatus[nPortNo] = 1;
//	else			gMes.nLotStatus[nPortNo] = 4;
	CString strSend, strLogID;
	if (sLotId.GetLength() < 5) strLogID = m_sLotStartID;
	else						strLogID = sLotId;
	strSend.Format("LOT,START,%s,%s,%d,%s,%s", strLogID, sMGZId, nSlot, sTrayID, sRecipe);
	Send_Command(strSend);
}

void CMesAgent::Set_LotEnd(CString sLotId, CString sMZId, CString sTrayId, CString sRecipe, int nHCount, int nOk, int nNg)
{
	if (sTrayId.GetLength() < 2) sTrayId = m_sCarrier[1];
	CString strSend;
	strSend.Format("LOT,END,%s,%s,%s,%s,%d,%d,%d", sLotId, sMZId, sTrayId, sRecipe, nHCount, nOk, nNg);
	Send_Command(strSend);
}

void CMesAgent::Set_LotEnd(int nPortNo, CString sLotId, CString sDeepData1, CString sDeepData2, CString sDeepData3, CString sDeepData4, CString sDeepData5)
{
	if (nPortNo < 1 || nPortNo > 30) return;
	int nNo = nPortNo - 1;
	if (gLot.sCarID_GD[nNo].GetLength() < 2) gLot.sCarID_GD[nNo] = m_sCarrier[1];

	CString strSend;
	strSend.Format("LOT,END,%s,%s,%s,%s,%d,%d,%d,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d",
				sLotId, gLot.sMZID_GD[nNo], gLot.sCarID_GD[nNo], gLot.sRecipeName[nNo], gLot.nCmCount[nNo], gLot.nGoodCount[nNo], gLot.nNgCount[nNo]+gLot.nSkipCount[nNo],
				sDeepData1, sDeepData2, sDeepData3, sDeepData4, sDeepData5,
				gLot.nRosJugCount[nNo][0], gLot.nRosJugCount[nNo][1], gLot.nRosJugCount[nNo][3], gLot.nRosJugCount[nNo][2]+gLot.nRosJugCount[nNo][4], gLot.nRosJugCount[nNo][10],
				                                                      gLot.nRosJugCount[nNo][3] +gLot.nRosJugCount[nNo][2]+gLot.nRosJugCount[nNo][4] +gLot.nRosJugCount[nNo][10]);
				//Deep Learning투입수, Deep Learning양품수, Deep Learning NG수, Deep Learning TimeOut수, Deep Learning투입 Image수
				//ROS투입수,                ROS양품수,                 ROS NG수,                  ROS Repair수,                                        ROS Skip수,   MVI수(ROS NG+ROS Repair+ROS Skip)
	Send_Command(strSend);
}

void CMesAgent::Set_LotAbort(CString sLotId)
{
	CString strSend;
	strSend.Format("LOT,ABORT,%s", sLotId);
	Send_Command(strSend);
}

void CMesAgent::Set_OperUpdate(CString sOperId)
{
	if (sOperId.GetLength() < 4) return;
	CString strSend;
	strSend.Format("OPER,UPDATE,%s", sOperId);
	Send_Command(strSend);
}

void CMesAgent::Set_IdleReport(CString sOperId, CString sSTime, CString sETime, CString sCode, CString sType)
{
	CString strSend;
	strSend.Format("IDLE,REPORT,%s,%s,%s,%s,%s", sOperId, sSTime, sETime, sCode, sType);
	Send_Command(strSend);
}

void CMesAgent::Set_RecipeList(int nFlag)						// 0:All, 1:Current Recipe
{
	CString strSend;

	if (nFlag == 1) {	// 1:Current Recipe
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		strSend.Format("RECIPE,REQUEST,1,1,%s", pEquipData->sModelName);
		Send_Command(strSend);
	}
	if (nFlag == 0) {	// 0:All Recipe
		int		nCount = 0;
		CString sPathSource, sRcipeData = "";

		sPathSource = gsCurrentDir + "\\System\\Model";

		if (GetFileAttributes(sPathSource) == -1) {	// 디렉토리 없음
			strSend.Format("RECIPE,REQUEST,0,%d,%s", nCount, sRcipeData);
			Send_Command(strSend);
			return;
		}

		CFileFind ff;
		BOOL bFile = ff.FindFile(sPathSource + _T("\\*.*"));

		while(bFile)
		{
			bFile = ff.FindNextFile();

			CString str;	// = ff.GetFileName();
			if(ff.IsDots()) continue;

			if(ff.IsDirectory()){
				str = ff.GetFileName();	nCount++;
				if (nCount == 1) sRcipeData = str;
				else			 sRcipeData = sRcipeData + "," + str;
			}
		}
		ff.Close();

		strSend.Format("RECIPE,REQUEST,0,%d,%s", nCount, sRcipeData);
		Send_Command(strSend);
	}
}

void CMesAgent::Set_CmRequest(CString sLotId, CString sCmId, int nPortNo, int nTrayNo, int nCmNo)
{
	return;	//DFA는 사용하지 않습니다.

	CString strSend;
	strSend.Format("CM,REQUEST,%s,%s", sLotId, sCmId);
	Send_Command(strSend);

	m_dwReqStart[nPortNo-1][nTrayNo-1][nCmNo-1] = GetTickCount();
}

void CMesAgent::Set_CmEnd(int nType, int nPortNo, int nTrayNo, int nCmNo, int nOut, CString sCarID)
{
	if (nPortNo < 1 || nPortNo > 30 || nCmNo < 1 || nCmNo > 40) return;

	CString sLotID, strCmId, strResult, strNgCode, strSend, sFmMZ, sFmTray, sToTray, sROSJudge, strNGCd, sMarginal;
	sLotID  = gLot.sLotID[nPortNo-1];
	strCmId = gLot.sBarCode[nPortNo-1][nTrayNo-1][nCmNo-1];
	strNGCd = gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCmNo-1][0];
	sFmMZ =   gLot.sMZID_LD[nPortNo-1];
	sFmTray = gLot.sCarID_LD[nPortNo-1];
	sToTray = sCarID;
	sROSJudge = "";
	sMarginal = "";

	if (nType == 1) {
		strResult = "NG";
		sToTray = gData.sGoodCarrierID;
		if (sLotID != m_sLotID || m_nPorketNo < 1 || m_nPorketNo > 40) {
			m_sLotID = sLotID; 
			m_nPorketNo = 1;
		}
		if	(gLot.nJudge_R[nPortNo-1][nTrayNo-1][nCmNo-1] == 4) { sROSJudge = "REPAIR"; strNgCode = "Repair"; }
		else													{ sROSJudge = "NG";		strNgCode = Set_NGSort(nPortNo, nTrayNo, nCmNo);}
		if	(strNGCd == "ROS Timeout") strNgCode = "ROS Timeout";
		if	(strNGCd == "Skip_ROS_R")  strNgCode = "Skip ROS R";
		if	(strNGCd == "Skip_ROS_N")  strNgCode = "Skip ROS N";
		if	(strNGCd == "BARCODE_NOREAD" || strNgCode == "BARCODE_NOREAD") { strCmId = "NOREAD"; strNgCode = "BARCODE_NOREAD"; }

		strSend.Format("CM,END,%s,%s,%s,%s,%s,%s,%d,%s,%d,%s,%s", sLotID, strCmId, strResult, strNgCode, sFmMZ, sFmTray, nCmNo, sToTray, m_nPorketNo, sROSJudge, sMarginal);
		m_nPorketNo++;
	} else {
		strResult = "OK";
		strNgCode = "00";
		if (gLot.nJudge_R[nPortNo-1][nTrayNo-1][nCmNo-1] == 2) sROSJudge = "OK";
		if (gLot.nMarginal[nPortNo-1][nCmNo-1] > 0) sMarginal = "OK";
		strSend.Format("CM,END,%s,%s,%s,%s,%s,%s,%d,%s,%d,%s,%s", sLotID, strCmId, strResult, strNgCode, sFmMZ, sFmTray, nCmNo, sToTray, nOut, sROSJudge, sMarginal);
	}
	Send_Command(strSend);	
}

CString CMesAgent::Set_NGSort(int nPno, int nTNo, int CNo)
{
	CString sNGCode, sCvtNGCode;

	sNGCode = gLot.sNGCode_I[nPno-1][nTNo-1][CNo-1][0];
	if (sNGCode == "BARCODE_NOREAD" || sNGCode == "MES_NG" || sNGCode == "ROI_FAIL" || sNGCode == "GRAB_FAIL") return sNGCode;
	if (sNGCode == "Vision Result Timeout" || sNGCode == "ROS Timeout" || sNGCode == "MCBTM" || sNGCode == "FAI_RECOFAIL_SP") return sNGCode;
	if (sNGCode == "Skip_ROS_N" || sNGCode == "Skip_ROS_R" || sNGCode == "VMAC000003") return sNGCode;

	int nSeq[10] = { 999, 999, 999, 999, 999, 999, 999, 999, 999, 999 };
	for (int i=1; i<6; i++) {
		for (int j=0; j<m_nSizeCount; j++) {
			if (m_sSizeNG[j].GetLength() > 0 && gLot.sNGCode_I[nPno-1][nTNo-1][CNo-1][i] == m_sSizeNG[j]) {
				nSeq[i-1] = j; break;
			}
		}
	}

	int nNo = 999, nI, nType;
	for (int i=0; i<5; i++) {
		if (nNo > nSeq[i]) { nNo = nSeq[i]; nI = i; }
	}
	nType = gLot.nFOcapExist[nPno-1][nTNo-1][CNo-1];
	if (nNo < 100) { gLot.sNGCode_I[nPno-1][nTNo-1][CNo-1][0] = sNGCode = m_sSizeNG[nNo]; Set_OcapFaiNG(nPno, nType, sNGCode); }
	if (sNGCode.GetLength() < 1) sNGCode = "NG";
	return sNGCode;
}

void CMesAgent::Set_OcapFaiNG(int nPno, int nType, CString sFaiNGCode)
{
	if (sFaiNGCode.GetLength() < 1) return;

	/*for(int i=0; i<20; i++) {
		if (sFaiNGCode == gCap.sFAICode[i]) { gLot.nOcapCount[nPno-1][i]++; if (nType==1) gLot.nFOcapCount[nPno-1][i]++; return; }
	}

	CString sFAING;
	if (sFaiNGCode.GetLength() >= 7) {
		sFAING = sFaiNGCode.Left(7);
		for(int i=7; i<14; i++) {
			if (sFAING == gCap.sFAICode[i]) { gLot.nOcapCount[nPno-1][i]++; if (nType==1) gLot.nFOcapCount[nPno-1][i]++; return; }
		}
	}*/
}

void CMesAgent::Set_TerminalOK()
{
	CString strSend;
	strSend.Format("TERMINAL,MSG");
	Send_Command(strSend);
}

void CMesAgent::Set_MGZIDReport(CString sType, CString sMGZID)
{
	CString strSend;
	strSend.Format("MGZ,ID,%s,%s", sType, sMGZID);
	Send_Command(strSend);
}

void CMesAgent::Set_MGZIDRemove(CString sType, CString sMGZID, CString sRecipe)	//Type:N,G
{
	CString strSend;
	strSend.Format("MGZ,REMOVE,%s,%s,%s", sType, sMGZID, sRecipe);
	Send_Command(strSend);
}

void CMesAgent::Set_CarrierInMGZ(CString sType, CString sLotID, CString sMGZID, CString sCarrierID, int nSlot)	//Type:L,N,G
{
	CString strSend;
	strSend.Format("CARRIER,IN,%s,%s,%s,%s,%d", sType, sLotID, sMGZID, sCarrierID, nSlot);
	Send_Command(strSend);
}

void CMesAgent::Set_CarrierOutMGZ(CString sMGZID, CString sCarrierID, int nSlot)
{
	CString strSend;
	strSend.Format("CARRIER,OUT,%s,%s,%d", sMGZID, sCarrierID, nSlot);
	Send_Command(strSend);
}

void CMesAgent::Set_CarrierIDReport(CString sType, CString sMGZID, CString sCarrierID)
{
	if (sType == "L") m_sCarrier[0] = sCarrierID;
	if (sType == "G") m_sCarrier[1] = sCarrierID;
	if (sType == "N") m_sCarrier[2] = sCarrierID;

	CString strSend;
	strSend.Format("CARRIER,ID,%s,%s,%s", sType, sMGZID, sCarrierID);
	Send_Command(strSend);
}

void CMesAgent::Set_PPSelectReport(CString sLotId, CString sVersion)
{
	CString strSend, sBodyData;

	for(int i=0; i<4; i++) 
	{
		sBodyData = "";
		for(int j=0; j<50; j++) 
		{
			sBodyData = sBodyData + gMes.sBodyData[i][j] + ",";
		}
		strSend.Format("RECIPE,REPORT,%d,%s,%s", i, sVersion, sBodyData);
		Send_Command(strSend);
	}
}

void CMesAgent::Set_RMSCheck()
{
	CString strSend;

	gData.bRMSDone = FALSE;

	strSend.Format("RMS,CHECK");
	Send_Command(strSend);

}

///////////////////////////////////////////////////////////////////////////////

void CMesAgent::Send_Command(CString sSend)
{	
	CString strSendSocket, strLog;
//#ifdef AJIN_BOARD_USE
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseMES) return;	

	if (!m_bConnected) return;
	if (sSend.Left(11) != "OPER,UPDATE" && sSend.Left(7) != "CONTROL" && sSend.Left(9) != "LOT,ABORT") {
		if (!m_bHostOnline) return;
	}	

	strSendSocket.Format("@%s\n", sSend);

	char chSend[1001] = { 0 };	// Buffer 1000, Last 0x00
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	if (!m_Client.Write_Socket((BYTE*)chSend, nLength)) return;
//#endif
	// Host Log ////////////////////////////////////////////
	strLog.Format("[->] : %s", sSend);
	g_objLogFile.Save_MesAgentLog(strLog);
	///////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////
BOOL CMesAgent::Exist_Recipe(CString sRecipe)
{
	CString sPathSource;

	sPathSource = gsCurrentDir + "\\System\\Model";
	if (GetFileAttributes(sPathSource) == -1) return FALSE;

	CFileFind ff;
	BOOL bFile = ff.FindFile(sPathSource + _T("\\*.*"));

	while(bFile)
	{
		bFile = ff.FindNextFile();
		if(ff.IsDots()) continue;

		if(ff.IsDirectory()) {
			if (sRecipe == ff.GetFileName()) { ff.Close(); return TRUE; }
		}
	}
	ff.Close();

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////