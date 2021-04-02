//#############################################################################
//
// PARAMETERS
//
//#############################################################################

static const half  blurBlendFactor   = 0.25f;
static const half3 LUMINANCE_WEIGHTS = half3(0.27f, 0.67f, 0.06f);

float   Exposure;
float   FrameDelta;

#define MAX_SAMPLES 16

// Contains weights for Gaussian blurring
float  weights[MAX_SAMPLES];

//#############################################################################
//
// SAMPLERS
//
//#############################################################################
texture TextureA;
texture TextureB;
texture TextureC;
texture ScaledTexA;
texture ScaledTexB;

sampler2D AnisoSampler = sampler_state
{
    Texture       = <TextureA>;
    AddressU      = CLAMP;
    AddressV      = CLAMP;
    MinFilter     = ANISOTROPIC;
    MagFilter     = LINEAR;
    MipFilter     = NONE;
    MaxAnisotropy = 16;
};

sampler2D SamplerTex = sampler_state 
{
    Texture   = <TextureA>;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler2D SamplerTexC = sampler_state 
{
    Texture   = <TextureC>;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler2D SamplerA = sampler_state 
{
    Texture     = <TextureA>;
    AddressU    = WRAP;
    AddressV    = WRAP;
    MinFilter   = LINEAR;
    MagFilter   = LINEAR;
    MipFilter   = LINEAR;
    SRGBTexture = TRUE;
};

sampler2D SamplerB = sampler_state 
{
    Texture     = <TextureB>;
    AddressU    = WRAP;
    AddressV    = WRAP;
    MinFilter   = LINEAR;
    MagFilter   = LINEAR;
    MipFilter   = LINEAR;
    SRGBTexture = TRUE;
};

sampler2D SamplerScaledTexA = sampler_state 
{
    Texture     = <ScaledTexA>;
    AddressU    = CLAMP;
    AddressV    = CLAMP;
    MinFilter   = LINEAR;
    MagFilter   = LINEAR;
    MipFilter   = LINEAR;
    SRGBTexture = TRUE;
};

sampler2D SamplerScaledTexB = sampler_state 
{
    Texture     = <ScaledTexB>;
    AddressU    = CLAMP;
    AddressV    = CLAMP;
    MinFilter   = LINEAR;
    MagFilter   = LINEAR;
    MipFilter   = LINEAR;
    SRGBTexture = TRUE;
};

//#############################################################################
struct PS_Input_4 {
  float4 texCoord[2] : TEXCOORD0;
};

struct PS_Input_8 {
  float4 texCoord[4] : TEXCOORD0;
};

struct VS_OutputVTF
{
    float4 Pos : POSITION;
    float3 Tex : TEXCOORD0;
};

//#############################################################################
//
// VIGNETTING
//
//#############################################################################
void vignette(inout float3 c, const float2 win_bias)
{
	// convert window coord to [-1, 1] range
	float2 wpos = 2*(win_bias - (float2)(0.5, 0.5)); 

	// calculate distance from origin
	float r = length(wpos.xy);
	r = 1.0 - smoothstep(0.8, 1.5, r);
	c = c * r;
}

//#############################################################################
//
// TONE MAPPING
//
//#############################################################################
VS_OutputVTF VS_toneMapping(in float4 position: POSITION)
{
	VS_OutputVTF OUT;
	
	OUT = (VS_OutputVTF)0;

	OUT.Tex.z  = Exposure / max(0.001f, tex2Dlod(SamplerTexC, float4(0.5f, 0.5f, 0.0f, 0.0f)).x);
		
	OUT.Pos    = float4(position.x, position.y, 0.5f, 1.0f);
	OUT.Tex.xy = position.zw;
	
	return OUT;
}

float4 PS_toneMapping(in float3 Tex: TEXCOORD0) : COLOR
{
    // sum original and blurred image
    half3 c = (1.0f - blurBlendFactor) * tex2D(SamplerA, Tex.xy).rgb +  blurBlendFactor * tex2D(SamplerScaledTexA, Tex.xy).rgb;
    
    half3 L = c * Tex.z;
    
    // exposure
    c = L / (1 + L); 
    
    // vignette effect (makes brightness drop off with distance from center)
    vignette(c, Tex.xy); 
  
    return float4(c, 1.0);
}

float4 PS_toneMappingx2(in float3 Tex: TEXCOORD0) : COLOR
{
    // sum original and blurred image
    half3 c;
  
  	c.rg = (1.0f - blurBlendFactor) * tex2D(SamplerA, Tex.xy).rg +  blurBlendFactor * tex2D(SamplerScaledTexA, Tex.xy).rg;
	c.b  = (1.0f - blurBlendFactor) * tex2D(SamplerB, Tex.xy).r  +  blurBlendFactor * tex2D(SamplerScaledTexB, Tex.xy).r;

    half3 L = c * Tex.z;
    
    // exposure
    c = L / (1 + L); 
    
    // vignette effect (makes brightness drop off with distance from center)
 	vignette(c, Tex.xy); 

    return float4(c, 1.0);
}

//===================================================================
technique ToneMapping
{
    Pass Main
    {
		VertexShader = compile vs_3_0 VS_toneMapping();
		PixelShader  = compile ps_2_0 PS_toneMapping();
    }
}

//===================================================================
technique ToneMappingx2
{
    Pass Main
    {  
		VertexShader = compile vs_3_0 VS_toneMapping();
        PixelShader  = compile ps_2_0 PS_toneMappingx2();
    }
}

//#############################################################################
//
// BLUR
//
//#############################################################################
float4 PS_downScale4x4(in PS_Input_4 IN) : COLOR
{
	half4 sample = 0.0f;

	for( int i = 0; i < 2; i++ )
	{
		sample += tex2D( SamplerTex, IN.texCoord[i].xy);
		sample += tex2D( SamplerTex, IN.texCoord[i].zw);
	}
	
	sample *= 0.25f;
	
	return sample;
	
}

float4 PS_downScaleAniso(in float2 Tex: TEXCOORD0) : COLOR
{
	
	return tex2D( AnisoSampler, Tex.xy);
	
}

float4 PS_blurBilinear(in PS_Input_8 IN) : COLOR
{
    
    half4 sample = 0.0f;

	for( int i = 0; i < 4; i++ )
	{
		sample += weights[2*i + 0] * tex2D( SamplerTex, IN.texCoord[i].xy);
		sample += weights[2*i + 1] * tex2D( SamplerTex, IN.texCoord[i].zw);
	}
	
	return sample;
}

//=============================================================
technique Downscale4x4Bilinear
{
    pass Main
    {
        PixelShader  = compile ps_2_0 PS_downScale4x4();
    }
}

//=============================================================
technique DownscaleAniso
{
    pass Main
    {
        PixelShader  = compile ps_2_0 PS_downScaleAniso();
    }
}

//=============================================================
technique Blur1DBilinear
{
    Pass Main
    {
       PixelShader = compile ps_2_0 PS_blurBilinear();
    }
}

//#############################################################################
//
// LUMINANCE
//
//#############################################################################
float4 PS_downscaleLuminanceLog(in float4 Tex[2]: TEXCOORD0) : COLOR
{
	half luminance = 0; 
		
	for( int i = 0; i < 2; i++ )
	{
		luminance += log(dot(tex2D( SamplerA, Tex[i].xy), LUMINANCE_WEIGHTS)+ 0.0001);
		luminance += log(dot(tex2D( SamplerA, Tex[i].zw), LUMINANCE_WEIGHTS)+ 0.0001);
	}
	
	luminance *= 0.25f;
	
	return float4(luminance, 0.0f, 0.0f, 0.0f);
}

float4 PS_downscaleLuminanceLogx2(in float4 Tex[2]: TEXCOORD0) : COLOR
{
    half3 color;
    half luminance = 0; 
    
    for( int i = 0; i < 2; i++ )
	{
		color.xy = tex2D(SamplerA, Tex[i].xy).xy;
		color.z  = tex2D(SamplerB, Tex[i].xy).x;
    
		luminance += log(dot(color, LUMINANCE_WEIGHTS) + 0.0001);
		
		color.xy = tex2D(SamplerA, Tex[i].zw).xy;
		color.z  = tex2D(SamplerB, Tex[i].zw).x;
    
		luminance += log(dot(color, LUMINANCE_WEIGHTS) + 0.0001);
	
	}
	luminance *= 0.25f;
		
	return float4(luminance, 0.0f, 0.0f, 0.0f);
}

float4 PS_downscaleLuminance(in PS_Input_4 IN) : COLOR
{
	half sample = 0.0f;

	for( int i = 0; i < 2; i++ )
	{
		sample += tex2D( SamplerA, IN.texCoord[i].xy).x;
		sample += tex2D( SamplerA, IN.texCoord[i].zw).x;
	}
	
	sample *= 0.25f;
	
	return sample;
	
}

float4 PS_downscaleLuminanceExp(in PS_Input_4 IN) : COLOR
{
	half sample = 0.0f;

	for( int i = 0; i < 2; i++ )
	{
		sample += tex2D( SamplerA, IN.texCoord[i].xy).x;
		sample += tex2D( SamplerA, IN.texCoord[i].zw).x;
	}
	
	sample *= 0.25f;
	
	return exp(sample);
}

float4 PS_LuminanceAdaptation(in float2 Tex: TEXCOORD0) : COLOR
{
	half adaptedLum = tex2D(SamplerA, float2(0.5f, 0.5f));
    half currentLum = tex2D(SamplerB, float2(0.5f, 0.5f));
   
    return (adaptedLum + (currentLum - adaptedLum) * ( 1 - pow( 0.98f, 50 * FrameDelta ) ));
}

//=============================================================
technique DownscaleLuminanceLog
{
    Pass Main
    {
       PixelShader = compile ps_2_0 PS_downscaleLuminanceLog();
    }
}

//=============================================================
technique DownscaleLuminanceLogx2
{
    Pass Main
    {
       PixelShader = compile ps_2_0 PS_downscaleLuminanceLogx2();
    }
}

//=============================================================
technique DownscaleLuminance
{
    Pass Main
    {
       PixelShader = compile ps_2_0 PS_downscaleLuminance();
    }
}

//=============================================================
technique DownscaleLuminanceExp
{
    Pass Main
    {
       PixelShader = compile ps_2_0 PS_downscaleLuminanceExp();
    }
}

//=============================================================
technique LuminanceAdaptation
{
    Pass Main
    {
       PixelShader = compile ps_2_0 PS_LuminanceAdaptation();
    }
}
