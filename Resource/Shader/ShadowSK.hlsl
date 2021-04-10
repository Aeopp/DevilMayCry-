float4 World;
float4 ViewProjection;

float4 LightPosition;
float2 ClipPlanes;
bool IsPerspective;

int nMaxBonesRefPerVtx = 4;
int VTFPitch;
texture VTF;
sampler VTFSampler = sampler_state
{
    texture = VTF;

    minfilter = point;
    magfilter = point;
    mipfilter = point;
    sRGBTexture = false;
};

void VsVariance(
    in out float4 Position : POSITION,
    in float4 BoneIds0 : BLENDINDICES0,
    in float4 BoneIds1 : BLENDINDICES1,
    in float4 Weights0 : BLENDWEIGHT0,
    in float4 Weights1 : BLENDWEIGHT1,
    out float2 ZW : TEXCOORD0,
    out float4 WorldPosition : TEXCOORD1)
{
    float4 AnimPos = float4(0, 0, 0, 1);
    Position.w = 1.0f;
    const float UVCorrection = 0.5f;
    float       FVTFPitch = float(VTFPitch);
    int         IVTFPitch = int(VTFPitch);
    
    for (int i = 0; i < 4; ++i)
    {
        int Idx = BoneIds0[i] * 4;
        
        float2 VTFUVRow0 = float2((float(Idx % IVTFPitch) + UVCorrection) / FVTFPitch,
                                  (float(Idx / IVTFPitch) + UVCorrection) / FVTFPitch);
        
        float2 VTFUVRow1 = float2((float((Idx + 1) % IVTFPitch) + UVCorrection) / FVTFPitch,
                                  (float((Idx + 1) / IVTFPitch) + UVCorrection) / FVTFPitch);
        
        float2 VTFUVRow2 = float2((float((Idx + 2) % IVTFPitch) + UVCorrection) / FVTFPitch,
                                  (float((Idx + 2) / IVTFPitch) + UVCorrection) / FVTFPitch);
        
        float2 VTFUVRow3 = float2((float((Idx + 3) % IVTFPitch) + UVCorrection) / FVTFPitch,
                                  (float((Idx + 3) / IVTFPitch) + UVCorrection) / FVTFPitch);
        
        float4x4 AnimMatrix =
        {
            tex2Dlod(VTFSampler, float4(VTFUVRow0, 0.f, 0.f)),
            tex2Dlod(VTFSampler, float4(VTFUVRow1, 0.f, 0.f)),
            tex2Dlod(VTFSampler, float4(VTFUVRow2, 0.f, 0.f)),
            tex2Dlod(VTFSampler, float4(VTFUVRow3, 0.f, 0.f))
        };
        
        AnimPos += (mul(Position, AnimMatrix) * Weights0[i]);
    }
    
    if (nMaxBonesRefPerVtx > 4)
    {
        for (int i = 0; i < 4; ++i)
        {
            int Idx = BoneIds1[i] * 4;
        
            float2 VTFUVRow0 = float2((float(Idx % IVTFPitch) + UVCorrection) / FVTFPitch,
                                  (float(Idx / IVTFPitch) + UVCorrection) / FVTFPitch);
        
            float2 VTFUVRow1 = float2((float((Idx + 1) % IVTFPitch) + UVCorrection) / FVTFPitch,
                                  (float((Idx + 1) / IVTFPitch) + UVCorrection) / FVTFPitch);
        
            float2 VTFUVRow2 = float2((float((Idx + 2) % IVTFPitch) + UVCorrection) / FVTFPitch,
                                  (float((Idx + 2) / IVTFPitch) + UVCorrection) / FVTFPitch);
        
            float2 VTFUVRow3 = float2((float((Idx + 3) % IVTFPitch) + UVCorrection) / FVTFPitch,
                                  (float((Idx + 3) / IVTFPitch) + UVCorrection) / FVTFPitch);
        
            float4x4 AnimMatrix =
            {
                 tex2Dlod(VTFSampler, float4(VTFUVRow0, 0.f, 0.f)),
                 tex2Dlod(VTFSampler, float4(VTFUVRow1, 0.f, 0.f)),
                 tex2Dlod(VTFSampler, float4(VTFUVRow2, 0.f, 0.f)),
                 tex2Dlod(VTFSampler, float4(VTFUVRow3, 0.f, 0.f))
            };
            AnimPos += (mul(Position, AnimMatrix) * Weights1[i]);
        }
    }
    
    WorldPosition = mul(float4(AnimPos.xyz, 1.f), World);
    Position = mul(WorldPosition, ViewProjection);
    
    ZW.xy = Position.zw;
};

void PsVariance(
    in float2 ZW : TEXCOORD0,
    in float4 WorldPosition : TEXCOORD1,
    out float4 Color : COLOR0)
{
    float D = ZW.x;
    
    if (IsPerspective)
    {
        float Z = length(LightPosition.xyz - WorldPosition.xyz);
        D = (Z - ClipPlanes.x) / (ClipPlanes.y - ClipPlanes.x);
    };
    
    Color = float4(D, D * D, 0, 1);
};


technique Variance
{
    pass p0
    {
        ZEnable = true;
        vertexshader = compile vs_3_0 VsVariance();
        pixelshader = compile ps_3_0 PsVariance();
    }
}


