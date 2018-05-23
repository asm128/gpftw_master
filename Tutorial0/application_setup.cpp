#include "application.h"
#include <Windowsx.h>

static constexpr	const uint32_t														BMP_SCREEN_WIDTH							= 640;
static constexpr	const uint32_t														BMP_SCREEN_HEIGHT							= uint32_t(::BMP_SCREEN_WIDTH * (9.0 / 16.0));
static				::RECT																minClientRect								= {100, 100, 100 + 320, 100 + 200};

extern				::SApplication														* g_ApplicationInstance						;

					LRESULT WINAPI														mainWndProc									(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)							{
	::SApplication																				& applicationInstance						= *g_ApplicationInstance;
	static	const int																			adjustedMinRect								= ::AdjustWindowRectEx(&minClientRect, WS_OVERLAPPEDWINDOW, FALSE, 0);

	::gpk::SDisplay																				& mainDisplay								= applicationInstance.Framework.MainDisplay;
	::gpk::SInput																				& input										= applicationInstance.Framework.Input;
	::gpk::SDisplayPlatformDetail																& displayDetail								= mainDisplay.PlatformDetail;
	int32_t																						zDelta										= {};
	switch(uMsg) {
	default: break;		
	case WM_CLOSE			: ::DestroyWindow(hWnd); return 0;
	case WM_KEYDOWN			: if(wParam > ::gpk::size(input.KeyboardPrevious.KeyState)) break; input.KeyboardPrevious.KeyState[wParam] = input.KeyboardCurrent.KeyState[wParam]; input.KeyboardCurrent.KeyState[wParam]  =  1; return 0;
	case WM_KEYUP			: if(wParam > ::gpk::size(input.KeyboardPrevious.KeyState)) break; input.KeyboardPrevious.KeyState[wParam] = input.KeyboardCurrent.KeyState[wParam]; input.KeyboardCurrent.KeyState[wParam] &= ~1; return 0;
	case WM_MOUSEWHEEL		:
		zDelta																					= GET_WHEEL_DELTA_WPARAM(wParam);
		input.MouseCurrent.Deltas.z																= zDelta;
		return 0;
	case WM_MOUSEMOVE		: {
		int32_t																						xPos										= GET_X_LPARAM(lParam); 
		int32_t																						yPos										= GET_Y_LPARAM(lParam); 
		input.MouseCurrent.Position.x															= ::gpk::clamp(xPos, 0, (int32_t)mainDisplay.Size.x);
		input.MouseCurrent.Position.y															= ::gpk::clamp(yPos, 0, (int32_t)mainDisplay.Size.y);
		input.MouseCurrent.Deltas.x																= input.MouseCurrent.Position.x - input.MousePrevious.Position.x;
		input.MouseCurrent.Deltas.y																= input.MouseCurrent.Position.y - input.MousePrevious.Position.y;
		return 0;
	}
	case WM_GETMINMAXINFO	:	// Catch this message so to prevent the window from becoming too small.
		((::MINMAXINFO*)lParam)->ptMinTrackSize													= {minClientRect.right - minClientRect.left, minClientRect.bottom - minClientRect.top}; 
		return 0;
	case WM_SIZE			: 
		if(lParam) {
			::gpk::SCoord2<uint32_t>																	newMetrics									= ::gpk::SCoord2<WORD>{LOWORD(lParam), HIWORD(lParam)}.Cast<uint32_t>();
			if(newMetrics != mainDisplay.Size) {
				mainDisplay.PreviousSize																= mainDisplay.Size;
				mainDisplay.Size																		= newMetrics;
				mainDisplay.Resized																		= true;
				mainDisplay.Repaint																		= true; 
				char																						buffer		[256]							= {};
				sprintf_s(buffer, "[%u x %u]. Last frame seconds: %g. ", (uint32_t)newMetrics.x, (uint32_t)newMetrics.y, applicationInstance.Framework.Timer.LastTimeSeconds);
#if defined(UNICODE)
#else
				SetWindowText(mainDisplay.PlatformDetail.WindowHandle, buffer);
#endif
			}
		}
		if( wParam == SIZE_MINIMIZED ) {
			mainDisplay.MinOrMaxed = mainDisplay.NoDraw												= true;
		}
		else if( wParam == SIZE_MAXIMIZED ) {
			mainDisplay.Resized = mainDisplay.MinOrMaxed											= true;
			mainDisplay.NoDraw																		= false;
		}
		else if( wParam == SIZE_RESTORED ) {
			mainDisplay.Resized																		= true;
			mainDisplay.MinOrMaxed																	= true;
			mainDisplay.NoDraw																		= false;
		}
		else {
			//State.Resized									= true;	? 
			mainDisplay.MinOrMaxed = mainDisplay.NoDraw												= false;
		}
		break;
	case WM_PAINT			: break;
	case WM_DESTROY			: 
		::PostQuitMessage(0); 
		displayDetail.WindowHandle																= 0;
		mainDisplay.Closed																		= true;
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

					void																initWndClass								(::HINSTANCE hInstance, const TCHAR* className, WNDPROC wndProc, ::WNDCLASSEX& wndClassToInit)	{
	wndClassToInit																			= {sizeof(::WNDCLASSEX),};
	wndClassToInit.lpfnWndProc																= wndProc;
	wndClassToInit.hInstance																= hInstance;
	wndClassToInit.hIcon																	= LoadIcon	(NULL, IDC_ICON);
	wndClassToInit.hCursor																	= LoadCursor(NULL, IDC_ARROW);
	wndClassToInit.hbrBackground															= (::HBRUSH)(COLOR_3DFACE + 1);
	wndClassToInit.lpszClassName															= className;
}

					::gpk::error_t														mainWindowCreate							(::gpk::SDisplay& mainWindow, HINSTANCE hInstance)													{ 
	::gpk::SDisplayPlatformDetail																& displayDetail								= mainWindow.PlatformDetail;
	::initWndClass(hInstance, displayDetail.WindowClassName, ::mainWndProc, displayDetail.WindowClass);
	::RegisterClassEx(&displayDetail.WindowClass);
	mainWindow.Size																			= {::BMP_SCREEN_WIDTH, ::BMP_SCREEN_HEIGHT};
	::RECT																						finalClientRect								= {100, 100, 100 + (LONG)mainWindow.Size.x, 100 + (LONG)mainWindow.Size.y};
	DWORD																						windowStyle									= WS_OVERLAPPEDWINDOW; //WS_POPUP;
	::AdjustWindowRectEx(&finalClientRect, windowStyle, FALSE, 0);
	mainWindow.PlatformDetail.WindowHandle													= ::CreateWindowEx(0, displayDetail.WindowClassName, TEXT("Window"), windowStyle
		, finalClientRect.left
		, finalClientRect.top
		, finalClientRect.right		- finalClientRect.left
		, finalClientRect.bottom	- finalClientRect.top
		, 0, 0, displayDetail.WindowClass.hInstance, 0
		);
	::ShowWindow	(displayDetail.WindowHandle, SW_SHOW);
	::UpdateWindow	(displayDetail.WindowHandle);
	return 0;
}
