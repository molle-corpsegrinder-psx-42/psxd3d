#include "shader.hlsl"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
	
	// Interpolating normal can unnormalize it, so normalize it.
	input.Norm = normalize(input.Norm);


	float4 finalColor = 0;
    //do NdotL lighting for 2 lights	
    for(int i=0; i<2; i++)
    {
        finalColor += saturate( dot( (float3)DirLight[i].Direction,input.Norm) * DirLight[i].Diffuse );
		
    }
	
	// Common to take alpha from diffuse material.
    finalColor.a = 1;

    return finalColor * txDiffuse.Sample( samLinear, input.Tex );	
	
}


