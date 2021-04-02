//#############################################################################
//
// PARAMETERS
//
//#############################################################################

float4x4 InvWorldViewProjection;

textureCUBE EnvMap;

samplerCUBE EnvSampler = sampler_state
{ 
    Texture   = <EnvMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//-----------------------------------------------------------------------------
// Skybox stuff
//-----------------------------------------------------------------------------
struct SkyboxVS_Input
{
    float4 Pos : POSITION;
};

struct SkyboxVS_Output
{
    float4 Pos : POSITION;
    float3 Tex : TEXCOORD0;
};

//#############################################################################
//
// SHADERS
//
//#############################################################################

SkyboxVS_Output VS_Skybox( SkyboxVS_Input Input )
{
    SkyboxVS_Output Output;
    
    Output.Pos = Input.Pos;
    Output.Tex = normalize( mul(Input.Pos, InvWorldViewProjection) );
    
    return Output;
}

float4 PS_Skybox(SkyboxVS_Output Input) : COLOR
{
    float4 color = texCUBE( EnvSampler, Input.Tex );
    return color;
}


//#############################################################################
//
// TECHNIQUES
//
//#############################################################################

technique RenderSkybox
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS_Skybox();
        PixelShader  = compile ps_2_0 PS_Skybox();
    }
}


