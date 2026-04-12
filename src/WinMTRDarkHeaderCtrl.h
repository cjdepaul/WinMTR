#ifndef WINMTRDARKHEADERCTRL_H_
#define WINMTRDARKHEADERCTRL_H_

class WinMTRDarkHeaderCtrl : public CHeaderCtrl
{
public:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

#endif // WINMTRDARKHEADERCTRL_H_
