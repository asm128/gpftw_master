#include "application.h"
#include "gpk_bitmap_file.h"
#include "gpk_bitmap_target.h"
#include "gpk_grid_copy.h"

#define GPK_AVOID_LOCAL_APPLICATION_MODULE_MODEL_EXECUTABLE_RUNTIME
#include "gpk_app_impl.h"

GPK_DEFINE_APPLICATION_ENTRY_POINT(::gme::SApplication, "Module Explorer");
	
			::gpk::error_t												setupGUI					(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework					= app.Framework;
	::gpk::SGUI																	& gui						= framework.GUI;
	gui.ColorModeDefault													= ::gpk::GUI_COLOR_MODE_3D;
	gui.ThemeDefault														= ::gpk::ASCII_COLOR_DARKCYAN * 16 + 7;
	::gpk::controlListInitialize(gui, app.ControlListMain);
	int32_t																		idOptionList0				= app.ControlListMain.IdControl;
	gui.Controls.Controls	[idOptionList0].Area							= {{}, {0, 22}};
	gui.Controls.Controls	[idOptionList0].Align							= ::gpk::ALIGN_CENTER_TOP;
	gui.Controls.Modes		[idOptionList0].Design							= true;
	gui.Controls.Text		[idOptionList0].Text							= "Menu";
	gui.Controls.Constraints[idOptionList0].AttachSizeToControl.x			= idOptionList0; //app.OptionList.IdControls[firstOption];

	app.IdFile																= app.ControlListMain.IdControls[::gpk::controlListPush(gui, app.ControlListMain, "File")];
	::gpk::controlListPush(gui, app.ControlListMain, "Edit");
	::gpk::controlListPush(gui, app.ControlListMain, "View");
	::gpk::controlListPush(gui, app.ControlListMain, "Tool");
	::gpk::controlListPush(gui, app.ControlListMain, "Help");
	::gpk::SControl																& control0					= gui.Controls.Controls[app.ControlListMain.IdControl];
	control0.Margin = control0.Border										= {};

	::gpk::controlListInitialize(gui, app.ControlListFile);
	app.ControlListFile.Orientation											= ::gpk::CONTROL_LIST_DIRECTION_VERTICAL;
	int32_t																		idOptionList				= app.ControlListFile.IdControl;
	gui.Controls.Controls	[idOptionList].Area								= {};
	gui.Controls.Controls	[idOptionList].Align							= ::gpk::ALIGN_CENTER_TOP;
	gui.Controls.Modes		[idOptionList].Design							= true;
	gui.Controls.Text		[idOptionList].Text								= "File";
	gui.Controls.Constraints[idOptionList].AttachSizeToControl.x			= idOptionList;						
	gui.Controls.Constraints[idOptionList].DockToControl.y					= app.ControlListMain.IdControl;	
	gui.Controls.Constraints[idOptionList].Hidden							= true;

	app.IdNew																= app.ControlListFile.IdControls[::gpk::controlListPush(gui, app.ControlListFile, "New")];
	::gpk::controlListPush(gui, app.ControlListFile, "Open");
	::gpk::controlListPush(gui, app.ControlListFile, "Save");
	app.IdExit																= app.ControlListFile.IdControls[::gpk::controlListPush(gui, app.ControlListFile, "Exit")];

	::gpk::SControl																& control					= gui.Controls.Controls		[app.ControlListFile.IdControl];
	control.Margin = control.Border											= {};
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
		gui.Zoom.ZoomLevel														+= app.Framework.Input->MouseCurrent.Deltas.z * (1.0f / (120 * 4));
		::gpk::guiUpdateMetrics(gui, framework.MainDisplay.Size);
	}

	const ::gpk::SCoord2<int32_t>													paintOffset				= {0, (int32_t)gui.Controls.Metrics[app.ControlListMain.IdControl].Total.Global.Size.y};

	if(::gpk::in_range(gui.CursorPos.Cast<uint32_t>(), {paintOffset.Cast<uint32_t>(), app.PaintScreen->Color.View.metrics()})) {
		const ::gpk::SCoord2<int32_t>													mouseDeltas				= {framework.Input->MouseCurrent.Deltas.x, framework.Input->MouseCurrent.Deltas.y};
		if(app.Framework.Input->MouseCurrent.ButtonState[0]) {
			if(mouseDeltas.LengthSquared()) {
				const ::gpk::SLine2D<int32_t>													lineToDraw				= {gui.CursorPos.Cast<int32_t>() - paintOffset - mouseDeltas, gui.CursorPos.Cast<int32_t>() - paintOffset};
				::gpk::array_pod<::gpk::SCoord2<int32_t>>										pointsToDraw;
				//::gpk::drawLine(app.PaintScreen->Color.View, ::gpk::SColorBGRA{::gpk::YELLOW}, lineToDraw);
				::gpk::drawLine(app.PaintScreen->Color.View.metrics(), lineToDraw, pointsToDraw);
				for(uint32_t iPoint = 0; iPoint < pointsToDraw.size(); ++iPoint) 
					::gpk::drawPixelBrightness(app.PaintScreen->Color.View, pointsToDraw[iPoint], ::gpk::SColorBGRA{::gpk::YELLOW}, 0.1f, 5.0);
			}
			else if(app.Framework.Input->ButtonDown(0)) {
				::gpk::SCoord2<int32_t>															mousePos				= {framework.Input->MouseCurrent.Position.x, framework.Input->MouseCurrent.Position.y};
				mousePos																	-= paintOffset;
				::gpk::drawPixelBrightness(app.PaintScreen->Color.View, mousePos, ::gpk::SColorBGRA{::gpk::YELLOW}, 0.1f, 5.0);
			}
		}
	}
 
	for(uint32_t iControl = 0, countControls = gui.Controls.Controls.size(); iControl < countControls; ++iControl) {
		const ::gpk::SControlState														& controlState				= gui.Controls.States[iControl];
		if(controlState.Unused || controlState.Disabled)
			continue;
		if(iControl == (uint32_t)app.IdExit) {
			if(controlState.Execute) {
				info_printf("Executed %u.", iControl);
				return 1;
			}
		}
		if(iControl == (uint32_t)app.IdFile)
			if(controlState.Hover) 
				gui.Controls.Constraints[app.ControlListFile.IdControl].Hidden	= false;
			else {
				if(::gpk::in_range(gui.CursorPos.Cast<int32_t>(), gui.Controls.Metrics[app.ControlListFile.IdControl].Total.Global) && gui.Controls.Constraints[app.ControlListFile.IdControl].Hidden == false)
					gui.Controls.Constraints[app.ControlListFile.IdControl].Hidden	= false;
				else
					gui.Controls.Constraints[app.ControlListFile.IdControl].Hidden	= true;
			}

		if(iControl == (uint32_t)app.IdNew) {
			if(controlState.Execute) {
				::gme::mutex_guard															lock						(app.LockRender);
				app.PaintScreen.create();
				app.PaintScreen->Color			.resize(app.Offscreen->Color.View.metrics() - ::gpk::SCoord2<uint32_t>{0U, (uint32_t)gui.Controls.Metrics[app.ControlListMain.IdControl].Total.Global.Size.y});
				app.PaintScreen->DepthStencil	.resize(app.Offscreen->Color.View.metrics() - ::gpk::SCoord2<uint32_t>{0U, (uint32_t)gui.Controls.Metrics[app.ControlListMain.IdControl].Total.Global.Size.y});
				::gpk::clearTarget(*app.PaintScreen);
			}
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
	{
		::gme::mutex_guard															lock						(app.LockRender);
		::gpk::grid_copy(target->Color.View, app.PaintScreen->Color.View, ::gpk::SCoord2<int32_t>{0, app.Framework.GUI.Controls.Metrics[app.ControlListMain.IdControl].Total.Global.Size.y});
	}
	//::gpk::clearTarget(*target);
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