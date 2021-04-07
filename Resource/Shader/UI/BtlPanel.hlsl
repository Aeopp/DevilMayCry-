matrix Ortho;
matrix ScreenMat;    // (-width/2 ~ +width/2, +height/2 ~ -height/2)
float3 LightDirection = float3(0.f, 0.f, -1.f); // 화면 안으로 향하는 방향

float _AccumulationTexV;

float _HP_Degree = 0.f; // 0 ~ 360 범위
float2 _HP_StartPt;
float2 _HP_Normal0;
float2 _HP_Normal1;

texture NoiseMap;
sampler Noise = sampler_state
{
    texture = NoiseMap;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

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

texture TargetCursorMap;
sampler TargetCursor = sampler_state
{
    texture = TargetCursorMap;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

texture TargetHPMap;
sampler TargetHP = sampler_state
{
    texture = TargetHPMap;
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

struct VsOut_TargetHP
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
    float Clip : TEXCOORD1;
};

VsOut VsMain(VsIn In)
{
    VsOut Out = (VsIn) 0;

    Out.Position = mul(float4(In.Position.xyz, 1.f), ScreenMat);
    Out.Position = mul(float4(Out.Position.xyz, 1.f), Ortho);
    
    Out.UV = In.UV;
    
    return Out;
};

VsOut_TargetHP VsMain_TargetHP(VsIn In)
{
    VsOut_TargetHP Out = (VsOut_TargetHP) 0;

    Out.Position = mul(float4(In.Position.xyz, 1.f), ScreenMat);
    Out.Position = mul(float4(Out.Position.xyz, 1.f), Ortho);
    
    Out.UV = In.UV;
       
    float2 EndPtVec = Out.Position.xy - _HP_StartPt;
    float Result = dot(normalize(EndPtVec), _HP_Normal0);
    float Result2 = dot(normalize(EndPtVec), _HP_Normal1);
    
    if (_HP_Degree < 180.f)
    {
        if (Result < 0.f && Result2 > 0.f)
            Out.Clip = -1.f;
        else
            Out.Clip = 1.f;
    }
    else
    {
        if (Result > 0.f && Result2 < 0.f)
            Out.Clip = 1.f;
        else
            Out.Clip = -1.f;
    }
    
     return Out;
};


struct PsIn
{
    float2 UV : TEXCOORD0;
};

struct PsIn_TargetHP
{
    float2 UV : TEXCOORD0;
    float Clip : TEXCOORD1;
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

PsOut PsMain_TargetCursor(PsIn In)
{
    PsOut Out = (PsOut) 0;
    
    float2 newUV = In.UV;
    newUV.y -= _AccumulationTexV;
    
    float Alp = tex2D(TargetCursor, In.UV).r * tex2D(Noise, newUV);
    
    Out.Color = float4(1.f, 1.f, 1.f, Alp);

    return Out;
};

PsOut PsMain_TargetHP_0(PsIn_TargetHP In)
{
    PsOut Out = (PsOut) 0;
    
    clip(In.UV.y - 0.7f);
    clip(In.Clip);
    
    float2 newUV = In.UV;
    newUV.y -= _AccumulationTexV;
    
    Out.Color = float4(0.5f, 0.647f, 0.698f, tex2D(TargetHP, newUV).r);
    
    float AlpCorrection = 0.3f; // In.UV.y == 0.85일 떄 가장 큰 값
    AlpCorrection *= (1.f - (abs(In.UV.y - 0.85f) * 6.666f));   // 6.666 -> 0.15의 역수
    Out.Color.a *= AlpCorrection;
    
    return Out;
};

PsOut PsMain_TargetHP_1(PsIn_TargetHP In)
{
    PsOut Out = (PsOut) 0;
    
    clip(In.Clip);
    
    float2 newUV = In.UV;
    newUV.x += _AccumulationTexV;
 
    Out.Color = float4(0.694f, 0.851f, 0.898f, tex2D(Noise, newUV).g * 0.1f);
    
    return Out;
};


technique Default
{
	pass p0
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
        zenable = false;
        zwriteenable = false;

        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsMain_RedOrb();
    }
    pass p1
    {
        alphablendenable = true;
        srcblend = srcalpha;
        destblend = invsrcalpha;
        zenable = false;
        zwriteenable = false;

        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsMain_TargetCursor();
    }
    pass p2
    {
        alphablendenable = true;
        srcblend = srcalpha;
        destblend = invsrcalpha;
        zenable = false;
        zwriteenable = false;

        vertexshader = compile vs_3_0 VsMain_TargetHP();
        pixelshader = compile ps_3_0 PsMain_TargetHP_0();
    }
    pass p3
    {
        alphablendenable = true;
        srcblend = srcalpha;
        destblend = invsrcalpha;
        zenable = false;
        zwriteenable = false;

        vertexshader = compile vs_3_0 VsMain_TargetHP();
        pixelshader = compile ps_3_0 PsMain_TargetHP_1();
    }
};