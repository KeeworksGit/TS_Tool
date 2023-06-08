﻿
// KeeworksTSDlg.h: 헤더 파일
//
#include "CameraControl.h"

#pragma once

// CKeeworksTSDlg 대화 상자
class CKeeworksTSDlg : public CDialogEx
{
// 생성입니다.
public:
	CKeeworksTSDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	MIL_ID DigID_1;
	MIL_ID DigID_2;
	MIL_ID DigID_3;
	MIL_ID DigID_4;
	MIL_ID* GrabBuf;
	MIL_ID DispBuf;

	MIL_ID Disp;
	MIL_ID Disp2;
	MIL_ID Disp3;
	MIL_ID Disp4;

	MIL_INT SizeBit;
	MIL_INT SizeX;
	MIL_INT SizeY;
	MIL_INT SizeBand;

	MIL_DOUBLE FrameRate;
	MIL_DOUBLE AcquisitionLineRate;
	int DeviceType;

	CameraControl* CC = new CameraControl(); // Class 객체 동적 할당 ( Heap 에 할당 / delete 필수 )

	tuple<MIL_ID, MIL_ID*, MIL_ID, MIL_ID, MIL_INT, MIL_INT, MIL_INT, MIL_INT> Result1;
	tuple<MIL_ID, MIL_ID, MIL_ID> BufferID;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEEWORKSTS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	CComboBox m_list_board;
	CButton m_btn_boardalloc;
	afx_msg void OnBnClickedBtnBalloc();
	afx_msg void OnCbnSelchangeComboBoard();
	afx_msg void OnBnClickedBtnDcf();
	afx_msg void OnBnClickedButtonOpen();
	CButton m_btn_dcf1;
	CButton m_btn_dcfopen;
	CButton m_btn_dcffree;
	afx_msg void OnBnClickedButtonFree();
	void Opendisplay2();
	void Opendisplay3();
	void Opendisplay4();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
