// OCAPProcess.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMAI2100.h"
#include "OCAPProcess.h"
#include "Common.h"
#include "LogFile.h"
#include "DataManager.h"
#include "afxdialogex.h"


// OCAPProcess 대화 상자입니다.
OCAPProcess g_dlgOCAP;

IMPLEMENT_DYNAMIC(OCAPProcess, CDialogEx)

OCAPProcess::OCAPProcess(CWnd* pParent /*=NULL*/)
	: CDialogEx(OCAPProcess::IDD, pParent)
{

}

OCAPProcess::~OCAPProcess()
{
}

void OCAPProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 7; i++) DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);
	for (int i = 0; i < 9; i++) DDX_Control(pDX, IDC_STC_OCAP_VALUE_0 + i, m_stcOption[i]);
	DDX_Control(pDX, IDC_GRD_OCAP_DATA, m_grdData);
}


BEGIN_MESSAGE_MAP(OCAPProcess, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDCANCEL, &OCAPProcess::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK,		&OCAPProcess::OnBnClickedOk)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_OCAP_VALUE_0, IDC_STC_OCAP_VALUE_8, OnStcOptionClick)
END_MESSAGE_MAP()


BOOL OCAPProcess::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 75, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	Initial_Controls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL OCAPProcess::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void OCAPProcess::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow) {
		Display_Option();
		Initial_NameGrid(&m_grdData, 50, 96);
		Display_Status();
	} else {
	}
}

// OCAPProcess 메시지 처리기입니다.

void OCAPProcess::OnBnClickedCancel()
{
	ShowWindow(SW_HIDE);
}


void OCAPProcess::OnBnClickedOk()
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

void OCAPProcess::Initial_Controls() 
{
	for (int i = 0; i < 2; i++) m_Group[i].Init_Ctrl("바탕", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 6; i++) m_Label[i].Init_Ctrl("바탕", 12, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x00, 0x80));
	m_Label[0].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	m_Label[4].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	m_Label[5].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	m_Label[6].Init_Ctrl("바탕", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0x99, 0x99, 0x00));
	for (int i = 0; i < 9; i++) m_stcOption[i].Init_Ctrl("바탕", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
}

void OCAPProcess::Display_Option()
{
	CString strData;
	
}

void OCAPProcess::OnStcOptionClick(UINT nID)
{
	
}

void OCAPProcess::Initial_NameGrid(CGridCS *pGrid, int nRows, int nCols)
{
	
}

void OCAPProcess::Initial_DataGrid(CGridCS *pGrid, int nRows, int nCols)
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

void OCAPProcess::Display_Status()
{
}

void OCAPProcess::Display_Grid(int nDp, int nIx)
{
	
}


void OCAPProcess::AddCarToMZ(int nSlotNo, CString sType)
{
	if(nSlotNo <= 8 && nSlotNo > 0)  gCap.nCurrentMZIdx = 0;
	else if(nSlotNo > 8 && nSlotNo <= 16) gCap.nCurrentMZIdx = 1;
	else if(nSlotNo > 16 && nSlotNo <= 24) gCap.nCurrentMZIdx = 2;
	else if(nSlotNo > 24 && nSlotNo <= 30) gCap.nCurrentMZIdx = 3;

	CString strLog; 
	strLog.Format("nSlotNo: %d  CurrentMZ index:%d", nSlotNo, gCap.nCurrentMZIdx);
	g_objLogFile.Save_TestLog(strLog);
	
	if(sType == "GOOD") gCap.nTotalCntMZ[gCap.nCurrentMZIdx] += gCap.nGoodCnt[nSlotNo];
	else	gCap.nTotalCntMZ[gCap.nCurrentMZIdx] += gCap.nDefectCnt[nSlotNo];

}

void OCAPProcess::AddMZOut(CString sMZid)
{

}


void OCAPProcess::Set_AddMZData(int nPortNo)
{
	
}


void OCAPProcess::DelMZData(int nMZNo)
{
	
}



void OCAPProcess::Set_AddDEFECT(CString sMZid,int nPortNo, int nInfo)
{
	
}

void OCAPProcess::Check_DEFECT(CString sMZID)
{
	
}

void OCAPProcess::Check_DEFECTF(int nNo)
{
}
