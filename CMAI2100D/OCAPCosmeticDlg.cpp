// OCAPCosmeticDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMAI2100.h"
#include "OCAPCosmeticDlg.h"
#include "Common.h"
#include "LogFile.h"
#include "DataManager.h"
#include "afxdialogex.h"

#include <cmath>
#include <float.h>

// OCAPCosmeticDlg 대화 상자입니다.
OCAPCosmeticDlg g_dlgOCAPCosmetic;

IMPLEMENT_DYNAMIC(OCAPCosmeticDlg, CDialogEx)

OCAPCosmeticDlg::OCAPCosmeticDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(OCAPCosmeticDlg::IDD, pParent)
{

}

OCAPCosmeticDlg::~OCAPCosmeticDlg()
{
}

void OCAPCosmeticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_OCAP_CONS_MZ_0 + i, m_stcConsMZ[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_STC_OCAP_NG_PERCENT_0 + i, m_stcPercent[i]);
	DDX_Control(pDX, IDC_STC_OCAP_MIN_COUNT, m_stcMinCount);
	DDX_Control(pDX, IDC_GRD_OCAP_DATA, m_grdData);
}


BEGIN_MESSAGE_MAP(OCAPCosmeticDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDCANCEL, &OCAPCosmeticDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK,		&OCAPCosmeticDlg::OnBnClickedOk)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_OCAP_CONS_MZ_0, IDC_STC_OCAP_CONS_MZ_2, OnStcConstMagazineClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_OCAP_NG_PERCENT_0, IDC_STC_OCAP_NG_PERCENT_2, OnStcNGPercentClick)
	ON_BN_CLICKED(IDC_BTN_TEST, &OCAPCosmeticDlg::OnBnClickedBtnTest)
	ON_STN_CLICKED(IDC_STC_OCAP_MIN_COUNT, &OCAPCosmeticDlg::OnStnClickedStcOcapMinCount)
END_MESSAGE_MAP()


BOOL OCAPCosmeticDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL OCAPCosmeticDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void OCAPCosmeticDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		Display_Option();
		Initial_NameGrid(&m_grdData, 50, 96);
		Display_Status();
	} else {
	}
}

// OCAPCosmeticDlg 메시지 처리기입니다.

void OCAPCosmeticDlg::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}


void OCAPCosmeticDlg::OnBnClickedOk()
{
	int		nData1, nData2, nData3, nData5;
	double	dData1, dData2, dData3;
	CString strData, strLog;

	CIniFileCS INI(gsCurrentDir + "\\System\\OCAPData.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("OCAPData.ini File Not Found!");
		return;
	}

	m_stcConsMZ[0].GetWindowText(strData); nData1 = atoi(strData); INI.Set_Integer("OPTION", "CONS_MZ_COUNT_01", nData1);
	m_stcConsMZ[1].GetWindowText(strData); nData2 = atoi(strData); INI.Set_Integer("OPTION", "CONS_MZ_COUNT_02", nData2);
	m_stcConsMZ[2].GetWindowText(strData); nData3 = atoi(strData); INI.Set_Integer("OPTION", "CONS_MZ_COUNT_03", nData3);
	
	m_stcPercent[0].GetWindowText(strData); dData1 = atof(strData); INI.Set_Double("OPTION", "DEFECT_PERCENT_01", dData1, "%0.3lf");
	m_stcPercent[1].GetWindowText(strData); dData2 = atof(strData); INI.Set_Double("OPTION", "DEFECT_PERCENT_02", dData2, "%0.3lf");
	m_stcPercent[2].GetWindowText(strData); dData3 = atof(strData); INI.Set_Double("OPTION", "DEFECT_PERCENT_03", dData3, "%0.3lf");
		
	m_stcMinCount.GetWindowText(strData); nData5 = atoi(strData); INI.Set_Integer("OPTION", "MODULE_MIN_PER_MZ", nData5);

	strLog.Format("[OCAP Option] OnBnClickedOk - Data1(%d,%0.3lf) Data2(%d,%0.3lf) Data3(%d,%0.3lf)   Skip(%d)", nData1, dData1, nData2, dData2, nData3, dData3, nData5);
	g_objLogFile.Save_HandlerLog(strLog);

	g_objDataManager.Read_OCAPData();
	Display_Option();
	Display_Status();
}

void OCAPCosmeticDlg::Initial_Controls() 
{
	for (int i = 0; i < 2; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 6; i++) m_Label[i].Init_Ctrl("바탕", 12, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x00, 0x80));
	m_Label[0].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	m_Label[4].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	m_Label[5].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));	
	
	for (int i = 0; i < 3; i++) m_stcConsMZ[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 3; i++) m_stcPercent[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	m_stcMinCount.Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT , RGB(0xFF, 0xFF, 0xE0));	
}

void OCAPCosmeticDlg::Display_Option()
{
	CString strData;
	strData.Format("%d", gCap.nConsecutiveMZLimit[0]);	m_stcConsMZ[0].SetWindowText(strData);
	
	strData.Format("%d", gCap.nConsecutiveMZLimit[1]);	m_stcConsMZ[1].SetWindowText(strData);
	
	strData.Format("%d", gCap.nConsecutiveMZLimit[2]);	m_stcConsMZ[2].SetWindowText(strData);

	strData.Format("%0.3lf", gCap.dDefectPercent[0]);	m_stcPercent[0].SetWindowText(strData);
	strData.Format("%0.3lf", gCap.dDefectPercent[1]);	m_stcPercent[1].SetWindowText(strData);
	strData.Format("%0.3lf", gCap.dDefectPercent[2]);	m_stcPercent[2].SetWindowText(strData);

	strData.Format("%d", gCap.nMinModuleCnt);			m_stcMinCount.SetWindowText(strData);

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

void OCAPCosmeticDlg::OnStcConstMagazineClick(UINT nID)
{
	int nIndex = nID - IDC_STC_OCAP_CONS_MZ_0;

	CString strOld, strNew, strTemp;
	m_stcConsMZ[nIndex].GetWindowText(strOld);

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
		m_stcConsMZ[nIndex].SetWindowText(strNew);
	}
	else if(nIndex == 1) // 조건 2, 매거진 연속 수량
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
		m_stcConsMZ[nIndex].SetWindowText(strNew);
	}
	else if(nIndex == 2) // 조건 3, 매거진 연속 수량
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
		m_stcConsMZ[nIndex].SetWindowText(strNew);
	}
}

void OCAPCosmeticDlg::OnStcNGPercentClick(UINT nID)
{
	int nIndex = nID - IDC_STC_OCAP_NG_PERCENT_0;

	CString strOld, strNew, strTemp;
	m_stcConsMZ[nIndex].GetWindowText(strOld);
		
	if(nIndex == 0) //조건 1, 불량율 
	{
		if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;	
		m_stcPercent[nIndex].SetWindowText(strNew);
	}
	else if(nIndex == 1) //조건 2, 불량율 
	{
		if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;	
		m_stcPercent[nIndex].SetWindowText(strNew);
	}
	
	else if(nIndex == 2) //조건 3, 불량율 
	{
		if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;	
		m_stcPercent[nIndex].SetWindowText(strNew);
	}
}

void OCAPCosmeticDlg::OnStnClickedStcOcapMinCount()
{
	// 매거진당 모듈 최소 수량 
	CString strOld, strNew;

	m_stcMinCount.GetWindowText(strOld);

	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	int nLen = strNew.GetLength();
	if (nLen < 1) return;
	m_stcMinCount.SetWindowText(strNew);

}

void OCAPCosmeticDlg::Initial_NameGrid(CGridCS *pGrid, int nRows, int nCols)
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

void OCAPCosmeticDlg::Initial_DataGrid(CGridCS *pGrid, int nRows, int nCols)
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

void OCAPCosmeticDlg::Display_Status()
{
	
	//gCap.nMZCycle = 1;
	//gCap.sDate[0] = "1/1";			
	//gCap.sTime[0] = "01:01";			
	//gCap.sMZID[0] = "AAAAAAAAAA";			
	//gCap.nTotCount[0] = 320;		
	//gCap.nGoodCount[0] = 2;		
	//gCap.nConsmeticNGCount[0] = 150;
	//


	if (gCap.nMZCycle_Cosmetic < 0 || gCap.nMZCycle_Cosmetic > 49) return;

	int nD = 0;
	int nS = gCap.nMZCycle_Cosmetic - 1;
	if (nS < 0) nS = 49;
	for(int i=nS; i>=0; i--) {
		if (gCap.sDate_Cosmetic[i].GetLength() < 1) break;
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

void OCAPCosmeticDlg::Display_Grid(int nDp, int nIx)
{
	if (nDp < 1 || nDp > 50 || nIx < 0 || nIx > 49) return;
	CString str;

	str.Format(_T("%s"), gCap.sDate_Cosmetic[nIx]);
	m_grdData.Set_CellFont(nDp, 0, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 0, str);

	str.Format(_T("%s"), gCap.sTime_Cosmetic[nIx]);
	m_grdData.Set_CellFont(nDp, 1, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 1, str);

	str.Format(_T("%s"), gCap.sMZID_Cosmetic[nIx]);
	m_grdData.Set_CellFont(nDp, 2, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 2, str);

	str.Format(_T("%d"), gCap.nTotCount_Cosmetic[nIx]);
	m_grdData.Set_CellFont(nDp, 3, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 3, str);

	//	str.Format(_T("%d"), gCap.nGoodCount[nIx] + gCap.nROSNGCount[nIx] + gCap.nROSRfCount[nIx]);
	

	str.Format(_T("%d"), gCap.nGoodCount_Cosmetic[nIx]);
	m_grdData.Set_CellFont(nDp, 5, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 5, str);

	str.Format(_T("%d"), gCap.nNGCount_Consmetic[nIx]);
	m_grdData.Set_CellFont(nDp, 6, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 6, str);

	int temp = 0;
	for(int i = 7; i < (7 + gCap.nCosmeticCount); i++)
	{
		temp += gCap.nCosmeticCnt_MZ[i-7][nIx];
		str.Format(_T("%d"), gCap.nCosmeticCnt_MZ[i-7][nIx]); 
		m_grdData.Set_CellFont(nDp, i, str, 10, FALSE);
		m_grdData.Set_CellText(nDp, i, str);
	}
	
	str.Format(_T("%d"), gCap.nGoodCount_Cosmetic[nIx]+ temp);
	m_grdData.Set_CellFont(nDp, 4, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 4, str);

	
	
}

void OCAPCosmeticDlg::AddModuleToCarrier(int nSlotNo, CString sType, int nJudge, CString sNGCode, CString sNGCodeVision, int& nCurrentMzIdx)
{
	CString strTemp;

	if(sType == "GOOD")
	{
		gCap.nGoodCntInCarr_Cosmetic[nSlotNo]++;		
		gCap.nTotalCntCarrier[nSlotNo]++;		
			
		strTemp.Format("Good, nCarrierGood:%d, nTotalCntCarrier:%d, nSlotNo:%d, sNGCode:%s",gCap.nGoodCntInCarr_Cosmetic[nSlotNo],gCap.nTotalCntCarrier[nSlotNo],nSlotNo, sNGCode);
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
					gCap.nDefectCntInCarr_Cosmetic[i][nSlotNo]++;
					gCap.nTotalCntCarrier[nSlotNo] ++;
					strTemp.Format("NG, nCarrierCosmeticDefect:%d, nTotalCntCarrier:%d, nSlotNo:%d,sNGCode:%s,%s", gCap.nDefectCntInCarr_Cosmetic[i][nSlotNo],gCap.nTotalCntCarrier[nSlotNo],nSlotNo, sNGCode, gCap.sCosmeticCode[i]);
					g_objLogFile.Save_TestLog(strTemp);
				}				
			}						
		}
		else if(nJudge == 7 || nJudge == 8 || nJudge == 9 || nJudge == 17 || nJudge == 18 || nJudge == 19)
		{
			for(int i = 0; i < 50; i++)
			{
				/*if(sNGCode == gCap.sFAICode[i] && sNGCode != "")
				{
					gCap.nTotalCntCarrier[nSlotNo]++;
					strTemp.Format("NG, nCarrierFAIDefect:%d, nTotalCntCarrier:%d, nSlotNo:%d,sNGCode:%s,%s", gCap.nCarrierFAIDefect[i][nSlotNo],gCap.nTotalCntCarrier[nSlotNo],nSlotNo, sNGCode, gCap.sFAICode[i]);
					g_objLogFile.Save_TestLog(strTemp);
				}*/
			}		
		}		
	}
	//Only Vision Good이어도 코드는 NG
	if(sNGCode !="")
	{
		for(int i = 0; i < 50; i++)
		{
			if(sNGCodeVision == gCap.sCosmeticCode[i])
			{
				gCap.nDefectCntInCarr_CosmeVision[i][nSlotNo]++;			
			}				
		}	
	}
}

void OCAPCosmeticDlg::AddCarToMZ(int nSlotNo, CString sType)
{
	CString strLog; 

	
	if(sType == "GOOD")
	{
		gCap.nGoodCnt_Cosmetic[gCap.nMZIdx_Cosmetic][gCap.nLotIndex_Good] = gCap.nGoodCntInCarr_Cosmetic[nSlotNo];
		gCap.nGoodCnt_MZ[gCap.nMZIdx_Cosmetic] += gCap.nGoodCntInCarr_Cosmetic[nSlotNo];
		strLog.Format("AddCarToMZ,gCap.nGoodInMZ:%d, nSlotNo: %d", gCap.nGoodCnt_Cosmetic[gCap.nLotIndex_Good],nSlotNo);
		g_objLogFile.Save_TestLog(strLog);

		gCap.nLotIndex_Good++;
	}		
	else if(sType == "NG")
	{
		for(int i = 0; i < 50; i++)
		{			
			gCap.nDefectCnt_Cosmetic[i][gCap.nMZIdx_Cosmetic][gCap.nLotIndex_NG] = gCap.nDefectCntInCarr_Cosmetic[i][nSlotNo];
			gCap.nCosmeticCnt_MZ[i][gCap.nMZIdx_Cosmetic] += gCap.nDefectCntInCarr_Cosmetic[i][nSlotNo];
			gCap.nCosmeVisionCnt_MZ[i][gCap.nMZIdx_Cosmetic] += gCap.nDefectCntInCarr_CosmeVision[i][nSlotNo];
			strLog.Format("AddCarToMZ,gCap.nCosmeticDefectMZ:%d,nSlotNo:%d", gCap.nDefectCnt_Cosmetic[i][gCap.nLotIndex_NG],nSlotNo);
			g_objLogFile.Save_TestLog(strLog);
		}	
		gCap.nLotIndex_NG++;
	}

}

void OCAPCosmeticDlg::AddMZOut(CString sMZID, CString sType)
{
	CString strLog; 
		
	if(sType == "NG") return;
	
	//ADJ OverWritten
	Check_CosmeticDefect(0, sMZID);
	Check_CosmeticDefect(1, sMZID);
	Check_CosmeticDefect(2, sMZID);

	//Only Vision 
	Check_CosmeticDefect(4, sMZID);
	Check_CosmeticDefect(5, sMZID);
	Check_CosmeticDefect(6, sMZID);
	
	if(gCap.nMZIdx_Cosmetic == 49)
	{
		gCap.nMZIdx_Cosmetic = 0;

		memset(gCap.nDefectCnt_Cosmetic, 0, sizeof(int)*50*50*8);
		memset(gCap.nGoodCnt_Cosmetic, 0, sizeof(int)*50*8);

		memset(gCap.nTotalCnt_MZ, 0, sizeof(int)*50);
		memset(gCap.nGoodCnt_Cosmetic, 0, sizeof(int)*50);
		memset(gCap.nNGCnt_MZ, 0, sizeof(int)*50);
		memset(gCap.nCosmeticCnt_MZ, 0, sizeof(int)*50*50);

		memset(gCap.nDefectCnt_CosmeVision, 0, sizeof(int)*50*50*8);
		memset(gCap.nNGVisionCnt_MZ, 0, sizeof(int)*50);
		memset(gCap.nCosmeVisionCnt_MZ, 0, sizeof(int)*50*50);

	}
	else gCap.nMZIdx_Cosmetic++;


	if(gCap.bOCAPDone[0] == TRUE || gCap.bOCAPDone[1] == TRUE || gCap.bOCAPDone[2] == TRUE)
	{
		gCap.bOCAPDone[0] = FALSE;
		gCap.bOCAPDone[1] = FALSE;
		gCap.bOCAPDone[2] = FALSE;
	}
}


void OCAPCosmeticDlg::Check_CosmeticDefect(int nType, CString sMZID)
{
	double dPer = 0;

	//Cosmetic case 1 check 
	for(int i = 0; i < 50; i++)
	{

		if(nType != 0)
		{
			if(gCap.bOCAPDone[nType-1] == TRUE) break; // 앞번호 조건이 알람 우선순위 
		}

		double dPercentLimit = 0;
		int	   nConsecutiveMZLimit = 0;	

		if(nType > 2)
		{
			dPercentLimit = gCap.dDefectPercent[nType - 3];
			nConsecutiveMZLimit = gCap.nConsecutiveMZLimit[nType -3];
		}
		else{

		}

		gCap.nNGCnt_MZ[gCap.nMZIdx_Cosmetic] = 0;
		for(int j = 0; j < 50; j++)
		{				
			if(nType > 2) gCap.nNGCnt_MZ[gCap.nMZIdx_Cosmetic] += gCap.nCosmeticCnt_MZ[j][gCap.nMZIdx_Cosmetic];
			else gCap.nNGVisionCnt_MZ[gCap.nMZIdx_Cosmetic] += gCap.nCosmeVisionCnt_MZ[j][gCap.nMZIdx_Cosmetic];
		}		
		gCap.nTotalCnt_MZ[gCap.nMZIdx_Cosmetic] = gCap.nGoodCnt_MZ[gCap.nMZIdx_Cosmetic] + gCap.nNGCnt_MZ[gCap.nMZIdx_Cosmetic];
		
		if(nType > 2) dPer = (gCap.nCosmeticCnt_MZ[i][gCap.nMZIdx_Cosmetic] * 100.0) / gCap.nTotalCnt_MZ[gCap.nMZIdx_Cosmetic];
		else dPer = (gCap.nCosmeVisionCnt_MZ[i][gCap.nMZIdx_Cosmetic] * 100.0) / gCap.nTotalCnt_MZ[gCap.nMZIdx_Cosmetic];

		// 불량율 OCAP 기준보다 낮으면 Continue
		if (dPer < dPercentLimit || _isnan(dPer))
		{			
			continue;			
		}		

		// 연속 횟수 도달시 알람 발생 
		if(gCap.nConsecutiveMZCount[nType][i] + 1 < nConsecutiveMZLimit)
		{
			gCap.nConsecutiveMZCount[nType][i]++;
			continue;
		}
		else
		{
			gCap.nConsecutiveMZCount[nType][i]++; 
		}

		gCap.sAlmMZID_Cosmetic = sMZID;
		gCap.sAlmDefectName_Cosmetic.Format("%s", gCap.sCosmeticName[i]);
		gCap.dAlmDefectPercent_Cosmetic  = dPer;
		
		if(nType > 2) gCap.nAlmNGCount_Cosmetic   = gCap.nCosmeticCnt_MZ[i][gCap.nMZIdx_Cosmetic];
		else gCap.nAlmNGCount_Cosmetic   = gCap.nCosmeVisionCnt_MZ[i][gCap.nMZIdx_Cosmetic];

		if(gCap.nMZCycle_Cosmetic == 50)  gCap.nMZCycle_Cosmetic = 1;
		else gCap.nMZCycle_Cosmetic++;

		SYSTEMTIME time;
		GetLocalTime(&time);

		m_strLog.Format("%04d/%02d/%02d", time.wYear, time.wMonth, time.wDay);
		gCap.sDate_Cosmetic[gCap.nMZCycle_Cosmetic-1] = m_strLog;

		m_strLog.Format("%02d:%02d:%02d.%03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
		gCap.sTime_Cosmetic[gCap.nMZCycle_Cosmetic-1] = m_strLog;

		gCap.sMZID_Cosmetic[gCap.nMZCycle_Cosmetic - 1] = sMZID;
		gCap.nTotCount_Cosmetic[gCap.nMZCycle_Cosmetic - 1] = gCap.nTotalCnt_MZ[gCap.nMZIdx_Cosmetic];
		gCap.nGoodCount_Cosmetic[gCap.nMZCycle_Cosmetic - 1] = gCap.nGoodCnt_MZ[gCap.nMZIdx_Cosmetic];
		
		if(nType > 2) gCap.nNGCount_Consmetic[gCap.nMZCycle_Cosmetic - 1] = gCap.nCosmeticCnt_MZ[i][gCap.nMZIdx_Cosmetic];
		else gCap.nNGCount_Consmetic[gCap.nMZCycle_Cosmetic - 1] = gCap.nCosmeVisionCnt_MZ[i][gCap.nMZIdx_Cosmetic];
		
		gCap.nCosmeticCodeNum[nType] = i+1;

		Display_Status();		

		if(gCap.nTotalCnt_MZ[gCap.nMZIdx_Cosmetic] > gCap.nMinModuleCnt )
		{
			gCap.bErrorShowDone = FALSE;
			g_objCommon.Show_Error(9185 + nType, 1);
		}

		//에러창 쓰레드라서 
		int nTick = 0;
		while(!gCap.bErrorShowDone)
		{
			nTick++;
			if(nTick > 50) break;
		} 
		gCap.bErrorShowDone = FALSE;

		memset(gCap.nConsecutiveMZCount[nType],0,sizeof(int)*50); //에러율 초과 발생시 연속 발생 횟수 초기화 
		gCap.bOCAPDone[nType] = TRUE;

		//후처리 		
		break;

	}
}

void OCAPCosmeticDlg::Set_AddMZData(int nPortNo)
{

}


void OCAPCosmeticDlg::DelMZData(int nMZNo)
{

}



void OCAPCosmeticDlg::Set_AddDEFECT(CString sMZid,int nPortNo, int nInfo)
{

}

void OCAPCosmeticDlg::Check_DEFECTF(int nNo)
{
}


void OCAPCosmeticDlg::OnBnClickedBtnTest()
{

	gCap.nMZCycle_Cosmetic = 4;
	gCap.sDate_Cosmetic[0] = "1/1";	
	gCap.sDate_Cosmetic[1] = "1/1";	
	gCap.sDate_Cosmetic[2] = "1/1";	
	gCap.sDate_Cosmetic[3] = "1/1";	


	gCap.sTime_Cosmetic[0] = "01:01";			
	gCap.sMZID_Cosmetic[0] = "AAAAAAAAAA";			
	gCap.nTotCount_Cosmetic[0] = 320;		
	gCap.nGoodCount_Cosmetic[0] = 2;		
	gCap.nNGCount_Consmetic[0] = 150;

	//gCap.nCosmeticDefectMZ[0][0] = 15;
	//gCap.nCosmeticDefectMZ[0][1] = 16;
	//gCap.nCosmeticDefectMZ[0][2] = 17;
	//gCap.nCosmeticDefectMZ[0][3] = 18;


	Display_Status();

	
}


