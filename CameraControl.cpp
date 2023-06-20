#include "pch.h"
#include "CameraControl.h"

typedef struct
{
	MIL_ID MilDigitizer1;
	MIL_ID MilDigitizer2;
	MIL_ID MilDigitizer3;
	MIL_ID MilDigitizer4;

	MIL_ID* MilImage1;
	MIL_ID* MilImage2;
	MIL_ID* MilImage3;
	MIL_ID* MilImage4;
	MIL_ID MilDispImage1;
	MIL_ID MilDispImage2;
	MIL_ID MilDispImage3;
	MIL_ID MilDispImage4;
	MIL_ID MilDispImage1_Line;
	MIL_ID MilDispImagebuf1;
	MIL_INT GrabDoneCAM1;
	MIL_INT GrabDoneCAM2;
	MIL_INT GrabDoneCAM3;
	MIL_INT GrabDoneCAM4;
	MIL_INT ProcesseStop1;
	long    ProcessedImageCount1;
	long    ProcessedImageCount2;
	long    ProcessedImageCount3;
	long    ProcessedImageCount4;
	MIL_ID MilGrabDisplay1, MilGrabDisplay2, MilGrabDisplay3, MilGrabDisplay4;
	MIL_ID MilGrabDisplay21, MilGrabDisplay22, MilGrabDisplay23, MilGrabDisplay24;

	MIL_ID* BufChild;
	MIL_ID BufParent;
} HookDataStruct;
HookDataStruct UserHookData;

MIL_ID MilDispImage1_Line;

vector<Mat>Image_Buffer;
vector<Mat>Grab_Buffer;

MIL_INT BufX; // Buffer 생성 관련자
MIL_INT BufY;
MIL_INT BufBand;
MIL_INT BufBit;
MIL_INT BufX2;
MIL_INT BufY2;
MIL_INT BufBand2;
MIL_INT BufBit2;
MIL_INT BufX3;
MIL_INT BufY3;
MIL_INT BufBand3;
MIL_INT BufBit3;
MIL_INT BufX4;
MIL_INT BufY4;
MIL_INT BufBand4;
MIL_INT BufBit4;


MIL_INT MFTYPE ProcessingFunction1(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);
MIL_INT MFTYPE ProcessingFunction2(MIL_INT HookType, MIL_ID HookId, void* HookDataPtr);
MIL_INT MFTYPE MouseFct(MIL_INT HookType, MIL_ID EventID, void* UserDataPtr);

clock_t* stime = new clock_t;
clock_t* etime = new clock_t;
double* ttime = new double;
int BufCtrl_cpp;
double* min_cpp = new double;
double* max_cpp = new double;
int* Total_Frame_Num_cpp = new int;
MIL_ID MilSystem_backup;

MIL_ID BufParent;
MIL_ID BufParent_Test;
MIL_ID LineDisp;
MIL_ID LineDisp_Test;
MIL_ID* BufChild;
Mat Temp_Mat;

MIL_INT MemoryChk; // Non-Paged Memory Check

MIL_DOUBLE m_DisplayPositionX;
MIL_DOUBLE m_DisplayPositionY;
int disp_x;
int disp_y;

// Text 변수
Point MyPoint;
int MyFontFace = 2;
double MyFontScale = 15.0;

// Digitizer Count
int Digitizercnt_cpp; 


void CameraControl::BoardAlloc()
{
	if (boardselector == 0) // CXP
	{
		try
		{
			// Mil App, Sys Alloc
			MappAlloc(M_DEFAULT, &MilApplication);
			MsysAlloc(M_SYSTEM_RAPIXOCXP, M_DEV0, M_DEFAULT, &MilSystem);
			MilSystem_backup = MilSystem;
			AfxMessageBox(_T("(RapixoCXP) Board Allocation OK!"));
		}
		catch (CException* ex)
		{
			CString strMsg;
			strMsg.Format(_T("Board Alloc Fail!"));
			::AfxMessageBox(strMsg);
		}
	}
	else if (boardselector == 1) // Radient EV CL
	{
		try
		{
			// Mil App, Sys Alloc
			MappAlloc(M_DEFAULT, &MilApplication);
			MsysAlloc(M_SYSTEM_SOLIOS, M_DEV0, M_DEFAULT, &MilSystem);

			MilSystem_backup = MilSystem;
			AfxMessageBox(_T("(Solios) Board Allocation OK!"));
		}
		catch (CException* ex)
		{
			CString strMsg;
			strMsg.Format(_T("Board Alloc Fail!"));
			::AfxMessageBox(strMsg);
		}
	}
	else if (boardselector == 2)
	{
		try
		{
			// Mil App, Sys Alloc
			MappAlloc(M_DEFAULT, &MilApplication);
			MsysAlloc(M_SYSTEM_RADIENTEVCL, M_DEV0, M_DEFAULT, &MilSystem);
			MilSystem_backup = MilSystem;
			AfxMessageBox(_T("(Radient EV-CL) Board Allocation OK!"));
		}
		catch (CException* ex)
		{
			CString strMsg;
			strMsg.Format(_T("Board Alloc Fail!"));
			::AfxMessageBox(strMsg);
		}
	}
	else if (boardselector == 3)
	{
		try
		{
			// Mil App, Sys Alloc
			MappAlloc(M_DEFAULT, &MilApplication);
			MsysAlloc(M_SYSTEM_GIGE_VISION, M_DEV0, M_DEFAULT, &MilSystem);
			MilSystem_backup = MilSystem;
			AfxMessageBox(_T("(Radient EV-CL) Board Allocation OK!"));
		}
		catch (CException* ex)
		{
			CString strMsg;
			strMsg.Format(_T("Board Alloc Fail!"));
			::AfxMessageBox(strMsg);
		}
	}

}

int CameraControl::DigitAlloc(MIL_ID MilSystem, CString DCF_NAME, MIL_ID Digitizer)
{
	MappControl(M_DEFAULT, M_ERROR, M_PRINT_DISABLE);

	// cout << "Digitizer_Num : " << Digitizercnt << endl;

	if (boardselector == 0)
	{
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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;

				DevNum = _T("DEV0");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV1");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV2");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;

				DevNum = _T("DEV3");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
	else if (boardselector == 1) // Solios CL
	{
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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV0");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
			
				DevNum = _T("DEV1");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV2");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV3");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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

		MIL_STRING genICamPath = MIL_TEXT("C:\Program Files\Matrox Imaging\Drivers\GenICam\V3_3\bin/Win64_x64#GenCP_MD_VC120_v3_0_MTX.dll#GenICam#GenCP_01_00");
		// CString genICamPath2 = _T("C:\Program Files\Matrox Imaging\Drivers\GenICam\V3_3\bin/Win64_x64#GenCP_MD_VC120_v3_0_MTX.dll#GenICam#GenCP_01_00");
		MdigInquire(Digitizer, M_GC_CLPROTOCOL_DEVICE_ID + 1, genICamPath);
		MdigControl(Digitizer, M_GC_CLPROTOCOL_DEVICE_ID, genICamPath);
		MdigControl(Digitizer, M_GC_CLPROTOCOL, M_ENABLE);

		int bitsPerChannel = (int)SizeBit1 / 8;

		if (SizeBit1 % 8 != 0) {
			bitsPerChannel++;
		}

		bitsPerChannel *= 8;

		SizeBit1 = bitsPerChannel;

		return Digitizer;
	}
	else if (boardselector == 2) // Radient Ev CL
	{ 
		int alloccount = 0;
		while (1) {
			if (MdigAlloc(MilSystem, M_DEV0, DCF_NAME, M_DEFAULT, &Digitizer)) {
				cout << "M_DEV0 Digitizer Alloc Success!" << endl;
				AfxMessageBox(_T("M_DEV0 Digitizer Alloc Success!"));
				MdigInquire(Digitizer, M_SIZE_X, &SizeX1);
				MdigInquire(Digitizer, M_SIZE_Y, &SizeY1);
				MdigInquire(Digitizer, M_SIZE_BIT, &SizeBit1);
				MdigInquire(Digitizer, M_SIZE_BAND, &SizeBand1);
				
				DevNum = _T("DEV0");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);

				MdigControl(Digitizer, M_GRAB_TIMEOUT, M_INFINITE);
				BufX = SizeX1;
				BufY = SizeY1;
				cout << endl;
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV1");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV2");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV3");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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

		MIL_STRING genICamPath = MIL_TEXT("C:\Program Files\Matrox Imaging\Drivers\GenICam\V3_3\bin/Win64_x64#GenCP_MD_VC120_v3_0_MTX.dll#GenICam#GenCP_01_00");
		// CString genICamPath2 = _T("C:\Program Files\Matrox Imaging\Drivers\GenICam\V3_3\bin/Win64_x64#GenCP_MD_VC120_v3_0_MTX.dll#GenICam#GenCP_01_00");
		MdigInquire(Digitizer, M_GC_CLPROTOCOL_DEVICE_ID + 1, genICamPath);
		MdigControl(Digitizer, M_GC_CLPROTOCOL_DEVICE_ID, genICamPath);
		MdigControl(Digitizer, M_GC_CLPROTOCOL, M_ENABLE);

		int bitsPerChannel = (int)SizeBit1 / 8;

		if (SizeBit1 % 8 != 0) {
			bitsPerChannel++;
		}

		bitsPerChannel *= 8;

		SizeBit1 = bitsPerChannel;

		return Digitizer;
	}
	else if (boardselector == 3)
	{
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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV0");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV1");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV2");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
				cout << "[Camera - Info.]" << endl;
				cout << "  X : " << (int)SizeX1 << " Y : " << (int)SizeY1 << endl;
				cout << "  SizeBit : " << (int)SizeBit1 << endl;
				cout << "  SizeBand : " << (int)SizeBand1 << endl;
				
				DevNum = _T("DEV3");
				MdigInquireFeature(Digitizer, M_FEATURE_VALUE, MIL_TEXT("DeviceModelName"), M_TYPE_STRING, CamName);
				Digitizercnt++;

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
}

tuple<MIL_ID, MIL_ID*, MIL_ID, MIL_ID, MIL_INT, MIL_INT, MIL_INT, MIL_INT> CameraControl::BufnDispAlloc(MIL_ID Digitizer, MIL_ID* GrabBuf, MIL_ID DispBuf, MIL_ID Disp)
{
	if (Digitizercnt == 1) // Digitizer 1
	{
		BufX = SizeX1;
		BufY = SizeY1;
		BufBand = SizeBand1;
		BufBit = SizeBit1;

		MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &Disp);

		for (int i = 0; i < GRAB_NUM; i++)
		{
			MbufAllocColor(MilSystem, BufBand, BufX, BufY, BufBit + M_UNSIGNED, M_IMAGE + M_GRAB + M_DISP, GrabBuf + i);
			MbufClear(GrabBuf[i], 0);
		}
		MbufAllocColor(MilSystem, BufBand, BufX, BufY, BufBit + M_UNSIGNED, M_IMAGE + M_GRAB + M_DISP, &DispBuf);

		UserHookData.MilDigitizer1 = Digitizer;
		UserHookData.MilImage1 = GrabBuf;
		UserHookData.MilDispImage1 = DispBuf;
		UserHookData.ProcessedImageCount1 = 0;
		UserHookData.GrabDoneCAM1 = 0;

		cout << "  Camera 1 - Buffer & Display Alloc OK\n\n" << endl;
		return make_tuple(Digitizer, GrabBuf, DispBuf, Disp, BufBand, BufX, BufY, BufBit);
	}
	else if (Digitizercnt == 2) // Digitizer 2 
	{
		BufX2 = SizeX1;
		BufY2 = SizeY1;
		BufBand2 = SizeBand1;
		BufBit2 = SizeBit1;

		MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &Disp);

		for (int i = 0; i < GRAB_NUM; i++)
		{
			MbufAllocColor(MilSystem, BufBand2, BufX2, BufY2, BufBit2 + M_UNSIGNED, M_IMAGE + M_GRAB + M_DISP, GrabBuf + i);
			MbufClear(GrabBuf[i], 0);
		}
		MbufAllocColor(MilSystem, BufBand2, BufX2, BufY2, BufBit2 + M_UNSIGNED, M_IMAGE + M_GRAB + M_DISP, &DispBuf);

		UserHookData.MilDigitizer2 = Digitizer;
		UserHookData.MilImage2 = GrabBuf;
		UserHookData.MilDispImage2 = DispBuf;
		UserHookData.ProcessedImageCount2 = 0;
		UserHookData.GrabDoneCAM2 = 0;

		cout << "  Camera 2 - Buffer & Display Alloc OK\n" << endl;
		return make_tuple(Digitizer, GrabBuf, DispBuf, Disp, BufBand2, BufX2, BufY2, BufBit2);
	}
	else if (Digitizercnt == 3) // Digitizer 3
	{

	}
	else if (Digitizercnt == 4) // Digitizer 4
	{

	}

}

tuple<MIL_ID, MIL_ID, MIL_ID> CameraControl::otheralloc()
{
	if (Digitizercnt == 1) // Digitizer 1
	{
		MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilGrabDisplay2);
		MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilGrabDisplay3);
		MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilGrabDisplay4);

		return make_tuple(MilGrabDisplay2, MilGrabDisplay3, MilGrabDisplay4);
	}
	else if (Digitizercnt == 2) // Digitizer 2
	{
		MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilGrabDisplay22);
		MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilGrabDisplay23);
		MdispAlloc(MilSystem, M_DEFAULT, MIL_TEXT("M_DEFAULT"), M_DEFAULT, &MilGrabDisplay24);

		return make_tuple(MilGrabDisplay22, MilGrabDisplay23, MilGrabDisplay24);
	}
	else if (Digitizercnt == 3) // Digitizer 3
	{

	}
	else if (Digitizercnt == 4) // Digitizer 4
	{

	}
}

void CameraControl::GrabProcess(MIL_ID Digitizer, MIL_ID *GrabBuf, MIL_ID DispBuf, MIL_ID Disp)
{
	UserHookData.ProcessedImageCount1 = 0;
	UserHookData.GrabDoneCAM1 = 0;

	MdispControl(Disp, M_TITLE, MIL_TEXT("C.Grab Display"));
	MdispSelect(Disp, DispBuf);
	MdispHookFunction(Disp, M_MOUSE_MOVE, MouseFct, (void*)this);
	// MdigHookFunction(Digitizer, M_GRAB_FRAME_START, FrameHandler_1, &UserHookData);
	
	BufCtrl_cpp = BufCtrl;
	*min_cpp = min;
	*max_cpp = max;

	MyPoint.x = 100;
	MyPoint.y = 100;
	cout << BufX << " " << BufY << endl;

	MdigProcess(Digitizer, GrabBuf, GRAB_NUM, M_START, M_ASYNCHRONOUS, ProcessingFunction1, &UserHookData);

	while (1)
	{
		BufCtrl_cpp = BufCtrl;
		*min_cpp = min; // Thres Min
		*max_cpp = max; // Thres Max

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
	Mat Temp_Blur;
	Mat Temp_Gap;
	string min_int = to_string(int(*min_cpp));
	string max_int = to_string(int(*max_cpp));
	string cor_x = to_string(int(m_DisplayPositionX));
	string cor_y = to_string(int(m_DisplayPositionY));
	string MyText;
	int v;
	string sval;

	Temp.create(BufY, BufX, CV_8UC3);

	MIL_ID ModifiedBufferIdCAM1;

	MdigGetHookInfo(HookId, M_MODIFIED_BUFFER + M_BUFFER_ID, &ModifiedBufferIdCAM1);

	MbufGetColor2d(ModifiedBufferIdCAM1, M_PACKED + M_BGR24, M_ALL_BANDS, 0, 0, BufX, BufY, (void*)Temp.data);

	switch (BufCtrl_cpp)
	{
	case 0:
		// cvtColor(Temp, Temp, COLOR_BGR2GRAY);

		for (int i = 0; i < 70; i++)
		{
			for (int j = 0; j < 2300; j++)
			{
				Temp.at<uchar>(i, j) = 0;
			}
		}

		if (disp_x >= 0 && disp_y >= 0 && disp_x <= Temp.cols && disp_y <= Temp.rows)
		{
			v = (int)Temp.at<uchar>(disp_y, disp_x);
			sval = to_string(v);
			MyText = ("X : " + cor_x + " / Y : " + cor_y + " / " + sval);
		}
		else {
			MyText = ("X : - / Y : -  / - ");
		}
		
		putText(Temp, MyText, Point(30, 50), 1, 3.0, Scalar(255, 255, 255), 3, 8);
		MbufPutColor2d(UserHookDataPtr->MilDispImage1, M_PACKED + M_BGR24, M_ALL_BANDS, 0, 0, BufX, BufY, (void*)Temp.data);

		break;
	case 1: // Canny Edge Detection

		cvtColor(Temp, Temp, COLOR_BGR2GRAY);
		blur(Temp, Temp_Blur, Size(5, 5));
		Temp_Gap = Temp - Temp_Blur;
		Temp = Temp + Temp_Gap;
		Canny(Temp, Temp, *min_cpp, *max_cpp);
		MyText = ("Thres. Min : " + min_int + " / Max : " + max_int);
		putText(Temp, MyText, Point(30, 50), 1, 3.0, Scalar::all(255), 3, 8);

		MbufPut2d(UserHookDataPtr->MilDispImage1, 0, 0, BufX, BufY, (void*)Temp.data);

		break;
	case 2:
		break;
	case 3:
		break;
	}

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
		Temp_Mat = imread("./Grab_Result.bmp", 0);
		cout << "Save OK " << endl;
	}
	return 0;
}

MIL_INT MFTYPE MouseFct(MIL_INT HookType, MIL_ID EventID, void* UserDataPtr)
{
	MdispGetHookInfo(EventID, M_MOUSE_POSITION_BUFFER_X, &m_DisplayPositionX);
	MdispGetHookInfo(EventID, M_MOUSE_POSITION_BUFFER_Y, &m_DisplayPositionY);

	disp_x = m_DisplayPositionX;
	disp_y = m_DisplayPositionY;

	return 0;
}

MIL_INT MFTYPE MouseFct_Line(MIL_INT HookType, MIL_ID EventID, void* UserDataPtr)
{
	MdispGetHookInfo(EventID, M_MOUSE_POSITION_BUFFER_X, &m_DisplayPositionX);
	MdispGetHookInfo(EventID, M_MOUSE_POSITION_BUFFER_Y, &m_DisplayPositionY);

	disp_x = m_DisplayPositionX;
	disp_y = m_DisplayPositionY;

	// cout << disp_x << " " << disp_y << endl;

	if (disp_x >= 0 && disp_y >= 0 && disp_x <= Temp_Mat.cols && disp_y <= Temp_Mat.rows)
	{
		int v = (int)Temp_Mat.at<uchar>(disp_y, disp_x);

		cout << "X : " << disp_x << " Y : " << disp_y << " / " << v << endl;
	}
	else {
		cout << "X : - / Y : -  / - " << endl;
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
	MdispHookFunction(LineDisp_Test, M_MOUSE_MOVE, MouseFct_Line, (void*)this);
	MbufAlloc2d(MilSystem_backup, SizeX1, SizeY1 * (*Total_Frame_Num_cpp), SizeBit1 + M_UNSIGNED, M_IMAGE + M_GRAB + M_DISP, &BufParent_Test);

	return LineDisp_Test;
}

void CameraControl::DisplayAlloc_Free(MIL_ID Disp)
{
	MbufFree(BufParent_Test);
	MdispFree(Disp);
	// cout << "Memory Free OK " << endl;
}