#include "desktop.h"
#include "gpk_gui_desktop.h"
#include "gpk_framework.h"

#include <mutex>

#ifndef APPLICATION_H_2078934982734
#define APPLICATION_H_2078934982734

namespace gme // I'm gonna use a different namespace in order to test a few things about the macros.
{
	enum APP_MENU
		{ APP_MENU_MAIN		= 0
		, APP_MENU_FILE
		, APP_MENU_EDIT
		, APP_MENU_COUNT	
		};

	struct SApplication {
				::gpk::SFramework											Framework							;
				::gpk::ptr_obj<::gpk::SRenderTarget>						Offscreen							= {};
				::gpk::array_obj<::gpk::ptr_obj<::gpk::SRenderTarget>>		PaintScreen							= {};
				::gpk::array_obj<::gpk::SControlList>						Menus								= {};

				::gpk::SDesktop												Desktop								= {};
				::gme::SAppPaletteGrid										PaletteControl;
				::gpk::STexture<::gpk::SColorBGRA>							PaletteColors;
				

				::std::mutex												LockGUI;
				::std::mutex												LockRender;

				::gpk::SColorBGRA											ColorPaint;

																			SApplication						(::gpk::SRuntimeValues & runtimeValues)		: Framework(runtimeValues)		{}
	};

	typedef	::std::lock_guard<::std::mutex>									mutex_guard;


} // namespace

#endif // APPLICATION_H_2078934982734
