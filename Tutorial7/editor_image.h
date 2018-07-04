#include "gpk_gui_desktop.h"
#include "gpk_ptr.h"
#include "menu.h"

#ifndef CONTEXT_EDITOR_IMAGE_H_87645873657386587
#define CONTEXT_EDITOR_IMAGE_H_87645873657386587

namespace gme
{
	enum EDITOR_IMAGE_FORMAT : uint32_t 
		{ EDITOR_IMAGE_FORMAT_8_bit_BGRA				
		, EDITOR_IMAGE_FORMAT_8_bit_BGR				
		, EDITOR_IMAGE_FORMAT_8_bit_Grayscale		
		, EDITOR_IMAGE_FORMAT_4_bit_Grayscale		
		, EDITOR_IMAGE_FORMAT_2_bit_Grayscale		
		, EDITOR_IMAGE_FORMAT_Monochrome				
		, EDITOR_IMAGE_FORMAT_Indexed_2_colors		
		, EDITOR_IMAGE_FORMAT_Indexed_4_colors		
		, EDITOR_IMAGE_FORMAT_Indexed_16_colors		
		, EDITOR_IMAGE_FORMAT_Indexed_256_colors		
		, EDITOR_IMAGE_FORMAT_Indexed_65535_colors	
		};

	struct SContextEditorImage {
		::gpk::SDesktop											Desktop								= {};	
		::gpk::SColorBGRA										ColorPaint							= {0xFF, 0xFF, 0xFF, 0xFF};

		::gme::EDITOR_IMAGE_FORMAT								ImageFormat							= EDITOR_IMAGE_FORMAT_8_bit_BGRA;

		::gpk::ptr_obj<::gpk::SImage<::gpk::SColorBGRA>>		PaintScreen							= {};	
		::gpk::ptr_obj<::gpk::SImage<::gpk::SColorBGR>>			PaintScreenBGR						= {};	
		::gpk::ptr_obj<::gpk::SImage<uint8_t>>					PaintScreen8						= {};	
	};

	::gpk::error_t											contextEditorImageInitialize		(::gpk::SGUI & gui, ::gme::SContextEditorImage & editor, int32_t parentControlId);
	::gpk::error_t											contextEditorImageUpdate			(::gpk::SGUI & gui, ::gme::SContextEditorImage & editor, ::gpk::SInput & input);
}

#endif // CONTEXT_EDITOR_IMAGE_H_87645873657386587
