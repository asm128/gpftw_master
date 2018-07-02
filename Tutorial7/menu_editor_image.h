#include "menu.h"

#ifndef MENU_EDITOR_IMAGE_H_29879342872
#define MENU_EDITOR_IMAGE_H_29879342872

namespace gme
{

	enum EDIT_IMAGE_MENU_EVENT : int32_t
		{ EDIT_IMAGE_MENU_EVENT_NONE					= 0

		, EDIT_IMAGE_MENU_EVENT_NEW
		, EDIT_IMAGE_MENU_EVENT_OPEN
		, EDIT_IMAGE_MENU_EVENT_SAVE
		, EDIT_IMAGE_MENU_EVENT_EXIT

		, EDIT_IMAGE_MENU_EVENT_COUNT
		, EDIT_IMAGE_MENU_EVENT_INVALID					= -1
		};

	static	const SMenuItem								g_MenuOptionsMain	[]		=	
		{ {"File"			}
		, {"Edit"			}
		, {"View"			}
		, {"Tool"			}
		, {"Help"			}
		};

	static	const SMenuItem								g_MenuOptionsFile	[]		=	
		{ {"New	"			, EDIT_IMAGE_MENU_EVENT_NEW		}
		, {"Open"			, EDIT_IMAGE_MENU_EVENT_OPEN	}
		, {"Save"			, EDIT_IMAGE_MENU_EVENT_SAVE	}
		, {"Exit"			, EDIT_IMAGE_MENU_EVENT_EXIT	}
		};
}

#endif // MENU_EDITOR_IMAGE_H_29879342872
