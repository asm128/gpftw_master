#include "gpk_framework.h"
#include "gpk_scene.h"
#include "gpk_ro_rsw.h"
#include "gpk_ro_gnd.h"
#include "gpk_ro_rsm.h"

#include "editor_image.h"
#include "gpk_ascii_target.h"

#include <mutex>

#ifndef APPLICATION_H_2078934982734
#define APPLICATION_H_2078934982734

namespace gme // I'm gonna use a different namespace in order to test a few things about the macros.
{
	struct SConsole {
						::gpk::SDesktop															Desktop								= {};

						::gpk::SASCIITarget														Target;
	};

	struct SApplication {
						::gpk::SDesktop															Desktop								= {};
						::gpk::SFramework														Framework							;
						::gpk::ptr_obj<::gpk::SRenderTarget<::gpk::SColorBGRA, uint32_t>>		Offscreen							= {};

						::std::mutex															LockGUI;
						::std::mutex															LockRender;

						::gpk::array_obj<::gpk::ptr_obj<::gpk::SImage<::gpk::SColorBGRA>>>	PaletteColors;
						::gpk::array_obj<::gme::SContextEditorImage>							EditorsImage;

																								SApplication						(::gpk::SRuntimeValues & runtimeValues)		: Framework(runtimeValues)		{}
	};

	typedef			::std::lock_guard<::std::mutex>											mutex_guard;
} // namespace

#endif // APPLICATION_H_2078934982734
