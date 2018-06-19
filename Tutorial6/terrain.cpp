#include "application.h"


static		::gpk::error_t									gndGenerateFaceGeometryRight							(uint32_t baseX, uint32_t baseZ, float tileScale, int32_t skinIndexRight, const ::gme::STileHeights & geometryTile, const ::gme::STileHeights * geometryTileRight, const ::gpk::array_view<::gme::STileSkin>& lstTileSkinData, ::gme::SModelNode& generated, ::gme::STileMapping & out_mapping)	{ 
	const uint32_t													baseVertexIndex											= generated.Vertices.size();
	const ::gpk::SCoord3<float>										faceVerts	[4]											= 
		{ {baseX + 0.0f, (geometryTile.fHeight[2] / tileScale), baseZ + 1.0f}
		, {baseX + 1.0f, (geometryTile.fHeight[3] / tileScale), baseZ + 1.0f}
		, {baseX + 0.0f, geometryTileRight ? (geometryTileRight->fHeight[0] / tileScale) : 0.0f, baseZ + 1.0f}
		, {baseX + 1.0f, geometryTileRight ? (geometryTileRight->fHeight[1] / tileScale) : 0.0f, baseZ + 1.0f}
		};
	const ::gpk::SCoord3<float>										faceNormals	[4]											=
		{ (faceVerts[1] - faceVerts[0]).Cross(faceVerts[2] - faceVerts[0]).Normalize()
		, (faceVerts[3] - faceVerts[1]).Cross(faceVerts[0] - faceVerts[1]).Normalize()
		, (faceVerts[0] - faceVerts[2]).Cross(faceVerts[3] - faceVerts[2]).Normalize()
		, (faceVerts[2] - faceVerts[3]).Cross(faceVerts[1] - faceVerts[3]).Normalize()
		};

	generated.Vertices	.append(faceVerts	);
	generated.Normals	.append(faceNormals	);
	for(uint32_t i = 0; i < 4; ++i)
		out_mapping.VerticesRight[i]								= baseVertexIndex + i;

	{
		const int32_t													faceSkins	[4]										= 
			{ skinIndexRight
			, skinIndexRight
			, skinIndexRight
			, skinIndexRight
			};
		//generated.SkinIndices.append(faceSkins);
		const ::gme::STileSkin											& skinTile											= lstTileSkinData[faceSkins[0]];
		::gpk::SCoord2<float>											faceUVs	[4]											;
		for(uint32_t i = 0; i < 4; ++i)
			faceUVs[i]													= skinTile.UVs[i];
		generated.UVs.append(faceUVs);
	}
	{
		const ::gpk::STriangleWeights<uint32_t>							faceIndices[6]										= 
			{	{ baseVertexIndex + 0 // + 0//+ 0 // 0
				, baseVertexIndex + 1 // + 1//+ 2 // 1
				, baseVertexIndex + 2 // + 2//+ 1 // 2
				} //			 //// 	+	 //// 
			,	{ baseVertexIndex + 1 // + 1//+ 1 // 1
				, baseVertexIndex + 3 // + 3//+ 2 // 3
				, baseVertexIndex + 2 // + 2//+ 3 // 2
			}
			};
		generated.VertexIndices	.append(faceIndices);
	}	
	return 0; 
}

static		::gpk::error_t									gndGenerateFaceGeometryFront							(uint32_t baseX, uint32_t baseZ, float tileScale, int32_t skinIndexFront, const ::gme::STileHeights & geometryTile, const ::gme::STileHeights* geometryTileFront, const ::gpk::array_view<::gme::STileSkin>& lstTileSkinData, ::gme::SModelNode& generated, ::gme::STileMapping & out_mapping)	{ 
	const uint32_t													baseVertexIndex											= generated.Vertices.size();
	const ::gpk::SCoord3<float>										faceVerts	[4]											= 
		{ {baseX + 1.0f, (geometryTile.fHeight[3] / tileScale), baseZ + 1.0f}
		, {baseX + 1.0f, (geometryTile.fHeight[1] / tileScale), baseZ + 0.0f}
		, {baseX + 1.0f, geometryTileFront ? (geometryTileFront->fHeight[2] / tileScale) : 0.0f, baseZ + 1.0f}
		, {baseX + 1.0f, geometryTileFront ? (geometryTileFront->fHeight[0] / tileScale) : 0.0f, baseZ + 0.0f}
		};
	const ::gpk::SCoord3<float>										faceNormals	[4]											=
		{ (faceVerts[1] - faceVerts[0]).Cross(faceVerts[2] - faceVerts[0]).Normalize()
		, (faceVerts[3] - faceVerts[1]).Cross(faceVerts[0] - faceVerts[1]).Normalize()
		, (faceVerts[0] - faceVerts[2]).Cross(faceVerts[3] - faceVerts[2]).Normalize()
		, (faceVerts[2] - faceVerts[3]).Cross(faceVerts[1] - faceVerts[3]).Normalize()
		};
	generated.Vertices		.append(faceVerts	);
	generated.Normals		.append(faceNormals	);
	for(uint32_t i = 0; i < 4; ++i)
		out_mapping.VerticesFront[i]								= baseVertexIndex + i;
	{
		const int32_t													faceSkins	[4]										= 
			{ skinIndexFront
			, skinIndexFront
			, skinIndexFront
			, skinIndexFront
			};
		//generated.SkinIndices.append(faceSkins);
		const ::gme::STileSkin										& skinTile											= lstTileSkinData[faceSkins[0]];
		::gpk::SCoord2<float>											faceUVs	[4]											;
		for(uint32_t i = 0; i < 4; ++i)
			faceUVs[i]													= skinTile.UVs[i];
		generated.UVs.append(faceUVs);
		const ::gpk::STriangleWeights<uint32_t>							faceIndices[6]										= 
			{	{ baseVertexIndex + 0 // + 0//+ 0 // 0
				, baseVertexIndex + 1 // + 1//+ 2 // 1
				, baseVertexIndex + 2 // + 2//+ 1 // 2
				} //			 //// 	+	 //// 
			,	{ baseVertexIndex + 1 // + 1//+ 1 // 1
				, baseVertexIndex + 3 // + 3//+ 2 // 3
				, baseVertexIndex + 2 // + 2//+ 3 // 2
			}
			};
		generated.VertexIndices	.append(faceIndices);
	}	
	return 0; 
}
#pragma warning(default : 4100)

static		::gpk::error_t									gndGenerateFaceGeometryTop								(uint32_t baseX, uint32_t baseZ, float tileScale, int32_t skinIndexTop, const ::gme::STileHeights & geometryTile, const ::gpk::array_view<::gme::STileSkin>& lstTileSkinData, ::gme::SModelNode& generated, ::gme::STileMapping & out_mapping)	{
	const uint32_t													baseVertexIndex											= generated.Vertices.size();
	const ::gpk::SCoord3<float>										faceVerts	[4]											= 
		{ {baseX + 0.0f, (geometryTile.fHeight[0] / tileScale), baseZ + 0.0f}
		, {baseX + 1.0f, (geometryTile.fHeight[1] / tileScale), baseZ + 0.0f}
		, {baseX + 0.0f, (geometryTile.fHeight[2] / tileScale), baseZ + 1.0f}
		, {baseX + 1.0f, (geometryTile.fHeight[3] / tileScale), baseZ + 1.0f}
		};
	const ::gpk::SCoord3<float>										faceNormals	[4]											=
		{ (faceVerts[1] - faceVerts[0]).Cross(faceVerts[2] - faceVerts[0]).Normalize()
		, (faceVerts[3] - faceVerts[1]).Cross(faceVerts[0] - faceVerts[1]).Normalize()
		, (faceVerts[0] - faceVerts[2]).Cross(faceVerts[3] - faceVerts[2]).Normalize()
		, (faceVerts[2] - faceVerts[3]).Cross(faceVerts[1] - faceVerts[3]).Normalize()
		};
	//::gpk::SCoord3<float>											normal												
	//	= faceNormals	[0]
	//	+ faceNormals	[1]
	//	+ faceNormals	[2]
	//	+ faceNormals	[3]
	//	;
	//(normal /=4).Normalize();
	//faceNormals	[0] = normal;
	//faceNormals	[1] = normal;
	//faceNormals	[2] = normal;
	//faceNormals	[3] = normal;
	generated.Vertices		.append(faceVerts	);
	generated.Normals		.append(faceNormals	);
	for(uint32_t i = 0; i < 4; ++i)
		out_mapping.VerticesTop[i]										= baseVertexIndex + i;

	{
		const int32_t													faceSkins	[4]										= 
			{ skinIndexTop
			, skinIndexTop
			, skinIndexTop
			, skinIndexTop
			};
		//generated.SkinIndices.append(faceSkins);
		const ::gme::STileSkin											& skinTile											= lstTileSkinData[faceSkins[0]];
		::gpk::SCoord2<float>											faceUVs	[4]											;
		for(uint32_t i = 0; i < 4; ++i)
			faceUVs[i]													= skinTile.UVs[i];
		generated.UVs.append(faceUVs);
	}
	{
		const ::gpk::STriangleWeights<uint32_t>							faceIndices[6]										= 
			{	{ baseVertexIndex + 0 // + 0//+ 0 // 0
				, baseVertexIndex + 1 // + 1//+ 2 // 1
				, baseVertexIndex + 2 // + 2//+ 1 // 2
				} //			 //// 	+	 //// 
			,	{ baseVertexIndex + 1 // + 1//+ 1 // 1
				, baseVertexIndex + 3 // + 3//+ 2 // 3
				, baseVertexIndex + 2 // + 2//+ 3 // 2
			}
			};
		generated.VertexIndices	.append(faceIndices);
	}
	return 0;
}
			::gpk::error_t										gme::gndGenerateFaceGeometry
	( const ::gpk::array_pod<STileSkin			>	lstTileSkinData
	, const ::gpk::array_pod<STileHeights		>	lstTileGeometryData
	, const ::gpk::array_pod<STileFaceSkinMapping>	lstTileSkinMapping
	, const ::gpk::SCoord2<uint32_t>				tileMapMetrics
	, TILE_FACE_FACING								facing_direction
	, int32_t										textureIndex
	, SModelNode									& generated
	, ::gpk::grid_view<::gme::STileMapping>			& out_mapping
	) {
	::gpk::grid_view<const ::gme::STileHeights>							geometryView										= {lstTileGeometryData.begin(), tileMapMetrics};
	::gpk::grid_view<const ::gme::STileFaceSkinMapping>					skinMappingView										= {lstTileSkinMapping.begin(), tileMapMetrics};
	for(uint32_t y = 0; y < geometryView.metrics().y; ++y)
	for(uint32_t x = 0; x < geometryView.metrics().x; ++x) {
		const ::gme::STileHeights											& geometryTile										= geometryView		[y][x];
		const ::gme::STileFaceSkinMapping									& faceSkinMapping									= skinMappingView	[y][x];
		const ::gme::STileHeights											* geometryTileFront									= ((x + 1) < geometryView.metrics().x) ? &geometryView	[y][x + 1]	: 0;
		const ::gme::STileHeights											* geometryTileRight									= ((y + 1) < geometryView.metrics().y) ? &geometryView	[y + 1][x]	: 0;
		//const ::gme::STileHeights												* geometryTileFrontRight							
		//	= ((x + 1) < geometryView.metrics().x 
		//	&& (y + 1) < geometryView.metrics().y) 
		//	? &geometryView	[y + 1][x + 1] : 0;
		::gme::TILE_FACE_FACING												facingDirection										= facing_direction;
		if(facingDirection == ::gme::TILE_FACE_FACING_FRONT && geometryTileFront && (geometryTileFront->fHeight[0] > geometryTile.fHeight[1] || geometryTileFront->fHeight[2] > geometryTile.fHeight[3])) facingDirection = ::gme::TILE_FACE_FACING_BACK;
		if(facingDirection == ::gme::TILE_FACE_FACING_BACK	&& geometryTileFront && (geometryTileFront->fHeight[0] < geometryTile.fHeight[1] || geometryTileFront->fHeight[2] < geometryTile.fHeight[3])) facingDirection = ::gme::TILE_FACE_FACING_FRONT;
		if(facingDirection == ::gme::TILE_FACE_FACING_RIGHT && geometryTileRight && (geometryTileRight->fHeight[0] > geometryTile.fHeight[2] || geometryTileRight->fHeight[1] > geometryTile.fHeight[3])) facingDirection = ::gme::TILE_FACE_FACING_LEFT;
		if(facingDirection == ::gme::TILE_FACE_FACING_LEFT	&& geometryTileRight && (geometryTileRight->fHeight[0] < geometryTile.fHeight[2] || geometryTileRight->fHeight[1] < geometryTile.fHeight[3])) facingDirection = ::gme::TILE_FACE_FACING_RIGHT;
		if(facingDirection == ::gme::TILE_FACE_FACING_FRONT && 0 == geometryTileFront && (0 > geometryTile.fHeight[1] || 0 > geometryTile.fHeight[3])) facingDirection = ::gme::TILE_FACE_FACING_BACK;
		if(facingDirection == ::gme::TILE_FACE_FACING_BACK	&& 0 == geometryTileFront && (0 < geometryTile.fHeight[1] || 0 < geometryTile.fHeight[3])) facingDirection = ::gme::TILE_FACE_FACING_FRONT;
		if(facingDirection == ::gme::TILE_FACE_FACING_RIGHT && 0 == geometryTileRight && (0 > geometryTile.fHeight[2] || 0 > geometryTile.fHeight[3])) facingDirection = ::gme::TILE_FACE_FACING_LEFT;
		if(facingDirection == ::gme::TILE_FACE_FACING_LEFT	&& 0 == geometryTileRight && (0 < geometryTile.fHeight[2] || 0 < geometryTile.fHeight[3])) facingDirection = ::gme::TILE_FACE_FACING_RIGHT;
		if(facingDirection != facing_direction)
			continue;
		switch(facing_direction) {
		default:
			break;
		//case ::gme::TILE_FACE_FACING_BOTTOM: 
		case ::gme::TILE_FACE_FACING_TOP	: 
			if(-1 == faceSkinMapping.SkinIndexTop)
				continue;
			if(textureIndex != -1 && textureIndex != lstTileSkinData[faceSkinMapping.SkinIndexTop].TextureIndex)
				continue;
			::gndGenerateFaceGeometryTop(x, y, 1.0, faceSkinMapping.SkinIndexTop, geometryTile, lstTileSkinData, generated, out_mapping[y][x]);
			break;
		case ::gme::TILE_FACE_FACING_BACK	: 
		case ::gme::TILE_FACE_FACING_FRONT	: 
			if(-1 == faceSkinMapping.SkinIndexFront)
				continue;
			if(textureIndex != -1 && textureIndex != lstTileSkinData[faceSkinMapping.SkinIndexFront].TextureIndex)
				continue;
			::gndGenerateFaceGeometryFront(x, y, 1.0, faceSkinMapping.SkinIndexFront, geometryTile, geometryTileFront, lstTileSkinData, generated, out_mapping[y][x]);
			break;
		case ::gme::TILE_FACE_FACING_LEFT	: 
		case ::gme::TILE_FACE_FACING_RIGHT	: 
			if(-1 == faceSkinMapping.SkinIndexRight)
				continue;
			if(textureIndex != -1 && textureIndex != lstTileSkinData[faceSkinMapping.SkinIndexRight].TextureIndex)
				continue;
			::gndGenerateFaceGeometryRight(x, y, 1.0, faceSkinMapping.SkinIndexRight, geometryTile, geometryTileRight, lstTileSkinData, generated, out_mapping[y][x]);
			break;
		}
	}
	return 0;
}

			::gpk::error_t										gme::blendGNDNormals								(const ::gpk::grid_view<::gme::STileHeights> &tileGeometryView, const ::gpk::grid_view<::gme::STileFaceSkinMapping>	& skinMappingView, const ::gpk::array_view<::gme::STileSkin>& lstTileSkinData, const ::gpk::grid_view<::gme::STileMapping>& tileMappingView, ::gpk::array_view<::gme::SModelNode> & gndModelNodes)																						{
	for(uint32_t y = 0; y < tileGeometryView.metrics().y - 1; ++y) {
		const ::gpk::array_view<const ::gme::STileHeights>					rowTileGeometry										= tileGeometryView	[y];
		const ::gpk::array_view<const ::gme::STileMapping>					rowTileMapping										= tileMappingView	[y];
		const ::gpk::array_view<const ::gme::STileHeights>					rowNextTileGeometry									= tileGeometryView	[y + 1];
		const ::gpk::array_view<const ::gme::STileMapping>					rowNextTileMapping									= tileMappingView	[y + 1];

		const ::gpk::array_view<const ::gme::STileFaceSkinMapping>			rowTileSkin											= skinMappingView	[y];
		const ::gpk::array_view<const ::gme::STileFaceSkinMapping>			rowNextTileSkin										= skinMappingView	[y + 1];
		for(uint32_t x = 0; x < tileGeometryView.metrics().x - 1; ++x) {	
			//const ::gme::STileHeights											& tileGeometry0										= rowTileGeometry		[x];
			//const ::gme::STileHeights											& tileGeometry1										= rowTileGeometry		[x + 1];
			//const ::gme::STileHeights											& tileGeometry2										= rowNextTileGeometry	[x];
			//const ::gme::STileHeights											& tileGeometry3										= rowNextTileGeometry	[x + 1];
			const ::gme::STileFaceSkinMapping									& tileSkin0											= rowTileSkin			[x];
			const ::gme::STileFaceSkinMapping									& tileSkin1											= rowTileSkin			[x + 1];
			const ::gme::STileFaceSkinMapping									& tileSkin2											= rowNextTileSkin		[x];
			const ::gme::STileFaceSkinMapping									& tileSkin3											= rowNextTileSkin		[x + 1];

			const ::gme::STileMapping											& tileMapping0										= rowTileMapping		[x + 0];
			const ::gme::STileMapping											& tileMapping1										= rowTileMapping		[x + 1];
			const ::gme::STileMapping											& tileMapping2										= rowNextTileMapping	[x + 0];
			const ::gme::STileMapping											& tileMapping3										= rowNextTileMapping	[x + 1];

			const bool															processTile0										= (tileSkin0.SkinIndexTop != -1);// && (applicationInstance.GNDData.lstTileTextureData[tileGeometry0.SkinMapping.SkinIndexTop].TextureIndex != -1);
			const bool															processTile1										= (tileSkin1.SkinIndexTop != -1) && tileSkin0.SkinIndexFront == -1;// && (applicationInstance.GNDData.lstTileTextureData[tileGeometry1.SkinMapping.SkinIndexTop].TextureIndex != -1);
			const bool															processTile2										= (tileSkin2.SkinIndexTop != -1) && tileSkin0.SkinIndexRight == -1;// && (applicationInstance.GNDData.lstTileTextureData[tileGeometry2.SkinMapping.SkinIndexTop].TextureIndex != -1);
			const bool															processTile3										= (tileSkin3.SkinIndexTop != -1) && (tileSkin0.SkinIndexFront == -1 && tileSkin0.SkinIndexRight == -1);// && (applicationInstance.GNDData.lstTileTextureData[tileGeometry3.SkinMapping.SkinIndexTop].TextureIndex != -1);
			const int32_t														texIndex0											= processTile0 ? lstTileSkinData[tileSkin0.SkinIndexTop].TextureIndex : -1;
			const int32_t														texIndex1											= processTile1 ? lstTileSkinData[tileSkin1.SkinIndexTop].TextureIndex : -1;
			const int32_t														texIndex2											= processTile2 ? lstTileSkinData[tileSkin2.SkinIndexTop].TextureIndex : -1;
			const int32_t														texIndex3											= processTile3 ? lstTileSkinData[tileSkin3.SkinIndexTop].TextureIndex : -1;
			::gpk::SCoord3<float>												normal												= {};
			uint32_t															divisor												= 0;
			if(processTile0) { ++divisor; ::gme::SModelNode & gndNode0 = gndModelNodes[texIndex0]; normal += gndNode0.Normals[tileMapping0.VerticesTop[3]]; }
			if(processTile1) { ++divisor; ::gme::SModelNode & gndNode1 = gndModelNodes[texIndex1]; normal += gndNode1.Normals[tileMapping1.VerticesTop[2]]; }
			if(processTile2) { ++divisor; ::gme::SModelNode & gndNode2 = gndModelNodes[texIndex2]; normal += gndNode2.Normals[tileMapping2.VerticesTop[1]]; }
			if(processTile3) { ++divisor; ::gme::SModelNode & gndNode3 = gndModelNodes[texIndex3]; normal += gndNode3.Normals[tileMapping3.VerticesTop[0]]; }
			if(divisor) {
				(normal /= divisor).Normalize();
				if(processTile0) { ::gme::SModelNode & gndNode0 = gndModelNodes[texIndex0]; gndNode0.Normals[tileMapping0.VerticesTop[3]] = normal; }
				if(processTile1) { ::gme::SModelNode & gndNode1 = gndModelNodes[texIndex1]; gndNode1.Normals[tileMapping1.VerticesTop[2]] = normal; }
				if(processTile2) { ::gme::SModelNode & gndNode2 = gndModelNodes[texIndex2]; gndNode2.Normals[tileMapping2.VerticesTop[1]] = normal; }
				if(processTile3) { ::gme::SModelNode & gndNode3 = gndModelNodes[texIndex3]; gndNode3.Normals[tileMapping3.VerticesTop[0]] = normal; }
			}
		}
	}
	return 0;
}
