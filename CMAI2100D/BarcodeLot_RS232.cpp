// BarcodeLot_RS232.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMAI2100.h"
#include "BarcodeLot_RS232.h"

#include "DataManager.h"
#include "WorkDlg.h"

#define LF				0x0A
#define CR				0x0D

CBarcodeLot_RS232 g_objBarcodeLot_RS232;

IMPLEMENT_DYNAMIC(CBarcodeLot_RS232, CWnd)

CBarcodeLot_RS232::CBarcodeLot_RS232()
{
	m_bConnected = FALSE;
	m_strBarcodeLot = "";
	m_nRecvPos = 0;
	memset(m_chRecv, 0x00, BARCODE_BUFFER_MAX);
}

CBarcodeLot_RS232::~CBarcodeLot_RS232()
{
}

BEGIN_MESSAGE_MAP(CBarcodeLot_RS232, CWnd)
	ON_MESSAGE(UM_PORT_RECEIVE, OnPortReceive)
END_MESSAGE_MAP()

// CBarcodeLot_RS232 메시지 처리기입니다.

/////////////////////////////////////////////////////////////////////////////
// Serial Port Receive Event

LRESULT CBarcodeLot_RS232::OnPortReceive(WPARAM nPort, LPARAM lParam)
{
	if (m_Port.Get_PortNum() != (int)nPort) return 0;

	BYTE byRecv[BARCODE_BUFFER_MAX] = { 0 };
	int nLen = m_Port.Read_Byte(byRecv);

	if (nLen < 1) return 0;

	int nTotalLen = m_nRecvPos + nLen;
	if (nTotalLen >= BARCODE_BUFFER_MAX || nTotalLen < 3) {

		m_strBarcodeLot = "";
		m_nRecvPos = 0;
		memset(m_chRecv, 0x00, BARCODE_BUFFER_MAX);
		return 0;
	}

	memcpy(m_chRecv + m_nRecvPos, byRecv, nLen);
	m_nRecvPos += nLen;

	if (m_chRecv[m_nRecvPos - 1] == CR) {
		CString strTemp = (CString)m_chRecv;
		int nCR = strTemp.Find(CR);

		m_strBarcodeLot = strTemp.Left(nCR);
		m_nRecvPos = 0;
		memset(m_chRecv, 0x00, BARCODE_BUFFER_MAX);

		g_dlgWork.PostMessage(UM_UPDATE_BARCODE_RS232, NULL, NULL);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CBarcodeLot_RS232::Initialize()
{
	if (m_bConnected) return TRUE;

	//if (!m_Port.Open_Port(nPort, 9600, 8, 0, 0, this)) return FALSE;	// cino
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
//	if (!m_Port.Open_Port(pEquipData->nLotBarcodePort, 9600, 8, 1, 0, this)) return FALSE;	// hanywell
	if (!m_Port.Open_Port(pEquipData->nLotBarcodePort, 115200, 8, 1, 0, this)) return FALSE;	// hanywell
	m_bConnected = TRUE;
	return TRUE;
}

void CBarcodeLot_RS232::Terminate()
{
	m_bConnected = FALSE;
	m_Port.Close_Port();
}

///////////////////////////////////////////////////////////////////////////////
