#include "gpk_framework.h"
#include "gpk_gui.h"
#include "gpk_gui_control.h"

#include <mutex>

#ifndef APPLICATION_H_2078934982734
#define APPLICATION_H_2078934982734

namespace gme // I'm gonna use a different namespace in order to test a few things about the macros.
{
	struct SAppMenu {
		::gpk::SControlList													ControlListMain						= {};
		::gpk::SControlList													ControlListFile						= {};
		int32_t																ControlSelectedMain					= -1;
	};

	struct SApplication {
		::gpk::SFramework													Framework;
		::gpk::ptr_obj<::gpk::SRenderTarget<::gpk::SColorBGRA, uint32_t>>	Offscreen							= {};
		::gpk::ptr_obj<::gpk::STexture<::gpk::SColorBGRA>>					PaintScreen							= {};

		SAppMenu															Menu								= {};
		int32_t																IdExit								= -1;
		int32_t																IdFile								= -1;
		int32_t																IdNew								= -1;

		::std::mutex														LockGUI;
		::std::mutex														LockRender;

																			SApplication						(::gpk::SRuntimeValues & runtimeValues)		: Framework(runtimeValues)		{}
	};

	typedef ::std::lock_guard<::std::mutex>								mutex_guard;


} // namespace

#endif // APPLICATION_H_2078934982734
