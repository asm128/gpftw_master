#include "application.h"
#include "gpk_bitmap_file.h"
#include "gpk_grid_copy.h"
#include "gpk_label.h"

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
	controlConstraints	.AttachSizeToText									= {true, true};
	switch(menu.Orientation) {	
	case ::gme::MENU_ORIENTATION_HORIZONTAL	: controlConstraints.AttachSizeToControl.y = menu.IdControl; if(menu.IdControls.size()) controlConstraints.DockToControl.Right	 = menu.IdControls[menu.IdControls.size() - 1]; break;
	case ::gme::MENU_ORIENTATION_VERTICAL	: controlConstraints.AttachSizeToControl.x = menu.IdControl; if(menu.IdControls.size()) controlConstraints.DockToControl.Bottom	 = menu.IdControls[menu.IdControls.size() - 1]; break;
	}
	return menu.IdControls.push_back(idControl);			
}

			::gpk::error_t												setupGUI					(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework					= app.Framework;
	::gpk::SGUI																	& gui						= framework.GUI;
	gui.ColorModeDefault													= ::gpk::GUI_COLOR_MODE_3D;
	gui.ThemeDefault														= ::gpk::ASCII_COLOR_DARKCYAN * 16 + 7;
	::optionListInitialize(gui, app.OptionListMain);
	int32_t																		idOptionList0				= app.OptionListMain.IdControl;
	gui.Controls.Controls	[idOptionList0].Area							= {{}, {0, (int32_t)gui.FontCharSize.y + 4}};
	gui.Controls.Controls	[idOptionList0].Align							= ::gpk::ALIGN_CENTER_TOP;
	gui.Controls.States		[idOptionList0].Design							= true;
	gui.Controls.Text		[idOptionList0].Text							= "Menu";
	gui.Controls.Constraints[idOptionList0].AttachSizeToControl	.x			= idOptionList0; 

	::gme::optionListPush(gui, app.OptionListMain, "File");
	::gme::optionListPush(gui, app.OptionListMain, "Edit");
	::gme::optionListPush(gui, app.OptionListMain, "View");
	::gme::optionListPush(gui, app.OptionListMain, "Tool");
	::gme::optionListPush(gui, app.OptionListMain, "Help");
	::gpk::SControl																& control0					= gui.Controls.Controls		[app.OptionListMain.IdControl];
	control0.Border = control0.Margin										= {}; 

	::optionListInitialize(gui, app.OptionListFile);
	app.OptionListFile.Orientation											= ::gme::MENU_ORIENTATION_VERTICAL;
	int32_t																		idOptionList				= app.OptionListFile.IdControl;
	gui.Controls.Controls	[idOptionList].Align							= ::gpk::ALIGN_CENTER_TOP;
	gui.Controls.States		[idOptionList].Design							= true;
	gui.Controls.Text		[idOptionList].Text								= "File";
	gui.Controls.Constraints[idOptionList].AttachSizeToControl.x			= idOptionList;
	gui.Controls.Constraints[idOptionList].DockToControl.Bottom				= app.OptionListMain.IdControl;

	::gme::optionListPush(gui, app.OptionListFile, "New");
	::gme::optionListPush(gui, app.OptionListFile, "Open");
	::gme::optionListPush(gui, app.OptionListFile, "Save");
	app.IdExit																= app.OptionListFile.IdControls[::gme::optionListPush(gui, app.OptionListFile, "Exit")];

	::gpk::SControl																& control					= gui.Controls.Controls		[app.OptionListFile.IdControl];
	control.Margin = control.Border											= {};
	return 0;
}

			::gpk::error_t												cleanup						(::gme::SApplication & app)						{ return ::gpk::mainWindowDestroy(app.Framework.MainDisplay); }
			::gpk::error_t												setup						(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework					= app.Framework;
	::gpk::SDisplay																& mainWindow				= framework.MainDisplay;
	ree_if(0 == framework.Input.create(), "%s", "Out of memory?");
	error_if(errored(::gpk::mainWindowCreate(mainWindow, framework.RuntimeValues.PlatformDetail, framework.Input)), "Failed to create main window why?????!?!?!?!?");
	gpk_necall(setupGUI(app), "Unknown error.");

	::gpk::label																textureNames []				= 
		{ "..\\gpk_data\\images\\pow_0.bmp"
		, "..\\gpk_data\\images\\pow_1.bmp" 
		, "..\\gpk_data\\images\\pow_2.bmp" 
		, "..\\gpk_data\\images\\pow_3.bmp" 
		, "..\\gpk_data\\images\\crosshair_0.bmp"														
		, "..\\gpk_data\\images\\crosshair_1.bmp"														
		, "..\\gpk_data\\images\\crosshair_2.bmp"														
		, "..\\gpk_data\\images\\crosshair_3.bmp"														
		, "..\\gpk_data\\images\\crosshair_4.bmp"														
		};

	uint32_t																	myTextureCount				= ::gpk::size(textureNames);
	app.TexturesTest.resize(myTextureCount);

	for(uint32_t iTexture = 0; iTexture < myTextureCount; ++iTexture) {
		::gpk::SImage<::gpk::SColorBGRA>											& textureLoaded				= app.TexturesTest[iTexture];
		::gpk::bmpFileLoad(textureNames[iTexture], textureLoaded);
	}
	return 0; 
}

			::gpk::error_t												draw						(::gme::SApplication & app)						{ 
	::gpk::STimer																timer;
	app;
	::gpk::ptr_obj<::gpk::SRenderTarget<::gpk::SColorBGRA, uint32_t>>			target;
	target.create();
	target->Color		.resize(app.Framework.MainDisplay.Size);
	target->DepthStencil.resize(target->Color.View.metrics());

	//--- Dibujar sobrne el target
	for(uint32_t iTex = 0; iTex < app.TexturesTest.size(); ++iTex)
		::gpk::grid_copy_alpha(target->Color.View, app.TexturesTest[iTex].View, ::gpk::SCoord2<int32_t>{500, 256 + 32*(int32_t)iTex}, ::gpk::SColorBGRA{0xFF, 0x00, 0xFF, 0xFF});

	{
		::gme::mutex_guard															lock						(app.LockGUI);
		::gpk::guiDraw(app.Framework.GUI, target->Color.View);
	}
	{
		::gme::mutex_guard															lock						(app.LockRender);
		app.Offscreen															= target;
	}
	//timer.Frame();
	//info_printf("Draw time: %f.", (float)timer.LastTimeSeconds);
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
	retval_info_if(::gpk::APPLICATION_STATE_EXIT, ::gpk::APPLICATION_STATE_EXIT == ::gpk::updateFramework(app.Framework), "%s", "Exit requested by framework update.");

	::gpk::SGUI																	& gui						= framework.GUI;
	{
		::gme::mutex_guard															lock						(app.LockGUI);
		::gpk::guiProcessInput(gui, *app.Framework.Input);
	}
	if(app.Framework.Input->MouseCurrent.Deltas.z) {
		gui.Zoom.ZoomLevel														+= app.Framework.Input->MouseCurrent.Deltas.z * (1.0 / (120 * 4));
		::gpk::guiUpdateMetrics(gui, framework.MainDisplay.Size, true);
	}
 
	for(uint32_t iControl = 0, countControls = gui.Controls.Controls.size(); iControl < countControls; ++iControl) {
		const ::gpk::SControlState													& controlState				= gui.Controls.States[iControl];
		if(controlState.Unused || controlState.Disabled)
			continue;
		if(controlState.Execute) {
			info_printf("Executed %u.", iControl);
			if(iControl == (uint32_t)app.IdExit)
				return ::gpk::APPLICATION_STATE_EXIT;
		}
	}
	//timer.Frame();
	//info_printf("Update time: %f.", (float)timer.LastTimeSeconds);
	return 0; 
}
