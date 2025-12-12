// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// CMAI2100.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

std::vector<CIniItem> glFAI_Info;
std::vector<CIniItem> glLight_Info;
std::vector<CIniItem> glParam_Info;

CString gsCurrentDir;		// 현재 프로젝트 폴더

GLOVAL_DATA			gData;
GLOVAL_LOT			gLot;
GLOVAL_ALM			gAlm;
GLOVAL_MES			gMes;
GLOVAL_OCAP			gCap;
GLOVAL_TRACKING		gTracking;

GLOVAL_NGCODE* gNG = new GLOVAL_NGCODE;

GLOVAL_INTERLOCKDATA	gIt;

/*
//GLOVAL_TEST* Gt = new GLOVAL_TEST[3];
GLOVAL_TEST* Gt = new GLOVAL_TEST[1000000];
//GLOVAL_TEST* G1 = new GLOVAL_TEST[1000000];
//GLOVAL_TEST* G2 = new GLOVAL_TEST[1000000];
//GLOVAL_TEST* G3 = new GLOVAL_TEST[1000000]; delete []G3;
*/