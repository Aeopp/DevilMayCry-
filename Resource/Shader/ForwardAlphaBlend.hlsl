matrix World;
matrix View;
matrix Projection;

texture ALBMMap;

sampler ALBM = sampler_state
{
    texture = ALBMMap;
    minfilter = anisotropic;
    magfilter = anisotropic;
    mipfilter = anisotropic;
    MaxAnisotropy = 16;
};

struct VsIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct VsOut
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
};

VsOut VsMain(VsIn In)
{
    VsOut Out = (VsOut) 0;
    matrix WVP = mul(World, View);
    WVP = mul(WVP, Projection);
    
    Out.UV = In.UV;
    Out.Position = mul(float4(In.Position.xyz, 1.f), WVP);
    return Out;
};


struct PsIn
{
    float2 UV : TEXCOORD0;
};

struct PsOut
{
    float4 Color : COLOR0;
};

PsOut PsMain(PsIn In)
{
    PsOut Out = (PsOut) 0;
    Out.Color = tex2D(ALBM, In.UV);
    
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
        cullmode = ccw;
        fillmode = solid;
        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsMain();
    }
}


