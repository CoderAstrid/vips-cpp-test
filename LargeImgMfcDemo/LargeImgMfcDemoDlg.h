
// LargeImgMfcDemoDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "LargeImgProc.h"


// CLargeImgMfcDemoDlg dialog
class CLargeImgMfcDemoDlg : public CDialogEx
{
// Construction
public:
	CLargeImgMfcDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LARGEIMGMFCDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CLargeImgProc m_proc;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_edtPath;
	afx_msg void OnBnClickedBtnPath();
	CEdit m_edtW;
	CEdit m_edtH;
	CButton m_chkSave;
	CEdit m_edtInfo;
	CStatic m_wndRes;
	CComboBox m_cbTiles;
};
