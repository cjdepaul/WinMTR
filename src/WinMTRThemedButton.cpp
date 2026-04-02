//*****************************************************************************
// FILE:            WinMTRThemedButton.cpp
//
//*****************************************************************************

#include "WinMTRGlobal.h"
#include "WinMTRThemedButton.h"

BEGIN_MESSAGE_MAP(WinMTRThemedButton, CButton)
END_MESSAGE_MAP()

WinMTRThemedButton::WinMTRThemedButton()
	: m_originalStyleType(BS_PUSHBUTTON)
{
}

void WinMTRThemedButton::PreSubclassWindow()
{
	m_originalStyleType = GetButtonStyle() & BS_TYPEMASK;
	ModifyStyle(BS_TYPEMASK, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void WinMTRThemedButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	const WinMTRThemeColors& colors = WinMTRGetThemeColors();
	CRect rect(lpDrawItemStruct->rcItem);
	bool isDark = WinMTRIsDarkModeEnabled();
	bool isPressed = (lpDrawItemStruct->itemState & ODS_SELECTED) != 0;
	bool isDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED) != 0;
	bool hasFocus = (lpDrawItemStruct->itemState & ODS_FOCUS) != 0;
	bool isCheckbox = m_originalStyleType == BS_AUTOCHECKBOX ||
		m_originalStyleType == BS_AUTO3STATE ||
		m_originalStyleType == BS_3STATE ||
		m_originalStyleType == BS_CHECKBOX;

	COLORREF fillColor = isDark ? RGB(45, 45, 48) : RGB(245, 245, 245);
	COLORREF borderColor = isDark ? RGB(96, 96, 96) : RGB(160, 160, 160);
	COLORREF textColor = isDisabled ? colors.mutedTextColor : colors.textColor;
	if(isPressed) {
		fillColor = isDark ? RGB(62, 62, 66) : RGB(225, 225, 225);
	}

	dc.FillSolidRect(rect, fillColor);
	dc.Draw3dRect(rect, borderColor, borderColor);

	CString caption;
	GetWindowText(caption);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(textColor);

	if(isCheckbox) {
		CRect box = rect;
		box.right = box.left + 14;
		box.DeflateRect(1, 1);
		int boxSize = min(box.Width(), box.Height());
		box.bottom = box.top + boxSize;
		box.left += 2;
		box.top = rect.top + (rect.Height() - boxSize) / 2;
		box.right = box.left + boxSize;
		box.bottom = box.top + boxSize;

		COLORREF boxFill = isDark ? RGB(30, 30, 30) : RGB(255, 255, 255);
		dc.FillSolidRect(box, boxFill);
		dc.Draw3dRect(box, borderColor, borderColor);
		if(GetCheck() != BST_UNCHECKED) {
			CPen pen(PS_SOLID, 2, isDark ? RGB(220, 220, 220) : RGB(32, 32, 32));
			CPen* oldPen = dc.SelectObject(&pen);
			dc.MoveTo(box.left + 3, box.top + box.Height() / 2);
			dc.LineTo(box.left + box.Width() / 2 - 1, box.bottom - 4);
			dc.LineTo(box.right - 3, box.top + 3);
			dc.SelectObject(oldPen);
		}

		CRect textRect = rect;
		textRect.left = box.right + 6;
		dc.DrawText(caption, textRect, DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	} else {
		CRect textRect = rect;
		dc.DrawText(caption, textRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	if(hasFocus) {
		CRect focusRect = rect;
		focusRect.DeflateRect(3, 3);
		dc.DrawFocusRect(focusRect);
	}

	dc.Detach();
}
