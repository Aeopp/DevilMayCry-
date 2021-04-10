float2 PixelSize;
float2 TexelSize;

texture RenderScene;
sampler RenderSceneSample = sampler_state
{
    texture = RenderScene;
};

 // ZEnable = false;
void VsMain(
    in out float2 Position : POSITION,
    in out float2 TexCoord : TEXCOORD0)
{
    Position.xy -= PixelSize.xy;
};

void PsBoxBlur3x3(
   in float2 TexCoord : TEXCOORD0,
    out float4 Color : COLOR0)
{
    Color = 0;

    // ÀÎÁ¢ ÇÈ¼¿ 9Ä­ ºí·¯ .
    // ¸óÅ× Ä«¸¦·Î . 
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-0.5f, -0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(0.5f, -0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(1.5f, -0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-0.5f, 0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(0.5f, 0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(1.5f, 0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-0.5f, 1.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(0.5f, 1.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(1.5f, 1.5f));

    Color /= 9.0f;
}

void PsBoxBlur5x5(
  in float2 TexCoord : TEXCOORD0,
  out float4 Color : COLOR0)
{
    Color = 0;
    
    // 25 Ä­ ºí·¯
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-1.5f, -1.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-0.5f, -1.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(0.5f, -1.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(1.5f, -1.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(2.5f, -1.5f));

    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-1.5f, -0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-0.5f, -0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(0.5f, -0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(1.5f, -0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(2.5f, -0.5f));

    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-1.5f, 0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-0.5f, 0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(0.5f, 0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(1.5f, 0.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(2.5f, 0.5f));

    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-1.5f, 1.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-0.5f, 1.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(0.5f, 1.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(1.5f, 1.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(2.5f, 1.5f));

    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-1.5f, 2.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(-0.5f, 2.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(0.5f, 2.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(1.5f, 2.5f));
    Color += tex2D(RenderSceneSample, TexCoord + TexelSize * float2(2.5f, 2.5f));

    color /= 25.0f;
}

technique Blur3x3
{
    pass p0
    {
        ZEnable = false;

        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsBoxBlur3x3();  
    } 
};

technique Blur5x5
{
    pass p0
    {
        ZEnable = false;
        vertexshader = compile vs_3_0 VsMain();
        pixelshader = compile ps_3_0 PsBoxBlur5x5();
    }
};

