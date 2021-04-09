
float4 World;
float4 ViewProjection;

float4 LightPosition;
float2 ClipPlanes;
bool IsPerspective;

void VsVariance(
in out float4 Position : POSITION,
    out float2 ZW : TEXCOORD0,
out float4 WorldPosition : TEXCOORD1)
{
    WorldPosition = mul(float4(Position.xyz, 1), World);
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
        vertexshader = compile vs_3_0 VsVariance();
        pixelshader = compile ps_3_0 PsVariance();
    }
}


