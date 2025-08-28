// CarrierRFID_Good.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMAI2100.h"
#include "CarrierRFID_Good.h"

#include "LogFile.h"
#include "DataManager.h"
#include "WorkDlg.h"

#include "CMAI2100Dlg.h"

#define RFID_IP		"192.168.12.40"	// Good RFID Reader IP
#define RFID_PORT	5000			// RFID Port (Server)

CCarrierRFID_Good g_objCarrierRFID_Good;

IMPLEMENT_DYNAMIC(CCarrierRFID_Good, CWnd)

CCarrierRFID_Good::CCarrierRFID_Good()
{
	m_bConnected = FALSE;
	m_bRecvComplete = FALSE;
	m_strRecvCmd = "";
}

CCarrierRFID_Good::~CCarrierRFID_Good()
{
}

BEGIN_MESSAGE_MAP(CCarrierRFID_Good, CWnd)
	ON_MESSAGE(UM_CLIENT_CONNECT, OnClientConnect)
	ON_MESSAGE(UM_CLIENT_RECEIVE, OnClientReceive)
	ON_MESSAGE(UM_CLIENT_CLOSE, OnClientClose)
END_MESSAGE_MAP()

// CCarrierRFID_Good 메시지 처리기입니다.

/////////////////////////////////////////////////////////////////////////////
// Message Handlers

LRESULT CCarrierRFID_Good::OnClientConnect(WPARAM wConnect, LPARAM lParam)
{
	m_bConRequet = FALSE;
	m_bConnected = (BOOL)wConnect;
	if (!m_bConnected) return 0;

	return 0;
}

LRESULT CCarrierRFID_Good::OnClientClose(WPARAM wParam, LPARAM lParam)
{
	m_bConRequet = FALSE;
	m_bConnected = FALSE;
	m_Client.Close_Socket();

	return 0;
}

LRESULT CCarrierRFID_Good::OnClientReceive(WPARAM wClientIdx, LPARAM lServerPort)
{
	BYTE byRecv[1025] = { 0 };	// Buffer 1024, Last 0x00
	int nLen = m_Client.Read_Socket(byRecv);

	CString strRecvSocket, strLog;
	strRecvSocket.Format("%s", byRecv);
	m_strRecvCmd += strRecvSocket;

	while (!m_strRecvCmd.IsEmpty()) {
		int nLenth = m_strRecvCmd.GetLength();
		int nStart = m_strRecvCmd.Find("A");
		int nEnd = nStart + 20;

		if (nLenth < nEnd) break;	// 버퍼에 들어오는 중...

		if (nLenth > nEnd) {
			strLog.Format("[<-] : <<Error>> %s", m_strRecvCmd);
			m_strRecvCmd.Delete(nEnd, nLenth-nEnd);	// 쓰레기값이 채워져 있어서...
			continue;
		}
		if (nStart == -1) break;

		CString strRecv = m_strRecvCmd.Mid(nStart + 2, nLenth - 4);
		m_strRecvCmd.Delete(0, nEnd);
		strRecv.Trim();

		int nCnt = strRecv.Find(" ");
		if (nCnt > 0) m_strCarID = strRecv.Left(nCnt);
		else		  m_strCarID = strRecv;
		m_bRecvComplete = TRUE;

		// Inspector Log ////////////////////////////////////////////////////////////
		strLog.Format("[<-RFIDG] : %s", strRecv);
		g_objLogFile.Save_BarcodeLog(strLog);
		/////////////////////////////////////////////////////////////////////////////
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

void CCarrierRFID_Good::Initialize()
{
#ifndef AJIN_BOARD_USE
	m_bConnected = TRUE;
	m_nCarSeq = 1;
#endif

	if (m_bConnected) return;
	if (m_bConRequet) return;

	m_bConRequet = TRUE;
	m_Client.Open_Socket(RFID_IP, RFID_PORT, this);
}

void CCarrierRFID_Good::Terminate()
{
	m_bConnected = FALSE;
	m_Client.Close_Socket();
}

BOOL CCarrierRFID_Good::Send_RFIDRead()
{
#ifndef AJIN_BOARD_USE
	m_bRecvComplete = TRUE;
	m_strCarID.Format("GOODCAR%02d", m_nCarSeq++);
	return TRUE;
#endif

	CString strSendSocket, strLog;
	m_bRecvComplete = FALSE;
	if (!m_bConnected) return FALSE;

	char chSend[20];
	memset(chSend,0x20,sizeof(char) * 20);
	chSend[0] = 0x44;
	chSend[1] = 0x30;
	chSend[18] = 0x31;
	chSend[19] = 0x45;

	if (!m_Client.Write_Socket((BYTE*)chSend, 20)) return FALSE;

	strLog.Format(_T("[RFIDG->] : Load Carrier RFID Read Send."));
	g_objLogFile.Save_BarcodeLog(strLog);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
