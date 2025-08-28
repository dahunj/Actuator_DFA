// AJinAXL.h : 헤더 파일
//
#pragma once

// *** AJin Configuration ***************************
// Motion Board    : RTEX
// In / Out Board  : RTEX
// Library Version : EzSoftwareUC V4.2.0.4042, 32Bit Version
// ********************************************************

#include "AJinDefine.h"

class CAJinAXL
{
public:
	CAJinAXL();
	virtual ~CAJinAXL();

private:
	DX_DATA_00	m_DX00;					// Input (X0000 - X0031)
	DX_DATA_01	m_DX01;					// Input (X0100 - X0131)
	DX_DATA_02	m_DX02;					// Input (X0200 - X0231)
	DX_DATA_03	m_DX03;					// Input (X0300 - X0331)
	DX_DATA_04	m_DX04;					// Input (X0400 - X0431)
	DX_DATA_05	m_DX05;					// Input (X0500 - X0531)
	DX_DATA_06	m_DX06;					// Input (X0600 - X0631)
	DX_DATA_07	m_DX07;					// Input (X0700 - X0731)
	DX_DATA_08	m_DX08;					// Input (X0800 - X0831)
	DX_DATA_09	m_DX09;					// Input (X0900 - X0931)
	DX_DATA_10	m_DX10;					// Input (X1000 - X1031)
	DX_DATA_11	m_DX11;					// Input (X1100 - X1131)
	DX_DATA_12	m_DX12;					// Input (X1200 - X1231)
	DX_DATA_13	m_DX13;					// Input (X1300 - X1331)
	DX_DATA_14	m_DX14;					// Input (X1400 - X1431)
	DX_DATA_15	m_DX15;					// Input (X1500 - X1531)
	DX_DATA_16	m_DX16;					// Input (X1600 - X1631)
	DX_DATA_17	m_DX17;					// Input (X1700 - X1731)
	DX_DATA_18	m_DX18;					// Input (X1800 - X1831)
	DX_DATA_19	m_DX19;					// Input (X1900 - X1931)
	DX_DATA_20	m_DX20;					// Input (X2000 - X2031)

	DY_DATA_00	m_DY00;					// Output (Y0000 - Y0031)
	DY_DATA_01	m_DY01;					// Output (Y0100 - Y0131)
	DY_DATA_02	m_DY02;					// Output (Y0200 - Y0231)
	DY_DATA_03	m_DY03;					// Output (Y0300 - Y0331)
	DY_DATA_04	m_DY04;					// Output (Y0400 - Y0431)
	DY_DATA_05	m_DY05;					// Output (Y0500 - Y0531)
	DY_DATA_06	m_DY06;					// Output (Y0600 - Y0631)
	DY_DATA_07	m_DY07;					// Output (Y0700 - Y0731)
	DY_DATA_08	m_DY08;					// Output (Y0800 - Y0831)
	DY_DATA_09	m_DY09;					// Output (Y0900 - Y0931)
	DY_DATA_10	m_DY10;					// Output (Y1000 - Y1031)
	DY_DATA_11	m_DY11;					// Output (Y1100 - Y1131)
	DY_DATA_12	m_DY12;					// Output (Y1200 - Y1231)
	DY_DATA_13	m_DY13;					// Output (Y1300 - Y1331)
	DY_DATA_14	m_DY14;					// Output (Y1400 - Y1431)
	DY_DATA_15	m_DY15;					// Output (Y1500 - Y1531)
	DY_DATA_16	m_DY16;					// Output (Y1600 - Y1631)
	DY_DATA_17	m_DY17;					// Output (Y1700 - Y1731)
	DY_DATA_18	m_DY18;					// Output (Y1800 - Y1831)
	DY_DATA_19	m_DY19;					// Output (Y1900 - Y1931)
	DY_DATA_20	m_DY20;					// Output (Y2000 - Y2031)

	AXIS_STATUS	m_Status[AXIS_COUNT];		// Axis Status Data
	AXIS_PARAM	m_Param[AXIS_COUNT];		// Axis Parameter Data
	CString		m_strAxisName[AXIS_COUNT];	// Axis Name

	BOOL		m_bReadVelocity;		// Setup Mode 에서만 읽기

protected:
	BOOL		m_bThreadAJin;
	CWinThread *m_pThreadAJin;
	static UINT	Thread_AJin(LPVOID lpVoid);

	void Sim_SetMotion(int nNo, int nAxis, double dPos);
	void Sim_SetOutToIn(int nNo);

public:
	BOOL Initialize();
	void Terminate();

	BOOL Device_Check();
	BOOL Device_Rescan();	// 전원 차단->복구 후 재검색

	void Read_Input();					// DI Read
	void Read_Output();					// DO Read
	void Write_Output(int nModule);		// DO Write

	void Read_MotionStatus();			// Motion Status
	void Set_ReadVelocity(BOOL bRead) { m_bReadVelocity = bRead; }

	void Set_ServoOn(int nAxis);
	void Set_ServoOff(int nAxis);
	void Set_EncoderType(int nAxis, int nType);
	BOOL Get_EncoderTypeInc(int nAxis);
	BOOL Get_EncoderTypeAbs(int nAxis);
	void Home_Search(int nAxis);
	void Set_Home(int nAxis);
	void Move_Absolute(int nAxis, double dPos, double dSpeed=1.0);
	void Move_Relative(int nAxis, double dPos, double dSpeed=1.0);
	void Jog_Positive(int nAxis, double dSpeed=0.0);
	void Jog_Negative(int nAxis, double dSpeed=0.0);
	void Stop_Motion(int nAxis);
	void EStop_Motion(int nAxis);
	void Alarm_Reset(int nAxis);

	void Move_Abs_Override(int nAxis, double dPos, double dAt, double dSpeed=0.5);
	void Move_Rel_Override(int nAxis, double dPos, double dAt);

	BOOL Is_Done(int nAxis);
	BOOL Is_Home(int nAxis);
	BOOL Is_MoveDone(int nAxis, double dPos, double dRange = 0.1);
	void Set_CVActPos(int nAxis); 

	void Start_Trigger(int nAxis, double dStartPos, double dEndPos, double dPeriod, double dWidth);
	void Stop_Trigger(int nAxis);

	void Start_Scan(int nAxis, double dPos, double dVel, double dTrigStart, double dTrigEnd, double dPeriod, double dWidth);
	void Stop_Scan(int nAxis);

	void Read_AxisList();	// Axis Name & Param
	void Read_MotionParam(int nAxis);
	void Save_MotionParam(int nAxis, double dSpeedM, double dSpeedJ, double dAccel);

	BOOL Is_ServoAxis(int nAxis);
	BOOL Use_OrgAxis(int nAxis);
	BOOL Use_ElpAxis(int nAxis);
	BOOL Use_ElnAxis(int nAxis);

	DXY_DATA *Get_pDX(int nIndex);
	DXY_DATA *Get_pDY(int nIndex);

	DX_DATA_00 *Get_pDX00() { return &m_DX00; }
	DX_DATA_01 *Get_pDX01() { return &m_DX01; }
	DX_DATA_02 *Get_pDX02() { return &m_DX02; }
	DX_DATA_03 *Get_pDX03() { return &m_DX03; }
	DX_DATA_04 *Get_pDX04() { return &m_DX04; }
	DX_DATA_05 *Get_pDX05() { return &m_DX05; }
	DX_DATA_06 *Get_pDX06() { return &m_DX06; }
	DX_DATA_07 *Get_pDX07() { return &m_DX07; }
	DX_DATA_08 *Get_pDX08() { return &m_DX08; }
	DX_DATA_09 *Get_pDX09() { return &m_DX09; }
	DX_DATA_10 *Get_pDX10() { return &m_DX10; }
	DX_DATA_11 *Get_pDX11() { return &m_DX11; }
	DX_DATA_12 *Get_pDX12() { return &m_DX12; }
	DX_DATA_13 *Get_pDX13() { return &m_DX13; }
	DX_DATA_14 *Get_pDX14() { return &m_DX14; }
	DX_DATA_15 *Get_pDX15() { return &m_DX15; }
	DX_DATA_16 *Get_pDX16() { return &m_DX16; }
	DX_DATA_17 *Get_pDX17() { return &m_DX17; }
	DX_DATA_18 *Get_pDX18() { return &m_DX18; }
	DX_DATA_19 *Get_pDX19() { return &m_DX19; }
	DX_DATA_20 *Get_pDX20() { return &m_DX20; }

	DY_DATA_00 *Get_pDY00() { return &m_DY00; }
	DY_DATA_01 *Get_pDY01() { return &m_DY01; }
	DY_DATA_02 *Get_pDY02() { return &m_DY02; }
	DY_DATA_03 *Get_pDY03() { return &m_DY03; }
	DY_DATA_04 *Get_pDY04() { return &m_DY04; }
	DY_DATA_05 *Get_pDY05() { return &m_DY05; }
	DY_DATA_06 *Get_pDY06() { return &m_DY06; }
	DY_DATA_07 *Get_pDY07() { return &m_DY07; }
	DY_DATA_08 *Get_pDY08() { return &m_DY08; }
	DY_DATA_09 *Get_pDY09() { return &m_DY09; }
	DY_DATA_10 *Get_pDY10() { return &m_DY10; }
	DY_DATA_11 *Get_pDY11() { return &m_DY11; }
	DY_DATA_12 *Get_pDY12() { return &m_DY12; }
	DY_DATA_13 *Get_pDY13() { return &m_DY13; }
	DY_DATA_14 *Get_pDY14() { return &m_DY14; }
	DY_DATA_15 *Get_pDY15() { return &m_DY15; }
	DY_DATA_16 *Get_pDY16() { return &m_DY16; }
	DY_DATA_17 *Get_pDY17() { return &m_DY17; }
	DY_DATA_18 *Get_pDY18() { return &m_DY18; }
	DY_DATA_19 *Get_pDY19() { return &m_DY19; }
	DY_DATA_20 *Get_pDY20() { return &m_DY20; }

	AXIS_STATUS *Get_pStatus(int nAxis) { return &m_Status[nAxis]; }
	AXIS_PARAM *Get_pParam(int nAxis) { return &m_Param[nAxis]; }

	CString Get_AxisName(int nAxis) { return m_strAxisName[nAxis]; }
	double Get_Position(int nAxis) { return m_Status[nAxis].dPos; }
	BOOL   Get_ServoOn(int nAxis) { return m_Status[nAxis].bSOn; }
	BOOL   Get_AmpAlarm(int nAxis) { return m_Status[nAxis].bALM; }
	BOOL   Get_EndLimitPos(int nAxis) { return m_Status[nAxis].bELP; }
	BOOL   Get_EndLimitNeg(int nAxis) { return m_Status[nAxis].bELN; }
	BOOL   Get_HomeDone(int nAxis) { return m_Status[nAxis].bHom; }
	BOOL   Get_MotorRun(int nAxis) { return m_Status[nAxis].bRun; }
};

extern CAJinAXL g_objAJinAXL;

///////////////////////////////////////////////////////////////////////////////
