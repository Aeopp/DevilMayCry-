matrix Ortho;
matrix ScreenMat;    // (-width/2 ~ +width/2, +height/2 ~ -height/2)
float3 LightDirection = float3(0.f, 0.f, 1.f);

float _AccumulationTexV;
float _TotalAccumulateTime;

float _HP_Degree = 0.f; // 0 ~ 360 ����
float2 _HP_StartPt;
float2 _HP_Normal0;
float2 _HP_Normal1;

float _BossGaugeCurXPosOrtho;

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

texture BossGaugeATOSMap;
sampler BossGaugeATOS = sampler_state
{
    texture = BossGaugeATOSMap;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

texture BossGaugeNRMRMap;
sampler BossGaugeNRMR = sampler_state
{
    texture = BossGaugeNRMRMap;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
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

struct VsOut_Clip
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 Tangent : TEXCOORD2;
    float3 BiNormal : TEXCOORD3;
    float Clip : TEXCOORD4;
};

struct VsOut_NoiseClip
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 Tangent : TEXCOORD2;
    float3 BiNormal : TEXCOORD3;
    float2 NoiseCoord0 : TEXCOORD4;
    float2 NoiseCoord1 : TEXCOORD5;
    float2 NoiseCoord2 : TEXCOORD6;
    float Clip : TEXCOORD7;
};


VsOut VsMain(VsIn In)
{
    VsOut Out = (VsIn) 0;

    Out.Position = mul(float4(In.Position.xyz, 1.f), ScreenMat);
    Out.Position = mul(float4(Out.Position.xyz, 1.f), Ortho);
    Out.Normal = normalize(mul(float4(In.Normal.xyz, 0.f), ScreenMat));
    Out.Tangent = normalize(mul(float4(In.BiNormal.xyz, 0.f), ScreenMat));
    Out.BiNormal = normalize(mul(float4(In.Position.xyz, 0.f), ScreenMat));
    Out.UV = In.UV;
    
    return Out;
};

VsOut_NoiseClip VsMain_TargetHP(VsIn In)
{
    VsOut_NoiseClip Out = (VsOut_NoiseClip) 0;

    Out.Position = mul(float4(In.Position.xyz, 1.f), ScreenMat);
    Out.Position = mul(float4(Out.Position.xyz, 1.f), Ortho);
    Out.Normal = normalize(mul(float4(In.Normal.xyz, 0.f), ScreenMat));
    Out.Tangent = normalize(mul(float4(In.BiNormal.xyz, 0.f), ScreenMat));
    Out.BiNormal = normalize(mul(float4(In.Position.xyz, 0.f), ScreenMat));
    Out.UV = In.UV;
       
    // noise
    Out.NoiseCoord0 = Out.UV;
    Out.NoiseCoord0.y += _TotalAccumulateTime * 0.2f; // scrollspeed;
    Out.NoiseCoord1 = Out.UV;
    Out.NoiseCoord1.y += _TotalAccumulateTime * 0.1f; // scrollspeed;
    Out.NoiseCoord2 = Out.UV;
    Out.NoiseCoord2.y += _TotalAccumulateTime * 0.3f; // scrollspeed;
    
    // clip
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

VsOut_Clip VsMain_BossGauge(VsIn In)
{
    VsOut_Clip Out = (VsOut_Clip) 0;

    Out.Position = mul(float4(In.Position.xyz, 1.f), ScreenMat);
    Out.Position = mul(float4(Out.Position.xyz, 1.f), Ortho);
    Out.Normal = normalize(mul(float4(In.Normal.xyz, 0.f), ScreenMat));
    Out.Tangent = normalize(mul(float4(In.BiNormal.xyz, 0.f), ScreenMat));
    Out.BiNormal = normalize(mul(float4(In.Position.xyz, 0.f), ScreenMat));
    Out.UV = In.UV;
               
    Out.Clip = Out.Position.x;  // Ps ���� ���
    
    return Out;
};


struct PsIn
{
    float2 UV : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 Tangent : TEXCOORD2;
    float3 BiNormal : TEXCOORD3;
};

struct PsIn_Clip
{
    float2 UV : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 Tangent : TEXCOORD2;
    float3 BiNormal : TEXCOORD3;
    float Clip : TEXCOORD4;
};

struct PsIn_NoiseClip
{
    float2 UV : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 Tangent : TEXCOORD2;
    float3 BiNormal : TEXCOORD3;
    float2 NoiseCoord0 : TEXCOORD4;
    float2 NoiseCoord1 : TEXCOORD5;
    float2 NoiseCoord2 : TEXCOORD6;
    float Clip : TEXCOORD7;
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
    
    float2 NormalXY = NRMRSample.xy * 2.f - 1.f;
    float NormalZ = sqrt(1 - dot(NormalXY, NormalXY));
   
    float3x3 TBN = float3x3(normalize(In.Tangent),
                            normalize(In.BiNormal),
                            normalize(In.Normal));

    float3 WorldNormal = normalize(mul(float3(NormalXY, NormalZ), TBN));
    
    float Diffuse = saturate(dot(WorldNormal, -normalize(LightDirection)));
    float Ambient = ATOSSample.g * 0.2f;

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
    
    Out.Color = float4(1.f, 0.4f, 0.4f, Alp);

    return Out;
};

PsOut PsMain_TargetHP(PsIn_NoiseClip In)
{
    PsOut Out = (PsOut) 0;
    
    clip(In.UV.y - 0.8f);
    clip(In.Clip);
    
    float4 Noise0 = tex2D(Noise, In.NoiseCoord0).rrrr;
    float4 Noise1 = tex2D(Noise, In.NoiseCoord1).gggg;
    float4 Noise2 = tex2D(Noise, In.NoiseCoord2).bbbb;
    
    // -1 ~ 1 ����
    Noise0 = (Noise0 - 0.5f) * 2.f;
    Noise1 = (Noise1 - 0.5f) * 2.f;
    Noise2 = (Noise2 - 0.5f) * 2.f;
 
    // distortion
    Noise0.xy *= float2(0.3f, 0.1f);
    Noise1.xy *= float2(-0.1f, 0.2f);
    Noise2.xy *= float2(0.2f, -0.3f);
    
    float4 FinalNoise = Noise0 + Noise1 + Noise2;
    FinalNoise *= 0.5f; //scale;
    float2 NoiseCoord = FinalNoise.xy + In.UV;
       
    Out.Color = float4(0.5f, 0.647f, 0.698f, tex2D(TargetHP, NoiseCoord).r);
    Out.Color.a *= ((In.UV.y - 0.8f) * 5.f);
    
    return Out;
};

PsOut PsMain_BossGauge0(PsIn In)
{
    PsOut Out = (PsOut) 0;
    
    float4 ATOSSample = tex2D(BossGaugeATOS, In.UV);
    float4 NRMRSample = tex2D(BossGaugeNRMR, In.UV);
    
    float2 NormalXY = NRMRSample.xy * 2.f - 1.f;
    float NormalZ = sqrt(1 - dot(NormalXY, NormalXY));
   
    float3x3 TBN = float3x3(normalize(In.Tangent),
                            normalize(In.BiNormal),
                            normalize(In.Normal));

    float3 WorldNormal = normalize(mul(float3(NormalXY, NormalZ), TBN));
    
    float Diffuse = saturate(dot(WorldNormal, -normalize(LightDirection)));
    
    Out.Color.rgb = Diffuse * float3(0.416f, 0.525f, 0.65f);
    Out.Color.a = ATOSSample.a;
    
    return Out;
};

PsOut PsMain_BossGauge1(PsIn In)
{
    PsOut Out = (PsOut) 0;
    
    float4 ATOSSample = tex2D(BossGaugeATOS, In.UV);
    float4 NRMRSample = tex2D(BossGaugeNRMR, In.UV);
    
    float2 NormalXY = NRMRSample.xy * 2.f - 1.f;
    float NormalZ = sqrt(1 - dot(NormalXY, NormalXY));
   
    float3x3 TBN = float3x3(normalize(In.Tangent),
                            normalize(In.BiNormal),
                            normalize(In.Normal));

    float3 WorldNormal = normalize(mul(float3(NormalXY, NormalZ), TBN));
    
    float Diffuse = saturate(dot(WorldNormal, -normalize(LightDirection)));
    
    Out.Color.rgb = Diffuse * float3(0.282f, 0.391f, 0.588f);
    Out.Color.a = ATOSSample.g;
    
    return Out;
};

PsOut PsMain_BossGauge2(PsIn_Clip In)
{
    PsOut Out = (PsOut) 0;

    clip(_BossGaugeCurXPosOrtho - In.Clip);
    
    float4 ATOSSample = tex2D(BossGaugeATOS, In.UV);
    float4 NRMRSample = tex2D(BossGaugeNRMR, In.UV);
    
    float2 NormalXY = NRMRSample.xy * 2.f - 1.f;
    float NormalZ = sqrt(1 - dot(NormalXY, NormalXY));
   
    float3x3 TBN = float3x3(normalize(In.Tangent),
                            normalize(In.BiNormal),
                            normalize(In.Normal));

    float3 WorldNormal = normalize(mul(float3(NormalXY, NormalZ), TBN));
    
    float Diffuse = saturate(dot(WorldNormal, -normalize(LightDirection)));
    
    Out.Color.rgb = Diffuse * float3(0.263f, 0.227f, 0.733f); // ��
    Out.Color.a = ATOSSample.b;
    
    //Out.Color = Diffuse * float4(0.176f, 0.149f, 0.255f, ATOSSample.b); // �ٴ�
    
    return Out;
};

PsOut PsMain_BossGauge3(PsIn_Clip In)
{
    PsOut Out = (PsOut) 0;
    
    clip(_BossGaugeCurXPosOrtho - In.Clip);
    
    float4 ATOSSample = tex2D(BossGaugeATOS, In.UV);
    float4 NRMRSample = tex2D(BossGaugeNRMR, In.UV);
    
    float2 NormalXY = NRMRSample.xy * 2.f - 1.f;
    float NormalZ = sqrt(1 - dot(NormalXY, NormalXY));
   
    float3x3 TBN = float3x3(normalize(In.Tangent),
                            normalize(In.BiNormal),
                            normalize(In.Normal));

    float3 WorldNormal = normalize(mul(float3(NormalXY, NormalZ), TBN));
    
    float Diffuse = saturate(dot(WorldNormal, -normalize(LightDirection)));
    
    Out.Color.rgb = Diffuse * float3(0.627f, 0.674f, 0.984f);
    Out.Color.a = ATOSSample.r;
    
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
        pixelshader = compile ps_3_0 PsMain_TargetHP();
    }
    pass p3
    {
        alphablendenable = true;
        srcblend = srcalpha;
        destblend = invsrcalpha;
        zenable = false;
        zwriteenable = false;

        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsMain_BossGauge0();
    }
    pass p4
    {
        alphablendenable = true;
        srcblend = srcalpha;
        destblend = invsrcalpha;
        zenable = false;
        zwriteenable = false;

        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsMain_BossGauge1();
    }
    pass p5
    {
        alphablendenable = true;
        srcblend = srcalpha;
        destblend = invsrcalpha;
        zenable = false;
        zwriteenable = false;

        vertexshader = compile vs_3_0 VsMain_BossGauge();
        pixelshader = compile ps_3_0 PsMain_BossGauge2();
    }
    pass p6
    {
        alphablendenable = true;
        srcblend = srcalpha;
        destblend = invsrcalpha;
        zenable = false;
        zwriteenable = false;

        vertexshader = compile vs_3_0 VsMain_BossGauge();
        pixelshader = compile ps_3_0 PsMain_BossGauge3();
    }
};