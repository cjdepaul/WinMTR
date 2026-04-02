//*****************************************************************************
// FILE:            WinMTRThemedComboBox.h
//
//*****************************************************************************

#ifndef WINMTRTHEMEDCOMBOBOX_H_
#define WINMTRTHEMEDCOMBOBOX_H_

class WinMTRThemedComboBox : public CComboBox
{
public:
	WinMTRThemedComboBox();
	virtual void PreSubclassWindow();
	void RefreshTheme();

protected:
	CWnd m_editProxy;
	CWnd m_listProxy;

	void AttachChildren();
	void DrawDarkFrame();

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif // WINMTRTHEMEDCOMBOBOX_H_
