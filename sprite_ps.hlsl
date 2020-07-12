#include "shader.hlsl"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( SPRITE_PS_INPUT input ) : SV_Target
{
    return txDiffuse.Sample( samLinear, input.Tex ) * input.opacity;	
	
}


