matrix Ortho;
matrix ScreenMat;    // (-width/2 ~ +width/2, +height/2 ~ -height/2)
float3 LightDirection = float3(0.f, 0.f, -1.f); // 화면 안으로 향하는 방향

texture RedOrbALBMMap;
sampler RedOrbALBM = sampler_state
{
    texture = RedOrbALBMMap;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

texture RedOrbATOSMap;
sampler RedOrbATOS = sampler_state
{
    texture = RedOrbATOSMap;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

texture RedOrbNRMRMap;
sampler RedOrbNRMR = sampler_state
{
    texture = RedOrbNRMRMap;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
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
    VsOut Out = (VsIn) 0;

    Out.Position = mul(float4(In.Position.xyz, 1.f), ScreenMat);
    Out.Position = mul(float4(Out.Position.xyz, 1.f), Ortho);
    
    Out.UV = In.UV;

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

PsOut PsMain_RedOrb(PsIn In)
{
    PsOut Out = (PsOut) 0;
    
    float4 ALBMSample = tex2D(RedOrbALBM, In.UV);
    float4 ATOSSample = tex2D(RedOrbATOS, In.UV);
    float4 NRMRSample = tex2D(RedOrbNRMR, In.UV);

    float Diffuse = saturate(dot(normalize(-LightDirection), normalize(NRMRSample.rgb)));
    float Ambient = ATOSSample.g * 0.5f;

    Out.Color = (Diffuse + Ambient) * float4(ALBMSample.rgb, 1.f);
    Out.Color.a = ATOSSample.r;

    return Out;
};

technique Default
{
	pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
        zenable = false;
        zwriteenable = false;

        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsMain_RedOrb();
    }
};