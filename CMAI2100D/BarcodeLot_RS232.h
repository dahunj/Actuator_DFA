// BarcodeLot_RS232.h : 헤더 파일
//
#pragma once

#define BARCODE_BUFFER_MAX	1024

class CBarcodeLot_RS232 : public CWnd
{
	DECLARE_DYNAMIC(CBarcodeLot_RS232)

public:
	CBarcodeLot_RS232();
	virtual ~CBarcodeLot_RS232();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnPortReceive(WPARAM nPort, LPARAM lParam);

private:
	CComPortCS m_Port;

	BOOL	m_bConnected;
	BOOL	m_bReadOk;
	CString	m_strBarcodeLot;

	int		m_nRecvPos;
	char	m_chRecv[BARCODE_BUFFER_MAX];

public:
	BOOL Initialize();
	void Terminate();

	BOOL Is_Connected() { return m_bConnected; }

	CString Get_BarcodeLot() { return m_strBarcodeLot; }
};

extern CBarcodeLot_RS232 g_objBarcodeLot_RS232;

///////////////////////////////////////////////////////////////////////////////
