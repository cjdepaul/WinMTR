//*****************************************************************************
// FILE:            WinMTRThemedButton.h
//
//*****************************************************************************

#ifndef WINMTRTHEMEDBUTTON_H_
#define WINMTRTHEMEDBUTTON_H_

class WinMTRThemedButton : public CButton
{
public:
	WinMTRThemedButton();
	virtual void PreSubclassWindow();

protected:
	UINT m_originalStyleType;
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()
};

#endif // WINMTRTHEMEDBUTTON_H_
