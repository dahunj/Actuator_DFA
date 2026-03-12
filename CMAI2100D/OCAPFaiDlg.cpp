// OCAPFaiDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMAI2100.h"
#include "OCAPFaiDlg.h"
#include "Common.h"
#include "LogFile.h"
#include "DataManager.h"
#include "afxdialogex.h"


// OCAPFaiDlg 대화 상자입니다.
OCAPFaiDlg g_dlgOCAPFai;

IMPLEMENT_DYNAMIC(OCAPFaiDlg, CDialogEx)

OCAPFaiDlg::OCAPFaiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(OCAPFaiDlg::IDD, pParent)
{

}

OCAPFaiDlg::~OCAPFaiDlg()
{
}

void OCAPFaiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 9; i++) DDX_Control(pDX, IDC_STC_OCAP_VALUE_0 + i, m_stcOption[i]);
	DDX_Control(pDX, IDC_GRD_OCAP_DATA, m_grdData);
}


BEGIN_MESSAGE_MAP(OCAPFaiDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDCANCEL, &OCAPFaiDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK,		&OCAPFaiDlg::OnBnClickedOk)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_OCAP_VALUE_0, IDC_STC_OCAP_VALUE_8, OnStcOptionClick)
END_MESSAGE_MAP()


BOOL OCAPFaiDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL OCAPFaiDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void OCAPFaiDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		Display_Option();
		Initial_NameGrid(&m_grdData, 50, 96);
		Display_Status();
	} else {
	}
}

// OCAPFaiDlg 메시지 처리기입니다.

void OCAPFaiDlg::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}


void OCAPFaiDlg::OnBnClickedOk()
{
	int		nData1, nData2, nData3, nData4, nData5;
	double	dData1, dData2, dData3, dData4;
	CString strData, strLog;

	CIniFileCS INI(gsCurrentDir + "\\System\\OCAPData.ini");
	if (!INI.Check_File()) {
		AfxMessageBox("OCAPData.ini File Not Found!");
		return;
	}

	m_stcOption[0].GetWindowText(strData); nData1 = atoi(strData); INI.Set_Integer("OPTION", "MZ_COUNT_01", nData1);
	m_stcOption[1].GetWindowText(strData); dData1 = atof(strData); INI.Set_Double("OPTION", "DEFECTP_01", dData1, "%0.3lf");
	m_stcOption[3].GetWindowText(strData); nData2 = atoi(strData); INI.Set_Integer("OPTION", "MZ_COUNT_02", nData2);
	m_stcOption[4].GetWindowText(strData); dData2 = atof(strData); INI.Set_Double("OPTION", "DEFECTP_02", dData2, "%0.3lf");
	m_stcOption[5].GetWindowText(strData); nData3 = atoi(strData); INI.Set_Integer("OPTION", "MZ_COUNT_03", nData3);
	m_stcOption[6].GetWindowText(strData); dData3 = atof(strData); INI.Set_Double("OPTION", "DEFECTP_03", dData3, "%0.3lf");
	m_stcOption[7].GetWindowText(strData); nData4 = atoi(strData); INI.Set_Integer("OPTION", "MZ_COUNT_04", nData4);
	m_stcOption[8].GetWindowText(strData); dData4 = atof(strData); INI.Set_Double("OPTION", "DEFECTP_04", dData4, "%0.3lf");
	m_stcOption[2].GetWindowText(strData); nData5 = atoi(strData); INI.Set_Integer("OPTION", "MZ_MIN_COUNT", nData5);

	strLog.Format("[OCAP Option] OnBnClickedOk - Data1(%d,%0.3lf) Data2(%d,%0.3lf) Data3(%d,%0.3lf)  Data4(%d,%0.3lf) Skip(%d)", nData1, dData1, nData2, dData2, nData3, dData3, nData4, dData4, nData5);
	g_objLogFile.Save_HandlerLog(strLog);

	g_objDataManager.Read_OCAPData();
	Display_Option();
	Display_Status();
}

void OCAPFaiDlg::Initial_Controls() 
{
	for (int i = 0; i < 2; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 6; i++) m_Label[i].Init_Ctrl("바탕", 12, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x00, 0x80));
	m_Label[0].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	m_Label[4].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	m_Label[5].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	m_Label[6].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	for (int i = 0; i < 9; i++) m_stcOption[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
}

void OCAPFaiDlg::Display_Option()
{
	CString strData;
	strData.Format("%d", gCap.nGiMZCnt[0]);	     m_stcOption[0].SetWindowText(strData);
	strData.Format("%0.3lf", gCap.dGiDefect[0]); m_stcOption[1].SetWindowText(strData);
	strData.Format("%d", gCap.nGiMZCnt[1]);	     m_stcOption[3].SetWindowText(strData);
	strData.Format("%0.3lf", gCap.dGiDefect[1]); m_stcOption[4].SetWindowText(strData);
	strData.Format("%d", gCap.nGiMZCnt[2]);	     m_stcOption[5].SetWindowText(strData);
	strData.Format("%0.3lf", gCap.dGiDefect[2]); m_stcOption[6].SetWindowText(strData);
	strData.Format("%d", gCap.nGiMZCnt[3]);	     m_stcOption[7].SetWindowText(strData);
	strData.Format("%0.3lf", gCap.dGiDefect[3]); m_stcOption[8].SetWindowText(strData);
	strData.Format("%d", gCap.nGiMinCnt);	     m_stcOption[2].SetWindowText(strData);
}

void OCAPFaiDlg::OnStcOptionClick(UINT nID)
{
	int		nData = 0;
	double	dData = 0.0;
	int ID = nID - IDC_STC_OCAP_VALUE_0;

	CString strOld, strNew, strLog;
	m_stcOption[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	if (ID == 0 || ID == 3 || ID == 5 || ID == 7) { nData = atoi(strNew); if (nData < 0 || nData > 5) { AfxMessageBox("연속 매거진 수량 Error.."); return; } }
	if (ID == 1 || ID == 4 || ID == 6 || ID == 8) { dData  = atof(strNew); if (dData <= 0.0 || dData >= 100.0) { AfxMessageBox("기준 불량율 Error.."); return; } }
	if (ID == 2) { nData = atoi(strNew); if (nData <= 0 || nData > 300) { AfxMessageBox("매거진 최소수량 Error.."); return; } }

	m_stcOption[ID].SetWindowText(strNew);
}

void OCAPFaiDlg::Initial_NameGrid(CGridCS *pGrid, int nRows, int nCols)
{
	pGrid->Set_RowCount(nRows);
	pGrid->Set_ColCount(nCols);
	pGrid->Set_FixRowCount(0);
	pGrid->Set_FixColCount(0);

	long lTotalW = pGrid->Get_GridWidth();
	long lTotalH = pGrid->Get_GridHeight();
	long lCellW = lTotalW / nCols;
	long lCellH = (lTotalH / nRows) + 2;
	long  lWidth[16]= { 60,    60,      100,          60,     60,      60,    50,   50,    50,    50,    50,    50,    50,     50,    50,     50};
	CString sTxt[16]= {"날짜","시간","배출배거진ID", "투입", "완공", "양품", "RN", "RR", "치명", "BCR", "MES", "MC", "치수", "7FAI", "Tilt", "Gap"};
	long  lWidthName[2]= { 40, 50 };

	for (int i=0; i<nRows; i++) {
		pGrid->Set_RowHeight(i, lCellH);
		for (int j=0; j<16; j++) {
			if(i==0) {
				pGrid->Set_ColWidth(j, lWidth[j]); pGrid->Set_CellText(i, j, sTxt[j]);
				pGrid->Set_CellBackClr(i, j, RGB(0xCC, 0xCC, 0xCC));
			}
		}

		int nW = 0, nName=0; 
		for (int j=16; j<56; j++) {
			if(i==0) {
				pGrid->Set_ColWidth(j, lWidthName[nW]);
				if (nW == 0) pGrid->Set_CellText(i, j, "FAI");
				else		 pGrid->Set_CellText(i, j, gCap.sFAIName[nName]);
				nW++; if (nW == 2) { nW = 0; nName++; }
				if (j>=16 && j<=29) pGrid->Set_CellBackClr(i, j, RGB(0xFF, 0xFF, 0xCC));
				if (j>=30 && j<=45) pGrid->Set_CellBackClr(i, j, RGB(0x99, 0xFF, 0x99));
				if (j>=46 && j<=55) pGrid->Set_CellBackClr(i, j, RGB(0x33, 0xFF, 0x33));
			}
		}
		nW = 0; nName=0; 
		for (int j=56; j<96; j++) {
			if(i==0) {
				pGrid->Set_ColWidth(j, lWidthName[nW]);
				if (nW == 0) pGrid->Set_CellText(i, j, "ERR");
				else		 pGrid->Set_CellText(i, j, gCap.sFAIName[nName]);
				nW++; if (nW == 2) { nW = 0; nName++; }
				if (j>=56 && j<=69) pGrid->Set_CellBackClr(i, j, RGB(0xCC, 0xCC, 0xCC));
				if (j>=70 && j<=85) pGrid->Set_CellBackClr(i, j, RGB(0x99, 0x99, 0x99));
				if (j>=86 && j<=95) pGrid->Set_CellBackClr(i, j, RGB(0x66, 0x66, 0x66));
			}
		}
	}
}

void OCAPFaiDlg::Initial_DataGrid(CGridCS *pGrid, int nRows, int nCols)
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

void OCAPFaiDlg::Display_Status()
{/*
	gCap.nMZCycle = 1;
	gCap.sDate[0] = "1/1";			//날자(07/31)
	gCap.sTime[0] = "01:01";			//시간(07:31)
	gCap.sMZID[0] = "AAAAAAAAAA";			//양품MZ
	gCap.nTotCount[0] = 320;		//투입-Module수
	gCap.nGoodCount[0] = 2;		//양품수
	gCap.nROSNGCount[0] = 3;	//ROS-NG수
	gCap.nROSRfCount[0] = 4;	//ROS-Repaier수
	gCap.nBCRCount[0] = 5;		//Barcode[NG수
	gCap.nMESCount[0] = 6;		//MES[NG수
	gCap.nMCCount[0] = 7;		//MC[NG수
	for (int i=0; i<20; i++) {
		gCap.nCount[0][i] = i+1;		//MZ,FAI별 발생수
		gCap.nFCount[0][i] = i+1;		//MZ,FAI별 발생수
	}
*/
	if (gCap.nMZCycle < 0 || gCap.nMZCycle > 49) return;

	int nD = 0;
	int nS = gCap.nMZCycle;
	if (nS < 0) nS = 49;
	for(int i=nS-1; i >= 0; i--) {
		if (gCap.sDate[i].GetLength() < 1) break;
		nD++;
		Display_Grid(nD, i);
	}
	for(int i=49; i>=0; i--) {
		if (gCap.sDate[i].GetLength() < 1) break;
		nD++;
		Display_Grid(nD, i);
		if (nD >= 51) return;
	}
}

void OCAPFaiDlg::Display_Grid(int nDp, int nIx)
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
	str.Format(_T("%d"), gCap.nGoodCount[nIx] + gCap.nROSRfCount[nIx]);
	m_grdData.Set_CellFont(nDp, 4, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 4, str);

	str.Format(_T("%d"), gCap.nGoodCount[nIx]);
	m_grdData.Set_CellFont(nDp, 5, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 5, str);

	str.Format(_T("%d"), gCap.nROSNGCount[nIx]);
	m_grdData.Set_CellFont(nDp, 6, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 6, str);

	str.Format(_T("%d"), gCap.nROSRfCount[nIx]);
	m_grdData.Set_CellFont(nDp, 7, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 7, str);

	str.Format(_T("%d"), gCap.nBCRCount[nIx] + gCap.nMESCount[nIx] + gCap.nMCCount[nIx]);
	m_grdData.Set_CellFont(nDp, 8, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 8, str);

	str.Format(_T("%d"), gCap.nBCRCount[nIx]);
	m_grdData.Set_CellFont(nDp, 9, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 9, str);

	str.Format(_T("%d"), gCap.nMESCount[nIx]);
	m_grdData.Set_CellFont(nDp, 10, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 10, str);

	str.Format(_T("%d"), gCap.nMCCount[nIx]);
	m_grdData.Set_CellFont(nDp, 11, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 11, str);

	int nA1 = gCap.nFAITotCount[nIx][0] + gCap.nFAITotCount[nIx][1] + gCap.nFAITotCount[nIx][2] + gCap.nFAITotCount[nIx][3] + gCap.nFAITotCount[nIx][4] + gCap.nFAITotCount[nIx][5] + gCap.nFAITotCount[nIx][6];
	int nA2 = gCap.nFAITotCount[nIx][7] + gCap.nFAITotCount[nIx][8] + gCap.nFAITotCount[nIx][9] + gCap.nFAITotCount[nIx][10] + gCap.nFAITotCount[nIx][11] + gCap.nFAITotCount[nIx][12] + gCap.nFAITotCount[nIx][13] + gCap.nFAITotCount[nIx][14];
	int nA3 = gCap.nFAITotCount[nIx][15] + gCap.nFAITotCount[nIx][16] + gCap.nFAITotCount[nIx][17] + gCap.nFAITotCount[nIx][18] + gCap.nFAITotCount[nIx][19];
	str.Format(_T("%d"), nA1 + nA2 + nA3);
	m_grdData.Set_CellFont(nDp, 12, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 12, str);

	str.Format(_T("%d"), nA1);
	m_grdData.Set_CellFont(nDp, 13, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 13, str);

	str.Format(_T("%d"), nA2);
	m_grdData.Set_CellFont(nDp, 14, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 14, str);

	str.Format(_T("%d"), nA3);
	m_grdData.Set_CellFont(nDp, 15, str, 10, FALSE);
	m_grdData.Set_CellText(nDp, 15, str);

	double dValue;
	int	   nNo = 16;
	for(int i=0; i<20; i++) {
		str.Format(_T("%d"), gCap.nFAITotCount[nIx][i]);
		m_grdData.Set_CellFont(nDp, nNo, str, 10, FALSE);
		m_grdData.Set_CellText(nDp, nNo, str);
		nNo++;

		dValue = (gCap.nFAITotCount[nIx][i] * 100.0) / gCap.nTotCount[nIx];
		str.Format(_T("%0.1f%%"), dValue);
		m_grdData.Set_CellFont(nDp, nNo, str, 10, FALSE);
		m_grdData.Set_CellText(nDp, nNo, str);
		if		(gCap.dGiDefect[2] < dValue)	m_grdData.Set_CellBackClr(nDp, nNo, RGB(0xFF, 0x00, 0x00));	// 3 NG
		else if	(gCap.dGiDefect[1] < dValue)	m_grdData.Set_CellBackClr(nDp, nNo, RGB(0x00, 0x00, 0xFF));	// 2 NG
		else if	(gCap.dGiDefect[0] < dValue)	m_grdData.Set_CellBackClr(nDp, nNo, RGB(0x00, 0xFF, 0x00));	// 1 NG
		else									m_grdData.Set_CellBackClr(nDp, nNo, RGB(0xFF, 0xFF, 0xFF));	// 0 Empty

		if (gCap.nTotCount[nIx] < gCap.nGiMinCnt) m_grdData.Set_CellBackClr(nDp, nNo, RGB(0xFF, 0xFF, 0xFF));	// 0 Empty
		nNo++;
	}
	nNo = 56;
	for(int i=0; i<20; i++) 
	{
		str.Format(_T("%d"), gCap.nFCount[nIx][i]);
		m_grdData.Set_CellFont(nDp, nNo, str, 10, FALSE);
		m_grdData.Set_CellText(nDp, nNo, str);
		nNo++;

		dValue = (gCap.nFCount[nIx][i] * 100.0) / gCap.nTotCount[nIx];
		str.Format(_T("%0.1f%%"), dValue);
		m_grdData.Set_CellFont(nDp, nNo, str, 10, FALSE);
		m_grdData.Set_CellText(nDp, nNo, str);
		if		(gCap.dGiDefect[3] < dValue)	m_grdData.Set_CellBackClr(nDp, nNo, RGB(0xFF, 0x00, 0x00));	// 3 NG
		else									m_grdData.Set_CellBackClr(nDp, nNo, RGB(0xFF, 0xFF, 0xFF));	// 0 Empty
		if (gCap.nTotCount[nIx] < gCap.nGiMinCnt) m_grdData.Set_CellBackClr(nDp, nNo, RGB(0xFF, 0xFF, 0xFF));	// 0 Empty
		nNo++;
	}
}






void OCAPFaiDlg::Check_DEFECT(int nNo)
{
	int nX = nNo - 1;
	if (gCap.nGiMZCnt[nX] <= 0 || gCap.nGiMZCnt[nX] > 40) return;

	int nDefCount;
	double dPer = 0.0;
	CString sDefect = "";

	for (int i=0; i<20; i++) {
		if (gCap.sFAIName[i].GetLength() < 1) continue;

		nDefCount = 0;
		int nMZno = gCap.nMZCycle;	if (nMZno < 0) nMZno = 49;
		for (int j=0; j<gCap.nGiMZCnt[nX]; j++) {
			if (gCap.nTotCount[nMZno] < gCap.nGiMinCnt) continue;

			dPer = (gCap.nFAITotCount[nMZno][i] * 100.0) / gCap.nTotCount[nMZno];
			if (dPer < gCap.dGiDefect[nX]) break;

			nDefCount++;
			if (nDefCount == 1) {
				gCap.sAlmMZID = gCap.sMZID[nMZno];
				gCap.sAlmFAIName.Format("FAI-%s", gCap.sFAIName[i]);
				gCap.dAlmDefect  = dPer;
				gCap.nAlmCount   = nDefCount;
			}
			nMZno--;
			if (nMZno < 0) nMZno = 49;
		}
		if (nDefCount >= gCap.nGiMZCnt[nX]) {
			sDefect = sDefect + gCap.sAlmFAIName + ". ";
		}
	}

	if (sDefect.GetLength() > 1) 
	{
		m_bError = TRUE;

		gCap.nMZCycle++; if(gCap.nMZCycle > 49) gCap.nMZCycle = 0;

		gAlm.sAlmLotID[0] = sDefect;
		g_objCommon.Show_Error(9180 + nX+1);
		return;
	}
}

void OCAPFaiDlg::Check_DEFECTF(int nNo)
{
	int nX = nNo - 1;
	if (gCap.nGiMZCnt[nX] <= 0 || gCap.nGiMZCnt[nX] > 40) return;

	int nDefCount;
	double dPer = 0.0;
	CString sDefect = "";

	for (int i=0; i<20; i++) {
		if (gCap.sFAIName[i].GetLength() < 1) continue;

		nDefCount = 0;
		int nMZno = gCap.nMZCycle;	if (nMZno < 0) nMZno = 49;
		for (int j=0; j<gCap.nGiMZCnt[nX]; j++) {
			if (gCap.nTotCount[nMZno] < gCap.nGiMinCnt) continue;

			dPer = (gCap.nFAITotCount[nMZno][i] * 100.0) / gCap.nTotCount[nMZno];
			if (dPer < gCap.dGiDefect[nX]) break;

			nDefCount++;
			if (nDefCount == 1) {
				gCap.sAlmMZID = gCap.sMZID[nMZno];
				gCap.sAlmFAIName.Format("ERR-%s", gCap.sFAIName[i]);
				gCap.dAlmDefect  = dPer;
				gCap.nAlmCount   = nDefCount;
			}
			nMZno--;
			if (nMZno < 0) nMZno = 49;
		}
		if (nDefCount >= gCap.nGiMZCnt[nX]) {
			sDefect = sDefect + gCap.sAlmFAIName + ". ";
		}
	}

	if (sDefect.GetLength() > 1) 
	{
		m_bError = TRUE;
		gCap.nMZCycle++; if(gCap.nMZCycle > 49) gCap.nMZCycle = 0;

		gAlm.sAlmLotID[0] = sDefect;
		g_objCommon.Show_Error(9184);
		return;
	}
}



void OCAPFaiDlg::AddModuleToCarrier(CString sType, int nPNo, CString sCode)
{
	if (gCap.nMZCycle < 0 || gCap.nMZCycle > 49) return;	

	if(sType == "Good") 
	{
		//NG 없으면 Good
		gCap.nGoodCount[nPNo]++; return;
		
	}
	
	for(int j=0; j<20; j++) 
	{
		if (sCode.Find(gCap.sFAICode[j]) > -1 ) //if (sCode == gCap.sFAICode[j]) 			 
		{
			gCap.nFAICount[nPNo][j]++;
			return;
		}
	}
	gCap.nNGCount[nPNo]++;

	return;

}



void OCAPFaiDlg::AddCarrierToMZ(CString sType, CString sToMZID, int nPortNo)
{
	if(sType == "Good")
	{
		gCap.nCarrNo.push_back(nPortNo);
		gCap.sMZID[nPortNo] = sToMZID;		
		gCap.nTotCount[gCap.nMZCycle] += gCap.nGoodCount[nPortNo];
		gCap.nGoodTotCount[gCap.nMZCycle]+= gCap.nGoodCount[nPortNo];
	}
	else
	{		
		for (size_t i = 0; i < gCap.nCarrNo.size(); ++i)
		{
			for(int j=0; j< 20; j++) 
			{		
				gCap.nTotCount[gCap.nMZCycle] += gCap.nFAICount[gCap.nCarrNo[i]][j];
				gCap.nFAITotCount[gCap.nMZCycle][j] += gCap.nFAICount[gCap.nCarrNo[i]][j];

				gCap.nTotCount[gCap.nMZCycle] += gCap.nNGCount[gCap.nCarrNo[i]];				
			}			
		}
		gCap.nCarrNo.clear();
	}
		
}


void OCAPFaiDlg::AddMZOut(CString sMZid)
{
	CString strLog;
	if (gCap.sMZID[gCap.nMZCycle].GetLength() < 1) {
		strLog.Format("[OCAP Option] MZ Out1 - Cycle[%d] Data-MZ[%s] Read-MZ[%s]", gCap.nMZCycle, gCap.sMZID[gCap.nMZCycle], sMZid);
		g_objLogFile.Save_HandlerLog(strLog);
		return;
	}

	SYSTEMTIME time;
	GetLocalTime(&time);

	gCap.sDate[gCap.nMZCycle].Format("%02d/%02d", time.wMonth, time.wDay);
	gCap.sTime[gCap.nMZCycle].Format("%02d:%02d", time.wHour,  time.wMinute);

	if (gCap.sMZID[gCap.nMZCycle] != sMZid) {
		strLog.Format("[OCAP Option] MZ Out2 - Cycle[%d] Data-MZ[%s] Read-MZ[%s]", gCap.nMZCycle, gCap.sMZID[gCap.nMZCycle], sMZid);
		g_objLogFile.Save_HandlerLog(strLog);
	}
	
	g_objLogFile.Save_OCAPFAILog(gCap.nMZCycle);

	m_bError = FALSE;
	Check_DEFECT(3); if (m_bError) return;
	Check_DEFECT(2); if (m_bError) return;
	Check_DEFECT(1); if (m_bError) return;
	Check_DEFECTF(4); if (m_bError) return;

	gCap.nMZCycle++; if(gCap.nMZCycle > 49) gCap.nMZCycle = 0;
}
