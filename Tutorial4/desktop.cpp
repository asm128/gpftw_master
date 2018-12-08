#include "desktop.h"
#include "gpk_label.h"

			::gpk::error_t											gme::paletteInitialize					(::gpk::SGUI& gui, ::gme::SAppPaletteGrid& palette)				{
	const int16_t															heightTitleBar							= gui.FontCharSize.y + 4;
	const uint32_t															widthTarget								= 256;
	const uint32_t															heightTarget							= 256;
	static int32_t															coutnerPalette							= 0;
	++coutnerPalette;
	{
		palette.IdControl													= ::gpk::controlCreate(gui);
		::gpk::SControl															& control								= gui.Controls.Controls[palette.IdControl];
		const uint32_t															widthViewport							= widthTarget + control.Border.Left + control.Border.Right + control.Margin.Left + control.Margin.Right;
		const uint32_t															heightViewport							= heightTarget + heightTitleBar + control.Border.Top + control.Border.Bottom + control.Margin.Top + control.Margin.Bottom;
		control.Area.Size													= {(int16_t)widthViewport, (int16_t)heightViewport};
		control.Align														= ::gpk::ALIGN_CENTER;
	}
	{
		palette.IdControls.clear();
		palette.IdControls.push_back(::gpk::controlCreate(gui));
		::gpk::SControl															& control								= gui.Controls.Controls		[palette.IdControls[0]];
		::gpk::SControlConstraints												& controlConstraints					= gui.Controls.Constraints	[palette.IdControls[0]];
		::gpk::SControlText														& controlText							= gui.Controls.Text			[palette.IdControls[0]];
		controlConstraints.AttachSizeToControl.x							= palette.IdControls[0];
		control.Area.Size.y														= heightTitleBar;
		char																		buffer [256];
		const int32_t																titleLen								= sprintf_s(buffer, "Palette #%i", coutnerPalette);
		controlText.Text														= {::gpk::label(buffer).begin(), (uint32_t)titleLen};
		controlText.Align														= ::gpk::ALIGN_CENTER_TOP;
		::gpk::controlSetParent(gui, palette.IdControls[0], palette.IdControl);
	}
	return 0;
}

			::gpk::error_t											gme::paletteColorsSet					(::gpk::SGUI& gui, ::gme::SAppPaletteGrid& palette, const ::gpk::view_grid<::gpk::SColorBGRA>& colors)			{
	if(-1 == palette.IdControl)
		::gme::paletteInitialize(gui, palette);

	const ::gpk::SCoord2<uint32_t>											& gridMetrics							= colors.metrics();
	static constexpr	const uint32_t										offsetControl							= 1;
	palette.IdControls.resize(colors.size() + offsetControl);
	palette.Colors														= colors;

	const ::gpk::SCoord2<double>											targetSize								= {256.0, 256.0};
	::gpk::SCoord2<double>													controlSize								= {targetSize.x / colors.metrics().x, targetSize.y / colors.metrics().y};
	const uint32_t															offsetY									= gui.FontCharSize.y + 4;

	for(uint32_t y = 0; y < gridMetrics.y; ++y)																				  
	for(uint32_t x = 0; x < gridMetrics.x; ++x) {
		const int32_t															linearIndex								= y * gridMetrics.x + x;
		uint32_t																iFoundColor								= 0;
		int32_t																	themeIndex								= -1;
		const ::gpk::SColorBGRA													& gridCellColor							= colors[y][x];
		for(; iFoundColor < gui.Palette.size(); ++iFoundColor) 
			if(gui.Palette[iFoundColor] == gridCellColor) {
				themeIndex															= iFoundColor;
				break; 
			}
		if(iFoundColor >= gui.Palette.size()) {
			int32_t																	indexColor								= gui.Palette.push_back(gridCellColor);
			themeIndex															= gui.ControlThemes.push_back({});
			::gpk::SControlTheme													& theme									= gui.ControlThemes[themeIndex];
			::gpk::array_static<uint32_t, ::gpk::GUI_CONTROL_COLOR_COUNT>			& colorComboDisabled					= theme.ColorCombos[::gpk::GUI_CONTROL_PALETTE_DISABLED	]	= {};
			::gpk::array_static<uint32_t, ::gpk::GUI_CONTROL_COLOR_COUNT>			& colorComboPressed 					= theme.ColorCombos[::gpk::GUI_CONTROL_PALETTE_PRESSED		]	= {};
			::gpk::array_static<uint32_t, ::gpk::GUI_CONTROL_COLOR_COUNT>			& colorComboSelected					= theme.ColorCombos[::gpk::GUI_CONTROL_PALETTE_SELECTED	]	= {};
			::gpk::array_static<uint32_t, ::gpk::GUI_CONTROL_COLOR_COUNT>			& colorComboHover 						= theme.ColorCombos[::gpk::GUI_CONTROL_PALETTE_HOVER		]	= {};
			::gpk::array_static<uint32_t, ::gpk::GUI_CONTROL_COLOR_COUNT>			& colorComboNormal						= theme.ColorCombos[::gpk::GUI_CONTROL_PALETTE_NORMAL		]	= {};
			colorComboDisabled	[::gpk::GUI_CONTROL_COLOR_BACKGROUND	]			= indexColor;
			colorComboDisabled	[::gpk::GUI_CONTROL_COLOR_CLIENT		]			= colorComboDisabled[::gpk::GUI_CONTROL_COLOR_BACKGROUND];
			colorComboNormal	[::gpk::GUI_CONTROL_COLOR_BACKGROUND	]			= indexColor;
			colorComboNormal	[::gpk::GUI_CONTROL_COLOR_CLIENT		]			= colorComboDisabled[::gpk::GUI_CONTROL_COLOR_BACKGROUND];
			colorComboSelected	[::gpk::GUI_CONTROL_COLOR_BACKGROUND	]			= indexColor;
			colorComboSelected	[::gpk::GUI_CONTROL_COLOR_CLIENT		]			= colorComboDisabled[::gpk::GUI_CONTROL_COLOR_BACKGROUND];
			colorComboHover		[::gpk::GUI_CONTROL_COLOR_BACKGROUND	]			= indexColor;
			colorComboHover		[::gpk::GUI_CONTROL_COLOR_CLIENT		]			= colorComboDisabled[::gpk::GUI_CONTROL_COLOR_BACKGROUND];
			colorComboPressed	[::gpk::GUI_CONTROL_COLOR_BACKGROUND	]			= indexColor;
			colorComboPressed	[::gpk::GUI_CONTROL_COLOR_CLIENT		]			= colorComboDisabled[::gpk::GUI_CONTROL_COLOR_BACKGROUND];
			//for(uint32_t iState = 0; iState < theme.ColorCombos.size(); ++iState) 
			//	for(uint32_t iArea = 0; iArea < ::gpk::GUI_CONTROL_COLOR_COUNT; ++iArea) 
			//		theme.ColorCombos[iState][iArea]									= ::gpk::min((uint32_t)theme.ColorCombos[iState][iArea], gui.Palette.size() - 1);
		}
		const uint32_t															paletteElemIndex						= linearIndex + offsetControl;
		palette.IdControls[paletteElemIndex]								= ::gpk::controlCreate(gui);
		::gpk::SControl															& control								= gui.Controls.Controls[palette.IdControls[paletteElemIndex]];
		control.ColorTheme													= themeIndex + 1;
		control.Area.Offset													= {(int16_t)(controlSize.x * x), (int16_t)(controlSize.y * y + offsetY)};
		control.Area.Size													= controlSize.Cast<int16_t>();
		::gpk::controlSetParent(gui, palette.IdControls[paletteElemIndex], palette.IdControl);
	}
	return 0;
}

