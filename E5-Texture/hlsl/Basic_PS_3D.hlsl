#include "Basic.hlsli"

// 3D像素着色器
float4 PS_3D(VertexHWPosNorTex pIn) : SV_Target
{
    // 标准化法向量
    pIn.normalW = normalize(pIn.normalW);

    // 顶点指向眼睛的向量
    float3 toEyeW = normalize(gEyePosW - pIn.posW);

    // 初始化为0 
    float4 ambient, diffuse, spec, A, D, S;
    ambient = diffuse = spec = A = D = S = float4(0.0f, 0.0f, 0.0f, 0.0f);
    int i;
    
    [unroll]
    for (i = 0; i < gNumDirLight; i++)
    {
        ComputeDirectionalLight(gMaterial, gDirLight[i], pIn.normalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
        
    }
    [unroll]
    for (i = 0; i < gNumPointLight; i++)
    {

        ComputePointLight(gMaterial, gPointLight[i], pIn.posW, pIn.normalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
    [unroll]
    for (i = 0; i < gNumSpotLight; i++)
    {
        ComputeSpotLight(gMaterial, gSpotLight[i], pIn.posW, pIn.normalW, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;
    }
    float4 texColor = gTexture.Sample(gSamplerState, pIn.texTure);
    float4 texColor1 = gTexture1.Sample(gSamplerState, pIn.texTure);
    float4 litColor = texColor * texColor1 * (ambient + diffuse) + spec;
    litColor.a = texColor.a * gMaterial.diffuse.a;
	
    return litColor;
}