#include "gpk_view_array.h"

#ifndef MENU_H_02837498237498023
#define MENU_H_02837498237498023

namespace gme 
{
	struct SMenuItem {
							const ::gpk::view_const_string					Text;
							int32_t											IdEvent;
		inline constexpr													SMenuItem			
			(	const ::gpk::view_const_string	text
			,	int32_t							eventId		= -1
			)	: Text		(text	)
				, IdEvent	(eventId)
		{}
	};

	::gpk::error_t										getMenuItemIndex			(const ::gpk::view_array<const SMenuItem>& items, const ::gpk::view_const_string& text);

}

#endif // MENU_H_02837498237498023
