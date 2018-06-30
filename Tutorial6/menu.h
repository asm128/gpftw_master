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

	enum APP_MENU_EVENT : int32_t
		{ APP_MENU_EVENT_NONE				= 0
		, APP_MENU_EVENT_EXIT				
		, APP_MENU_EVENT_NEW_TERRAIN
		, APP_MENU_EVENT_NEW_IMAGE
		, APP_MENU_EVENT_NEW_PALETTE
		, APP_MENU_EVENT_NEW_MODEL
		, APP_MENU_EVENT_OPEN_TERRAIN
		, APP_MENU_EVENT_OPEN_IMAGE
		, APP_MENU_EVENT_OPEN_PALETTE
		, APP_MENU_EVENT_OPEN_MODEL
		, APP_MENU_EVENT_SAVE_TERRAIN
		, APP_MENU_EVENT_SAVE_IMAGE
		, APP_MENU_EVENT_SAVE_PALETTE
		, APP_MENU_EVENT_SAVE_MODEL
		, APP_MENU_EVENT_ABOUT
		, APP_MENU_EVENT_COUNT
		, APP_MENU_EVENT_INVALID			= -1
		};

	::gpk::error_t										getMenuItemIndex			(const ::gpk::view_array<const SMenuItem>& items, const ::gpk::view_const_string& text)					{
		for(uint32_t iItem = 0; iItem < items.size(); ++iItem) 
			if(text == items[iItem].Text)
				return iItem;
		return 0;
	}

	static	const SMenuItem								g_MenuOptionsMain	[]		=	
		{ {"File"			}
		, {"Edit"			}
		, {"View"			}
		, {"Tool"			}
		, {"Help"			}
		};

	static	const SMenuItem								g_MenuOptionsFile	[]		=	
		{ {"New"			}
		, {"Open"			}
		, {"Save"			}
		, {"Exit"			, APP_MENU_EVENT_EXIT}
		};

	static	const SMenuItem								g_MenuOptionsNew	[]		=	
		{ {"New Terrain"	, APP_MENU_EVENT_NEW_TERRAIN	}
		, {"New Model"		, APP_MENU_EVENT_NEW_MODEL		}
		, {"New Image"		, APP_MENU_EVENT_NEW_IMAGE		}
		, {"New Palette"	, APP_MENU_EVENT_NEW_PALETTE	}
		};

	static	const SMenuItem								g_MenuOptionsOpen	[]		=	
		{ {"Open Terrain"	, APP_MENU_EVENT_OPEN_TERRAIN	}
		, {"Open Model"		, APP_MENU_EVENT_OPEN_MODEL		}
		, {"Open Image"		, APP_MENU_EVENT_OPEN_IMAGE		}
		, {"Open Palette"	, APP_MENU_EVENT_OPEN_PALETTE	}
		};

	static	const SMenuItem								g_MenuOptionsSave	[]		=	
		{ {"Save Terrain"	, APP_MENU_EVENT_SAVE_TERRAIN	}
		, {"Save Model"		, APP_MENU_EVENT_SAVE_MODEL		}
		, {"Save Image"		, APP_MENU_EVENT_SAVE_IMAGE		}
		, {"Save Palette"	, APP_MENU_EVENT_SAVE_PALETTE	}
		};

	static	const SMenuItem								g_MenuOptionsEdit	[]		=	
		{ {"Selection"	}
		, {"BitDepth"	}
		, {"Pencil"		}
		};

	static	const SMenuItem								g_MenuOptionsHelp	[]		=	
		{ {"About..."	, APP_MENU_EVENT_ABOUT}
		};
}

#endif // MENU_H_02837498237498023
