#include "gpk_gui_desktop.h"
#include "gpk_framework.h"

#include <mutex>

#ifndef APPLICATION_H_2078934982734
#define APPLICATION_H_2078934982734

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
				::gpk::view_const_string													FrameRateString						= {};

																							SApplication						(::gpk::SRuntimeValues & runtimeValues)		: Framework(runtimeValues)		{}
	};

	typedef	::std::lock_guard<::std::mutex>												mutex_guard;
} // namespace

#endif // APPLICATION_H_2078934982734
