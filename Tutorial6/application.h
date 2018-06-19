#include "gpk_gui_desktop.h"
#include "gpk_framework.h"
#include "gpk_scene.h"
#include "gpk_ro_rsw.h"
#include "gpk_ro_gnd.h"
#include "gpk_ro_rsm.h"

#include <mutex>

#ifndef APPLICATION_H_2078934982734
#define APPLICATION_H_2078934982734

struct STiledTerrainCounters {
						uint32_t														nBaseTileCount						;	// Base tile count is equal to (tile map width*tile map depth)
						uint32_t														nTileColumnCount					;	// Stores the amount of tile columns that contain a single map chunk
						uint32_t														nTileRowCount						;	// Stores the amount of tile rows that contain a single map chunk
						uint32_t														nTopTileFaceCount					;	// Stores the number of tiles facing upwards that contain valid attributes and geometry

						uint32_t														nFrontTileFaceCount					;	// Stores the number of tiles facing front that contain valid attributes and geometry
						uint32_t														nRightTileFaceCount					;	// Stores the number of tiles facing right that contain valid attributes and geometry
						uint32_t														nBottomTileFaceCount				;	// Stores the number of tiles facing upwards that contain valid attributes and geometry
						uint32_t														nBackTileFaceCount					;	// Stores the number of tiles facing front that contain valid attributes and geometry

						uint32_t														nLeftTileFaceCount					;	// Stores the number of tiles facing right that contain valid attributes and geometry
						uint32_t														nTotalTileFaceCount					;	// Stores the total number of valid tiles, which should be equal to the addition of top, front and right tiles.
						uint32_t														nChunkColumnTileCount				;	// Stores the amount of tile columns that contain a single map chunk
						uint32_t														nChunkRowTileCount					;	// Stores the amount of tile rows that contain a single map chunk

						uint32_t														nChunkTotalTileCount				;	// Stores the total number of tiles contained in a single chunk
						uint32_t														nColumnChunkCount					;	// Stores the amount of column chunks
						uint32_t														nRowChunkCount						;	// Stores the amount of row chunks
						uint32_t														nTotalChunkCount					;	// Stores the total chunks contained in a map
};

struct SRenderCache {
						::gpk::array_pod<::gpk::SCoord2<int32_t>>						TrianglePixelCoords					= {};
						::gpk::array_pod<::gpk::STriangleWeights<double>>				TrianglePixelWeights				= {};
						::gpk::array_pod<::gpk::SCoord2<int32_t>>						WireframePixelCoords				= {};

						::gpk::array_pod<::gpk::STriangle3D<float>>						TransformedNormalsVertex			= {};

						::gpk::array_pod<int32_t>										Triangle3dIndices					= {};
						::gpk::array_pod<::gpk::STriangle3D<float>>						Triangle3dToDraw					= {};
						::gpk::array_pod<::gpk::STriangle3D<float>>						Triangle3dWorld						= {};

						uint32_t														TrianglesDrawn						= 0;
						uint32_t														PixelsDrawn							= 0;
						uint32_t														PixelsSkipped						= 0;
};

struct SApplicationThreadsState { 
						bool															Running								: 1;
						bool															Closed								: 1;
						bool															RequestedClose						: 1;
};

struct SApplicationThreadsCall {
						void															* Call								= 0;
						void															* Arguments							= 0;
};

static constexpr	const uint32_t													RENDER_THREAD_COUNT					= 4;

struct SApplicationThreads {
						uintptr_t														Handles	[RENDER_THREAD_COUNT]		;
						SApplicationThreadsState										States	[RENDER_THREAD_COUNT]		;
						::gpk::array_pod<SApplicationThreadsCall>						Arguments							;
};

struct SApplication;

struct SThreadArgs {
						::SApplicationThreads											* ApplicationThreads				;
						int32_t															ThreadId							;
						SApplication													* ApplicationInstance				;
};

struct SDrawTrianglesArgs {
						const ::gpk::array_view	<::gpk::STriangleWeights<uint32_t>>		VertexIndexList	;
						const ::gpk::array_view	<::gpk::SCoord3<float>>					Vertices		;
						const ::gpk::array_view	<::gpk::SCoord2<float>>					Uvs				;
						const ::gpk::grid_view	<::gpk::SColorBGRA>						TextureView		;
						double															FFar			;
						double															FNear			;
						const ::gpk::SCoord3<float>										LightDir		;
						::SRenderCache													RenderCache		;
						::gpk::grid_view	<uint32_t>									TargetDepthView	;
						::gpk::grid_view	<::gpk::SColorBGRA>							TargetView		;
						const ::gpk::SColorFloat										DiffuseColor	;						
						const ::gpk::SColorFloat										AmbientColor	;						
						const ::gpk::array_view<::gpk::SLightInfoRSW>					Lights			;
						uint32_t														* PixelsDrawn	;
						uint32_t														* PixelsSkipped	;
};

namespace gme // I'm gonna use a different namespace in order to test a few things about the macros.
{
#pragma pack(push, 1)
	enum APP_MENU
		{ APP_MENU_MAIN		= 0
		, APP_MENU_FILE
		, APP_MENU_EDIT
		, APP_MENU_COUNT	
		};

	// U and V TexCoords follow the same order from GAT format heights

	struct STileSkin {
				::gpk::SCoord2<float>														UVs[4]							= { {0, 0}, {1, 0}, {0, 1}, {1, 1} };				//< west->east, north->south ordering; 0=left 1=right
				int16_t																		TextureIndex					= -1			;		//< -1 for none
	};

	struct STileFaceSkinMapping {
				int32_t																		SkinIndexTop					= -1;	// <= -1 for none
				int32_t																		SkinIndexRight					= -1;	// <= -1 for none
				int32_t																		SkinIndexFront					= -1;	// <= -1 for none
	};

	struct STileHeights {
				float																		fHeight[4]						= {0, 0, 0, 0};				// west->east, north->south ordering
	};

	struct STerrainEditorTiles {
				::gpk::array_pod<::gme::STileHeights		>								Heights							;	// 
				::gpk::array_pod<::gme::STileFaceSkinMapping>								SkinMapping						;	// 
				::gpk::array_pod<::gme::STileSkin			>								SkinData						;	// 
	};

	struct STerrainEditor {
				STerrainEditorTiles															TileData						;
	};

	struct SModelNode {
				::gpk::array_pod<::gpk::SCoord3<float>>										Normals							;
				::gpk::array_pod<::gpk::SCoord3<float>>										Vertices						;
				::gpk::array_pod<::gpk::SCoord2<float>>										UVs								;
				::gpk::array_pod<::gpk::STriangleWeights<uint32_t>>							VertexIndices					; 
				//::gpk::array_pod<int32_t>													SkinIndices						; // one per triangle (VertexIndices.size() / 3)
	};

	struct STileMapping {
				int32_t																		nChunkIndex						;
				int32_t																		SubsetIndexTop					;
				int32_t																		SubsetIndexFront				;
				int32_t																		SubsetIndexRight				;
				int32_t																		VerticesTop						[4];
				int32_t																		VerticesFront					[4];
				int32_t																		VerticesRight					[4];
	};

	struct SModel {
				::gpk::array_obj<::gme::SModelNode>											Nodes;
				::gpk::STexture<::gme::STileMapping>										TileMapping;
	};

	enum TILE_FACE_FACING 
		{ TILE_FACE_FACING_TOP		= 0
		, TILE_FACE_FACING_FRONT
		, TILE_FACE_FACING_RIGHT
		, TILE_FACE_FACING_BOTTOM
		, TILE_FACE_FACING_BACK
		, TILE_FACE_FACING_LEFT
		};

			::gpk::error_t												gndGenerateFaceGeometry
	( const ::gpk::array_pod<STileSkin			>	lstTileSkinData
	, const ::gpk::array_pod<STileHeights		>	lstTileGeometryData
	, const ::gpk::array_pod<STileFaceSkinMapping>	lstTileSkinMapping
	, const ::gpk::SCoord2<uint32_t>				tileMapMetrics
	, TILE_FACE_FACING								facing_direction
	, int32_t										textureIndex
	, SModelNode									& generated
	, ::gpk::grid_view<::gme::STileMapping>			& out_mapping
	);

			::gpk::error_t												blendGNDNormals								
	( const ::gpk::grid_view<::gme::STileHeights		>	& tileGeometryView
	, const ::gpk::grid_view<::gme::STileFaceSkinMapping>	& skinMappingView
	, const ::gpk::array_view<::gme::STileSkin			>	& lstTileSkinData
	, const ::gpk::grid_view<::gme::STileMapping		>	& tileMappingView
	, ::gpk::array_view<::gme::SModelNode				>	& gndModelNodes
	);

#pragma pack(pop)

	struct SApplication {
				::gpk::SColorBGRA															ColorPaint;
				::gpk::SDesktop																Desktop								= {};
				::gpk::SFramework															Framework							;
				::gpk::ptr_obj<::gpk::SRenderTarget>										Offscreen							= {};
				::gpk::ptr_obj<::gme::SModel>												TerrainModel						= {};
				
				::std::mutex																LockGUI;
				::std::mutex																LockRender;

				::gpk::array_obj<::gpk::ptr_obj<::gpk::SRenderTarget>>						PaintScreen							= {};
				::gpk::array_obj<::gpk::ptr_obj<::gpk::STexture<::gpk::SColorBGRA>>>		PaletteColors;


						::gpk::array_obj<::gpk::STexture<::gpk::SColorBGRA>>			TexturesGND							= {};
						::gpk::STexture<::gpk::SColorBGRA>								TextureMinimap						= {};
						::gpk::SGUI														GUI									= {};

						::gpk::SModelPivot	<float>										GridPivot							= {};

						::gpk::SScene													Scene;
						::gpk::SCoord3<float>											LightDirection						= {10, 5, 0};
						::gpk::SGNDFileContents											GNDData								= {};
						::gpk::SRSWFileContents											RSWData								= {};
						::gpk::array_obj<::gpk::SRSMFileContents>						RSMData								= {};

						::gpk::SModelGND												GNDModel							= {};
						//::gpk::array_obj<::gpk::SModelRSM>							RSMModel							= {};

						// cabildo 2954
						::SRenderCache													RenderCache	[RENDER_THREAD_COUNT]	= {};
						//::SApplicationThreads											Threads								= {};
						//::SThreadArgs													ThreadArgs	[RENDER_THREAD_COUNT]	= {};

						::gpk::array_obj<::gpk::ptr_obj<::gpk::SRenderTarget>>			RenderTargets						= {};
						::gpk::array_obj<::gpk::ptr_obj<::SRenderCache		>>			RenderCaches						= {};

						::std::mutex													RenderLock;
						::std::mutex													UpdateLock;

																							SApplication						(::gpk::SRuntimeValues & runtimeValues)		: Framework(runtimeValues)		{}
	};

	typedef	::std::lock_guard<::std::mutex>												mutex_guard;
} // namespace

#endif // APPLICATION_H_2078934982734
