// BarcodeLot_Cognex.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMAI2100.h"
#include "BarcodeLot_Cognex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "WorkDlg.h"

#include "CMAI2100Dlg.h"

#define COGNEX_IP_1		"192.168.0.31"	// Load C/V 2F IP
#define COGNEX_IP_2		"192.168.0.32"	// Load C/V 1F IP
#define COGNEX_IP_3		"192.168.0.33"	// Unload C/V 2F IP
#define COGNEX_IP_4		"192.168.0.34"	// Unload C/V 1F IP
#define COGNEX_PORT		9000			// Handler Port (Server)

#define LF	0x0A
#define CR	0x0D

CBarcodeLot_Cognex g_objBarcodeLot_Cognex;

IMPLEMENT_DYNAMIC(CBarcodeLot_Cognex, CWnd)

CBarcodeLot_Cognex::CBarcodeLot_Cognex()
{
	m_bConnected = FALSE;
	for (int i = 0; i < 4; i++) { m_strBarcode[i] = ""; m_bConnect[i] = FALSE; }
}

CBarcodeLot_Cognex::~CBarcodeLot_Cognex()
{
}

BEGIN_MESSAGE_MAP(CBarcodeLot_Cognex, CWnd)
	ON_MESSAGE(UM_SERVER_ACCEPT, &CBarcodeLot_Cognex::OnServerAccept)
	ON_MESSAGE(UM_SERVER_RECEIVE, &CBarcodeLot_Cognex::OnServerReceive)
	ON_MESSAGE(UM_SERVER_REMOVE, &CBarcodeLot_Cognex::OnServerRemove)
END_MESSAGE_MAP()

// CBarcodeLot_Cognex 메시지 처리기입니다.

/////////////////////////////////////////////////////////////////////////////
// Message Handlers

LRESULT CBarcodeLot_Cognex::OnServerAccept(WPARAM wClientIdx, LPARAM lServerPort)
{
	int nClient = (int)wClientIdx;
	CString strIP = "", strLog;
	UINT nPort = 0;

	if (!m_Server.Get_ClientInfo(nClient, strIP, nPort)) return 0;
	if (nClient >= 0 && nClient <= 3) m_bConnect[nClient] = TRUE;
	if (m_bConnect[0] && m_bConnect[1] && m_bConnect[2] && m_bConnect[3]) m_bConnected = TRUE;

	strLog.Format(_T("Cognex Barcode Connected. IP:%s, Client:%d"), strIP, nClient);
	g_objLogFile.Save_BarcodeLog(strLog);

	return 0;
}

LRESULT CBarcodeLot_Cognex::OnServerReceive(WPARAM wClientIdx, LPARAM lServerPort)
{
	int nClient = (int)wClientIdx;
	CString strIP = _T(""), strLog;
	UINT nPort = 0;

	if (!m_Server.Get_ClientInfo(nClient, strIP, nPort)) return 0;

	int nIdx = (strIP == COGNEX_IP_1 ? 0 : (strIP == COGNEX_IP_2 ? 1 : (strIP == COGNEX_IP_3 ? 2 : (strIP == COGNEX_IP_4 ? 3 : -1))));
	if (nIdx < 0) return 0;

	BYTE byRecv[1025] = { 0 };	// 마지막 0x00
	int nLen = m_Server.Read_Socket(nClient, byRecv);

	if (byRecv[nLen-2] != CR) return 0;	// Barcode + CR + LF

	CString strRecv, strBarcode;
	strRecv.Format("%s", byRecv);
	strBarcode = strRecv.Left(nLen - 2);

	m_strBarcode[nIdx] = strBarcode;
//	if (g_dlgWork.IsWindowVisible()) g_dlgWork.PostMessage(UM_UPDATE_BARCODE_COGNEX, nIdx+1, NULL);

	strLog.Format(_T("Cognex Barcode Recive. IP:%s, Client:%d, Barcode:%s"), strIP, nClient, strBarcode);
	g_objLogFile.Save_BarcodeLog(strLog);

	return 0;
}

LRESULT CBarcodeLot_Cognex::OnServerRemove(WPARAM wClientIdx, LPARAM lServerPort)
{
	int nClient = (int)wClientIdx;
	CString strIP = "", strLog;
	UINT nPort = 0;

	if (!m_Server.Get_ClientInfo(nClient, strIP, nPort)) return 0;
	m_bConnected = FALSE;

	strLog.Format(_T("Cognex Barcode Disconnected. IP:%s, Client:%d"), strIP, nClient);
	g_objLogFile.Save_BarcodeLog(strLog);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CBarcodeLot_Cognex::Initialize()
{
#ifndef AJIN_BOARD_USE
	m_bConnected = TRUE;
	for (int i = 0; i < 4; i++) m_nCarSeq[i] = 1;
#endif

	if (m_bConnected) return TRUE;

	for (int i = 0; i < 4; i++) m_bConnect[i] = FALSE;
	m_Server.Listen_Socket(COGNEX_PORT, this);

	CString strLog;
	strLog.Format(_T("Cognex Barcode Initialize(Listen_Socket).. Port:%d"), COGNEX_PORT);
	g_objLogFile.Save_BarcodeLog(strLog);

	return TRUE;
}

void CBarcodeLot_Cognex::Terminate()
{
	m_bConnected = FALSE;
	m_Server.Close_Socket();

	CString strLog;
	strLog.Format(_T("Cognex Barcode Terminate(Close_Socket).."));
	g_objLogFile.Save_BarcodeLog(strLog);
}

BOOL CBarcodeLot_Cognex::Set_Trigger(int nNo, BOOL bOn)
{
#ifndef AJIN_BOARD_USE
	if (bOn) {
		if (nNo == 1) m_strBarcode[0].Format("LDNGMGZ%02d", m_nCarSeq[0]++);
		if (nNo == 2) m_strBarcode[1].Format("LOADMGZ%02d", m_nCarSeq[1]++);
		if (nNo == 3) m_strBarcode[2].Format("ULNGMGZ%02d", m_nCarSeq[2]++);
		if (nNo == 4) m_strBarcode[3].Format("ULGDMGZ%02d", m_nCarSeq[3]++);
	}
	return TRUE;
#endif

	CString strIP = "", strLog;
	UINT nPort = 0;
	int nClient = -1;

	m_strBarcode[nNo-1] = "";
	CString strTarget = (nNo == 1 ? COGNEX_IP_1 : (nNo == 2 ? COGNEX_IP_2 : (nNo == 3 ? COGNEX_IP_3 : (nNo == 4 ? COGNEX_IP_4 : ""))));
	int nCount = m_Server.Get_ClientCount();

	for (int i = 0; i < nCount; i++) {
		if (!m_Server.Get_ClientInfo(i, strIP, nPort)) return FALSE;
		if (strIP == strTarget) { nClient = i; break; }
	}
	if (nClient < 0) return FALSE;

	CString strCommand = (bOn ? "||>TRIGGER ON\r\n" : "||>TRIGGER OFF\r\n");

	char chSend[2048] = { 0 };
	int nLength = strCommand.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strCommand, nLength);

	if (!m_Server.Write_Socket(nClient, (BYTE*)chSend, nLength)) return FALSE;

	CString strOn = (bOn ? "On" : "Off");
	strLog.Format(_T("Cognex Barcode Trigger %s. IP:%s, Client:%d"), strOn, strIP, nClient);
	g_objLogFile.Save_BarcodeLog(strLog);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
