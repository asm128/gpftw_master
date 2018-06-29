#include "gpk_view_array.h"

#ifndef MENU_H_02837498237498023
#define MENU_H_02837498237498023

namespace gme 
{
	enum MENU_OPTION_MAIN : int32_t
		{ MENU_OPTION_MAIN_File				= 0
		, MENU_OPTION_MAIN_Edit
		, MENU_OPTION_MAIN_View
		, MENU_OPTION_MAIN_Tool
		, MENU_OPTION_MAIN_Help
		, MENU_OPTION_MAIN_COUNT
		};

	enum MENU_OPTION_FILE : int32_t
		{ MENU_OPTION_FILE_New				= 0
		, MENU_OPTION_FILE_Open
		, MENU_OPTION_FILE_Save
		, MENU_OPTION_FILE_Exit
		, MENU_OPTION_FILE_COUNT
		};

	enum MENU_OPTION_EDIT : int32_t
		{ MENU_OPTION_EDIT_Selection		= 0
		, MENU_OPTION_EDIT_Palette
		, MENU_OPTION_EDIT_BitDepth
		, MENU_OPTION_EDIT_Pencil
		, MENU_OPTION_EDIT_COUNT
		};

	static	const ::gpk::view_const_string				g_MenuOptionsMain	[::gme::MENU_OPTION_MAIN_COUNT]		=	
		{ "File"
		, "Edit"
		, "View"
		, "Tool"
		, "Help"
		};

	static	const ::gpk::view_const_string				g_MenuOptionsFile	[::gme::MENU_OPTION_FILE_COUNT]		=	
		{ "New "
		, "Open"
		, "Save"
		, "Exit"
		};

	static	const ::gpk::view_const_string				g_MenuOptionsEdit	[::gme::MENU_OPTION_EDIT_COUNT]		=	
		{ "Selection"
		, "Palette"
		, "BitDepth"
		, "Pencil"
		};
}

#endif // MENU_H_02837498237498023
