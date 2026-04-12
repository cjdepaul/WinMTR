#include "WinMTRGlobal.h"
#include "WinMTRDarkHeaderCtrl.h"

BEGIN_MESSAGE_MAP(WinMTRDarkHeaderCtrl, CHeaderCtrl)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL WinMTRDarkHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	if(!WinMTRIsDarkModeEnabled()) {
		return CHeaderCtrl::OnEraseBkgnd(pDC);
	}

	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect, WinMTRGetThemeColors().controlBackground);
	return TRUE;
}

void WinMTRDarkHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	CRect rect(lpDrawItemStruct->rcItem);
	const WinMTRThemeColors& colors = WinMTRGetThemeColors();
	const bool dark = WinMTRIsDarkModeEnabled();

	COLORREF background = dark ? colors.controlBackground : ::GetSysColor(COLOR_BTNFACE);
	COLORREF textColor = dark ? colors.textColor : ::GetSysColor(COLOR_BTNTEXT);
	COLORREF borderColor = dark ? RGB(85, 85, 85) : ::GetSysColor(COLOR_3DSHADOW);

	dc.FillSolidRect(rect, background);
	dc.Draw3dRect(rect, borderColor, borderColor);

	HDITEM item = {0};
	char text[256] = {0};
	item.mask = HDI_TEXT | HDI_FORMAT;
	item.pszText = text;
	item.cchTextMax = sizeof(text) - 1;
	GetItem(static_cast<int>(lpDrawItemStruct->itemID), &item);

	CRect textRect(rect);
	textRect.DeflateRect(6, 2);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(textColor);
	UINT format = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
	format |= (item.fmt & HDF_CENTER) ? DT_CENTER : ((item.fmt & HDF_RIGHT) ? DT_RIGHT : DT_LEFT);
	dc.DrawText(text, textRect, format);

	dc.Detach();
}
