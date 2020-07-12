#include "shader.hlsl"


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
SPRITE VS( SPRITE input )
{
	SPRITE output = input;
	
	// Transform to homogeneous clip space.
	output.Pos = mul( input.Pos, g_matWorldViewProj );
//	output.dimension = input.dimension;
//	output.opacity = input.opacity;
	return output;
	//return input;
}


