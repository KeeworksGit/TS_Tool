
// KeeworksTSDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "KeeworksTS.h"
#include "KeeworksTSDlg.h"
#include "afxdialogex.h"

#include "CameraControl.h"
#include "DigitCtrlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

using namespace std;
using namespace cv;

// CameraControl CC;
DigitCtrlDlg* CamCtrl;


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKeeworksTSDlg 대화 상자
CKeeworksTSDlg::CKeeworksTSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_KEEWORKSTS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeeworksTSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BOARD, m_list_board);
	DDX_Control(pDX, IDC_BTN_BALLOC, m_btn_boardalloc);
	DDX_Control(pDX, IDC_BTN_DCF, m_btn_dcf1);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_btn_dcfopen);
	DDX_Control(pDX, IDC_BUTTON_FREE, m_btn_dcffree);
	DDX_Control(pDX, IDC_BUTTON_OPEN2, m_btn_dcfopen2);
	DDX_Control(pDX, IDC_BUTTON_FREE2, m_btn_dcffree2);
	DDX_Control(pDX, IDC_BTN_DCF2, m_btn_dcf2);
}

BEGIN_MESSAGE_MAP(CKeeworksTSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BTN_BALLOC, &CKeeworksTSDlg::OnBnClickedBtnBalloc)
	ON_CBN_SELCHANGE(IDC_COMBO_BOARD, &CKeeworksTSDlg::OnCbnSelchangeComboBoard)
	ON_BN_CLICKED(IDC_BTN_DCF, &CKeeworksTSDlg::OnBnClickedBtnDcf)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CKeeworksTSDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_FREE, &CKeeworksTSDlg::OnBnClickedButtonFree)
	ON_BN_CLICKED(IDC_BTN_DCF2, &CKeeworksTSDlg::OnBnClickedBtnDcf2)
	ON_BN_CLICKED(IDC_BUTTON_OPEN2, &CKeeworksTSDlg::OnBnClickedButtonOpen2)
END_MESSAGE_MAP()


// CKeeworksTSDlg 메시지 처리기

BOOL CKeeworksTSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowText(_T("TS_Solution_Tool_Keeworks"));

	m_list_board.InsertString(0, _T("Rapixo CXP"));
	m_list_board.InsertString(1, _T("Solios CL"));
	m_list_board.InsertString(2, _T("Radient EV_CL"));
	m_list_board.InsertString(3, _T("GigE Vision"));
	
	GetDlgItem(IDC_COMBO_BOARD)->MoveWindow(20, 66, 206, 23); // Board Selec
	GetDlgItem(IDC_BTN_BALLOC)->MoveWindow(237, 66, 103, 23); // Board Alloc

	GetDlgItem(IDC_STATIC_DCF)->MoveWindow(20, 170, 331, 33); // DCF1
	GetDlgItem(IDC_STATIC_DCF2)->MoveWindow(20, 253, 331, 33); // DCF1
	GetDlgItem(IDC_BTN_DCF)->MoveWindow(359, 170, 103, 33); // Set
	GetDlgItem(IDC_BTN_DCF2)->MoveWindow(359, 253, 103, 33); // Set
	GetDlgItem(IDC_BUTTON_OPEN)->MoveWindow(469, 170, 103, 33); // open
	GetDlgItem(IDC_BUTTON_OPEN2)->MoveWindow(469, 253, 103, 33); // open
	GetDlgItem(IDC_BUTTON_FREE)->MoveWindow(579, 170, 103, 33); // Free
	GetDlgItem(IDC_BUTTON_FREE2)->MoveWindow(579, 253, 103, 33); // Free

	// 상하 여백 + - 5
	CClientDC dc(GetDlgItem(IDC_STATIC_DCF));
	CRect rt; GetDlgItem(IDC_STATIC_DCF)->GetClientRect(&rt);
	rt.top += 5;
	rt.bottom -= 5;
	((CEdit*)GetDlgItem(IDC_STATIC_DCF))->SetRect(&rt);

	CClientDC dc2(GetDlgItem(IDC_STATIC_DCF2));
	CRect rt2; GetDlgItem(IDC_STATIC_DCF2)->GetClientRect(&rt2);
	rt2.top += 5;
	rt2.bottom -= 5;
	((CEdit*)GetDlgItem(IDC_STATIC_DCF2))->SetRect(&rt2);


	m_btn_dcfopen.EnableWindow(false);
	m_btn_dcffree.EnableWindow(false);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CKeeworksTSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CKeeworksTSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CPaintDC dc(this);

		CDC MemDC;
		CBitmap bmp;

		CRect rct;
		this->GetClientRect(&rct);

		MemDC.CreateCompatibleDC(&dc);
		bmp.LoadBitmap(IDB_BMP_BACK);
		MemDC.SelectObject(&bmp);

		dc.BitBlt(0, 0, rct.Width(), rct.Height(), &MemDC, 0, 0, SRCCOPY);

	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CKeeworksTSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKeeworksTSDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize = CPoint(715, 635);
	lpMMI->ptMaxTrackSize = CPoint(715, 635);

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CKeeworksTSDlg::OnBnClickedBtnBalloc()
{
	//((CameraControl*)GetParent())->BoardAlloc();
	CC->BoardAlloc();
}

void CKeeworksTSDlg::OnCbnSelchangeComboBoard()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iSel = m_list_board.GetCurSel();

	if (iSel == 0) {
		CC->boardselector = 0;
	}
	else if (iSel == 1) {
		CC->boardselector = 1;
	}
	else if (iSel == 2) {
		CC->boardselector = 2;
	}
	else if (iSel == 3) {
		CC->boardselector = 3;
	}
}

void CKeeworksTSDlg::OnBnClickedBtnDcf()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str = _T("dcf 파일 (*.dcf) | *.dcf|"); // 모든 파일 표시
	// _T("Excel 파일 (*.xls, *.xlsx) |*.xls; *.xlsx|"); 와 같이 확장자를 제한하여 표시할 수 있음
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK)
	{
		CC->DCF_Name_1 = dlg.GetPathName();
		CString Filename = dlg.GetFileName();
		// 파일 경로를 가져와 사용할 경우, Edit Control에 값 저장
		SetDlgItemText(IDC_STATIC_DCF, _T(" ") + Filename);
	}
	// cout << string(CT2CA(CC->DCF_Name_1)) << endl;

	m_btn_dcfopen.EnableWindow(true);
}

void CKeeworksTSDlg::OnBnClickedBtnDcf2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str = _T("dcf 파일 (*.dcf) | *.dcf|"); // 모든 파일 표시
	// _T("Excel 파일 (*.xls, *.xlsx) |*.xls; *.xlsx|"); 와 같이 확장자를 제한하여 표시할 수 있음
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK)
	{
		CC->DCF_Name_2 = dlg.GetPathName();
		CString Filename = dlg.GetFileName();
		// 파일 경로를 가져와 사용할 경우, Edit Control에 값 저장
		SetDlgItemText(IDC_STATIC_DCF2, _T(" ") + Filename);
	}
	// cout << string(CT2CA(CC->DCF_Name_2)) << endl;

	m_btn_dcfopen2.EnableWindow(true);
}

void CKeeworksTSDlg::OnBnClickedButtonOpen() // DCF 1 Open
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CamCtrl = new DigitCtrlDlg;

	try {
		DigID_1 = CC->DigitAlloc(CC->MilSystem, CC->DCF_Name_1, CC->MilDigitizer1);

		// MdigAlloc(CC->MilSystem, M_DEV1, CC->DCF_Name_1, M_DEFAULT, &CC->MilDigitizer1);

		MIL_STRING DeviceScanType;
		CString DevType;
		MdigInquireFeature(DigID_1, M_FEATURE_VALUE, MIL_TEXT("DeviceScanType"), M_TYPE_STRING, DeviceScanType);
		DevType = DeviceScanType.c_str();

		cout << "  Device Type : " << string(CT2CA(DeviceScanType.c_str())) << endl;

		if (DevType.CompareNoCase(_T("Areascan")) == 0) // AreaScan 
		{
			cout << "  This is AreaScan Camera!" << endl;
			DeviceType = 1;
			MdigInquire(DigID_1, M_SELECTED_FRAME_RATE, &FrameRate);
			cout << "  FrameRate : " << (double)FrameRate << endl;
		}
		else if (DevType.CompareNoCase(_T("LineScan")) == 0)
		{
			cout << "  This is LineScan Camera!" << endl;
			DeviceType = 2;
			MdigInquireFeature(DigID_1, M_FEATURE_VALUE, MIL_TEXT("AcquisitionLineRate"), M_TYPE_DOUBLE, &AcquisitionLineRate);
			cout << "  LineRate : " << (double)AcquisitionLineRate << endl;
		}

		Result1 = CC->BufnDispAlloc(DigID_1, CC->MilImage1, CC->MilDispImage1, CC->MilGrabDisplay1);
		BufferID = CC->otheralloc();

		GrabBuf = get<1>(Result1); // GrabBUf
		DispBuf = get<2>(Result1); // DispBuf
		Disp = get<3>(Result1);	// Disp ID
		SizeBand = get<4>(Result1);	// Disp ID
		SizeX = get<5>(Result1);	// Disp ID
		SizeY = get<6>(Result1);	// Disp ID
		SizeBit = get<7>(Result1);	// Disp ID
		
		Disp2 = get<0>(BufferID);
		Disp3 = get<1>(BufferID);
		Disp4 = get<2>(BufferID);

		//cout << "SizeBand : " << (int)SizeBand << endl;
		//cout << "SizeX : " << (int)SizeX << endl;
		//cout << "SizeY : " << (int)SizeY << endl;
		//cout << "SizeBit : " << (int)SizeBit << endl;


		CamCtrl->Create(IDD_CAM_CONTROL);
		CamCtrl->CenterWindow(CWnd::GetDesktopWindow());
		CamCtrl->ShowWindow(SW_SHOW);

		// Dev List 갱신
		CamCtrl->AddDevice(CC->DevNum);
		CString CCamName = (CC->CamName.c_str());
		CamCtrl->AddCam(CCamName);

		m_btn_dcfopen2.EnableWindow(false);
		m_btn_dcffree2.EnableWindow(true);

	}
	catch (string error)
	{
		cout << error << endl;
	}
	//pDlg->DestroyWindow( m);
	//pDlg = NULL;
}

void CKeeworksTSDlg::OnBnClickedButtonFree()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CC->DigitFree(DigID_1, DispBuf, Disp, GrabBuf, Disp2, Disp3, Disp4);

	delete CamCtrl;

	m_btn_dcffree.EnableWindow(false);
}

void CKeeworksTSDlg::Opendisplay2()
{
	MdispSelect(Disp2, DispBuf);
}

void CKeeworksTSDlg::Opendisplay3()
{
	MdispSelect(Disp2, DispBuf);
	MdispSelect(Disp3, DispBuf);
}

void CKeeworksTSDlg::Opendisplay4()
{
	MdispSelect(Disp2, DispBuf);
	MdispSelect(Disp3, DispBuf);
	MdispSelect(Disp4, DispBuf);
}

BOOL CKeeworksTSDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CKeeworksTSDlg::OnBnClickedButtonOpen2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	try {
		DigID_2 = CC->DigitAlloc(CC->MilSystem, CC->DCF_Name_2, CC->MilDigitizer2);

		MIL_STRING DeviceScanType;
		CString DevType;
		MdigInquireFeature(DigID_2, M_FEATURE_VALUE, MIL_TEXT("DeviceScanType"), M_TYPE_STRING, DeviceScanType);
		DevType = DeviceScanType.c_str();

		cout << "  Device Type : " << string(CT2CA(DeviceScanType.c_str())) << endl;

		if (DevType.CompareNoCase(_T("Areascan")) == 0) // AreaScan 
		{
			cout << "  This is AreaScan Camera!" << endl;
			DeviceType = 1;
			MdigInquire(DigID_2, M_SELECTED_FRAME_RATE, &FrameRate);
			cout << "  FrameRate : " << (double)FrameRate << endl;
		}
		else if (DevType.CompareNoCase(_T("LineScan")) == 0)
		{
			cout << "  This is LineScan Camera!" << endl;
			DeviceType = 2;
			MdigInquireFeature(DigID_2, M_FEATURE_VALUE, MIL_TEXT("AcquisitionLineRate"), M_TYPE_DOUBLE, &AcquisitionLineRate);
			cout << "  LineRate : " << (double)AcquisitionLineRate << endl;
		}

		Result2 = CC->BufnDispAlloc(DigID_2, CC->MilImage2, CC->MilDispImage2, CC->MilGrabDisplay21);
		BufferID2 = CC->otheralloc();

		GrabBuf2 = get<1>(Result1); // GrabBUf
		DispBuf2 = get<2>(Result1); // DispBuf
		Disp21 = get<3>(Result1);	// Disp ID
		SizeBand2 = get<4>(Result1);	// Disp ID
		SizeX2 = get<5>(Result1);	// Disp ID
		SizeY2 = get<6>(Result1);	// Disp ID
		SizeBit2 = get<7>(Result1);	// Disp ID

		Disp22 = get<0>(BufferID2);
		Disp23 = get<1>(BufferID2);
		Disp24 = get<2>(BufferID2);

		//cout << "SizeBand : " << (int)SizeBand << endl;
		//cout << "SizeX : " << (int)SizeX << endl;
		//cout << "SizeY : " << (int)SizeY << endl;
		//cout << "SizeBit : " << (int)SizeBit << endl;

		// CamCtrl->Create(IDD_CAM_CONTROL);
		// CamCtrl->CenterWindow(CWnd::GetDesktopWindow());
		// CamCtrl->ShowWindow(SW_SHOW);

		// Dev List 갱신
		CamCtrl->AddDevice(CC->DevNum);
		CString CCamName = (CC->CamName.c_str());
		CamCtrl->AddCam(CCamName);

		CamCtrl->AddDevice(_T("DEV5"));
		CCamName = (CC->CamName.c_str());
		CamCtrl->AddCam(CCamName);

		m_btn_dcfopen.EnableWindow(false);
		m_btn_dcffree.EnableWindow(true);

	}
	catch (string error)
	{
		cout << error << endl;
	}

}
