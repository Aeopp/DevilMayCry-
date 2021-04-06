matrix World;
matrix View;
matrix Projection;
//float3 LightDirection = float3(0, -1, 0);

float _SliceAmount = 0.f;
float _TexV = 0.5f;  // 0 ~ 1

texture Color0Map; // ó���� ��� �Ͼ��
sampler Color0 = sampler_state
{
    texture = Color0Map;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

texture Color1Map; // �� ������ ��� ������(����)
sampler Color1 = sampler_state
{
    texture = Color1Map;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

texture AlpMap; // ��������� ���ķ� ����ϱ� ����
sampler Alpha = sampler_state
{
    texture = AlpMap;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};

texture NoiseMap;
sampler Noise = sampler_state
{
    texture = NoiseMap;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
};


struct VsIn
{
	float4 Position : POSITION;
    float2 UV : TEXCOORD1;
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
    
    Out.Position = mul(float4(In.Position.xyz, 1.f), WVP);
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

// ó���� ��� �Ͼ��
PsOut PsMain_White(PsIn In)
{
    PsOut Out = (PsOut) 0;
    
    float2 NewUV = In.UV;
    NewUV.y = _TexV;
    
    float4 ColorSample = tex2D(Color0, NewUV);
    float4 NoiseSample = tex2D(Noise, NewUV);
    
    float4 Noise = NoiseSample.rrrr;    // r,g,b ���� �ٸ� ������. �� �� �ϳ��� ���
    Noise.rgb -= _SliceAmount;
    clip(Noise);
 
    Out.Color = ColorSample;
    Out.Color.a *= 0.4f;    // ��� ����

    return Out;
};

// �� ������ ��� ������(����)
PsOut PsMain_Lightning(PsIn In)
{
    PsOut Out = (PsOut) 0;
    
    float2 NewUV = In.UV;
    NewUV.y = _TexV;
    
    float4 AlpSample = tex2D(Alpha, NewUV);
    float4 ColorSample = tex2D(Color1, NewUV);
    float4 NoiseSample = tex2D(Noise, NewUV);
    
    float4 Noise = NoiseSample.rrrr;
    Noise.rgb -= _SliceAmount;
    clip(Noise);
 
    Out.Color = float4(ColorSample.rgb, AlpSample.r);
    //Out.Color = float4(In.UV.x, In.UV.y, 0.f, 1.f);
    
    return Out;
};

technique Default
{
	pass
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;
        //zenable = false;
        zwriteenable = false;

        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsMain_White();
    }
    pass
    {
        alphablendenable = true;
        srcblend = srcalpha;
        destblend = invsrcalpha;
        //zenable = false;
        zwriteenable = false;

        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsMain_Lightning();
    }
};