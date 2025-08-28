// LogFile.cpp: implementation of the CLogFile class.
//
#include "stdafx.h"
#include "LogFile.h"
#include "Inspector.h"

// CLogFile
CLogFile g_objLogFile;

CCriticalSection g_csAlarmLog;
CCriticalSection g_csHandlerLog;
CCriticalSection g_csInspectorLog;
CCriticalSection g_csJobListLog;
CCriticalSection g_csAlignerLog;
CCriticalSection g_csMesAgentLog;
CCriticalSection g_csDispatcherLog;
CCriticalSection g_csDailyLotLog;
CCriticalSection g_csOperatingRatioLog;

CLogFile::CLogFile()
{
}

CLogFile::~CLogFile()
{
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Create_Folder(CString sPath)
{
	if (sPath == _T("")) return;
	if (sPath.Right(1) == _T("\\")) sPath = sPath.Left(sPath.GetLength() - 1);
	if (GetFileAttributes(sPath) != -1) return;	// Directory Exist!!!

	int nFound = sPath.ReverseFind('\\');
	Create_Folder(sPath.Left(nFound));

	CreateDirectory(sPath, NULL);
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Save_AlarmLog(CString sLog)
{
	g_csAlarmLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Alarm";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csAlarmLog.Unlock();
}

void CLogFile::Save_AlarmResetLog(CString strLog)
{
	Save_AlarmECMLog(1, strLog);
}

void CLogFile::Save_AlarmECMLog(int nType, CString strLog)	//nType:1[Reset], 0[Set]
{
	CString strFile, sTitle, strTime1, strTime2, strSave, sNGCodeList, sTitleTemp;

	SYSTEMTIME time;
	GetLocalTime(&time);
	strTime1.Format("%04d%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour);
	strTime2.Format("%02d:%02d:%02d.%03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	sTitle.Format("Time,LotNum,Code,Alarm,Start,End,Duration,min,Unit,Group,Detail Items\r\n");
	strFile.Format("%s%s_%s_Alarm.csv", ECM_LOG, gAlm.sLotID, strTime1);

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());

		if (nType == 0) strSave.Format("%s,%s,,,,,%s,%s,%s\r\n", strTime2, strLog, gAlm.sUnit, gAlm.sGroup, gAlm.sItems);
		else			strSave.Format("%s,%s,%s,%s,%s\r\n", strTime2, strLog, gAlm.sUnit, gAlm.sGroup, gAlm.sItems);

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}

	CString strLAS;
	strLAS.Format("%s\\LOG\\LAS\\%04d\\%02d\\%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strLAS);
	strLAS.Format("%s\\LOG\\LAS\\%04d\\%02d\\%02d\\%s_%s_Alarm.csv", gsCurrentDir, time.wYear, time.wMonth, time.wDay, gAlm.sLotID, strTime1);
	CopyFile(strFile, strLAS, FALSE);	// Backup
}

void CLogFile::Save_HandlerLog(CString sLog)
{
	g_csHandlerLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Handler";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csHandlerLog.Unlock();
}

void CLogFile::Save_InspectorLog(CString sLog)
{
	g_csInspectorLog.Lock();

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strSave;
	strPath.Format("%s\\LOG\\Inspector\\%04d\\%02d\\%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	strFile.Format("%s\\%04d%02d%02d-%02d.csv", strPath, time.wYear, time.wMonth, time.wDay, time.wHour);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csInspectorLog.Unlock();
}


void CLogFile::Save_JobListLog(CString sLog)
{
	g_csJobListLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\JobList";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, sTitle, strSave;

	strFile.Format("%s\\%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);
	if (time.wHour < 7) {
		CTime PreDay(time);
		PreDay -= CTimeSpan(1,0,0,0);	//CTimeSpan(일,시,분,초)
		strFile.Format("%s\\%04d%02d%02d.csv", strPath,  PreDay.GetYear(), PreDay.GetMonth(), PreDay.GetDay());
	}

//	sTitle.Format("Time,PortNo,LotID,Count,NG_Count,Good_Count,Rate,Start_Time,End_Time,Tack\r\n");
	sTitle.Format("Time,PortNo,LotID,Count,NG_Count,Good_Count,Rate,Skip_Count,Bar_NoRead,RosJudge,RosGood,RosNG,RosRepair,RosTimeOver,Start_Time,End_Time,Tack,FAI_NG\r\n");

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			CString strSave;
			if (file.GetLength() <= 0) {
				strSave.Format("%s%02d:%02d:%02d %03d,%s\r\n", sTitle, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);
			} else {
				strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);
			}

			file.SeekToEnd();	
			file.Write(strSave, strSave.GetLength());
			file.Close();
		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csJobListLog.Unlock();
}

void CLogFile::Save_ECMLog(int nType, CString strLog)	//nType:1[Alarm], 2[Joblist]
{
	CString strFile, sTitle, strTime, strDate, strTime2, strTime1, strSave;

	SYSTEMTIME time;
	GetLocalTime(&time);
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	strDate.Format("%04d-%02d-%02d", time.wYear, time.wMonth, time.wDay);
	strTime2.Format("%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
	strTime1.Format("%02d:%02d:%02d.%03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	sTitle.Format("Time,Station,SensorID,Barcode,Date,Time2,Machine_Code,Time,LotNum,PortNo,Count,NG_Count,Good_Count,Rate,Skip_Count,Bar_NoRead,RosJudge,RosGood,RosNG,RosRepair,RosTimeOver,Start_Time,End_Time,Tack,FAI_NG\r\n");
	strFile.Format("%s%s_%04d%02d%02d%02d_JobList.csv", ECM_LOG, gLot.sELotID, time.wYear, time.wMonth, time.wDay, time.wHour);

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());

		strSave.Format("%s,%s,,,%s,%s,%s,%s,%s,%s\r\n", strTime, m_sPCID, strDate, strTime2, m_sMachineCode, strTime1, gLot.sELotID, strLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}

	CString strLAS;
	strLAS.Format("%s\\LOG\\LAS\\%04d\\%02d\\%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strLAS);
	strLAS.Format("%s\\LOG\\LAS\\%04d\\%02d\\%02d\\%s_%04d%02d%02d%02d_JobList.csv", gsCurrentDir, time.wYear, time.wMonth, time.wDay, gLot.sELotID, time.wYear, time.wMonth, time.wDay, time.wHour);
	CopyFile(strFile, strLAS, FALSE);	// Backup
}

void CLogFile::Save_ECMOutPut(int nPNo, int nTNo, int nMNo, int nStageNo, int nType)
{
	CString sLotID, strFile, sTitle, strTime, strDate, strTime2, strTime1, strSave, sData1, sData2, sData3;
	if (nPNo < 1 || nPNo > 30) return;
	if (nTNo < 1 || nTNo > 20) return;
	if (nMNo < 1 || nMNo > 40) return;
	sLotID = gLot.sLotID[nPNo-1];

	SYSTEMTIME time;
	GetLocalTime(&time);
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	strDate.Format("%04d-%02d-%02d", time.wYear, time.wMonth, time.wDay);
	strTime2.Format("%02d:%02d:%02d", time.wHour, time.wMinute, time.wSecond);
	strTime1.Format("%02d:%02d:%02d.%03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	sTitle.Format("Time,Station,SensorID,Barcode,Date,Time2,Machine_Code,Time,LotNum,LoadPort,LoadTray,LoadPos,LoadPicker,LoadPickerNo,VisionStage,VisionStageNo,UnloadPicker,UnloadPickerNo,Output,OutTray,OutPos,Judge,JudgeB1,DefectCodeB1,JudgeAG,DefectCodeAG,JudgeT1,DefectCodeT1,JudgeTG,DefectCodeTG,JudgeT2,DefectCodeT2,ADJ Result,Flow Time,BTM Vision Process Time,Top1 Vision Process Time,Top2 Vision Process Time,ULD delay,ROS Operator,ROS Images,ROS Result,ROS Judge Time,ROS Response Time,최종 판정 결과 코드,LoadMZID,InputCarrierID,OutputCarrierID,Repair,MN-Code\r\n");
	strFile.Format("%s%s_%04d%02d%02d%02d_Output.csv", ECM_LOG, sLotID, time.wYear, time.wMonth, time.wDay, time.wHour);

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());

		int		nOutTray;
		CString sOutput, sJudge[6], sCode[6], sRosJ, sCarStage;

		if (nType == 1) {	//NG
			nOutTray = gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][6]; sOutput = "NG Tray"; sJudge[0] = "N"; sCode[0] = gLot.sNGCode_I[nPNo-1][nTNo-1][nMNo-1][0];
			sCarStage = gData.sCarID_GoodTray[nStageNo];
		} else {			//Good
			nOutTray = gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][5]; sOutput = "OK Tray"; sJudge[0] = "G"; sCode[0] = "";
			sCarStage = gData.sCarID_NGTray[nStageNo];;
		}
		for(int i=1; i<6; i++) {
			sJudge[i] = "G"; sCode[i] = "";
			if (gLot.nJudge_I[nPNo-1][nTNo-1][nMNo-1][i] > 2) {
				sJudge[i] = "N"; sCode[i] = gLot.sNGCode_I[nPNo-1][nTNo-1][nMNo-1][i];
			}
		}
		
		if		(gLot.nJudge_R[nPNo-1][nTNo-1][nMNo-1] == 2) sRosJ = "G";
		else if (gLot.nJudge_R[nPNo-1][nTNo-1][nMNo-1] == 4) sRosJ = "R";
		else if (gLot.nJudge_R[nPNo-1][nTNo-1][nMNo-1] >= 3) sRosJ = "N";
		else												 sRosJ = "";

		sData1.Format("%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%d,", nPNo, nTNo, nMNo, 
				gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][1], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][3], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2],
				gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][4], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], sOutput, nOutTray, gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][7]);

		sData2.Format("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,", sJudge[0], sJudge[1], sCode[1], sJudge[2], sCode[2], sJudge[3], sCode[3], sJudge[4], sCode[4], sJudge[5], sCode[5]);

		//ROS Images,ROS Result,ROS Judge Time,ROS Response Time,최종 판정 결과 코드,LoadMZID,InputCarrierID,OutputCarrierID,Repair,MN-Code\r\n");
		sData3.Format(",,,,,,,%d,%s,,,%s,%s,%s,%s,,%s", (gLot.nImageCnt[nPNo-1][nTNo-1][nMNo-1][0]+gLot.nImageCnt[nPNo-1][nTNo-1][nMNo-1][1]), sRosJ, sJudge[0], gLot.sMZID_LD[nPNo-1], gLot.sCarID_LD[nPNo-1], sCarStage, sCode[0]);

		strSave.Format("%s,%s,,%s,%s,%s,%s,%s,%s,%s%s%s\r\n", strTime, m_sPCID, gLot.sBarCode[nPNo-1][nTNo-1][nMNo-1], strDate, strTime2, m_sMachineCode, strTime1, sLotID, sData1, sData2, sData3);

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}

	CString strLAS;
	strLAS.Format("%s\\LOG\\LAS\\%04d\\%02d\\%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strLAS);
	strLAS.Format("%s\\LOG\\LAS\\%04d\\%02d\\%02d\\%s_%04d%02d%02d%02d_Output.csv", gsCurrentDir, time.wYear, time.wMonth, time.wDay, sLotID, time.wYear, time.wMonth, time.wDay, time.wHour);
	CopyFile(strFile, strLAS, FALSE);	// Backup
}

void CLogFile::Save_MCCLog(CString sLog)
{
	CString strPath = gsCurrentDir + "\\LOG\\MCC";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, sTitle, strSave;
	strFile.Format("%s\\%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);
	sTitle.Format("Time,Fun_No,Funtion,List_No,List_Text,Tack_Time\r\n");

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			CString strSave;
			if (file.GetLength() <= 0) {
				strSave.Format("%s%02d:%02d:%02d %03d,%s\r\n", sTitle, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);
			} else {
				strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);
			}

			file.SeekToEnd();	
			file.Write(strSave, strSave.GetLength());
			file.Close();
		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_TackLog(CString sLog)
{
	CString strPath = gsCurrentDir + "\\LOG\\Tack";
	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, sTitle, strSave;
	strFile.Format("%s\\%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			CString strSave;
			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.SeekToEnd();	
			file.Write(strSave, strSave.GetLength());
			file.Close();
		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_LotResult(CString sLog)
{
	CString strPath = gsCurrentDir + "\\LOG\\LotResult";

	Create_Folder(strPath);

	CTime tNow = CTime::GetCurrentTime();
	CTime tLog = tNow - CTimeSpan(7 * 60 * 60);

	CString strFile, strTitle, strDate, strSave;
	strFile.Format("%s\\%04d%02d%02d.csv", strPath, tLog.GetYear(), tLog.GetMonth(), tLog.GetDay());

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) return;

	if (gData.nLanguage == 0) strTitle.Format("날짜,Lot Start,Lot End,Term,호기,Lot ID,투입수,양품수,불량수,불량률,HREN(H),HREP(P),Spider(S),BA(B),Chip(C),MES(M)\r\n");
	else					  strTitle.Format("Date,Lot Start,Lot End,Term,No,Lot ID,Number of inputs,Number of good,Number of NG,Defect rate,HREN(H),HREP(P),Spider(S),BA(B),Chip(C),MES(M)\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strDate.Format("%02d/%02d", tNow.GetMonth(), tNow.GetDay());

		strSave.Format("%s,%s\r\n", strDate, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_MesAgentLog(CString sLog)
{
	g_csMesAgentLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\MESAgent";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csMesAgentLog.Unlock();
}

void CLogFile::Save_MesNgLog(CString sLog, int nPortIdx)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3 = gsCurrentDir + "\\LOG\\MES_NG";

	Create_Folder(strPath1);
	Create_Folder(strPath2);
	Create_Folder(strPath3);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile1, strFile2, strFile3, strTitle, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d%02d_MESNG.csv", strPath1, gLot.sLotID[nPortIdx], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile2.Format("%s\\%s_%04d%02d%02d%02d_MESNG.csv", strPath2, gLot.sLotID[nPortIdx], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile3.Format("%s\\%04d%02d%02d.csv", strPath3, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) return;

	strTitle.Format("Date,Lot ID,Module ID,MES Judge,NG Code,NG Text\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup

	} catch (CFileException *pEx) {
		pEx->Delete();
	}

	if (!file.Open(strFile3, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) return;

	try{
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();
	}
	catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_DispatcherLog(CString sLog)
{
	g_csDispatcherLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Dispatcher";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csDispatcherLog.Unlock();
}

void CLogFile::Save_OutTray(CString strLotID, CString strOut, int nPosX, int nPosY, int nMzNo, int nCarNo, int nLineNo, int nPickNo)
{
	if (nPosX < 1 || nPosY < 1 || nMzNo < 1 || nCarNo < 1 || nLineNo < 1 || nPickNo < 1) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle, strSave;
	strPath.Format("%s\\LOG\\OutTray\\%04d-%02d-%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	strFile.Format("%s\\%s_%s.csv", strPath, strLotID, strOut);	// LotID, NG/GOOD

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) return;

	strTitle.Format("TRAY_X,TRAY_Y,MZ_NO,CARRIER_NO,LINE_NO,PICK_NO,JUDGE\r\n");
	/*
	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		//검사결과 (0:Empty, 1:Good, 2:NG)
		int nJudge = gData.InfoUnloadPick[nPickNo-1];
		strSave.Format("%d,%d,%d,%d,%d,%d,%d\r\n", nPosX, nPosY, nMzNo, nCarNo, nLineNo, nPickNo, nJudge);

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}/*/
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Save_TestLog(CString sLog)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("Test\\%04d%02d%02d_000000_Test.csv", time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_LotLog()
{
/*
	CString sFileName, strFile, strCreDate, sCreFile, sTemp, sData, sInsResult, sBarResult;
	SYSTEMTIME time;
	GetLocalTime(&time);

	if (gLot.sLotID == "") gLot.sLotID = "LOT_ID_DEFAULT";
	strCreDate.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	strFile.Format("LOG/LotData/%04d/%02d/%02d/%s_%s.txt", time.wYear, time.wMonth, time.wDay, gLot.sLotID, strCreDate);
	sFileName = strFile;

	sCreFile.Format("%s\\LOG\\LotData\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	MakeFolder(sCreFile);

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) return;

	sTemp.Format("LotID,%s,Start_Time,%s,End_Time,%s,Tray_Count,%02d,CM_Count,%04d,Tack,%0.7lf,\r\n\r\n", gLot.sLotID, gLot.sStartTime, gLot.sEndTime, gLot.nTrayCount, gLot.nCmCount, gLot.dTackTime);
	sData.Format("%sTray_No,Pocket_No,ID,Inspetion Result,ID Result,\r\n", sTemp);
	file.Write(sData, sData.GetLength());

	try {
		int	nCmCount = 0;
		for(int i=0; i<30; i++) {
			if (nCmCount > gLot.nCmCount) break;

			for(int j=0; j<42; j++){
				nCmCount++;
				if (nCmCount > gLot.nCmCount) break;

				file.SeekToEnd();

				sData.Format("%d,%d,%s,%s,%s,\r\n", i+1, j+1, gCM.sBarID[i][j], gCM.sJudge[i][j], gCM.sNGCode[i][j]);
				file.Write(sData, sData.GetLength());
			}
		}

		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
*/
}

void CLogFile::Save_DailyLot(CString sLog)
{
	g_csDailyLotLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\DailyLot";
	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave, strTitle;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time \tLot_ID \tTotal \tGood \tNG \tRate \tSkip \tBar_NoRead \tRosRequest \tRosGood \tRosNg \tRosRepair \tRosTimeOut \tFiltering1 \tFiltering2 \tRateAvi \tRunTime \tTack\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csDailyLotLog.Unlock();
}

void CLogFile::Save_ECMDailyLot(CString sLog)
{
	CString strFile, sTitle, strSave;

	SYSTEMTIME time;
	GetLocalTime(&time);

	strFile.Format("%s%s_%04d%02d%02d%02d_DailyLot.csv", ECM_LOG, gLot.sELotID, time.wYear, time.wMonth, time.wDay, time.wHour);

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) {
			sTitle.Format("Time,Lot_ID,Total,Good,NG,Rate,Skip,Bar_NoRead,RosRequest,RosGood,RosNg,RosRepair,RosTimeOut,Filtering1,Filtering2,RateAvi,RunTime,Tack\r\n");
			file.Write(sTitle, sTitle.GetLength());
		}

		strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}

	CString strLAS;
	strLAS.Format("%s\\LOG\\LAS\\%04d\\%02d\\%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strLAS);
	strLAS.Format("%s\\LOG\\LAS\\%04d\\%02d\\%02d\\%s_%04d%02d%02d%02d_DailyLot.csv", gsCurrentDir, time.wYear, time.wMonth, time.wDay, gLot.sELotID, time.wYear, time.wMonth, time.wDay, time.wHour);
	CopyFile(strFile, strLAS, FALSE);	// Backup
}

void CLogFile::Save_LotLog(int nPortNo)
{
	CString sFileName, strFile, strCreDate, sCreFile, sTemp, sData, sInsResult, sBarResult, sJudge;
	SYSTEMTIME time;
	GetLocalTime(&time);

	if (gLot.sLotID[nPortNo-1] == "") gLot.sLotID[nPortNo-1] = "LOT_ID_DEFAULT";
	sCreFile.Format("%s\\LOG\\LotData\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(sCreFile);

	strCreDate.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	strFile.Format("LOG/LotData/%04d/%02d/%02d/%s_%s.txt", time.wYear, time.wMonth, time.wDay, gLot.sLotID[nPortNo-1], strCreDate);
	sFileName = strFile;

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) return;

	sTemp.Format("LotID,%s,Start_Time,%s,End_Time,%s,Tray_Count,%02d,CM_Count,%04d,Tack,%0.7lf,\r\n\r\n", gLot.sLotID[nPortNo-1], gLot.sLLStartTime[nPortNo-1], gLot.sLLEndTime[nPortNo-1], 1, gLot.nCmCount[nPortNo-1], gLot.dLLTackTime[nPortNo-1]);
	sData.Format("%sTray_No,Pocket_No,ID,Inspetion Result,ID Result,\r\n", sTemp);
	file.Write(sData, sData.GetLength());

	try {
		int	nCmCount = 0;
		for (int i = 0; i < 1; i++) {
			for (int j = 0; j < 40; j++) {
				if (gLot.nJudge_I[nPortNo-1][i][j][0] == 0) continue;

				file.SeekToEnd();
				sJudge = (gLot.nJudge_I[nPortNo-1][i][j][0] <= 2 ? "" : "N");
				sData.Format("%d,%d,%s,%s,%s,\r\n", i+1, j+1, gLot.sBarCode[nPortNo-1][i][j], sJudge, gLot.sNGCode_I[nPortNo-1][i][j][0]);
				file.Write(sData, sData.GetLength());
			}
		}
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_OperatingRatio(CString sLog)	// 가동률 작업 중
{
	g_csOperatingRatioLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\OperatingRatio";
	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile,strFile2, strSave, strTitle;
	strFile.Format("%s\\%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);
	strFile2.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	strTitle.Format("Time,호기,Lot ID,Lot Start,Lot End,Cycle Time,LM(EA),Run Time,Stop Time,Error Time,Error Count,수율,UPH,MTB\r\n");
	strSave.Format("[%02d:%02d:%02d %03d]%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();
			if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	if (file.Open(strFile2, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();
			if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csOperatingRatioLog.Unlock();
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Save_OutTrayGLog(int nPNo, int nTNo, int nMNo, int nGdStageNo)
{
	CString sLotID, strFile, strTitle, strFolder, strSave, sRosJ, sJurge;
	if (nPNo < 1 || nPNo > 30) return;
	if (nTNo < 1 || nTNo > 20) return;
	if (nMNo < 1 || nMNo > 40) return;
	if (nGdStageNo < 0 || nGdStageNo > 1) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	strFolder.Format("%s\\LOG\\OutTray\\%04d-%02d-%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strFolder);

	sLotID = gLot.sLotID[nPNo-1];
	strFile.Format("LOG/OutTray/%04d-%02d-%02d/%s_Good.csv", time.wYear, time.wMonth, time.wDay, sLotID);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
//				strTitle.Format("LoadPort,LoadTray,LoadPos,Barcode,LoadPicker,LoadPickerNo,VisionStage,VisionStageNo,UnloadPicker,UnloadPickerNo,GoodTray,GoodPos,Judge\r\n");
				strTitle.Format("LoadPort,LoadTray,LoadPos,Barcode,LoadPicker,LoadPickerNo,VisionStage,VisionStageNo,UnloadPicker,UnloadPickerNo,GoodTray,GoodPos,Judge,LoadMZID,LoadCarrierID,GoodCarrierID,ROSJudge\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			if (gLot.nJudge_R[nPNo-1][nTNo-1][nMNo-1] == 2) sRosJ = "G"; else sRosJ = "";
			if (gLot.nMarginal[nPNo-1][nMNo-1] > 0) sJurge = "Cosmetic NG"; else sJurge = "G";

			strSave.Format("%d,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%s,%s,%s,%s,%s\r\n",
				nPNo, nTNo, nMNo, gLot.sBarCode[nPNo-1][nTNo-1][nMNo-1], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][1], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][3], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], 
				gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][4], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][5], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][7],
				sJurge, gLot.sMZID_LD[nPNo-1], gLot.sCarID_LD[nPNo-1], gData.sCarID_GoodTray[nGdStageNo], sRosJ);

			file.Write(strSave, strSave.GetLength());
			file.Close();

			//PortNo(1~6),LoadTrayNo(1~8),LoadPorket(1~40),Barcode,LoadPickerNo(1,2),LoadPickerIndex(1~8),VisionStageNo(1~4),VisionStageIndex(1~8),
			//UnloadPickerNo(1,2),UnloadPickerIndex(1~8),Judge(N,G),GoodTrayNo(1~8),GoodTrayPorket(1~40),NGTrayNo(1~8),NGTrayPorket(1~40)
			strSave.Format("%d,%d,%d,%s,%d,%d,%d,%d,%d,%d,G,%d,%d,-1,-1",
				nPNo, nTNo, nMNo, gLot.sBarCode[nPNo-1][nTNo-1][nMNo-1], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][1], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][3], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], 
				gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][4], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][5], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][7]);
			g_objInspector.Set_HistoryReply(INSPECTOR_ALL, strSave);

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	Save_ECMOutPut(nPNo, nTNo, nMNo, nGdStageNo, 0);
}

void CLogFile::Save_OutTrayNLog(int nPNo, int nTNo, int nMNo, int nNgStageNo)
{
	CString sLotID, strFile, strTitle, strFolder, strSave, sJudge[5], sCode[5], sRosRepair, sNGCode, sNGCode20[3];
	if (nPNo < 1 || nPNo > 30) return;
	if (nTNo < 1 || nTNo > 20) return;
	if (nMNo < 1 || nMNo > 40) return;
	if (nNgStageNo < 0 || nNgStageNo > 1) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	strFolder.Format("%s\\LOG\\OutTray\\%04d-%02d-%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strFolder);

	sLotID = gLot.sLotID[nPNo-1];
	strFile.Format("LOG/OutTray/%04d-%02d-%02d/%s_NG.csv", time.wYear, time.wMonth, time.wDay, sLotID);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				sNGCode20[0] = "";
				if (gData.bJahwa==FALSE) {
					for(int i=0; i<5; i++) {
						if (i==0) sNGCode20[1] = "B1";
						if (i==1) sNGCode20[1] = "AG";
						if (i==2) sNGCode20[1] = "T1";
						if (i==3) sNGCode20[1] = "TG";
						if (i==4) sNGCode20[1] = "T2";
						for(int j=0; j<20; j++) {
							sNGCode20[2].Format(",%sNGCode%02d", sNGCode20[1], j+1);
							sNGCode20[0] = sNGCode20[0] + sNGCode20[2];
						}
					}
				}
				
				strTitle.Format("LoadPort,LoadTray,LoadPos,Barcode,LoadPicker,LoadPickerNo,VisionStage,VisionStageNo,UnloadPicker,UnloadPickerNo,NGTray,NGPos,Judge,JudgeB1,DefectCodeB1,JudgeAG,DefectCodeAG,JudgeT1,DefectCodeT1,JudgeTG,DefectCodeTG,JudgeT2,DefectCodeT2,LoadMZID,LoadCarrierID,NGCarrierID,Repair,MN-Code%s\r\n", sNGCode20[0]);
				file.Write(strTitle, strTitle.GetLength());
			}

			for(int i=1; i<6; i++) {
				sJudge[i-1] = "G"; sCode[i-1] = "";
				if (gLot.nJudge_I[nPNo-1][nTNo-1][nMNo-1][i] > 2) {
					sJudge[i-1] = "N";
					sCode[i-1]  = gLot.sNGCode_I[nPNo-1][nTNo-1][nMNo-1][i];
				}
			}
			sRosRepair = "";
			if (gLot.nJudge_R[nPNo-1][nTNo-1][nMNo-1] == 4) sRosRepair = "R";
			sNGCode = gLot.sNGCode_I[nPNo-1][nTNo-1][nMNo-1][0];

			sNGCode20[0] = "";
			if (gData.bJahwa==FALSE) {
				for(int i=0; i<5; i++) {
					for(int j=0; j<20; j++) {
						sNGCode20[2].Format(",%s", gNG->sNGCode[nPNo-1][nTNo-1][nMNo-1][i][j]);
						sNGCode20[0] = sNGCode20[0] + sNGCode20[2];
					}
				}
			}

			strSave.Format("%d,%d,%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,N,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s%s\r\n",
				nPNo, nTNo, nMNo, gLot.sBarCode[nPNo-1][nTNo-1][nMNo-1], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][1], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][3], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], 
				gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][4], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][6], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][7],
				sJudge[0], sCode[0], sJudge[1], sCode[1], sJudge[2], sCode[2], sJudge[3], sCode[3], sJudge[4], sCode[4], 
				gLot.sMZID_LD[nPNo-1], gLot.sCarID_LD[nPNo-1], gData.sCarID_NGTray[nNgStageNo], sRosRepair, sNGCode, sNGCode20[0]);

			file.Write(strSave, strSave.GetLength());
			file.Close();

			//PortNo(1~6),LoadTrayNo(1~8),LoadPorket(1~40),Barcode,LoadPickerNo(1,2),LoadPickerIndex(1~8),VisionStageNo(1~4),VisionStageIndex(1~8),
			//UnloadPickerNo(1,2),UnloadPickerIndex(1~8),Judge(N,G),GoodTrayNo(1~8),GoodTrayPorket(1~40),NGTrayNo(1~8),NGTrayPorket(1~40)
			strSave.Format("%d,%d,%d,%s,%d,%d,%d,%d,%d,%d,N,-1,-1,%d,%d",
				nPNo, nTNo, nMNo, gLot.sBarCode[nPNo-1][nTNo-1][nMNo-1], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][1], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][3], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], 
				gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][4], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][2], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][6], gLot.nHistory[nPNo-1][nTNo-1][nMNo-1][7]);
			g_objInspector.Set_HistoryReply(INSPECTOR_ALL, strSave);

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	Save_ECMOutPut(nPNo, nTNo, nMNo, nNgStageNo, 1);
}

void CLogFile::Save_SpcMccLog(CString sLog, CString sLotID)
{
	CString strFile, strTitle, strFolder, strSave;
	if (sLotID.GetLength() < 1) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	strFolder.Format("%s\\LOG\\MCC\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strFolder);

	strFile.Format("LOG/MCC/%04d/%02d/%02d/%s.txt", time.wYear, time.wMonth, time.wDay, sLotID);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,,Seq No,Lot ID,Barcode,Port No,Tray No,Cm No,Cm Count,Seq Name,Seq No,Category,Text,Cycle,Time,ErrorTime\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}
			strSave.Format("%02d:%02d:%02d.%03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_SpcMotionLog(CString sLog, CString sLotID)
{
	CString strFile, strTitle, strFolder, strSave;
	if (sLotID.GetLength() < 1) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	strFolder.Format("%s\\LOG\\Motion\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strFolder);

	strFile.Format("LOG/Motion/%04d/%02d/%02d/%s.txt", time.wYear, time.wMonth, time.wDay, sLotID);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,Lot ID,Barcode,Port No,Tray No,Cm No,Cm Count,Category,Axis,Reference Value,Encoder Value\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("%02d:%02d:%02d.%03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_SpcErrorLog(CString sLog, CString sLotID)
{
	CString strFile, strTitle, strFolder, strSave;
	if (sLotID.GetLength() < 1) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	strFolder.Format("%s\\LOG\\Error\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strFolder);

	strFile.Format("LOG/Error/%04d/%02d/%02d/%s.txt", time.wYear, time.wMonth, time.wDay, sLotID);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,Lot ID,ErrorCode,Category,LotID,Barcode,Port No,Tray No,Cm No,Cm Count,Text,StartTime,EndTime,UseTime\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("%02d:%02d:%02d.%03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_BarcodeLog(CString sLog)
{
	CString strPath = gsCurrentDir + "\\LOG\\Barcode";
	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_OCAPDataLog(int nIdx)
{
	CString strPath = gsCurrentDir + "\\LOG\\OCAPData";
	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave, sTitle, sFAIData1, sFAIData2, sData;
	strFile.Format("%s\\%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);
//	sTitle.Format("Time,배출매거진ID,투입,완공,양품,RN,RR,치명,BCR,MES,MC,치수,7FAI,Tilt,Gap,FAI,5LS,FAI,9LS,FAI,10LS,FAI,24,FAI,29LS,FAI,31LS,FAI,34LS,FAI,11,FAI,13,FAI,14,FAI,26,FAI,27,FAI,32,FAI,33,FAI,30,FAI,19,FAI,20,FAI,21,FAI,22,FAI,44,ERR,5LS,ERR,9LS,ERR,10LS,ERR,24,ERR,29LS,ERR,31LS,ERR,34LS,ERR,11,ERR,13,ERR,14,ERR,26,ERR,27,ERR,32,ERR,33,ERR,30,ERR,19,ERR,20,ERR,21,ERR,22,ERR,44\r\n");
	sTitle.Format("Time,배출매거진ID,DFA랏ID1,DFA랏ID2,DFA랏ID3,DFA랏ID4,DFA랏ID5,DFA랏ID6,DFA랏ID7,DFA랏ID8,투입,완공,양품,RN,RR,치명,BCR,MES,MC,치수,7FAI,Tilt,Gap,FAI,5LS,FAI,9LS,FAI,10LS,FAI,24,FAI,29LS,FAI,31LS,FAI,34LS,FAI,11,FAI,13,FAI,14,FAI,26,FAI,27,FAI,32,FAI,33,FAI,30,FAI,19,FAI,20,FAI,21,FAI,22,FAI,44,ERR,5LS,ERR,9LS,ERR,10LS,ERR,24,ERR,29LS,ERR,31LS,ERR,34LS,ERR,11,ERR,13,ERR,14,ERR,26,ERR,27,ERR,32,ERR,33,ERR,30,ERR,19,ERR,20,ERR,21,ERR,22,ERR,44\r\n");

	sFAIData1 = sFAIData2 = "";
	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());

			int n7FAI = gCap.nCount[nIdx][0]+gCap.nCount[nIdx][1]+gCap.nCount[nIdx][2]+gCap.nCount[nIdx][3]+gCap.nCount[nIdx][4]+gCap.nCount[nIdx][5]+gCap.nCount[nIdx][6];
			int nTilt = gCap.nCount[nIdx][7]+gCap.nCount[nIdx][8]+gCap.nCount[nIdx][9]+gCap.nCount[nIdx][10]+gCap.nCount[nIdx][11]+gCap.nCount[nIdx][12]+gCap.nCount[nIdx][13]+gCap.nCount[nIdx][14];
			int nGap  = gCap.nCount[nIdx][15]+gCap.nCount[nIdx][16]+gCap.nCount[nIdx][17]+gCap.nCount[nIdx][18]+gCap.nCount[nIdx][19];
			for (int i=0; i<20; i++) {
				double dDef = (gCap.nCount[nIdx][i] * 100.0) / gCap.nTotCount[nIdx];
				sData.Format("%d,%01f%%,", gCap.nCount[nIdx][i], dDef);
				sFAIData1 = sFAIData1 + sData;
			}
			for (int i=0; i<20; i++) {
				double dDef = (gCap.nFCount[nIdx][i] * 100.0) / gCap.nTotCount[nIdx];
				sData.Format("%d,%01f%%,", gCap.nFCount[nIdx][i], dDef);
				sFAIData2 = sFAIData2 + sData;
			}
			//완공=양품+ROS Repair
			strSave.Format("%02d:%02d:%02d %03d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
				gCap.sMZID[nIdx], gCap.sLotID[nIdx][0], gCap.sLotID[nIdx][1], gCap.sLotID[nIdx][2], gCap.sLotID[nIdx][3], gCap.sLotID[nIdx][4], gCap.sLotID[nIdx][5], gCap.sLotID[nIdx][6], gCap.sLotID[nIdx][7],
				gCap.nTotCount[nIdx], (gCap.nGoodCount[nIdx]+gCap.nROSRfCount[nIdx]), gCap.nGoodCount[nIdx], gCap.nROSNGCount[nIdx], gCap.nROSRfCount[nIdx],
				(gCap.nBCRCount[nIdx]+gCap.nMESCount[nIdx]+gCap.nMCCount[nIdx]), gCap.nBCRCount[nIdx], gCap.nMESCount[nIdx], gCap.nMCCount[nIdx],
				(n7FAI+nTilt+nGap), n7FAI, nTilt, nGap, sFAIData1, sFAIData2);

/*
			strSave.Format("%02d:%02d:%02d %03d,%s,%s,%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
				gCap.sMZID[nIdx], gCap.sLotID[nIdx][0], gCap.sLotID[nIdx][1], gCap.sLotID[nIdx][2], gCap.sLotID[nIdx][3], gCap.sLotID[nIdx][4], gCap.sLotID[nIdx][5], gCap.sLotID[nIdx][6], gCap.sLotID[nIdx][7],
				gCap.nTotCount[nIdx], (gCap.nGoodCount[nIdx]+gCap.nROSNGCount[nIdx]+gCap.nROSRfCount[nIdx]), gCap.nGoodCount[nIdx], gCap.nROSNGCount[nIdx], gCap.nROSRfCount[nIdx],
				(gCap.nBCRCount[nIdx]+gCap.nMESCount[nIdx]+gCap.nMCCount[nIdx]), gCap.nBCRCount[nIdx], gCap.nMESCount[nIdx], gCap.nMCCount[nIdx],
				(n7FAI+nTilt+nGap), n7FAI, nTilt, nGap, sFAIData1, sFAIData2);
*/
			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_RFBarData(int nType, CString strDataID, int nCount)
{
	//0:BL1F, 1:BL2F, 2:BU1F, 3:BU2F, 4:RFL, 5:RFG, 6:RFN
	CString strPath = gsCurrentDir + "\\LOG\\Barcode";
	Create_Folder(strPath);

	CString strFile, strSave, sData;
	if (nType == 0) sData.Format("MZID-Load-Module:%s", strDataID);
	if (nType == 1) sData.Format("MZID-Load-NG    :%s", strDataID);
	if (nType == 2) sData.Format("MZID-Unload-Good:%s", strDataID);
	if (nType == 3) sData.Format("MZID-Unload-NG  :%s", strDataID);
	if (nType == 4) sData.Format("RFID-Load       :%s, Count:%d", strDataID, nCount);
	if (nType == 5) sData.Format("RFID-Good       :%s", strDataID);
	if (nType == 6) sData.Format("RFID-NG         :%s", strDataID);

	SYSTEMTIME time;
	GetLocalTime(&time);

	if (nType == 0 || nType == 1 ||nType == 4)
		strFile.Format("%s\\%04d%02d%02d-Input.txt", strPath, time.wYear, time.wMonth, time.wDay);
	else
		strFile.Format("%s\\%04d%02d%02d-Output.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone)) {
		try {
			file.SeekToEnd();

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sData);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}
