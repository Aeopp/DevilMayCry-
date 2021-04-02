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

struct VsIn
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
};

struct VsOut
{
    float4 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
};

VsOut VsMain(VsIn In)
{
    VsOut Out = (VsOut) 0;
    matrix WVP = mul(World, View);
    WVP = mul(WVP, Projection);
    
    Out.UV = In.UV;
    Out.Position = mul(float4(In.Position.xyz, 1.f), WVP);
    Out.Normal = mul(float4(In.Normal, 0.f), WVP);
    
    return Out;
};


struct PsIn
{
    float2 UV : TEXCOORD0;
    float3 Normal : NORMAL;
};

struct PsOut
{
    float4 Color : COLOR0;
};

PsOut PsMain(PsIn In)
{
    PsOut Out = (PsOut) 0;
    Out.Color = tex2D(ALBM, In.UV);
    In.Normal = normalize(In.Normal);
    
    float Diffuse = saturate(dot(In.Normal, -normalize(LightDirection)));
    
    Out.Color *= Diffuse;
    
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


