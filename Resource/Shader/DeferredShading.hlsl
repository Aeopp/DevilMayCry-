#define QUAD_PI 12.566370614359172

matrix LightViewProjection;
matrix ViewProjectionInverse;

float4 LightPosition;
float3 LightColor = { 1, 1, 1 };
float LightFlux = 10.0f;
float LightIlluminance = 1.5f;
float LightRadius = 5.0f;
float SpecularPower = 80.0f;

float4 EyePosition;
float2 PixelSize;
float2 ClipPlanes;

texture ALBM;
texture NRMR;
texture Depth;
texture Shadow;
textureCUBE CubeShadow;

samplerCUBE CubeShadowSample = sampler_state
{
    texture = CubeShadow;

    minfilter = linear;
    magfilter = linear;
    mipfilter = none;
    addressu = clamp;
    addressv = clamp;
};

sampler ShadowSample = sampler_state
{
    texture = Shadow;
    minfilter = linear;
    magfilter = linear;
    mipfilter = none;
    addressu = clamp;
    addressv = clamp;
    SRGBTEXTURE = false;
};

sampler ALBMSample = sampler_state
{
    texture = ALBM;
    minfilter = linear;
    magfilter = linear;
    mipfilter = none;
    addressu = clamp;
    addressv = clamp;
    SRGBTEXTURE = true;
};


sampler NRMRSample = sampler_state
{
    texture = NRMR;
    minfilter = linear;
    magfilter = linear;
    mipfilter = none;
    addressu = clamp;
    addressv = clamp;
    SRGBTEXTURE = false;
}; 


sampler DepthSample = sampler_state
{
    texture = Depth;
    minfilter = linear;
    magfilter = linear;
    mipfilter = none;
    addressu = clamp;
    addressv = clamp;
    SRGBTEXTURE = false;
};

void VsMain(
    in out float4 Position : POSITION,
    in out
float2 TexCoord : TEXCOORD0)
{
    Position.xy -= PixelSize.xy;
}

float ShadowVariance(float2 Moments, float D)
{
    float Mean = Moments.x;
    float Variance = max(Moments.y - Moments.x * Moments.x, 1e-5f);
    float Md = Mean - D;
    float Chebychev = Variance / (Variance + Md * Md);

    Chebychev = smoothstep(0.1f, 1.0f, Chebychev);
    
    return max(((D <= Mean) ? 1.0f : 0.0f), Chebychev);
};

float3 Luminance_Blinn_Directional(float3 albedo, float3 wpos, float3 wnorm)
{
	// 태양의 각도 지름은 [0.526, 0.545].
    const float sinAngularRadius = 0.0046251;
    const float cosAngularRadius = 0.9999893;

    float3 v = normalize(EyePosition.xyz - wpos);
    float3 n = normalize(wnorm);

	// 디스크에 가장 가까운 지점 . 
    float3 D = normalize(LightPosition.xyz);
    float3 R = reflect(-v, n);

    float DdotR = dot(D, R);
    float r = sinAngularRadius;
    float d = cosAngularRadius;

    float3 S = R - DdotR * D;
    float3 L = ((DdotR < d) ? normalize(d * D + normalize(S) * r) : R);

	//BRDF (참고 : 값 및 노출과 일치해야 함)
    float3 h = normalize(L + v);

    float ndotl = saturate(dot(n, L));
    float ndoth = saturate(dot(n, h));

    float3 f_diffuse = albedo;
    float f_specular = pow(ndoth, SpecularPower);

    float costheta = saturate(dot(n, D));
    float illuminance = LightIlluminance * costheta;

	// calculate shadow (assumes ortho projection)
    float4 lspos = mul(float4(wpos, 1), LightViewProjection);
	
    d = lspos.z;

    float2 ptex = (lspos.xy / lspos.w) * float2(0.5f, -0.5f) + 0.5f;
    float2 moments = tex2D(ShadowSample, ptex).rg;
    float  shadow = ShadowVariance(moments, d);

    return (f_diffuse + f_specular) * LightColor* illuminance * shadow;
}

float3 Luminance_Blinn_Point(float3 albedo, float3 wpos, float3 wnorm)
{
    float3 ldir = LightPosition.xyz - wpos;

    float3 l = normalize(ldir);
    float3 v = normalize(EyePosition.xyz - wpos);
    float3 h = normalize(l + v);
    float3 n = normalize(wnorm);

    float dist2 = max(dot(ldir, ldir), 1e-4f);
    float ndotl = saturate(dot(n, l));
    float ndoth = saturate(dot(n, h));

    float3 f_diffuse = albedo;
    float f_specular = pow(ndoth, SpecularPower);

	// calculate shadow
    float2 moments = texCUBE(CubeShadowSample, -l).xy;

    float z = length(ldir);
    float d = (z - ClipPlanes.x) / (ClipPlanes.y - ClipPlanes.x);
    float shadow = ShadowVariance(moments, d);

    float illuminance = (LightFlux/ (QUAD_PI * dist2)) * ndotl;
    float attenuation = max(0, 1 - sqrt(dist2) / LightRadius);

    return (f_diffuse + f_specular) * LightColor* illuminance * attenuation * shadow;
}

void ps_deferred(
	in float2 tex : TEXCOORD0,
	out float4 color : COLOR0)
{
    float4 base = tex2D(ALBMSample, tex);
    float3 wnorm = tex2D(NRMRSample, tex).rgb * 2.0f - 1.0f;
    float d = tex2D(DepthSample, tex).r;
    float4 wpos = float4(tex.x * 2 - 1, 1 - 2 * tex.y, d, 1);

    if (d > 0.0f)
    {
        wpos = mul(wpos,ViewProjectionInverse);
        wpos /= wpos.w;

        if (LightPosition.w < 0.5f)
        {
			// 방향 
            color.rgb = Luminance_Blinn_Directional(base.rgb, wpos.xyz, wnorm);
        }
        else
        {
			// 포인트 라이트
            color.rgb = Luminance_Blinn_Point(base.rgb, wpos.xyz, wnorm);
        }

        color.a = 1;
    }
    else
    {
        color = float4(0, 0, 0, 0);
    }
}

technique DeferredShading
{
    pass p0
    {
        ZWriteEnable = false;
        ZEnable = false;
        ALPHABLENDENABLE = true;
        SRCBLEND = ONE;
        DESTBLEND = ONE;
        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsDeferred();
    }
}