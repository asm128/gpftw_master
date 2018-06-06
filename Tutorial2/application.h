#include "gpk_framework.h"
#include "gpk_gui.h"
#include "gpk_gui_control.h"

#include <mutex>

#ifndef APPLICATION_H_2078934982734
#define APPLICATION_H_2078934982734

namespace gme // I'm gonna use a different namespace in order to test a few things about the macros.
{
	struct SApplication {
		::gpk::SFramework								Framework;
		::gpk::ptr_obj<::gpk::SRenderTarget>			Offscreen							= {};

		int32_t											IdExit								= -1;
		::gpk::SControlList								ControlListMain						= {};
		::gpk::SControlList								ControlListFile						= {};

		::std::mutex									LockGUI;
		::std::mutex									LockRender;

														SApplication						(::gpk::SRuntimeValues & runtimeValues)		: Framework(runtimeValues)		{}
	};

	typedef ::std::lock_guard<::std::mutex>			mutex_guard;


} // namespace

#endif // APPLICATION_H_2078934982734
