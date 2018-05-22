#include "gpk_ro_rsw.h"
#include "gpk_ro_gnd.h"
#include "gpk_ro_rsm.h"

#include "gpk_scene.h"	
#include "gpk_camera.h"

#include "gpk_framework.h"
#include "gpk_gui.h"
 
#ifndef APPLICATION_H_098273498237423
#define APPLICATION_H_098273498237423

static constexpr const uint32_t													RENDER_THREAD_COUNT					= 4;

struct STiledTerrainCounters {
						uint32_t												nBaseTileCount						;	// Base tile count is equal to (tile map width*tile map depth)
						uint32_t												nTileColumnCount					;	// Stores the amount of tile columns that contain a single map chunk
						uint32_t												nTileRowCount						;	// Stores the amount of tile rows that contain a single map chunk
						uint32_t												nTopTileFaceCount					;	// Stores the number of tiles facing upwards that contain valid attributes and geometry

						uint32_t												nFrontTileFaceCount					;	// Stores the number of tiles facing front that contain valid attributes and geometry
						uint32_t												nRightTileFaceCount					;	// Stores the number of tiles facing right that contain valid attributes and geometry
						uint32_t												nBottomTileFaceCount				;	// Stores the number of tiles facing upwards that contain valid attributes and geometry
						uint32_t												nBackTileFaceCount					;	// Stores the number of tiles facing front that contain valid attributes and geometry

						uint32_t												nLeftTileFaceCount					;	// Stores the number of tiles facing right that contain valid attributes and geometry
						uint32_t												nTotalTileFaceCount					;	// Stores the total number of valid tiles, which should be equal to the addition of top, front and right tiles.
						uint32_t												nChunkColumnTileCount				;	// Stores the amount of tile columns that contain a single map chunk
						uint32_t												nChunkRowTileCount					;	// Stores the amount of tile rows that contain a single map chunk

						uint32_t												nChunkTotalTileCount				;	// Stores the total number of tiles contained in a single chunk
						uint32_t												nColumnChunkCount					;	// Stores the amount of column chunks
						uint32_t												nRowChunkCount						;	// Stores the amount of row chunks
						uint32_t												nTotalChunkCount					;	// Stores the total chunks contained in a map
};

struct SRenderCache {
						::gpk::array_pod<::gpk::SCoord2<int32_t>>				TrianglePixelCoords					= {};
						::gpk::array_pod<::gpk::STriangleWeights<double>>		TrianglePixelWeights				= {};
						::gpk::array_pod<::gpk::SCoord2<int32_t>>				WireframePixelCoords				= {};

						::gpk::array_pod<::gpk::STriangle3D<float>>				TransformedNormalsVertex			= {};

						::gpk::array_pod<int32_t>								Triangle3dIndices					= {};
						::gpk::array_pod<::gpk::STriangle3D<float>>				Triangle3dToDraw					= {};
						::gpk::array_pod<::gpk::STriangle3D<float>>				Triangle3dWorld						= {};

						uint32_t												TrianglesDrawn						= 0;
						uint32_t												PixelsDrawn							= 0;
						uint32_t												PixelsSkipped						= 0;
};

struct SApplicationThreadsState { 
						bool													Running								: 1;
						bool													Closed								: 1;
						bool													RequestedClose						: 1;
};

struct SApplicationThreadsCall {
						void													* Call								= 0;
						void													* Arguments							= 0;
};

struct SApplicationThreads {
						uintptr_t												Handles	[RENDER_THREAD_COUNT]		;
						SApplicationThreadsState								States	[RENDER_THREAD_COUNT]		;
						::gpk::array_pod<SApplicationThreadsCall>				Arguments							;
};

struct SApplication;

struct SThreadArgs {
						::SApplicationThreads										* ApplicationThreads				;
						int32_t														ThreadId							;
						SApplication												* ApplicationInstance				;
};

struct SDrawTrianglesArgs {
						const ::gpk::array_view	<::gpk::STriangleWeights<uint32_t>>	VertexIndexList	;
						const ::gpk::array_view	<::gpk::SCoord3<float>>				Vertices		;
						const ::gpk::array_view	<::gpk::SCoord2<float>>				Uvs				;
						const ::gpk::grid_view	<::gpk::SColorBGRA>					TextureView		;
						double														FFar			;
						double														FNear			;
						const ::gpk::SCoord3<float>									LightDir		;
						::SRenderCache												RenderCache		;
						::gpk::grid_view	<uint32_t>								TargetDepthView	;
						::gpk::grid_view	<::gpk::SColorBGRA>						TargetView		;
						const ::gpk::SColorFloat									DiffuseColor	;							
						const ::gpk::SColorFloat									AmbientColor	;							
						const ::gpk::array_view<::gpk::SLightInfoRSW>				Lights			;
						uint32_t													* PixelsDrawn	;
						uint32_t													* PixelsSkipped	;
};

struct SApplication {
						::gpk::SFramework										Framework							;

						::gpk::array_obj<::gpk::STexture<::gpk::SColorBGRA>>	TexturesGND							= {};
						::gpk::STexture<::gpk::SColorBGRA>						TextureFont							= {};
						::gpk::STextureMonochrome<uint32_t>						TextureFontMonochrome				= {};
						::gpk::STexture<::gpk::SColorBGRA>						TextureMinimap						= {};
						::gpk::SGUI												GUI									= {};

						::gpk::SModelPivot	<float>								GridPivot							= {};

						::gpk::SScene											Scene;
						::gpk::SCoord3<float>									LightDirection						= {10, 5, 0};
						::gpk::SGNDFileContents									GNDData								= {};
						::gpk::SRSWFileContents									RSWData								= {};
						::gpk::array_obj<::gpk::SRSMFileContents>				RSMData								= {};

						::gpk::SModelGND										GNDModel							= {};
						//::gpk::array_obj<::gpk::SModelRSM>					RSMModel							= {};

						// cabildo 2954
						::SRenderCache											RenderCache	[RENDER_THREAD_COUNT]	= {};
						::SApplicationThreads									Threads								= {};
						::SThreadArgs											ThreadArgs	[RENDER_THREAD_COUNT]	= {};

						::gpk::SRenderTarget									OffscreenGND						= {};

																				SApplication						(::gpk::SRuntimeValues& runtimeValues)			noexcept	: Framework(runtimeValues) {}
};

#endif // APPLICATION_H_098273498237423

