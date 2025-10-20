// OCAPSelectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CMAI2100.h"
#include "OCAPSelectDlg.h"
#include "afxdialogex.h"
#include "OCAPCosmeticDlg.h"
#include "OCAPProcess.h"


// COCAPSelectDlg 대화 상자입니다.

COCAPSelectDlg g_dlgOCAPSelect;

IMPLEMENT_DYNAMIC(COCAPSelectDlg, CDialogEx)

COCAPSelectDlg::COCAPSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COCAPSelectDlg::IDD, pParent)
{

}

COCAPSelectDlg::~COCAPSelectDlg()
{
}

void COCAPSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_COSMETIC, m_btnCosmeticOCAP);
	DDX_Control(pDX, IDC_BTN_FAI, m_btnFAIOCAP);
}


BEGIN_MESSAGE_MAP(COCAPSelectDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_COSMETIC, &COCAPSelectDlg::OnBnClickedBtnCosmetic)
	ON_BN_CLICKED(IDC_BTN_FAI, &COCAPSelectDlg::OnBnClickedBtnFai)
END_MESSAGE_MAP()


// COCAPSelectDlg 메시지 처리기입니다.


BOOL COCAPSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_dlgOCAPCosmetic.Create(OCAPCosmeticDlg::IDD, this);
	g_dlgOCAP.Create(OCAPProcess::IDD, this);

	m_btnCosmeticOCAP.Init_Ctrl("Arial", 10, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0x00), 0, 0);
	m_btnFAIOCAP.Init_Ctrl("Arial", 10, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0x00), 0, 0);
	return TRUE;  
	
}


BOOL COCAPSelectDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;


	return CDialogEx::PreTranslateMessage(pMsg);
}



void COCAPSelectDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	g_dlgOCAPCosmetic.DestroyWindow();
	g_dlgOCAP.DestroyWindow();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void COCAPSelectDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	SetWindowPos(this, 785, 350, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void COCAPSelectDlg::OnBnClickedBtnCosmetic()
{
	if (g_dlgOCAPCosmetic.IsWindowVisible()) g_dlgOCAPCosmetic.ShowWindow(SW_HIDE);
	else g_dlgOCAPCosmetic.ShowWindow(SW_SHOW);	

	this->ShowWindow(SW_HIDE);
}


void COCAPSelectDlg::OnBnClickedBtnFai()
{
	if (g_dlgOCAP.IsWindowVisible()) g_dlgOCAP.ShowWindow(SW_HIDE);
	else g_dlgOCAP.ShowWindow(SW_SHOW);	

	this->ShowWindow(SW_HIDE);
}



BOOL COCAPSelectDlg::DestroyWindow()
{
	return CDialogEx::DestroyWindow();
}
