//*****************************************************************************
// FILE:            WinMTRGlobal.cpp
//
//
//*****************************************************************************

#include "WinMTRGlobal.h"

#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "UxTheme.lib")

namespace {
	bool g_themeInitialized = false;
	bool g_darkModeEnabled = false;
	CBrush g_windowBrush;
	CBrush g_controlBrush;
	WinMTRThemeColors g_themeColors = { RGB(255, 255, 255), RGB(255, 255, 255), RGB(0, 0, 0), RGB(96, 96, 96), RGB(255, 255, 255) };

	enum PreferredAppMode {
		DefaultAppMode,
		AllowDarkAppMode,
		ForceDarkAppMode,
		ForceLightAppMode,
		MaxAppMode
	};

	typedef BOOL (WINAPI* AllowDarkModeForWindowFn)(HWND, BOOL);
	typedef PreferredAppMode (WINAPI* SetPreferredAppModeFn)(PreferredAppMode);
	typedef VOID (WINAPI* FlushMenuThemesFn)();

	AllowDarkModeForWindowFn g_allowDarkModeForWindow = NULL;
	SetPreferredAppModeFn g_setPreferredAppMode = NULL;
	FlushMenuThemesFn g_flushMenuThemes = NULL;
	bool g_nativeDarkModeInitialized = false;

	void InitializeNativeDarkModeSupport()
	{
		if(g_nativeDarkModeInitialized) return;
		g_nativeDarkModeInitialized = true;

		HMODULE uxTheme = ::LoadLibraryW(L"uxtheme.dll");
		if(!uxTheme) return;

		g_allowDarkModeForWindow = reinterpret_cast<AllowDarkModeForWindowFn>(::GetProcAddress(uxTheme, MAKEINTRESOURCEA(133)));
		g_setPreferredAppMode = reinterpret_cast<SetPreferredAppModeFn>(::GetProcAddress(uxTheme, MAKEINTRESOURCEA(135)));
		g_flushMenuThemes = reinterpret_cast<FlushMenuThemesFn>(::GetProcAddress(uxTheme, MAKEINTRESOURCEA(136)));
	}

	bool QueryDarkModeSetting()
	{
		DWORD value = 1;
		DWORD size = sizeof(value);
		LSTATUS status = RegGetValue(
			HKEY_CURRENT_USER,
			"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
			"AppsUseLightTheme",
			RRF_RT_REG_DWORD,
			NULL,
			&value,
			&size
		);
		if(status != ERROR_SUCCESS) return false;
		return value == 0;
	}

	void RebuildThemeBrushes()
	{
		if((HBRUSH)g_windowBrush) g_windowBrush.DeleteObject();
		if((HBRUSH)g_controlBrush) g_controlBrush.DeleteObject();

		if(g_darkModeEnabled) {
			g_themeColors.windowBackground = RGB(32, 32, 32);
			g_themeColors.controlBackground = RGB(45, 45, 48);
			g_themeColors.textColor = RGB(240, 240, 240);
			g_themeColors.mutedTextColor = RGB(200, 200, 200);
			g_themeColors.listBackground = RGB(30, 30, 30);
		} else {
			g_themeColors.windowBackground = ::GetSysColor(COLOR_3DFACE);
			g_themeColors.controlBackground = RGB(255, 255, 255);
			g_themeColors.textColor = ::GetSysColor(COLOR_WINDOWTEXT);
			g_themeColors.mutedTextColor = ::GetSysColor(COLOR_GRAYTEXT);
			g_themeColors.listBackground = ::GetSysColor(COLOR_WINDOW);
		}

		g_windowBrush.CreateSolidBrush(g_themeColors.windowBackground);
		g_controlBrush.CreateSolidBrush(g_darkModeEnabled ? g_themeColors.controlBackground : g_themeColors.listBackground);
	}

	void ApplyNativeDarkModePreference()
	{
		InitializeNativeDarkModeSupport();
		if(g_setPreferredAppMode) {
			g_setPreferredAppMode(g_darkModeEnabled ? AllowDarkAppMode : DefaultAppMode);
		}
		if(g_flushMenuThemes) {
			g_flushMenuThemes();
		}
	}

	void ApplyNativeThemeToControl(HWND hWnd)
	{
		if(!::IsWindow(hWnd)) return;
		InitializeNativeDarkModeSupport();
		if(g_allowDarkModeForWindow) {
			g_allowDarkModeForWindow(hWnd, g_darkModeEnabled ? TRUE : FALSE);
		}

		char className[64] = {0};
		::GetClassNameA(hWnd, className, sizeof(className));

		LPCWSTR subAppName = NULL;
		if(_stricmp(className, WC_LISTVIEWA) == 0) {
			subAppName = g_darkModeEnabled ? L"Explorer" : NULL;
		} else if(_stricmp(className, WC_HEADERA) == 0) {
			subAppName = g_darkModeEnabled ? L"ItemsView" : NULL;
		} else if(_stricmp(className, WC_COMBOBOXA) == 0 || _stricmp(className, "ComboBox") == 0 ||
			_stricmp(className, WC_EDITA) == 0 || _stricmp(className, "Edit") == 0 ||
			_stricmp(className, "ComboLBox") == 0) {
			subAppName = g_darkModeEnabled ? L"CFD" : NULL;
		} else if(_stricmp(className, WC_BUTTONA) == 0 || _stricmp(className, "Button") == 0 ||
			_stricmp(className, STATUSCLASSNAMEA) == 0 || _stricmp(className, WC_STATICA) == 0) {
			subAppName = g_darkModeEnabled ? L"Explorer" : NULL;
		}

		::SetWindowTheme(hWnd, subAppName, NULL);
		::SendMessage(hWnd, WM_THEMECHANGED, 0, 0);
		::InvalidateRect(hWnd, NULL, TRUE);
	}
}

void WinMTRRefreshTheme()
{
	g_darkModeEnabled = QueryDarkModeSetting();
	RebuildThemeBrushes();
	ApplyNativeDarkModePreference();
	g_themeInitialized = true;
}

bool WinMTRIsDarkModeEnabled()
{
	if(!g_themeInitialized) WinMTRRefreshTheme();
	return g_darkModeEnabled;
}

const WinMTRThemeColors& WinMTRGetThemeColors()
{
	if(!g_themeInitialized) WinMTRRefreshTheme();
	return g_themeColors;
}

HBRUSH WinMTRHandleCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if(!g_themeInitialized) WinMTRRefreshTheme();
	if(!WinMTRIsDarkModeEnabled()) return NULL;

	switch(nCtlColor) {
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(g_themeColors.textColor);
		pDC->SetBkColor(g_themeColors.windowBackground);
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)g_windowBrush;
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		pDC->SetTextColor(g_themeColors.textColor);
		pDC->SetBkColor(g_themeColors.controlBackground);
		return (HBRUSH)g_controlBrush;
	case CTLCOLOR_BTN:
		pDC->SetTextColor(g_themeColors.textColor);
		pDC->SetBkColor(g_themeColors.windowBackground);
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)g_windowBrush;
	default:
		return (HBRUSH)g_windowBrush;
	}
}

void WinMTRApplyThemeToWindow(CWnd* window)
{
	if(!window || !::IsWindow(window->GetSafeHwnd())) return;
	if(!g_themeInitialized) WinMTRRefreshTheme();

	const BOOL enableDark = WinMTRIsDarkModeEnabled() ? TRUE : FALSE;
	const DWORD darkModeAttribute = 20;
	const DWORD darkModeAttributeLegacy = 19;
	InitializeNativeDarkModeSupport();
	if(g_allowDarkModeForWindow) {
		g_allowDarkModeForWindow(window->GetSafeHwnd(), enableDark);
	}
	::DwmSetWindowAttribute(window->GetSafeHwnd(), darkModeAttribute, &enableDark, sizeof(enableDark));
	::DwmSetWindowAttribute(window->GetSafeHwnd(), darkModeAttributeLegacy, &enableDark, sizeof(enableDark));
	ApplyNativeThemeToControl(window->GetSafeHwnd());
}

void WinMTRApplyThemeToChildren(CWnd* parent)
{
	if(!parent || !::IsWindow(parent->GetSafeHwnd())) return;
	if(!g_themeInitialized) WinMTRRefreshTheme();

	for(CWnd* child = parent->GetWindow(GW_CHILD); child; child = child->GetNextWindow()) {
		HWND hWnd = child->GetSafeHwnd();
		if(!hWnd) continue;
		ApplyNativeThemeToControl(hWnd);
	}
}

void WinMTRConfigureListCtrl(CListCtrl& list)
{
	if(!::IsWindow(list.GetSafeHwnd())) return;
	if(!g_themeInitialized) WinMTRRefreshTheme();

	if(WinMTRIsDarkModeEnabled()) {
		list.SetBkColor(g_themeColors.listBackground);
		list.SetTextBkColor(g_themeColors.listBackground);
		list.SetTextColor(g_themeColors.textColor);
	} else {
		list.SetBkColor(::GetSysColor(COLOR_WINDOW));
		list.SetTextBkColor(::GetSysColor(COLOR_WINDOW));
		list.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
	}
	ApplyNativeThemeToControl(list.GetSafeHwnd());
	HWND header = ListView_GetHeader(list.GetSafeHwnd());
	if(header) {
		ApplyNativeThemeToControl(header);
	}
	::InvalidateRect(list.GetSafeHwnd(), NULL, TRUE);
}

//*****************************************************************************
// gettimeofday
//
// win32 port of unix gettimeofday
//*****************************************************************************
/*
int gettimeofday(struct timeval* tv, struct timezone* / *tz* /)
{
   if(!tv)
      return -1;
   struct _timeb timebuffer;
   
   _ftime(&timebuffer);

   tv->tv_sec = (long)timebuffer.time;
   tv->tv_usec = timebuffer.millitm * 1000 + 500;
   return 0;
}// */

