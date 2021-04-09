float4x4 World;
float4x4 View;
float4x4 Projection;
float2 UVScale = { 1, 1 };

texture ALBM0Map;
sampler ALBM0 = sampler_state
{
    texture = ALBM0Map;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
    addressu = wrap;
    addressv = wrap;
    // 이후에 쉐이딩 수행할시 감마보정...
    sRGBTexture = false;
};

texture NRMR0Map;
sampler NRMR0 = sampler_state
{
    texture = NRMR0Map;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
    addressu = wrap;
    addressv = wrap;
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
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 BiNormal : BINORMAL;
    float2 UV : TEXCOORD0;
    float2 ZW : TEXCOORD1;
};

VsOut VsGBuffer(VsIn In)
{
    VsOut Out = (VsOut) 0;
    float4x4 ViewProjection = mul(View, Projection);
    
    float4x4 WVP = mul(World, ViewProjection);
    Out.UV = In.UV * UVScale;
    Out.Normal = mul(float4(In.Normal.xyz, 0.f), World);
    Out.Tangent = mul(float4(In.Tangent.xyz, 0.f), World);
    Out.BiNormal = mul(float4(In.BiNormal.xyz, 0.f), World);
    Out.Position = mul(float4(In.Position.xyz, 1.f), WVP);
    Out.ZW = Out.Position.zw;
    
    return Out;
}

struct PsIn
{
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 BiNormal : BINORMAL;
    float2 UV : TEXCOORD0;
    float2 ZW : TEXCOORD1;
};

struct PsOut
{
    vector Albm : COLOR0; // Albedo , Metalness
    vector Nrmr : COLOR1; // Normal xy , Unknown , Roughness
    vector Depth : COLOR2; // Depth , 채널 X
};

PsOut PsGBuffer(PsIn In)
{
    PsOut Out = (PsOut) 0;
    
    float3x3 TBN = float3x3(normalize(In.Tangent),
                            normalize(In.BiNormal),
                            normalize(In.Normal));
    
    float4 sampleNrmr = tex2D(NRMR0, In.UV);
    float2 NormalXY = sampleNrmr.xy * 2.0 - 1.0f;
    const float NormalZ = sqrt(1 - dot(NormalXY, NormalXY));
    
    float3 PackNormal = normalize(mul(
    normalize(float3(NormalXY, NormalZ)), TBN));
    
    Out.Albm = tex2D(ALBM0, In.UV);
    Out.Nrmr = float4(PackNormal.xyz * 0.5f + 0.5f, sampleNrmr.w);
    /*원근나누기*/
    Out.Depth = float4(In.ZW.x / In.ZW.y, 0, 0, 0);
    
    return Out;
};

technique GBuffer
{
    pass
    {
        alphablendenable = false;
        zenable = true;
        zwriteenable = true;
        sRGBWRITEENABLE = false;
        cullmode = ccw;
        fillmode = solid;
        vertexshader = compile vs_3_0 VsGBuffer();
        pixelshader = compile ps_3_0 PsGBuffer();
    }
}

