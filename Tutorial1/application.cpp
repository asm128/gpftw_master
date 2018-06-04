#include "application.h"
#include "gpk_bitmap_file.h"

#define GPK_AVOID_LOCAL_APPLICATION_MODULE_MODEL_EXECUTABLE_RUNTIME
#include "gpk_app_impl.h"

GPK_DEFINE_APPLICATION_ENTRY_POINT(::gme::SApplication, "Module Explorer");

			::gpk::error_t												optionListInitialize		(::gpk::SGUI& gui, ::gme::SOptionList& menu)													{
	gpk_necall(menu.IdControl = ::gpk::controlCreate(gui), "Failed to create menu control!");
	return 0;
}

			::gpk::error_t												gme::optionListPush			(::gpk::SGUI& gui, ::gme::SOptionList& menu, const ::gpk::view_const_string& text)				{
	if(menu.IdControl == -1) 
		gpk_necall(::optionListInitialize(gui, menu), "");
	const int32_t																idControl					= ::gpk::controlCreate(gui);
	gpk_necall(idControl, "Failed to create control! Out of memory?");
	::gpk::controlSetParent(gui, idControl, menu.IdControl);
	::gpk::SControl																& control					= gui.Controls.Controls		[idControl];
	::gpk::SControlText															& controlText				= gui.Controls.Text			[idControl];
	::gpk::SControlConstraints													& controlConstraints		= gui.Controls.Constraints	[idControl];
	control				.Margin												= {4, 4, 4, 4};
	controlText			.Text												= {text.begin(), text.size()};
	controlText			.Align												= ::gpk::ALIGN_CENTER_LEFT;
	controlConstraints	.AttachSizeToText.x									= true; // menu.Orientation == ::gme::MENU_ORIENTATION_VERTICAL	;
	controlConstraints	.AttachSizeToText.y									= true; // menu.Orientation == ::gme::MENU_ORIENTATION_HORIZONTAL;
	switch(menu.Orientation) {	
	case ::gme::MENU_ORIENTATION_HORIZONTAL	: controlConstraints.AttachSizeToControl.y = menu.IdControl; if(menu.IdControls.size()) controlConstraints.DockToControl.x = menu.IdControls[menu.IdControls.size() - 1]; break;
	case ::gme::MENU_ORIENTATION_VERTICAL	: controlConstraints.AttachSizeToControl.x = menu.IdControl; if(menu.IdControls.size()) controlConstraints.DockToControl.y = menu.IdControls[menu.IdControls.size() - 1]; break;
	}
	return menu.IdControls.push_back(idControl);			
}

	
			::gpk::error_t												setupGUI					(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework					= app.Framework;
	::gpk::SGUI																	& gui						= framework.GUI;
	gui.ColorModeDefault													= ::gpk::GUI_COLOR_MODE_3D;
	gui.ThemeDefault														= ::gpk::ASCII_COLOR_DARKCYAN * 16 + 7;
	//app.OptionList.Orientation												= ::gme::MENU_ORIENTATION_VERTICAL;
	/*int32_t firstOption = */
	::optionListInitialize(gui, app.OptionList);
	int32_t																		idOptionList				= app.OptionList.IdControl;
	gui.Controls.Controls	[idOptionList].Align							= ::gpk::ALIGN_CENTER_TOP;
	gui.Controls.Modes		[idOptionList].Design							= true;
	gui.Controls.Text		[idOptionList].Text								= "Menu";
	gui.Controls.Constraints[idOptionList].AttachSizeToControl.x			= idOptionList; //app.OptionList.IdControls[firstOption];

	::gme::optionListPush(gui, app.OptionList, "File");
	::gme::optionListPush(gui, app.OptionList, "Edit");
	::gme::optionListPush(gui, app.OptionList, "View");
	::gme::optionListPush(gui, app.OptionList, "Tool");
	::gme::optionListPush(gui, app.OptionList, "Help");
	::gpk::SControl																& control					= gui.Controls.Controls		[app.OptionList.IdControl];
	//::gpk::SControlText															& controlText				= gui.Controls.Text			[app.OptionList.IdControl];
	//::gpk::SControlConstraints													& controlConstraints		= gui.Controls.Constraints	[app.OptionList.IdControl];
	control.Margin = control.Border											= {};
	//	::gpk::guiUpdateMetrics(gui, framework.MainDisplay.Size);
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
		//::gpk::controlDrawHierarchy(app.Framework.GUI, 0, target->Color.View);
		::gpk::controlDrawHierarchy(app.Framework.GUI, app.OptionList.IdControl, target->Color.View);
	}
	{
		::gme::mutex_guard															lock						(app.LockRender);
		app.Offscreen															= target;
	}
	//timer.Frame();
	//info_printf("Draw time: %f.", (float)timer.LastTimeSeconds);
	return 0; 
}