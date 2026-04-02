//*****************************************************************************
// FILE:            WinMTRThemedHeaderCtrl.cpp
//
//*****************************************************************************

#include "WinMTRGlobal.h"
#include "WinMTRThemedHeaderCtrl.h"

BEGIN_MESSAGE_MAP(WinMTRThemedHeaderCtrl, CHeaderCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL WinMTRThemedHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	if(!WinMTRIsDarkModeEnabled()) {
		return CHeaderCtrl::OnEraseBkgnd(pDC);
	}
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, RGB(45, 45, 48));
	return TRUE;
}

void WinMTRThemedHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	const WinMTRThemeColors& colors = WinMTRGetThemeColors();
	CRect rect(lpDrawItemStruct->rcItem);
	dc.FillSolidRect(rect, WinMTRIsDarkModeEnabled() ? RGB(45, 45, 48) : ::GetSysColor(COLOR_BTNFACE));
	dc.Draw3dRect(rect,
		WinMTRIsDarkModeEnabled() ? RGB(75, 75, 78) : ::GetSysColor(COLOR_3DSHADOW),
		WinMTRIsDarkModeEnabled() ? RGB(75, 75, 78) : ::GetSysColor(COLOR_3DSHADOW));

	HDITEM item = {0};
	TCHAR text[128] = {0};
	item.mask = HDI_TEXT | HDI_FORMAT;
	item.pszText = text;
	item.cchTextMax = sizeof(text) / sizeof(TCHAR);
	GetItem((int)lpDrawItemStruct->itemID, &item);

	CRect textRect = rect;
	textRect.DeflateRect(6, 0);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(colors.textColor);
	UINT format = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	format |= (item.fmt & HDF_RIGHT) ? DT_RIGHT : ((item.fmt & HDF_CENTER) ? DT_CENTER : DT_LEFT);
	dc.DrawText(text, textRect, format);

	dc.Detach();
}
