#include "menu.h"
#include "application.h"
#include "gpk_bitmap_file.h"
#include "gpk_bitmap_target.h"
#include "gpk_grid_copy.h"

//#define GPK_AVOID_LOCAL_APPLICATION_MODULE_MODEL_EXECUTABLE_RUNTIME
#include "gpk_app_impl.h"

GPK_DEFINE_APPLICATION_ENTRY_POINT(::gme::SApplication, "Module Explorer");
//
//			::gpk::error_t												setupMenu								
//	( ::gpk::SGUI													& gui
//	, ::gpk::SControlList											& menuControlList
//	, int32_t														parentControl
//	, ::gpk::ALIGN													menuAlign
//	, ::gpk::CONTROL_LIST_DIRECTION									direction
//	, const ::gpk::array_view<const ::gpk::view_const_string>		& menuOptions 
//	) { 
//
//	if(-1 == menuControlList.IdControl)
//		::gpk::controlListInitialize(gui, menuControlList);
//
//	menuControlList.Orientation												= direction;
//	int32_t																		idOptionList							= menuControlList.IdControl;
//	::gpk::SControl																& control								= gui.Controls.Controls		[idOptionList];
//	::gpk::SControlConstraints													& controlConstraints					= gui.Controls.Constraints	[idOptionList];
//	::gpk::SControlMode															& controlMode							= gui.Controls.Modes		[idOptionList];
//	control.Align															= menuAlign;	// Align should not be hardcoded
//	control.Margin = control.Border											= {};
//	controlConstraints.AttachSizeToControl.x								= (parentControl == -1) ? idOptionList : -1; 
//	controlConstraints.DockToControl.Bottom									= parentControl;
//	controlMode.Design														= true;
//	if(parentControl != -1) {
//		control.Area.Offset.x													= (parentControl == -1) ? -gui.Controls.Controls[parentControl].Area.Size.x : 0; 
//		controlConstraints.DockToControl.Left									= parentControl;
//	}
//
//	if(parentControl != -1) {
//		gui.Controls.States		[idOptionList].Hidden												= true;
//		gui.Controls.Text		[idOptionList].Text								= gui.Controls.Text[parentControl].Text;
//	}
//	//else {
//	//	gui.Controls.Text		[idOptionList].Text								= {};//"Menu";
//	//}
//
//	if(direction == ::gpk::CONTROL_LIST_DIRECTION_HORIZONTAL) 
//		control.Area															= {{}, {0, (int32_t)gui.FontCharSize.y + 4}};	// Height should not be hardcoded
//	else {
//		const ::gpk::SCoord2<int32_t>												parentOffset							= (parentControl != -1) ? gui.Controls.Controls[parentControl].Area.Offset : ::gpk::SCoord2<int32_t>{};
//		control.Area															= ::gpk::SRectangle2D<int32_t>{parentOffset, {100, }};	// Width should not be hardcoded
//	}
//
//	for(uint32_t iOption = 0; iOption < menuOptions.size(); ++iOption)
//		::gpk::controlListPush(gui, menuControlList, menuOptions[iOption]);
//
//	return 0;
//}

			::gpk::error_t												setupGUI								(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework								= app.Framework;
	::gpk::SGUI																	& gui									= framework.GUI;
	gui.ColorModeDefault													= ::gpk::GUI_COLOR_MODE_3D;
	gui.ThemeDefault														= 179; //::gpk::ASCII_COLOR_DARKGREEN * 16 + 5;

	// --- Setup desktop  
	app.Desktop.IdControl													= ::gpk::controlCreate(gui);
	//gui.Controls.Modes		[app.ControlDesktop].Design						= true;
	gui.Controls.Constraints[app.Desktop.IdControl].AttachSizeToControl		= {app.Desktop.IdControl, app.Desktop.IdControl};
	gui.Controls.Controls	[app.Desktop.IdControl].ColorTheme				= ::gpk::ASCII_COLOR_DARKGREY * 16 + 13;
	gui.Controls.Controls	[app.Desktop.IdControl].Border					= 
	gui.Controls.Controls	[app.Desktop.IdControl].Margin					= {};

	//app.Desktop.Menus.resize(::gme::APP_MENU_COUNT);
	//
	//// --- Setup "Main" menu
	//gpk_necall(::setupMenu(gui, app.Desktop.Menus[::gme::APP_MENU_MAIN], -1, ::gpk::ALIGN_CENTER_TOP, ::gpk::CONTROL_LIST_DIRECTION_HORIZONTAL, ::gme::g_MenuOptionsMain), "No known reason for this to fail other than memory corruption.");
	//gpk_necall(::setupMenu(gui, app.Desktop.Menus[::gme::APP_MENU_FILE], app.Desktop.Menus[::gme::APP_MENU_MAIN].IdControls[::gme::MENU_OPTION_MAIN_File], ::gpk::ALIGN_TOP_LEFT, ::gpk::CONTROL_LIST_DIRECTION_VERTICAL, ::gme::g_MenuOptionsFile), "No known reason for this to fail other than memory corruption.");
	//gpk_necall(::setupMenu(gui, app.Desktop.Menus[::gme::APP_MENU_EDIT], app.Desktop.Menus[::gme::APP_MENU_MAIN].IdControls[::gme::MENU_OPTION_MAIN_Edit], ::gpk::ALIGN_TOP_LEFT, ::gpk::CONTROL_LIST_DIRECTION_VERTICAL, ::gme::g_MenuOptionsEdit), "No known reason for this to fail other than memory corruption.");

	::gpk::SDesktop																& desktop								= app.Desktop;
	{
		::gpk::error_t																idMenu									= ::gpk::desktopCreateControlList(gui, desktop);
		desktop.Items.ControlLists[idMenu].Orientation							= ::gpk::CONTROL_LIST_DIRECTION_HORIZONTAL;
		for(uint32_t iOption = 0; iOption < ::gpk::size(::gme::g_MenuOptionsMain); ++iOption) {
			::gpk::controlListPush(gui, desktop.Items.ControlLists[idMenu], ::gme::g_MenuOptionsMain[iOption]);
		}
	}
	{
		::gpk::error_t																idMenu									= ::gpk::desktopCreateControlList(gui, desktop);
		desktop.Items.ControlLists[idMenu].Orientation							= ::gpk::CONTROL_LIST_DIRECTION_VERTICAL;
		uint32_t																	maxTextLength							= 0;
		for(uint32_t iOption = 0; iOption < ::gpk::size(::gme::g_MenuOptionsFile); ++iOption) {
			maxTextLength															= ::gpk::max(maxTextLength, ::gme::g_MenuOptionsFile[iOption].size());
			::gpk::controlListPush(gui, desktop.Items.ControlLists[idMenu], ::gme::g_MenuOptionsFile[iOption]);
		}
		gui.Controls.States[desktop.Items.ControlLists[idMenu].IdControl].Hidden = true;
		//gui.Controls.Controls[desktop.Items.ControlLists[idMenu].IdControl].Area.Size.x	= maxTextLength * gui.FontCharSize.x + ::gpk::controlNCSpacing(gui.Controls.Controls[desktop.Items.ControlLists[idMenu].IdControls[0]]).x;
		::gpk::desktopControlListSetParent(gui, desktop, idMenu, 0, 0);
	}

	{
		::gpk::error_t																idMenu									= ::gpk::desktopCreateControlList(gui, desktop);
		desktop.Items.ControlLists[idMenu].Orientation							= ::gpk::CONTROL_LIST_DIRECTION_VERTICAL;
		uint32_t																	maxTextLength							= 0;
		for(uint32_t iOption = 0; iOption < ::gpk::size(::gme::g_MenuOptionsNew); ++iOption) {
			maxTextLength															= ::gpk::max(maxTextLength, ::gme::g_MenuOptionsNew[iOption].size());
			::gpk::controlListPush(gui, desktop.Items.ControlLists[idMenu], ::gme::g_MenuOptionsNew[iOption]);
		}
		gui.Controls.States[desktop.Items.ControlLists[idMenu].IdControl].Hidden = true;
		//gui.Controls.Controls[desktop.Items.ControlLists[idMenu].IdControl].Area.Size.x	= maxTextLength * gui.FontCharSize.x + ::gpk::controlNCSpacing(gui.Controls.Controls[desktop.Items.ControlLists[idMenu].IdControls[0]]).x;
		::gpk::desktopControlListSetParent(gui, desktop, idMenu, 1, 0);
	}

	return 0;
}

			::gpk::error_t												cleanup									(::gme::SApplication & app)						{ return ::gpk::mainWindowDestroy(app.Framework.MainDisplay); }
			::gpk::error_t												setup									(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework								= app.Framework;
	::gpk::SDisplay																& mainWindow							= framework.MainDisplay;
	ree_if(0 == framework.Input.create(), "Out of memory?");
	error_if(errored(::gpk::mainWindowCreate(mainWindow, framework.RuntimeValues.PlatformDetail, framework.Input)), "Failed to create main window why?????!?!?!?!?");
	gpk_necall(::setupGUI(app), "Unknown error.");
	return 0; 
}

			::gpk::error_t												draw									(::gme::SApplication & app)						{ 
	::gpk::STimer																timer;
	::gpk::ptr_obj<::gpk::SRenderTarget>										target;
	target.create();
	target->Color		.resize(app.Framework.MainDisplay.Size);
	target->DepthStencil.resize(target->Color.View.metrics());
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

			::gpk::error_t												paintViewportCreate						(::gme::SApplication & app)						{ 
	::gpk::ptr_obj<::gpk::SRenderTarget>										newPaintScreen						= {};
	int32_t																		indexViewport						= -1;
	::gpk::SGUI																	& gui									= app.Framework.GUI;
	::gpk::SDesktop																& desktop								= app.Desktop;
	{
		::gme::mutex_guard															lock								(app.LockGUI);
		indexViewport															= ::gpk::desktopCreateViewport(gui, desktop);
		if(indexViewport >= (int32_t)app.PaintScreen.size()) {
			app.PaintScreen.resize(indexViewport + 1);
			newPaintScreen.create();
			app.PaintScreen[indexViewport]											= newPaintScreen;
		}
	}
	newPaintScreen															= app.PaintScreen[indexViewport];
	::gpk::SViewport															& viewportToSetUp					= desktop.Items.Viewports[indexViewport];
	newPaintScreen->Color		.resize(gui.Controls.Controls[viewportToSetUp.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Area.Size.Cast<uint32_t>());
	newPaintScreen->DepthStencil.resize(newPaintScreen->Color.View.metrics());
	::gpk::clearTarget(*newPaintScreen);
	{
		::gme::mutex_guard															lock								(app.LockGUI);
		gui.Controls.Controls[viewportToSetUp.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Image	= newPaintScreen->Color.View;
	}
	return 0;
}

			::gpk::error_t												paletteCreate							(::gme::SApplication & app)						{ 
 	::gpk::STexture<::gpk::SColorBGRA>											& paletteData							= *(app.PaletteColors[app.PaletteColors.push_back({})]).create();
	gpk_necall(paletteData.resize(16, 16), "Out of memory?");
	for(uint32_t iColor = 0; iColor < paletteData.Texels.size(); ++iColor)
		paletteData.Texels[iColor]												= {rand() & 0xFFU, rand() & 0xFFU, rand() & 0xFFU, 0xFFU};//gui.Palette[iColor];

	::gpk::SGUI																	& gui									= app.Framework.GUI;
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
		const ::gpk::SCoord2<int32_t>												vpNCSpacing							= ::gpk::controlNCSpacing(gui.Controls.Controls[viewportToSetUp.IdControl]);
		gui.Controls.Controls[viewportToSetUp.IdControl].Area.Size				= {256 + vpNCSpacing.x, 256 + vpNCSpacing.y + gui.Controls.Controls[viewportToSetUp.IdControls[::gpk::VIEWPORT_CONTROL_TITLE]].Area.Size.y};
		paletteGrid.IdControl													= viewportToSetUp.IdControls[::gpk::VIEWPORT_CONTROL_TARGET];
		gui.Controls.Controls[paletteGrid.IdControl].Area.Size					= {256, 256};
		::gpk::guiUpdateMetrics(gui, gui.LastSize, false);
		::gpk::paletteGridColorsSet(gui, paletteGrid, paletteData.View);
		::gpk::controlDelete(gui, oldPaletteControl);
	}
	return 0;
}

			::gpk::error_t												update									(::gme::SApplication & app, bool exitSignal)	{ 
	::gpk::STimer																timer;
	retval_info_if(::gpk::APPLICATION_STATE_EXIT, exitSignal, "Exit requested by runtime.");
	{
		::gme::mutex_guard															lock									(app.LockRender);
		app.Framework.MainDisplayOffscreen										= app.Offscreen;
	}
	::gpk::SFramework															& framework								= app.Framework;
	retval_info_if(::gpk::APPLICATION_STATE_EXIT, ::gpk::APPLICATION_STATE_EXIT == ::gpk::updateFramework(app.Framework), "Exit requested by framework update.");

	::gpk::SGUI																	& gui									= framework.GUI;
	int32_t																		hoveredControl							= -1;
	::gpk::SInput																& input									= *app.Framework.Input;
	{
		::gme::mutex_guard															lock									(app.LockGUI);
		hoveredControl															= ::gpk::guiProcessInput(gui, input);
	}

	if(input.MouseCurrent.Deltas.z) {
		gui.Zoom.ZoomLevel														+= input.MouseCurrent.Deltas.z * (1.0f / (120 * 4));
		::gpk::guiUpdateMetrics(gui, framework.MainDisplay.Size, true);
	}

	::gpk::array_pod<uint32_t>														controlsToProcess					= {};
	::gpk::guiGetProcessableControls(gui, controlsToProcess);

	::gpk::SDesktop																	& desktop							= app.Desktop;
	bool																			inControlArea						= false;
	//bool																			anyControlPressed					= false;
	for(uint32_t iControlToProcess = 0, countControls = controlsToProcess.size(); iControlToProcess < countControls; ++iControlToProcess) {
		const uint32_t																	iControl							= controlsToProcess[iControlToProcess];
		const ::gpk::SControlState														& controlState						= gui.Controls.States[iControl];
		const ::gpk::SControlMetrics													& controlMetrics					= gui.Controls.Metrics[iControl];
		if(iControl != (uint32_t)desktop.IdControl)
			inControlArea																= inControlArea || ::gpk::in_range(gui.CursorPos.Cast<int32_t>(), controlMetrics.Total.Global);

		//if(controlState.Pressed && (int32_t)iControl != desktop.IdControl)
		//	anyControlPressed															= true;

		if(controlState.Execute) {
			info_printf("Executed %u.", iControl);
			desktop.SelectedMenu													= -1;
			for(uint32_t iMenu = 0, countMenus = (uint32_t)::gpk::max(0, (int32_t)desktop.Menus.size() - 1); iMenu < countMenus; ++iMenu) {
				if(false == ::gpk::in_range(gui.CursorPos.Cast<int32_t>(), gui.Controls.Metrics[desktop.Menus[::gme::APP_MENU_MAIN].IdControls[iMenu]].Total.Global)) 
					gui.Controls.States[desktop.Menus[iMenu + 1].IdControl].Hidden			= true;
			}
		}
	}
	if(desktop.Menus.size()) {
			 if(gui.Controls.States[desktop.Menus[::gme::APP_MENU_FILE].IdControls[::gme::MENU_OPTION_FILE_Exit		]].Execute)	return ::gpk::APPLICATION_STATE_EXIT;
		else if(gui.Controls.States[desktop.Menus[::gme::APP_MENU_FILE].IdControls[::gme::MENU_OPTION_FILE_New		]].Execute)	::paintViewportCreate	(app);
		else if(gui.Controls.States[desktop.Menus[::gme::APP_MENU_EDIT].IdControls[::gme::MENU_OPTION_EDIT_Palette	]].Execute)	::paletteCreate			(app);
	}

	for(uint32_t iMenu = 0, countMenus = (uint32_t)::gpk::max(0, (int32_t)desktop.Menus.size() - 1); iMenu < countMenus; ++iMenu) {
		::gpk::SControlState															& controlState						= gui.Controls.States[desktop.Menus[::gme::APP_MENU_MAIN].IdControls[iMenu]];
		if(controlState.Hover) {
			controlState.Hidden												= false;
			if(controlState.Execute) 
				desktop.SelectedMenu													= iMenu;
		}
		else {
			const ::gpk::SControlMetrics													& controlListMetrics				= gui.Controls.Metrics[desktop.Menus[iMenu + 1].IdControl];
			if(::gpk::in_range(gui.CursorPos.Cast<int32_t>(), controlListMetrics.Total.Global) && controlState.Hidden == false)
				controlState.Hidden												= false;
			else if(desktop.SelectedMenu != (int32_t)iMenu)
				controlState.Hidden												= true;
		}
	}

	for(uint32_t iPalette = 0; iPalette < desktop.Items.PaletteGrids.size(); ++iPalette) {
		if(desktop.Items.PaletteGrids.Unused[iPalette])
			continue;

		::gpk::SPaletteGrid															& paletteControl									= desktop.Items.PaletteGrids[iPalette];
		if(paletteControl.IdControl != -1) 
			for(uint32_t iColorControl = 0, colorControlStop = paletteControl.IdControls.size(); iColorControl < colorControlStop; ++iColorControl) {
				if(gui.Controls.States[(uint32_t)paletteControl.IdControls[iColorControl]].Execute) {
					app.ColorPaint															= paletteControl.Colors[(iColorControl) / paletteControl.Colors.metrics().x][(iColorControl) % paletteControl.Colors.metrics().x];
					break;
				}
			}
	}

	{
		::gme::mutex_guard																lock								(app.LockGUI);
		::gpk::desktopUpdate(gui, desktop, input);
	}

	if(input.ButtonDown(1) || input.ButtonDown(2)) {
		desktop.SelectedMenu														= -1;
		for(uint32_t iMenu = 0, countMenus = (uint32_t)::gpk::max(0, (int32_t)desktop.Menus.size() - 1); iMenu < countMenus; ++iMenu) 
			gui.Controls.States[desktop.Menus[iMenu + 1].IdControl].Hidden			= true;
	}

	if(false == inControlArea) {
		if(input.ButtonDown(0)) {
			desktop.SelectedMenu														= -1;
			for(uint32_t iMenu = 0, countMenus = (uint32_t)::gpk::max(0, (int32_t)desktop.Menus.size() - 1); iMenu < countMenus; ++iMenu) 
				gui.Controls.States[desktop.Menus[iMenu + 1].IdControl].Hidden			= true;
		}
	}

	for(uint32_t iViewport = 0; iViewport < desktop.Items.Viewports.size(); ++iViewport) {
		::gpk::SViewport																& currentViewport					= desktop.Items.Viewports[iViewport];
		if(desktop.Items.Viewports.Unused[iViewport] || currentViewport.ControlType != -1)
			continue;

		//if(gui.Controls.Metrics[currentViewport.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Client.Global.Size != gui.Controls.Controls[currentViewport.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Image.metrics().Cast<int32_t>()) {
		//	app.PaintScreen[iViewport]->Color		.resize(gui.Controls.Metrics[currentViewport.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Client.Global.Size.Cast<uint32_t>());
		//	app.PaintScreen[iViewport]->DepthStencil.resize(gui.Controls.Metrics[currentViewport.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Client.Global.Size.Cast<uint32_t>());
		//	gui.Controls.Controls[currentViewport.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Image = app.PaintScreen[iViewport]->Color.View;
		//}

		int32_t																			idTarget							= currentViewport.IdControls[::gpk::VIEWPORT_CONTROL_TARGET];
		const ::gpk::SCoord2<int32_t>													paintOffset							= gui.Controls.Metrics[idTarget].Client.Global.Offset;
		/*if(::gpk::in_range(gui.CursorPos.Cast<int32_t>(), {paintOffset, (paintOffset + gui.Controls.Metrics[currentViewport.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Client.Global.Size)}))*/ {
			const ::gpk::SCoord2<int32_t>													mouseDeltas							= {input.MouseCurrent.Deltas.x, input.MouseCurrent.Deltas.y};
			if(gui.Controls.States[idTarget].Pressed) {
				if(mouseDeltas.LengthSquared()) {
					const ::gpk::SLine2D<int32_t>													lineToDraw							= {gui.CursorPos.Cast<int32_t>() - paintOffset - mouseDeltas, gui.CursorPos.Cast<int32_t>() - paintOffset};
					::gpk::array_pod<::gpk::SCoord2<int32_t>>										pointsToDraw;
					//::gpk::drawLine(app.PaintScreen->Color.View, ::gpk::SColorBGRA{::gpk::YELLOW}, lineToDraw);
					::gpk::SRenderTarget															* paintScreen						= app.PaintScreen[iViewport];
					::gpk::drawLine(paintScreen->Color.View.metrics(), lineToDraw, pointsToDraw);
					for(uint32_t iPoint = 0; iPoint < pointsToDraw.size(); ++iPoint) 
						::gpk::drawPixelBrightness(paintScreen->Color.View, pointsToDraw[iPoint], app.ColorPaint, 0.1f, 5.0);
				}
				else if(input.ButtonDown(0)) {
					::gpk::SCoord2<int32_t>															mousePos							= {input.MouseCurrent.Position.x, input.MouseCurrent.Position.y};
					mousePos																	-= paintOffset;
					::gpk::drawPixelBrightness(app.PaintScreen[iViewport]->Color.View, mousePos, app.ColorPaint, 0.1f, 5.0);
				}
			}
		}
	}
	static char frameRatebuffer[128] = {};
	sprintf_s(frameRatebuffer, "Frames per second (update) : %f", framework.FrameInfo.Seconds.LastFrame);
	gui.Controls.Text[0].Align	= ::gpk::ALIGN_BOTTOM_LEFT;
	gui.Controls.Text[0].Text	= frameRatebuffer;
 	//timer.Frame();
	//info_printf("Update time: %f.", (float)timer.LastTimeSeconds);
	return 0; 
}
