#include "editor_image.h"
#include "menu_editor_image.h"
#include "gpk_png.h"

static		::gpk::error_t												setupDesktop							(::gpk::SGUI & gui, ::gpk::SDesktop & desktop)		{ 
	// --- Setup desktop  
	for(uint32_t iChildList = 0; iChildList < desktop.Children.size(); ++iChildList)
		desktop.Children[iChildList].clear();
	for(uint32_t iChildList = 0; iChildList < desktop.Items.ControlLists.size(); ++iChildList) 
		desktop.Items.ControlLists.Unused[iChildList]							= true;
	for(uint32_t iChildList = 0; iChildList < desktop.Items.Viewports.size(); ++iChildList) 
		desktop.Items.Viewports.Unused[iChildList]								= true;
	for(uint32_t iChildList = 0; iChildList < desktop.Items.PaletteGrids.size(); ++iChildList) 
		desktop.Items.PaletteGrids.Unused[iChildList]							= true;
	gpk_necall(desktop.IdControl = ::gpk::controlCreate(gui), "Why would this ever happen? Out of memory?");
	gui.Controls.Modes		[desktop.IdControl].Design						= true;
	gui.Controls.Constraints[desktop.IdControl].AttachSizeToControl			= {desktop.IdControl, desktop.IdControl};
	gui.Controls.Controls	[desktop.IdControl].ColorTheme					= ::gpk::ASCII_COLOR_DARKGREY * 16 + 13;
	gui.Controls.Controls	[desktop.IdControl].Border						= 
	gui.Controls.Controls	[desktop.IdControl].Margin						= {};
	return 0;
}	
				::gpk::error_t											gme::contextEditorImageInitialize		(::gpk::SGUI & gui, ::gme::SContextEditorImage & editor, int32_t parentControlId)			{
	::gpk::SDesktop																& desktop								= editor.Desktop;
	gpk_necall(::setupDesktop(gui, desktop), "Why would this ever happen? Out of memory?");
	if(parentControlId != -1)
		gpk_necall(::gpk::controlSetParent(gui, desktop.IdControl, parentControlId), "Invalid parent id?");
	::gpk::error_t																idMenuMain								= ::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsMain, -1, -1);
	::gpk::error_t																idMenuFile								= ::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsFile, idMenuMain, ::gme::getMenuItemIndex(::gme::g_MenuOptionsMain, "File"));
	idMenuFile;
	::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsNew	, idMenuFile, ::gme::getMenuItemIndex(::gme::g_MenuOptionsFile, "New"));
	//	::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsOpen	, idMenuFile, ::gme::getMenuItemIndex(::gme::g_MenuOptionsFile, "Open"));
	//	::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsSave	, idMenuFile, ::gme::getMenuItemIndex(::gme::g_MenuOptionsFile, "Save"));
	//::gpk::error_t idEdit = ::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsEdit	, idMenuMain, ::gme::getMenuItemIndex(::gme::g_MenuOptionsMain, "Edit")); idEdit;
	//::gpk::error_t idHelp = ::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsHelp	, idMenuMain, ::gme::getMenuItemIndex(::gme::g_MenuOptionsMain, "Help")); idHelp;
	return 0;
}

			::gpk::error_t												paintViewportSave						(::gme::SContextEditorImage & editor, const ::gpk::view_const_string& filename)							{ 
	FILE																		* fp									= 0;
	::gpk::array_pod<ubyte_t>													data;
	gpk_necall(::gpk::pngFileWrite(editor.PaintScreen->View, data), "Failed to encode PNG!");
	if(data.size()) {
		fopen_s(&fp, filename.begin(), "wb");
		if(fp) {
			fwrite(data.begin(), 1, data.size(), fp);
			fclose(fp);
		}
		else
			error_printf("Failed to open file: %s.", filename.begin());
	}
	return 0;
}

				::gpk::error_t											gme::contextEditorImageUpdate			(::gpk::SGUI & gui, ::gme::SContextEditorImage & editor, ::gpk::SInput & input)				{
	::gpk::SDesktop																& desktop								= editor.Desktop;
	::gpk::error_t																command									= (::gpk::error_t)::gpk::desktopUpdate(gui, desktop, input);
	char																		buffer [128]							= {};
	static	int32_t																nameIndex								= 0;
	switch(command) {
	case 0: break;
	case ::gme::EDIT_IMAGE_MENU_EVENT_SAVE	: 
		sprintf_s(buffer, "gpk_image_%u.png", nameIndex);
		::paintViewportSave(editor, buffer);	
		break;
	case ::gme::EDIT_IMAGE_MENU_EVENT_NEW	: 
		memset(editor.PaintScreen->Texels.begin(), 0, editor.PaintScreen->Texels.size() * sizeof(::gpk::SColorBGRA));	
		break;
	case ::gme::EDIT_IMAGE_MENU_EVENT_EXIT: 
		::gpk::controlDelete(gui, desktop.IdControl);
		desktop.IdControl														= -1;
		return 0;
	}
	++nameIndex;
	return 0;				
}
