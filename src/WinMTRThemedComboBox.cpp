//*****************************************************************************
// FILE:            WinMTRThemedComboBox.cpp
//
//*****************************************************************************

#include "WinMTRGlobal.h"
#include "WinMTRThemedComboBox.h"

BEGIN_MESSAGE_MAP(WinMTRThemedComboBox, CComboBox)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

WinMTRThemedComboBox::WinMTRThemedComboBox()
{
}

void WinMTRThemedComboBox::PreSubclassWindow()
{
	CComboBox::PreSubclassWindow();
	RefreshTheme();
}

void WinMTRThemedComboBox::AttachChildren()
{
	COMBOBOXINFO info = {0};
	info.cbSize = sizeof(info);
	if(!GetComboBoxInfo(&info)) return;

	if(info.hwndItem && !m_editProxy.GetSafeHwnd()) {
		m_editProxy.Attach(info.hwndItem);
	}
	if(info.hwndList && !m_listProxy.GetSafeHwnd()) {
		m_listProxy.Attach(info.hwndList);
	}
}

void WinMTRThemedComboBox::RefreshTheme()
{
	if(!::IsWindow(GetSafeHwnd())) return;
	AttachChildren();

	if(WinMTRIsDarkModeEnabled()) {
		::SetWindowTheme(GetSafeHwnd(), L"", L"");
		if(m_editProxy.GetSafeHwnd()) ::SetWindowTheme(m_editProxy.GetSafeHwnd(), L"", L"");
		if(m_listProxy.GetSafeHwnd()) ::SetWindowTheme(m_listProxy.GetSafeHwnd(), L"DarkMode_Explorer", NULL);
	} else {
		::SetWindowTheme(GetSafeHwnd(), NULL, NULL);
		if(m_editProxy.GetSafeHwnd()) ::SetWindowTheme(m_editProxy.GetSafeHwnd(), NULL, NULL);
		if(m_listProxy.GetSafeHwnd()) ::SetWindowTheme(m_listProxy.GetSafeHwnd(), NULL, NULL);
	}
	Invalidate();
}

HBRUSH WinMTRThemedComboBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH brush = WinMTRHandleCtlColor(pDC, this, nCtlColor);
	if(brush) return brush;
	return NULL;
}

void WinMTRThemedComboBox::DrawDarkFrame()
{
	if(!WinMTRIsDarkModeEnabled()) return;

	CWindowDC dc(this);
	CRect rect;
	GetWindowRect(&rect);
	rect.OffsetRect(-rect.left, -rect.top);

	const WinMTRThemeColors& colors = WinMTRGetThemeColors();
	dc.FillSolidRect(rect, colors.controlBackground);
	dc.Draw3dRect(rect, RGB(96, 96, 96), RGB(96, 96, 96));

	COMBOBOXINFO info = {0};
	info.cbSize = sizeof(info);
	if(GetComboBoxInfo(&info)) {
		CRect buttonRect(info.rcButton);
		ScreenToClient(&buttonRect);
		dc.FillSolidRect(buttonRect, RGB(45, 45, 48));
		dc.Draw3dRect(buttonRect, RGB(96, 96, 96), RGB(96, 96, 96));

		CPoint center((buttonRect.left + buttonRect.right) / 2, (buttonRect.top + buttonRect.bottom) / 2);
		CPen pen(PS_SOLID, 1, colors.textColor);
		CPen* oldPen = dc.SelectObject(&pen);
		dc.MoveTo(center.x - 4, center.y - 1);
		dc.LineTo(center.x, center.y + 3);
		dc.LineTo(center.x + 4, center.y - 1);
		dc.SelectObject(oldPen);
	}
}

LRESULT WinMTRThemedComboBox::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = CComboBox::WindowProc(message, wParam, lParam);

	switch(message) {
	case WM_PAINT:
	case WM_NCPAINT:
	case WM_SIZE:
	case WM_SETTINGCHANGE:
		if(message == WM_SETTINGCHANGE) {
			WinMTRRefreshTheme();
			RefreshTheme();
		}
		DrawDarkFrame();
		break;
	}

	return result;
}
