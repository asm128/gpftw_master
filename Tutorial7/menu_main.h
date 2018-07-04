#include "menu.h"

#ifndef MENU_MAIN_H_9283742893
#define MENU_MAIN_H_9283742893

namespace gme 
{
	
	enum APP_MENU_EVENT : int32_t
		{ APP_MENU_EVENT_NONE				= 0
		, APP_MENU_EVENT_EXIT				
		, APP_MENU_EVENT_NEW_PROJECT
		, APP_MENU_EVENT_NEW_CONTEXT_TERRAIN
		, APP_MENU_EVENT_NEW_CONTEXT_IMAGE
		, APP_MENU_EVENT_NEW_CONTEXT_MODEL
		, APP_MENU_EVENT_NEW_PALETTE
		, APP_MENU_EVENT_OPEN_PROJECT
		, APP_MENU_EVENT_OPEN_TERRAIN
		, APP_MENU_EVENT_OPEN_IMAGE
		, APP_MENU_EVENT_OPEN_MODEL
		, APP_MENU_EVENT_OPEN_PALETTE
		, APP_MENU_EVENT_SAVE_PROJECT
		, APP_MENU_EVENT_ABOUT
		, APP_MENU_EVENT_NEW_PALETTE_8_bit_BGRA				
		, APP_MENU_EVENT_NEW_PALETTE_8_bit_BGR				
		, APP_MENU_EVENT_NEW_PALETTE_8_bit_Grayscale		
		, APP_MENU_EVENT_NEW_PALETTE_4_bit_Grayscale		
		, APP_MENU_EVENT_NEW_PALETTE_2_bit_Grayscale		
		, APP_MENU_EVENT_NEW_PALETTE_Monochrome				
		, APP_MENU_EVENT_NEW_PALETTE_Indexed_2_colors		
		, APP_MENU_EVENT_NEW_PALETTE_Indexed_4_colors		
		, APP_MENU_EVENT_NEW_PALETTE_Indexed_16_colors		
		, APP_MENU_EVENT_NEW_PALETTE_Indexed_256_colors		
		, APP_MENU_EVENT_NEW_PALETTE_Indexed_65535_colors	
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

	static	const SMenuItem								g_MenuOptionsNew		[]		=	
		{ {"Project"		, APP_MENU_EVENT_NEW_PROJECT			}
		, {"Terrain"		, APP_MENU_EVENT_NEW_CONTEXT_TERRAIN	}
		, {"Model"			, APP_MENU_EVENT_NEW_CONTEXT_MODEL		}
		, {"Image"			, APP_MENU_EVENT_NEW_CONTEXT_IMAGE		}
		, {"Palette"		, APP_MENU_EVENT_NEW_PALETTE			}
		};

	static	const SMenuItem								g_MenuOptionsOpen		[]		=	
		{ {"Open Terrain"	, APP_MENU_EVENT_OPEN_TERRAIN	}
		, {"Open Model"		, APP_MENU_EVENT_OPEN_MODEL		}
		, {"Open Image"		, APP_MENU_EVENT_OPEN_IMAGE		}
		, {"Open Palette"	, APP_MENU_EVENT_OPEN_PALETTE	}
		};

	static	const SMenuItem								g_MenuOptionsPalette	[]		=	
		{ {"8-bit BGRA"				, APP_MENU_EVENT_NEW_PALETTE_8_bit_BGRA				}
		, {"8-bit BGR"				, APP_MENU_EVENT_NEW_PALETTE_8_bit_BGR				}
		, {"8-bit Grayscale"		, APP_MENU_EVENT_NEW_PALETTE_8_bit_Grayscale		}
		, {"4-bit Grayscale"		, APP_MENU_EVENT_NEW_PALETTE_4_bit_Grayscale		}
		, {"2-bit Grayscale"		, APP_MENU_EVENT_NEW_PALETTE_2_bit_Grayscale		}
		, {"Monochrome"				, APP_MENU_EVENT_NEW_PALETTE_Monochrome				}
		, {"Indexed 2 colors"		, APP_MENU_EVENT_NEW_PALETTE_Indexed_2_colors		}
		, {"Indexed 4 colors"		, APP_MENU_EVENT_NEW_PALETTE_Indexed_4_colors		}
		, {"Indexed 16 colors"		, APP_MENU_EVENT_NEW_PALETTE_Indexed_16_colors		}
		, {"Indexed 256 colors"		, APP_MENU_EVENT_NEW_PALETTE_Indexed_256_colors		}
		, {"Indexed 65535 colors"	, APP_MENU_EVENT_NEW_PALETTE_Indexed_65535_colors	}
		};

	static	const SMenuItem								g_MenuOptionsSave	[]		=	
		{ {"Save Project"	, APP_MENU_EVENT_SAVE_PROJECT	}
		};

	static	const SMenuItem								g_MenuOptionsEdit	[]		=	
		{ {"Selection"		}
		, {"BitDepth"		}
		, {"Pencil"			}
		};

	static	const SMenuItem								g_MenuOptionsHelp	[]		=	
		{ {"About..."		, APP_MENU_EVENT_ABOUT	}
		};
} // namespace 

#endif // MENU_MAIN_H_9283742893
