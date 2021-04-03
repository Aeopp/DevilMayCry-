matrix World;
matrix View;
matrix Projection;
float3 LightDirection = float3(0, -1, 0);

texture ALBMMap;
sampler ALBM = sampler_state
{
    texture = ALBMMap;
    minfilter = anisotropic;
    magfilter = anisotropic;
    mipfilter = anisotropic;
    sRGBTexture = true;
    MaxAnisotropy = 16;
};

texture NRMRMap;
sampler NRMR = sampler_state
{
    texture = NRMRMap;
    minfilter = point;
    magfilter = point;
    mipfilter = point;
    sRGBTexture = false;
};


struct VsIn
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 BiNormal : BINORMAL;
    float2 UV : TEXCOORD0;
};

struct VsOut
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 Tangent : TEXCOORD2;
    float3 BiNormal : TEXCOORD3;
};

VsOut VsMain(VsIn In)
{
    VsOut Out = (VsOut) 0;
    matrix WVP = mul(World, View);
    WVP = mul(WVP, Projection);
    
    Out.UV = In.UV;
    Out.Position = mul(float4(In.Position.xyz, 1.f), WVP);
    Out.Normal = mul(float4(In.Normal, 0.f), World);
    Out.Tangent = mul(float4(In.Tangent, 0.f), World);
    Out.BiNormal = mul(float4(In.BiNormal, 0.f), World);
    
    return Out;
};


struct PsIn
{
    float2 UV : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 Tangent : TEXCOORD2;
    float3 BiNormal : TEXCOORD3;
};

struct PsOut
{
    float4 Color : COLOR0;
};

PsOut PsMain(PsIn In)
{
    PsOut Out = (PsOut) 0;
    Out.Color = tex2D(ALBM, In.UV);
    
    float3x3 TBN = float3x3(normalize(In.Tangent),
                            normalize(In.BiNormal),
                            normalize(In.Normal));
    
    
   const  float2 NormalXY = tex2D(NRMR, In.UV).xy * 2.f - 1.f;
   const  float NormalZ =sqrt(1 - dot(NormalXY, NormalXY));
    
    const float3 WorldNormal = normalize(mul(float3(NormalXY, NormalZ), TBN));
    float Diffuse = saturate(dot(WorldNormal, -normalize(LightDirection)));
    
    Out.Color.rgb *= Diffuse;
    float Roughness = Out.Color.a;
    // Out.Color = float4(0.77f, 0.55f, 0.33f, 1.f);
    
    return Out;
};


technique Default
{
    pass
    {
        alphablendenable = false;
        zenable = true;
        zwriteenable = true;
        sRGBWRITEENABLE = true;
        cullmode = ccw;
        fillmode = solid;
        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsMain();
    }
}


