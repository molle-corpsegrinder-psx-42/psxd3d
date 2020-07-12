#include "shader.hlsl"
//struct SPRITE
//{
//
//	float2 topLeft : ANCHOR;
//	float2 dimensions : DIMENSIONS;
//	float opacity : OPACITY;
//};
//struct PS_INPUT
//{
//	float4 PosW: POSITION;
//	float4 Pos : SV_POSITION;
//	//float4 Color : COLOR;
//	float3 Norm : COLOR;
//	float2 Tex : TEXCOORD0;
//};



//--------------------------------------------------------------------------------------
// Geometry Shader 
//--------------------------------------------------------------------------------------
[maxvertexcount(8)]
void GS(point EXPLOSIONPOS gsinputSpriteCoord[1], inout TriangleStream<PS_INPUT> triStream)
{
	PS_INPUT psinputCopy;

/*
   float2 l_vPos, l_vNextPos;
   l_vPos = gsinputSpriteCoord[0].Pos.xy/gsinputSpriteCoord[0].Pos.w;
   l_vNextPos = gsinputSpriteCoord[0].NextPos.xy/gsinputSpriteCoord[0].NextPos.w;
*/


	//top left 
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x,
									gsinputSpriteCoord[0].Pos.y+m_fScaleBillboard,
									gsinputSpriteCoord[0].Pos.z,
									gsinputSpriteCoord[0].Pos.w);
	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(0,0);
	psinputCopy.Norm = float3(1,0,0);
	triStream.Append(psinputCopy);

	//bottom left 
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x,
							 gsinputSpriteCoord[0].Pos.y-m_fScaleBillboard,
							 gsinputSpriteCoord[0].Pos.z,
							 gsinputSpriteCoord[0].Pos.w);
	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(0,1);
	psinputCopy.Norm = float3(1,0,0);
	triStream.Append(psinputCopy);
	 
	
	//bottom right 
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x + m_fScaleBillboard,
							 gsinputSpriteCoord[0].Pos.y - m_fScaleBillboard,
							 gsinputSpriteCoord[0].Pos.z,
							 gsinputSpriteCoord[0].Pos.w);
	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(1, 1);
	psinputCopy.Norm = float3(1, 0, 0);
	triStream.Append(psinputCopy);

	//top left 
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x,
							 gsinputSpriteCoord[0].Pos.y+m_fScaleBillboard,
							 gsinputSpriteCoord[0].Pos.z,
							 gsinputSpriteCoord[0].Pos.w);
	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(0,0);
	psinputCopy.Norm = float3(1,0,0);
	triStream.Append(psinputCopy);

	//top right 
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x + m_fScaleBillboard,
							 gsinputSpriteCoord[0].Pos.y + m_fScaleBillboard,
							 gsinputSpriteCoord[0].Pos.z,
							 gsinputSpriteCoord[0].Pos.w);
	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(1, 0);
	psinputCopy.Norm = float3(1, 0, 0);
	triStream.Append(psinputCopy);

	//bottom right 
	psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x + m_fScaleBillboard,
							 gsinputSpriteCoord[0].Pos.y - m_fScaleBillboard,
							 gsinputSpriteCoord[0].Pos.z,
							 gsinputSpriteCoord[0].Pos.w);
	psinputCopy.PosW = psinputCopy.Pos;
	psinputCopy.Tex = float2(1, 1);
	psinputCopy.Norm = float3(1, 0, 0);
	triStream.Append(psinputCopy);

}
