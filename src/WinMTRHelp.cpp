// WinMTRHelp.cpp : implementation file
//

#include "WinMTRGlobal.h"
#include "WinMTRHelp.h"
#include "afxdialogex.h"


// WinMTRHelp dialog

IMPLEMENT_DYNAMIC(WinMTRHelp, CDialog)

WinMTRHelp::WinMTRHelp(CWnd* pParent /*=NULL*/)
	: CDialog(WinMTRHelp::IDD, pParent)
{

}

WinMTRHelp::~WinMTRHelp()
{
}

void WinMTRHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(WinMTRHelp, CDialog)
	ON_BN_CLICKED(IDOK, &WinMTRHelp::OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()


// WinMTRHelp message handlers

BOOL WinMTRHelp::OnInitDialog()
{
	CDialog::OnInitDialog();
	WinMTRRefreshTheme();
	WinMTRApplyThemeToWindow(this);
	WinMTRApplyThemeToChildren(this);
	return TRUE;
}

void WinMTRHelp::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

HBRUSH WinMTRHelp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH brush = WinMTRHandleCtlColor(pDC, pWnd, nCtlColor);
	if(brush) return brush;
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void WinMTRHelp::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDialog::OnSettingChange(uFlags, lpszSection);
	WinMTRRefreshTheme();
	WinMTRApplyThemeToWindow(this);
	WinMTRApplyThemeToChildren(this);
	Invalidate(TRUE);
}
