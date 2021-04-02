//#############################################################################
//
// PARAMETERS
//
//#############################################################################

#define AIR_REF_INDEX          1.0f
#define GLASS_REF_INDEX        1.61f
#define WATER_REF_INDEX        1.33f
#define DIAMOND_REF_INDEX      2.42f
#define ZIRCONIUM_REF_INDEX    2.17f

// The per-color weighting to be used for luminance calculations in RGB order.
static const half3 MODEL_COLOR        = half3(0.2f, 0.2f, 0.3f);
static const half  EtaValue           = AIR_REF_INDEX / WATER_REF_INDEX;
static const half  R0                 = pow(1.0-EtaValue, 2.0) / pow(1.0+EtaValue, 2.0);

float4x4 WorldViewProjection;
float4x4 World;
float3   EyePosition;

textureCUBE EnvMapF;
textureCUBE EnvMapA;
textureCUBE EnvMapB;

textureCUBE DifMapF;
textureCUBE DifMapA;
textureCUBE DifMapB;

// Environment map samplers
samplerCUBE EnvSamplerF = sampler_state
{ 
    Texture   = <EnvMapF>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

samplerCUBE EnvSamplerA = sampler_state
{ 
    Texture   = <EnvMapA>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

samplerCUBE EnvSamplerB = sampler_state
{ 
    Texture   = <EnvMapB>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// Diffuse map samplers
samplerCUBE DifSamplerF = sampler_state
{ 
    Texture   = <DifMapF>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

samplerCUBE DifSamplerA = sampler_state
{ 
    Texture   = <DifMapA>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

samplerCUBE DifSamplerB = sampler_state
{ 
    Texture   = <DifMapB>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

// Shaders IN/OUT formats
struct VS_Out {
    float4 Pos	        : POSITION;
    float3 WorldNormal	: TEXCOORD0;
    float3 WorldEyeVec	: TEXCOORD1;
};

struct VS_In 
{
    float3 Pos          : POSITION; //in object space
    float3 Norm         : NORMAL;   //in object space
    float3 WorldNormal	: TEXCOORD0;
    float3 WorldEyeVec	: TEXCOORD1;
};

struct PS_MRTOut
{
	float4 Color0 : COLOR0;
	float4 Color1 : COLOR1;
};

//#############################################################################
//
// SUPPORT FUNCTIONS
//
//#############################################################################

half3 my_refract(half3 I, half3 N, half eta)
{
  half IdotN = dot(I,N);
  half k = 1 - eta*eta*(1 - IdotN*IdotN);
  return eta*I - (eta*IdotN + sqrt(k))*N;
}

// fresnel approximation
half fresnel(half3 I, half3 N)
{
	half cosAngle = 1-abs(dot(I, N));
	half result   = cosAngle * cosAngle;
	result = result * result;
	result = result * cosAngle;
	result = min(.7, saturate(result * (1 - saturate(R0)) + R0));
	return result;
}


//#############################################################################
//
// VERTEX SHADERS
//
//#############################################################################

VS_Out VS_Main(VS_In In)
{
	VS_Out OUT;

	OUT = (VS_Out)0;
	OUT.Pos = mul(float4(In.Pos, 1.0), WorldViewProjection);
	
	float3 WorldVertPos = mul(In.Pos, World).xyz;

    OUT.WorldEyeVec  = EyePosition - WorldVertPos;
	OUT.WorldNormal  = mul(In.Norm, (float3x3)World).xyz;

	return OUT;
}

//#############################################################################
//
// PIXEL SHADERS
//
//#############################################################################

half4 PS_RenderModel_Reflective(VS_Out In) :COLOR0
{
	half3 V = normalize(half3(In.WorldEyeVec));
    half3 N = normalize(half3(In.WorldNormal));

    half3 R      = reflect(-V, N);
        
    half fresnel = fresnel(V, N);

	half3 Cnatural = texCUBE(DifSamplerF, N).rgb; // lookup reflection in HDR diffuse map
    half3 Creflect = texCUBE(EnvSamplerF, R).rgb; // lookup reflection in HDR cube map
    	
    half3 Cout = lerp( MODEL_COLOR * Cnatural, Creflect, fresnel);

    return half4(Cout, 1);
}

PS_MRTOut PS_2xRenderModel_Reflective(VS_Out In) :COLOR0
{
	PS_MRTOut OUT = (PS_MRTOut)0;
	
	half3 V = normalize(half3(In.WorldEyeVec));
    half3 N = normalize(half3(In.WorldNormal));

	half3 R = reflect(-V, N);
		
    half fresnel = fresnel(V, N);
   
    half3 Creflect;
    half3 Cdiffuse;
    
    Cdiffuse.rg = texCUBE(DifSamplerA, N).rg; // lookup reflection in HDR cube map
	Cdiffuse.b  = texCUBE(DifSamplerB, N).r;
	
    Creflect.rg = texCUBE(EnvSamplerA, R).rg; // lookup reflection in HDR cube map
	Creflect.b  = texCUBE(EnvSamplerB, R).r;
	
    half4 color;
    
    color.rgb = lerp(MODEL_COLOR * Cdiffuse, Creflect, fresnel); 
	color.a   = 1;
	
	OUT.Color0.rg = color.rg;
	OUT.Color1.rg = color.ba;

    return OUT;
}

half4 PS_RenderModel_Refractive(VS_Out In) :COLOR0
{
	half3 V = normalize(half3(In.WorldEyeVec));
    half3 N = normalize(half3(In.WorldNormal));

    half3 R = reflect(-V, N);
    half3 T = refract(-V, N, EtaValue);
    half fresnel = fresnel(V, N);

    half3 Creflect = texCUBE(EnvSamplerF, R).rgb; // lookup reflection in HDR cube map
    half3 Crefract = texCUBE(EnvSamplerF, T).rgb; // refraction
	
    half3 Cout = lerp(Crefract, Creflect, fresnel);

    return half4(Cout, 1);
}

PS_MRTOut PS_2xRenderModel_Refractive(VS_Out In) :COLOR0
{
	PS_MRTOut OUT = (PS_MRTOut)0;
	
	half3 V = normalize(half3(In.WorldEyeVec));
    half3 N = normalize(half3(In.WorldNormal));

    half3 R      = reflect(-V, N);
    half3 T      = refract(-V, N, EtaValue);
    half fresnel = fresnel(V, N);

    half3 Creflect; 
    half3 Crefract; 
   
    Creflect.rg = texCUBE(EnvSamplerA, R).rg; // lookup reflection in HDR cube map
	Creflect.b  = texCUBE(EnvSamplerB, R).r;
	
	Crefract.rg = texCUBE(EnvSamplerA, T).rg; // lookup reflection in HDR cube map
	Crefract.b  = texCUBE(EnvSamplerB, T).r;
	
	half4 color;
    
    color.rgb = lerp(Crefract, Creflect, fresnel);
	color.a   = 1;
	
	OUT.Color0.rg = color.rg;
	OUT.Color1.rg = color.ba;
	
    return OUT;
}



//#############################################################################
//
// TECHNIQUES
//
//#############################################################################

technique RenderModel_reflective
{
    Pass Main
    {
       VertexShader = compile vs_2_0 VS_Main();
       PixelShader  = compile ps_2_0 PS_RenderModel_Reflective();
    }
}

technique RenderModel_refractive
{
    Pass Main
    {
       VertexShader = compile vs_2_0 VS_Main();
       PixelShader  = compile ps_2_0 PS_RenderModel_Refractive();
    }
}

technique RenderModel_2RT_reflective
{
    Pass Main
    {
       VertexShader = compile vs_2_0 VS_Main();
       PixelShader  = compile ps_2_0 PS_2xRenderModel_Reflective();
    }
}

technique RenderModel_2RT_refractive
{
    Pass Main
    {
       VertexShader = compile vs_2_0 VS_Main();
       PixelShader  = compile ps_2_0 PS_2xRenderModel_Refractive();
    }
}

