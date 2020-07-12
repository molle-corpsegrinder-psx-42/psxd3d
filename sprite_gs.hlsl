#include "shader.hlsl"
//struct SPRITE
//{
//
//	float2 topLeft : ANCHOR;
//	float2 dimensions : DIMENSIONS;
//	float opacity : OPACITY;
//};

//struct SPRITE_PS_INPUT
//{
//    //float4 Color : COLOR;
//	float2 Tex : TEXCOORD0;
//	float opacity : OPACITY;
//};

//static const float g_fDimension = 2.0f;
//static const float g_fHalfDimension = 1.0f;


//--------------------------------------------------------------------------------------
// Geometry Shader 
//--------------------------------------------------------------------------------------
[maxvertexcount(8)]
void GS(point SPRITE gsinputSpriteCoord[1], inout TriangleStream<SPRITE_PS_INPUT> triStream)
{
	SPRITE_PS_INPUT psinputCopy = (SPRITE_PS_INPUT) 0;

/*
   float2 l_vPos, l_vTopLeft;
   l_vPos = gsinputSpriteCoord[0].Pos.xy/gsinputSpriteCoord[0].Pos.w;
   l_vTopLeft = gsinputSpriteCoord[0].TopLeft.xy/gsinputSpriteCoord[0].TopLeft.w;
*/
//   float l_fHalfDimension = 
	//triStream.RestartStrip();

	//top left 
	psinputCopy.opacity = gsinputSpriteCoord[0].opacity;
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x - gsinputSpriteCoord[0].dimension.x * 0.5f,
									gsinputSpriteCoord[0].Pos.y+gsinputSpriteCoord[0].dimension.y * 0.5f,
									gsinputSpriteCoord[0].Pos.z,
									gsinputSpriteCoord[0].Pos.w);
//	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(0,0);
//	psinputCopy.Norm = float3(1,0,0);
	triStream.Append(psinputCopy);

	//bottom left 
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x - gsinputSpriteCoord[0].dimension.x * 0.5f,
							 gsinputSpriteCoord[0].Pos.y-gsinputSpriteCoord[0].dimension.y * 0.5f,
							 gsinputSpriteCoord[0].Pos.z,
							 gsinputSpriteCoord[0].Pos.w);
//	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(0,1);
//	psinputCopy.Norm = float3(1,0,0);
	triStream.Append(psinputCopy);
	 
	
	//bottom right 
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x + gsinputSpriteCoord[0].dimension.x *0.5f,
							 gsinputSpriteCoord[0].Pos.y - gsinputSpriteCoord[0].dimension.y * 0.5f,
							 gsinputSpriteCoord[0].Pos.z,
							 gsinputSpriteCoord[0].Pos.w);
//	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(1, 1);
//	psinputCopy.Norm = float3(1, 0, 0);
	triStream.Append(psinputCopy);

	//top left 
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x - gsinputSpriteCoord[0].dimension.x * 0.5f,
							 gsinputSpriteCoord[0].Pos.y+gsinputSpriteCoord[0].dimension.y * 0.5f,
							 gsinputSpriteCoord[0].Pos.z,
							 gsinputSpriteCoord[0].Pos.w);
//	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(0,0);
//	psinputCopy.Norm = float3(1,0,0);
	triStream.Append(psinputCopy);

	//top right 
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x + gsinputSpriteCoord[0].dimension.x *0.5f,
							 gsinputSpriteCoord[0].Pos.y + gsinputSpriteCoord[0].dimension.y * 0.5f,
							 gsinputSpriteCoord[0].Pos.z,
							 gsinputSpriteCoord[0].Pos.w);
//	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(1, 0);
//	psinputCopy.Norm = float3(1, 0, 0);
	triStream.Append(psinputCopy);

	//bottom right 
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x + gsinputSpriteCoord[0].dimension.x *0.5f,
							 gsinputSpriteCoord[0].Pos.y - gsinputSpriteCoord[0].dimension.y * 0.5f,
							 gsinputSpriteCoord[0].Pos.z,
							 gsinputSpriteCoord[0].Pos.w);
//	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(1, 1);
//	psinputCopy.Norm = float3(1, 0, 0);
	triStream.Append(psinputCopy);

}
