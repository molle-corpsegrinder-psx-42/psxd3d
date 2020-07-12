#include "shader.hlsl"

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input ) : SV_Target
{
   //return input.Color;
	
	// Interpolating normal can unnormalize it, so normalize it.
	input.Norm = normalize(input.Norm);

	float3 toEyeW = normalize(vCamera - input.PosW);

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	float4 A, D, S;

	/*
	float4 finalColor = 0;
    //do NdotL lighting for 2 lights	
    for(int i=0; i<2; i++)
    {
        finalColor += saturate( dot( (float3)DirLight[i].Direction,input.Norm) * DirLight[i].Diffuse );
		
    }
	*/
	
	
    for(int i=0; i<2; i++)
    {		
        //finalColor += saturate( dot( (float3)vLightDir[i],input.Norm) * vLightColor[i] );
		
		ComputeDirectionalLight(d3dMaterial, DirLight[i], input.Norm, toEyeW, A, D, S);

		ambient += A;  
		diffuse += D;
		spec    += S;
	}
	
	
	//ComputePointLight(d3dMaterial, _PointLight, input.PosW, input.Norm, toEyeW, A, D, S);
	//ambient += A;
	//diffuse += D;
	//spec    += S;
	

	
	ComputeSpotLight(d3dMaterial, _SpotLight, input.PosW, input.Norm, toEyeW, A, D, S);
	ambient += A;
	diffuse += D;
	spec    += S;
	

	// The vector from the surface to the light.
	//float3 lightVec = _SpotLight.Position - input.Pos;

	// The distance from surface to light.
	//float d = length(lightVec);

	// Normalize the light vector.
	//lightVec /= d; 

	//float diffuseFactor = dot(toEyeW, input.Norm);
	//diffuse = diffuse + diffuseFactor * _SpotLight.Diffuse;
		

	//float4 finalColor = saturate(ambient + diffuse + spec);
	float4 finalColor = ambient + diffuse + spec;

	// Common to take alpha from diffuse material.
   finalColor.a = 1;


   return finalColor *txDiffuse.Sample(samLinear, input.Tex) + txEmissive.Sample(samLinear, input.Tex);
	
}


