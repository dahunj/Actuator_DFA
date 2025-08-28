// LoopTimer.cpp : 구현 파일
//
#include "StdAfx.h"
#include "LoopTimer.h"

#include "LogFile.h"
#include "DataManager.h"

CLoopTimer::CLoopTimer()
{
	m_dwLoopStart = 0;
	m_dwLoopTerm = 0;

	m_bWaitingRun = FALSE;
	m_dwWaitingStart = 0;
}

CLoopTimer::~CLoopTimer()
{
}

void CLoopTimer::Set_LoopTime(DWORD msec)
{
	m_dwLoopStart = GetTickCount();
	m_dwLoopTerm = msec;
}

BOOL CLoopTimer::Over_LoopTime()
{
	//return FALSE;	// No Timeout Error

	DWORD dwTerm = GetTickCount() - m_dwLoopStart;
	if (dwTerm >= m_dwLoopTerm)
		return TRUE;
	else
		return FALSE;
}

BOOL CLoopTimer::Delay_LoopTime(DWORD msec)
{
	DWORD dwTerm = GetTickCount() - m_dwLoopStart;
	if (dwTerm >= msec) return TRUE;
	else return FALSE;
}

///////////////////////////////////////////////////////////////////////////////

BOOL CLoopTimer::Waiting_Time(DWORD msec)
{
	if (!m_bWaitingRun) {
		m_bWaitingRun = TRUE;
		m_dwWaitingStart = GetTickCount();
	}

	DWORD dwTerm = GetTickCount() - m_dwWaitingStart;
	if (dwTerm < msec) return FALSE;
	
	m_bWaitingRun = FALSE;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

void CLoopTimer::Takt_Start()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseTaktLog) return;

	m_dwTakt = GetTickCount();
}

void CLoopTimer::Takt_Save(int nFun, int nId)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseTaktLog) return;

	double dPos = 0.0;
	int nSeqTotal = 0;
	CString strFun, strLog, strMsg, strInfo, strLotId;
	switch (nFun) {
	case 1:
		strFun = "Elevator_1"; nSeqTotal = 4;
		switch (nId) {
		case  1: strMsg = "Elevator_1,High Sensor On First Move,start"; break;
		case  2: strMsg = "Elevator_1,High Sensor Off Move,ing"; break;
		case  3: strMsg = "Elevator_1,High Sensor On Last Move,ing"; break;
		case  4: strMsg = "Elevator_1,Elevator Post Position Move,end"; break;
		}
		break;
	case 2:
		strFun = "Elevator_2"; nSeqTotal = 4;
		switch (nId) {
		case  1: strMsg = "Elevator_2,High Sensor On First Move,start"; break;
		case  2: strMsg = "Elevator_2,High Sensor Off Move,ing"; break;
		case  3: strMsg = "Elevator_2,High Sensor On Last Move,ing"; break;
		case  4: strMsg = "Elevator_2,Elevator Post Position Move,end"; break;
		}
		break;
	case 3:
		strFun = "Elevator_3"; nSeqTotal = 4;
		switch (nId) {
		case  1: strMsg = "Elevator_3,High Sensor On First Move,start"; break;
		case  2: strMsg = "Elevator_3,High Sensor Off Move,ing"; break;
		case  3: strMsg = "Elevator_3,High Sensor On Last Move,ing"; break;
		case  4: strMsg = "Elevator_3,Elevator Post Position Move,end"; break;
		}
		break;
	case 4:
		strFun = "Elevator_4"; nSeqTotal = 4;
		switch (nId) {
		case  1: strMsg = "Elevator_4,High Sensor On First Move,start"; break;
		case  2: strMsg = "Elevator_4,High Sensor Off Move,ing"; break;
		case  3: strMsg = "Elevator_4,High Sensor On Last Move,ing"; break;
		case  4: strMsg = "Elevator_4,Elevator Post Position Move,end"; break;
		}
		break;
	case 5:
		strFun = "Elevator_5"; nSeqTotal = 4;
		switch (nId) {
		case  1: strMsg = "Elevator_5,High Sensor On First Move,start"; break;
		case  2: strMsg = "Elevator_5,High Sensor Off Move,ing"; break;
		case  3: strMsg = "Elevator_5,High Sensor On Last Move,ing"; break;
		case  4: strMsg = "Elevator_5,Elevator Post Position Move,end"; break;
		}
		break;
	case 6:
		strFun = "Elevator_6"; nSeqTotal = 4;
		switch (nId) {
		case  1: strMsg = "Elevator_6,High Sensor On First Move,start"; break;
		case  2: strMsg = "Elevator_6,High Sensor Off Move,ing"; break;
		case  3: strMsg = "Elevator_6,High Sensor On Last Move,ing"; break;
		case  4: strMsg = "Elevator_6,Elevator Post Position Move,end"; break;
		}
		break;
	case 7:
		strFun = "Elevator_7"; nSeqTotal = 4;
		switch (nId) {
		case  1: strMsg = "Elevator_7,High Sensor On First Move,start"; break;
		case  2: strMsg = "Elevator_7,High Sensor Off Move,ing"; break;
		case  3: strMsg = "Elevator_7,High Sensor On Last Move,ing"; break;
		case  4: strMsg = "Elevator_7,Elevator Post Position Move,end"; break;
		}
		break;
	case 8:
		strFun = "Transfer_1"; nSeqTotal = 39;
		switch (nId) {
		case  1: strMsg = "Load Stage Up,X1 Axis Move to Work Position,start"; break;
		case  2: strMsg = "Load Stage Up,X1 Axis Wait for Work,ing"; break;
		case  3: strMsg = "Load Stage Up,Z1 Axis Move to Down Position,ing"; break;
		case  4: strMsg = "Load Stage Up,Grib Close,ing"; break;
		case  5: strMsg = "Load Stage Up,Z1 Axis Move to Ready Up Position,ing"; break;
		case  6: strMsg = "Load Lot1 Up,X1 Axis Wait for Load1,ing"; break;
		case  7: strMsg = "Load Lot1 Up,Elevator1 Clamp Off,ing"; break;
		case  8: strMsg = "Load Lot1 Up,X1 Axis Wait for Load1,ing"; break;
		case  9: strMsg = "Load Lot1 Up,Z1 Axis Move to Load1 Down Position,ing"; break;
		case 10: strMsg = "Load Lot1 Up,Grib Close,ing"; break;
		case 11: strMsg = "Load Lot1 Up,Z1 Axis Move to Ready Up Position,ing"; break;
		case 12: strMsg = "Load Lot2 Up,X1 Axis Wait for Load2,ing"; break;
		case 13: strMsg = "Load Lot2 Up,Elevator2 Clamp Off,ing"; break;
		case 14: strMsg = "Load Lot2 Up,X1 Axis Wait for Load2,ing"; break;
		case 15: strMsg = "Load Lot2 Up,Z1 Axis Move to Load2 Down Position,ing"; break;
		case 16: strMsg = "Load Lot2 Up,Grib Close,ing"; break;
		case 17: strMsg = "Load Lot2 Up,Z1 Axis Move to Ready Up Position,ing"; break;
		case 18: strMsg = "Load Stage Down,X1 Axis Wait for Stage,ing"; break;
		case 19: strMsg = "Load Stage Down,X1 Axis Move to Stage Position,ing"; break;
		case 20: strMsg = "Load Stage Down,Z1 Axis Move to Stage Down Position,ing"; break;
		case 21: strMsg = "Load Stage Down,Grib Close,ing"; break;
		case 22: strMsg = "Load Stage Down,Z1 Axis Move to Ready Up Position,ing"; break;
		case 23: strMsg = "Empty NG Down,X1 Axis Wait for Empty-NG,ing"; break;
		case 24: strMsg = "Empty NG Down,X1 Axis Wait for Empty-NG,ing"; break;
		case 25: strMsg = "Empty NG Down,X1 Axis Move to Load2 Position,ing"; break;
		case 26: strMsg = "Empty NG Down,X1 Axis Move to Empty-NG Position,ing"; break;
		case 27: strMsg = "Empty NG Down,X1 Axis Wait for Empty-NG Elveator,ing"; break;
		case 28: strMsg = "Empty NG Down,Z1 Axis Move to Empty-NG Down Position,ing"; break;
		case 29: strMsg = "Empty NG Down,Grib Open,ing"; break;
		case 30: strMsg = "Empty NG Down,Z1 Axis Move to Ready Up Position,ing"; break;
		case 31: strMsg = "Empty Good Down,X1 Axis Wait for Empty-Good,ing"; break;
		case 32: strMsg = "Empty Good Down,X1 Axis Wait for Empty-Good,ing"; break;
		case 33: strMsg = "Empty Good Down,X1 Axis Move to Load2 Position,ing"; break;
		case 34: strMsg = "Empty Good Down,X1 Axis Move to Empty-Good Position,ing"; break;
		case 35: strMsg = "Empty Good Down,X1 Axis Wait for Empty-Good Elveator,ing"; break;
		case 36: strMsg = "Empty Good Down,Z1 Axis Move to Empty-Good Down Position,ing"; break;
		case 37: strMsg = "Empty Good Down,Grib Open,ing"; break;
		case 38: strMsg = "Empty Good Down,Z1 Axis Move to Ready Up Position,ing"; break;
		case 39: strMsg = "Transfer_1,Seq End,end"; break;
		}
		break;
	case 9:
		strFun = "Transfer_2"; nSeqTotal = 43;
		switch (nId) {
		case  1: strMsg = "Transfer_2,X2 Axis Move to Work-Up Position,start"; break;
		case  2: strMsg = "Empty NG Up,X2 Axis Wait for Empty-NG,ing"; break;
		case  3: strMsg = "Empty NG Up,Z2 Axis Move to Empty-NG Position,ing"; break;
		case  4: strMsg = "Empty NG Up,Grib Close,ing"; break;
		case  5: strMsg = "Empty NG Up,Z2 Axis Move to Ready Up Position,ing"; break;
		case  6: strMsg = "Empty NG Up,Elevator3 Clamp Off,ing"; break;
		case  7: strMsg = "Empty Good Up,X2 Axis Wait for Empty-Good,ing"; break;
		case  8: strMsg = "Empty Good Up,Z2 Axis Move to Empty-Good Position,ing"; break;
		case  9: strMsg = "Empty Good Up,Grib Close,ing"; break;
		case 10: strMsg = "Empty Good Up,Z2 Axis Move to Ready Up Position,ing"; break;
		case 11: strMsg = "Empty Good Up,Elevator4 Clamp Off,ing"; break;
		case 12: strMsg = "Buffer NG Up,X2 Axis Wait for Buffer-NG,ing"; break;
		case 13: strMsg = "Buffer NG Up,Z2 Axis Move to Buffer-NG Position,ing"; break;
		case 14: strMsg = "Buffer NG Up,Grib Close,ing"; break;
		case 15: strMsg = "Buffer NG Up,Z2 Axis Move to Ready Up Position,ing"; break;
		case 16: strMsg = "Buffer NG Up,Elevator5 Clamp Off,ing"; break;
		case 17: strMsg = "NG Stage Up,X2 Axis Wait for NG-Stage,ing"; break;
		case 18: strMsg = "NG Stage Up,Z2 Axis Move to NG-Stage Position,ing"; break;
		case 19: strMsg = "NG Stage Up,Grib Close,ing"; break;
		case 20: strMsg = "NG Stage Up,Z2 Axis Move to Ready Up Position,ing"; break;
		case 21: strMsg = "Good Stage Up,X2 Axis Wait for Good-Stage,ing"; break;
		case 22: strMsg = "Good Stage Up,Z2 Axis Move to Good-Stage Position,ing"; break;
		case 23: strMsg = "Good Stage Up,Grib Close,ing"; break;
		case 24: strMsg = "Good Stage Up,Z2 Axis Move to Ready Up Position,ing"; break;
		case 25: strMsg = "Transfer_2,X2 Axis Wait for Work,ing"; break;
		case 26: strMsg = "Transfer_2,X2 Axis Move to Work-Down Position,ing"; break;
		case 27: strMsg = "Buffer NG Down,X2 Axis Wait for Buffer-NG,ing"; break;
		case 28: strMsg = "Buffer NG Down,Z2 Axis Move to Buffer-NG Position,ing"; break;
		case 29: strMsg = "Buffer NG Down,Grib Open,ing"; break;
		case 30: strMsg = "Buffer NG Down,Z2 Axis Move to Ready Up Position,ing"; break;
		case 31: strMsg = "Lot Unload Down,X2 Axis Wait for Unload,ing"; break;
		case 32: strMsg = "Lot Unload Down,Z2 Axis Move to Unload Position,ing"; break;
		case 33: strMsg = "Lot Unload Down,Grib Open,ing"; break;
		case 34: strMsg = "Lot Unload Down,Z2 Axis Move to Ready Up Position,ing"; break;
		case 35: strMsg = "NG Stage Down,X2 Axis Wait for NG-Stage,ing"; break;
		case 36: strMsg = "NG Stage Down,Z2 Axis Move to NG-Stage Position,ing"; break;
		case 37: strMsg = "NG Stage Down,Grib Open,ing"; break;
		case 38: strMsg = "NG Stage Down,Z2 Axis Move to Ready Up Position,ing"; break;
		case 39: strMsg = "Good Stage Down,X2 Axis Wait for Good-Stage,ing"; break;
		case 40: strMsg = "Good Stage Down,Z2 Axis Move to Good-Stage Position,ing"; break;
		case 41: strMsg = "Good Stage Down,Grib Open,ing"; break;
		case 42: strMsg = "Good Stage Down,Z2 Axis Move to Ready Up Position,ing"; break;
		case 43: strMsg = "Transfer_2,Seq End,end"; break;
		}
		break;
	case 10:
		strFun = "LoadStage_1"; nSeqTotal = 19;
		switch (nId) {
		case  1: strMsg = "Load Stage1,Master In,start"; break;
		case  2: strMsg = "Load Stage1,Slave In,ing"; break;
		case  3: strMsg = "Load Stage1,Align Wait,ing"; break;
		case  4: strMsg = "Load Stage1,Y1 Axis Move to Align1 Position,ing"; break;
		case  5: strMsg = "Load Stage1,Y1 Axis Align1 Scan Complete,ing"; break;
		case  6: strMsg = "Load Stage1,Y1 Axis Move to Align2 Position,ing"; break;
		case  7: strMsg = "Load Stage1,Y1 Axis Align2 Scan Complete,ing"; break;
		case  8: strMsg = "Load Stage1,Y1 Axis Move to Align3 Position,ing"; break;
		case  9: strMsg = "Load Stage1,Y1 Axis Align3 Scan Done,ing"; break;
		case 10: strMsg = "Load Stage1,Y1 Axis Move to Wait Position,ing"; break;
		case 11: strMsg = "Load Stage1,Y1 Axis Move to Unload Position,ing"; break;
		case 12: strMsg = "Load Stage1,Wait for Load Picker Work,ing"; break;
		case 13: strMsg = "Load Stage1,Y1 Stage Down,ing"; break;
		case 14: strMsg = "Load Stage1,Y1 Axis Move to Load Position,ing"; break;
		case 15: strMsg = "Load Stage1,Wait for Align,ing"; break;
		case 16: strMsg = "Load Stage1,Y1 Stage Up,ing"; break;
		case 17: strMsg = "Load Stage1,Slave Out,ing"; break;
		case 18: strMsg = "Load Stage1,Master Out,ing"; break;
		case 19: strMsg = "Load Stage1,Seq End,end"; break;
		}
		break;
	case 11:
		strFun = "LoadStage_2"; nSeqTotal = 19;
		switch (nId) {
		case  1: strMsg = "Load Stage2,Master In,start"; break;
		case  2: strMsg = "Load Stage2,Slave In,ing"; break;
		case  3: strMsg = "Load Stage2,Align Wait,ing"; break;
		case  4: strMsg = "Load Stage2,Y2 Axis Move to Align1 Position,ing"; break;
		case  5: strMsg = "Load Stage2,Y2 Axis Align1 Scan Complete,ing"; break;
		case  6: strMsg = "Load Stage2,Y2 Axis Move to Align2 Position,ing"; break;
		case  7: strMsg = "Load Stage2,Y2 Axis Align2 Scan Complete,ing"; break;
		case  8: strMsg = "Load Stage2,Y2 Axis Move to Align3 Position,ing"; break;
		case  9: strMsg = "Load Stage2,Y2 Axis Align3 Scan Done,ing"; break;
		case 10: strMsg = "Load Stage2,Y2 Axis Move to Wait Position,ing"; break;
		case 11: strMsg = "Load Stage2,Y2 Axis Move to Unload Position,ing"; break;
		case 12: strMsg = "Load Stage2,Wait for Load Picker Work,ing"; break;
		case 13: strMsg = "Load Stage2,Y2 Stage Down,ing"; break;
		case 14: strMsg = "Load Stage2,Y2 Axis Move to Load Position,ing"; break;
		case 15: strMsg = "Load Stage2,Wait for Align,ing"; break;
		case 16: strMsg = "Load Stage2,Y2 Stage Up,ing"; break;
		case 17: strMsg = "Load Stage2,Slave Out,ing"; break;
		case 18: strMsg = "Load Stage2,Master Out,ing"; break;
		case 19: strMsg = "Load Stage2,Seq End,end"; break;
		}
		break;
	case 12:
		strFun = "Load_Picker_1"; nSeqTotal = 32;
		switch (nId) {
		case  1: strMsg = "Load Picker1,Load Stage Y Axis Move to 1 Position,start"; break;
		case  2: strMsg = "Load Picker1,Load Picker 1-4 Down,ing"; break;
		case  3: strMsg = "Load Picker1,Load Picker Z Axis Move to Down Position,ing"; break;
		case  4: strMsg = "Load Picker1,Load Picker 1-4 Close,ing"; break;
		case  5: strMsg = "Load Picker1,Load Picker 1-4 Up,ing"; break;
		case  6: strMsg = "Load Picker1,Load Picker X Axis Wait,ing"; break;
		case  7: strMsg = "Load Picker1,Load Picker X Axis Move to 5 Position,ing"; break;
		case  8: strMsg = "Load Picker1,Load Picker Z Axis Move to Down Position,ing"; break;
		case  9: strMsg = "Load Picker1,Load Picker 5-8 Close,ing"; break;
		case 10: strMsg = "Load Picker1,Load Picker 5-8 Up,ing"; break;
		case 11: strMsg = "Load Picker1,Move Wait to BTM,ing"; break;
		case 12: strMsg = "Load Picker1,BTM Angle Y Move Postion,ing"; break;
		case 13: strMsg = "Load Picker1,BTM Light-Focus Move Postion,ing"; break;
		case 14: strMsg = "Load Picker1,BTM Pitch Move,ing"; break;
		case 15: strMsg = "Load Picker1,BTM Pitch Scan Complete,ing"; break;
		case 16: strMsg = "Load Picker1,BTM Angle Light-Focus Move Postion,ing"; break;
		case 17: strMsg = "Load Picker1,BTM Angle Y Move Position,ing"; break;
		case 18: strMsg = "Load Picker1,BTM Angle Pitch Move,ing"; break;
		case 19: strMsg = "Load Picker1,BTM Angle Pitch Scan Complete,ing"; break;
		case 20: strMsg = "Load Picker1,Move Wait to Align,ing"; break;
		case 21: strMsg = "Load Picker1,Load Picker X Axis Move to Align Position,ing"; break;
		case 22: strMsg = "Load Picker1,Move Wait to Inspection Stage,ing"; break;
		case 23: strMsg = "Load Picker1,Load Picker Down,ing"; break;
		case 24: strMsg = "Load Picker1,Load Picker Z Axis Down Position,ing"; break;
		case 25: strMsg = "Load Picker1,Load Picker Open,ing"; break;
		case 26: strMsg = "Load Picker1,Load Picker Up,ing"; break;
		case 27: strMsg = "Load Picker1,Align Master In,ing"; break;
		case 28: strMsg = "Load Picker1,Align Slave In,ing"; break;
		case 29: strMsg = "Load Picker1,Align Slave Out,ing"; break;
		case 30: strMsg = "Load Picker1,Align Master Out,ing"; break;
		case 31: strMsg = "Load Picker1,Load Picker Y Axis Ready Position,ing"; break;
		case 32: strMsg = "Load Picker1,Load Picker X Axis Load Position,end"; break;
		}
		break;
	case 13:
		strFun = "Load_Picker_2"; nSeqTotal = 32;
		switch (nId) {
		case  1: strMsg = "Load Picker2,Load Stage Y Axis Move to 1 Position,start"; break;
		case  2: strMsg = "Load Picker2,Load Picker 1-4 Down,ing"; break;
		case  3: strMsg = "Load Picker2,Load Picker Z Axis Move to Down Position,ing"; break;
		case  4: strMsg = "Load Picker2,Load Picker 1-4 Close,ing"; break;
		case  5: strMsg = "Load Picker2,Load Picker 1-4 Up,ing"; break;
		case  6: strMsg = "Load Picker2,Load Picker X Axis Wait,ing"; break;
		case  7: strMsg = "Load Picker2,Load Picker X Axis Move to 5 Position,ing"; break;
		case  8: strMsg = "Load Picker2,Load Picker Z Axis Move to Down Position,ing"; break;
		case  9: strMsg = "Load Picker2,Load Picker 5-8 Close,ing"; break;
		case 10: strMsg = "Load Picker2,Load Picker 5-8 Up,ing"; break;
		case 11: strMsg = "Load Picker2,Move Wait to BTM,ing"; break;
		case 12: strMsg = "Load Picker2,BTM Angle Y Move Postion,ing"; break;
		case 13: strMsg = "Load Picker2,BTM Light-Focus Move Postion,ing"; break;
		case 14: strMsg = "Load Picker2,BTM Pitch Move,ing"; break;
		case 15: strMsg = "Load Picker2,BTM Pitch Scan Complete,ing"; break;
		case 16: strMsg = "Load Picker2,BTM Angle Light-Focus Move Postion,ing"; break;
		case 17: strMsg = "Load Picker2,BTM Angle Y Move Position,ing"; break;
		case 18: strMsg = "Load Picker2,BTM Angle Pitch Move,ing"; break;
		case 19: strMsg = "Load Picker2,BTM Angle Pitch Scan Complete,ing"; break;
		case 20: strMsg = "Load Picker2,Move Wait to Align,ing"; break;
		case 21: strMsg = "Load Picker2,Load Picker X Axis Move to Align Position,ing"; break;
		case 22: strMsg = "Load Picker2,Move Wait to Inspection Stage,ing"; break;
		case 23: strMsg = "Load Picker2,Load Picker Down,ing"; break;
		case 24: strMsg = "Load Picker2,Load Picker Z Axis Down Position,ing"; break;
		case 25: strMsg = "Load Picker2,Load Picker Open,ing"; break;
		case 26: strMsg = "Load Picker2,Load Picker Up,ing"; break;
		case 27: strMsg = "Load Picker2,Align Master In,ing"; break;
		case 28: strMsg = "Load Picker2,Align Slave In,ing"; break;
		case 29: strMsg = "Load Picker2,Align Slave Out,ing"; break;
		case 30: strMsg = "Load Picker2,Align Master Out,ing"; break;
		case 31: strMsg = "Load Picker2,Load Picker Y Axis Ready Position,ing"; break;
		case 32: strMsg = "Load Picker2,Load Picker X Axis Load Position,end"; break;
		}
		break;
	case 14:
		strFun = "Inspect_Stage_1"; nSeqTotal = 28;
		switch (nId) {
		case  1: strMsg = "Inspect Stage1,X1 Axis Move To Top1 Position,start"; break;
		case  2: strMsg = "Inspect Stage1,Wait for Top1 Scan,ing"; break;
		case  3: strMsg = "Inspect Stage1,X1 Axis Move To Top1 Pitch,ing"; break;
		case  4: strMsg = "Inspect Stage1,X1 Top1 Scan Complete,ing"; break;
		case  5: strMsg = "Inspect Stage1,Wait for Top1 Angle Scan,ing"; break;
		case  6: strMsg = "Inspect Stage1,X1 Axis Move To Top1 Angle Pitch,ing"; break;
		case  7: strMsg = "Inspect Stage1,X1 Top1 Angle Scan Complete,ing"; break;
		case  8: strMsg = "Inspect Stage1,Wait for Move Top2,ing"; break;
		case  9: strMsg = "Inspect Stage1,X1 Axis Move To Top2 Position,ing"; break;
		case 10: strMsg = "Inspect Stage1,Wait for Top2 Scan,ing"; break;
		case 11: strMsg = "Inspect Stage1,X1 Axis Move To Top2 Pitch,ing"; break;
		case 12: strMsg = "Inspect Stage1,X1 Top2 Scan Complete,ing"; break;
		case 13: strMsg = "Inspect Stage1,Wait for Move Buffer,ing"; break;
		case 14: strMsg = "Inspect Stage1,X1 Axis Move To Buffer Position,ing"; break;
		case 15: strMsg = "Inspect Stage1,Wait for Move Unload,ing"; break;
		case 16: strMsg = "Inspect Stage1,X1 Axis Move To Unload Position,ing"; break;
		case 17: strMsg = "Inspect Stage1,Wait for Work Unload_Picker,ing"; break;
		case 18: strMsg = "Inspect Stage1,X1 Stage Down,ing"; break;
		case 19: strMsg = "Inspect Stage1,X1 Stage Out,ing"; break;
		case 20: strMsg = "Inspect Stage1,Wait for Move Down-Top2,ing"; break;
		case 21: strMsg = "Inspect Stage1,X1 Axis Move Back Top2 Position,ing"; break;
		case 22: strMsg = "Inspect Stage1,Wait for Move Down-Top1,ing"; break;
		case 23: strMsg = "Inspect Stage1,X1 Axis Move Back Top1 Position,ing"; break;
		case 24: strMsg = "Inspect Stage1,Wait for Move Down-Align,ing"; break;
		case 25: strMsg = "Inspect Stage1,X1 Axis Move Back Align Position,ing"; break;
		case 26: strMsg = "Inspect Stage1,Wait for Stage In,ing"; break;
		case 27: strMsg = "Inspect Stage1,X1 Stage In,ing"; break;
		case 28: strMsg = "Inspect Stage1,X1 Stage Up,end"; break;
		}
		break;
	case 15:
		strFun = "Inspect_Stage_2"; nSeqTotal = 28;
		switch (nId) {
		case  1: strMsg = "Inspect Stage2,X2 Axis Move To Top1 Position,start"; break;
		case  2: strMsg = "Inspect Stage2,Wait for Top1 Scan,ing"; break;
		case  3: strMsg = "Inspect Stage2,X2 Axis Move To Top1 Pitch,ing"; break;
		case  4: strMsg = "Inspect Stage2,X2 Top1 Scan Complete,ing"; break;
		case  5: strMsg = "Inspect Stage2,Wait for Top1 Angle Scan,ing"; break;
		case  6: strMsg = "Inspect Stage2,X2 Axis Move To Top1 Angle Pitch,ing"; break;
		case  7: strMsg = "Inspect Stage2,X2 Top1 Angle Scan Complete,ing"; break;
		case  8: strMsg = "Inspect Stage2,Wait for Move Top2,ing"; break;
		case  9: strMsg = "Inspect Stage2,X2 Axis Move To Top2 Position,ing"; break;
		case 10: strMsg = "Inspect Stage2,Wait for Top2 Scan,ing"; break;
		case 11: strMsg = "Inspect Stage2,X2 Axis Move To Top2 Pitch,ing"; break;
		case 12: strMsg = "Inspect Stage2,X2 Top2 Scan Complete,ing"; break;
		case 13: strMsg = "Inspect Stage2,Wait for Move Buffer,ing"; break;
		case 14: strMsg = "Inspect Stage2,X2 Axis Move To Buffer Position,ing"; break;
		case 15: strMsg = "Inspect Stage2,Wait for Move Unload,ing"; break;
		case 16: strMsg = "Inspect Stage2,X2 Axis Move To Unload Position,ing"; break;
		case 17: strMsg = "Inspect Stage2,Wait for Work Unload_Picker,ing"; break;
		case 18: strMsg = "Inspect Stage2,X2 Stage Down,ing"; break;
		case 19: strMsg = "Inspect Stage2,X2 Stage Out,ing"; break;
		case 20: strMsg = "Inspect Stage2,Wait for Move Down-Top2,ing"; break;
		case 21: strMsg = "Inspect Stage2,X2 Axis Move Back Top2 Position,ing"; break;
		case 22: strMsg = "Inspect Stage2,Wait for Move Down-Top1,ing"; break;
		case 23: strMsg = "Inspect Stage2,X2 Axis Move Back Top1 Position,ing"; break;
		case 24: strMsg = "Inspect Stage2,Wait for Move Down-Align,ing"; break;
		case 25: strMsg = "Inspect Stage2,X2 Axis Move Back Align Position,ing"; break;
		case 26: strMsg = "Inspect Stage2,Wait for Stage In,ing"; break;
		case 27: strMsg = "Inspect Stage2,X2 Stage In,ing"; break;
		case 28: strMsg = "Inspect Stage2,X2 Stage Up,end"; break;
		}
		break;
	case 16:
		strFun = "Inspect_Stage_3"; nSeqTotal = 28;
		switch (nId) {
		case  1: strMsg = "Inspect Stage3,X3 Axis Move To Top1 Position,start"; break;
		case  2: strMsg = "Inspect Stage3,Wait for Top1 Scan,ing"; break;
		case  3: strMsg = "Inspect Stage3,X3 Axis Move To Top1 Pitch,ing"; break;
		case  4: strMsg = "Inspect Stage3,X3 Top1 Scan Complete,ing"; break;
		case  5: strMsg = "Inspect Stage3,Wait for Top1 Angle Scan,ing"; break;
		case  6: strMsg = "Inspect Stage3,X3 Axis Move To Top1 Angle Pitch,ing"; break;
		case  7: strMsg = "Inspect Stage3,X3 Top1 Angle Scan Complete,ing"; break;
		case  8: strMsg = "Inspect Stage3,Wait for Move Top2,ing"; break;
		case  9: strMsg = "Inspect Stage3,X3 Axis Move To Top2 Position,ing"; break;
		case 10: strMsg = "Inspect Stage3,Wait for Top2 Scan,ing"; break;
		case 11: strMsg = "Inspect Stage3,X3 Axis Move To Top2 Pitch,ing"; break;
		case 12: strMsg = "Inspect Stage3,X3 Top2 Scan Complete,ing"; break;
		case 13: strMsg = "Inspect Stage3,Wait for Move Buffer,ing"; break;
		case 14: strMsg = "Inspect Stage3,X3 Axis Move To Buffer Position,ing"; break;
		case 15: strMsg = "Inspect Stage3,Wait for Move Unload,ing"; break;
		case 16: strMsg = "Inspect Stage3,X3 Axis Move To Unload Position,ing"; break;
		case 17: strMsg = "Inspect Stage3,Wait for Work Unload_Picker,ing"; break;
		case 18: strMsg = "Inspect Stage3,X3 Stage Down,ing"; break;
		case 19: strMsg = "Inspect Stage3,X3 Stage Out,ing"; break;
		case 20: strMsg = "Inspect Stage3,Wait for Move Down-Top2,ing"; break;
		case 21: strMsg = "Inspect Stage3,X3 Axis Move Back Top2 Position,ing"; break;
		case 22: strMsg = "Inspect Stage3,Wait for Move Down-Top1,ing"; break;
		case 23: strMsg = "Inspect Stage3,X3 Axis Move Back Top1 Position,ing"; break;
		case 24: strMsg = "Inspect Stage3,Wait for Move Down-Align,ing"; break;
		case 25: strMsg = "Inspect Stage3,X3 Axis Move Back Align Position,ing"; break;
		case 26: strMsg = "Inspect Stage3,Wait for Stage In,ing"; break;
		case 27: strMsg = "Inspect Stage3,X3 Stage In,ing"; break;
		case 28: strMsg = "Inspect Stage3,X3 Stage Up,end"; break;
		}
		break;
	case 17:
		strFun = "Inspect_Stage_4"; nSeqTotal = 28;
		switch (nId) {
		case  1: strMsg = "Inspect Stage4,X4 Axis Move To Top1 Position,start"; break;
		case  2: strMsg = "Inspect Stage4,Wait for Top1 Scan,ing"; break;
		case  3: strMsg = "Inspect Stage4,X4 Axis Move To Top1 Pitch,ing"; break;
		case  4: strMsg = "Inspect Stage4,X4 Top1 Scan Complete,ing"; break;
		case  5: strMsg = "Inspect Stage4,Wait for Top1 Angle Scan,ing"; break;
		case  6: strMsg = "Inspect Stage4,X4 Axis Move To Top1 Angle Pitch,ing"; break;
		case  7: strMsg = "Inspect Stage4,X4 Top1 Angle Scan Complete,ing"; break;
		case  8: strMsg = "Inspect Stage4,Wait for Move Top2,ing"; break;
		case  9: strMsg = "Inspect Stage4,X4 Axis Move To Top2 Position,ing"; break;
		case 10: strMsg = "Inspect Stage4,Wait for Top2 Scan,ing"; break;
		case 11: strMsg = "Inspect Stage4,X4 Axis Move To Top2 Pitch,ing"; break;
		case 12: strMsg = "Inspect Stage4,X4 Top2 Scan Complete,ing"; break;
		case 13: strMsg = "Inspect Stage4,Wait for Move Buffer,ing"; break;
		case 14: strMsg = "Inspect Stage4,X4 Axis Move To Buffer Position,ing"; break;
		case 15: strMsg = "Inspect Stage4,Wait for Move Unload,ing"; break;
		case 16: strMsg = "Inspect Stage4,X4 Axis Move To Unload Position,ing"; break;
		case 17: strMsg = "Inspect Stage4,Wait for Work Unload_Picker,ing"; break;
		case 18: strMsg = "Inspect Stage4,X4 Stage Down,ing"; break;
		case 19: strMsg = "Inspect Stage4,X4 Stage Out,ing"; break;
		case 20: strMsg = "Inspect Stage4,Wait for Move Down-Top2,ing"; break;
		case 21: strMsg = "Inspect Stage4,X4 Axis Move Back Top2 Position,ing"; break;
		case 22: strMsg = "Inspect Stage4,Wait for Move Down-Top1,ing"; break;
		case 23: strMsg = "Inspect Stage4,X4 Axis Move Back Top1 Position,ing"; break;
		case 24: strMsg = "Inspect Stage4,Wait for Move Down-Align,ing"; break;
		case 25: strMsg = "Inspect Stage4,X4 Axis Move Back Align Position,ing"; break;
		case 26: strMsg = "Inspect Stage4,Wait for Stage In,ing"; break;
		case 27: strMsg = "Inspect Stage4,X4 Stage In,ing"; break;
		case 28: strMsg = "Inspect Stage4,X4 Stage Up,end"; break;
		}
		break;
	case 18:
		strFun = "Unload_Picker_1"; nSeqTotal = 24;
		switch (nId) {
		case  1: strMsg = "Unload Picker1,Unload Picker Down,start"; break;
		case  2: strMsg = "Unload Picker1,Z1 Axis Move to Down Position,ing"; break;
		case  3: strMsg = "Unload Picker1,Unload Picker Close,ing"; break;
		case  4: strMsg = "Unload Picker1,Z1 Axis Move to Ready-Up Position,ing"; break;
		case  5: strMsg = "Unload Picker1,Unload Picker Module Check,ing"; break;
		case  6: strMsg = "Unload Picker1,Check Inspection Done,ing"; break;
		case  7: strMsg = "Unload Picker1,Check ROS Done,ing"; break;
		case  8: strMsg = "Unload Picker1,Wait NG Stage Work,ing"; break;
		case  9: strMsg = "Unload Picker1,Picker NG X-Y Move to Work Position,ing"; break;
		case 10: strMsg = "Unload Picker1,Z1 Axis Move to Down Position,ing"; break;
		case 11: strMsg = "Unload Picker1,Unload Picker Down,ing"; break;
		case 12: strMsg = "Unload Picker1,Unload Picker Open,ing"; break;
		case 13: strMsg = "Unload Picker1,Unload Picker Up,ing"; break;
		case 14: strMsg = "Unload Picker1,Wait Good Stage Work,ing"; break;
		case 15: strMsg = "Unload Picker1,Picker Good X-Y Move to Work Position,ing"; break;
		case 16: strMsg = "Unload Picker1,Z1 Axis Move to Down Position,ing"; break;
		case 17: strMsg = "Unload Picker1,Unload Picker Down,ing"; break;
		case 18: strMsg = "Unload Picker1,Unload Picker Open,ing"; break;
		case 19: strMsg = "Unload Picker1,Unload Picker Up,ing"; break;
		case 20: strMsg = "Unload Picker1,Unload Picker Picker Work End,ing"; break;
		case 21: strMsg = "Unload Picker1,Y1 Axis Move to Ready Position,ing"; break;
		case 22: strMsg = "Unload Picker1,X1 Axis Move to Stage Position,ing"; break;
		case 23: strMsg = "Unload Picker1,Wait Unload Picker 2 Interlock,ing"; break;
		case 24: strMsg = "Unload Picker1,Y1 Axis Move to Work Position,end"; break;
		}
		break;
	case 19:
		strFun = "Unload_Picker_2"; nSeqTotal = 24;
		switch (nId) {
		case  1: strMsg = "Unload Picker2,Unload Picker Down,start"; break;
		case  2: strMsg = "Unload Picker2,Z1 Axis Move to Down Position,ing"; break;
		case  3: strMsg = "Unload Picker2,Unload Picker Close,ing"; break;
		case  4: strMsg = "Unload Picker2,Z1 Axis Move to Ready-Up Position,ing"; break;
		case  5: strMsg = "Unload Picker2,Unload Picker Module Check,ing"; break;
		case  6: strMsg = "Unload Picker2,Check Inspection Done,ing"; break;
		case  7: strMsg = "Unload Picker2,Check ROS Done,ing"; break;
		case  8: strMsg = "Unload Picker2,Wait NG Stage Work,ing"; break;
		case  9: strMsg = "Unload Picker2,Picker NG X-Y Move to Work Position,ing"; break;
		case 10: strMsg = "Unload Picker2,Z1 Axis Move to Down Position,ing"; break;
		case 11: strMsg = "Unload Picker2,Unload Picker Down,ing"; break;
		case 12: strMsg = "Unload Picker2,Unload Picker Open,ing"; break;
		case 13: strMsg = "Unload Picker2,Unload Picker Up,ing"; break;
		case 14: strMsg = "Unload Picker2,Wait Good Stage Work,ing"; break;
		case 15: strMsg = "Unload Picker2,Picker Good X-Y Move to Work Position,ing"; break;
		case 16: strMsg = "Unload Picker2,Z1 Axis Move to Down Position,ing"; break;
		case 17: strMsg = "Unload Picker2,Unload Picker Down,ing"; break;
		case 18: strMsg = "Unload Picker2,Unload Picker Open,ing"; break;
		case 19: strMsg = "Unload Picker2,Unload Picker Up,ing"; break;
		case 20: strMsg = "Unload Picker2,Unload Picker Picker Work End,ing"; break;
		case 21: strMsg = "Unload Picker2,Y1 Axis Move to Ready Position,ing"; break;
		case 22: strMsg = "Unload Picker2,X1 Axis Move to Stage Position,ing"; break;
		case 23: strMsg = "Unload Picker2,Wait Unload Picker 2 Interlock,ing"; break;
		case 24: strMsg = "Unload Picker2,Y1 Axis Move to Work Position,end"; break;
		}
		break;
	case 20:
		strFun = "NG_Stage_1"; nSeqTotal = 16;
		switch (nId) {
		case  1: strMsg = "NG Stage1,Stage Master In,start"; break;
		case  2: strMsg = "NG Stage1,Stage Slave In,ing"; break;
		case  3: strMsg = "NG Stage1,Wait Align Move,ing"; break;
		case  4: strMsg = "NG Stage1,Y Axis Move to Align-1 Position,ing"; break;
		case  5: strMsg = "NG Stage1,Wait Align End,ing"; break;
		case  6: strMsg = "NG Stage1,Y Axis Move to Wait or Work Position,ing"; break;
		case  7: strMsg = "NG Stage1,Y Axis Move to Work Position,ing"; break;
		case  8: strMsg = "NG Stage1,Wait Unload Picker Work,ing"; break;
		case  9: strMsg = "NG Stage1,Stage Down,ing"; break;
		case 10: strMsg = "NG Stage1,Wait Move to Load Position,ing"; break;
		case 11: strMsg = "NG Stage1,Y Axis Move to Load Position,ing"; break;
		case 12: strMsg = "NG Stage1,Wait Stage Up,ing"; break;
		case 13: strMsg = "NG Stage1,Stage Up,ing"; break;
		case 14: strMsg = "NG Stage1,Stage Slave Out,ing"; break;
		case 15: strMsg = "NG Stage1,Stage Master Out,ing"; break;
		case 16: strMsg = "NG Stage1,Stage Work End,end"; break;
		}
		break;
	case 21:
		strFun = "NG_Stage_2"; nSeqTotal = 16;
		switch (nId) {
		case  1: strMsg = "NG Stage2,Stage Master In,start"; break;
		case  2: strMsg = "NG Stage2,Stage Slave In,ing"; break;
		case  3: strMsg = "NG Stage2,Wait Align Move,ing"; break;
		case  4: strMsg = "NG Stage2,Y Axis Move to Align-1 Position,ing"; break;
		case  5: strMsg = "NG Stage2,Wait Align End,ing"; break;
		case  6: strMsg = "NG Stage2,Y Axis Move to Wait or Work Position,ing"; break;
		case  7: strMsg = "NG Stage2,Y Axis Move to Work Position,ing"; break;
		case  8: strMsg = "NG Stage2,Wait Unload Picker Work,ing"; break;
		case  9: strMsg = "NG Stage2,Stage Down,ing"; break;
		case 10: strMsg = "NG Stage2,Wait Move to Load Position,ing"; break;
		case 11: strMsg = "NG Stage2,Y Axis Move to Load Position,ing"; break;
		case 12: strMsg = "NG Stage2,Wait Stage Up,ing"; break;
		case 13: strMsg = "NG Stage2,Stage Up,ing"; break;
		case 14: strMsg = "NG Stage2,Stage Slave Out,ing"; break;
		case 15: strMsg = "NG Stage2,Stage Master Out,ing"; break;
		case 16: strMsg = "NG Stage2,Stage Work End,end"; break;
		}
		break;
	case 22:
		strFun = "Good_Stage_1"; nSeqTotal = 16;
		switch (nId) {
		case  1: strMsg = "Good Stage1,Stage Master In,start"; break;
		case  2: strMsg = "Good Stage1,Stage Slave In,ing"; break;
		case  3: strMsg = "Good Stage1,Wait Align Move,ing"; break;
		case  4: strMsg = "Good Stage1,Y Axis Move to Align-1 Position,ing"; break;
		case  5: strMsg = "Good Stage1,Wait Align End,ing"; break;
		case  6: strMsg = "Good Stage1,Y Axis Move to Wait or Work Position,ing"; break;
		case  7: strMsg = "Good Stage1,Y Axis Move to Work Position,ing"; break;
		case  8: strMsg = "Good Stage1,Wait Unload Picker Work,ing"; break;
		case  9: strMsg = "Good Stage1,Stage Down,ing"; break;
		case 10: strMsg = "Good Stage1,Wait Move to Load Position,ing"; break;
		case 11: strMsg = "Good Stage1,Y Axis Move to Load Position,ing"; break;
		case 12: strMsg = "Good Stage1,Wait Stage Up,ing"; break;
		case 13: strMsg = "Good Stage1,Stage Up,ing"; break;
		case 14: strMsg = "Good Stage1,Stage Slave Out,ing"; break;
		case 15: strMsg = "Good Stage1,Stage Master Out,ing"; break;
		case 16: strMsg = "Good Stage1,Stage Work End,end"; break;
		}
		break;
	case 23:
		strFun = "Good_Stage_2"; nSeqTotal = 16;
		switch (nId) {
		case  1: strMsg = "Good Stage2,Stage Master In,start"; break;
		case  2: strMsg = "Good Stage2,Stage Slave In,ing"; break;
		case  3: strMsg = "Good Stage2,Wait Align Move,ing"; break;
		case  4: strMsg = "Good Stage2,Y Axis Move to Align-1 Position,ing"; break;
		case  5: strMsg = "Good Stage2,Wait Align End,ing"; break;
		case  6: strMsg = "Good Stage2,Y Axis Move to Wait or Work Position,ing"; break;
		case  7: strMsg = "Good Stage2,Y Axis Move to Work Position,ing"; break;
		case  8: strMsg = "Good Stage2,Wait Unload Picker Work,ing"; break;
		case  9: strMsg = "Good Stage2,Stage Down,ing"; break;
		case 10: strMsg = "Good Stage2,Wait Move to Load Position,ing"; break;
		case 11: strMsg = "Good Stage2,Y Axis Move to Load Position,ing"; break;
		case 12: strMsg = "Good Stage2,Wait Stage Up,ing"; break;
		case 13: strMsg = "Good Stage2,Stage Up,ing"; break;
		case 14: strMsg = "Good Stage2,Stage Slave Out,ing"; break;
		case 15: strMsg = "Good Stage2,Stage Master Out,ing"; break;
		case 16: strMsg = "Good Stage2,Stage Work End,end"; break;
		}
		break;
	case 24:
		strFun = "Ship_Align"; nSeqTotal = 6;
		switch (nId) {
		case 1: strMsg = "Ship Align1,Align 1 Position,start"; break;
		case 2: strMsg = "Ship Align1,Align 1 Complete,ing"; break;
		case 3: strMsg = "Ship Align2,Align 2 Position,ing"; break;
		case 4: strMsg = "Ship Align2,Align 2 Complete,ing"; break;
		case 5: strMsg = "Ship Align3,Align 3 Position,ing"; break;
		case 6: strMsg = "Ship Align3,Align 3 Complete,end"; break;
		}
		break;
	}

	strLotId = "";
	if (nFun ==  8) { strLotId = gData.sLotID_Tansfer[0];		if (strLotId == "") strLotId = gData.sLotID_Tansfer[2]; }
	if (nFun ==  9) { strLotId = gData.sLotID_Tansfer[1];		if (strLotId == "") strLotId = gData.sLotID_Tansfer[2]; }
	if (nFun == 10)	{ strLotId = gData.sLotID_LoadStage[0];		if (strLotId == "") strLotId = gData.sLotID_LoadStage[2]; }
	if (nFun == 11)	{ strLotId = gData.sLotID_LoadStage[1];		if (strLotId == "") strLotId = gData.sLotID_LoadStage[2]; }
	if (nFun == 12) { strLotId = gData.sLotID_LoadPicker[0];	if (strLotId == "") strLotId = gData.sLotID_LoadPicker[2]; }
	if (nFun == 13) { strLotId = gData.sLotID_LoadPicker[1];	if (strLotId == "") strLotId = gData.sLotID_LoadPicker[2]; }
	if (nFun == 14)	{ strLotId = gData.sLotID_VisionStage[0];	if (strLotId == "") strLotId = gData.sLotID_VisionStage[4]; }
	if (nFun == 15)	{ strLotId = gData.sLotID_VisionStage[1];	if (strLotId == "") strLotId = gData.sLotID_VisionStage[4]; }
	if (nFun == 16)	{ strLotId = gData.sLotID_VisionStage[2];	if (strLotId == "") strLotId = gData.sLotID_VisionStage[4]; }
	if (nFun == 17)	{ strLotId = gData.sLotID_VisionStage[3];	if (strLotId == "") strLotId = gData.sLotID_VisionStage[4]; }
	if (nFun == 18) { strLotId = gData.sLotID_UnloadPicker[0];	if (strLotId == "") strLotId = gData.sLotID_UnloadPicker[2]; }
	if (nFun == 19) { strLotId = gData.sLotID_UnloadPicker[1];	if (strLotId == "") strLotId = gData.sLotID_UnloadPicker[2]; }
	if (nFun == 20)	{ strLotId = gData.sLotID_NGTray[0];		if (strLotId == "") strLotId = gData.sLotID_NGTray[2]; }
	if (nFun == 21)	{ strLotId = gData.sLotID_NGTray[1];		if (strLotId == "") strLotId = gData.sLotID_NGTray[2]; }
	if (nFun == 22)	{ strLotId = gData.sLotID_GoodTray[0];		if (strLotId == "") strLotId = gData.sLotID_GoodTray[2]; }
	if (nFun == 23)	{ strLotId = gData.sLotID_GoodTray[1];		if (strLotId == "") strLotId = gData.sLotID_GoodTray[2]; }
	if (strLotId == "") strLotId = gData.sLotID_Start;

	double dTime = (GetTickCount() - m_dwTakt) / 1000.0;	if (dTime > 60.0) return;
	strInfo = Add_ModuleInfo(nFun, nId);

	strLog.Format("MCC,(%02d),%s,%s,%s,%02d/%02d,%s,%0.3lf", nFun, strLotId, strInfo, strFun, nId, nSeqTotal, strMsg, dTime);
	g_objLogFile.Save_SpcMccLog(strLog, strLotId);

//	strLog.Format("%02d,%s,%02d,%s,%0.3lf", nFun, strFun, nId, strMsg, dTime);
//	g_objLogFile.Save_MCCLog(strLog);
}

CString CLoopTimer::Add_ModuleInfo(int nFun, int nId)
{
	int nModuleCount = 0;
	CString strReturn, strTemp, strBarcode, strPort, strTray, strCm;
	strReturn = strTemp = strBarcode = strPort = strTray = strCm = "";

	if (nFun == 12) {	// Load Picker1 X,Y,Z
		int nNo = gData.InfoLoadPick[0][9];
		if (nNo > 0) {
			for (int i = 0; i < 8; i++) {
				if (gData.InfoLoadPick[0][i] > 0) {
					strTemp.Format("%d;", gData.nPortNo_LoadPicker[0]); strPort += strTemp;
					strTemp.Format("%d;", gData.nTrayNo_LoadPicker[0]); strTray += strTemp;
					strTemp.Format("%d;", nNo+i);						strCm += strTemp;
					strTemp.Format(";");								  strBarcode += strTemp;
					nModuleCount++;
				}
			}
		}
	}
	if (nFun == 13) {	// Load Picker2 X,Y,Z
		int nNo = gData.InfoLoadPick[1][9];
		if (nNo > 0) {
			for (int i = 0; i < 8; i++) {
				if (gData.InfoLoadPick[1][i] > 0) {
					strTemp.Format("%d;", gData.nPortNo_LoadPicker[1]); strPort += strTemp;
					strTemp.Format("%d;", gData.nTrayNo_LoadPicker[1]); strTray += strTemp;
					strTemp.Format("%d;", nNo+i);						strCm += strTemp;
					strTemp.Format(";");								  strBarcode += strTemp;
					nModuleCount++;
				}
			}
		}
	}
	if (nFun >= 14 && nFun <= 17) {	// Inspection Stage X
		int nX = nFun - 14;
		int nNo = gData.InfoVision[nX][9];
		if (nNo > 0) {
			for (int i = 0; i < 8; i++) {
				if (gData.InfoVision[nX][i] > 0) {
					strTemp.Format("%d;", gData.nPortNo_VisionStage[nX]); strPort += strTemp;
					strTemp.Format("%d;", gData.nTrayNo_VisionStage[nX]); strTray += strTemp;
					strTemp.Format("%d;", nNo+i);						  strCm += strTemp;
					strTemp.Format(";");								  strBarcode += strTemp;
					nModuleCount++;
				} 
			}
		}
	}
	if (nFun == 18) {	// Unload Picker1 X,Y,Z
		int nNo = gData.InfoUnloadPick[0][9];
		if (nNo > 0) {
			for (int i = 0; i < 8; i++) {
				if (gData.InfoUnloadPick[0][i] > 0) {
					strTemp.Format("%d;", gData.nPortNo_UnloadPicker[0]); strPort += strTemp;
					strTemp.Format("%d;", gData.nTrayNo_UnloadPicker[0]); strTray += strTemp;
					strTemp.Format("%d;", nNo+i);						  strCm += strTemp;
					strTemp.Format("%s;", gLot.sBarCode[gData.nPortNo_UnloadPicker[0]-1][gData.nTrayNo_UnloadPicker[0]-1][nNo+i-1]); strBarcode += strTemp;
					nModuleCount++;
				}
			}
		}
	}
	if (nFun == 19) {	// Unload Picker2 X,Y,Z
		int nNo = gData.InfoUnloadPick[1][9];
		if (nNo > 0) {
			for (int i = 0; i < 8; i++) {
				if (gData.InfoUnloadPick[1][i] > 0) {
					strTemp.Format("%d;", gData.nPortNo_UnloadPicker[1]); strPort += strTemp;
					strTemp.Format("%d;", gData.nTrayNo_UnloadPicker[1]); strTray += strTemp;
					strTemp.Format("%d;", nNo+i);						  strCm += strTemp;
					strTemp.Format("%s;", gLot.sBarCode[gData.nPortNo_UnloadPicker[1]-1][gData.nTrayNo_UnloadPicker[1]-1][nNo+i-1]); strBarcode += strTemp;
					nModuleCount++;
				}
			}
		}
	}

	if (strBarcode.Right(1) == ';') strBarcode.Delete(strBarcode.GetLength()-1);
	if (strPort.Right(1) == ';') strPort.Delete(strPort.GetLength()-1);
	if (strTray.Right(1) == ';') strTray.Delete(strTray.GetLength()-1);
	if (strCm.Right(1) == ';') strCm.Delete(strCm.GetLength()-1);

	strReturn.Format("%s,%s,%s,%s,%d", strBarcode, strPort, strTray, strCm, nModuleCount);
	return strReturn;
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
