#include "application.h"
#include "gpk_png.h"
#include "gpk_bitmap_target.h"
#include "gpk_grid_copy.h"

//#define GPK_AVOID_LOCAL_APPLICATION_MODULE_MODEL_EXECUTABLE_RUNTIME
#include "gpk_app_impl.h"

#include "menu_main.h"

GPK_DEFINE_APPLICATION_ENTRY_POINT(::gme::SApplication, "Module Explorer");

static		::gpk::error_t												setupDesktop							(::gpk::SGUI & gui, ::gpk::SDesktop & desktop)		{ 
	// --- Setup desktop  
	desktop.IdControl														= ::gpk::controlCreate(gui);
	gui.Controls.States		[desktop.IdControl].Design						= true;
	gui.Controls.Constraints[desktop.IdControl].AttachSizeToControl			= {desktop.IdControl, desktop.IdControl};
	gui.Controls.Controls	[desktop.IdControl].ColorTheme					= ::gpk::ASCII_COLOR_DARKGREY * 16 + 13;
	gui.Controls.Controls	[desktop.IdControl].Border						= 
	gui.Controls.Controls	[desktop.IdControl].Margin						= {};
	return 0;
}

			::gpk::error_t												setupGUI								(::gme::SApplication & app)							{ 
	::gpk::SFramework															& framework								= app.Framework;
	::gpk::SGUI																	& gui									= framework.GUI;
	gui.ColorModeDefault													= ::gpk::GUI_COLOR_MODE_3D;
	gui.ThemeDefault														= 179; //::gpk::ASCII_COLOR_DARKGREEN * 16 + 5;

	::gpk::SDesktop																& desktop								= app.Desktop;
	::setupDesktop(gui, desktop);
	::gpk::error_t																idMenuMain								= ::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsMain, -1, -1);
	::gpk::error_t																idMenuFile								= ::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsFile, idMenuMain, ::gme::getMenuItemIndex(::gme::g_MenuOptionsMain, "File"));
		::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsNew	, idMenuFile, ::gme::getMenuItemIndex(::gme::g_MenuOptionsFile, "New" ));
		::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsOpen	, idMenuFile, ::gme::getMenuItemIndex(::gme::g_MenuOptionsFile, "Open"));
		::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsSave	, idMenuFile, ::gme::getMenuItemIndex(::gme::g_MenuOptionsFile, "Save"));
	::gpk::error_t idEdit = ::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsEdit	, idMenuMain, ::gme::getMenuItemIndex(::gme::g_MenuOptionsMain, "Edit")); idEdit;
	::gpk::error_t idHelp = ::gme::setupMenu(gui, desktop, ::gme::g_MenuOptionsHelp	, idMenuMain, ::gme::getMenuItemIndex(::gme::g_MenuOptionsMain, "Help")); idHelp;
	return 0;
}

			::gpk::error_t												cleanup									(::gme::SApplication & app)							{ return ::gpk::mainWindowDestroy(app.Framework.MainDisplay); }
			::gpk::error_t												setup									(::gme::SApplication & app)							{ 
	::gpk::SFramework															& framework								= app.Framework;
	::gpk::SDisplay																& mainWindow							= framework.MainDisplay;
	ree_if(0 == framework.Input.create(), "%s", "Out of memory?");
	error_if(errored(::gpk::mainWindowCreate(mainWindow, framework.RuntimeValues.PlatformDetail, framework.Input)), "Failed to create main window why?????!?!?!?!?");
	gpk_necall(::setupGUI(app), "%s", "Unknown error.");
	return 0; 
}

			::gpk::error_t												draw									(::gme::SApplication & app)							{ 
	::gpk::STimer																timer;
	::gpk::ptr_obj<::gpk::SRenderTarget<::gpk::SColorBGRA, uint32_t>>			target;
	target.create();
	target->resize(app.Framework.MainDisplay.Size, ::gpk::DARKBLUE / 2.0, 0xFFFFFFFF);
	//::gpk::clearTarget(*target);
	{
		::gme::mutex_guard															lock									(app.LockGUI);
		::gpk::guiDraw(app.Framework.GUI, target->Color.View);
	}
	{
		::gme::mutex_guard															lock									(app.LockRender);
		app.Offscreen															= target;
	}
	//timer.Frame();
	//info_printf("Draw time: %f.", (float)timer.LastTimeSeconds);
	return 0; 
}

			::gpk::error_t												paintViewportCreate						(::gme::SApplication & app)							{ 
	::gpk::ptr_obj<::gpk::SImage<::gpk::SColorBGRA>>							newPaintScreen							= {};
	int32_t																		indexViewport							= -1;
	::gpk::SGUI																	& gui									= app.Framework.GUI;
	::gpk::SDesktop																& desktop								= app.Desktop;
	{
		::gme::mutex_guard															lock								(app.LockGUI);
		indexViewport															= ::gpk::desktopCreateViewport(gui, desktop);
		if(indexViewport >= (int32_t)app.EditorsImage.size()) {
			app.EditorsImage.resize(indexViewport + 1);
			newPaintScreen.create();
			app.EditorsImage[indexViewport].PaintScreen								= newPaintScreen;
		}
	}
	newPaintScreen															= app.EditorsImage[indexViewport].PaintScreen;
	::gpk::SViewport															& viewportToSetUp						= desktop.Items.Viewports[indexViewport];
	gui.Controls.Controls[viewportToSetUp.IdControl].Area.Offset			= gui.CursorPos.Cast<int16_t>();
	gui.Controls.Controls[viewportToSetUp.IdControl].Area.Offset			+= ::gpk::SCoord2<int16_t>{20, 20};
	{
		::gme::mutex_guard															lock								(app.LockGUI);
		gpk_necall(::gme::contextEditorImageInitialize(gui, app.EditorsImage[indexViewport], viewportToSetUp.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]), "");
	}
	gpk_necall(newPaintScreen->resize(gui.Controls.Controls[viewportToSetUp.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Area.Size.Cast<uint32_t>()), "Out of memory?");
	memset(newPaintScreen->Texels.begin(), 0, newPaintScreen->Texels.size() * sizeof(::gpk::SColorBGRA));
	{
		::gme::mutex_guard															lock								(app.LockGUI);
		gui.Controls.Controls[viewportToSetUp.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Image	= newPaintScreen->View;
	}
	return 0;
}

			::gpk::error_t												paletteCreate							(::gme::SApplication & app)							{ 
 	::gpk::SImage<::gpk::SColorBGRA>											& paletteData							= *(app.PaletteColors[app.PaletteColors.push_back({})]).create();
	gpk_necall(paletteData.resize(16, 16), "Out of memory?");
	::gpk::SGUI																	& gui									= app.Framework.GUI;
	::gpk::SColorBGRA															colorsBase	[6]							= 
		{ ::gpk::RED
		, ::gpk::YELLOW
		, ::gpk::GREEN
		, ::gpk::CYAN
		, ::gpk::BLUE
		, ::gpk::MAGENTA
		};
	static constexpr	const double											scaleColorBase							= (16.0 / 6.0);
	uint32_t																	indicesColorBase	[6]					= {};
	for(uint32_t iColor = 0; iColor < paletteData.View.metrics().x; ++iColor) {
		if((uint32_t)((iColor) / scaleColorBase) >= 6)
			break;
		indicesColorBase[(uint32_t)((iColor) / scaleColorBase)]	= iColor;	
	}
	for(uint32_t iColor = 0; iColor < paletteData.View.metrics().x; ++iColor) {
		//paletteData.Texels[iColor]												= gui.Palette[iColor + 128];//{rand() & 0xFFU, rand() & 0xFFU, rand() & 0xFFU, 0xFFU};//gui.Palette[iColor];
		uint32_t																	indexColorBase	= (uint32_t)((iColor) / scaleColorBase) % 6;	
		::gpk::SMinMax<uint32_t>													minMax			= {::gpk::min(indicesColorBase[indexColorBase], indicesColorBase[(indexColorBase + 1) % 6]), ::gpk::max(indicesColorBase[indexColorBase], indicesColorBase[(indexColorBase + 1) % 6])};
		double f = (iColor % (minMax.Max - minMax.Min)) / double(minMax.Max - minMax.Min);
		::gpk::SColorBGRA															color0			= colorsBase[indexColorBase];
		::gpk::SColorBGRA															color1			= colorsBase[(indexColorBase + 1) % 6];
		paletteData.Texels[iColor * 16]											= ::gpk::interpolate_linear(color0, color1, f);//color0;//{rand() & 0xFFU, rand() & 0xFFU, rand() & 0xFFU, 0xFFU};//gui.Palette[iColor];
	}

	::gpk::SDesktop																& desktop								= app.Desktop;
	{
		::gme::mutex_guard															lock								(app.LockGUI);
		const int32_t																indexPalette						= ::gpk::desktopCreatePaletteGrid(gui, desktop);
		const int32_t																indexViewport						= ::gpk::desktopCreateViewport(gui, desktop);
		::gpk::SPaletteGrid															& paletteGrid						= desktop.Items.PaletteGrids[indexPalette];
		::gpk::SViewport															& viewportToSetUp					= desktop.Items.Viewports[indexViewport];
		viewportToSetUp.ControlType												= ::gpk::GUI_CONTROL_TYPE_PaletteGrid;
		viewportToSetUp.IdDesktopElement										= indexPalette;
		const int32_t																oldPaletteControl					= paletteGrid.IdControl;
		const ::gpk::SCoord2<int16_t>												vpNCSpacing							= ::gpk::controlNCSpacing(gui.Controls.Controls[viewportToSetUp.IdControl]);
		gui.Controls.Controls[viewportToSetUp.IdControl].Area.Size				= {256 + vpNCSpacing.x, 256 + vpNCSpacing.y + gui.Controls.Controls[viewportToSetUp.IdControls[::gpk::VIEWPORT_CONTROL_TITLE]].Area.Size.y};
		paletteGrid.IdControl													= viewportToSetUp.IdControls[::gpk::VIEWPORT_CONTROL_TARGET];
		gui.Controls.Controls[paletteGrid.IdControl].Area.Size					= {256, 256};
		gpk_necall(::gpk::guiUpdateMetrics		(gui, gui.LastSize, false)			, "Unknown.");
		gpk_necall(::gpk::paletteGridColorsSet	(gui, paletteGrid, paletteData.View), "Unknown.");
		gpk_necall(::gpk::controlDelete			(gui, oldPaletteControl)			, "Unknown.");
	}
	return 0;
}

			::gpk::error_t												update									(::gme::SApplication & app, bool exitSignal)		{ 
	::gpk::STimer																timer;
	retval_info_if(::gpk::APPLICATION_STATE_EXIT, exitSignal, "Exit requested by runtime.");
	{
		::gme::mutex_guard															lock									(app.LockRender);
		app.Framework.MainDisplayOffscreen										= app.Offscreen;
	}
	::gpk::SFramework															& framework								= app.Framework;
	retval_info_if(::gpk::APPLICATION_STATE_EXIT, ::gpk::APPLICATION_STATE_EXIT == ::gpk::updateFramework(app.Framework), "%s", "Exit requested by framework update.");

	::gpk::SGUI																	& gui									= framework.GUI;
	int32_t																		hoveredControl							= -1;
	::gpk::SInput																& input									= *app.Framework.Input;
	{
		::gme::mutex_guard															lock									(app.LockGUI);
		hoveredControl															= ::gpk::guiProcessInput(gui, input);
	}

	if(input.MouseCurrent.Deltas.z) {
		::gme::mutex_guard															lock									(app.LockGUI);
		gui.Zoom.ZoomLevel														+= input.MouseCurrent.Deltas.z * (1.0f / (120 * 4));
		::gpk::guiUpdateMetrics(gui, framework.MainDisplay.Size, true);
	}

	::gpk::array_pod<uint32_t>													controlsToProcess					= {};
	error_if(errored(::gpk::guiGetProcessableControls(gui, controlsToProcess)), "Floers.");

	::gpk::SDesktop																& desktop							= app.Desktop;
	for(uint32_t iPalette = 0; iPalette < desktop.Items.PaletteGrids.size(); ++iPalette) {
		if(desktop.Items.PaletteGrids.Unused[iPalette])
			continue;

		::gpk::SPaletteGrid															& paletteControl					= desktop.Items.PaletteGrids[iPalette];
		if(paletteControl.IdControl != -1) 
			for(uint32_t iColorControl = 0, colorControlStop = paletteControl.IdControls.size(); iColorControl < colorControlStop; ++iColorControl) {
				if(gui.Controls.States[(uint32_t)paletteControl.IdControls[iColorControl]].Execute) {			// Set the color clicked on the palette.
					for(uint32_t iContextImage = 0; iContextImage < app.EditorsImage.size(); ++iContextImage) 
						app.EditorsImage[iContextImage].ColorPaint								= paletteControl.Colors[(iColorControl) / paletteControl.Colors.metrics().x][(iColorControl) % paletteControl.Colors.metrics().x];
					break;
				}
			}
	}

	int64_t																		desktopEvent						= 0;
	{
		::gme::mutex_guard															lock								(app.LockGUI);
		desktopEvent															= ::gpk::desktopUpdate(gui, desktop, input);
	}
	if(desktopEvent)
		switch(desktopEvent) {
		case ::gme::APP_MENU_EVENT_NONE					: break;
		case ::gme::APP_MENU_EVENT_EXIT					: return 1;
		case ::gme::APP_MENU_EVENT_NEW_PALETTE			: ::paletteCreate		(app);	break;
		case ::gme::APP_MENU_EVENT_NEW_CONTEXT_IMAGE	: ::paintViewportCreate	(app);	break;
		}

	for(uint32_t iViewport = 0; iViewport < desktop.Items.Viewports.size(); ++iViewport) {
		::gpk::SViewport															& currentViewport					= desktop.Items.Viewports[iViewport];
		if(desktop.Items.Viewports.Unused[iViewport] || currentViewport.ControlType != -1)
			continue;
		int32_t																		idTarget							= currentViewport.IdControls[::gpk::VIEWPORT_CONTROL_TARGET];
		{
			::gme::mutex_guard															lock								(app.LockGUI);
			::gme::contextEditorImageUpdate(gui, app.EditorsImage[iViewport], input);
			if(-1 == app.EditorsImage[iViewport].Desktop.IdControl) {
				::gpk::desktopDeleteViewport(gui, desktop, iViewport);
				continue;
			}
		}
		//const ::gpk::SCoord2<int32_t>													& currentViewportClientSize			= gui.Controls.Metrics[idTarget].Client.Global.Size;
		//if(currentViewportClientSize != gui.Controls.Controls[idTarget].Image.metrics().Cast<int32_t>()) {
		//	app.EditorsImage[iViewport].PaintScreen->resize(currentViewportClientSize.Cast<uint32_t>());
		//	app.EditorsImage[iViewport].PaintScreen->resize(currentViewportClientSize.Cast<uint32_t>());
		//	gui.Controls.Controls[idTarget].Image										= app.EditorsImage[iViewport].PaintScreen->View;
		//}
		const ::gpk::SCoord2<int32_t>												paintOffset							= gui.Controls.Metrics[idTarget].Client.Global.Offset;
		if(gui.Controls.States[idTarget].Pressed) {
			const ::gpk::SCoord2<int32_t>												mouseDeltas							= {input.MouseCurrent.Deltas.x, input.MouseCurrent.Deltas.y};
			::gme::SContextEditorImage													contextImage						= app.EditorsImage[iViewport];
			if(mouseDeltas.LengthSquared()) {
				const ::gpk::SLine2D<int32_t>												lineToDraw							= {gui.CursorPos.Cast<int32_t>() - paintOffset - mouseDeltas, gui.CursorPos.Cast<int32_t>() - paintOffset};
				::gpk::array_pod<::gpk::SCoord2<int32_t>>									pointsToDraw;
				//::gpk::drawLine(app.PaintScreen->Color.View, ::gpk::SColorBGRA{::gpk::YELLOW}, lineToDraw);
				::gpk::drawLine(contextImage.PaintScreen->View.metrics(), lineToDraw, pointsToDraw);
				for(uint32_t iPoint = 0; iPoint < pointsToDraw.size(); ++iPoint) 
					::gpk::drawPixelBrightness(contextImage.PaintScreen->View, pointsToDraw[iPoint], contextImage.ColorPaint, 0.1f, 5.0);
			}
			else if(input.ButtonDown(0)) {
				::gpk::SCoord2<int32_t>														mousePos							= {input.MouseCurrent.Position.x, input.MouseCurrent.Position.y};
				mousePos																-= paintOffset;
				::gpk::drawPixelBrightness(contextImage.PaintScreen->View, mousePos, contextImage.ColorPaint, 0.1f, 5.0);
			}
		}
	}
	static char																	frameRatebuffer	[128]				= {};
	sprintf_s(frameRatebuffer, "Frames per second (update) : %f", framework.FrameInfo.Seconds.LastFrame);
	gui.Controls.Text[0].Align												= ::gpk::ALIGN_BOTTOM_LEFT;
	gui.Controls.Text[0].Text												= frameRatebuffer;
 	//timer.Frame();
	//info_printf("Update time: %f.", (float)timer.LastTimeSeconds);
	return 0; 
}
