// BarcodeLot_Cognex.h : 헤더 파일
//
#pragma once

class CBarcodeLot_Cognex : public CWnd
{
	DECLARE_DYNAMIC(CBarcodeLot_Cognex)

public:
	CBarcodeLot_Cognex();
	virtual ~CBarcodeLot_Cognex();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnServerAccept(WPARAM wClientIdx, LPARAM lServerPort);
	afx_msg LRESULT OnServerReceive(WPARAM wClientIdx, LPARAM lServerPort);
	afx_msg LRESULT OnServerRemove(WPARAM wClientIdx, LPARAM lServerPort);

private:
	CServerSocketCS m_Server;
	
	BOOL		m_bConnected;
	BOOL		m_bConnect[4];
	CString		m_strBarcode[4];	// 0:Load C/V 2F, 1:Load C/V 1F, 2:Unload C/V 2F, 3:Unload C/V 1F
	int			m_nCarSeq[4];

public:
	BOOL Initialize();
	void Terminate();

	BOOL Is_Connected() { return m_bConnected; }
	CString Get_BarcodeLot(int nNo) { return m_strBarcode[nNo-1]; }

	BOOL Set_Trigger(int nNo, BOOL bOn);
};

extern CBarcodeLot_Cognex g_objBarcodeLot_Cognex;

///////////////////////////////////////////////////////////////////////////////
