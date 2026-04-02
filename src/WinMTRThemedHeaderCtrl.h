//*****************************************************************************
// FILE:            WinMTRThemedHeaderCtrl.h
//
//*****************************************************************************

#ifndef WINMTRTHEMEDHEADERCTRL_H_
#define WINMTRTHEMEDHEADERCTRL_H_

class WinMTRThemedHeaderCtrl : public CHeaderCtrl
{
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

#endif // WINMTRTHEMEDHEADERCTRL_H_
