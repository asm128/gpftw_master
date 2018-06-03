#include "application.h"
#include "gpk_bitmap_file.h"

#define GPK_AVOID_LOCAL_APPLICATION_MODULE_MODEL_EXECUTABLE_RUNTIME
#include "gpk_app_impl.h"

GPK_DEFINE_APPLICATION_ENTRY_POINT(::gme::SApplication, "Module Explorer");

			::gpk::error_t												menuInitialize				(::gpk::SGUI& gui, ::gme::SMenu& menu)	{
	gpk_necall(menu.IdControl = ::gpk::controlCreate(gui), "Failed to create menu control!");
	return 0;
}

			::gpk::error_t												gme::addMenuItem			(::gpk::SGUI& gui, ::gme::SMenu& menu, const ::gpk::view_const_string& text, int32_t idParent)	{
	if(menu.IdControl == -1) 
		gpk_necall(::menuInitialize(gui, menu), "");

	::gme::SMenuItem															newMenuItem					= {};
	newMenuItem.IdParent													= idParent;
	newMenuItem.IdControl													= ::gpk::controlCreate(gui);
	{
		gpk_necall(newMenuItem.IdControl, "Failed to create control! Out of memory?");
		::gpk::SControl																& control					= gui.Controls.Controls		[newMenuItem.IdControl];
		::gpk::SControlText															& controlText				= gui.Controls.Text			[newMenuItem.IdControl];
		::gpk::SControlConstraints													& controlConstraints		= gui.Controls.Constraints	[newMenuItem.IdControl];
		const int32_t																idControlParent				= (menu.Items.size() > (uint32_t)idParent) ? menu.Items[idParent].IdControl : menu.IdControl;
		::gpk::controlSetParent(gui, newMenuItem.IdControl, idControlParent);
		control.Margin															= {2, 2, 2, 2};
		control.Area															= {{}, {24, 24}};
		controlText			.Text												= {text.begin(), text.size()};
		controlText			.Align												= ::gpk::ALIGN_CENTER_LEFT;
		controlConstraints	.AttachSizeToText.x									= true;
		if(newMenuItem.IdParent != -1) {
			ree_if(menu.Items.size() <= (uint32_t)newMenuItem.IdParent, "Invalid parent id."); 
			if(0 < menu.Children[newMenuItem.IdParent].size()) {
				const int32_t																idPreviousChild				= menu.Children[newMenuItem.IdParent].size() - 1;
				switch(menu.Orientation) {
				case ::gme::MENU_ORIENTATION_HORIZONTAL	: controlConstraints.DockToControl.y = menu.Items[idPreviousChild].IdControl; break;
				case ::gme::MENU_ORIENTATION_VERTICAL	: controlConstraints.DockToControl.x = menu.Items[idPreviousChild].IdControl; break;
				}
			}
		}
		else if(0 < menu.Items.size()) {
			const int32_t																idPreviousChild				= menu.Items.size() - 1;
			switch(menu.Orientation) {	
			case ::gme::MENU_ORIENTATION_HORIZONTAL	: controlConstraints.DockToControl.x = menu.Items[idPreviousChild].IdControl; break;
			case ::gme::MENU_ORIENTATION_VERTICAL	: controlConstraints.DockToControl.y = menu.Items[idPreviousChild].IdControl; break;
			}
		}


	}
	int32_t																		newIndex					= menu.Items.push_back(newMenuItem);
	if(newMenuItem.IdParent != -1)
		menu.Children[newMenuItem.IdParent].push_back(newIndex);

	return 0;
}


			::gpk::error_t												setupGUI					(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework					= app.Framework;
	::gpk::SGUI																	& gui						= framework.GUI;
	gui.ColorModeDefault													= ::gpk::GUI_COLOR_MODE_THEME;
	gui.ThemeDefault														= ::gpk::ASCII_COLOR_DARKCYAN * 16 + 7;
	{
		gpk_necall(app.IdExit = ::gpk::controlCreate(gui), "Out of memory?");
		::gpk::SControl																& controlExit				= gui.Controls.Controls		[app.IdExit];
		::gpk::SControlText															& controlText				= gui.Controls.Text			[app.IdExit];
		::gpk::SControlConstraints													& controlConstraints		= gui.Controls.Constraints	[app.IdExit];
		controlExit.Area														= {{0, 0}, {64, 20}};
		controlExit.Align														= ::gpk::ALIGN_BOTTOM_RIGHT;
		controlText.Text														= "Exit";
		controlConstraints.AttachSizeToControl.x								= app.IdExit;
	}
	::gme::addMenuItem(gui, app.Menu, "File", -1);
	::gme::addMenuItem(gui, app.Menu, "Edit", -1);
	::gme::addMenuItem(gui, app.Menu, "View", -1);
	::gme::addMenuItem(gui, app.Menu, "Tool", -1);
	::gme::addMenuItem(gui, app.Menu, "Help", -1);
	
	gui.Controls.Controls	[app.Menu.IdControl].Align						= ::gpk::ALIGN_CENTER_TOP;
	gui.Controls.Text		[app.Menu.IdControl].Text						= "Menu";
	gui.Controls.Constraints[app.Menu.IdControl].AttachSizeToControl.x		= app.Menu.IdControl;

	::gpk::guiUpdateMetrics(gui, framework.MainDisplay.Size);
	return 0;
}

			::gpk::error_t												setup						(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework					= app.Framework;
	::gpk::SDisplay																& mainWindow				= framework.MainDisplay;
	ree_if(0 == framework.Input.create(), "Out of memory?");
	error_if(errored(::gpk::mainWindowCreate(mainWindow, framework.RuntimeValues.PlatformDetail, framework.Input)), "Failed to create main window why?????!?!?!?!?");
	gpk_necall(setupGUI(app), "Unknown error.");
	return 0; 
}

			::gpk::error_t												update						(::gme::SApplication & app, bool exitSignal)	{ 
	::gpk::STimer																timer;
	retval_info_if(::gpk::APPLICATION_STATE_EXIT, exitSignal, "Exit requested by runtime.");
	{
		::gme::mutex_guard															lock						(app.LockRender);
		app.Framework.MainDisplayOffscreen										= app.Offscreen;
	}
	::gpk::SFramework															& framework					= app.Framework;
	retval_info_if(::gpk::APPLICATION_STATE_EXIT, ::gpk::APPLICATION_STATE_EXIT == ::gpk::updateFramework(app.Framework), "Exit requested by framework update.");

	::gpk::SGUI																	& gui						= framework.GUI;
	{
		::gme::mutex_guard															lock						(app.LockGUI);
		::gpk::guiProcessInput(gui, *app.Framework.Input);
	}
	if(app.Framework.Input->MouseCurrent.Deltas.z) {
		gui.Zoom.ZoomLevel												+= app.Framework.Input->MouseCurrent.Deltas.z * (1.0f / (120 * 4));
		::gpk::guiUpdateMetrics(gui, framework.MainDisplay.Size);
	}
 
	for(uint32_t iControl = 0, countControls = gui.Controls.Controls.size(); iControl < countControls; ++iControl) {
		const ::gpk::SControlState													& controlState				= gui.Controls.States[iControl];
		if(controlState.Unused || controlState.Disabled)
			continue;
		if(controlState.Execute) {
			info_printf("Executed %u.", iControl);
			if(iControl == (uint32_t)app.IdExit)
				return 1;
		}
	}

	//timer.Frame();
	//info_printf("Update time: %f.", (float)timer.LastTimeSeconds);
	return 0; 
}

			::gpk::error_t												cleanup						(::gme::SApplication & app)						{ 
	::gpk::mainWindowDestroy(app.Framework.MainDisplay);
	return 0; 
}

			::gpk::error_t												draw						(::gme::SApplication & app)						{ 
	::gpk::STimer																timer;
	app;
	::gpk::ptr_obj<::gpk::SRenderTarget>										target;
	target.create();
	target->Color		.resize(app.Framework.MainDisplay.Size);
	target->DepthStencil.resize(target->Color.View.metrics());
	//::gpk::clearTarget(*target);
	{
		::gme::mutex_guard															lock						(app.LockGUI);
		::gpk::controlDrawHierarchy(app.Framework.GUI, 0, target->Color.View);
		::gpk::controlDrawHierarchy(app.Framework.GUI, 1, target->Color.View);
	}
	{
		::gme::mutex_guard															lock						(app.LockRender);
		app.Offscreen															= target;
	}
	//timer.Frame();
	//info_printf("Draw time: %f.", (float)timer.LastTimeSeconds);
	return 0; 
}