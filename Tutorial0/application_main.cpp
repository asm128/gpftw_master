// Tip: Hold Left ALT + SHIFT while tapping or holding the arrow keys in order to select multiple columns and write on them at once. 
//		Also useful for copy & paste operations in which you need to copy a bunch of variable or function names and you can't afford the time of copying them one by one.
#include "application.h"

#include "gpk_bitmap_target.h"
#include "gpk_bitmap_file.h"
#include "gpk_gui_text.h"

#include "gpk_app_impl.h"
#include "gpk_ro_rsm.h"
#include "gpk_ro_rsw.h"
#include "gpk_encoding.h"

#include <process.h>

static constexpr	const uint32_t										ASCII_SCREEN_WIDTH							= 132	;
static constexpr	const uint32_t										ASCII_SCREEN_HEIGHT							= 50	;

GPK_DEFINE_APPLICATION_ENTRY_POINT(::SApplication, "Tutorial 0");	

					::SApplication										* g_ApplicationInstance						= 0;

static				::gpk::error_t										updateSizeDependentResources				(::SApplication& applicationInstance)											{ 
	const ::gpk::SCoord2<uint32_t>												newSize										= applicationInstance.Framework.MainDisplay.Size;// / 2; 
	::gpk::SImage<::gpk::SColorBGRA>											& offscreen									= applicationInstance.Framework.MainDisplayOffscreen->Color;
	const ::gpk::SCoord2<uint32_t>												& offscreenMetrics							= offscreen.View.metrics();
	if(newSize != offscreenMetrics) {
		::gpk::SMatrix4<float>														& finalProjection							= applicationInstance.Scene.Transforms.FinalProjection	;
		::gpk::SMatrix4<float>														& fieldOfView								= applicationInstance.Scene.Transforms.FieldOfView		;
		::gpk::SMatrix4<float>														& viewport									= applicationInstance.Scene.Transforms.Viewport			;
		::gpk::SMatrix4<float>														& viewportInverse							= applicationInstance.Scene.Transforms.ViewportInverse	;
		::gpk::SMatrix4<float>														& viewportInverseCentered					= applicationInstance.Scene.Transforms.ViewportInverse	;
		fieldOfView.FieldOfView(applicationInstance.Scene.CameraAngle * ::gpk::math_pi, newSize.x / (double)newSize.y, applicationInstance.Scene.Camera.Range.Near, applicationInstance.Scene.Camera.Range.Far);
		viewport.Viewport(newSize, applicationInstance.Scene.Camera.Range.Far, applicationInstance.Scene.Camera.Range.Near);
		viewportInverse															= viewport.GetInverse();
		const ::gpk::SCoord2<int32_t>												screenCenter								= {(int32_t)newSize.x / 2, (int32_t)newSize.y / 2};
		viewportInverseCentered													= viewportInverse;
		viewportInverseCentered._41												+= screenCenter.x;
		viewportInverseCentered._42												+= screenCenter.y;
		finalProjection															= fieldOfView * viewportInverseCentered;
		applicationInstance.Scene.Transforms.FinalProjectionInverse				= finalProjection.GetInverse();
	}
	gpk_necall(::gpk::updateSizeDependentTarget(applicationInstance.Framework.MainDisplayOffscreen->Color			, newSize), "??");
	gpk_necall(::gpk::updateSizeDependentTarget(applicationInstance.Framework.MainDisplayOffscreen->DepthStencil	, newSize), "??");
	return 0;
}
// --- Cleanup application resources.
					::gpk::error_t										cleanup										(::SApplication& applicationInstance)											{
	::gpk::SDisplay																& displayMain								= applicationInstance.Framework.MainDisplay;
	::gpk::SDisplayPlatformDetail												& displayDetail								= displayMain.PlatformDetail;
	if(displayDetail.WindowHandle) 
		::gpk::mainWindowDestroy(displayMain);
	::UnregisterClass(displayDetail.WindowClassName, displayDetail.WindowClass.hInstance);
	bool																		waiting										= true;
	for(uint32_t iThread = 0, threadCount = ::gpk::size(applicationInstance.Threads.Handles); iThread < threadCount; ++iThread) 
		applicationInstance.Threads.States[iThread].RequestedClose				= true;
	while(waiting) {
		waiting																	= false;
		for(uint32_t iThread = 0, threadCount = ::gpk::size(applicationInstance.Threads.Handles); iThread < threadCount; ++iThread) {
			if(false == applicationInstance.Threads.States[iThread].Closed) {
				waiting																	= true;
				break;
			}
		}
	}

	g_ApplicationInstance													= 0;
	return 0;
}

//static				void												myThread									(void* _applicationThreads)														{
//	::SThreadArgs																& threadArgs								= *(::SThreadArgs*)_applicationThreads;
//	::SApplicationThreads														& applicationThreads						= *threadArgs.ApplicationThreads;
//	int32_t																		threadId									= threadArgs.ThreadId;
//	while(false == applicationThreads.States[threadId].RequestedClose) {
//		Sleep(10);
//	}
//	applicationThreads.States[threadId].Closed								= true;
//}

static				::gpk::error_t										setupThreads								(::SApplication& applicationInstance)													{
	for(uint32_t iThread = 0, threadCount = ::gpk::size(applicationInstance.Threads.Handles); iThread < threadCount; ++iThread) {
		applicationInstance.Threads.States	[iThread]							= {false, true, true};//{true,};									
		applicationInstance.Threads.Handles	[iThread]							= 0;//_beginthread(myThread, 0, &(applicationInstance.ThreadArgs[iThread] = {&applicationInstance.Threads, (int32_t)iThread, &applicationInstance}));
	}
	return 0;
}

					::gpk::error_t										setup										(::SApplication& applicationInstance)													{
	g_ApplicationInstance													= &applicationInstance;
	
	error_if(errored(setupThreads(applicationInstance)), "Unknown.");
	::gpk::SDisplay																& mainWindow								= applicationInstance.Framework.MainDisplay;
	applicationInstance.Framework.Input.create();
	error_if(errored(::gpk::mainWindowCreate(mainWindow, applicationInstance.Framework.RuntimeValues.PlatformDetail, applicationInstance.Framework.Input)), "Failed to create main window why?????!?!?!?!?");
	char																		bmpFileName2	[]							= "Codepage-437-24.bmp";
	error_if(errored(::gpk::bmpOrBmgLoad(bmpFileName2, applicationInstance.TextureFont)), "");
	const ::gpk::SCoord2<uint32_t>												& textureFontMetrics						= applicationInstance.TextureFont.View.metrics();
	gpk_necall(applicationInstance.TextureFontMonochrome.resize(textureFontMetrics), "Whou would we failt ro resize=");
	for(uint32_t y = 0, yMax = textureFontMetrics.y; y < yMax; ++y)
	for(uint32_t x = 0, xMax = textureFontMetrics.x; x < xMax; ++x) {
		const ::gpk::SColorBGRA														& srcColor									= applicationInstance.TextureFont.View[y][x];
		applicationInstance.TextureFontMonochrome.View[y * textureFontMetrics.x + x]	
			=	0 != srcColor.r
			||	0 != srcColor.g
			||	0 != srcColor.b
			;
	}

	static constexpr const char													ragnaPath	[]								= "..\\data_2006\\data\\";
	::gpk::array_pod<char_t>													base64Encoded								= {};
	::gpk::array_pod<ubyte_t>													base64Decoded								= {};
	::gpk::base64Encode({(const ubyte_t*)ragnaPath, ::gpk::size(ragnaPath)}, base64Encoded);
	::gpk::base64Decode(base64Encoded, base64Decoded);
	info_printf("base64Encoded: %s.", &base64Encoded[0]);
	info_printf("base64Decoded: %s.", &base64Decoded[0]);

	char																		temp		[512]							= {};
	::gpk::SRSWFileContents														& rswData									= applicationInstance.RSWData;
	::gpk::SGNDFileContents														& gndData									= applicationInstance.GNDData;
	sprintf_s(temp, "%s%s%s", ragnaPath, "", "alberta.rsw");			gpk_necall(::gpk::rswFileLoad(rswData, ::gpk::view_const_string(temp)), "Error");
	sprintf_s(temp, "%s%s%s", ragnaPath, "", &rswData.GNDFilename[0]);	gpk_necall(::gpk::gndFileLoad(gndData, ::gpk::view_const_string(temp)), "Error");

	applicationInstance.RSMData.resize(rswData.RSWModels.size());
	for(uint32_t iRSM = 0; iRSM < (uint32_t)rswData.RSWModels.size(); ++iRSM){
		::gpk::SRSMFileContents														& rsmData									= applicationInstance.RSMData[iRSM];
		::gpk::view_const_string													rsmFilename									= rswData.RSWModels[iRSM].Filename;
		bool																		bNotLoaded									= true;
		for(uint32_t iLoaded = 0; iLoaded < iRSM; ++iLoaded)
			if(0 == strcmp(&rsmFilename[0], &rswData.RSWModels[iLoaded].Filename[0])) {
				bNotLoaded																= false;
				break;
			}
		if(bNotLoaded) {
			sprintf_s(temp, "%s%s%s", ragnaPath, "model\\", &rsmFilename[0]);	
			error_if(errored(::gpk::rsmFileLoad(rsmData, ::gpk::view_const_string(temp, (uint32_t)-1))), "Failed to load file: %s.", temp);
		}
	}
	for(uint32_t iLight = 0; iLight < rswData.RSWLights.size(); ++iLight) {
		rswData.RSWLights[iLight].Position										*= 1.0 / gndData.Metrics.TileScale;
		rswData.RSWLights[iLight].Position										+= ::gpk::SCoord3<float>{gndData.Metrics.Size.x / 2.0f, 0.0f, (gndData.Metrics.Size.y / 2.0f)};
		rswData.RSWLights[iLight].Position.y									*= -1;
	}

	applicationInstance.TexturesGND.resize(gndData.TextureNames.size());
	for(uint32_t iTex = 0; iTex < gndData.TextureNames.size(); ++ iTex) {
		sprintf_s(temp, "%s%s%s", ragnaPath, "texture\\", &gndData.TextureNames[iTex][0]);	
		error_if(errored(::gpk::bmpFileLoad(::gpk::view_const_string(temp), applicationInstance.TexturesGND[iTex])), "Not found? %s.", temp);
	}

	applicationInstance.GNDModel.Nodes		.resize(gndData.TextureNames.size() * 6);
	applicationInstance.GNDModel.TileMapping.resize(gndData.Metrics.Size);
	::gpk::view_grid<::gpk::STileGeometryGND>									tileGeometryView								= {gndData.lstTileGeometryData.begin(), gndData.Metrics.Size};
	::gpk::SMinMax<float>														heightMinMax									= {};
	for(uint32_t iTile = 0; iTile < gndData.lstTileGeometryData.size(); ++iTile)
		if(gndData.lstTileGeometryData[iTile].SkinMapping.SkinIndexTop != -1) {
			for(uint32_t iHeight = 0; iHeight < 4; ++iHeight) {
				heightMinMax.Max												= ::gpk::max(gndData.lstTileGeometryData[iTile].fHeight[iHeight] * -1, heightMinMax.Max);
				heightMinMax.Min												= ::gpk::min(gndData.lstTileGeometryData[iTile].fHeight[iHeight] * -1, heightMinMax.Min);
			}
		}
	applicationInstance.TextureMinimap.resize(gndData.Metrics.Size);
	::gpk::view_grid<::gpk::SColorBGRA>											& minimapView									= applicationInstance.TextureMinimap.View;
	const float																	heightRange										= heightMinMax.Max - heightMinMax.Min;
	for(uint32_t y = 0, yMax = minimapView.metrics().y; y < yMax; ++y)
	for(uint32_t x = 0, xMax = minimapView.metrics().x; x < xMax; ++x) {
		float																		fAverageHeight									= 0;
		for(uint32_t iHeight = 0; iHeight < 4; ++iHeight)
			fAverageHeight															+= tileGeometryView[y][x].fHeight[iHeight] * -1;
		fAverageHeight															*= .25f;
		float																		colorRatio										= (fAverageHeight - heightMinMax.Min) / heightRange;
		//minimapView[y][minimapView.metrics().x - 1 - x]							= ((fAverageHeight < 0) ? ::gpk::WHITE : ::gpk::BLUE) * colorRatio;
		minimapView[y][x]														= ::gpk::WHITE * colorRatio;
	}

	for(uint32_t iTex = 0, texCount = gndData.TextureNames.size(); iTex < texCount; ++iTex) {
		int32_t indexTop	= iTex + gndData.TextureNames.size() * ::gpk::TILE_FACE_FACING_TOP		; gpk_necall(::gpk::gndGenerateFaceGeometry(gndData.lstTileTextureData, gndData.lstTileGeometryData, gndData.Metrics,::gpk::TILE_FACE_FACING_TOP	, iTex, applicationInstance.GNDModel.Nodes[indexTop		], applicationInstance.GNDModel.TileMapping.View), "");
		int32_t indexFront	= iTex + gndData.TextureNames.size() * ::gpk::TILE_FACE_FACING_FRONT	; gpk_necall(::gpk::gndGenerateFaceGeometry(gndData.lstTileTextureData, gndData.lstTileGeometryData, gndData.Metrics,::gpk::TILE_FACE_FACING_FRONT	, iTex, applicationInstance.GNDModel.Nodes[indexFront	], applicationInstance.GNDModel.TileMapping.View), "");
		int32_t indexRight	= iTex + gndData.TextureNames.size() * ::gpk::TILE_FACE_FACING_RIGHT	; gpk_necall(::gpk::gndGenerateFaceGeometry(gndData.lstTileTextureData, gndData.lstTileGeometryData, gndData.Metrics,::gpk::TILE_FACE_FACING_RIGHT	, iTex, applicationInstance.GNDModel.Nodes[indexRight	], applicationInstance.GNDModel.TileMapping.View), "");
		//int32_t indexBottom	= iTex + gndData.TextureNames.size() * ::gpk::TILE_FACE_FACING_BOTTOM	; gpk_necall(::gpk::gndGenerateFaceGeometry(gndData.lstTileTextureData, gndData.lstTileGeometryData, gndData.Metrics,::gpk::TILE_FACE_FACING_BOTTOM	, iTex, applicationInstance.GNDModel.Nodes[indexBottom	], applicationInstance.GNDModel.TileMapping.View), "");
		int32_t indexBack	= iTex + gndData.TextureNames.size() * ::gpk::TILE_FACE_FACING_BACK		; gpk_necall(::gpk::gndGenerateFaceGeometry(gndData.lstTileTextureData, gndData.lstTileGeometryData, gndData.Metrics,::gpk::TILE_FACE_FACING_BACK	, iTex, applicationInstance.GNDModel.Nodes[indexBack	], applicationInstance.GNDModel.TileMapping.View), "");
		int32_t indexLeft	= iTex + gndData.TextureNames.size() * ::gpk::TILE_FACE_FACING_LEFT		; gpk_necall(::gpk::gndGenerateFaceGeometry(gndData.lstTileTextureData, gndData.lstTileGeometryData, gndData.Metrics,::gpk::TILE_FACE_FACING_LEFT	, iTex, applicationInstance.GNDModel.Nodes[indexLeft	], applicationInstance.GNDModel.TileMapping.View), "");
	}
	blendGNDNormals(tileGeometryView, gndData.lstTileTextureData, applicationInstance.GNDModel.TileMapping.View, applicationInstance.GNDModel.Nodes); // Blend normals.

	gpk_necall(::updateSizeDependentResources(applicationInstance), "Cannot update offscreen and textures and this could cause an invalid memory access later on.");
	applicationInstance.Scene.Camera.Points.Position						= {0, 30, -20};
	applicationInstance.Scene.Camera.Range.Far								= 1000;
	applicationInstance.Scene.Camera.Range.Near								= 0.001;
	return 0;
}

					::gpk::error_t										update										(::SApplication& applicationInstance, bool systemRequestedExit)					{ 
	//::gpk::STimer																benchTimer;
	::gpk::SFramework															& framework									= applicationInstance.Framework;
	::gpk::SFrameInfo															& frameInfo									= framework.FrameInfo;
	rvi_if(1, systemRequestedExit, "%s", "Exiting because the runtime asked for close. We could also ignore this value and just continue execution if we don't want to exit.");
	{
		mutex_guard																	lock										(applicationInstance.RenderLock);
		if(applicationInstance.RenderTargets.size()) {
			if(applicationInstance.RenderTargets.size() > 1)
				warning_printf("2 Buffers have been rendered.");
			::gpk::ptr_obj<::gpk::SRenderTarget<::gpk::SColorBGRA, uint32_t>>			renderTarget								= applicationInstance.RenderTargets[applicationInstance.RenderTargets.size() - 1];
			if(renderTarget->Color.View.metrics() == framework.MainDisplay.Size)
				framework.MainDisplayOffscreen											= renderTarget;
			applicationInstance.RenderTargets.resize(0);
		}
		//else
		//	warning_printf("No frames rendered.");
	}
	::gpk::error_t																frameworkResult								= ::gpk::updateFramework(applicationInstance.Framework);
	ree_if(errored(frameworkResult), "%s", "Unknown error.");
	rvi_if(1, frameworkResult == 1, "%s", "Framework requested close. Terminating execution.");
	ree_if(errored(::updateSizeDependentResources(applicationInstance)), "%s", "Cannot update offscreen and this could cause an invalid memory access later on.");

	//----------------------------------------------
	bool																		updateProjection							= false;
	::gpk::SInput																& input										= *framework.Input;
	if(input.KeyboardCurrent.KeyState[VK_ADD		])	{ updateProjection = true; applicationInstance.Scene.CameraAngle += frameInfo.Seconds.LastFrame * .05f; }
	if(input.KeyboardCurrent.KeyState[VK_SUBTRACT	])	{ updateProjection = true; applicationInstance.Scene.CameraAngle -= frameInfo.Seconds.LastFrame * .05f; }
	if(updateProjection) {
		::gpk::SImage<::gpk::SColorBGRA>											& offscreen									= framework.MainDisplayOffscreen->Color;
		const ::gpk::SCoord2<uint32_t>												& offscreenMetrics							= offscreen.View.metrics();

		::gpk::SMatrix4<float>														& finalProjection							= applicationInstance.Scene.Transforms.FinalProjection	;
		::gpk::SMatrix4<float>														& fieldOfView								= applicationInstance.Scene.Transforms.FieldOfView		;
		::gpk::SMatrix4<float>														& viewport									= applicationInstance.Scene.Transforms.Viewport			;
		::gpk::SMatrix4<float>														& viewportInverse							= applicationInstance.Scene.Transforms.ViewportInverse	;
		::gpk::SMatrix4<float>														& viewportInverseCentered					= applicationInstance.Scene.Transforms.ViewportInverse	;
		{
			mutex_guard																	lock										(applicationInstance.UpdateLock);
			fieldOfView.FieldOfView(applicationInstance.Scene.CameraAngle * ::gpk::math_pi, offscreenMetrics.x / (double)offscreenMetrics.y, applicationInstance.Scene.Camera.Range.Near, applicationInstance.Scene.Camera.Range.Far);
			viewport.Viewport(offscreenMetrics, applicationInstance.Scene.Camera.Range.Far, applicationInstance.Scene.Camera.Range.Near);
			viewportInverse															= viewport.GetInverse();
			const ::gpk::SCoord2<int32_t>												screenCenter								= {(int32_t)offscreenMetrics.x / 2, (int32_t)offscreenMetrics.y / 2};
			viewportInverseCentered													= viewportInverse;
			viewportInverseCentered._41												+= screenCenter.x;
			viewportInverseCentered._42												+= screenCenter.y;
			finalProjection															= fieldOfView * viewportInverseCentered;
			applicationInstance.Scene.Transforms.FinalProjectionInverse				= finalProjection.GetInverse();
		}
	}

	//------------------------------------------------ Camera
		::gpk::SCameraPoints														newCamera									= applicationInstance.Scene.Camera.Points;
		newCamera.Position.RotateY(input.MouseCurrent.Deltas.x / 20.0f / (input.KeyboardCurrent.KeyState[VK_CONTROL] ? 2.0 : 1));
		if(input.MouseCurrent.Deltas.z) {
			::gpk::SCoord3<float>														zoomVector									= newCamera.Position;
			zoomVector.Normalize();
			const double																zoomWeight									= input.MouseCurrent.Deltas.z * (input.KeyboardCurrent.KeyState[VK_SHIFT] ? 10 : 1) / 240.;
			newCamera.Position														+= zoomVector * zoomWeight * .5;
		}
		applicationInstance.Scene.Camera.Points									= newCamera;
		::gpk::SMatrix4<float>														& viewMatrix								= applicationInstance.Scene.Transforms.View;
		::gpk::SCameraVectors														newCameraVectors							= applicationInstance.Scene.Camera.Vectors;
		newCameraVectors.Up														= {0, 1, 0};
		newCameraVectors.Front													= (newCamera.Target - newCamera.Position).Normalize();
		newCameraVectors.Right													= newCameraVectors.Up		.Cross(newCameraVectors.Front	).Normalize();
		newCameraVectors.Up														= newCameraVectors.Front	.Cross(newCameraVectors.Right	).Normalize();
		applicationInstance.Scene.Camera.Vectors								= newCameraVectors;
	{
		mutex_guard																	lock										(applicationInstance.UpdateLock);
		viewMatrix.View3D(newCamera.Position, newCameraVectors.Right, newCameraVectors.Up, newCameraVectors.Front);
	}
	//const	::gpk::SCoord2<uint32_t>												& gndDataMetrics							= applicationInstance.GNDData.Metrics.Size;
	//viewMatrix.LookAt(camera.Position, {(gndDataMetrics.x / 2.0f), 0, -(gndDataMetrics.y / 2.0f)}, {0, 1, 0});

	//------------------------------------------------ Lights
	::gpk::SCoord3<float>														& lightDir									= applicationInstance.LightDirection;
	lightDir.RotateY(frameInfo.Microseconds.LastFrame / 1000000.0f);
	lightDir.Normalize();

	::gpk::SRSWFileContents														& rswData									= applicationInstance.RSWData;
	const ::gpk::SCoord3<float>													halfMapDir									= ::gpk::SCoord3<float>{applicationInstance.GNDData.Metrics.Size.x / 2.0f, 0.0f, (applicationInstance.GNDData.Metrics.Size.y / 2.0f)};
	for(uint32_t iLight = 0; iLight < rswData.RSWLights.size(); ++iLight) {
		::gpk::SCoord3<float>														newLightPos									= rswData.RSWLights[iLight].Position;
		newLightPos																-= halfMapDir;
		newLightPos.RotateY(frameInfo.Seconds.LastFrame);
		newLightPos																+= halfMapDir;
		rswData.RSWLights[iLight].Position										= newLightPos;
	}
	//------------------------------------------------ 
	//applicationInstance.GridPivot.Scale										= {2.f, 4.f, 2.f};
	applicationInstance.GridPivot.Scale										= {1, -1, -1};
	//applicationInstance.GridPivot.Orientation.y								= (float)(sinf((float)(frameInfo.Seconds.Total / 2)) * ::gpk::math_2pi);
	//applicationInstance.GridPivot.Orientation.w								= 1;
	//applicationInstance.GridPivot.Orientation.Normalize();
	//applicationInstance.GridPivot.Position									= {applicationInstance.GNDData.Metrics.Size.x / 2.0f * -1, 0, applicationInstance.GNDData.Metrics.Size.y / 2.0f * -1};
	//benchTimer.Frame();
	//info_printf("Update frame: %f.", benchTimer.LastTimeSeconds);
	return 0;
}

					::gpk::error_t										drawGND										(::SApplication& applicationInstance, ::gpk::SRenderTarget<::gpk::SColorBGRA, uint32_t>& renderTargetGND);

					::gpk::error_t										draw										(::SApplication& applicationInstance)											{	// --- This function will draw some coloured symbols in each cell of the ASCII screen.
	//::gpk::STimer																benchTimer;
	//::gpk::STimer																benchTimer0;
	::gpk::SFramework															& framework									= applicationInstance.Framework;
	const ::gpk::view_grid<::gpk::SColorBGRA>									& fontAtlasView								= applicationInstance.TextureFont.View;
	::gpk::SRenderTarget<::gpk::SColorBGRA, uint32_t>							newRenderTargetGND							= {};
	::gpk::ptr_obj<::gpk::SRenderTarget<::gpk::SColorBGRA, uint32_t>>			newRenderTargetComposite					= {};
	{
		const ::gpk::SCoord2<uint32_t>												& offscreenMetrics							= framework.MainDisplay.Size;
		newRenderTargetGND		.Color			.resize(offscreenMetrics);
		newRenderTargetGND		.DepthStencil	.resize(offscreenMetrics);
		newRenderTargetComposite->Color			.resize(offscreenMetrics);
		newRenderTargetComposite->DepthStencil	.resize(offscreenMetrics);
	}
	::gpk::clearTarget(newRenderTargetGND);
	::gpk::clearTarget(*newRenderTargetComposite);
	::gpk::SFramework::TOffscreen												& offscreen									= newRenderTargetComposite->Color;
	::gpk::view_grid<::gpk::SColorBGRA>											& offscreenView								= offscreen.View;
	const ::gpk::SCoord2<uint32_t>												& offscreenMetrics							= offscreen.View.metrics();
	for(uint32_t y = 0, yMax = offscreenMetrics.y; y < yMax; ++y) {	// Draw background gradient.
		const uint8_t																colorHeight									= (uint8_t)(y / 10);
		for(uint32_t x = 0, xMax = offscreenMetrics.x; x < xMax; ++x)
			offscreen.View[y][x]													= {colorHeight, 0, 0, 0xFF};
	}
	//benchTimer0.Frame();
	//info_printf("Draw frame 0 step 0: %f.", benchTimer0.LastTimeSeconds);

	int32_t 																	pixelsDrawn1								= 0;
	//{
	//	mutex_guard																	lock										(applicationInstance.UpdateLock);
		pixelsDrawn1															= drawGND(applicationInstance, newRenderTargetGND); 
	//}
	//benchTimer0.Frame();
	//info_printf("Draw frame 0 step 1: %f.", benchTimer0.LastTimeSeconds);


	error_if(errored(pixelsDrawn1), "%s", "??");
	for(uint32_t y = 0, yMax = newRenderTargetGND.Color.View.metrics().y; y < yMax; ++y) 
		memcpy(offscreen[offscreenMetrics.y - 1 - y].begin(), newRenderTargetGND.Color.View[y].begin(), offscreenMetrics.x * sizeof(::gpk::SColorBGRA));
	const ::gpk::view_grid<::gpk::SColorBGRA>									& minimapView									= applicationInstance.TextureMinimap.View;
	::gpk::grid_copy(offscreenView, minimapView, ::gpk::SCoord2<int32_t>{(int32_t)(offscreenMetrics.x - minimapView.metrics().x), 0});

	static constexpr const ::gpk::SCoord2<int32_t>								sizeCharCell								= {9, 16};
	uint32_t																	lineOffset									= 0;
	static constexpr const char													textLine2	[]								= "Press ESC to exit.";
	const ::gpk::SColorBGRA														textColor									= {0, framework.FrameInfo.FrameNumber % 0xFFU, 0, 0xFFU};
	::gpk::textLineDrawAlignedFixedSizeLit(offscreenView, applicationInstance.TextureFontMonochrome.View, fontAtlasView.metrics(), lineOffset = offscreenMetrics.y / sizeCharCell.y - 1, offscreenMetrics, sizeCharCell, textLine2, textColor);

	::gpk::STimer																& timer										= applicationInstance.Framework.Timer;
	::gpk::SDisplay																& mainWindow								= applicationInstance.Framework.MainDisplay;
	const ::gpk::SGNDFileContents												& gndData									= applicationInstance.GNDData;
	const ::gpk::SRSWFileContents												& rswData									= applicationInstance.RSWData;
	const ::SRenderCache														& renderCache								= applicationInstance.RenderCache[0];
	char																		buffer		[256]							= {};
	uint32_t

	textLen																	= (uint32_t)sprintf_s(buffer, "[%u x %u]. FPS: %g. Last frame seconds: %g."					, mainWindow.Size.x, mainWindow.Size.y, 1 / timer.LastTimeSeconds, timer.LastTimeSeconds);							::gpk::textLineDrawAlignedFixedSizeRGBA(offscreenView, fontAtlasView, --lineOffset, offscreenMetrics, sizeCharCell, {buffer, textLen});
	textLen																	= (uint32_t)sprintf_s(buffer, "Triangles drawn: %u. Pixels drawn: %u. Pixels skipped: %u."	, (uint32_t)renderCache.TrianglesDrawn, (uint32_t)renderCache.PixelsDrawn, (uint32_t)renderCache.PixelsSkipped);	::gpk::textLineDrawAlignedFixedSizeRGBA(offscreenView, fontAtlasView, --lineOffset, offscreenMetrics, sizeCharCell, {buffer, textLen});
	textLen																	= (uint32_t)sprintf_s(buffer, "Tile grid size: {x=%u, z=%u}. Dynamic light count: %u."		, gndData.Metrics.Size.x, gndData.Metrics.Size.y, rswData.RSWLights.size());										::gpk::textLineDrawAlignedFixedSizeRGBA(offscreenView, fontAtlasView, --lineOffset, offscreenMetrics, sizeCharCell, {buffer, textLen});

	{
		mutex_guard																lock										(applicationInstance.RenderLock);
		applicationInstance.RenderTargets.push_back(newRenderTargetComposite);
	}
	//benchTimer0.Frame();
	//info_printf("Draw frame 0 step 2: %f.", benchTimer0.LastTimeSeconds);
	//::textDrawAlignedFixedSize(offscreenView, applicationInstance.TextureFontMonochrome.View, fontAtlasView.metrics(), --lineOffset, offscreenMetrics, sizeCharCell, {buffer, textLen}, textColor);

	//benchTimer.Frame();
	//info_printf("Draw frame: %f.", benchTimer.LastTimeSeconds);
	return 0;																																																
}
		