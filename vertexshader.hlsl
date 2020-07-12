#include "shader.hlsl"


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	// Transform to world space space.
	output.PosW = mul(input.Pos, g_matWorld);
	output.Norm = mul(input.Norm, g_matWorld ).xyz;

	// Transform to homogeneous clip space.
    output.Pos = mul( input.Pos, g_matWorldViewProj );

	//output.Norm = normalize(input.Pos);	
    //output.Color = input.Color;

    output.Tex = input.Tex;
    return output;
}


