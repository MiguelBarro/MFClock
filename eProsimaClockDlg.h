
// eProsimaClockDlg.h : header file
//

#pragma once

#include "resource.h"		// main symbols

class CeProsimaClockApp;


// CeProsimaClockDlg dialog
class CeProsimaClockDlg : public CDialogEx
{
// Construction
public:
	CeProsimaClockDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLOCK_DIALOG };

	protected:
	void DoDataExchange(CDataExchange* pDX) override;	// DDX/DDV support
    BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) override;

// Implementation
protected:
	HICON m_hIcon;
	CFont prefix_font, count_font;

    CeProsimaClockApp * GetApp();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedTimerState();
    afx_msg void OnListenerCallback(); // callback translation into command

    afx_msg void OnDestroy();
};
