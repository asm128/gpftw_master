#include "menu.h"
#include "application.h"
#include "gpk_bitmap_file.h"
#include "gpk_bitmap_target.h"
#include "gpk_grid_copy.h"

#define GPK_AVOID_LOCAL_APPLICATION_MODULE_MODEL_EXECUTABLE_RUNTIME
#include "gpk_app_impl.h"

GPK_DEFINE_APPLICATION_ENTRY_POINT(::gme::SApplication, "Module Explorer");

			::gpk::error_t												setupMenu								
	( ::gpk::SGUI													& gui
	, ::gpk::SControlList											& menuControlList
	, int32_t														parentControl
	, ::gpk::ALIGN													menuAlign
	, ::gpk::CONTROL_LIST_DIRECTION									direction
	, const ::gpk::view_array<const ::gpk::view_const_string>		& menuOptions 
	) { 
	::gpk::controlListInitialize(gui, menuControlList);
	menuControlList.Orientation												= direction;
	int32_t																		idOptionList							= menuControlList.IdControl;
	::gpk::SControl																& control								= gui.Controls.Controls		[idOptionList];
	::gpk::SControlConstraints													& controlConstraints					= gui.Controls.Constraints	[idOptionList];
	control.Align															= menuAlign;	// Align should not be hardcoded
	control.Margin = control.Border											= {};
	controlConstraints.AttachSizeToControl.x								= (parentControl == -1) ? idOptionList : -1; 
	controlConstraints.DockToControl.Bottom									= parentControl;
	gui.Controls.Modes		[idOptionList].Design							= true;
	if(parentControl != -1) {
		control.Area.Offset.x													= (parentControl == -1) ? -gui.Controls.Controls[parentControl].Area.Size.x : 0; 
		gui.Controls.Constraints[idOptionList].DockToControl.Left				= parentControl;
	}

	if(parentControl != -1) {
		gui.Controls.States		[idOptionList].Hidden							= true;
		gui.Controls.Text		[idOptionList].Text								= gui.Controls.Text[parentControl].Text;
	}
	//else {
	//	gui.Controls.Text		[idOptionList].Text								= {};//"Menu";
	//}

	if(direction == ::gpk::CONTROL_LIST_DIRECTION_HORIZONTAL) 
		control.Area															= {{}, {0, (int32_t)gui.FontCharSize.y + 4}};	// Height should not be hardcoded
	else {
		const ::gpk::SCoord2<int16_t>												parentOffset							= (parentControl != -1) ? gui.Controls.Controls[parentControl].Area.Offset : ::gpk::SCoord2<int16_t>{};
		control.Area															= ::gpk::SRectangle2D<int16_t>{parentOffset, {100, }};	// Width should not be hardcoded
	}

	for(uint32_t iOption = 0; iOption < menuOptions.size(); ++iOption)
		::gpk::controlListPush(gui, menuControlList, menuOptions[iOption]);

	return 0;
}

			::gpk::error_t												setupGUI								(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework								= app.Framework;
	::gpk::SGUI																	& gui									= *framework.GUI;
	gui.ColorModeDefault													= ::gpk::GUI_COLOR_MODE_3D;
	gui.ThemeDefault														= ::gpk::ASCII_COLOR_DARKGREEN * 16 + 5;

	// --- Setup desktop  
	app.Desktop.IdControl													= ::gpk::controlCreate(gui);
	//gui.Controls.Modes		[app.ControlDesktop].Design						= true;
	gui.Controls.Constraints[app.Desktop.IdControl].AttachSizeToControl		= {app.Desktop.IdControl, app.Desktop.IdControl};
	gui.Controls.Controls	[app.Desktop.IdControl].ColorTheme					= ::gpk::ASCII_COLOR_DARKGREY * 16 + 13;

	app.Menus.resize(::gme::APP_MENU_COUNT);

	// --- Setup "Main" menu
	gpk_necall(setupMenu(gui, app.Menus[::gme::APP_MENU_MAIN], -1, ::gpk::ALIGN_CENTER_TOP, ::gpk::CONTROL_LIST_DIRECTION_HORIZONTAL, ::gme::g_MenuOptionsMain), "%s", "No known reason for this to fail other than memory corruption.");
	gpk_necall(setupMenu(gui, app.Menus[::gme::APP_MENU_FILE], app.Menus[::gme::APP_MENU_MAIN].IdControls[::gme::MENU_OPTION_MAIN_File], ::gpk::ALIGN_TOP_LEFT, ::gpk::CONTROL_LIST_DIRECTION_VERTICAL, ::gme::g_MenuOptionsFile), "%s", "No known reason for this to fail other than memory corruption.");
	gpk_necall(setupMenu(gui, app.Menus[::gme::APP_MENU_EDIT], app.Menus[::gme::APP_MENU_MAIN].IdControls[::gme::MENU_OPTION_MAIN_Edit], ::gpk::ALIGN_TOP_LEFT, ::gpk::CONTROL_LIST_DIRECTION_VERTICAL, ::gme::g_MenuOptionsEdit), "%s", "No known reason for this to fail other than memory corruption.");
	return 0;
}

			::gpk::error_t												cleanup									(::gme::SApplication & app)						{ return ::gpk::mainWindowDestroy(app.Framework.MainDisplay); }
			::gpk::error_t												setup									(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework								= app.Framework;
	::gpk::SDisplay																& mainWindow							= framework.MainDisplay;
	ree_if(0 == framework.Input.create(), "%s", "Out of memory?");
	error_if(errored(::gpk::mainWindowCreate(mainWindow, framework.RuntimeValues.PlatformDetail, framework.Input)), "Failed to create main window why?????!?!?!?!?");
	gpk_necall(setupGUI(app), "%s", "Unknown error.");
	return 0; 
}

			::gpk::error_t												draw									(::gme::SApplication & app)						{ 
	::gpk::STimer																timer;
	::gpk::ptr_obj<::gpk::SRenderTarget<::gpk::SColorBGRA, uint32_t>>										target;
	target.create();
	target->Color		.resize(app.Framework.MainDisplay.Size);
	target->DepthStencil.resize(target->Color.View.metrics());
	//::gpk::clearTarget(*target);
	{
		::gme::mutex_guard															lock									(app.LockGUI);
		::gpk::guiDraw(*app.Framework.GUI, target->Color.View);
	}
	{
		::gme::mutex_guard															lock									(app.LockRender);
		app.Offscreen															= target;
	}
	//timer.Frame();
	//info_printf("Draw time: %f.", (float)timer.LastTimeSeconds);
	return 0; 
}

			::gpk::error_t												update									(::gme::SApplication & app, bool exitSignal)	{ 
	::gpk::STimer																timer;
	retval_info_if(::gpk::APPLICATION_STATE_EXIT, exitSignal, "%s", "Exit requested by runtime.");
	{
		::gme::mutex_guard															lock									(app.LockRender);
		app.Framework.MainDisplayOffscreen										= app.Offscreen;
	}
	::gpk::SFramework															& framework								= app.Framework;
	retval_info_if(::gpk::APPLICATION_STATE_EXIT, ::gpk::APPLICATION_STATE_EXIT == ::gpk::updateFramework(app.Framework), "%s", "Exit requested by framework update.");

	::gpk::SGUI																	& gui									= *framework.GUI;
	{
		::gme::mutex_guard															lock									(app.LockGUI);
		::gpk::guiProcessInput(gui, *app.Framework.Input);
	}
	if(app.Framework.Input->MouseCurrent.Deltas.z) {
		gui.Zoom.ZoomLevel														+= app.Framework.Input->MouseCurrent.Deltas.z * (1.0f / (120 * 4));
		::gpk::guiUpdateMetrics(gui, framework.MainDisplay.Size, true);
	}

	::gpk::array_pod<uint32_t>													controlsToProcess						= {};
	::gpk::guiGetProcessableControls(gui, controlsToProcess);

	::gpk::SDesktop																& desktop								= app.Desktop;
	bool																		inControlArea							= false;
	bool																		anyControlPressed						= false;
	for(uint32_t iControlToProcess = 0, countControls = controlsToProcess.size(); iControlToProcess < countControls; ++iControlToProcess) {
		const uint32_t																iControl								= controlsToProcess[iControlToProcess];
		const ::gpk::SControlState													& controlState							= gui.Controls.States[iControl];
		const ::gpk::SControlMetrics												& controlMetrics						= gui.Controls.Metrics[iControl];
		if(iControl != (uint32_t)desktop.IdControl)
			inControlArea															= inControlArea || ::gpk::in_range(gui.CursorPos.Cast<int32_t>(), controlMetrics.Total.Global);

		if(controlState.Pressed && (int32_t)iControl != desktop.IdControl)
			anyControlPressed															= true;

		if(controlState.Execute) {
			info_printf("Executed %u.", iControl);
			app.SelectedMenu														= -1;
			for(uint32_t iMenu = 0, countMenus = app.Menus.size() - 1; iMenu < countMenus; ++iMenu) {
				if(false == ::gpk::in_range(gui.CursorPos.Cast<int32_t>(), gui.Controls.Metrics[app.Menus[::gme::APP_MENU_MAIN].IdControls[iMenu]].Total.Global)) 
					gui.Controls.States[app.Menus[iMenu + 1].IdControl].Hidden				= true;
			}
		}
		if(iControl == (uint32_t)app.Menus[::gme::APP_MENU_FILE].IdControls[::gme::MENU_OPTION_FILE_Exit]) {
			if(controlState.Execute) {
				return ::gpk::APPLICATION_STATE_EXIT;
			}
		}

		if(iControl == (uint32_t)app.Menus[::gme::APP_MENU_EDIT].IdControls[::gme::MENU_OPTION_EDIT_Palette]) {
			if(controlState.Execute) { 
				::gme::mutex_guard															lock								(app.LockGUI);
				app.PaletteColors.resize(16, 16);
				for(uint32_t iColor = 0; iColor < app.PaletteColors.Texels.size(); ++iColor)
					app.PaletteColors.Texels[iColor]										= {rand() & 0xFFU, rand() & 0xFFU, rand() & 0xFFU, 0xFFU};//gui.Palette[iColor];
				if(-1 == app.PaletteControl.IdControl) {
					::gme::paletteInitialize(gui, app.PaletteControl);
					::gpk::controlSetParent(gui, app.PaletteControl.IdControl, desktop.IdControl);
				}
				::gme::paletteColorsSet	(gui, app.PaletteControl, app.PaletteColors.View);
			}
		}

		if(iControl == (uint32_t)app.Menus[::gme::APP_MENU_FILE].IdControls[::gme::MENU_OPTION_FILE_New]) {
			if(controlState.Execute) {
				desktop.Items.Viewports.push_back({});
				::gpk::SViewport															& viewportToSetUp					= desktop.Items.Viewports[desktop.Items.Viewports.size() - 1];
				{
					::gme::mutex_guard															lock								(app.LockGUI);
					::gpk::viewportInitialize(gui, viewportToSetUp);
					::gpk::controlSetParent(gui, viewportToSetUp.IdControl, desktop.IdControl);
				}
				::gpk::ptr_obj<::gpk::SImage<::gpk::SColorBGRA>>			newPaintScreen						= {};
				newPaintScreen.create();
				newPaintScreen->resize(gui.Controls.Controls[viewportToSetUp.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Area.Size.Cast<uint32_t>());
				memset(newPaintScreen->Texels.begin(), 0, newPaintScreen->Texels.size() * sizeof(::gpk::SColorBGRA));
				{
					::gme::mutex_guard															lock								(app.LockGUI);
					gui.Controls.Controls[viewportToSetUp.IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Image	= newPaintScreen->View;
				}
				app.PaintScreen.push_back(newPaintScreen);
			}
		}

		for(uint32_t iMenu = 0, countMenus = app.Menus.size() - 1; iMenu < countMenus; ++iMenu) 
			if(iControl == (uint32_t)app.Menus[::gme::APP_MENU_MAIN].IdControls[iMenu]) {
				::gpk::SControlState														& controlListStates					= gui.Controls.States[app.Menus[iMenu + 1].IdControl];
				if(controlState.Hover) {
					controlListStates.Hidden												= false;
					if(controlState.Execute) 
						app.SelectedMenu														= iMenu;
				}
				else {
					const ::gpk::SControlMetrics												& controlListMetrics				= gui.Controls.Metrics[app.Menus[iMenu + 1].IdControl];
					if(::gpk::in_range(gui.CursorPos.Cast<int32_t>(), controlListMetrics.Total.Global) && controlListStates.Hidden == false)
						controlListStates.Hidden												= false;
					else if(app.SelectedMenu != (int32_t)iMenu)
						controlListStates.Hidden												= true;
				}
			}
	}

	::gme::SAppPaletteGrid															& paletteControl									= app.PaletteControl;
	if(paletteControl.IdControl != -1) {
		for(uint32_t iColorControl = 1, colorControlStop = app.PaletteControl.IdControls.size(); iColorControl < colorControlStop; ++iColorControl) {
			if(gui.Controls.States[(uint32_t)paletteControl.IdControls[iColorControl]].Execute) {
				app.ColorPaint																= app.PaletteColors.Texels[iColorControl - 1];
				break;
			}
		}
		if(gui.Controls.States[(uint32_t)paletteControl.IdControls[0]].Pressed) {
			const int32_t																	parentControlId						= gui.Controls.Controls[paletteControl.IdControl].IndexParent;
			for(uint32_t iChild = 0, childCount = gui.Controls.Children[parentControlId].size(); iChild < childCount; ++iChild)
				if(gui.Controls.Children[parentControlId][iChild] == paletteControl.IdControl) {
					gui.Controls.Children[parentControlId].remove(iChild);
					gui.Controls.Children[parentControlId].push_back(paletteControl.IdControl);
					break;
				}
			if(framework.Input->MouseCurrent.Deltas.x || framework.Input->MouseCurrent.Deltas.y) {
				gui.Controls.Controls	[paletteControl.IdControl].Area.Offset				+= {(int16_t)framework.Input->MouseCurrent.Deltas.x, (int16_t)framework.Input->MouseCurrent.Deltas.y};
				::gpk::controlMetricsInvalidate(gui, paletteControl.IdControl);
			}
		}
	}
	{
		::gme::mutex_guard																lock								(app.LockGUI);
		::gpk::desktopUpdate(gui, desktop, *app.Framework.Input);
	}

	if(app.Framework.Input->ButtonDown(1) || app.Framework.Input->ButtonDown(2)) {
		app.SelectedMenu																= -1;
		for(uint32_t iMenu = 0, countMenus = app.Menus.size() - 1; iMenu < countMenus; ++iMenu) 
			gui.Controls.States[app.Menus[iMenu + 1].IdControl].Hidden						= true;
	}

	if(false == inControlArea) {
		if(app.Framework.Input->ButtonDown(0)) {
			app.SelectedMenu																= -1;
			for(uint32_t iMenu = 0, countMenus = app.Menus.size() - 1; iMenu < countMenus; ++iMenu) 
				gui.Controls.States[app.Menus[iMenu + 1].IdControl].Hidden						= true;
		}
	}

	for(uint32_t iViewport = 0; iViewport < desktop.Items.Viewports.size(); ++iViewport) {
		const ::gpk::SCoord2<int32_t>													paintOffset							= gui.Controls.Metrics[desktop.Items.Viewports[iViewport].IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Client.Global.Offset;
		if(::gpk::in_range(gui.CursorPos.Cast<int32_t>(), {paintOffset, (paintOffset + gui.Controls.Metrics[desktop.Items.Viewports[iViewport].IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Client.Global.Size)})) {
			const ::gpk::SCoord2<int32_t>													mouseDeltas							= {framework.Input->MouseCurrent.Deltas.x, framework.Input->MouseCurrent.Deltas.y};
			if(gui.Controls.States[desktop.Items.Viewports[iViewport].IdControls[::gpk::VIEWPORT_CONTROL_TARGET]].Pressed) {
				if(mouseDeltas.LengthSquared()) {
					const ::gpk::SLine2D<int32_t>													lineToDraw							= {gui.CursorPos.Cast<int32_t>() - paintOffset - mouseDeltas, gui.CursorPos.Cast<int32_t>() - paintOffset};
					::gpk::array_pod<::gpk::SCoord2<int32_t>>										pointsToDraw;
					//::gpk::drawLine(app.PaintScreen->Color.View, ::gpk::SColorBGRA{::gpk::YELLOW}, lineToDraw);
					::gpk::drawLine(app.PaintScreen[iViewport]->View.metrics(), lineToDraw, pointsToDraw);
					for(uint32_t iPoint = 0; iPoint < pointsToDraw.size(); ++iPoint) 
						::gpk::drawPixelBrightness(app.PaintScreen[iViewport]->View, pointsToDraw[iPoint], app.ColorPaint, 0.1f, 5.0);
				}
				else if(app.Framework.Input->ButtonDown(0)) {
					::gpk::SCoord2<int32_t>															mousePos							= {framework.Input->MouseCurrent.Position.x, framework.Input->MouseCurrent.Position.y};
					mousePos																	-= paintOffset;
					::gpk::drawPixelBrightness(app.PaintScreen[iViewport]->View, mousePos, app.ColorPaint, 0.1f, 5.0);
				}
			}
		}
	}
 	//timer.Frame();
	//info_printf("Update time: %f.", (float)timer.LastTimeSeconds);
	return 0; 
}
