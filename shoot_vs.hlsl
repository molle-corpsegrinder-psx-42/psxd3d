#include "shader.hlsl"


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
SHOOTPOS VS( SHOOTPOS input )
{
    SHOOTPOS output = (SHOOTPOS)0;
	
	// Transform to homogeneous clip space.
    output.Pos = mul( input.Pos, g_matWorldViewProj );
	output.NextPos = mul(input.NextPos, g_matWorldViewProj);
		
    return output;
}


