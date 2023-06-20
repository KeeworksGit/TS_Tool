
#pragma once

#include "Mil.h"
#include <iostream>
#include <string>
#include <chrono>
#include <tuple>
#include <time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>

#define GRAB_NUM 20

using namespace std;
using namespace cv;
using namespace chrono;

class CameraControl
{
public:
    MIL_INT TOTAL_FRAME_NUM;
	MIL_ID MilApplication;
	MIL_ID MilSystem;

	MIL_ID MilDigitizer1 = 1;
	MIL_ID MilDigitizer2 = 2;
	MIL_ID MilDigitizer3 = 3;
	MIL_ID MilDigitizer4 = 4;
	MIL_ID MilImage1[GRAB_NUM];
	MIL_ID MilImage2[GRAB_NUM];
	MIL_ID MilImage3[GRAB_NUM];
	MIL_ID MilImage4[GRAB_NUM];

	CString DevNum;
	MIL_STRING CamName;

	MIL_ID MilGrabDisplay1; // Display Value
	MIL_ID Mil1SGrabDisplay1; // Display Value
	MIL_ID MilGrabDisplay2;
	MIL_ID MilGrabDisplay3;
	MIL_ID MilGrabDisplay4;

	MIL_ID MilGrabDisplay21;
	MIL_ID MilGrabDisplay22;
	MIL_ID MilGrabDisplay23;
	MIL_ID MilGrabDisplay24;

	MIL_ID Mil1SGrabDispImage1; // Display buffer
	MIL_ID Mil1SGrabDispImage2; // Display buffer
	MIL_ID Mil1SGrabDispImage3; // Display buffer
	MIL_ID Mil1SGrabDispImage4; // Display buffer
	MIL_ID MilDispImage1; // Display buffer
	MIL_ID MilDispImage2;
	MIL_ID MilDispImage3;
	MIL_ID MilDispImage4;

	MIL_INT SizeX1;
	MIL_INT SizeY1;
	MIL_INT SizeBit1;
	MIL_INT SizeBand1 = 3;
	MIL_STRING PixelFormat1;
	CString CPixelFormat1;
	MIL_INT SizeX2;
	MIL_INT SizeY2;
	MIL_INT SizeBit2;
	MIL_INT SizeBand2 = 3;
	MIL_STRING PixelFormat2;
	CString CPixelFormat2;
	MIL_INT SizeX3;
	MIL_INT SizeY3;
	MIL_INT SizeBit3;
	MIL_INT SizeBand3 = 3;
	CString CPixelFormat3;
	MIL_INT SizeX4;
	MIL_INT SizeY4;
	MIL_INT SizeBit4;
	MIL_INT SizeBand4 = 3;
	CString CPixelFormat4;
	MIL_DOUBLE FrameRate;

	CString DCF_Name_1;
	CString DCF_Name_2;
	CString DCF_Name_3;
	CString DCF_Name_4;

	CString sDriveCAM;
	CString filePath;
	CTime file_time;
	CString strTime;
	CString file_format = _T(".bmp"); // DEFAULT VALUEW

	int boardselector; // 보드 셀렉터
	int Digitizercnt = 0; // 카메라 개수

	bool Processing; // Processing Switch
	int ProcessingID_1; // Processing ID 1
	int ProcessingID_2; // Processing ID 1
	int ProcessingID_3; // Processing ID 1
	int ProcessingID_4; // Processing ID 1
	int BufCtrl = 0;
	int TotalFrameNum; // 


	// Edge Finder 변수
	double min = 50;
	double max = 150;

	void BoardAlloc();
	int DigitAlloc(MIL_ID MilSystem, CString DCF_NAME, MIL_ID Digitizer);
	tuple<MIL_ID, MIL_ID*, MIL_ID, MIL_ID, MIL_INT, MIL_INT, MIL_INT, MIL_INT> BufnDispAlloc(MIL_ID Digitizer, MIL_ID* GrabBuf, MIL_ID DispBuf, MIL_ID Disp);
	tuple<MIL_ID, MIL_ID, MIL_ID> otheralloc();
	void GrabProcess(MIL_ID Digitizer, MIL_ID* GrabBuf, MIL_ID DispBuf, MIL_ID Disp);
	void ProcessWindowMessage();
	void GrabStop();
	Mat* ShotGrab(MIL_ID Digitizer, MIL_ID Disp, MIL_ID DispBuf, MIL_INT SizeX, MIL_INT SizeY);
	Mat* GrabProcess_Line(MIL_ID Digitizer, MIL_ID Disp, MIL_ID DispBuf, MIL_INT SizeX, MIL_INT SizeY);
	void GrabProcess_Line2(MIL_ID Digitizer, MIL_ID* GrabBuf);
	void GrabLine_Alloc(MIL_ID Digitizer, MIL_ID* GrabBuf);
	void FeatureBrowser(MIL_ID Digitizer);
	void DigitFree(MIL_ID Digitizer, MIL_ID DispBuf, MIL_ID Disp, MIL_ID* GrabBuf, MIL_ID Disp2, MIL_ID Disp3, MIL_ID Disp4);
	int DisplayAlloc(MIL_ID Digitizer);
	void DisplayAlloc_Free(MIL_ID Disp);
};

