// EdgeControlDlg.cpp: 구현 파일
//

#include "pch.h"
#include "KeeworksTS.h"
#include "afxdialogex.h"
#include "EdgeControlDlg.h"
#include "KeeworksTSDlg.h"
#include "CameraControl.h"


// EdgeControlDlg 대화 상자

IMPLEMENT_DYNAMIC(EdgeControlDlg, CDialogEx)

EdgeControlDlg::EdgeControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDGE_CONTROL, pParent)
{

}

EdgeControlDlg::~EdgeControlDlg()
{
}

void EdgeControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ON, m_btn_on);
	DDX_Control(pDX, IDC_BUTTON_OFF, m_btn_off);
	DDX_Control(pDX, IDC_EDIT_MINVAL, m_edit_minval);
	DDX_Control(pDX, IDC_EDIT_MAXVAL, m_edit_maxval);

}


BEGIN_MESSAGE_MAP(EdgeControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ON, &EdgeControlDlg::OnBnClickedButtonOn)
	ON_BN_CLICKED(IDC_BUTTON_OFF, &EdgeControlDlg::OnBnClickedButtonOff)
	ON_EN_CHANGE(IDC_EDIT_MINVAL, &EdgeControlDlg::OnEnChangeEditMinval)
	ON_EN_CHANGE(IDC_EDIT_MAXVAL, &EdgeControlDlg::OnEnChangeEditMaxval)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// EdgeControlDlg 메시지 처리기
BOOL EdgeControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowText(_T("EdgeFinder Control"));

	// SetDlgItemText(IDC_EDIT_FRNUM, _T("1"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void EdgeControlDlg::OnBnClickedButtonOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	(((CameraControl*)GetParent())->BufCtrl) = 1;
}

void EdgeControlDlg::OnBnClickedButtonOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	(((CameraControl*)GetParent())->BufCtrl) = 0;
}

void EdgeControlDlg::OnEnChangeEditMinval()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Cmin;
	
	if (UpdateData(true))
	{
		GetDlgItemText(IDC_EDIT_MINVAL, Cmin);
		int a = _ttoi(Cmin);

		if (a <= 255 && a >= 0)
		{
			(((CameraControl*)GetParent())->min) = a;
			// cout << "Set Min Value : " << a << endl;
		}
		else
		{
			cout << "Over 0 & Under 255 Value!" << endl;
			(((CameraControl*)GetParent())->min) = 0;
			cout << "Set Min Value : 0" << endl;
		}

	}

}

void EdgeControlDlg::OnEnChangeEditMaxval()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString Cmax;

	if (UpdateData(true))
	{
		GetDlgItemText(IDC_EDIT_MAXVAL, Cmax);
		int b = _ttoi(Cmax);

		if (b <= 255 && b >= 0)
		{
			(((CameraControl*)GetParent())->max) = b;
			// cout << "Set Min Value : " << b << endl;
		}
		else
		{
			cout << "Over 0 & Under 255 Value!" << endl;
			(((CameraControl*)GetParent())->max) = 255;
			cout << "Set Min Value : 255" << endl;
		}
	}
}

BOOL EdgeControlDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			// ESC 키 이벤트에 대한 처리 추가
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			// Enter 키 이벤트에 대한 처리 추가
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void EdgeControlDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
