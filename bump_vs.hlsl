#include "shader.hlsl"


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;	
	output.PosW = input.Pos; // mul(input.Pos, g_matWorld);
	// -- Transform to world space space.
	// -- output.Norm = mul(input.Norm, World ).xyz;
	output.Norm = input.Norm.xyz;

	// Transform to homogeneous clip space.
    output.Pos = mul( input.Pos, g_matWorldViewProj );
	
	//output.Norm = normalize(input.Pos);	
    //output.Color = input.Color;

    output.Tex = input.Tex;
    return output;
}


