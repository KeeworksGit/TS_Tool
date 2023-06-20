#pragma once
#include "afxdialogex.h"


// DigitCtrlDlg 대화 상자

class DigitCtrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DigitCtrlDlg)

public:
	DigitCtrlDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~DigitCtrlDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAM_CONTROL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonCgrab();
	afx_msg void OnBnClickedButtonCgrabstop();
	afx_msg void OnBnClickedButton1scrab();
	afx_msg void OnBnClickedButton1ssave();
	afx_msg void OnBnClickedButtonCamset();
	CButton m_btn_cgrab1;
	CButton m_btn_cgrabstop1;
	CButton m_btn_1shot1;
	CButton m_btn_save1;
	CButton m_btn_feature1;
	CEdit m_edit_fr;
	CComboBox m_list_devcpy;
	afx_msg void OnCbnSelchangeComboDevcpy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnPaint();
	HBITMAP m_hBitmap;
	BITMAP m_bitmap;
	void MoveToParentCenter(HWND hWnd);
	CEdit m_edti_frnum;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_btn_edge;
	afx_msg void OnBnClickedButtonEdge();
	CListCtrl m_list_device;
	CTreeCtrl m_tree_dev;
	void AddDevice(CString Dev);
	void AddCam(CString Cam);
};
