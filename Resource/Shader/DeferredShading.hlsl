#define QUAD_PI 12.566370614359172

texture ALBM;
texture NRMR;
texture DEPTH;

sampler ALBMSample = sampler_state
{
    texture = ALBM;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
    addressu = wrap;
    addressv = wrap;
};

sampler NRMRSample = sampler_state
{
    texture = NRMR;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
    addressu = wrap;
    addressv = wrap;
}; 


sampler ALBMSampler = sampler_state
{
    texture = ALBM;
    
    minfilter = linear;
};


    //sRGBWriteEnable = false;
    //cullmode = cw;
 
