#include "gpk_gui_control.h"
//#include "gpk_grid_view.h"
//#include "gpk_array_static.h"
//#include "gpk_array.h"

#ifndef DESKTOP_H_827364792647823
#define DESKTOP_H_827364792647823

namespace gme
{
	struct SAppPaletteGrid {
				int32_t														IdControl							= -1;
				::gpk::array_pod<int32_t>									IdControls							= {};
				::gpk::grid_view<::gpk::SColorBGRA>							Colors								= {};
	};

			::gpk::error_t												paletteInitialize					(::gpk::SGUI& gui, ::gme::SAppPaletteGrid& palette);
			::gpk::error_t												paletteColorsSet					(::gpk::SGUI& gui, ::gme::SAppPaletteGrid& palette, const ::gpk::grid_view<::gpk::SColorBGRA>& colors);
} // namespace

#endif // DESKTOP_H_827364792647823