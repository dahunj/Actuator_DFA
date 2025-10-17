// OCAPCosmeticProcess.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMAI2100.h"
#include "OCAPCosmeticProcess.h"
#include "Common.h"
#include "LogFile.h"
#include "DataManager.h"
#include "afxdialogex.h"


// OCAPCosmeticProcess 대화 상자입니다.
OCAPCosmeticProcess g_dlgOCAP;

IMPLEMENT_DYNAMIC(OCAPCosmeticProcess, CDialogEx)

OCAPCosmeticProcess::OCAPCosmeticProcess(CWnd* pParent /*=NULL*/)
	: CDialogEx(OCAPCosmeticProcess::IDD, pParent)
{

}

OCAPCosmeticProcess::~OCAPCosmeticProcess()
{
}

void OCAPCosmeticProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 9; i++) DDX_Control(pDX, IDC_STC_OCAP_VALUE_0 + i, m_stcOption[i]);
	DDX_Control(pDX, IDC_GRD_OCAP_DATA, m_grdData);
}


BEGIN_MESSAGE_MAP(OCAPCosmeticProcess, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDCANCEL, &OCAPCosmeticProcess::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK,		&OCAPCosmeticProcess::OnBnClickedOk)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_OCAP_VALUE_0, IDC_STC_OCAP_VALUE_8, OnStcOptionClick)
	ON_BN_CLICKED(IDC_BTN_TEST, &OCAPCosmeticProcess::OnBnClickedBtnTest)
END_MESSAGE_MAP()


BOOL OCAPCosmeticProcess::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL OCAPCosmeticProcess::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void OCAPCosmeticProcess::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		Display_Option();
		Initial_NameGrid(&m_grdData, 50, 96);
		Display_Status();
	} else {
	}
}

// OCAPCosmeticProcess 메시지 처리기입니다.

void OCAPCosmeticProcess::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}


void OCAPCosmeticProcess::OnBnClickedOk()
{
	int		nData1, nData2, nData3, nData4, nData5;
	double	dData1, dData2, dData3, dData4;
	CString strData, strLog;

	CIniFileCS INI(gsCurrentDir + "\\System\\OCAPData.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("OCAPData.ini File Not Found!");
		return;
	}

	m_stcOption[0].GetWindowText(strData); nData1 = atoi(strData); INI.Set_Integer("OPTION", "CONS_MZ_COUNT_01", nData1);
	m_stcOption[1].GetWindowText(strData); dData1 = atof(strData); INI.Set_Double("OPTION", "DEFECT_PERCENT_01", dData1, "%0.3lf");
	m_stcOption[3].GetWindowText(strData); nData2 = atoi(strData); INI.Set_Integer("OPTION", "CONS_MZ_COUNT_02", nData2);
	m_stcOption[4].GetWindowText(strData); dData2 = atof(strData); INI.Set_Double("OPTION", "DEFECT_PERCENT_02", dData2, "%0.3lf");
	m_stcOption[5].GetWindowText(strData); nData3 = atoi(strData); INI.Set_Integer("OPTION", "CONS_MZ_COUNT_03", nData3);
	m_stcOption[6].GetWindowText(strData); dData3 = atof(strData); INI.Set_Double("OPTION", "DEFECT_PERCENT_03", dData3, "%0.3lf");
	m_stcOption[7].GetWindowText(strData); nData4 = atoi(strData); INI.Set_Integer("OPTION", "CONS_MZ_COUNT_04", nData4);
	m_stcOption[8].GetWindowText(strData); dData4 = atof(strData); INI.Set_Double("OPTION", "DEFECT_PERCENT_04", dData4, "%0.3lf");
	m_stcOption[2].GetWindowText(strData); nData5 = atoi(strData); INI.Set_Integer("OPTION", "MODULE_MIN_PER_MZ", nData5);

	strLog.Format("[OCAP Option] OnBnClickedOk - Data1(%d,%0.3lf) Data2(%d,%0.3lf) Data3(%d,%0.3lf)  Data4(%d,%0.3lf) Skip(%d)", nData1, dData1, nData2, dData2, nData3, dData3, nData4, dData4, nData5);
	g_objLogFile.Save_HandlerLog(strLog);

	g_objDataManager.Read_OCAPData();
	Display_Option();
	Display_Status();
}

void OCAPCosmeticProcess::Initial_Controls() 
{
	for (int i = 0; i < 2; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 6; i++) m_Label[i].Init_Ctrl("바탕", 12, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x00, 0x80));
	m_Label[0].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	m_Label[4].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	m_Label[5].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	m_Label[6].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	for (int i = 0; i < 9; i++) m_stcOption[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	
}

void OCAPCosmeticProcess::Display_Option()
{
	CString strData;
	strData.Format("%d", gCap.nConsecutiveMZLimit[0]);	m_stcOption[0].SetWindowText(strData);
	strData.Format("%0.3lf", gCap.dDefectPercent[0]);	m_stcOption[1].SetWindowText(strData);
	strData.Format("%d", gCap.nConsecutiveMZLimit[1]);	m_stcOption[3].SetWindowText(strData);
	strData.Format("%0.3lf", gCap.dDefectPercent[1]);	m_stcOption[4].SetWindowText(strData);
	strData.Format("%d", gCap.nConsecutiveMZLimit[2]);	m_stcOption[5].SetWindowText(strData);
	strData.Format("%0.3lf", gCap.dDefectPercent[2]);	m_stcOption[6].SetWindowText(strData);
	strData.Format("%d", gCap.nConsecutiveMZLimit[3]);	m_stcOption[7].SetWindowText(strData);
	strData.Format("%0.3lf", gCap.dDefectPercent[3]);	m_stcOption[8].SetWindowText(strData);
	strData.Format("%d", gCap.nMinModuleCnt);			m_stcOption[2].SetWindowText(strData);

	//CIniFileCS INI(gsCurrentDir + "\\System\\OCAPData.ini");
	//if (!INI.Check_File()) {
	//	AfxMessageBox("OCAPData.ini File Not Found!");
	//	return;
	//}

	//int		nData1, nData2, nData3, nData4, nData5;
	//double	dData1, dData2, dData3, dData4;
	//CString strData, strLog;

	//nData1 = INI.Get_Integer("OPTION", "MZ_COUNT_01", 1); 
	//strData.Format("%d", nData1);
	//m_stcOption[0].SetWindowText(strData);

	//nData2 = INI.Get_Integer("OPTION", "MZ_COUNT_02", 1);
	//strData.Format("%d", nData2);
	//m_stcOption[3].SetWindowText(strData);

	//nData3 = INI.Get_Integer("OPTION", "MZ_COUNT_03", 1);
	//strData.Format("%d", nData3);
	//m_stcOption[5].SetWindowText(strData); 

	//nData4 = INI.Get_Integer("OPTION", "MZ_COUNT_04", 3);
	//strData.Format("%d", nData4);
	//m_stcOption[7].SetWindowText(strData); 

	//dData1 =INI.Get_Double("OPTION", "DEFECT_PERCENT_01", 3);
	//strData.Format("%0.3lf", dData1);
	//m_stcOption[1].SetWindowText(strData);

	//dData2 =INI.Get_Double("OPTION", "DEFECT_PERCENT_02", 3);
	//strData.Format("%0.3lf", dData2);
	//m_stcOption[4].SetWindowText(strData);

	//dData3 =INI.Get_Double("OPTION", "DEFECT_PERCENT_03", 9);
	//strData.Format("%0.3lf", dData3);
	//m_stcOption[6].SetWindowText(strData);

	//dData4 =INI.Get_Double("OPTION", "DEFECT_PERCENT_04", 3);
	//strData.Format("%0.3lf", dData4);
	//m_stcOption[8].SetWindowText(strData);	 

	//nData5 = INI.Get_Integer("OPTION", "MODULE_MIN_PER_MZ", 150);
	//strData.Format("%d", nData5);
	//m_stcOption[2].SetWindowText(strData); 
	
}

void OCAPCosmeticProcess::OnStcOptionClick(UINT nID)
{
	int nIndex = nID - IDC_STC_OCAP_VALUE_0;

	CString strOld, strNew, strTemp;
	m_stcOption[nIndex].GetWindowText(strOld);

	if(nIndex == 0) // 조건 1, 매거진 연속 수량 
	{
		if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

		int nLen = strNew.GetLength();
		if (nLen < 1) return;

		for (int i = 0; i < nLen; i++ ) {
			strTemp = strNew.Mid(i, 1);
			if (strTemp < "0" || strTemp > "9") {
				AfxMessageBox("Input Error......");
				return;
			}
		}
		m_stcOption[nIndex].SetWindowText(strNew);
	}
	else if(nIndex == 1) //조건 1, 불량율 
	{
		if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;	
		m_stcOption[nIndex].SetWindowText(strNew);
	}
	else if(nIndex == 2) // 매거진당 모듈 최소 수량 
	{
		if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

		int nLen = strNew.GetLength();
		if (nLen < 1) return;
		m_stcOption[nIndex].SetWindowText(strNew);
	}
	else if(nIndex == 3) // 조건 2, 매거진 연속 수량
	{
		if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

		int nLen = strNew.GetLength();
		if (nLen < 1) return;

		for (int i = 0; i < nLen; i++ ) {
			strTemp = strNew.Mid(i, 1);
			if (strTemp < "0" || strTemp > "9") {
				AfxMessageBox("Input Error......");
				return;
			}
		}
		m_stcOption[nIndex].SetWindowText(strNew);
	}
	else if(nIndex == 4) //조건 2, 불량율 
	{
		if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;	
		m_stcOption[nIndex].SetWindowText(strNew);
	}
	else if(nIndex == 5) // 조건 3, 매거진 연속 수량
	{
		if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

		int nLen = strNew.GetLength();
		if (nLen < 1) return;

		for (int i = 0; i < nLen; i++ ) {
			strTemp = strNew.Mid(i, 1);
			if (strTemp < "0" || strTemp > "9") {
				AfxMessageBox("Input Error......");
				return;
			}
		}
		m_stcOption[nIndex].SetWindowText(strNew);
	}
	else if(nIndex == 6) //조건 3, 불량율 
	{
		if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;	
		m_stcOption[nIndex].SetWindowText(strNew);
	}
	else if(nIndex == 7)
	{

	}
	else if(nIndex == 8)
	{

	}

}

void OCAPCosmeticProcess::Initial_NameGrid(CGridCS *pGrid, int nRows, int nCols)
{
	pGrid->Set_RowCount(nRows);
	pGrid->Set_ColCount(nCols);
	pGrid->Set_FixRowCount(0);
	pGrid->Set_FixColCount(0);

	long lTotalW = pGrid->Get_GridWidth();
	long lTotalH = pGrid->Get_GridHeight();
	long lCellW = lTotalW / nCols;
	long lCellH = (lTotalH / nRows) + 2;
	long  lWidth[7]= { 140,   140,      140,          60,     60,      60,    100  };
	CString sTxt[7]= {"날짜","시간","배출매거진ID", "투입", "완공", "양품", "OCAP NG"};
	long  lWidthName[2]= { 40, 50 };

	for (int i=0; i<nRows; i++)
	{
		pGrid->Set_RowHeight(i, lCellH);
		for (int j=0; j<7; j++) 
		{
			if(i==0) 
			{
				pGrid->Set_ColWidth(j, lWidth[j]); pGrid->Set_CellText(i, j, sTxt[j]);
				pGrid->Set_CellBackClr(i, j, RGB(0xCC, 0xCC, 0xCC));
			}
		}		
	}
	
	for (int i=0; i < nRows; i++)
	{
		pGrid->Set_RowHeight(i, lCellH);
		for (int j=7; j < (7 + gCap.nCosmeticCount); j++) 
		{
			if(i==0) 
			{
				pGrid->Set_ColWidth(j, 160); pGrid->Set_CellText(i, j, gCap.sCosmeticCode[j-7]);
				pGrid->Set_CellBackClr(i, j, RGB(0xCC, 0xCC, 0xCC));
			}
		}		
	}


}

void OCAPCosmeticProcess::Initial_DataGrid(CGridCS *pGrid, int nRows, int nCols)
{
	pGrid->Set_RowCount(nRows);
	pGrid->Set_ColCount(nCols);
	pGrid->Set_FixRowCount(0);
	pGrid->Set_FixColCount(0);

	long lTotalW = pGrid->Get_GridWidth();
	long lTotalH = pGrid->Get_GridHeight();
	long lCellW = lTotalW / nCols;
	long lCellH = (lTotalH / nRows);
	long  lWidth[50]= { 35, 105, 50, 33, 50};
	for (int i=5; i<nCols; i=i+2) {
		lWidth[i] = lWidth[3];
	}
	for (int i=6; i<nCols; i=i+2) {
		lWidth[i] = lWidth[4];
	}

	for (int i=0; i<nRows; i++) {
		pGrid->Set_RowHeight(i, lCellH);
		for (int j=0; j<nCols; j++) {
			if(i==0) {
				pGrid->Set_ColWidth(j, lWidth[j]);
				pGrid->Set_CellBackClr(i, j, RGB(0xF0, 0xF0, 0xC0));
			}
		}
	}
}

void OCAPCosmeticProcess::Display_Status()
{
	
	//gCap.nMZCycle = 1;
	//gCap.sDate[0] = "1/1";			
	//gCap.sTime[0] = "01:01";			
	//gCap.sMZID[0] = "AAAAAAAAAA";			
	//gCap.nTotCount[0] = 320;		
	//gCap.nGoodCount[0] = 2;		
	//gCap.nConsmeticNGCount[0] = 150;
	//


	if (gCap.nMZCycle < 0 || gCap.nMZCycle > 49) return;

	int nD = 0;
	int nS = gCap.nMZCycle - 1;
	if (nS < 0) nS = 49;
	for(int i=nS; i>=0; i--) {
		if (gCap.sDate[i].GetLength() < 1) break;
		nD++;
		Display_Grid(nD, i);
	}
	/*for(int i=49; i>=0; i--) {
	if (gCap.sDate[i].GetLength() < 1) break;
	nD++;
	Display_Grid(nD, i);
	if (nD >= 51) return;
	}*/
}

void OCAPCosmeticProcess::Display_Grid(int nDp, int nIx)
{
	if (nDp < 1 || nDp > 50 || nIx < 0 || nIx > 49) return;
	CString str;

	str.Format(_T("%s"), gCap.sDate[nIx]);
	m_grdData.Set_CellFont(nDp, 0, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 0, str);

	str.Format(_T("%s"), gCap.sTime[nIx]);
	m_grdData.Set_CellFont(nDp, 1, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 1, str);

	str.Format(_T("%s"), gCap.sMZID[nIx]);
	m_grdData.Set_CellFont(nDp, 2, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 2, str);

	str.Format(_T("%d"), gCap.nTotCount[nIx]);
	m_grdData.Set_CellFont(nDp, 3, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 3, str);

	//	str.Format(_T("%d"), gCap.nGoodCount[nIx] + gCap.nROSNGCount[nIx] + gCap.nROSRfCount[nIx]);
	

	str.Format(_T("%d"), gCap.nGoodCount[nIx]);
	m_grdData.Set_CellFont(nDp, 5, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 5, str);

	str.Format(_T("%d"), gCap.nConsmeticNGCount[nIx]);
	m_grdData.Set_CellFont(nDp, 6, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 6, str);

	int temp = 0;
	for(int i = 7; i < (7 + gCap.nCosmeticCount); i++)
	{
		temp += gCap.nCosmeticDefectMZ[i-7][nIx];
		str.Format(_T("%d"), gCap.nCosmeticDefectMZ[i-7][nIx]); 
		m_grdData.Set_CellFont(nDp, i, str, 10, FALSE);
		m_grdData.Set_CellText(nDp, i, str);
	}
	
	str.Format(_T("%d"), gCap.nGoodCount[nIx]+ temp);
	m_grdData.Set_CellFont(nDp, 4, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 4, str);

	
	
}

void OCAPCosmeticProcess::AddModuleToCarrier(int nSlotNo, CString sType, int nJudge, CString sNGCode, int& nCurrentMzIdx)
{
	CString strTemp;

	if(sType == "GOOD")
	{
		gCap.nCarrierGood[nSlotNo]++;
		gCap.nTotalCntCarrier[nSlotNo]++;		
			
		strTemp.Format("Good, nCarrierGood:%d, nTotalCntCarrier:%d, nSlotNo:%d, sNGCode:%s",gCap.nCarrierGood[nSlotNo],gCap.nTotalCntCarrier[nSlotNo],nSlotNo, sNGCode);
		g_objLogFile.Save_TestLog(strTemp);
	}
	else
	{
		if(nJudge == 3 || nJudge == 5)
		{
			for(int i = 0; i < 50; i++)
			{
				if(sNGCode == gCap.sCosmeticCode[i] && sNGCode !="")
				{
					gCap.nCarrierCosmeticDefect[i][nSlotNo]++;
					gCap.nTotalCntCarrier[nSlotNo] ++;
					strTemp.Format("NG, nCarrierCosmeticDefect:%d, nTotalCntCarrier:%d, nSlotNo:%d,sNGCode:%s,%s", gCap.nCarrierCosmeticDefect[i][nSlotNo],gCap.nTotalCntCarrier[nSlotNo],nSlotNo, sNGCode, gCap.sCosmeticCode[i]);
					g_objLogFile.Save_TestLog(strTemp);
				}
				/*else{
					strTemp.Format("NG, What is this?,sNGCode:%s", sNGCode);
					g_objLogFile.Save_TestLog(strTemp);
				}*/
			}						
		}
		else if(nJudge == 7 || nJudge == 8 || nJudge == 9 || nJudge == 17 || nJudge == 18 || nJudge == 19)
		{
			for(int i = 0; i < 50; i++)
			{
				if(sNGCode == gCap.sFAICode[i] && sNGCode != "")
				{
					gCap.nCarrierFAIDefect[i][nSlotNo] ++;
					gCap.nTotalCntCarrier[nSlotNo]++;
					strTemp.Format("NG, nCarrierFAIDefect:%d, nTotalCntCarrier:%d, nSlotNo:%d,sNGCode:%s,%s", gCap.nCarrierFAIDefect[i][nSlotNo],gCap.nTotalCntCarrier[nSlotNo],nSlotNo, sNGCode, gCap.sFAICode[i]);
					g_objLogFile.Save_TestLog(strTemp);
				}
			}		
		}
		else{
			strTemp.Format("NG, What is this?,sNGCode:%s", sNGCode);
			g_objLogFile.Save_TestLog(strTemp);
		}
	}
}

void OCAPCosmeticProcess::AddCarToMZ(int nSlotNo, CString sType)
{
	CString strLog; 

	
	if(sType == "GOOD")
	{
		gCap.nGoodInMZ[gCap.nOcapMzIndex] += gCap.nCarrierGood[nSlotNo];
		strLog.Format("AddCarToMZ,gCap.nGoodInMZ:%d, nSlotNo: %d  CurrentMZ index:%d", gCap.nGoodInMZ[gCap.nOcapMzIndex],nSlotNo, gCap.nOcapMzIndex);
		g_objLogFile.Save_TestLog(strLog);

	}		
	else if(sType == "NG")
	{
		for(int i = 0; i < 50; i++)
		{
			gCap.nFAIDefectMZ[i][gCap.nOcapMzIndex] += gCap.nCarrierFAIDefect[i][nSlotNo];
			gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex] += gCap.nCarrierCosmeticDefect[i][nSlotNo];
			
			strLog.Format("AddCarToMZ,gCap.nCosmeticDefectMZ:%d,nSlotNo:%d,nOcapMzIndex:%d", gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex],nSlotNo,gCap.nOcapMzIndex);
			g_objLogFile.Save_TestLog(strLog);
		}	
		
	}

}

void OCAPCosmeticProcess::AddMZOut(CString sMZid, CString sType)
{
	CString strLog; 
		
	if(sType == "NG") return;
	
	double dPer = 0;
		
	//Cosmetic case 1 check 
	for(int i = 0; i < 50; i++)
	{
		int nCntTemp = 0;
		for(int j = 0; j < 50; j++)
		{			
			nCntTemp += gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex];			
			g_objLogFile.Save_TestLog(strLog);
		}
		gCap.nTotalCntMZ[gCap.nOcapMzIndex] = gCap.nGoodInMZ[gCap.nOcapMzIndex] + nCntTemp;

		dPer = (gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex] * 100.0) / gCap.nTotalCntMZ[gCap.nOcapMzIndex];
		
		// 불량율 OCAP 기준보다 낮으면 Continue
		if (dPer < gCap.dDefectPercent[0])
		{			
			continue;			
		}		

		// 연속 횟수 도달시 알람 발생 
		if(gCap.nConsecutiveMZCount[0][i] + 1 < gCap.nConsecutiveMZLimit[0])
		{
			gCap.nConsecutiveMZCount[0][i]++;
			continue;
		}
		else
		{
			gCap.nConsecutiveMZCount[0][i]++; 
		}
		
		gCap.sAlmMZID = sMZid;
		gCap.sAlmDefectName.Format("%s", gCap.sCosmeticName[i]);
		gCap.dAlmDefectPercent  = dPer;
		gCap.nAlmNGCount   = gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex];
				
		if(gCap.nMZCycle == 50)  gCap.nMZCycle = 1;
		else gCap.nMZCycle++;

		SYSTEMTIME time;
		GetLocalTime(&time);

		m_strLog.Format("%04d/%02d/%02d", time.wYear, time.wMonth, time.wDay);
		gCap.sDate[gCap.nMZCycle-1] = m_strLog;

		m_strLog.Format("%02d:%02d:%02d.%03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
		gCap.sTime[gCap.nMZCycle-1] = m_strLog;

		gCap.sMZID[gCap.nMZCycle - 1] = sMZid;
		gCap.nTotCount[gCap.nMZCycle - 1] = gCap.nTotalCntMZ[gCap.nOcapMzIndex];
		gCap.nGoodCount[gCap.nMZCycle - 1] = gCap.nGoodInMZ[gCap.nOcapMzIndex];
		gCap.nConsmeticNGCount[gCap.nMZCycle - 1] = gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex];
		gCap.nCosmeticCodeNum[0] = i+1;

		Display_Status();		

		if(gCap.nTotalCntMZ[gCap.nOcapMzIndex] > gCap.nMinModuleCnt )
		{
			gCap.bErrorShowDone = FALSE;
			g_objCommon.Show_Error(9181,1);
		}

		//에러창 쓰레드라서 
		int nTick = 0;
		while(!gCap.bErrorShowDone)
		{
			nTick++;
			if(nTick > 50) break;
		} 
		gCap.bErrorShowDone = FALSE;

		memset(gCap.nConsecutiveMZCount[0],0,sizeof(int)*50); //에러율 초과 발생시 연속 발생 횟수 초기화 
		gCap.bOCAPDone[0] = TRUE;

		//후처리 		
		break;
		
	}

	////Cosmetic Case 2 Check 
	for(int i = 0; i < 50; i++)
	{
		//if(gCap.bOCAPDone[0]) break;

		int nCntTemp = 0;
		for(int j = 0; j < 50; j++)
		{
			nCntTemp += gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex];			
			g_objLogFile.Save_TestLog(strLog);
		}
		gCap.nTotalCntMZ[gCap.nOcapMzIndex] = gCap.nGoodInMZ[gCap.nOcapMzIndex] + nCntTemp;

		dPer = (gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex] * 100.0) / gCap.nTotalCntMZ[gCap.nOcapMzIndex];

		if (dPer < gCap.dDefectPercent[1])
		{

			continue;			
		}
				
		// 연속 횟수 도달시 알람 발생 
		if(gCap.nConsecutiveMZCount[1][i] + 1 < gCap.nConsecutiveMZLimit[1])
		{
			gCap.nConsecutiveMZCount[1][i]++;
			continue;
		}
		else
		{
			gCap.nConsecutiveMZCount[1][i]++; 
		}
		
		gCap.sAlmMZID = sMZid;
		gCap.sAlmDefectName.Format("%s", gCap.sCosmeticName[i]);
		gCap.dAlmDefectPercent  = dPer;
		gCap.nAlmNGCount   = gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex];

		if(gCap.nMZCycle == 50)  gCap.nMZCycle = 1;
		else gCap.nMZCycle++;

		SYSTEMTIME time;
		GetLocalTime(&time);

		m_strLog.Format("%04d/%02d/%02d", time.wYear, time.wMonth, time.wDay);
		gCap.sDate[gCap.nMZCycle-1] = m_strLog;

		m_strLog.Format("%02d:%02d:%02d.%03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
		gCap.sTime[gCap.nMZCycle-1] = m_strLog;

		gCap.sMZID[gCap.nMZCycle - 1] = sMZid;
		gCap.nTotCount[gCap.nMZCycle - 1] = gCap.nTotalCntMZ[gCap.nOcapMzIndex];
		gCap.nGoodCount[gCap.nMZCycle - 1] = gCap.nGoodInMZ[gCap.nOcapMzIndex];
		gCap.nConsmeticNGCount[gCap.nMZCycle - 1] = gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex];
		gCap.nCosmeticCodeNum[1] = i+1;
	

		Display_Status();

		if(gCap.nTotalCntMZ[gCap.nOcapMzIndex] > gCap.nMinModuleCnt )
		{
			gCap.bErrorShowDone = FALSE;
			g_objCommon.Show_Error(9182, 1);
			
		}

		//에러창 쓰레드라서 
		int nTick = 0;
		while(!gCap.bErrorShowDone)
		{			
			nTick++;
			if(nTick > 50) break;
		} 
		gCap.bErrorShowDone = FALSE;
		memset(gCap.nConsecutiveMZCount[1],0,sizeof(int)*50); //에러율 초과 발생시 연속 발생 횟수 초기화 
		gCap.bOCAPDone[1] = TRUE;
		
		//후처리 		
		break;
	}

	//
	////Cosmetic Case 3 Check 
	//for(int i = 0; i < 50; i++)
	//{
	//	if(gCap.bOCAPDone) break;

	//	dPer = (gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex] * 100.0) / gCap.nTotalCntMZ[gCap.nOcapMzIndex];

	//	if (dPer < gCap.dDefectPercent[2])
	//	{
	//		if(i == 49) gCap.nConsecutiveMZCount[2] = 0;// 알람 없이 넘어가면 연속 발생 매거진 횟수 초기화 
	//		continue;			
	//	}

	//	gCap.nConsecutiveMZCount[2]++; //알람 발생시 연속 발생 매거진 횟수 증가 
	//	if(gCap.nConsecutiveMZCount[2] < gCap.nConsecutiveMZLimit[2]) continue;

	//	gCap.sAlmMZID = sMZid;
	//	gCap.sAlmDefectName.Format("%s", gCap.sCosmeticName[i]);
	//	gCap.dAlmDefectPercent  = dPer;
	//	gCap.nAlmNGCount   = gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex];

	//	if(gCap.nMZCycle == 50)  gCap.nMZCycle = 1;
	//	else gCap.nMZCycle++;

	//	SYSTEMTIME time;
	//	GetLocalTime(&time);

	//	m_strLog.Format("%04d/%02d/%02d", time.wYear, time.wMonth, time.wDay);
	//	gCap.sDate[gCap.nMZCycle-1] = m_strLog;

	//	m_strLog.Format("%02d:%02d:%02d.%03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	//	gCap.sTime[gCap.nMZCycle-1] = m_strLog;

	//	gCap.sMZID[gCap.nMZCycle - 1] = sMZid;
	//	gCap.nTotCount[gCap.nMZCycle - 1] = gCap.nTotalCntMZ[gCap.nOcapMzIndex];
	//	gCap.nGoodCount[gCap.nMZCycle - 1] = gCap.nGoodInMZ[gCap.nOcapMzIndex];
	//	gCap.nConsmeticNGCount[gCap.nMZCycle - 1] = gCap.nCosmeticDefectMZ[i][gCap.nOcapMzIndex];

	//	Display_Status();

	//	g_objCommon.Show_Error(9183);

	//	//후처리 
	//	if(gCap.nOcapMzIndex == 49) gCap.nOcapMzIndex = 0;
	//	else gCap.nOcapMzIndex++;
	//	break;

	//}
	if(gCap.bOCAPDone[0] == TRUE || gCap.bOCAPDone[1] == TRUE)
	{
		if(gCap.nOcapMzIndex == 49) gCap.nOcapMzIndex = 0;
		else gCap.nOcapMzIndex++;

		gCap.bOCAPDone[0] = FALSE;
		gCap.bOCAPDone[1] = FALSE;
		gCap.bOCAPDone[2] = FALSE;

	}
}


void OCAPCosmeticProcess::Set_AddMZData(int nPortNo)
{
	
}


void OCAPCosmeticProcess::DelMZData(int nMZNo)
{
	
}



void OCAPCosmeticProcess::Set_AddDEFECT(CString sMZid,int nPortNo, int nInfo)
{
	
}

void OCAPCosmeticProcess::Check_DEFECT(CString sMZID)
{
	
}

void OCAPCosmeticProcess::Check_DEFECTF(int nNo)
{
}


void OCAPCosmeticProcess::OnBnClickedBtnTest()
{

	gCap.nMZCycle = 4;
	gCap.sDate[0] = "1/1";	
	gCap.sDate[1] = "1/1";	
	gCap.sDate[2] = "1/1";	
	gCap.sDate[3] = "1/1";	


	gCap.sTime[0] = "01:01";			
	gCap.sMZID[0] = "AAAAAAAAAA";			
	gCap.nTotCount[0] = 320;		
	gCap.nGoodCount[0] = 2;		
	gCap.nConsmeticNGCount[0] = 150;

	gCap.nCosmeticDefectMZ[0][0] = 15;
	gCap.nCosmeticDefectMZ[0][1] = 16;
	gCap.nCosmeticDefectMZ[0][2] = 17;
	gCap.nCosmeticDefectMZ[0][3] = 18;


	Display_Status();

	
}
