//  [11/23/2020 astrid]
// LargeImgMfcDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LargeImgMfcDemo.h"
#include "LargeImgMfcDemoDlg.h"
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
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CLargeImgMfcDemoDlg dialog



CLargeImgMfcDemoDlg::CLargeImgMfcDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LARGEIMGMFCDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLargeImgMfcDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_edtPath);
	DDX_Control(pDX, IDC_EDIT_W, m_edtW);
	DDX_Control(pDX, IDC_EDIT_H, m_edtH);
	DDX_Control(pDX, IDC_CHK_SAVE, m_chkSave);
	DDX_Control(pDX, IDC_EDIT_INFO, m_edtInfo);
	DDX_Control(pDX, IDC_PIC, m_wndRes);
	DDX_Control(pDX, IDC_CB_TILES, m_cbTiles);
}

BEGIN_MESSAGE_MAP(CLargeImgMfcDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CLargeImgMfcDemoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLargeImgMfcDemoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_PATH, &CLargeImgMfcDemoDlg::OnBnClickedBtnPath)
END_MESSAGE_MAP()


// CLargeImgMfcDemoDlg message handlers

BOOL CLargeImgMfcDemoDlg::OnInitDialog()
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
	
	m_edtW.SetWindowText(_T("3500"));
	m_edtH.SetWindowText(_T("2800"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLargeImgMfcDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLargeImgMfcDemoDlg::OnPaint()
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
HCURSOR CLargeImgMfcDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLargeImgMfcDemoDlg::OnBnClickedOk()
{
	CString sFileName;
	m_edtPath.GetWindowText(sFileName);
	errno_t err = 0;
	if ((err = _taccess_s(sFileName, 0)) != 0) {
		return;
	}

	m_proc.SetPathName(sFileName);
	m_edtInfo.SetWindowText(m_proc.GetInfo());

	int cw = 0, ch = 0;
	BOOL bSave = FALSE;
	CString s;

	bSave = (m_chkSave.GetCheck() == BST_CHECKED);
	m_edtW.GetWindowText(s);
	cw = _ttoi(s);

	m_edtH.GetWindowText(s);
	ch = _ttoi(s);

	if (cw < 1 || ch < 1)
		return;
	m_proc.Convert(cw, ch, bSave);
}


void CLargeImgMfcDemoDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CLargeImgMfcDemoDlg::OnBnClickedBtnPath()
{
	TCHAR szFilter[] = _T("Bitmap Files (*.bmp)|*.bmp|")
		_T("Image Files|*.bmp; *.jpg; *.png; *.tif; *.tiff |All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("bmp"), NULL, 6, szFilter);
	if (dlg.DoModal() != IDOK)
		return;
	
	CString pathName = dlg.GetPathName();
	m_edtPath.SetWindowText(pathName);
	
	m_proc.SetPathName(pathName);
	m_edtInfo.SetWindowText(m_proc.GetInfo());
}
