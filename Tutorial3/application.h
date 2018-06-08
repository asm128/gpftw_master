#include "gpk_framework.h"
#include "gpk_gui.h"
#include "gpk_gui_control.h"
#include "gpk_label.h"

#include <mutex>

#ifndef APPLICATION_H_2078934982734
#define APPLICATION_H_2078934982734

namespace gme // I'm gonna use a different namespace in order to test a few things about the macros.
{
	struct SAppMenu {
		::gpk::SControlList								ControlListMain						= {};
		::gpk::SControlList								ControlListFile						= {};
		int32_t											ControlSelectedMain					= -1;
	};

	struct SApplication {
		::gpk::SFramework								Framework;
		::gpk::ptr_obj<::gpk::SRenderTarget>			Offscreen							= {};
		::gpk::ptr_obj<::gpk::SRenderTarget>			PaintScreen							= {};

		SAppMenu										Menu								= {};


		::std::mutex									LockGUI;
		::std::mutex									LockRender;

														SApplication						(::gpk::SRuntimeValues & runtimeValues)		: Framework(runtimeValues)		{}
	};

	typedef ::std::lock_guard<::std::mutex>			mutex_guard;


} // namespace

#endif // APPLICATION_H_2078934982734
