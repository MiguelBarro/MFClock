
// eProsimaClockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "eProsimaClock.h"
#include "eProsimaClockDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CeProsimaClockDlg dialog




CeProsimaClockDlg::CeProsimaClockDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CeProsimaClockDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CeProsimaClockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CeProsimaClockDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PAUSAR, &CeProsimaClockDlg::OnBnClickedPausar)
	ON_BN_CLICKED(IDC_ALTERNAR, &CeProsimaClockDlg::OnBnClickedAlternar)
	ON_BN_CLICKED(IDC_RESETEAR, &CeProsimaClockDlg::OnBnClickedResetear)
	ON_UPDATE_COMMAND_UI_RANGE(IDC_EDIT_A, IDC_EDIT_B, &CeProsimaClockDlg::OnUpdateTimers) 
	ON_UPDATE_COMMAND_UI(IDC_ALTERNAR, &CeProsimaClockDlg::OnUpdateAlternar)
	ON_UPDATE_COMMAND_UI(IDC_PAUSAR, &CeProsimaClockDlg::OnUpdatePausar)
END_MESSAGE_MAP()


// CeProsimaClockDlg message handlers

BOOL CeProsimaClockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Crear la nueva fuente
	fuente.CreatePointFont(200,_T("Courier New"),GetDC());
	GetDlgItem(IDC_EDIT_A)->SetFont(&fuente);
	GetDlgItem(IDC_EDIT_B)->SetFont(&fuente);

	// Resetear de entrada
	OnBnClickedResetear();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CeProsimaClockDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CeProsimaClockDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CeProsimaClockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CeProsimaClockDlg::OnTimer(UINT_PTR nIDEvent)
{
	CTime actual = CTime::GetCurrentTime();

	// Actualizar el time span correspondiente
	CButton * pB = (CButton*)GetDlgItem(IDC_ALTERNAR);
	int estado = pB->GetCheck();

	if( estado == BST_CHECKED )
	{	// Contador B
		counterB = actual - reference;
	}
	else
	{	// Contador A
		counterA = actual - reference;
	}

	// Actualizar todos los controles
	UpdateDialogControls(this,TRUE);

	CDialogEx::OnTimer(nIDEvent);
}

void CeProsimaClockDlg::OnBnClickedPausar()
{
	CButton * pB = (CButton*)GetDlgItem(IDC_PAUSAR);
	int estado = pB->GetCheck();

	if( estado == BST_CHECKED )
	{	// Arrancar
		SetTimer(IDC_PAUSAR,1000,0);
		OnBnClickedAlternar();
	}
	else
	{	// Pausar
		KillTimer(IDC_PAUSAR);
	}

	// Actualizar todos los controles
	UpdateDialogControls(this,TRUE);
}

void CeProsimaClockDlg::OnBnClickedAlternar()
{
	CTime actual = CTime::GetCurrentTime();

	// Modificar el tiempo de reset
	CButton * pB = (CButton*)GetDlgItem(IDC_ALTERNAR);
	int estado = pB->GetCheck();

	if( estado == BST_CHECKED )
	{	// Contador B
		reference = actual - counterB;	
	}
	else
	{	// Contador A
		reference = actual - counterA;
	}

	// Actualizar todos los controles
	UpdateDialogControls(this,TRUE);
}

void CeProsimaClockDlg::OnBnClickedResetear()
{
	CTimeSpan aux;
	counterA = aux;
	counterB = aux;

	OnBnClickedAlternar();

	// Actualizar todos los controles
	UpdateDialogControls(this,TRUE);
}

void CeProsimaClockDlg::OnUpdateTimers(CCmdUI *pCmdUI)
{
	CTimeSpan ts;
	CWnd * pE = pCmdUI->m_pOther;
	
	if( pE->GetDlgCtrlID() == IDC_EDIT_A )
		ts = counterA;
	else
		ts = counterB;

	pE->SetWindowText(ts.Format(_T("%H:%M:%S")));
}

void CeProsimaClockDlg::OnUpdateAlternar(CCmdUI *pCmdUI)
{
	CButton * pB = (CButton*)pCmdUI->m_pOther;
	int estado = pB->GetCheck();
	CString caption, actual;

	if( estado == BST_CHECKED )
	{	// Contador B
		caption = _T("Run B");
	}
	else
	{	// Contador A
		caption = _T("Run A");
	}

	pB->GetWindowText(actual);
	if( caption != actual)
		pB->SetWindowText(caption);
}

void CeProsimaClockDlg::OnUpdatePausar(CCmdUI *pCmdUI)
{
	CButton * pB = (CButton*)pCmdUI->m_pOther;
	int estado = pB->GetCheck();
	CString caption, actual;

	if( estado == BST_CHECKED )
	{	// Parar
		caption = _T("Stop");
	}
	else
	{	// Arrancar
		caption = _T("Start");
	}

	pB->GetWindowText(actual);
	if( caption != actual)
		pB->SetWindowText(caption);

}
