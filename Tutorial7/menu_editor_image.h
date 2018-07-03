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

		, EDIT_IMAGE_MENU_EVENT_NEW_8_bit_BGRA
		, EDIT_IMAGE_MENU_EVENT_NEW_8_bit_BGR		
		, EDIT_IMAGE_MENU_EVENT_NEW_8_bit_Grayscale
		, EDIT_IMAGE_MENU_EVENT_NEW_4_bit_Grayscale
		, EDIT_IMAGE_MENU_EVENT_NEW_2_bit_Grayscale
		, EDIT_IMAGE_MENU_EVENT_NEW_Monochrome
		, EDIT_IMAGE_MENU_EVENT_NEW_Indexed_2_colors
		, EDIT_IMAGE_MENU_EVENT_NEW_Indexed_4_colors		
		, EDIT_IMAGE_MENU_EVENT_NEW_Indexed_16_colors		
		, EDIT_IMAGE_MENU_EVENT_NEW_Indexed_256_colors		
		, EDIT_IMAGE_MENU_EVENT_NEW_Indexed_65535_colors	

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
		{ {"New "			, EDIT_IMAGE_MENU_EVENT_NEW		}
		, {"Open"			, EDIT_IMAGE_MENU_EVENT_OPEN	}
		, {"Save"			, EDIT_IMAGE_MENU_EVENT_SAVE	}
		, {"Close"			, EDIT_IMAGE_MENU_EVENT_EXIT	}
		};

	static	const SMenuItem								g_MenuOptionsNew	[]		=	
		{ {"8-bit BGRA"				, EDIT_IMAGE_MENU_EVENT_NEW_8_bit_BGRA				}
		, {"8-bit BGR"				, EDIT_IMAGE_MENU_EVENT_NEW_8_bit_BGR				}
		, {"8-bit Grayscale"		, EDIT_IMAGE_MENU_EVENT_NEW_8_bit_Grayscale			}
		, {"4-bit Grayscale"		, EDIT_IMAGE_MENU_EVENT_NEW_4_bit_Grayscale			}
		, {"2-bit Grayscale"		, EDIT_IMAGE_MENU_EVENT_NEW_2_bit_Grayscale			}
		, {"Monochrome"				, EDIT_IMAGE_MENU_EVENT_NEW_Monochrome				}
		, {"Indexed 2 colors"		, EDIT_IMAGE_MENU_EVENT_NEW_Indexed_2_colors		}
		, {"Indexed 4 colors"		, EDIT_IMAGE_MENU_EVENT_NEW_Indexed_4_colors		}
		, {"Indexed 16 colors"		, EDIT_IMAGE_MENU_EVENT_NEW_Indexed_16_colors		}
		, {"Indexed 256 colors"		, EDIT_IMAGE_MENU_EVENT_NEW_Indexed_256_colors		}
		, {"Indexed 65535 colors"	, EDIT_IMAGE_MENU_EVENT_NEW_Indexed_65535_colors	}
		};


}

#endif // MENU_EDITOR_IMAGE_H_29879342872
