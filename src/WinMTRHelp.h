#pragma once

// WinMTRHelp dialog

class WinMTRHelp : public CDialog
{
	DECLARE_DYNAMIC(WinMTRHelp)

public:
	WinMTRHelp(CWnd* pParent = NULL);   // standard constructor
	virtual ~WinMTRHelp();

// Dialog Data
	enum { IDD = IDD_DIALOG_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
};
