#pragma once
#include "afxdialogex.h"


// EdgeControlDlg 대화 상자

class EdgeControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EdgeControlDlg)

public:
	EdgeControlDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~EdgeControlDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDGE_CONTROL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_btn_on;
	CButton m_btn_off;
	afx_msg void OnBnClickedButtonOn();
	afx_msg void OnBnClickedButtonOff();
	CEdit m_edit_minval;
	CEdit m_edit_maxval;
	afx_msg void OnEnChangeEditMinval();
	afx_msg void OnEnChangeEditMaxval();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
