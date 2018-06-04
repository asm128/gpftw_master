#include "gpk_framework.h"
#include "gpk_gui.h"

#include <mutex>

#ifndef APPLICATION_H_2078934982734
#define APPLICATION_H_2078934982734

namespace gme // I'm gonna use a different namespace in order to test a few things about the macros.
{
#pragma pack(push, 1)
	struct SMenuItem {
		int32_t											IdControl;
		int32_t											IdParent ;
	};

	enum MENU_ORIENTATION : int32_t 
		{	MENU_ORIENTATION_HORIZONTAL	= 1
		,	MENU_ORIENTATION_VERTICAL	= 2
		};

	struct SMenu {
		int32_t											IdControl							= -1;
		MENU_ORIENTATION								Orientation							= MENU_ORIENTATION_HORIZONTAL;
		::gpk::array_pod<SMenuItem>						Items								= {};
		::gpk::array_obj<::gpk::array_pod<int32_t>>		Children							= {};
	};

	::gpk::error_t									addMenuItem							(::gpk::SGUI& gui, SMenu& menu, const ::gpk::view_const_string& text, int32_t idParent);
#pragma pack(pop)

	struct SApplication {
		::gpk::SFramework								Framework;
		::gpk::ptr_obj<::gpk::SRenderTarget>			Offscreen							= {};

		int32_t											IdExit								= -1;
		SMenu											Menu								= {};

		::std::mutex									LockGUI;
		::std::mutex									LockRender;

														SApplication						(::gpk::SRuntimeValues & runtimeValues)		: Framework(runtimeValues)		{}
	};

	typedef ::std::lock_guard<::std::mutex>			mutex_guard;

} // namespace

#endif // APPLICATION_H_2078934982734
