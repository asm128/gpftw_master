#include "gpk_gui_desktop.h"
#include "gpk_ptr.h"
#include "menu.h"

#ifndef CONTEXT_EDITOR_IMAGE_H_87645873657386587
#define CONTEXT_EDITOR_IMAGE_H_87645873657386587

namespace gme
{
	struct SContextEditorImage {
		::gpk::SDesktop											Desktop								= {};	
		::gpk::SColorBGRA										ColorPaint							= {0xFF, 0xFF, 0xFF, 0xFF};

		::gpk::ptr_obj<::gpk::SImage<::gpk::SColorBGRA>>		PaintScreen							= {};	
	};

	::gpk::error_t											contextEditorImageInitialize		(::gpk::SGUI & gui, ::gme::SContextEditorImage & editor, int32_t parentControlId);
	::gpk::error_t											contextEditorImageUpdate			(::gpk::SGUI & gui, ::gme::SContextEditorImage & editor, ::gpk::SInput & input);
}

#endif // CONTEXT_EDITOR_IMAGE_H_87645873657386587