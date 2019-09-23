
// TskClockDlg.h : header file
//

#pragma once


// CTskClockDlg dialog
class CTskClockDlg : public CDialogEx
{
// Construction
public:
	CTskClockDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TSKCLOCK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	CFont fuente;

	CTimeSpan counterA, counterB;
	CTime reference;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedPausar();
	afx_msg void OnBnClickedAlternar();
	afx_msg void OnBnClickedResetear();
	afx_msg void OnUpdateTimers(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAlternar(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePausar(CCmdUI *pCmdUI);

};
