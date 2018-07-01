#include "gpk_framework.h"
#include "gpk_gui.h"
#include "gpk_gui_control.h"
#include "gpk_label.h"

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

	struct SAppMenu {
				::gpk::array_static<::gpk::SControlList, APP_MENU_COUNT>	Items								= {};
				int32_t														ItemSelectedMain					= -1;
	};

	enum VIEWPORT_CONTROL
		{ VIEWPORT_CONTROL_TITLE	= 0
		, VIEWPORT_CONTROL_TARGET
		, VIEWPORT_CONTROL_COUNT
		};

	struct SViewport {
				int32_t														IdControl							= -1;
				::gpk::array_static<int32_t, ::gme::VIEWPORT_CONTROL_COUNT>	IdControls							= {};
	};

			::gpk::error_t												viewportInitialize					(::gpk::SGUI& gui, ::gme::SViewport& viewport);
			::gpk::error_t												viewportResize						(::gpk::SGUI& gui, ::gme::SViewport& viewport);

	struct SApplication {
				::gpk::SFramework													Framework;
				::gpk::ptr_obj<::gpk::SRenderTarget<::gpk::SColorBGRA, uint32_t>>	Offscreen							= {};
				::gpk::ptr_obj<::gpk::STexture<::gpk::SColorBGRA>>					PaintScreen							= {};

				int32_t																ControlDesktop						= -1;
				::gme::SAppMenu														Menu								= {};
				::gme::SViewport													PaintViewport						= {};

				::std::mutex														LockGUI;
				::std::mutex														LockRender;

																					SApplication						(::gpk::SRuntimeValues & runtimeValues)		: Framework(runtimeValues)		{}
	};

	typedef	::std::lock_guard<::std::mutex>										mutex_guard;


} // namespace

#endif // APPLICATION_H_2078934982734
