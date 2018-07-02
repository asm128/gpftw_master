#include "gpk_gui_desktop.h"
#include "gpk_ptr.h"
#include "gpk_ptr.h"

#ifndef CONTEXT_EDITOR_IMAGE_H_87645873657386587
#define CONTEXT_EDITOR_IMAGE_H_87645873657386587

namespace gme
{
	struct SContextEditorImage {
		::gpk::SDesktop											Desktop								= {};	
		::gpk::SColorBGRA										ColorPaint							= {0xFF, 0xFF, 0xFF, 0xFF};

		::gpk::ptr_obj<::gpk::STexture<::gpk::SColorBGRA>>		PaintScreen							= {};	
	};
}

#endif // CONTEXT_EDITOR_IMAGE_H_87645873657386587
