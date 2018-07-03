#include "gpk_gui_desktop.h"

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

						::gpk::error_t									getMenuItemIndex			(const ::gpk::view_array<const SMenuItem>& items, const ::gpk::view_const_string& text);
						::gpk::error_t									setupMenu					(::gpk::SGUI & gui, ::gpk::SDesktop & desktop, const ::gpk::view_array<const ::gme::SMenuItem>& menuItems, int32_t iParentList, int32_t iParentItem); // File


}

#endif // MENU_H_02837498237498023
