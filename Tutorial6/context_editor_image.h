#include "gpk_gui_desktop.h"
#include "gpk_ptr.h"
#include "gpk_ptr.h"

#ifndef CONTEXT_EDITOR_IMAGE_H_87645873657386587
#define CONTEXT_EDITOR_IMAGE_H_87645873657386587

namespace gme
{
	struct SContextEditorImage {
		::gpk::SDesktop																Desktop;	
		::gpk::array_obj<::gpk::ptr_obj<::gpk::STexture<::gpk::SColorBGRA>>>		PaintScreen							= {};	// One for each paint viewport
		::gpk::array_obj<::gpk::ptr_obj<::gpk::STexture<::gpk::SColorBGRA>>>		PaletteColors							;	// One for each palette viewport
		
	};
}

#endif // CONTEXT_EDITOR_IMAGE_H_87645873657386587
