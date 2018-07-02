#include "menu.h"

#ifndef MENU_MAIN_H_9283742893
#define MENU_MAIN_H_9283742893

namespace gme 
{
	
	enum APP_MENU_EVENT : int32_t
		{ APP_MENU_EVENT_NONE				= 0
		, APP_MENU_EVENT_EXIT				
		, APP_MENU_EVENT_NEW_CONTEXT_TERRAIN
		, APP_MENU_EVENT_NEW_CONTEXT_IMAGE
		, APP_MENU_EVENT_NEW_CONTEXT_MODEL
		, APP_MENU_EVENT_NEW_PALETTE
		, APP_MENU_EVENT_OPEN_TERRAIN
		, APP_MENU_EVENT_OPEN_IMAGE
		, APP_MENU_EVENT_OPEN_MODEL
		, APP_MENU_EVENT_OPEN_PALETTE
		, APP_MENU_EVENT_SAVE_TERRAIN
		, APP_MENU_EVENT_SAVE_IMAGE
		, APP_MENU_EVENT_SAVE_MODEL
		, APP_MENU_EVENT_SAVE_PALETTE
		, APP_MENU_EVENT_ABOUT
		, APP_MENU_EVENT_COUNT
		, APP_MENU_EVENT_INVALID			= -1
		};

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
		{ {"New Terrain"	, APP_MENU_EVENT_NEW_CONTEXT_TERRAIN	}
		, {"New Model"		, APP_MENU_EVENT_NEW_CONTEXT_MODEL		}
		, {"New Image"		, APP_MENU_EVENT_NEW_CONTEXT_IMAGE		}
		, {"New Palette"	, APP_MENU_EVENT_NEW_PALETTE			}
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
		{ {"About..."	, APP_MENU_EVENT_ABOUT	}
		};
} // namespace 

#endif // MENU_MAIN_H_9283742893
