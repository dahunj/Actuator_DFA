// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.
#include <afxdisp.h>        // MFC 자동화 클래스입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

///////////////////////////////////////////////////////////////////////////////

// Library Add
#include "LedStatic.h"
#include "CSControls.h"
#include "CSIniFile.h" 
#include "CSComPort.h"
#include "CSUdpSocket.h"
#include "CSServerSocket.h"
#include "CSClientSocket.h"
#include "CSGrid.h"
#include "CSUdpClient.h"
#ifdef _DEBUG
	#pragma comment(lib, "CSControlsD.lib")
	#pragma comment(lib, "CSIniFileD.lib")
	#pragma comment(lib, "CSComPortD.lib")
	#pragma comment(lib, "CSUdpSocketD.lib")
	#pragma comment(lib, "CSServerSocketD.lib")
	#pragma comment(lib, "CSClientSocketD.lib")
	#pragma comment(lib, "CSGridD.lib")
	#pragma comment(lib, "CSUdpClientD.lib")
#else
	#pragma comment(lib, "CSControlsR.lib")
	#pragma comment(lib, "CSIniFileR.lib")
	#pragma comment(lib, "CSComPortR.lib")
	#pragma comment(lib, "CSUdpSocketR.lib")
	#pragma comment(lib, "CSServerSocketR.lib")
	#pragma comment(lib, "CSClientSocketR.lib")
	#pragma comment(lib, "CSGridR.lib")
	#pragma comment(lib, "CSUdpClientR.lib")
#endif

//****************************************************************************
//#define AJIN_BOARD_USE
#define CARRIER_TYPE_MEM
//CARRIER_OFFSETY=0

//#define CARRIER_TYPE_VARO	//CARRIER_OFFSETY=8
//****************************************************************************

#define MES_WAITTIME	10000
#define MAX_CM			40
#define TRAY_MAX_CM		40
#define TRAY_CM_X		4
#define TRAY_CM_Y		10
#define TRAY_PITCH		6
#define ELEVATOR_UD		5.0
#define ELEVATOR_DN		4.0
#define ELEVATOR_SL		0.5

#define PICK_CNT		8

#ifdef CARRIER_TYPE_MEM
	#define MAIN_VERSION	"Vd 3.1.11m"
	#define CARRIER_OFFSETX	8.0
	#define CARRIER_OFFSETY	0.0
#else
	#define MAIN_VERSION	"Vd 3.1.11v"
	#define CARRIER_OFFSETX	8.0
	#define CARRIER_OFFSETY	8.0
#endif

#define ECM_LOG			"D:\\EVMS\\TP\\LOG\\"
#define SIM_WAITTIMES	 50		//
#define SIM_WAITTIMEM	 50		//

#define	VELOCITY_WEIGHT	 1000
//****************************************************************************
//#define RESULT_TEST		// 비전 FOB 모드에서 전체 양품 처리가 안되어 추가함.
//#define ALIGN_TEST		// Align 반복 Test.
//****************************************************************************

extern CString gsCurrentDir;	// 현재 프로젝트 폴더
typedef struct 
{
	// 검사결과 (0:Empty, 1:미검사, 2:Good, 3:NG, 4:ROS_NG, 5:Barcode, 6:MES_NG, 7:FAI치수불량)
	int		InfoAlignTray[10][4];		// (y,x) 0:Empty, 1:NG      2:Good
	int		InfoLoadTray[10][4];		// (y,x) 0:Empty, 1:Module, 2:Good, NG:3~
	int		InfoLoadPick[2][10];		//[0~7]:상태 [9]:Module시작번호 
	int		InfoVision[4][10];
	int		InfoUnloadPick[2][10];
	int		InfoNgTray[10][4];
	int		InfoGoodTray[10][4];

	CString	sMZID[10];				//0:UCV, 1:ECV, 2:Elavator, 3:MZTransfer, 4:Load, 5:Buffer, 6:NG, 7:Good, 8:Unload-Good(1F), 9:Unload-NG(2F)
	CString	sMZID_Tansfer;
	CString	sMZID_LoadStage[2];
	CString	sMZID_NGElevator[2];	//0:NG-Feeder, 1:NG-Elevator
	int		nSlotNo_LDMZ;
	int		nSlotNo_Tansfer;
	int		nSlotNo_LoadStage[2];
	int		nLDMZ_LastCarrier[2];
	CString	sCarID_LoadStage[2];
	CString	sCarID_NGTray[2];
	CString	sCarID_GoodTray[2];
	CString	sCarID_Tansfer;
	CString	sCarID_Elevator[2];		//0:NG, 1:Good

	CString	sLotID_Start;
	CString	sLotID_Tansfer[3];
	CString	sLotID_LoadStage[3];
	CString	sLotID_LoadPicker[3];
	CString	sLotID_VisionStage[5];
	CString	sLotID_UnloadPicker[3];
	CString	sLotID_NGTray[3];
	CString	sLotID_GoodTray[3];
	CString	sLotID_NGBuffer;
	CString	sLotID_UnMZ[2];			//0:NG, 1:Good
	int		nTrayNo_Tansfer[2];
	int		nTrayNo_LoadStage[2];
	int		nTrayNo_LoadPicker[2];
	int		nTrayNo_VisionStage[4];
	int		nTrayNo_UnloadPicker[2];
	int		nTrayNo_NGTray[2];
	int		nTrayNo_GoodTray[2];
	int		nTrayNo_NGBuffer;
	int		nPortNo_Tansfer[2];
	int		nPortNo_LoadStage[2];
	int		nPortNo_LoadPicker[2];
	int		nPortNo_VisionStage[5];
	int		nPortNo_UnloadPicker[2];
	int		nPortNo_NGTray[2];
	int		nPortNo_GoodTray[2];
	int		nPortNo_NGBuffer;
	int		nPortNo_UnMZ[2];		//0:NG, 1:Good
	int		nVisionPos[2][5];		//0:Down, 1:Up
	int		nMZDoorOpen[2];			//0:Load, 1:Unload
	int		nLastCar_LoadStage[2];

	int		nUserLevel;				// OP , Engineer
	CString strUserName;			// LogIn 
	CString	sOperID;				// Operator
	CString	sRecipeName;			// Recipe Item
	BOOL	bCycleStop;				// Run 중간에 멈출때 (초기화 필요)
	BOOL	bUseDryRun;
	BOOL	bAlignScanDone;
	BOOL	bBTMScanDone;
	BOOL	bTop1ScanDone;
	BOOL	bTop2ScanDone;
	BOOL	bAlignA1Done;
	CString sAlarmTime[3];
	CString sAlarmList[3];
	int		m_nMS;
	BOOL	m_bTowerOn;
	BOOL	m_bBuzzerOff;
	BOOL	bDoorLock;
	BOOL	bAlarmShow;
	BOOL	bJahwa;
	BOOL	bDoorOpen;

	int		nTransferXLock;			//1:Lock
	int		nTransferX1Pos;			//1(LS1),2(LS2),3(L1),4(L2),5(EN),6(EG),7(NB),8(U1),9(U2),10(NG1),11(NG2),12(Good1),13(Good2)
	int		nTransferX2Pos;			//1(LS1),2(LS2),3(L1),4(L2),5(EN),6(EG),7(NB),8(U1),9(U2),10(NG1),11(NG2),12(Good1),13(Good2)
	int		nElevatorOpen[10];		//0:Stop,1:Open,2:Opened //1(L1),2(L2),3(EN),4(EG),5(NB),6(U1),7(U2)
	int		nUnloadPick1OpenCnt;
	int		nUnloadPick2OpenCnt;
	int		nUnloadPick1DownCnt;
	int		nUnloadPick2DownCnt;
	int		nTop1Cnt;
	int		nTop2Cnt;

	int		nLoadPicker1ErrNo[8];
	int		nLoadPicker2ErrNo[8];
	int		nVisionVacErrNo[8];
	int		nUnloadPicker1ErrNo[8];
	int		nUnloadPicker2ErrNo[8];
	int		nVisionStageNo;

	int		nDay_TotalCnt;
	int		nDay_NGCnt;
	int		nDay_GoodCnt;

	int		nSelectNo;
	CString sListName;
	CString sYYMMDD;

	int		nLanguage;			//0:Kor, 1:Eng
	CString	sComName;
	int		nRejectMaxCount;
	int		nRejectLotCount;
	int		nLoadStageSeqNo[4];

	//A2 3Point Align Data
	BOOL	bAlignShipDone;
	double	dAlignVisionX[4][3];
	double	dAlignVisionY[4][3];
	int		nLPCount1;
	int		nLPCount2;
	double	dAlignX;	//편차의기준값
	double	dAlignY;	//편차의기준값

	int		nSlideTime;		//Sec
	DWORD	dwSlideTime[7];
	int		nDoorLockTime;	//Min
	DWORD	dwDoorStartTime;

	BOOL	bInpectLotEnd;
	BOOL	bResultTest;	// 비전FOB 모드로 전체양품이 안되어 반복성을 돌릴수 없다.
	double	dElevatorDown;
	int		nVsNo[2];
	int		nVsIx[2];
	CString sOffsetInfor[3];
	double	dFocusOffset[4][8];	//Top1:1~4, Top2:5~8

	int		nVisionNo;
	double	dElevatorZPos[2][7];
	int		nCVElevatorFrom;		//1:Good, 2:Empty
	int		nCVJobSeq[2];
	int		nSimCount;
	int		nCarrierStart;		//0:End,1:Start
	CString sReadyRecipe;
	CString sGoodCarrierID;
	int		nCarrierCount;
	int		nROSReqCount;
	int		nNGMgzOut;
	int		nMZSlotNo[5];
	CString sLastLotID;
	int		nLastPorNo;
	int		nMZLastCar;
	int		nLoadAlignRetry;
	int		nScanRetry[3];		//0:BTM, 1:T1, 2:T2
	double	dAlignOffset[2][2];	//0:X, 1:Y
	int		nNG_MC[5][2];	//0:MC, 1:GF, 2:Barcode, 3:Vision-timover, 4:MES_NG

	int		nSimMzCntLoaded;

	int		nPosXFull[10];
	int		nPosYFull[10];

} GLOVAL_DATA;

typedef struct {
	int		nCarrierExist[3][8];	//0:None, 1:Exist

	int		nJobCycle;
	int		nJobStatus;		//0:Idle, 1:Run, 2:End
	int		nLotStatus[30];	//0:Wait, 1, 2:Run, 3:Lot_End
	CString sMZID_LD[30];
	CString sMZID_NG[30];
	CString sMZID_GD[30];
	CString sCarID_LD[30];
	CString sCarID_NG[30];
	CString sCarID_GD[30];
	CString	sLotID[30];
	CString	sRecipeName[30];
	int		nMZLastCar[30];		//1:Last Carrier
	int		nSlotNo_LD[30];
	int		nSlotNo_NG[30];
	int		nSlotNo_GD[30];
	int		nCmCount[30];
	BOOL	bEmptyTray[30];		//NG-MZ
	int		nOutTrayCnt[30][5];	//0:Good, 1:NG
	int		nTrayPutCnt[30][5];	//0:Good, 1:NG, 2:Load
	int		nGoodCount[30];
	int		nNgCount[30];
	int		nSkipCount[30];
	int		nJudge_I[30][10][40][6];	//2:G, 3:NG,     5:BAR_NOREAD, 6:MES_NG, 7:FAI치수불량
	int		nJudge_R[30][10][40];		//2:G, 3:ROS_NG, 4:ROS_Repair
	CString sNGCode_I[30][10][40][6];
	CString sNGCode_Vision[30][10][40][6];
//	CString sNGCode_R[30][10][40];
	CString sBarCode[30][10][40];
	int		nRosJugCount[30][15];		//0:Count, 1:Good, 2:TOver 3:NG, 4:Repair, 5:Bar_NoRead,MES-NG,MC, 6:FAI_NG(치수불량), 7:Bar, 8:MES-NG, 9:MC, 10:Ros Skip, 11:허수불량, 12:불량차감, 13FAI_RECOFAIL_SP
	int		nHistory[30][10][40][10];
	int		nImageCnt[30][10][40][2];	//0:외관불량수, 1:치수불량수
	int		nOcapCount[30][20];			//MZ,FAI별 발생수
	int		nFOcapCount[30][20];		//MZ,FAI별 발생수
	int		nFOcapExist[30][10][40];	//1:Exist
	int		nMarginal[30][40];			//1:Marginal

	//Result
	CString	sRstLotID[30];
	int		nRstCmCount[30];
	int		nRstGoodCount[30];
	int		nRstNgCount[30];
	int		nUnlGdTrayCount[30];
	int		nUnlNGTrayCount[30];
	CString	sUnLastLotID;
	int		nUnLastPortID;

	CString sLLStartTime[30];
	CString sLLEndTime[30];
	DWORD	dwLLLotStart[30];
	DWORD	dwLLLotEnd[30];
	double  dLLTackTime[30];
	CString sLLSHHMMSS[30];
	CString sLLEHHMMSS[30];

	BOOL	bMESStart;
	CString sMESStartTime[30];
	CString sMESEndTime[30];
	CString	sELotID;
	int		nRunPortNo;
	int		nErrorCount[30];
	DWORD	dwErrorTime[30];
	DWORD	dwStopTime[30];
	int		nMZCountGD;
	int		nMZCountNG;

	//JobList
	int		nJobNo;
	int		nJobPortNo[100];
	CString	sJobLotID[100];
	int		nJobCmCount[100];
	int		nJobGoodCount[100];
	int		nJobNgCount[100];
	double	dJobRate[100];
	CString	sJobStartTime[100];
	CString	sJobEndTime[100];
	double	dJobTack[100];

	int		nJobLog;
	DWORD	dwJobStart;
	DWORD	dwJobtEnd;
	int		nJobCount;
	double	dJobTotalTack;
	CString	sJobTotStartTime;
	CString	sJobTotEndTime;
	int		nStartPno;
} GLOVAL_LOT;

typedef struct {
	BOOL	bBegin;
	int		nAlmNo;
	int		nPortNo;
	CString sAlmMsg;
	CString sStartTime;
	CString sEndTime;
	CString sLotID;
	CString sSpcInfo;
	CString sUnit;
	CString sGroup;
	CString sItems;
	DWORD	dwStartTime;
	DWORD	dwEndTime;
	DWORD	dwProcTime;
	CString sAlmLotID[3];
	double  dMotionChkPos;
	double  dMotionPos[60];
	BOOL	bStage[20];

	CString sAlmLID[5];	//0:검사,1;BTM,2:Top1,3:Top2
	int		nAlmTNo[5];	//0:검사,1;BTM,2:Top1,3:Top2
	int		nAlmPNo[5];	//0:검사,1;BTM,2:Top1,3:Top2
} GLOVAL_ALM;

typedef struct {
	int		nLotPortNo;
	int		nLotStatus[30];					// 0:None, 1:Start Send, 2:Lot Start Receive, 3:Lot End
	DWORD	dwMesTime;
	int		nMZConfirm[3];					//0:L, 1:G, 2:N
	CString sMZConfirmID;
	int		nCarConfirm[3];					//0:L, 1:G, 2:N
	CString sCarConfirmID;

	BOOL	bRcpBodyDone;
	CString	sRcpVersion;
	CString sBodyData[4][50];				// 0:T1M-PC2(1~32), 1:T1S-PC4(1~32), 2:T2M-PC3(1~44), 3:T2S-PC5(1~44)

	// 장비 실적 Data
	int			nCmResult[30][10][40];		// 0:Not Receive, 1:Good Done, 2:NG Done
	
	// Host Data
	CString		sHostLotID;					// Host 수신 LotID
	CString		sHostRecipe;				// Host 수신 Recipe
	int			nHostCmCount;				// Host 수신 Lot당 CM 총갯수
	CString		sHostVendor;				// Host 수신 Vendor Type
	CString		sHostConfig;				// Host 수신 Config Code

	// Cancel Data
	CString		sHostCancelLotId;
	CString		sHostCancelModule;			// Host Cancel 수신 code
	CString		sHostCancelMGZID;			// Host Cancel 수신 code
	CString		sHostCancelTrayID;			// Host Cancel 수신 code
	CString		sHostCancelCode;			// Host Cancel 수신 code
	CString		sHostCancelText;			// Host Cancel 수신 내용
} GLOVAL_MES;

typedef struct {
	
	//매거진 인덱스 정보 전달 : 캐리어 --> 트랜스퍼 피커 --> 컨베이어 
	//0:Good . 1: NG
	int nCarrierMZIndex[2];
	int nTransferMZIndex[2];
	int nCVMZIndex[2];

	int nMZIdx_Cosmetic;
	int nOcapCarrierIndex;

	int nLotIndex_NG;
	int nLotIndex_Good;

	//ADJ Overwritten
	int nDefectCntInCarr_Cosmetic[50][28];	//md(module) in Carrier , NG category 50, 28 lot 
	int nGoodCntInCarr_Cosmetic[28]; // 28 lot 

	int nMDGoodCntOutput[50];
	int nMDNGCntOutput[50];
	
	int nDefectCnt_Cosmetic[50][50][28]; //50가지 종류의 불량,  50개의 매거진 루프, 28개 랏 
	int nGoodCnt_Cosmetic[50][28]; // 28개 랏 
	
	//Only Vision 
	int nDefectCntInCarr_CosmeVision[50][28];		
		
	int nNGVisionCnt_MZ[50];		
	int nCosmeVisionCnt_MZ[50][50]; //1번째 NG종류 50, 2번째 50개 매거진 루프
	
	//Commonly Used 
	int nTotalCnt_MZ[50];
	int nGoodCnt_MZ[50]; // 1번째: NG 종류, 2번째:매거진 루프 개수 
	int nNGCnt_MZ[50];	
	int nCosmeticCnt_MZ[50][50]; //1번째 NG종류 50, 2번째 50개 매거진 루프
		
	CString		sCosmeticName[50];
	CString		sCosmeticCode[50];
	int			nCosmeticCount;		// 외관 에러 항목 개수 

	double		dDefectPercent[3];		// 설정된 불량율%
	int			nConsecutiveMZLimit[3];	// MZ수량 (연속알람 발생 MZ 개수 설정된거)
	int			nMinModuleCnt;			// MZ당최소 Module 수량 (이하면 알람발생 제외처리)
	int			nConsecutiveMZCount[6][50];	
	int			nCosmeticCodeNum[6];
	
	CString		sAlmMZID_Cosmetic;
	CString		sAlmDefectName_Cosmetic;
	CString		sAlmDefectCode_Cosmetic;
	double		dAlmDefectPercent_Cosmetic;
	int			nAlmNGCount_Cosmetic;
	
	//display
	int			nMZCycle_Cosmetic;
	CString		sDate_Cosmetic[50];			//날자(07/31)
	CString		sTime_Cosmetic[50];			//시간(07:31)
	CString		sMZID_Cosmetic[50];			//양품MZ
	int			nTotCount_Cosmetic[50];		//투입-Module수
	int			nGoodCount_Cosmetic[50];		//양품수
	int			nNGCount_Consmetic[50];
	
	BOOL		bOCAPDone[6]; // 3가지 조건 
	BOOL		bErrorShowDone;


	//기존 변수들
	int			nMZCycle;
	CString		sDate[50];			//날자(07/31)
	CString		sTime[50];			//시간(07:31)
	CString		sMZID[50];			//양품MZ
	CString		sLotID[50][8];		//LotID
	int			nTotCount[50];		//투입-Module수
	int			nGoodCount[50];		//양품수
	int			nROSNGCount[50];	//ROS-NG수
	int			nROSRfCount[50];	//ROS-Repaier수
	int			nBCRCount[50];		//Barcode[NG수
	int			nMESCount[50];		//MES[NG수
	int			nMCCount[50];		//MC[NG수
	int			nCount[50][20];		//MZ,FAI별 발생수
	int			nFCount[50][20];	//MZ,FAI별 발생수

	CString		sFAIName[20];		//FAI이름-Display
	CString		sFAICode[20];		//FAI-NG-Code
	double		dGiDefect[4];		//불량율%
	int			nGiMZCnt[4];		//MZ수량(알람발생MZ대상수)
	int			nGiMinCnt;			//MZ당최소 Module수량(이하면 알람발생 제외처리)

	CString		sAlmMZID;
	CString		sAlmFAIName;
	double		dAlmDefect;
	int			nAlmCount;



} GLOVAL_OCAP;

typedef struct {

	CString sMZID_LoadMZElev;
	CString	sMZID_Trasnfer1;
	CString sMZID_LoadStage[2];
	CString sMZID_LoadStageModule[2][10][4];
	CString	sMZID_LoadPicker[2][PICK_CNT];
	CString	sMZID_InspectStage[4][PICK_CNT];
	CString	sMZID_UnloadPicker[2][PICK_CNT];
	CString sMZID_NGTray[10][4];
	CString sMZID_GoodTray[10][4];

	CString sMZID_Transfer2[10][4];

	CString sMZID_GoodMZElev[28][10][4];
	CString sMZID_NGMZElev[28][10][4];

	CString sMZID_GoodMZ[28][10][4];
	CString sMZID_NGMZ[28][10][4];

	CString sMZID_GOODCV;
	CString sMZID_NGCV;

} GLOVAL_TRACKING;


extern GLOVAL_DATA		gData;
extern GLOVAL_LOT		gLot;
extern GLOVAL_ALM		gAlm;
extern GLOVAL_MES		gMes;
extern GLOVAL_OCAP		gCap;
extern GLOVAL_TRACKING	gTracking;

typedef struct {
	CString		sNGCode[30][1][40][5][20];	//Port[30],TrayNo[1],Module[40],Inspection[5],NG-Code[20]
	CString		sModuleID[30][40];			//Port[30],Module[40], Barcode
	CString		sHaimNGCd[30][40];			//Port[30],Module[40], HaimNG_Code
	CString		sCosmeticNG[30][40];		//Port[30],Module[40], Cosmetic_Judge(NG)
	int			nTrayOX[3][2];				//0:Load, 1:NG, 2:Good,     1:Exist, Stage에 Tray가 있는지 여부 
	CString		sBackData[30][2];
	int			nBackCnt[30];
} GLOVAL_NGCODE;
extern GLOVAL_NGCODE* gNG;


typedef struct {
	int		nType;			//nType:1[정시], 2[해제] 3[설정]
	int		nOpenStart;		//1:Start
	DWORD	dwOpenStartTime;
	int		nOpenTime;
	int		nLogYY;		//등록년
	int		nLogMM;		//등록월
	int		nLogDD;		//등록날
	int		nLogHH;		//등록시간
} GLOVAL_INTERLOCKDATA;
extern  GLOVAL_INTERLOCKDATA	gIt;



/*
typedef struct {
	CString		sLotID;
	CString		sData1[320];
	CString		sData2[320];
	CString		sData3[320];
	CString		sData4[320];
	CString		sData5[320];
} GLOVAL_TEST;
extern GLOVAL_TEST* Gt;
*/

