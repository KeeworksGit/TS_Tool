// DigitCtrlDlg.cpp: 구현 파일
//

#include "pch.h"
#include "KeeworksTS.h"
#include "afxdialogex.h"
#include "DigitCtrlDlg.h"
#include "KeeworksTSDlg.h"
#include "CameraControl.h"
#include "EdgeControlDlg.h"

// DigitCtrlDlg 대화 상자

Mat* BufAd;
CString sDriveCAM;
CString filePath;
CTime file_time;
CString strTime;
CString file_format = _T(".bmp"); // DEFAULT VALUEW
IMPLEMENT_DYNAMIC(DigitCtrlDlg, CDialogEx)

MIL_ID MilGrabDisplay2;
MIL_ID MilGrabDisplay3;
MIL_ID MilGrabDisplay4;

bool DispAlloc_check = false;
MIL_ID LineDisplay;

HTREEITEM DEV0, CAM0; // Device List

EdgeControlDlg* EdgeCtrl;

DigitCtrlDlg::DigitCtrlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CAM_CONTROL, pParent)
{

}

DigitCtrlDlg::~DigitCtrlDlg()
{
}

void DigitCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CGRAB, m_btn_cgrab1);
	DDX_Control(pDX, IDC_BUTTON_CGRABSTOP, m_btn_cgrabstop1);
	DDX_Control(pDX, IDC_BUTTON_1SCRAB, m_btn_1shot1);
	DDX_Control(pDX, IDC_BUTTON_1SSAVE, m_btn_save1);
	DDX_Control(pDX, IDC_BUTTON_CAMSET, m_btn_feature1);
	DDX_Control(pDX, IDC_COMBO_DEVCPY, m_list_devcpy);
	DDX_Control(pDX, IDC_EDIT_FRNUM, m_edti_frnum);
	DDX_Control(pDX, IDC_BUTTON_EDGE, m_btn_edge);
	DDX_Control(pDX, IDC_TREE_DEVICE, m_tree_dev);
}


BEGIN_MESSAGE_MAP(DigitCtrlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CGRAB, &DigitCtrlDlg::OnBnClickedButtonCgrab)
	ON_BN_CLICKED(IDC_BUTTON_CGRABSTOP, &DigitCtrlDlg::OnBnClickedButtonCgrabstop)
	ON_BN_CLICKED(IDC_BUTTON_1SCRAB, &DigitCtrlDlg::OnBnClickedButton1scrab)
	ON_BN_CLICKED(IDC_BUTTON_1SSAVE, &DigitCtrlDlg::OnBnClickedButton1ssave)
	ON_BN_CLICKED(IDC_BUTTON_CAMSET, &DigitCtrlDlg::OnBnClickedButtonCamset)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVCPY, &DigitCtrlDlg::OnCbnSelchangeComboDevcpy)
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_EDGE, &DigitCtrlDlg::OnBnClickedButtonEdge)
END_MESSAGE_MAP()

// DigitCtrlDlg 메시지 처리기

BOOL DigitCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SetWindowText(_T("Digitizer 1 Control"));

	CRect rc;
	rc.SetRect(0, 0, 500, 340);
	MoveWindow(rc);

	m_btn_save1.EnableWindow(false);
	m_btn_cgrabstop1.EnableWindow(false);

	m_list_devcpy.InsertString(0, _T("1 Disp Add"));
	m_list_devcpy.InsertString(1, _T("2 Disp Add"));
	m_list_devcpy.InsertString(2, _T("3 Disp Add"));

	SetDlgItemText(IDC_EDIT_FRNUM, _T("1"));

	GetDlgItem(IDC_BUTTON_CAMSET)->MoveWindow(204, 36, 84, 25); // Feature Browser
	GetDlgItem(IDC_BUTTON_CGRAB)->MoveWindow(14, 72, 84, 25); // Feature Browser
	GetDlgItem(IDC_BUTTON_CGRABSTOP)->MoveWindow(103, 72, 84, 25); // Feature Browser
	GetDlgItem(IDC_COMBO_DEVCPY)->MoveWindow(192, 72, 96, 27); // Feature Browser
	GetDlgItem(IDC_BUTTON_1SCRAB)->MoveWindow(14, 145, 84, 25); // Feature Browser
	GetDlgItem(IDC_BUTTON_1SSAVE)->MoveWindow(103, 145, 84, 25); // Feature Browser
	GetDlgItem(IDC_EDIT_FRNUM)->MoveWindow(192, 145, 96, 25); // Feature Browser
	GetDlgItem(IDC_BUTTON_EDGE)->MoveWindow(14, 218, 84, 25); // Feature Browser
	GetDlgItem(IDC_TREE_DEVICE)->MoveWindow(298, 72, 175, 212); // Device List

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void DigitCtrlDlg::OnBnClickedButtonCgrab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// Tree 개수 Count 
	int cnt = 0; 
	HTREEITEM Hcnt = m_tree_dev.GetChildItem(TVI_ROOT);
	while (Hcnt){
		cnt++;
		Hcnt = m_tree_dev.GetNextSiblingItem(Hcnt);
	}

	// Index Inquire
	int index = 0;
	HTREEITEM hItem = m_tree_dev.GetSelectedItem();
	HTREEITEM hChild = m_tree_dev.GetParentItem(hItem);
	while (hChild)
	{
		if (hChild == hItem) break;
		hChild = m_tree_dev.GetNextItem(hChild, TVGN_NEXT);
		++index;
	}

	cout << cnt - index << endl;

	if (cnt - index == 0) { // Digitizer 1 C.Grab 

		m_btn_cgrab1.EnableWindow(false);
		m_btn_1shot1.EnableWindow(false);
		m_btn_cgrabstop1.EnableWindow(true);
		
		(((CameraControl*)GetParent())->BufCtrl) = 0;
		cout << "Dig ID : " << ((CKeeworksTSDlg*)GetParent())->DigID_1 << " / Grab Buf Address : " << ((CKeeworksTSDlg*)GetParent())->GrabBuf << endl << endl;
		cout << "MdigProcess (Grab Infinite) Start" << endl << endl;
		((CameraControl*)GetParent())->GrabProcess(((CKeeworksTSDlg*)GetParent())->DigID_1, ((CKeeworksTSDlg*)GetParent())->GrabBuf, ((CKeeworksTSDlg*)GetParent())->DispBuf, ((CKeeworksTSDlg*)GetParent())->Disp); 
	}
	else if (cnt - index == 1) { // Digitizer 2 C.Grab 

	}
	else if (cnt - index == 2) { // Digitizer 3 C.Grab 

	}
	else if (cnt - index == 3) { // Digitizer 4 C.Grb 

	}
	else {
		AfxMessageBox(_T("Warning :: Select a device"));
	}
}

void DigitCtrlDlg::OnBnClickedButtonCgrabstop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_btn_cgrab1.EnableWindow(true);
	m_btn_1shot1.EnableWindow(true);
	m_btn_cgrabstop1.EnableWindow(false);

	((CameraControl*)GetParent())->GrabStop();
}

void DigitCtrlDlg::OnBnClickedButton1scrab()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (((CKeeworksTSDlg*)GetParent())->DeviceType == 1) // Area
	{
		BufAd = ((CameraControl*)GetParent())->ShotGrab(((CKeeworksTSDlg*)GetParent())->DigID_1, ((CKeeworksTSDlg*)GetParent())->Disp, ((CKeeworksTSDlg*)GetParent())->DispBuf, ((CKeeworksTSDlg*)GetParent())->SizeX, ((CKeeworksTSDlg*)GetParent())->SizeY);
		cout << "Dig ID : " << ((CKeeworksTSDlg*)GetParent())->DigID_1 << " / Grab Buf Address : " << ((CKeeworksTSDlg*)GetParent())->GrabBuf << endl << endl;

		m_btn_save1.EnableWindow(true);
	}
	else if (((CKeeworksTSDlg*)GetParent())->DeviceType == 2) // Line
	{
		CString frnum;
		GetDlgItemText(IDC_EDIT_FRNUM, frnum);
		int a = _ttoi(frnum);

		(((CameraControl*)GetParent())->TotalFrameNum) = a;

		if (LineDisplay != 0) // Display 가 없을 때 Display & Buffer Alloc
		{
			((CameraControl*)GetParent())->DisplayAlloc_Free(LineDisplay);
		}

		LineDisplay = (((CameraControl*)GetParent())->DisplayAlloc(((CKeeworksTSDlg*)GetParent())->DigID_1)); // Display ID GET

		((CameraControl*)GetParent())->GrabLine_Alloc(((CKeeworksTSDlg*)GetParent())->DigID_1, ((CKeeworksTSDlg*)GetParent())->GrabBuf);
		((CameraControl*)GetParent())->GrabProcess_Line2(((CKeeworksTSDlg*)GetParent())->DigID_1, ((CKeeworksTSDlg*)GetParent())->GrabBuf);
	}
}

void DigitCtrlDlg::OnBnClickedButton1ssave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_btn_save1.EnableWindow(false);

	Mat temp = *BufAd;

	CString strInitPath = _T("C:\\");
	CString strDailyFolderPath;
	CString strOriFolderPath;
	CFolderPickerDialog Picker(strInitPath, OFN_FILEMUSTEXIST, NULL, 0);

	if (Picker.DoModal() == IDOK)
	{
		strDailyFolderPath = Picker.GetPathName();
		strOriFolderPath = Picker.GetPathName();

		sDriveCAM = strDailyFolderPath;
		filePath = sDriveCAM + _T("\\");
		file_time = CTime::GetCurrentTime();
		strTime = file_time.Format(_T("%Y%m%d_%H%M%S"));

		imwrite(string(CT2CA(filePath + strTime + file_format)), temp);
		cout << "FilePath : " << string(CT2CA(filePath)) << " - Save OK" << endl;
	}

	// cout << "Pop Up End" << endl;
}

void DigitCtrlDlg::OnBnClickedButtonCamset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.	
	((CameraControl*)GetParent())->FeatureBrowser(((CKeeworksTSDlg*)GetParent())->DigID_1);
}

void DigitCtrlDlg::OnCbnSelchangeComboDevcpy()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iSel = m_list_devcpy.GetCurSel();

	if (iSel == 0) {
		((CKeeworksTSDlg*)GetParent())->Opendisplay2();

	}
	else if (iSel == 1) { 
		((CKeeworksTSDlg*)GetParent())->Opendisplay3();
	}
	else if (iSel == 2) {
		((CKeeworksTSDlg*)GetParent())->Opendisplay4();
	}
}

void DigitCtrlDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void DigitCtrlDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

	CRect rect;
	GetClientRect(&rect);

	int cx = rect.Width();
	int cy = rect.Height();

	CBitmap resBack;
	CBitmap* old_resBack;
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	resBack.LoadBitmapW(IDB_BMP_BACK2);
	old_resBack = memDC.SelectObject(&resBack);

	dc.BitBlt(0, 0, cx, cy, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(old_resBack);
	resBack.DeleteObject();
	memDC.DeleteDC();
}

void DigitCtrlDlg::MoveToParentCenter(HWND hWnd)
{
	RECT    sttParentRect, sttClientRect;
	HWND    hParent;

	hParent = ::GetParent(hWnd);
	if (hParent == NULL) return;

	if (::IsIconic(hWnd))
	{
		::ShowWindow(hParent, SW_RESTORE);
	}

	::GetWindowRect(hParent, &sttParentRect);
	::GetWindowRect(hWnd, &sttClientRect);
	::SetWindowPos(hWnd, NULL
		, sttParentRect.left + (sttParentRect.right - sttParentRect.left) / 2 - (sttClientRect.right - sttClientRect.left) / 2
		, sttParentRect.top + (sttParentRect.bottom - sttParentRect.top) / 2 - (sttClientRect.bottom - sttClientRect.top) / 2
		, 0
		, 0
		, SWP_NOSIZE | SWP_NOZORDER);
}

BOOL DigitCtrlDlg::PreTranslateMessage(MSG* pMsg)
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

void DigitCtrlDlg::OnBnClickedButtonEdge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	EdgeCtrl = new EdgeControlDlg;
	EdgeCtrl->Create(IDD_EDGE_CONTROL);
	EdgeCtrl->CenterWindow(CWnd::GetDesktopWindow());
	EdgeCtrl->ShowWindow(SW_SHOW);
}

void DigitCtrlDlg::AddDevice(CString Dev)
{
	DEV0 = m_tree_dev.InsertItem(Dev, 0, 0, TVI_ROOT, TVI_LAST);
}

void DigitCtrlDlg::AddCam(CString Cam)
{
	CAM0 = m_tree_dev.InsertItem(Cam, 0, 0, DEV0, TVI_LAST);
}