#include "pch.h"
#include "CameraControl.h"

typedef struct
{
	MIL_ID MilDigitizer1;
	MIL_ID MilGrabDisplay1;
	MIL_ID* MilImage1;
	MIL_ID MilDispImage1;
	MIL_ID MilDispImagebuf1;
	MIL_INT GrabDoneCAM1;
	MIL_INT ProcesseStop1;
	long    ProcessedImageCount1;
	MIL_ID MilGrabDisplay2;
	MIL_ID MilGrabDisplay3;
	MIL_ID MilGrabDisplay4;
	MIL_ID MilDispImage2;
	MIL_ID MilDispImage3;
	MIL_ID MilDispImage4;

	MIL_ID* BufChild;
	MIL_ID BufParent;
} HookDataStruct;
HookDataStruct UserHookData;

vector<Mat>Image_Buffer;
vector<Mat>Grab_Buffer;

MIL_INT BufX;
MIL_INT BufY;

MIL_INT MFTYPE ProcessingFunction1(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);
MIL_INT MFTYPE FrameHandler_1(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);
MIL_INT MFTYPE ProcessingFunction2(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);

clock_t* stime = new clock_t;
clock_t* etime = new clock_t;
double* ttime = new double;
int* BufCtrl_cpp = new int;
int* Total_Frame_Num_cpp = new int;
MIL_ID MilSystem_backup;

MIL_ID BufParent;
MIL_ID BufParent_Test;
MIL_ID LineDisp;
MIL_ID LineDisp_Test;
MIL_ID* BufChild;

void CameraControl::BoardAlloc()
{
	if (boardselector == 0) // CXP
	{
		// Mil App, Sys Alloc
		MappAlloc(M_DEFAULT, &MilApplication);
		MsysAlloc(M_SYSTEM_RAPIXOCXP, M_DEV0, M_DEFAULT, &MilSystem);

		MilSystem_backup = MilSystem;

		//MIL_INT DIG_NUM;
		//MIL_INT Discover_device_Num;

		//MsysInquire(MilSystem, M_DIGITIZER_NUM, &DIG_NUM);
		//MsysInquire(MilSystem, M_DISCOVER_DEVICE_COUNT, &Discover_device_Num);

		//cout << (int)DIG_NUM << endl;
		//cout << (int)Discover_device_Num << endl;

		AfxMessageBox(_T("Board Allocation OK!"));
	}
	else if (boardselector == 1) // CL
	{

	}
}

int CameraControl::DigitAlloc(MIL_ID MilSystem, CString DCF_NAME, MIL_ID Digitizer)
{
	MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);

	int alloccount = 0;

	while (1) {
		if (MdigAlloc(MilSystem, M_DEV0, DCF_NAME, M_DEFAULT, &Digitizer)) {
			cout << "M_DEV0 Digitizer Alloc Success!" << endl;
			AfxMessageBox(_T("M_DEV0 Digitizer Alloc Success!"));
			MdigInquire(Digitizer, M_SIZE_X, &SizeX1);
			MdigInquire(Digitizer, M_SIZE_Y, &SizeY1);
			MdigInquire(Digitizer, M_SIZE_BIT, &SizeBit1);
			MdigInquire(Digitizer, M_SIZE_BAND, &SizeBand1);
			// MdigInquire(Digitizer, M_SELECTED_FRAME_RATE, &FrameRate);

			MdigControl(Digitizer, M_GRAB_TIMEOUT, M_INFINITE);
			BufX = SizeX1;
			BufY = SizeY1;
			cout << endl;
			cout << "[Camera 1 - Info.]" << endl;
			cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
			cout << "  SizeBit : " << (int)SizeBit1 << endl;
			cout << "  SizeBand : " << (int)SizeBand1 << endl;
			// cout << "FrameRate : " << (int)FrameRate << endl;
			break;
		}
		else if (MdigAlloc(MilSystem, M_DEV1, DCF_NAME, M_DEFAULT, &Digitizer)) {
			cout << "M_DEV1 Digitizer Alloc Success!" << endl;
			AfxMessageBox(_T("M_DEV1 Digitizer Alloc Success!"));
			MdigInquire(Digitizer, M_SIZE_X, &SizeX1);
			MdigInquire(Digitizer, M_SIZE_Y, &SizeY1);
			MdigInquire(Digitizer, M_SIZE_BIT, &SizeBit1);
			MdigInquire(Digitizer, M_SIZE_BAND, &SizeBand1);
			// MdigInquire(Digitizer, M_SELECTED_FRAME_RATE, &FrameRate);

			MdigControl(Digitizer, M_GRAB_TIMEOUT, M_INFINITE);
			BufX = SizeX1;
			BufY = SizeY1;
			cout << endl;
			cout << "[Camera 1 - Info.]" << endl;
			cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
			cout << "  SizeBit : " << (int)SizeBit1 << endl;
			cout << "  SizeBand : " << (int)SizeBand1 << endl;
			// cout << "FrameRate : " << (int)FrameRate << endl;
			break;
		}
		else if (MdigAlloc(MilSystem, M_DEV2, DCF_NAME, M_DEFAULT, &Digitizer)) {
			cout << "M_DEV2 Digitizer Alloc Success!" << endl;
			AfxMessageBox(_T("M_DEV2 Digitizer Alloc Success!"));
			MdigInquire(Digitizer, M_SIZE_X, &SizeX1);
			MdigInquire(Digitizer, M_SIZE_Y, &SizeY1);
			MdigInquire(Digitizer, M_SIZE_BIT, &SizeBit1);
			MdigInquire(Digitizer, M_SIZE_BAND, &SizeBand1);
			// MdigInquire(Digitizer, M_SELECTED_FRAME_RATE, &FrameRate);

			MdigControl(Digitizer, M_GRAB_TIMEOUT, M_INFINITE);
			BufX = SizeX1;
			BufY = SizeY1;
			cout << endl;
			cout << "[Camera 1 - Info.]" << endl;
			cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
			cout << "  SizeBit : " << (int)SizeBit1 << endl;
			cout << "  SizeBand : " << (int)SizeBand1 << endl;
			// cout << "FrameRate : " << (int)FrameRate << endl;
			break;
		}
		else if (MdigAlloc(MilSystem, M_DEV3, DCF_NAME, M_DEFAULT, &Digitizer)) {
			cout << "M_DEV3 Digitizer Alloc Success!" << endl;
			AfxMessageBox(_T("M_DEV3 Digitizer Alloc Success!"));
			MdigInquire(Digitizer, M_SIZE_X, &SizeX1);
			MdigInquire(Digitizer, M_SIZE_Y, &SizeY1);
			MdigInquire(Digitizer, M_SIZE_BIT, &SizeBit1);
			MdigInquire(Digitizer, M_SIZE_BAND, &SizeBand1);
			// MdigInquire(Digitizer, M_SELECTED_FRAME_RATE, &FrameRate);

			MdigControl(Digitizer, M_GRAB_TIMEOUT, M_INFINITE);
			BufX = SizeX1;
			BufY = SizeY1;
			cout << endl;
			cout << "[Camera 1 - Info.]" << endl;
			cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
			cout << "  SizeBit : " << (int)SizeBit1 << endl;
			cout << "  SizeBand : " << (int)SizeBand1 << endl;
			// cout << "FrameRate : " << (int)FrameRate << endl;
			break;
		}
		else if (alloccount == 3) {
			cout << "No Digitizer on Board" << endl;
			AfxMessageBox(_T("No Digitizer on Board"));
			break;
		}
		alloccount++;
	}
	MappControl(M_DEFAULT, M_ERROR, M_PRINT_ENABLE);
	return Digitizer;
}

tuple<MIL_ID, MIL_ID*, MIL_ID, MIL_ID, MIL_INT, MIL_INT, MIL_INT, MIL_INT> CameraControl::BufnDispAlloc(MIL_ID Digitizer, MIL_ID* GrabBuf, MIL_ID DispBuf, MIL_ID Disp)
{
	MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &Disp);

	for (int i = 0; i < GRAB_NUM; i++)
	{
		MbufAllocColor(MilSystem, SizeBand1, SizeX1, SizeY1, SizeBit1 + M_UNSIGNED, M_IMAGE + M_GRAB + M_DISP, GrabBuf + i);
		MbufClear(GrabBuf[i], 0);
	}

	MbufAllocColor(MilSystem, SizeBand1, SizeX1, SizeY1, SizeBit1 + M_UNSIGNED, M_IMAGE + M_GRAB + M_DISP, &DispBuf);

	
	UserHookData.MilDigitizer1 = Digitizer;
	UserHookData.MilImage1 = GrabBuf;
	UserHookData.MilDispImage1 = DispBuf;

	UserHookData.ProcessedImageCount1 = 0;
	UserHookData.GrabDoneCAM1 = 0;

	cout << "  Camera 1 - Buffer & Display Alloc OK" << endl;

	return make_tuple(Digitizer, GrabBuf, DispBuf, Disp, SizeBand1, SizeX1, SizeY1, SizeBit1);
}

tuple<MIL_ID, MIL_ID, MIL_ID> CameraControl::otheralloc()
{
	MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilGrabDisplay2);
	MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilGrabDisplay3);
	MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilGrabDisplay4);

	return make_tuple(MilGrabDisplay2, MilGrabDisplay3, MilGrabDisplay4);
}

void CameraControl::GrabProcess(MIL_ID Digitizer, MIL_ID *GrabBuf, MIL_ID DispBuf, MIL_ID Disp)
{
	UserHookData.ProcessedImageCount1 = 0;
	UserHookData.GrabDoneCAM1 = 0;
	cout << Disp << endl;

	MdispSelect(Disp, DispBuf);

	cout << Disp << endl;
	// MdigHookFunction(Digitizer, M_GRAB_FRAME_START, FrameHandler_1, &UserHookData);
	
	*BufCtrl_cpp = BufCtrl;

	MdigProcess(Digitizer, GrabBuf, GRAB_NUM, M_START, M_ASYNCHRONOUS, ProcessingFunction1, &UserHookData);

	while (1)
	{
		*BufCtrl_cpp = BufCtrl;

		if (UserHookData.GrabDoneCAM1 == 1)
		{
			MdigProcess(Digitizer, GrabBuf, GRAB_NUM, M_STOP, M_DEFAULT, ProcessingFunction1, &UserHookData);
			cout << endl;
			cout << "STOP SIGNAL" << endl;
			MdigHalt(Digitizer);
			break;
		}
		ProcessWindowMessage();

		// cout << (double)(1000.0 / (double)*ttime) << " fps" << endl;
	}
}

MIL_INT MFTYPE ProcessingFunction1(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
{
	HookDataStruct* UserHookDataPtr = (HookDataStruct*)HookDataPtr;

	Mat Temp;
	Temp.create(BufY, BufX, CV_8UC3);
	// Mat Temp_Thres;

	MIL_ID ModifiedBufferIdCAM1;

	/* Retrieve the MIL_ID of the grabbed buffer. */
	MdigGetHookInfo(HookId, M_MODIFIED_BUFFER + M_BUFFER_ID, &ModifiedBufferIdCAM1);

	MbufCopy(ModifiedBufferIdCAM1, UserHookDataPtr->MilDispImage1);

	MbufGetColor2d(ModifiedBufferIdCAM1, M_PACKED + M_BGR24, M_ALL_BANDS, 0, 0, BufX, BufY, (void*)Temp.data);

	switch (*BufCtrl_cpp) // 0 = Normal Grab , 1 = Double Grab , 2 = Triple Grab
	{
	case 1:
		threshold(Temp, Temp, 100, 255, THRESH_BINARY);
		imshow("Test", Temp);
		waitKey(1);
		break;
	case 2:
		break;
	case 3:
		break;
	}
	// imshow("test", Temp);
	//if (cyscustom.Processing == true)
	//{
	//	switch (cyscustom.ProcessingID_1)
	//	{
	//	case 1:
	//		threshold(Temp, Temp_Thres, cyscustom.ThresValue, cyscustom.ThresMaxValue, cyscustom.Threstype);
	//		break;
	//	case 2:
	//		break;
	//	}
	//}
	// MbufPutColor2d(UserHookDataPtr->MilDispImage1, M_PACKED + M_BGR24, M_ALL_BANDS, 0, 0, cyscustom.SizeX1, cyscustom.SizeY1, (void*)Temp_Thres.data);

	UserHookDataPtr->ProcessedImageCount1++;
	*etime = clock();
	double Ttime = ((double)(*etime - *stime));
	*ttime = Ttime;
	return 0;
}

MIL_INT MFTYPE ProcessingFunction2(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
{
	HookDataStruct* UserHookDataPtr = (HookDataStruct*)HookDataPtr;
	MIL_ID ModifiedBufferId;
	MdigGetHookInfo(HookId, M_MODIFIED_BUFFER + M_BUFFER_ID, &ModifiedBufferId);
	MbufCopy(ModifiedBufferId, UserHookDataPtr->BufChild[UserHookDataPtr->ProcessedImageCount1]);
	UserHookDataPtr->ProcessedImageCount1++;
	if (UserHookDataPtr->ProcessedImageCount1 == *Total_Frame_Num_cpp)
	{
		MbufSave(MIL_TEXT(".\\Grab_Result.bmp"), UserHookDataPtr->BufParent);
		cout << endl;
		cout << "Save OK " << endl;
	}
	return 0;
}

void CameraControl::ProcessWindowMessage()
{
	MSG msg;

	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}
}

void CameraControl::GrabStop()
{
	if (UserHookData.GrabDoneCAM1 == 0) {
		UserHookData.GrabDoneCAM1 = 1;
	}
}

Mat* CameraControl::ShotGrab(MIL_ID Digitizer, MIL_ID Disp, MIL_ID DispBuf, MIL_INT SizeX, MIL_INT SizeY)
{
	MdispSelect(Disp, DispBuf);
	MdigGrab(Digitizer, DispBuf);

	Mat temp;
	temp.create(SizeY, SizeX, CV_8UC3);
	MbufGetColor2d(DispBuf, M_PACKED + M_BGR24, M_ALL_BANDS, 0, 0, SizeX, SizeY, (void*)temp.data);

	Image_Buffer.clear();
	Image_Buffer.push_back(temp);

	Mat* BufAd = &Image_Buffer[0];

	return BufAd;
}

Mat* CameraControl::GrabProcess_Line(MIL_ID Digitizer, MIL_ID Disp, MIL_ID DispBuf, MIL_INT SizeX, MIL_INT SizeY)
{
	*Total_Frame_Num_cpp = TotalFrameNum;
	Grab_Buffer.clear();

	for (int i = 0; i < *Total_Frame_Num_cpp; i++)
	{
		MdigGrab(Digitizer, DispBuf);
		Mat temp;
		temp.create(SizeY, SizeX, CV_8UC3);
		MbufGetColor2d(DispBuf, M_PACKED + M_BGR24, M_ALL_BANDS, 0, 0, SizeX, SizeY, (void*)temp.data);
		Grab_Buffer.push_back(temp);
	}
	
	cout << "Grab_Buffer Size : " << Grab_Buffer.size() << endl;

	Mat temp1;
	Mat temp2;
	Mat result;

	int count = 0;

	while (count < Grab_Buffer.size())
	{
		if (count == 0) {
			temp1 = Grab_Buffer[count];
			temp2 = Grab_Buffer[count + 1];
			vconcat(temp1, temp2, result);
			count = 2;
		}
		temp1 = result;
		temp2 = Grab_Buffer[count];
		vconcat(temp1, temp2, result);
		count++;
	}

	Grab_Buffer.clear();
	Image_Buffer.clear();
	Image_Buffer.push_back(result);

	Mat* BufAd = &Image_Buffer[0];
	return BufAd;
}

void CameraControl::GrabProcess_Line2(MIL_ID Digitizer, MIL_ID* GrabBuf)
{
	MbufClear(BufParent, 0);

	MdigProcess(Digitizer, GrabBuf, GRAB_NUM, M_SEQUENCE + M_COUNT(*Total_Frame_Num_cpp), M_ASYNCHRONOUS, ProcessingFunction2, &UserHookData);

	while (1)
	{
		if (UserHookData.ProcessedImageCount1 == *Total_Frame_Num_cpp)
		{
			MdigProcess(Digitizer, GrabBuf, GRAB_NUM, M_STOP, M_DEFAULT, ProcessingFunction2, &UserHookData);

			MbufCopy(BufParent, BufParent_Test);
			MdispSelect(LineDisp_Test, BufParent_Test);
			UserHookData.ProcessedImageCount1 = 0;
			MdigHalt(Digitizer);

			for (int i = 0; i < *Total_Frame_Num_cpp; i++)
			{
				MbufFree(BufChild[i]);
			}
			MbufFree(BufParent);

			// cout << "Memory Free OK " << endl;
			break;
		}
	}
}

void CameraControl::GrabLine_Alloc(MIL_ID Digitizer, MIL_ID* GrabBuf)
{
	*Total_Frame_Num_cpp = TotalFrameNum;
	BufChild = new MIL_ID[*Total_Frame_Num_cpp];
	UserHookData.ProcessedImageCount1 = 0;

	MdigInquire(Digitizer, M_SIZE_X, &SizeX1);
	MdigInquire(Digitizer, M_SIZE_Y, &SizeY1);
	MdigInquire(Digitizer, M_SIZE_BIT, &SizeBit1);
	MdigInquire(Digitizer, M_SIZE_BAND, &SizeBand1);

	MbufAlloc2d(MilSystem_backup, SizeX1, SizeY1 * (*Total_Frame_Num_cpp), SizeBit1 + M_UNSIGNED, M_IMAGE + M_GRAB + M_DISP, &BufParent);

	// MdispAlloc(MilSystem_backup, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &LineDisp);

	for (int i = 0; i < *Total_Frame_Num_cpp; i++)
	{
		MbufChild2d(BufParent, 0, SizeY1 * i, SizeX1, SizeY1, BufChild + i);
		MbufClear(BufChild[i], 0);
	}

	UserHookData.MilDigitizer1 = Digitizer;
	UserHookData.MilImage1 = GrabBuf;
	UserHookData.BufChild = BufChild;
	UserHookData.BufParent = BufParent;

	// MdispSelect(LineDisp, BufParent);
}

void CameraControl::FeatureBrowser(MIL_ID Digitizer)
{
	MdigControl(Digitizer, M_GC_FEATURE_BROWSER, M_OPEN + M_ASYNCHRONOUS);
}

MIL_INT MFTYPE FrameHandler_1(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr)
{
	*stime = clock();

	return 0;
}

void CameraControl::DigitFree(MIL_ID Digitizer, MIL_ID DispBuf, MIL_ID Disp, MIL_ID* GrabBuf, MIL_ID Disp2, MIL_ID Disp3, MIL_ID Disp4)
{
	MbufFree(BufParent_Test);
	MdispFree(LineDisp_Test);
	MdispFree(Disp);
	MdispFree(Disp2);
	MdispFree(Disp3);
	MdispFree(Disp4);
	MbufFree(DispBuf);

	for (int i = 0; i < GRAB_NUM; i++)
	{
		MbufFree(GrabBuf[i]);
	}

	MdigFree(Digitizer);
	MsysFree(MilSystem_backup);
	MappFree(MilApplication);
	cout << endl;
	cout << "Digitizer Free OK" << endl;
}

int CameraControl::DisplayAlloc(MIL_ID Digitizer)
{
	*Total_Frame_Num_cpp = TotalFrameNum;
	MdigInquire(Digitizer, M_SIZE_X, &SizeX1);
	MdigInquire(Digitizer, M_SIZE_Y, &SizeY1);
	MdigInquire(Digitizer, M_SIZE_BIT, &SizeBit1);
	MdigInquire(Digitizer, M_SIZE_BAND, &SizeBand1);

	MdispAlloc(MilSystem_backup, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &LineDisp_Test);
	MbufAlloc2d(MilSystem_backup, SizeX1, SizeY1 * (*Total_Frame_Num_cpp), SizeBit1 + M_UNSIGNED, M_IMAGE + M_GRAB + M_DISP, &BufParent_Test);

	return LineDisp_Test;
}

void CameraControl::DisplayAlloc_Free(MIL_ID Disp)
{
	MbufFree(BufParent_Test);
	MdispFree(Disp);
	// cout << "Memory Free OK " << endl;
}