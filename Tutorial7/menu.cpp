#include "menu.h"


				::gpk::error_t									gme::getMenuItemIndex			(const ::gpk::view_array<const SMenuItem>& items, const ::gpk::view_const_string& text)					{
	for(uint32_t iItem = 0; iItem < items.size(); ++iItem) 
		if(text == items[iItem].Text)
			return iItem;
	return 0;
}

				::gpk::error_t									gme::setupMenu					(::gpk::SGUI & gui, ::gpk::SDesktop & desktop, const ::gpk::view_array<const ::gme::SMenuItem>& menuItems, int32_t iParentList, int32_t iParentItem)		{ 
	::gpk::error_t																idMenu									= ::gpk::desktopCreateControlList(gui, desktop); 
	for(uint32_t iOption = 0; iOption < menuItems.size(); ++iOption) {
		::gme::SMenuItem															item									= menuItems[iOption];
		error_if(errored(::gpk::controlListPush(gui, desktop.Items.ControlLists[idMenu], item.Text, item.IdEvent)), "??"); 
	}
	if(-1 == iParentList || -1 == iParentItem)
			desktop.Items.ControlLists[idMenu].Orientation							= ::gpk::CONTROL_LIST_DIRECTION_HORIZONTAL;
	else
		error_if(errored(::gpk::desktopControlListSetParent(gui, desktop, idMenu, iParentList, iParentItem)), "Invalid parent?"); 
	return idMenu;
} // File
