#include "menu.h"
#include "application.h"
#include "gpk_bitmap_file.h"
#include "gpk_bitmap_target.h"
#include "gpk_grid_copy.h"

#define GPK_AVOID_LOCAL_APPLICATION_MODULE_MODEL_EXECUTABLE_RUNTIME
#include "gpk_app_impl.h"

GPK_DEFINE_APPLICATION_ENTRY_POINT(::gme::SApplication, "Module Explorer");

			::gpk::error_t												gme::viewportInitialize					(::gpk::SGUI& gui, ::gme::SViewport& viewport)				{
	const int16_t																heightTitleBar							= gui.FontCharSize.y + 4;
	const uint32_t																widthTarget								= 800;
	const uint32_t																heightTarget							= uint32_t(widthTarget * (9 / 16.0));
	{
		viewport.IdControl														= ::gpk::controlCreate(gui);
		::gpk::SControl																& control								= gui.Controls.Controls	[viewport.IdControl];
		const uint32_t																widthViewport							= widthTarget + control.Border.Left + control.Border.Right + control.Margin.Left + control.Margin.Right;
		const uint32_t																heightViewport							= heightTarget + heightTitleBar + control.Border.Top + control.Border.Bottom + control.Margin.Top + control.Margin.Bottom;
		control.Area.Size														= {(int16_t)widthViewport, (int16_t)heightViewport};
		control.Align															= ::gpk::ALIGN_CENTER;
	}
	{
		viewport.IdControls[VIEWPORT_CONTROL_TITLE	]							= ::gpk::controlCreate(gui);
		::gpk::SControl																& control								= gui.Controls.Controls		[viewport.IdControls[VIEWPORT_CONTROL_TITLE]];
		::gpk::SControlText															& controlText							= gui.Controls.Text			[viewport.IdControls[VIEWPORT_CONTROL_TITLE]];
		::gpk::SControlConstraints													& controlConstraints					= gui.Controls.Constraints	[viewport.IdControls[VIEWPORT_CONTROL_TITLE]];
		control.Area.Size.y														= heightTitleBar;
		controlText.Text														= "Viewport #0";
		controlText.Align														= ::gpk::ALIGN_CENTER_TOP;
		controlConstraints.AttachSizeToControl.x								= viewport.IdControls[VIEWPORT_CONTROL_TITLE];
		::gpk::controlSetParent(gui, viewport.IdControls[VIEWPORT_CONTROL_TITLE], viewport.IdControl);
	}
	{
		viewport.IdControls[VIEWPORT_CONTROL_TARGET	]							= ::gpk::controlCreate(gui);
		::gpk::SControl																& control								= gui.Controls.Controls		[viewport.IdControls[VIEWPORT_CONTROL_TARGET]];
		::gpk::SControlText															& controlText							= gui.Controls.Text			[viewport.IdControls[VIEWPORT_CONTROL_TARGET]];
		::gpk::SControlConstraints													& controlConstraints					= gui.Controls.Constraints	[viewport.IdControls[VIEWPORT_CONTROL_TARGET]];
		controlText.Align														= ::gpk::ALIGN_CENTER_TOP;
		control.Border = control.Margin											= {};
		control.Area.Size														= {(int16_t)widthTarget, (int16_t)heightTarget};
		control.Area.Offset	.y													= heightTitleBar;
		control.ColorTheme														= 60;
		controlConstraints.AttachSizeToControl.x								= viewport.IdControls[VIEWPORT_CONTROL_TARGET];
		::gpk::controlSetParent(gui, viewport.IdControls[VIEWPORT_CONTROL_TARGET], viewport.IdControl);
	}
	return 0;
}


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
	gui.Controls.States		[idOptionList].Design							= true;
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
	::gpk::SGUI																	& gui									= framework.GUI;
	gui.ColorModeDefault													= ::gpk::GUI_COLOR_MODE_3D;
	gui.ThemeDefault														= ::gpk::ASCII_COLOR_DARKGREEN * 16 + 5;

	// --- Setup desktop  
	app.ControlDesktop														= ::gpk::controlCreate(gui);
	//gui.Controls.Modes		[app.ControlDesktop].Design						= true;
	gui.Controls.Constraints[app.ControlDesktop].AttachSizeToControl		= {app.ControlDesktop, app.ControlDesktop};
	gui.Controls.Controls	[app.ControlDesktop].ColorTheme					= ::gpk::ASCII_COLOR_DARKGREY * 16 + 13;

	// --- Setup "Main" menu
	gpk_necall(setupMenu(gui, app.Menu.Items[::gme::APP_MENU_MAIN], -1, ::gpk::ALIGN_CENTER_TOP, ::gpk::CONTROL_LIST_DIRECTION_HORIZONTAL, ::gme::g_MenuOptionsMain), "No known reason for this to fail other than memory corruption.");
	gpk_necall(setupMenu(gui, app.Menu.Items[::gme::APP_MENU_FILE], app.Menu.Items[::gme::APP_MENU_MAIN].IdControls[::gme::MENU_OPTION_MAIN_File], ::gpk::ALIGN_TOP_LEFT, ::gpk::CONTROL_LIST_DIRECTION_VERTICAL, ::gme::g_MenuOptionsFile), "No known reason for this to fail other than memory corruption.");
	gpk_necall(setupMenu(gui, app.Menu.Items[::gme::APP_MENU_EDIT], app.Menu.Items[::gme::APP_MENU_MAIN].IdControls[::gme::MENU_OPTION_MAIN_Edit], ::gpk::ALIGN_TOP_LEFT, ::gpk::CONTROL_LIST_DIRECTION_VERTICAL, ::gme::g_MenuOptionsEdit), "No known reason for this to fail other than memory corruption.");
	return 0;
}

			::gpk::error_t												cleanup									(::gme::SApplication & app)						{ return ::gpk::mainWindowDestroy(app.Framework.MainDisplay); }
			::gpk::error_t												setup									(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework								= app.Framework;
	::gpk::SDisplay																& mainWindow							= framework.MainDisplay;
	ree_if(0 == framework.Input.create(), "Out of memory?");
	error_if(errored(::gpk::mainWindowCreate(mainWindow, framework.RuntimeValues.PlatformDetail, framework.Input)), "Failed to create main window why?????!?!?!?!?");
	gpk_necall(setupGUI(app), "Unknown error.");
	return 0; 
}

			::gpk::error_t												draw									(::gme::SApplication & app)						{ 
	::gpk::STimer																timer;
	::gpk::ptr_obj<::gpk::SRenderTarget<::gpk::SColorBGRA, uint32_t>>			target;
	target.create();
	target->Color		.resize(app.Framework.MainDisplay.Size);
	target->DepthStencil.resize(target->Color.View.metrics());
	//{
	//	::gme::mutex_guard															lock									(app.LockRender);
	//	::gpk::grid_copy(target->Color.View, app.PaintScreen->Color.View, ::gpk::SCoord2<int32_t>{0, app.Framework.GUI.Controls.Metrics[app.Menu.Items[::gme::APP_MENU_MAIN].IdControl].Total.Global.Size.y});
	//}
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
	{
		::gme::mutex_guard															lock									(app.LockGUI);
		::gpk::guiProcessInput(gui, *app.Framework.Input);
	}
	if(app.Framework.Input->MouseCurrent.Deltas.z) {
		gui.Zoom.ZoomLevel														+= app.Framework.Input->MouseCurrent.Deltas.z * (1.0f / (120 * 4));
		::gpk::guiUpdateMetrics(gui, framework.MainDisplay.Size, true);
	}

	::gpk::array_pod<uint32_t>														controlsToProcess					= {};
	::gpk::guiGetProcessableControls(gui, controlsToProcess);

	bool																			inControlArea						= false;
	bool																			anyControlPressed					= false;
	for(uint32_t iControlToProcess = 0, countControls = controlsToProcess.size(); iControlToProcess < countControls; ++iControlToProcess) {
		const uint32_t																	iControl							= controlsToProcess[iControlToProcess];
		const ::gpk::SControlState														& controlState						= gui.Controls.States[iControl];
		const ::gpk::SControlMetrics													& controlMetrics					= gui.Controls.Metrics[iControl];
		if(iControl != (uint32_t)app.ControlDesktop)
			inControlArea																= inControlArea || ::gpk::in_range(gui.CursorPos.Cast<int32_t>(), controlMetrics.Total.Global);

		if(controlState.Pressed && (int32_t)iControl != app.ControlDesktop)
			anyControlPressed = true;
		if(controlState.Execute) {
			info_printf("Executed %u.", iControl);
			app.Menu.ItemSelectedMain													= -1;
			for(uint32_t iMenu = 0, countMenus = app.Menu.Items.size() - 1; iMenu < countMenus; ++iMenu) {
				if(false == ::gpk::in_range(gui.CursorPos.Cast<int32_t>(), gui.Controls.Metrics[app.Menu.Items[::gme::APP_MENU_MAIN].IdControls[iMenu]].Total.Global)) 
					gui.Controls.States[app.Menu.Items[iMenu + 1].IdControl].Hidden			= true;
			}
		}
		if(iControl == (uint32_t)app.Menu.Items[::gme::APP_MENU_FILE].IdControls[::gme::MENU_OPTION_FILE_Exit]) {
			if(controlState.Execute) {
				return ::gpk::APPLICATION_STATE_EXIT;
			}
		}

		for(uint32_t iMenu = 0, countMenus = app.Menu.Items.size() - 1; iMenu < countMenus; ++iMenu) 
			if(iControl == (uint32_t)app.Menu.Items[::gme::APP_MENU_MAIN].IdControls[iMenu]) {
				::gpk::SControlState															& controlListStates			= gui.Controls.States[app.Menu.Items[iMenu + 1].IdControl];
				if(controlState.Hover) {
					controlListStates.Hidden												= false;
					if(controlState.Execute) 
						app.Menu.ItemSelectedMain													= iMenu;
				}
				else {
					const ::gpk::SControlMetrics													& controlListMetrics				= gui.Controls.Metrics[app.Menu.Items[iMenu + 1].IdControl];
					if(::gpk::in_range(gui.CursorPos.Cast<int32_t>(), controlListMetrics.Total.Global) && controlListStates.Hidden == false)
						controlListStates.Hidden												= false;
					else if(app.Menu.ItemSelectedMain != (int32_t)iMenu)
						controlListStates.Hidden												= true;
				}
			}

		if(iControl == (uint32_t)app.Menu.Items[::gme::APP_MENU_FILE].IdControls[::gme::MENU_OPTION_FILE_New]) {
			if(controlState.Execute) {
				{
					::gme::mutex_guard																lock								(app.LockGUI);
					if(-1 == app.PaintViewport.IdControl)
						::gme::viewportInitialize(gui, app.PaintViewport);
					::gpk::ptr_obj<::gpk::STexture<::gpk::SColorBGRA>>								newPaintScreen						= {};
					newPaintScreen.create();
					newPaintScreen->resize(gui.Controls.Controls[app.PaintViewport.IdControls[::gme::VIEWPORT_CONTROL_TARGET]].Area.Size.Cast<uint32_t>());
					memset(newPaintScreen->Texels.begin(), 0, newPaintScreen->Texels.size() * sizeof(::gpk::SColorBGRA));
					gui.Controls.Controls[app.PaintViewport.IdControls[::gme::VIEWPORT_CONTROL_TARGET]].Image	= newPaintScreen->View;
					app.PaintScreen																= newPaintScreen;
				}
			}
		}
		if(iControl == (uint32_t)app.PaintViewport.IdControls[::gme::VIEWPORT_CONTROL_TITLE]) {
			if(controlState.Pressed && (framework.Input->MouseCurrent.Deltas.x || framework.Input->MouseCurrent.Deltas.y)) 
				gui.Controls.Controls[app.PaintViewport.IdControl].Area.Offset					+= {(int16_t)framework.Input->MouseCurrent.Deltas.x, (int16_t)framework.Input->MouseCurrent.Deltas.y};
		}
	}

	if(app.Framework.Input->ButtonDown(1) || app.Framework.Input->ButtonDown(2)) {
		app.Menu.ItemSelectedMain														= -1;
		for(uint32_t iMenu = 0, countMenus = app.Menu.Items.size() - 1; iMenu < countMenus; ++iMenu) 
			gui.Controls.States[app.Menu.Items[iMenu + 1].IdControl].Hidden					= true;
	}

	if(false == inControlArea) {
		if(app.Framework.Input->ButtonDown(0)) {
			app.Menu.ItemSelectedMain														= -1;
			for(uint32_t iMenu = 0, countMenus = app.Menu.Items.size() - 1; iMenu < countMenus; ++iMenu) 
				gui.Controls.States[app.Menu.Items[iMenu + 1].IdControl].Hidden					= true;
		}
	}
	{
		const ::gpk::SCoord2<int32_t>													paintOffset							= gui.Controls.Metrics[app.PaintViewport.IdControls[::gme::VIEWPORT_CONTROL_TARGET]].Client.Global.Offset;
		if(::gpk::in_range(gui.CursorPos.Cast<int32_t>(), {paintOffset, (paintOffset + gui.Controls.Metrics[app.PaintViewport.IdControls[::gme::VIEWPORT_CONTROL_TARGET]].Client.Global.Size)})) {
			const ::gpk::SCoord2<int32_t>													mouseDeltas							= {framework.Input->MouseCurrent.Deltas.x, framework.Input->MouseCurrent.Deltas.y};
			if(gui.Controls.States[app.PaintViewport.IdControls[::gme::VIEWPORT_CONTROL_TARGET]].Pressed) {
				if(mouseDeltas.LengthSquared()) {
					const ::gpk::SLine2D<int32_t>													lineToDraw							= {gui.CursorPos.Cast<int32_t>() - paintOffset - mouseDeltas, gui.CursorPos.Cast<int32_t>() - paintOffset};
					::gpk::array_pod<::gpk::SCoord2<int32_t>>										pointsToDraw;
					//::gpk::drawLine(app.PaintScreen->Color.View, ::gpk::SColorBGRA{::gpk::YELLOW}, lineToDraw);
					::gpk::drawLine(app.PaintScreen->View.metrics(), lineToDraw, pointsToDraw);
					for(uint32_t iPoint = 0; iPoint < pointsToDraw.size(); ++iPoint) 
						::gpk::drawPixelBrightness(app.PaintScreen->View, pointsToDraw[iPoint], ::gpk::SColorBGRA{::gpk::YELLOW}, 0.1f, 5.0);
				}
				else if(app.Framework.Input->ButtonDown(0)) {
					::gpk::SCoord2<int32_t>															mousePos							= {framework.Input->MouseCurrent.Position.x, framework.Input->MouseCurrent.Position.y};
					mousePos																	-= paintOffset;
					::gpk::drawPixelBrightness(app.PaintScreen->View, mousePos, ::gpk::SColorBGRA{::gpk::YELLOW}, 0.1f, 5.0);
				}
			}
		}
	}
 	//timer.Frame();
	//info_printf("Update time: %f.", (float)timer.LastTimeSeconds);
	return 0; 
}
