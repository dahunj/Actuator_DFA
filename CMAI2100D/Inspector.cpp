// Inspector.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMAI2100.h"
#include "Inspector.h"
#include "Dispatcher.h"
#include "LogFile.h"
#include "Common.h"
#include "WorkDlg.h"
#include "ManualDlg.h"
#include "SequenceMain.h"
#include "MesAgent.h"

//#define UDP_PC1_HOST_IP		"127.0.0.1"
#define UDP_HANDLER_IP		"192.168.0.11"
#define UDP_PC1_HOST_IP		"192.168.0.21"
#define UDP_PC2_HOST_IP		"192.168.0.22"
#define UDP_PC3_HOST_IP		"192.168.0.23"
#define UDP_PC4_HOST_IP		"192.168.0.24"
#define UDP_PC5_HOST_IP		"192.168.0.25"

#define UDP_PC1_LPORT		8001
#define UDP_PC1_HPORT		8001
#define UDP_PC2_LPORT		8002
#define UDP_PC2_HPORT		8002
#define UDP_PC3_LPORT		8003
#define UDP_PC3_HPORT		8003
#define UDP_PC4_LPORT		8004
#define UDP_PC4_HPORT		8004
#define UDP_PC5_LPORT		8005
#define UDP_PC5_HPORT		8005

IMPLEMENT_DYNAMIC(CInspector, CWnd)

CInspector g_objInspector;

CCriticalSection g_csInspector;	// Send_Command 문제 해결하기 위함

// CInspector

CInspector::CInspector()
{
	m_strRecvCmd = "";

	m_bConnectVisionPC1 = m_bConnectVisionPC2 = m_bConnectVisionPC3 = m_bConnectVisionPC4 = m_bConnectVisionPC5 = FALSE;
	m_nStatusVisionPC1 = m_nStatusVisionPC2 = m_nStatusVisionPC3 = m_nStatusVisionPC4 = m_nStatusVisionPC5 = 0;
	m_bLotReady1 = m_bLotReady2 = m_bLotReady3 = m_bLotReady4 = m_bLotReady5 = FALSE;
}

CInspector::~CInspector()
{
}

BEGIN_MESSAGE_MAP(CInspector, CWnd)
	ON_WM_TIMER()
	ON_MESSAGE(UM_UDP_RECEIVE, OnUdpReceive)
END_MESSAGE_MAP()

// CInspector 메시지 처리기입니다.

void CInspector::Initialize()
{
#ifndef AJIN_BOARD_USE
	BOOL bVisionPC1Opened = m_UdpVisionPC1.Open_Socket(21000, 21001, "127.0.0.1", this);
	BOOL bVisionPC2Opened = m_UdpVisionPC2.Open_Socket(22000, 22001, "127.0.0.1", this);
	BOOL bVisionPC3Opened = m_UdpVisionPC3.Open_Socket(23000, 23001, "127.0.0.1", this);
	BOOL bVisionPC4Opened = m_UdpVisionPC4.Open_Socket(24000, 24001, "127.0.0.1", this);
	BOOL bVisionPC5Opened = m_UdpVisionPC5.Open_Socket(25000, 25001, "127.0.0.1", this);
#else
	BOOL bVisionPC1Opened = m_UdpVisionPC1.Open_Socket(UDP_PC1_LPORT, UDP_PC1_HPORT, UDP_PC1_HOST_IP, this);
	BOOL bVisionPC2Opened = m_UdpVisionPC2.Open_Socket(UDP_PC2_LPORT, UDP_PC2_HPORT, UDP_PC2_HOST_IP, this);
	BOOL bVisionPC3Opened = m_UdpVisionPC3.Open_Socket(UDP_PC3_LPORT, UDP_PC3_HPORT, UDP_PC3_HOST_IP, this);
	BOOL bVisionPC4Opened = m_UdpVisionPC4.Open_Socket(UDP_PC4_LPORT, UDP_PC4_HPORT, UDP_PC4_HOST_IP, this);
	BOOL bVisionPC5Opened = m_UdpVisionPC5.Open_Socket(UDP_PC5_LPORT, UDP_PC5_HPORT, UDP_PC5_HOST_IP, this);

#endif
	
	if (bVisionPC1Opened) Set_ConnectRequest(INSPECTOR_PC1);
	if (bVisionPC2Opened) Set_ConnectRequest(INSPECTOR_PC2);
	if (bVisionPC3Opened) Set_ConnectRequest(INSPECTOR_PC3);
	if (bVisionPC4Opened) Set_ConnectRequest(INSPECTOR_PC4);
	if (bVisionPC5Opened) Set_ConnectRequest(INSPECTOR_PC5);
}

void CInspector::Terminate()
{
	Set_ConnectEnd(INSPECTOR_ALL);

	m_UdpVisionPC1.Close_Socket();
	m_UdpVisionPC2.Close_Socket();

	m_UdpVisionPC3.Close_Socket();
	m_UdpVisionPC4.Close_Socket();
	m_UdpVisionPC5.Close_Socket();
}

///////////////////////////////////////////////////////////////////////////////
// UDP Socket Message

void CInspector::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	switch (nIDEvent) {
	case INSPECTOR_PC1:	m_nStatusVisionPC1 = 0; break;
	case INSPECTOR_PC2:	m_nStatusVisionPC2 = 0; break;
	case INSPECTOR_PC3:	m_nStatusVisionPC3 = 0; break;
	case INSPECTOR_PC4:	m_nStatusVisionPC4 = 0; break;
	case INSPECTOR_PC5:	m_nStatusVisionPC5 = 0; break;
	case 11:			Set_StatusRequest(INSPECTOR_PC1); break;
	case 12:			Set_StatusRequest(INSPECTOR_PC2); break;
	case 13:			Set_StatusRequest(INSPECTOR_PC3); break;
	case 14:			Set_StatusRequest(INSPECTOR_PC4); break;
	case 15:			Set_StatusRequest(INSPECTOR_PC5); break;
	}
	if (nIDEvent>=11) SetTimer(nIDEvent, 5000, NULL);
	CWnd::OnTimer(nIDEvent);
}

LRESULT CInspector::OnUdpReceive(WPARAM wLocalPort, LPARAM lParam)
{
	UINT nPort = (UINT)wLocalPort;
	int nInspector = 0, nLen = 0;
	BYTE byRecv[1024] = { 0 };
	CString strLog;

#ifndef AJIN_BOARD_USE
	if (nPort == 21001) { nInspector = INSPECTOR_PC1; nLen = m_UdpVisionPC1.Read_Socket(byRecv); }
	if (nPort == 22001) { nInspector = INSPECTOR_PC2; nLen = m_UdpVisionPC2.Read_Socket(byRecv); }
	if (nPort == 23001) { nInspector = INSPECTOR_PC3; nLen = m_UdpVisionPC3.Read_Socket(byRecv); }
	if (nPort == 24001) { nInspector = INSPECTOR_PC4; nLen = m_UdpVisionPC4.Read_Socket(byRecv); }
	if (nPort == 25001) { nInspector = INSPECTOR_PC5; nLen = m_UdpVisionPC5.Read_Socket(byRecv); }
#else
	if (nPort == UDP_PC1_HPORT) { nInspector = INSPECTOR_PC1; nLen = m_UdpVisionPC1.Read_Socket(byRecv); }
	if (nPort == UDP_PC2_HPORT) { nInspector = INSPECTOR_PC2; nLen = m_UdpVisionPC2.Read_Socket(byRecv); }
	if (nPort == UDP_PC3_HPORT) { nInspector = INSPECTOR_PC3; nLen = m_UdpVisionPC3.Read_Socket(byRecv); }
	if (nPort == UDP_PC4_HPORT) { nInspector = INSPECTOR_PC4; nLen = m_UdpVisionPC4.Read_Socket(byRecv); }
	if (nPort == UDP_PC5_HPORT) { nInspector = INSPECTOR_PC5; nLen = m_UdpVisionPC5.Read_Socket(byRecv); }

#endif

	
	if (nInspector == 0 || nLen < 1) {
		strLog.Format("[H<-V%d] , Local Port (%d) Mismatch or Receive Data Zero (%d)", nInspector, nPort, nLen);
		g_objLogFile.Save_InspectorLog(strLog);
		return 0;
	}

	CString strRecvSocket;
	strRecvSocket.Format("%s", byRecv);
	m_strRecvCmd += strRecvSocket;

	while (!m_strRecvCmd.IsEmpty()) {
		int nStart = m_strRecvCmd.Find("@");
		int nEnd = m_strRecvCmd.Find("\n");

		if (nEnd < 0) break;	// 버퍼에 들어오는 중...

		if (nStart < 0 || nStart > nEnd) {
			strLog.Format("[H<-V%d] , <<Error>> %s : Start(%d), End(%d)", nInspector, m_strRecvCmd, nStart, nEnd);
			g_objLogFile.Save_InspectorLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}

		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		char chSep = ',';
		CString strCmd, strOp;

		AfxExtractSubString(strCmd, strRecv, 0, chSep);
		AfxExtractSubString(strOp, strRecv, 1, chSep);

		// Inspector Log ////////////////////////////////////////
		if (strCmd != "STATUS" && strOp != "UPDATE" && strCmd != "HEART") {
			strLog.Format("[H<-V%d] , %s", nInspector, strRecv);
			g_objLogFile.Save_InspectorLog(strLog);
		}
		/////////////////////////////////////////////////////////

		CString strArg[15];
		for (int i = 0; i < 11; i++) AfxExtractSubString(strArg[i], strRecv, i + 2, chSep);

		if (strCmd == "CONNECT") {
			if (strOp == "REQUEST")	Get_ConnectRequest(nInspector);
			else if (strOp == "REPLY") Get_ConnectReply(nInspector);
			else if (strOp == "END") Get_ConnectEnd(nInspector);

		} else if (strCmd == "STATUS") {
			if (strOp == "REQUEST")	Get_StatusRequest(nInspector);
			else if (strOp == "REPLY") Get_StatusReply(nInspector, strArg[0]);
			else if (strOp == "UPDATE") Get_StatusUpdate(nInspector, strArg[0]);

		} else if (strCmd == "LOT") {
			if (strOp == "READY") Get_LotReady(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4]);
			else if (strOp == "END") Get_LotEnd(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strArg[5], strArg[6]);

		} else if (strCmd == "SCAN") {
			if (strOp == "COMPLETE") Get_ScanComplete(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4]);

		} else if (strCmd == "INSPECT") {
			if (strOp == "COMPLETE") Get_InspectComplete(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strArg[5], strArg[6], strArg[7], strArg[8], strArg[9], strArg[10], strRecv);

		} else if (strCmd == "AMOVE") {
			if (strOp == "REQUEST") Get_AMoveRequest(nInspector, strArg[0], strArg[1]);

		} else if (strCmd == "POSITION") {
			if (strOp == "REQUEST") Get_PositionRequest(nInspector, strArg[0]);

		} else if (strCmd == "BARCODE") {
			if (strOp == "RESULT") Get_BarcodeResult(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4]);

		} else if (strCmd == "ERROR") {
			if (strOp == "REQUEST") Get_ErrorRequest(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strArg[5], strArg[6]);

		} else if (strCmd == "HEART") {
			if (strOp == "BEAT") Get_HeartBeat(nInspector);

		} else if (strCmd == "ALIGN") {
			if (strOp == "COMPLETE") Get_AlignComplete(nInspector, strArg[0], strRecv);

		} else if (strCmd == "RECIPE") {
			if (strOp == "BODY") Get_RecipeBody(nInspector, strArg[0], strRecv);

		}
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Get Command

void CInspector::Get_ConnectRequest(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectVisionPC1 = TRUE;
	if (nInspector == INSPECTOR_PC2) m_bConnectVisionPC2 = TRUE;
	if (nInspector == INSPECTOR_PC3) m_bConnectVisionPC3 = TRUE;
	if (nInspector == INSPECTOR_PC4) m_bConnectVisionPC4 = TRUE;
	if (nInspector == INSPECTOR_PC5) m_bConnectVisionPC5 = TRUE;
	Set_ConnectReply(nInspector);
}

void CInspector::Get_ConnectReply(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectVisionPC1 = TRUE;
	if (nInspector == INSPECTOR_PC2) m_bConnectVisionPC2 = TRUE;
	if (nInspector == INSPECTOR_PC3) m_bConnectVisionPC3 = TRUE;
	if (nInspector == INSPECTOR_PC4) m_bConnectVisionPC4 = TRUE;
	if (nInspector == INSPECTOR_PC5) m_bConnectVisionPC5 = TRUE;
}

void CInspector::Get_ConnectEnd(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectVisionPC1 = FALSE;
	if (nInspector == INSPECTOR_PC2) m_bConnectVisionPC2 = FALSE;
	if (nInspector == INSPECTOR_PC3) m_bConnectVisionPC3 = FALSE;
	if (nInspector == INSPECTOR_PC4) m_bConnectVisionPC4 = FALSE;
	if (nInspector == INSPECTOR_PC5) m_bConnectVisionPC5 = FALSE;
	SetTimer(nInspector+10, 5000, NULL);
}

void CInspector::Get_StatusRequest(int nInspector)
{
	BOOL bStatus = g_objSequenceMain.Is_MainThreadRun();
	if (bStatus) Set_StatusReply(nInspector, 1);
	else		 Set_StatusReply(nInspector, 0);
}

void CInspector::Get_StatusReply(int nInspector, CString sStatus)
{
	if (nInspector == INSPECTOR_PC1) m_nStatusVisionPC1 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC2) m_nStatusVisionPC2 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC3) m_nStatusVisionPC3 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC4) m_nStatusVisionPC4 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC5) m_nStatusVisionPC5 = atoi(sStatus);
	KillTimer(nInspector+10);
}

void CInspector::Get_StatusUpdate(int nInspector, CString sStatus)
{
	KillTimer(nInspector);
	if (nInspector == INSPECTOR_PC1) m_nStatusVisionPC1 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC2) m_nStatusVisionPC2 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC3) m_nStatusVisionPC3 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC4) m_nStatusVisionPC4 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC5) m_nStatusVisionPC5 = atoi(sStatus);
	SetTimer(nInspector, 15000, NULL);
}

void CInspector::Get_LotReady(int nInspector, CString sLotID, CString sPortNo, CString sPgVer, CString sPaVer, CString sRcpName)
{
	if (nInspector == INSPECTOR_PC1) m_bLotReady1 = TRUE;
	if (nInspector == INSPECTOR_PC2) m_bLotReady2 = TRUE;
	if (nInspector == INSPECTOR_PC3) m_bLotReady3 = TRUE;
	if (nInspector == INSPECTOR_PC4) m_bLotReady4 = TRUE;
	if (nInspector == INSPECTOR_PC5) m_bLotReady5 = TRUE;
}

void CInspector::Get_ScanComplete(int nInspector, CString sType, CString sLotID, CString sPortNo, CString sTrayNo, CString sCMNo)
{
	int	nPortNo = atoi(sPortNo);
	int	nTrayNo = 1;
	int	nCMNo   = atoi(sCMNo);
	if (nPortNo < 1 || nPortNo > 30 || nTrayNo < 1 || nTrayNo > 10 || nCMNo < 1 || nCMNo > 40) { g_objCommon.Show_Error(9101); return; }
	if (sType != "B1" && sType == "AG" && sType == "TG" && sType == "T1" && sType == "T2" && sType == "A1") { g_objCommon.Show_Error(9102); return; }

	if (sType == "T2") m_dwT2ScanDone[nPortNo-1][nTrayNo-1][nCMNo-1] = GetTickCount();
	int nVNo = (sType == "B1" ? 1 : (sType == "AG" ? 2 : (sType == "T1" ? 3 : (sType == "TG" ? 4 : (sType == "T2" ? 5 : 0)))));
	if (nVNo >= 1 && nVNo <= 5) {
		if (m_bScanEnd[nVNo-1][0] == FALSE) m_bScanEnd[nVNo-1][0] = TRUE;
		else								m_bScanEnd[nVNo-1][1] = TRUE;
		if (m_bScanEnd[nVNo-1][0] == FALSE || m_bScanEnd[nVNo-1][1] == FALSE) return;
	}

	if (sType == "B1" || sType == "AG") { gData.bBTMScanDone = TRUE; return; }
	if (sType == "T1" || sType == "TG") { gData.bTop1ScanDone = TRUE; return; }
	if (sType == "T2")					{ gData.bTop2ScanDone = TRUE; return; }
	if (sType == "A1")					{ gData.bAlignScanDone = TRUE; return; }
}

void CInspector::Get_InspectComplete(int nInspector, CString sType, CString sLotID, CString sPortNo, CString sTrayNo, CString sCMNo, CString sJudge, CString sNGCode, CString sImageCnt1, CString sImageCnt2, CString sMarginal,CString sNGCodeVision, CString sRcvData)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strLog;
	int	nPortNo   = atoi(sPortNo);
	int	nTrayNo   = 1;
	int	nCMNo     = atoi(sCMNo);
	int	nImage1   = atoi(sImageCnt1);
	int	nImage2   = atoi(sImageCnt2);
	int	nMarginal = atoi(sMarginal);
	if (nPortNo < 1 || nPortNo > 30 || nTrayNo < 1 || nTrayNo > 10  || nCMNo < 1 || nCMNo > 40) { g_objCommon.Show_Error(9103); return; }
	if (sType != "B1" && sType != "AG" && sType != "T1" && sType != "TG" && sType != "T2") { g_objCommon.Show_Error(9104); return; }
	if (sJudge != "G" && sJudge != "N"  && sJudge != "S"  && sJudge != "T" && sJudge != "W" && sJudge != "X" && sJudge != "R" && sJudge != "SS"  && sJudge != "TS" && sJudge != "WS" ) { g_objCommon.Show_Error(9105); return; }

	int nVNo = (sType == "B1" ? 1 : (sType == "AG" ? 2 : (sType == "T1" ? 3 : (sType == "TG" ? 4 : (sType == "T2" ? 5 : 0)))));


	if (pEquipData->bUseNGSize) { if (sJudge == "N" || sJudge == "X" || sJudge == "R") sJudge = "G"; }
	if (sType == "B1" || sType == "AG") { if (sNGCode == "MC" || sNGCode == "ROI_FAIL") sNGCode = "MCBTM"; }
	if (sNGCode.Left(5) == "FDFAI") { sNGCode = sNGCode.Right(sNGCode.GetLength()-1); gLot.nFOcapExist[nPortNo-1][nTrayNo-1][nCMNo-1] = 1; }

	

	if		(sJudge == "G")  gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = 2;	//Good
	else if (sJudge == "S")  gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = 7;	//치수불량1(7-FAI)
	else if (sJudge == "T")  gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = 8;	//치수불량2(Gap)
	else if (sJudge == "W")  gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = 9;	//치수불량3(Tilt)
	else if (sJudge == "SS") gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = 17;	//치수불량1 ROS_Skip
	else if (sJudge == "TS") gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = 18;	//치수불량2 ROS_Skip
	else if (sJudge == "WS") gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = 19;	//치수불량3 ROS_Skip
	else if (sJudge == "R")  gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = 4;	//외관불량(Ros Skip to Repair)
	else if (sJudge == "X")  gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = 5;	//외관불량(Ros Skip to NG)
	else					 gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = 3;	//외관불량(ROS 판정)
	



	//OCAP Count 
	if (sJudge == "G")
	{
		gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = "";
		strLog.Format("INSPECT,COMPLETE,GOOD,Empty");
		g_objLogFile.Save_SeqLog(strLog);
	}
	else
	{
		gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][nVNo] = sNGCode;
		/*if(sJudge == "S" || sJudge == "T" || sJudge == "W" || sJudge == "SS" || sJudge == "TS" || sJudge == "WS")
		{
			for(int i = 0; i < 50; i++)
			{
				if(sNGCode == gCap.sFAICode[i])
				{
					strLog.Format("INSPECT,COMPLETE,NG,%s",gCap.sFAICode[i]);
					g_objLogFile.Save_SeqLog(strLog);
				}
			}		
		}
		else
		{
			for(int i = 0; i < 50; i++)
			{
				if(sNGCode == gCap.sCosmeticCode[i])
				{
					strLog.Format("INSPECT,COMPLETE,NG,%s",gCap.sCosmeticCode[i]);
					g_objLogFile.Save_SeqLog(strLog);
				}
			}			
		}*/
	}
	
	
	int		nPreIdx = 0, nLaterIdx = 0;
	CString sPreCode="", sLaterCode="";

	//ADJ Written
	BOOL bOVerWritten = FALSE;
	if (sJudge != "G" && sNGCode.GetLength() > 0) 
	{
		sPreCode = gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0];
		for(int i = 0; i < 50; i++)
		{
			if(sPreCode =="")
			{
				gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = sNGCode;
				strLog.Format("INSPECT,COMPLETE, PC:%d, NORMAL_CASE:%s, PortNo:%d, TrayNo:%d, CMNo:%d",nVNo,gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0], nPortNo, nTrayNo, nCMNo);
				g_objLogFile.Save_OCAPTestLog(strLog);
				break;
			}

			if(gCap.sCosmeticCode[i] == sPreCode)
			{
				nPreIdx = i;				
				break;
			}
		}		

		for(int i = 0; i < 50; i++)
		{
			if(gCap.sCosmeticCode[i] == sNGCode)
			{
				nLaterIdx = i;
				if(nLaterIdx < nPreIdx)
				{
					gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = sNGCode;
					strLog.Format("INSPECT,COMPLETE,PC:%d, OVER_CASE:%s, PortNo:%d, TrayNo:%d, CMNo:%d",nVNo,gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0], nPortNo, nTrayNo, nCMNo);
					g_objLogFile.Save_OCAPTestLog(strLog);
					bOVerWritten = TRUE;
				}
				
			}
		}		
	}

	//Only Vision 
	BOOL bOVerWrittenV = FALSE;
	nPreIdx = 0, nLaterIdx = 0;
	sPreCode="", sLaterCode="";

	if (sNGCodeVision.GetLength() > 0) 
	{
		sPreCode = gLot.sNGCode_Vision[nPortNo-1][nTrayNo-1][nCMNo-1][0];
		for(int i = 0; i < 50; i++)
		{
			if(sPreCode =="")
			{
				gLot.sNGCode_Vision[nPortNo-1][nTrayNo-1][nCMNo-1][0] = sNGCodeVision;
				strLog.Format("INSPECT,COMPLETE,PC:%d,NORMAL_CASE_V:%s, PortNo:%d, TrayNo:%d, CMNo:%d",nVNo, gLot.sNGCode_Vision[nPortNo-1][nTrayNo-1][nCMNo-1][0], nPortNo, nTrayNo, nCMNo);
				g_objLogFile.Save_OCAPTestLog(strLog);
				break;
			}

			if(gCap.sCosmeticCode[i] == sPreCode)
			{
				nPreIdx = i;
				bOVerWrittenV = TRUE;
				break;
			}
		}		
				
		for(int i = 0; i < 50; i++)
		{
			if(gCap.sCosmeticCode[i] == sNGCodeVision)
			{
				 nLaterIdx = i;
				 if(nLaterIdx < nPreIdx)
				{
					gLot.sNGCode_Vision[nPortNo-1][nTrayNo-1][nCMNo-1][0] = sNGCodeVision;
					strLog.Format("INSPECT,COMPLETE,PC:%d,OVER_CASE_V:%s, PortNo:%d, TrayNo:%d, CMNo:%d",nVNo, gLot.sNGCode_Vision[nPortNo-1][nTrayNo-1][nCMNo-1][0], nPortNo, nTrayNo, nCMNo);
					g_objLogFile.Save_OCAPTestLog(strLog);
				}				 
			}
		}		
	}
	
	gLot.nImageCnt[nPortNo-1][nTrayNo-1][nCMNo-1][1] = gLot.nImageCnt[nPortNo-1][nTrayNo-1][nCMNo-1][1] + nImage1;	//치수불량수
	gLot.nImageCnt[nPortNo-1][nTrayNo-1][nCMNo-1][0] = gLot.nImageCnt[nPortNo-1][nTrayNo-1][nCMNo-1][0] + nImage2;	//외관불량수
	
	if (nVNo >=1 && nVNo <=5) {
		for (int i = 0; i < 20; i++) AfxExtractSubString(gNG->sNGCode[nPortNo-1][0][nCMNo-1][nVNo-1][i], sRcvData, i + 12, ',');
	}
	
	if (nMarginal > 0) gLot.nMarginal[nPortNo-1][nCMNo-1] = nVNo;	//Marginal

	//Module 검사완료 Check
	if (pEquipData->bUseBottom && gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][1] < 2) return;
	if (pEquipData->bUseBotAng && gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][2] < 2) return;
	if (pEquipData->bUseTop1   && gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][3] < 2) return;
	if (pEquipData->bUseTopAng && gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][4] < 2) return;
	if (pEquipData->bUseTop2   && gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][5] < 2) return;

	if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] > 1) {
		strLog.Format("[Get_InspectComplete] Inspection Complete Already Error => LotID(%s) PortNo(%s) TrayNo(%s) CmNo(%d) Bar(%s) Judge(%d) NC(%s)", sLotID, sPortNo, sTrayNo, nCMNo, gLot.sBarCode[nPortNo-1][nTrayNo-1][nCMNo-1], gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0], gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0]);	g_objLogFile.Save_HandlerLog(strLog);
		return;
	}

	int nNGCnt = 0, nNGSize = 0, nNGSize1 = 0, nNGSize2 = 0, nNGSize3 = 0, nNGMC = 0, nNGGF = 0, nNGSkip = 0, nReSkip = 0, nMCBTM = 0, nNGSize1Sp = 0, nNGSize2Sp = 0, nNGSize3Sp = 0, nFaiFail = 0;
	for (int i = 1; i < 6; i++) {
		if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][i]  > 2) nNGCnt++;
		if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == 7) { nNGSize1++; gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i]; }
		if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == 8) { nNGSize2++; gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i]; }
		if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == 9) { nNGSize3++; gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i]; }
		if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == 17) { nNGSize1Sp++; gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i]; }
		if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == 18) { nNGSize2Sp++; gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i]; }
		if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == 19) { nNGSize3Sp++;  gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i]; }
		if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == 4) {  nReSkip++;  gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i]; }
		if (gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == 5) {  nNGSkip++;  gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i]; }
		if (gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == "MC" || gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == "ROI_FAIL") nNGMC++;
		if (gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == "GF" || gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == "GRAB_FAIL") nNGGF++;
		if (gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == "MCBTM") nMCBTM++;
		if (gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][i] == "FAI_RECOFAIL_SP") nFaiFail++;
		if (nNGMC > 0 || nNGGF > 0 || nMCBTM > 0 || nFaiFail > 0) {
			sType = (i == 1 ? "B1" : (i == 2 ? "AG" : (i == 3 ? "T1" : (i == 4 ? "TG" : (i == 5 ? "T2" : ""))))); 
		}
	}
	if		(nNGSize1Sp > 0) nNGSize = 17;
	else if (nNGSize2Sp > 0) nNGSize = 18;
	else if (nNGSize3Sp > 0) nNGSize = 19;
	else if	(nNGSize1 > 0)	 nNGSize = 7;
	else if (nNGSize2 > 0)	 nNGSize = 8;
	else if (nNGSize3 > 0)	 nNGSize = 9;

	DWORD dwTerm = GetTickCount() - m_dwT2ScanDone[nPortNo-1][nTrayNo-1][nCMNo-1];
	strLog.Format("[Get_InspectComplete] Inspection Complete LotID(%s) PortNo(%s) TrayNo(%s) CmNo(%d) Time(%d) NG(%d) Bar(%s) SNG(%d) MC(%d) GF(%d) SNG(%d) SRe(%d) MCBTM(%d) FDFAI(%d) NC(%s) SS(%d-%d-%d) FAIFail(%d) Marginal(%d)",
		sLotID, sPortNo, sTrayNo, nCMNo, dwTerm, nNGCnt, gLot.sBarCode[nPortNo-1][nTrayNo-1][nCMNo-1], nNGSize, nNGMC, nNGGF, nNGSkip, nReSkip, nMCBTM, gLot.nFOcapExist[nPortNo-1][nTrayNo-1][nCMNo-1], gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0], nNGSize1Sp, nNGSize2Sp, nNGSize3Sp, nFaiFail, gLot.nMarginal[nPortNo-1][nCMNo-1]);	g_objLogFile.Save_HandlerLog(strLog);


#ifndef AJIN_BOARD_USE
	//pass
#else
	//Barcdoe read fail(5) - ROS Skip
	gAlm.sAlmLotID[0] = sLotID; gAlm.sAlmLotID[1] = sType;
	if (gLot.sBarCode[nPortNo-1][nTrayNo-1][nCMNo-1] == "NOREAD" || gLot.sBarCode[nPortNo-1][nTrayNo-1][nCMNo-1] == "BARCODE_NOREAD" || gLot.sBarCode[nPortNo-1][nTrayNo-1][nCMNo-1].GetLength() < 15) {
		gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0]  = 12;	//BARCODE_NOREAD
		gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = "BARCODE_NOREAD";
		gLot.nRosJugCount[nPortNo-1][5]++; gLot.nRosJugCount[nPortNo-1][7]++;
		gData.nNG_MC[2][1]++;
		if (gData.nNG_MC[2][0] > 0 && gData.nNG_MC[2][0] <= gData.nNG_MC[2][1]) g_objCommon.Show_Error(9203);
		return;
	}


	gData.nNG_MC[2][1] = 0;
	gData.nNG_MC[3][1] = 0;
	if (nNGMC > 0) gData.nNG_MC[0][1]++;
	else		   gData.nNG_MC[0][1] = 0;
	if (nNGGF > 0) gData.nNG_MC[1][1]++;
	else		   gData.nNG_MC[1][1] = 0;

	if (pEquipData->bUseMES)
	{
		int nFind = 0;
		for (int i = 0; i < 40; i++) { //허수배출
			if (gLot.sBarCode[nPortNo-1][nTrayNo-1][nCMNo-1] == gNG->sModuleID[nPortNo-1][i]) {
				nFind = i+1; break;
			}
		}
		if (nFind == 0) {
			gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0]  = 11;	//허수불량
			gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = "NO_MES_INFO";
			gLot.nRosJugCount[nPortNo-1][11]++;
			strLog.Format("[Get_InspectComplete] Inspection Complete MES_1 => LotID(%s) PortNo(%s) TrayNo(%s) CmNo(%d) Bar(%s) Judge(%d) NC(%s)", sLotID, sPortNo, sTrayNo, nCMNo, gLot.sBarCode[nPortNo-1][nTrayNo-1][nCMNo-1], gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0], gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0]);	g_objLogFile.Save_HandlerLog(strLog);
			gAlm.sAlmLotID[1] = gLot.sBarCode[nPortNo-1][nTrayNo-1][nCMNo-1];
			if (pEquipData->bUseOppAlarm) g_objCommon.Show_Error(9211);
			return;
		}

		//HAIM NG : 자공정에서 하임시스템이라고 액티브로 자동 실행해서 검사 진행하는데 거기서 발생한 NG
		if (gNG->sHaimNGCd[nPortNo-1][nFind-1].GetLength() > 0) {	//Haim_NG
			gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = gNG->sHaimNGCd[nPortNo-1][nFind-1];
			if		(nNGSize > 10) gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = nNGSize - 10;	//치수불량: 7,8,9
			else if (nNGSize > 6)  gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = nNGSize;		//치수불량: 7,8,9
			else				   gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = 3;				//불량차감
			if (nNGSize > 6) gLot.nRosJugCount[nPortNo-1][6]++;	//치수불량
			else			 gLot.nRosJugCount[nPortNo-1][12]++;
			strLog.Format("[Get_InspectComplete] Inspection Complete MES_2 => LotID(%s) PortNo(%s) TrayNo(%s) CmNo(%d) Bar(%s) Judge(%d) NC(%s)", sLotID, sPortNo, sTrayNo, nCMNo, gLot.sBarCode[nPortNo-1][nTrayNo-1][nCMNo-1], gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0], gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0]);	g_objLogFile.Save_HandlerLog(strLog);
			return;
		}
		if (gNG->sCosmeticNG[nPortNo-1][nFind-1] == "NG") {	//Cosemetic_NG
			gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0]  = 15;	//Cosemetic_NG
			gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = "VMAC000003";
			gLot.nRosJugCount[nPortNo-1][14]++;
			strLog.Format("[Get_InspectComplete] Inspection Complete MES_3 => LotID(%s) PortNo(%s) TrayNo(%s) CmNo(%d) Bar(%s) Judge(%d) NC(%s)", sLotID, sPortNo, sTrayNo, nCMNo, gLot.sBarCode[nPortNo-1][nTrayNo-1][nCMNo-1], gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0], gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0]);	g_objLogFile.Save_HandlerLog(strLog);
			return;
		}
	}

/*		//MES-NG - DFA는 없음
	if (pEquipData->bUseMES && gMes.nCmResult[nPortNo-1][nTrayNo-1][nCMNo-1] == 2) {
		gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0]  = 11;	//MES-NG
		gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = "MES_NG";
		gLot.nRosJugCount[nPortNo-1][5]++; gLot.nRosJugCount[nPortNo-1][8]++;
		if (gData.nNG_MC[4][0] > 0 && gData.nNG_MC[4][0] <= gData.nNG_MC[4][1]) g_objCommon.Show_Error(9205);
		return;
	} */
	


	if (nNGGF > 0) {
		gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0]  = 5;	//GRAB_FAIL
		gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = "GRAB_FAIL";
		gLot.nRosJugCount[nPortNo-1][5]++; gLot.nRosJugCount[nPortNo-1][9]++;
		if (gData.nNG_MC[1][0] > 0 && gData.nNG_MC[1][0] <= gData.nNG_MC[1][1]) g_objCommon.Show_Error(9202);
		return;
	}
	if (nFaiFail > 0) {
		gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = 5;	//FAI_RECOFAIL_SP
		gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = "FAI_RECOFAIL_SP";
		gLot.nRosJugCount[nPortNo-1][5]++; gLot.nRosJugCount[nPortNo-1][13]++;
		return;
	}
	//치수불량(10) - ROS Skip
	if (nNGSize > 10) {
		gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = nNGSize - 10;	//치수불량: 7,8,9
		gLot.nRosJugCount[nPortNo-1][6]++;
		return;
	}
	if (nNGSize > 0 && (nNGMC > 0 || nMCBTM > 0 || nNGSkip > 0 || nReSkip > 0))
	{
		gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = nNGSize;	//치수불량: 7,8,9
		gLot.nRosJugCount[nPortNo-1][6]++;
		return;
	}
	if (nNGMC > 0) {
		gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0]  = 4;	//ROI_FAIL
		gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = "ROI_FAIL";
		gLot.nRosJugCount[nPortNo-1][5]++; gLot.nRosJugCount[nPortNo-1][9]++;
		if (gData.nNG_MC[0][0] > 0 && gData.nNG_MC[0][0] <= gData.nNG_MC[0][1]) g_objCommon.Show_Error(9201);
		return;
	}
	if (nMCBTM > 0) {
		gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = 5;	//MCBTM
		gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = "MCBTM";
		gLot.nRosJugCount[nPortNo-1][5]++; gLot.nRosJugCount[nPortNo-1][9]++;
		return;
	}
	if (nNGSkip > 0) {
		gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = 14;	//SKIP_ROS-N(Special NG)
		gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = "Skip_ROS_N";
		gLot.nRosJugCount[nPortNo-1][10]++;
		return;
	}
	if (nReSkip > 0) {
		gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = 13;	//SKIP_ROS-R(Special NG)
		gLot.sNGCode_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = "Skip_ROS_R";
		gLot.nRosJugCount[nPortNo-1][10]++;
		return;
	}
	
#endif
	if		(nNGSize > 0)	{ gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = nNGSize;	}
	else if (nNGCnt == 0)	{ gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = 2;			}
	else					{ gLot.nJudge_I[nPortNo-1][nTrayNo-1][nCMNo-1][0] = 3;			}
	
	if (pEquipData->bUseROS && nNGCnt > 0) g_objDispatcher.Set_JudgeRequest(nPortNo, nTrayNo, nCMNo);
}

void CInspector::Get_AMoveRequest(int nInspector, CString sType, CString strZ)
{
	double dZ = atof(strZ);
	if (dZ < 0.0) { g_objCommon.Show_Error(9106); return; }
	if (sType != "B1" && sType != "AG" && sType != "T1" && sType != "TG" && sType != "T2") { g_objCommon.Show_Error(9107); return; }

	if (sType == "B1" || sType == "AG") { Set_MotionB1(nInspector, sType, dZ); return; }
	if (sType == "T1" || sType == "TG") { Set_MotionT1(nInspector, sType, dZ); return; }
	if (sType == "T2")					{ Set_MotionT2(nInspector, sType, dZ); return; }

/*
	if (sType == "T2") {
		if (g_dlgWork.IsWindowVisible()) {
			int nCase = g_objSequenceMain.Get_MainRunCase(AX_ANGLE1_Z1);
			if (nCase != 10) { Exception_Log("AMove Request", sType, nCase); return; }
			g_objSequenceMain.m_dBtmZ = dZ;
			g_objSequenceMain.Set_MainRunCase(AX_INSPECTION_Y1, 11);
		}
		if (g_dlgManual.IsWindowVisible()) {
			if (!g_objAJinAXL.Is_Home(AX_BTM_FOCUS_Z) || dZ < 0.0) return;
			g_objAJinAXL.Move_Absolute(AX_ANGLE1_Z1, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_ANGLE1_Z1, dZ)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_MoveComplete(nInspector, sType);
		}
*/
}

void CInspector::Get_BarcodeResult(int nInspector, CString sLotID, CString sPortNo, CString sTrayNo, CString sCMNo, CString sBarCode)
{
	int	nPortNo = atoi(sPortNo);
	int	nTrayNo = 1;
	int	nCMNo   = atoi(sCMNo);

	if (nPortNo < 1 || nPortNo > 30 || nCMNo < 1 || nCMNo > 40) { g_objCommon.Show_Error(9108); return; }
	if (sBarCode != "NOREAD" && sBarCode.GetLength() > 6) g_objMesAgent.Set_CmRequest(sLotID, sBarCode, nPortNo, nTrayNo, nCMNo);

	gLot.sBarCode[nPortNo-1][nTrayNo-1][nCMNo-1] = sBarCode;
	Set_BarcodeUpdate(INSPECTOR_PC1, sLotID, sPortNo, sTrayNo, sCMNo, sBarCode);
	Set_BarcodeUpdate(INSPECTOR_PC2, sLotID, sPortNo, sTrayNo, sCMNo, sBarCode);
	Set_BarcodeUpdate(INSPECTOR_PC4, sLotID, sPortNo, sTrayNo, sCMNo, sBarCode);
	if (nInspector == INSPECTOR_PC3) Set_BarcodeUpdate(INSPECTOR_PC5, sLotID, sPortNo, sTrayNo, sCMNo, sBarCode);
	if (nInspector == INSPECTOR_PC5) Set_BarcodeUpdate(INSPECTOR_PC3, sLotID, sPortNo, sTrayNo, sCMNo, sBarCode);

}

void CInspector::Get_ErrorRequest(int nInspector, CString sType, CString sLotId, CString sPortNo, CString sTrayNo, CString sCMNo, CString sErrNo, CString sErrMsg)
{
	int	nPortNo = atoi(sPortNo);
	int	nTrayNo = atoi(sTrayNo);
	int	nCMNo   = atoi(sCMNo);
	int	nErrNo  = atoi(sErrNo);

	CString strErrMsg, strVision;
	strVision = sType;
	if (sType == "B1") strVision = "Bottom";
	if (sType == "AG") strVision = "Bottom Angle";
	if (sType == "T1") strVision = "Top1";
	if (sType == "TG") strVision = "Top1 Angle";
	if (sType == "T2") strVision = "Top2";
	if (sType == "A1") strVision = "Align";
	strErrMsg.Format("# ==> LotID:%s, PortNo:%d, TrayNo:%d, Module:%d, Vision:%s# ==> Vision-Code[%04d] Message[%s]", sLotId, nPortNo, nTrayNo, nCMNo, strVision, nErrNo, sErrMsg);

	g_objCommon.Set_ErrorSubMessage(strErrMsg);
	g_objCommon.Show_Error(9190);
}

void CInspector::Get_HeartBeat(int nInspector)
{
	KillTimer(nInspector);
	if (nInspector == INSPECTOR_PC1) m_nStatusVisionPC1 = 1;
	if (nInspector == INSPECTOR_PC2) m_nStatusVisionPC2 = 1;
	if (nInspector == INSPECTOR_PC3) m_nStatusVisionPC3 = 1;
	if (nInspector == INSPECTOR_PC4) m_nStatusVisionPC4 = 1;
	if (nInspector == INSPECTOR_PC5) m_nStatusVisionPC5 = 1;
	SetTimer(nInspector, 15000, NULL);
}

void CInspector::Get_PositionRequest(int nInspector, CString sType)
{
	double dZ = 0.0;

	if (sType=="B1")  { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_BTM_FOCUS_Z); dZ = pStatusZ->dPos; }
	if (sType=="AG")  { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_BTM_FOCUS_Z); dZ = pStatusZ->dPos; }
	if (sType=="T1")  { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_TOP1_VISION_Z); dZ = pStatusZ->dPos; }
	if (sType=="TG")  { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_TOP1_VISION_Z); dZ = pStatusZ->dPos; }
	if (sType=="T2")  { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_TOP2_VISION_Z); dZ = pStatusZ->dPos; }

	Set_PositionReply(nInspector, sType, dZ);
}

void CInspector::Get_AlignComplete(int nInspector, CString sType, CString sRcvData)
{
	char chSep = ',';
	CString sLog, strArg[50];

	if (sType=="A1") {
		for (int i = 0; i < 43; i++) AfxExtractSubString(strArg[i], sRcvData, i + 6, chSep);
		if (strArg[0] == "N") { g_objCommon.Show_Error(9121);	return; }
		if (strArg[0] == "R") {
			gData.nLoadAlignRetry++;
			if (gData.nLoadAlignRetry > 3) { gData.nLoadAlignRetry = 0; g_objCommon.Show_Error(9121); return; }

			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_STAGE_1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_LOAD_STAGE_2);
			if (nCase1 > 11 && nCase1 < 19) g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_STAGE_1, 11);
			if (nCase2 > 11 && nCase2 < 19) g_objSequenceMain.Set_MainRunCase(AUTO_LOAD_STAGE_2, 11);
			
			sLog.Format("Load Align Compete Data[%s] => Seq(%d/%d) Retry(%d) Data[%s]", strArg[0], nCase1, nCase2, gData.nLoadAlignRetry, sRcvData);
			g_objLogFile.Save_HandlerLog(sLog);
			return;
		}
		gData.dAlignOffset[0][0] = atof(strArg[41]);	//X편차값
		gData.dAlignOffset[0][1] = atof(strArg[42]);	//Y편차값

		// (y,x) 0:Empty, 1:NG      2:Good
		int nx = 1, ny = 0;
		for(int i=0; i<10; i++) {
			ny = 3;
			for(int j=0; j<4; j++) {
				if		(strArg[nx] == "E") gData.InfoAlignTray[i][ny] = 0;
				else if (strArg[nx] == "G") gData.InfoAlignTray[i][ny] = 2;
				else						gData.InfoAlignTray[i][ny] = 1;
				nx++; ny--;
			}
		}
		g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, NULL);
		for(int i=0; i<10; i++) {
			for(int j=0; j<4; j++) {
				if (gData.InfoAlignTray[i][j] == 1) {
					g_objCommon.Show_Error(9122);	return;
				}
			}
		}
#ifdef ALIGN_TEST
		gData.bAlignA1Done = FALSE;
#else
		gData.bAlignA1Done = TRUE;
#endif
	}

	if (sType=="A2") {
		AfxExtractSubString(strArg[0], sRcvData, 5, chSep);
		AfxExtractSubString(strArg[1], sRcvData, 6, chSep);
		AfxExtractSubString(strArg[2], sRcvData, 7, chSep);
		AfxExtractSubString(strArg[3], sRcvData, 8, chSep);
		int	   nJob = atoi(strArg[0]);
		int	   nPos = atoi(strArg[1]);
		double dX   = atof(strArg[2]);
		double dY   = atof(strArg[3]);
		if (nJob < 1 || nPos > 4) { g_objCommon.Show_Error(9123); return; }
		if (nPos < 1 || nPos > 3) { g_objCommon.Show_Error(9124); return; }

		if ( (dX>0 && gData.dAlignX < dX) || (dX<0 && gData.dAlignX < dX*-1)) gData.dAlignVisionX[nJob-1][nPos-1] = 0.0;
		else																  gData.dAlignVisionX[nJob-1][nPos-1] = dX;
		if ( (dY>0 && gData.dAlignY < dY) || (dY<0 && gData.dAlignY < dY*-1)) gData.dAlignVisionY[nJob-1][nPos-1] = 0.0;
		else																  gData.dAlignVisionY[nJob-1][nPos-1] = dY;
		if (nPos < 3) gData.bAlignShipDone = TRUE;

		if (nPos == 3) {
			int nI = nJob - 1;
			sLog.Format("A2 Data[%d] => XY(%0.3lf,%0.3lf) 1(%0.3lf,%0.3lf) 2(%0.3lf,%0.3lf) 3(%0.3lf,%0.3lf) X(%0.3lf,%0.3lf)", nJob, dX, dY,
						gData.dAlignVisionX[nI][0], gData.dAlignVisionY[nI][0], gData.dAlignVisionX[nI][1], gData.dAlignVisionY[nI][1], gData.dAlignVisionX[nI][2], gData.dAlignVisionY[nI][2],
						gData.dAlignX, gData.dAlignY);
			g_objLogFile.Save_HandlerLog(sLog);
#ifdef ALIGN_TEST
			gData.bAlignShipDone = FALSE;
#else
			gData.bAlignShipDone = TRUE;
#endif
		}
	}
}

void CInspector::Get_LotEnd(int nInspector, CString sLotID, CString sPortNo, CString sDeepData1, CString sDeepData2, CString sDeepData3, CString sDeepData4, CString sDeepData5)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseMES) { gData.bInpectLotEnd = TRUE; return; }

	int nPortNo = atoi(sPortNo);
	if (nPortNo < 1 || nPortNo > 30)		{ g_objCommon.Show_Error(9131); return; }
	if (gLot.sLotID[nPortNo-1] != sLotID)	{ g_objCommon.Show_Error(9132); return; }

	g_objMesAgent.Set_LotEnd(nPortNo, sLotID, sDeepData1, sDeepData2, sDeepData3, sDeepData4, sDeepData5);
	gData.bInpectLotEnd = TRUE;
}

/////////////////////////////////////////////////////////////////////////////

int CInspector::Get_VisionStatus(int nInspector)
{
	switch (nInspector) {
	case INSPECTOR_PC1:	return m_nStatusVisionPC1;
	case INSPECTOR_PC2:	return m_nStatusVisionPC2;
	case INSPECTOR_PC3:	return m_nStatusVisionPC3;
	case INSPECTOR_PC4:	return m_nStatusVisionPC4;
	case INSPECTOR_PC5:	return m_nStatusVisionPC5;
	default:			return 0;
	}
}

BOOL CInspector::Check_LotReady()
{
#ifndef AJIN_BOARD_USE
	m_bLotReady1 == TRUE;
	m_bLotReady2 == TRUE;
	m_bLotReady3 == TRUE;
	return TRUE;
#else
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (pEquipData->bUseBottom && m_bLotReady1 == FALSE) return FALSE;
	if (pEquipData->bUseBotAng && m_bLotReady1 == FALSE) return FALSE;
	if (pEquipData->bUseTop1   && m_bLotReady2 == FALSE) return FALSE;
	if (pEquipData->bUseTopAng && m_bLotReady2 == FALSE) return FALSE;
	if (pEquipData->bUseTop2   && m_bLotReady3 == FALSE) return FALSE;
	//	if (pEquipData->bUseTop1   && m_bLotReady4 == FALSE) return FALSE;
	//	if (pEquipData->bUseTop2   && m_bLotReady5 == FALSE) return FALSE;
#endif


	
	return TRUE;
}

void CInspector::Get_RecipeBody(int nInspector, CString sType, CString sRcvData)
{
	//sData1 = "RECIPE,BODY,구분,Lot_ID,Recipe ID,Recipe Version,";
	CString sLog, strArg;

	int nNo = 9, nMax;
	if (sType=="T1M") { nNo = 0; nMax = 32; }
	if (sType=="T1S") { nNo = 1; nMax = 32; }
	if (sType=="T2M") { nNo = 2; nMax = 44; }
	if (sType=="T2S") { nNo = 3; nMax = 44; }
	if (nNo > 3) { g_objCommon.Show_Error(9133); return; }

	m_bBodyDone[nNo] = TRUE;
	AfxExtractSubString(strArg, sRcvData, 5, ',');
	if (strArg.GetLength() > 0) gMes.sRcpVersion = strArg;
	for (int i = 0; i < nMax; i++) AfxExtractSubString(gMes.sBodyData[nNo][i], sRcvData, i + 6, ',');
	
	if (m_bBodyDone[0] == TRUE && m_bBodyDone[1] == TRUE && m_bBodyDone[2] == TRUE && m_bBodyDone[3] == TRUE) {
		gMes.bRcpBodyDone = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Set Command

void CInspector::Set_ConnectRequest(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_ConnectReply(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,REPLY");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_ConnectEnd(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,END");
	Send_Command(nInspector, strSendCmd);

	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC1) m_bConnectVisionPC1 = FALSE;
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC2) m_bConnectVisionPC2 = FALSE;
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC3) m_bConnectVisionPC3 = FALSE;
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC4) m_bConnectVisionPC4 = FALSE;
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC5) m_bConnectVisionPC5 = FALSE;
}

void CInspector::Set_StatusRequest(int nInspector)
{
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC1) m_nStatusVisionPC1 = 0;
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC2) m_nStatusVisionPC2 = 0;
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC3) m_nStatusVisionPC3 = 0;
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC4) m_nStatusVisionPC4 = 0;
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC5) m_nStatusVisionPC5 = 0;

	CString	strSendCmd;
	strSendCmd.Format("STATUS,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_StatusReply(int nInspector, int nStatus)
{
	CString	strSendCmd;
	strSendCmd.Format("STATUS,REPLY,%d", nStatus);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_StatusUpdate(int nInspector, int nStatus)
{
	CString	strSendCmd;
	strSendCmd.Format("STATUS,UPDATE,%d", nStatus);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_LoadComplete(int nInspector, CString sType, CString sLotID, int nPortNo, int nTrayNo1, int nCMNo1, int nTrayNo2, int nCMNo2)
{
	if (nCMNo1 < 1) nCMNo1 = -1;
	if (nCMNo2 < 1) nCMNo2 = -1;
	if (nCMNo1 < 1) nTrayNo1 = -1;
	if (nCMNo2 < 1) nTrayNo2 = -1;

	int nScanRetry = 0;
	if (sType == "B1") { nScanRetry = gData.nScanRetry[0]; }
	if (sType == "AG") { nScanRetry = gData.nScanRetry[0]; }
	if (sType == "T1") { nScanRetry = gData.nScanRetry[1]; }
	if (sType == "TG") { nScanRetry = gData.nScanRetry[1]; }
	if (sType == "T2") { nScanRetry = gData.nScanRetry[2]; }

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	CString	strSendCmd, strTemp;	
	if ((sType == "B1" && !pEquipData->bUseBottom) ||
		(sType == "AG" && !pEquipData->bUseBotAng) ||
		(sType == "T1" && !pEquipData->bUseTop1)   ||
		(sType == "TG" && !pEquipData->bUseTopAng) ||
		(sType == "T2" && !pEquipData->bUseTop2)) {
		strTemp.Format("Set_LoadComplete Skip (%s)", sType);
		g_objLogFile.Save_HandlerLog(strTemp);
		return;
	}
	int nVNo = (sType == "B1" ? 1 : (sType == "AG" ? 2 : (sType == "T1" ? 3 : (sType == "TG" ? 4 : (sType == "T2" ? 5 : 0)))));
	if (nVNo == 0) {
		strTemp.Format("Set_LoadComplete Skip (%s) Type Error..", sType);
		g_objLogFile.Save_HandlerLog(strTemp);
		return;
	}
	m_bScanEnd[nVNo-1][0] = m_bScanEnd[nVNo-1][1] = FALSE;
	if (nCMNo1 == -1) m_bScanEnd[nVNo-1][0] = TRUE;
	if (nCMNo2 == -1) m_bScanEnd[nVNo-1][1] = TRUE;

	strSendCmd.Format("LOAD,COMPLETE,%s,%s,%d,%d,%d,%d,%d,%d", sType, sLotID, nPortNo, nTrayNo1, nTrayNo2, nCMNo1, nCMNo2, nScanRetry);
	if (nCMNo1 == -1 && nCMNo2 == -1) {
		if (sType == "B1") gData.bBTMScanDone = TRUE;
		if (sType == "AG") gData.bBTMScanDone = TRUE;
		if (sType == "T1") gData.bTop1ScanDone = TRUE; 
		if (sType == "TG") gData.bTop1ScanDone = TRUE; 
		if (sType == "T2") gData.bTop2ScanDone = TRUE;

		strTemp.Format("Set_LoadComplete Skip (%s)", strSendCmd);
		g_objLogFile.Save_HandlerLog(strTemp);
		return;
	}

	if (sType == "B1") { Send_Command(INSPECTOR_PC1, strSendCmd); gData.nScanRetry[0] = 0; }
	if (sType == "AG") { Send_Command(INSPECTOR_PC1, strSendCmd); gData.nScanRetry[0] = 0; }
	if (sType == "T1") { Send_Command(INSPECTOR_PC2, strSendCmd); Send_Command(INSPECTOR_PC4, strSendCmd); gData.nScanRetry[1] = 0; }
	if (sType == "TG") { Send_Command(INSPECTOR_PC2, strSendCmd); Send_Command(INSPECTOR_PC4, strSendCmd); gData.nScanRetry[1] = 0; }
	if (sType == "T2") { Send_Command(INSPECTOR_PC3, strSendCmd); Send_Command(INSPECTOR_PC5, strSendCmd); gData.nScanRetry[2] = 0; }
}

void CInspector::Set_MoveComplete(int nInspector, CString sType, double dZ)
{
	CString	strSendCmd;
	strSendCmd.Format("MOVE,COMPLETE,%s,%0.3lf", sType, dZ);
	Send_Command(nInspector, strSendCmd);
//	if (nInspector == INSPECTOR_PC2) Send_Command(INSPECTOR_PC4, strSendCmd);
//	if (nInspector == INSPECTOR_PC3) Send_Command(INSPECTOR_PC5, strSendCmd);
}

void CInspector::Set_PositionReply(int nInspector, CString sType, double dZ)
{
	CString	strSendCmd;
	strSendCmd.Format("POSITION,REPLY,%s,%0.3lf", sType, dZ);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_BarcodeUpdate(int nInspector, CString sLotID, CString sPortNo, CString sTrayNo, CString sCMNo, CString sBarCode)
{
	CString	strSendCmd, strTemp;
	strSendCmd.Format("BARCODE,UPDATE,%s,%s,%s,%s,%s", sLotID, sPortNo, sTrayNo, sCMNo, sBarCode);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_TimeUpdate(int nInspector)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d %03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	CString	strSendCmd;
	strSendCmd.Format("TIME,UPDATE,%s", strTime);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_InitialRequest(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("INITIAL,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_CycleStop(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CYCLE,STOP");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_LotStart(int nInspector, CString sLotID, int nPortNo, int nTrayCnt, int nCMCnt, CString sRcp, CString sPro, CString sPara, BOOL nMarginal)
{
	CString	strSendCmd;
	strSendCmd.Format("LOT,START,%s,%d,%d,%d,%s,%s,%s,%s,%s,%d", sLotID, nPortNo, nTrayCnt, nCMCnt, sRcp, sPro, sPara, gMes.sHostVendor, gMes.sHostConfig, nMarginal);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_LotEnd(int nInspector, CString sLotID, int nPortNo)
{
	CString	strSendCmd;
	strSendCmd.Format("LOT,END,%s,%d", sLotID, nPortNo);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_AlignRequest(int nInspector, CString sType, CString sLotID, int nPortNo, int nTrayNo, int nSeqNo)
{
	CString	strSendCmd, strTemp;
	strSendCmd.Format("ALIGN,REQUEST,%s,%s,%d,%d,%d", sType, sLotID, nPortNo, nTrayNo, nSeqNo);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_HistoryReply(int nInspector, CString sData)
{
	CString	strSendCmd;
	strSendCmd.Format("HISTORY,REPLY,%s", sData);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_ModuleData(int nInspector, CString sLotID, int nNo, CString sModuleID, CString sSite, CString sEqpID, CString sEqpName, CString sPara, CString sToolC, int nCount)
{
	CString	strLog, strSendCmd, sData;
	if (nNo < 1 || nNo > 40 || nCount < 1 || nCount > 40) {
		strLog.Format("Set_ModuleData Error ==> No(%d) Count(%d)", nNo, nCount);
		g_objLogFile.Save_InspectorLog(strLog);
		return;
	}
	if (nNo <= 1) {
		for (int i=0; i<40; i++) m_sModuleData[i] = "";
	}

	sData.Format("MODULE,DATA,%s,%d,%s,%s,%s,%s,%s,%s", sLotID, nNo, sModuleID, sSite, sEqpID, sEqpName, sPara, sToolC);
	m_sModuleData[nNo-1] = sData;

	if (nNo >= nCount) {
		int k = 0;
		strSendCmd = "";
		for (int i=0; i<nCount; i++) {
			k++;
			strSendCmd = strSendCmd + m_sModuleData[i] + "~";
			if (k == 8) { Send_Command(nInspector, strSendCmd); k = 0; strSendCmd = ""; Sleep(10); }
		}
		if (k > 0) { Send_Command(nInspector, strSendCmd); k = 0; }
	}
}

void CInspector::Set_InspectRequest(int nInspector, CString sType, CString sLotID, int nPortNo, int nTrayNo, int nCMNo)
{
	CString	strSendCmd;

	strSendCmd.Format("INSPECT,REQUEST,%s,%s,%d,%d,%d", sType, sLotID, nPortNo, nTrayNo, nCMNo);

	if (sType == "B1") { Send_Command(INSPECTOR_PC1, strSendCmd); }
	if (sType == "AG") { Send_Command(INSPECTOR_PC1, strSendCmd); }
	if (sType == "T1") { Send_Command(INSPECTOR_PC2, strSendCmd); Send_Command(INSPECTOR_PC4, strSendCmd); }
	if (sType == "TG") { Send_Command(INSPECTOR_PC2, strSendCmd); Send_Command(INSPECTOR_PC4, strSendCmd); }
	if (sType == "T2") { Send_Command(INSPECTOR_PC3, strSendCmd); Send_Command(INSPECTOR_PC5, strSendCmd); }
}

void CInspector::Set_RecipeRequest(int nType, CString sLotID, CString sRcpID)
{
	CString	strSendCmd;

	if (nType == 0) {
		m_bBodyDone[0] = m_bBodyDone[1] = m_bBodyDone[2] = m_bBodyDone[3] = FALSE;
		for(int i=0; i<4; i++) for(int j=0; j<50; j++) gMes.sBodyData[i][j] = "";

		strSendCmd.Format("RECIPE,REQUEST,T1M,%s,%s", sLotID, sRcpID); Send_Command(INSPECTOR_PC2, strSendCmd);
		strSendCmd.Format("RECIPE,REQUEST,T1S,%s,%s", sLotID, sRcpID); Send_Command(INSPECTOR_PC4, strSendCmd);
		strSendCmd.Format("RECIPE,REQUEST,T2M,%s,%s", sLotID, sRcpID); Send_Command(INSPECTOR_PC3, strSendCmd);
		strSendCmd.Format("RECIPE,REQUEST,T2S,%s,%s", sLotID, sRcpID); Send_Command(INSPECTOR_PC5, strSendCmd);
		return;
	}

	if (m_bBodyDone[0]==FALSE) { strSendCmd.Format("RECIPE,REQUEST,T1M,%s,%s", sLotID, sRcpID); Send_Command(INSPECTOR_PC2, strSendCmd); }
	if (m_bBodyDone[1]==FALSE) { strSendCmd.Format("RECIPE,REQUEST,T1S,%s,%s", sLotID, sRcpID); Send_Command(INSPECTOR_PC4, strSendCmd); }
	if (m_bBodyDone[2]==FALSE) { strSendCmd.Format("RECIPE,REQUEST,T2M,%s,%s", sLotID, sRcpID); Send_Command(INSPECTOR_PC3, strSendCmd); }
	if (m_bBodyDone[3]==FALSE) { strSendCmd.Format("RECIPE,REQUEST,T2S,%s,%s", sLotID, sRcpID); Send_Command(INSPECTOR_PC5, strSendCmd); }
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// UDP Socket Send Message
void CInspector::Send_Command(int nInspector, CString strSend)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	// Inspector Log //////////////////////////////////////
	CString strLog;
	strLog.Format("[H->V%d] , %s", nInspector, strSend);
	g_objLogFile.Save_InspectorLog(strLog);
	///////////////////////////////////////////////////////

	g_csInspector.Lock();	// Critical Section

	CString strSendSocket;
	strSendSocket.Format("@%s\n", strSend);

	char chSend[1024] = { 0 };
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC1) m_UdpVisionPC1.Write_Socket((BYTE*)chSend, nLength);
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC2) m_UdpVisionPC2.Write_Socket((BYTE*)chSend, nLength);
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC3) m_UdpVisionPC3.Write_Socket((BYTE*)chSend, nLength);
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC4) m_UdpVisionPC4.Write_Socket((BYTE*)chSend, nLength);
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC5) m_UdpVisionPC5.Write_Socket((BYTE*)chSend, nLength);
	g_csInspector.Unlock();	// Critical Section
}
/////////////////////////////////////////////////////////////////////////////

void CInspector::DoEvents(int nSleep)
{
	MSG msg;
	if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (nSleep > 0) Sleep(nSleep);
}

void CInspector::Exception_Log(CString sFunc, CString sGbn, int nCase)
{
	CString strLog;
	strLog.Format("%s Exception ==> Vision:%s, Case:%d", sFunc, sGbn, nCase);
	g_objLogFile.Save_InspectorLog(strLog);
}

void CInspector::Set_LotReady(BOOL bSetValue)
{
	m_bLotReady1 = m_bLotReady2 = m_bLotReady3 = m_bLotReady4 = m_bLotReady5 = bSetValue;
}

/////////////////////////////////////////////////////////////////////////////

void CInspector::Set_MotionB1(int nInspector, CString sType, double dData)
{
	m_nB1 = nInspector;
	m_sB1 = sType;
	m_dB1 = dData;
	AfxBeginThread(Thread_MotionB1, NULL);
}

void CInspector::Set_MotionT1(int nInspector, CString sType, double dData)
{
	m_nT1 = nInspector;
	m_sT1 = sType;
	m_dT1 = dData;
	AfxBeginThread(Thread_MotionT1, NULL);
}

void CInspector::Set_MotionT2(int nInspector, CString sType, double dData)
{
	m_nT2 = nInspector;
	m_sT2 = sType;
	m_dT2 = dData;
	AfxBeginThread(Thread_MotionT2, NULL);
}

UINT CInspector::Thread_MotionB1(LPVOID lpVoid)
{
	DWORD dwStart = GetTickCount();
	while (!g_objAJinAXL.Is_Done(AX_BTM_FOCUS_Z)) {
		if (GetTickCount() - dwStart > 3000) break;
		g_objInspector.DoEvents();
	}
	g_objAJinAXL.Move_Absolute(AX_BTM_FOCUS_Z, g_objInspector.m_dB1);
	dwStart = GetTickCount();
	while (!g_objAJinAXL.Is_MoveDone(AX_BTM_FOCUS_Z, g_objInspector.m_dB1)) {
		if (GetTickCount() - dwStart > 3000) break;
		g_objInspector.DoEvents();
	}
	double dCurrentPos = g_objAJinAXL.Get_Position(AX_BTM_FOCUS_Z);
	g_objInspector.Set_MoveComplete(g_objInspector.m_nB1, g_objInspector.m_sB1, dCurrentPos);
	return 0;
}

UINT CInspector::Thread_MotionT1(LPVOID lpVoid)
{
	DWORD dwStart = GetTickCount();
	while (!g_objAJinAXL.Is_Done(AX_TOP1_VISION_Z)) {
		if (GetTickCount() - dwStart > 3000) break;
		g_objInspector.DoEvents();
	}
	double dZOffset = g_objInspector.m_dT1;
/*
	if (gData.nVsNo[0] > 0 && gData.nVsNo[0] < 5 && gData.nVsIx[0] > 0 && gData.nVsIx[0] < 5) {
		int nx = gData.nVsNo[0]-1;
		int ny = gData.nVsIx[0]-1;
		if (nx >= 0 && nx <= 3 && ny >= 0 && ny <= 7) {
			dZOffset = g_objInspector.m_dT1 + gData.dFocusOffset[nx][ny];
		}
	}
*/
	g_objAJinAXL.Move_Absolute(AX_TOP1_VISION_Z, dZOffset);
	dwStart = GetTickCount();
	while (!g_objAJinAXL.Is_MoveDone(AX_TOP1_VISION_Z, dZOffset)) {
		if (GetTickCount() - dwStart > 3000)
		{
			break;
		}
		g_objInspector.DoEvents();
	}
	double dCurrentPos = g_objAJinAXL.Get_Position(AX_TOP1_VISION_Z);
	g_objInspector.Set_MoveComplete(g_objInspector.m_nT1, g_objInspector.m_sT1, dCurrentPos);
	return 0;
}

UINT CInspector::Thread_MotionT2(LPVOID lpVoid)
{
	DWORD dwStart = GetTickCount();
	while (!g_objAJinAXL.Is_Done(AX_TOP2_VISION_Z)) {
		if (GetTickCount() - dwStart > 3000) break;
		g_objInspector.DoEvents();
	}
	double dZOffset = g_objInspector.m_dT2;
/*
	if (gData.nVsNo[1] > 0 && gData.nVsNo[1] < 5 && gData.nVsIx[1] > 0 && gData.nVsIx[1] < 5) {
		int nx = gData.nVsNo[1]-1;
		int ny = gData.nVsIx[1]-1+4;
		if (nx >= 0 && nx <= 3 && ny >= 0 && ny <= 7) {
			dZOffset = g_objInspector.m_dT2 + gData.dFocusOffset[nx][ny];
		}
	}
*/
	g_objAJinAXL.Move_Absolute(AX_TOP2_VISION_Z, dZOffset);
	dwStart = GetTickCount();
	while (!g_objAJinAXL.Is_MoveDone(AX_TOP2_VISION_Z, dZOffset)) {
		if (GetTickCount() - dwStart > 3000) break;
		g_objInspector.DoEvents();
	}
	double dCurrentPos = g_objAJinAXL.Get_Position(AX_TOP2_VISION_Z);
	g_objInspector.Set_MoveComplete(g_objInspector.m_nT2, g_objInspector.m_sT2, dCurrentPos);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
void CInspector::Test_Command(int nInspector, CString sType, CString sRcvData)
{
	//sData1 = "RECIPE,BODY,구분,Lot_ID,Recipe ID,Recipe Version,";
	CString sLog, strArg;

	int nNo = 9, nMax;
	if (sType=="T1M") { nNo = 0; nMax = 32; }
	if (sType=="T1S") { nNo = 1; nMax = 32; }
	if (sType=="T2M") { nNo = 2; nMax = 44; }
	if (sType=="T2S") { nNo = 3; nMax = 44; }
	if (nNo > 3) { g_objCommon.Show_Error(9133); return; }

	m_bBodyDone[nNo] = TRUE;
	AfxExtractSubString(strArg, sRcvData, 5, ',');
	if (strArg.GetLength() > 0) gMes.sRcpVersion = strArg;
	for (int i = 0; i < nMax; i++) AfxExtractSubString(gMes.sBodyData[nNo][i], sRcvData, i + 6, ',');
	
	if (m_bBodyDone[0] == TRUE && m_bBodyDone[1] == TRUE && m_bBodyDone[2] == TRUE && m_bBodyDone[3] == TRUE) {
		gMes.bRcpBodyDone = TRUE;
	}
}
