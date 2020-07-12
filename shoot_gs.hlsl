#include "shader.hlsl"
//struct SHOOTPOS
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

static const float g_fDimension = 2.0f;
static const float g_fHalfDimension = 1.0f;


//--------------------------------------------------------------------------------------
// Geometry Shader 
//--------------------------------------------------------------------------------------
[maxvertexcount(8)]
void GS(point SHOOTPOS gsinputSpriteCoord[1], inout TriangleStream<PS_INPUT> triStream)
{
	PS_INPUT psinputCopy;
    float l_fHalfDimension;

    float2 l_vPos, l_vNextPos;
    l_vPos = gsinputSpriteCoord[0].Pos.xy/gsinputSpriteCoord[0].Pos.w;
    l_vNextPos = gsinputSpriteCoord[0].NextPos.xy/gsinputSpriteCoord[0].NextPos.w;


	if (abs(l_vNextPos.x - l_vPos.x) > abs(l_vNextPos.y - l_vPos.y))
    {

	    if (l_vPos.x > l_vNextPos.x)
		{
			l_fHalfDimension = g_fHalfDimension;
		}else{
			l_fHalfDimension = -g_fHalfDimension;
		}
	

	    //bottom left far	
	    psinputCopy.Pos = float4(gsinputSpriteCoord[0].NextPos.x-l_fHalfDimension,
	                             gsinputSpriteCoord[0].NextPos.y-g_fDimension,
	                             gsinputSpriteCoord[0].NextPos.z,
	                             gsinputSpriteCoord[0].NextPos.w);
	    psinputCopy.PosW = psinputCopy.Pos;
	    psinputCopy.Tex = float2(0,1);
	    psinputCopy.Norm = float3(1,0,0);
	    triStream.Append(psinputCopy);
	 
	    //top left far
	    psinputCopy.Pos = float4(gsinputSpriteCoord[0].NextPos.x-l_fHalfDimension,
	                             gsinputSpriteCoord[0].NextPos.y+g_fDimension,
	                             gsinputSpriteCoord[0].NextPos.z,
	                             gsinputSpriteCoord[0].NextPos.w);
	    psinputCopy.PosW = psinputCopy.Pos;
	    psinputCopy.Tex = float2(0,0);
	    psinputCopy.Norm = float3(1,0,0);
	    triStream.Append(psinputCopy);
	
		//bottom middle far
		psinputCopy.Pos = float4(gsinputSpriteCoord[0].NextPos.x,
								 gsinputSpriteCoord[0].NextPos.y - g_fDimension,
								 gsinputSpriteCoord[0].NextPos.z,
								 gsinputSpriteCoord[0].NextPos.w);
		psinputCopy.PosW = psinputCopy.Pos;
		psinputCopy.Tex = float2(.5, 1);
		psinputCopy.Norm = float3(1, 0, 0);
		triStream.Append(psinputCopy);
	
		//top middle far
		psinputCopy.Pos = float4(gsinputSpriteCoord[0].NextPos.x,
								 gsinputSpriteCoord[0].NextPos.y + g_fDimension,
								 gsinputSpriteCoord[0].NextPos.z,
								 gsinputSpriteCoord[0].NextPos.w);
		psinputCopy.PosW = psinputCopy.Pos;
		psinputCopy.Tex = float2(.5, 0);
		psinputCopy.Norm = float3(1, 0, 0);
		triStream.Append(psinputCopy);
	
		//bottom middle near	
		psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x,
								 gsinputSpriteCoord[0].Pos.y - g_fDimension,
								 gsinputSpriteCoord[0].Pos.z,
								 gsinputSpriteCoord[0].Pos.w);
		psinputCopy.PosW = psinputCopy.Pos;
		psinputCopy.Tex = float2(.5, 1);
		psinputCopy.Norm = float3(1, 0, 0);
		triStream.Append(psinputCopy);
	
		//top middle near
		psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x,
								 gsinputSpriteCoord[0].Pos.y + g_fDimension,
								 gsinputSpriteCoord[0].Pos.z,
								 gsinputSpriteCoord[0].Pos.w);
		psinputCopy.PosW = psinputCopy.Pos;
		psinputCopy.Tex = float2(.5, 0);
		psinputCopy.Norm = float3(1, 0, 0);
		triStream.Append(psinputCopy);
	
	    //bottom right near
	    psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x+l_fHalfDimension,
	                             gsinputSpriteCoord[0].Pos.y-g_fDimension,
	                             gsinputSpriteCoord[0].Pos.z,
	                             gsinputSpriteCoord[0].Pos.w);
	    psinputCopy.PosW = psinputCopy.Pos;
	    psinputCopy.Tex = float2(1,1);
	    psinputCopy.Norm = float3(1,0,0);
	    triStream.Append(psinputCopy);
	 
	    //top right near
	    psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x+ l_fHalfDimension,
	                             gsinputSpriteCoord[0].Pos.y+ g_fDimension,
	                             gsinputSpriteCoord[0].Pos.z,
	                             gsinputSpriteCoord[0].Pos.w);
	    psinputCopy.PosW = psinputCopy.Pos;
	    psinputCopy.Norm = float3(1,0,0);
	    psinputCopy.Tex = float2(1,0);
	    triStream.Append(psinputCopy);
	
	}else{
	    if (l_vPos.y > l_vNextPos.y)
		{
			l_fHalfDimension = g_fHalfDimension;
		}else{
			l_fHalfDimension = -g_fHalfDimension;
		}
	
	    psinputCopy.Pos = float4(gsinputSpriteCoord[0].NextPos.x-g_fDimension,
	                             gsinputSpriteCoord[0].NextPos.y-l_fHalfDimension,
	                             gsinputSpriteCoord[0].NextPos.z,
	                             gsinputSpriteCoord[0].NextPos.w);
	    psinputCopy.PosW = psinputCopy.Pos;
	    psinputCopy.Tex = float2(0,1);
	    psinputCopy.Norm = float3(1,0,0);
	    triStream.Append(psinputCopy);
	 
	    psinputCopy.Pos = float4( gsinputSpriteCoord[0].NextPos.x+g_fDimension,
                                 gsinputSpriteCoord[0].NextPos.y-l_fHalfDimension,
	                             gsinputSpriteCoord[0].NextPos.z,
	                             gsinputSpriteCoord[0].NextPos.w);
	    psinputCopy.PosW = psinputCopy.Pos;
	    psinputCopy.Tex = float2(0,0);
	    psinputCopy.Norm = float3(1,0,0);
	    triStream.Append(psinputCopy);
	
		psinputCopy.Pos = float4(gsinputSpriteCoord[0].NextPos.x - g_fDimension,
                                 gsinputSpriteCoord[0].NextPos.y,
								 gsinputSpriteCoord[0].NextPos.z,
								 gsinputSpriteCoord[0].NextPos.w);
		psinputCopy.PosW = psinputCopy.Pos;
		psinputCopy.Tex = float2(.5, 1);
		psinputCopy.Norm = float3(1, 0, 0);
		triStream.Append(psinputCopy);
	
		psinputCopy.Pos = float4(gsinputSpriteCoord[0].NextPos.x + g_fDimension,
                                 gsinputSpriteCoord[0].NextPos.y,
								 gsinputSpriteCoord[0].NextPos.z,
								 gsinputSpriteCoord[0].NextPos.w);
		psinputCopy.PosW = psinputCopy.Pos;
		psinputCopy.Tex = float2(.5, 0);
		psinputCopy.Norm = float3(1, 0, 0);
		triStream.Append(psinputCopy);
	
		psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x - g_fDimension,
                                 gsinputSpriteCoord[0].Pos.y,
								 gsinputSpriteCoord[0].Pos.z,
								 gsinputSpriteCoord[0].Pos.w);
		psinputCopy.PosW = psinputCopy.Pos;
		psinputCopy.Tex = float2(.5, 1);
		psinputCopy.Norm = float3(1, 0, 0);
		triStream.Append(psinputCopy);
	
		psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x + g_fDimension,
                                 gsinputSpriteCoord[0].Pos.y,
								 gsinputSpriteCoord[0].Pos.z,
								 gsinputSpriteCoord[0].Pos.w);
		psinputCopy.PosW = psinputCopy.Pos;
		psinputCopy.Tex = float2(.5, 0);
		psinputCopy.Norm = float3(1, 0, 0);
		triStream.Append(psinputCopy);
	
	    psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x-g_fDimension,
                                 gsinputSpriteCoord[0].Pos.y+l_fHalfDimension,
	                             gsinputSpriteCoord[0].Pos.z,
	                             gsinputSpriteCoord[0].Pos.w);
	    psinputCopy.PosW = psinputCopy.Pos;
	    psinputCopy.Tex = float2(1,1);
	    psinputCopy.Norm = float3(1,0,0);
	    triStream.Append(psinputCopy);
	 
	    psinputCopy.Pos = float4(gsinputSpriteCoord[0].Pos.x+ g_fDimension,
                                 gsinputSpriteCoord[0].Pos.y+ l_fHalfDimension,
	                             gsinputSpriteCoord[0].Pos.z,
	                             gsinputSpriteCoord[0].Pos.w);
	    psinputCopy.PosW = psinputCopy.Pos;
	    psinputCopy.Norm = float3(1,0,0);
	    psinputCopy.Tex = float2(1,0);
	    triStream.Append(psinputCopy);
	}
}
