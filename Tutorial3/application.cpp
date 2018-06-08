#include "application.h"
#include "gpk_bitmap_file.h"
#include "gpk_bitmap_target.h"
#include "gpk_grid_copy.h"

#define GPK_AVOID_LOCAL_APPLICATION_MODULE_MODEL_EXECUTABLE_RUNTIME
#include "gpk_app_impl.h"

GPK_DEFINE_APPLICATION_ENTRY_POINT(::gme::SApplication, "Module Explorer");

namespace gme 
{
	template<typename _tReturn>
	struct SAppMenuItem {
		_tReturn					ReturnType;
		::gpk::view_const_string	Text;
	};

	enum MENU_OPTION_FILE : int32_t
		{ MENU_OPTION_FILE_New		= 0
		, MENU_OPTION_FILE_Open
		, MENU_OPTION_FILE_Save
		, MENU_OPTION_FILE_Exit
		, MENU_OPTION_FILE_COUNT
		};

	enum MENU_OPTION_MAIN : int32_t
		{ MENU_OPTION_MAIN_File		= 0
		, MENU_OPTION_MAIN_Edit
		, MENU_OPTION_MAIN_View
		, MENU_OPTION_MAIN_Tool
		, MENU_OPTION_MAIN_Help
		, MENU_OPTION_MAIN_COUNT
		};
}

static		::gme::SAppMenuItem<::gme::MENU_OPTION_MAIN>				g_MenuOptionsMain	[::gme::MENU_OPTION_MAIN_COUNT]		=	
	{ {::gme::MENU_OPTION_MAIN_File, "File"}
	, {::gme::MENU_OPTION_MAIN_Edit, "Edit"}
	, {::gme::MENU_OPTION_MAIN_View, "View"}
	, {::gme::MENU_OPTION_MAIN_Tool, "Tool"}
	, {::gme::MENU_OPTION_MAIN_Help, "Help"}
	};

static		::gme::SAppMenuItem<::gme::MENU_OPTION_FILE>				g_MenuOptionsFile	[::gme::MENU_OPTION_FILE_COUNT]		=	
	{ {::gme::MENU_OPTION_FILE_New , "New "}
	, {::gme::MENU_OPTION_FILE_Open, "Open"}
	, {::gme::MENU_OPTION_FILE_Save, "Save"}
	, {::gme::MENU_OPTION_FILE_Exit, "Exit"}
	};

template <typename _tReturn>
			::gpk::error_t												setupMenu								
	( ::gpk::SGUI													& gui
	, ::gpk::SControlList											& menuControlList
	, int32_t														parentControl
	, ::gpk::ALIGN													menuAlign
	, ::gpk::CONTROL_LIST_DIRECTION									direction
	, const ::gpk::array_view<const ::gme::SAppMenuItem<_tReturn>>	& menuOptions 
	) { 
	::gpk::controlListInitialize(gui, menuControlList);
	menuControlList.Orientation												= direction;
	int32_t																		idOptionList							= menuControlList.IdControl;
	::gpk::SControl																& control								= gui.Controls.Controls[idOptionList];
	control.Align															= menuAlign;	// Align should not be hardcoded
	control.Margin = control.Border											= {};
	gui.Controls.Modes		[idOptionList].Design							= true;
	gui.Controls.Constraints[idOptionList].AttachSizeToControl.x			= (parentControl == -1) ? idOptionList : -1; 
	gui.Controls.Constraints[idOptionList].DockToControl.y					= parentControl;
	if(parentControl != -1) {
		gui.Controls.Constraints[idOptionList].Hidden							= true;
		gui.Controls.Text		[idOptionList].Text								= gui.Controls.Text[parentControl].Text;
	}
	else {
		gui.Controls.Text		[idOptionList].Text								= "Menu";
	}
	if(direction == ::gpk::CONTROL_LIST_DIRECTION_HORIZONTAL) 
		control.Area															= {{}, {0, 22}};	// Height should not be hardcoded
	else {
		const ::gpk::SCoord2<int32_t>												& parentOffset							= gui.Controls.Controls[parentControl].Area.Offset;
		control.Area															= (parentControl != -1) ? ::gpk::SRectangle2D<int32_t>{parentOffset, {100, }} : ::gpk::SRectangle2D<int32_t>{{}, {100, }};	// Width should not be hardcoded
	}

	for(uint32_t iOption = 0; iOption < menuOptions.size(); ++iOption)
		::gpk::controlListPush(gui, menuControlList, {menuOptions[iOption].Text.begin(), menuOptions[iOption].Text.size()});

	return 0;
}

			::gpk::error_t												setupGUI								(::gme::SApplication & app)						{ 
	::gpk::SFramework															& framework								= app.Framework;
	::gpk::SGUI																	& gui									= framework.GUI;
	gui.ColorModeDefault													= ::gpk::GUI_COLOR_MODE_3D;
	gui.ThemeDefault														= ::gpk::ASCII_COLOR_DARKCYAN * 16 + 7;

	// Setup "Main" menu
	::gpk::array_view<const ::gme::SAppMenuItem<::gme::MENU_OPTION_MAIN>>		menuItemsMain							= g_MenuOptionsMain;
	gpk_necall(setupMenu(gui, app.Menu.ControlListMain, -1, ::gpk::ALIGN_CENTER_TOP, ::gpk::CONTROL_LIST_DIRECTION_HORIZONTAL, menuItemsMain), "No known reason for this to fail other than memory corruption.");

	// Setup "File" menu
	::gpk::array_view<const ::gme::SAppMenuItem<::gme::MENU_OPTION_FILE>>		menuItemsFile							= g_MenuOptionsFile;
	gpk_necall(setupMenu(gui, app.Menu.ControlListFile, app.Menu.ControlListMain.IdControls[::gme::MENU_OPTION_MAIN_File], ::gpk::ALIGN_TOP_LEFT, ::gpk::CONTROL_LIST_DIRECTION_VERTICAL, menuItemsFile), "No known reason for this to fail other than memory corruption.");
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
	::gpk::ptr_obj<::gpk::SRenderTarget>										target;
	target.create();
	target->Color		.resize(app.Framework.MainDisplay.Size);
	target->DepthStencil.resize(target->Color.View.metrics());
	{
		::gme::mutex_guard															lock									(app.LockRender);
		::gpk::grid_copy(target->Color.View, app.PaintScreen->Color.View, ::gpk::SCoord2<int32_t>{0, app.Framework.GUI.Controls.Metrics[app.Menu.ControlListMain.IdControl].Total.Global.Size.y});
	}
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
		::gpk::guiUpdateMetrics(gui, framework.MainDisplay.Size);
	}

	::gpk::array_pod<uint32_t>														controlsToProcess					= {};
	::gpk::guiGetProcessableControls(gui, controlsToProcess);

	bool																			inControlArea						= false;
	bool																			anyControlPressed					= false;
	const int32_t																	idFile								= ::gme::MENU_OPTION_MAIN_File	;
	const int32_t																	idExit								= ::gme::MENU_OPTION_FILE_Exit	;
	const int32_t																	idNew								= ::gme::MENU_OPTION_FILE_New	;
	for(uint32_t iControlToProcess = 0, countControls = controlsToProcess.size(); iControlToProcess < countControls; ++iControlToProcess) {
		const uint32_t																	iControl							= controlsToProcess[iControlToProcess];
		const ::gpk::SControlState														& controlState						= gui.Controls.States[iControl];
		const ::gpk::SControlMetrics													& controlMetrics					= gui.Controls.Metrics[iControl];
		inControlArea																= inControlArea || ::gpk::in_range(gui.CursorPos.Cast<int32_t>(), controlMetrics.Total.Global);
		if(controlState.Pressed)
			anyControlPressed = true;
		if(controlState.Execute) {
			app.Menu.ControlSelectedMain												= -1;
			if(false == ::gpk::in_range(gui.CursorPos.Cast<int32_t>(), gui.Controls.Metrics[app.Menu.ControlListMain.IdControls[idFile]].Total.Global))
				gui.Controls.Constraints[app.Menu.ControlListFile.IdControl].Hidden			= true;
		}
		if(iControl == (uint32_t)app.Menu.ControlListFile.IdControls[idExit]) {
			if(controlState.Execute) {
				info_printf("Executed %u.", iControl);
				return ::gpk::APPLICATION_STATE_EXIT;
			}
		}
		if(iControl == (uint32_t)app.Menu.ControlListMain.IdControls[idFile]) {
			::gpk::SControlConstraints														& controlListConstraints			= gui.Controls.Constraints[app.Menu.ControlListFile.IdControl];
			if(controlState.Hover) {
				controlListConstraints.Hidden												= false;
				if(controlState.Execute) 
					app.Menu.ControlSelectedMain												= idFile;
			}
			else {
				const ::gpk::SControlMetrics													& controlListMetrics				= gui.Controls.Metrics[app.Menu.ControlListFile.IdControl];
				if(::gpk::in_range(gui.CursorPos.Cast<int32_t>(), controlListMetrics.Total.Global) && controlListConstraints.Hidden == false)
					controlListConstraints.Hidden												= false;
				else if(app.Menu.ControlSelectedMain != idFile)
					controlListConstraints.Hidden												= true;
			}
		}
		if(iControl == (uint32_t)app.Menu.ControlListFile.IdControls[idNew]) {
			if(controlState.Execute) {
				::gme::mutex_guard																lock								(app.LockRender);
				app.PaintScreen.create();
				app.PaintScreen->Color			.resize(app.Offscreen	->Color.View.metrics() - ::gpk::SCoord2<uint32_t>{0U, (uint32_t)gui.Controls.Metrics[app.Menu.ControlListMain.IdControl].Total.Global.Size.y});
				app.PaintScreen->DepthStencil	.resize(app.PaintScreen	->Color.View.metrics());
				::gpk::clearTarget(*app.PaintScreen);
			}
		}
	}

	if(app.Framework.Input->ButtonDown(1) || app.Framework.Input->ButtonDown(2)) {
		app.Menu.ControlSelectedMain												= -1;
		gui.Controls.Constraints[app.Menu.ControlListFile.IdControl].Hidden			= true;
	}

	if(false == inControlArea) {
		if(app.Framework.Input->ButtonDown(0)) {
			app.Menu.ControlSelectedMain												= -1;
			gui.Controls.Constraints[app.Menu.ControlListFile.IdControl].Hidden			= true;
		}
		if(false == anyControlPressed) {
			const ::gpk::SCoord2<int32_t>													paintOffset							= {0, (int32_t)gui.Controls.Metrics[app.Menu.ControlListMain.IdControl].Total.Global.Size.y};
			if(::gpk::in_range(gui.CursorPos.Cast<uint32_t>(), {paintOffset.Cast<uint32_t>(), app.PaintScreen->Color.View.metrics()})) {
				const ::gpk::SCoord2<int32_t>													mouseDeltas							= {framework.Input->MouseCurrent.Deltas.x, framework.Input->MouseCurrent.Deltas.y};
				if(app.Framework.Input->MouseCurrent.ButtonState[0]) {
					if(mouseDeltas.LengthSquared()) {
						const ::gpk::SLine2D<int32_t>													lineToDraw							= {gui.CursorPos.Cast<int32_t>() - paintOffset - mouseDeltas, gui.CursorPos.Cast<int32_t>() - paintOffset};
						::gpk::array_pod<::gpk::SCoord2<int32_t>>										pointsToDraw;
						//::gpk::drawLine(app.PaintScreen->Color.View, ::gpk::SColorBGRA{::gpk::YELLOW}, lineToDraw);
						::gpk::drawLine(app.PaintScreen->Color.View.metrics(), lineToDraw, pointsToDraw);
						for(uint32_t iPoint = 0; iPoint < pointsToDraw.size(); ++iPoint) 
							::gpk::drawPixelBrightness(app.PaintScreen->Color.View, pointsToDraw[iPoint], ::gpk::SColorBGRA{::gpk::YELLOW}, 0.1f, 5.0);
					}
					else if(app.Framework.Input->ButtonDown(0)) {
						::gpk::SCoord2<int32_t>															mousePos							= {framework.Input->MouseCurrent.Position.x, framework.Input->MouseCurrent.Position.y};
						mousePos																	-= paintOffset;
						::gpk::drawPixelBrightness(app.PaintScreen->Color.View, mousePos, ::gpk::SColorBGRA{::gpk::YELLOW}, 0.1f, 5.0);
					}
				}
			}
		}
	}
 	//timer.Frame();
	//info_printf("Update time: %f.", (float)timer.LastTimeSeconds);
	return 0; 
}
