#include "gpk_gui_desktop.h"
#include "gpk_framework.h"
#include "gpk_scene.h"
#include "gpk_ro_rsw.h"
#include "gpk_ro_gnd.h"
#include "gpk_ro_rsm.h"

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
						::gpk::SColorBGRA															ColorPaint;
						::gpk::SDesktop																Desktop								= {};
						::gpk::SFramework															Framework							;
						::gpk::ptr_obj<::gpk::SRenderTarget>										Offscreen							= {};
						
						::std::mutex																LockGUI;
						::std::mutex																LockRender;

						::gpk::array_obj<::gpk::ptr_obj<::gpk::SRenderTarget>>						PaintScreen							= {};
						::gpk::array_obj<::gpk::ptr_obj<::gpk::STexture<::gpk::SColorBGRA>>>		PaletteColors;

																									SApplication						(::gpk::SRuntimeValues & runtimeValues)		: Framework(runtimeValues)		{}
	};

	typedef			::std::lock_guard<::std::mutex>												mutex_guard;
} // namespace

#endif // APPLICATION_H_2078934982734
