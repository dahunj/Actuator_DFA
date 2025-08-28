// CarrierRFID_Good.h : 헤더 파일
//
#pragma once

class CCarrierRFID_Good : public CWnd
{
	DECLARE_DYNAMIC(CCarrierRFID_Good)

public:
	CCarrierRFID_Good();
	virtual ~CCarrierRFID_Good();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnClientConnect(WPARAM wConnect, LPARAM lParam);
	afx_msg LRESULT OnClientReceive(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClientClose(WPARAM wParam, LPARAM lParam);

private:
	CClientSocketCS m_Client;

	BOOL	m_bConnected;
	BOOL	m_bRecvComplete;
	CString m_strRecvCmd;
	CString m_strCarID;
	int		m_nCarSeq;
	BOOL	m_bConRequet;

public:
	void Initialize();
	void Terminate();

	BOOL Is_Connected() { return m_bConnected; }
	BOOL Is_RecvComplete() { return m_bRecvComplete; }
	CString Get_CarrierID() { return m_strCarID; }

	BOOL Send_RFIDRead();
};

extern CCarrierRFID_Good g_objCarrierRFID_Good;

///////////////////////////////////////////////////////////////////////////////
