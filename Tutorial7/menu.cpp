#include "menu.h"


				::gpk::error_t									gme::getMenuItemIndex			(const ::gpk::view_array<const SMenuItem>& items, const ::gpk::view_const_string& text)					{
	for(uint32_t iItem = 0; iItem < items.size(); ++iItem) 
		if(text == items[iItem].Text)
			return iItem;
	return 0;
}