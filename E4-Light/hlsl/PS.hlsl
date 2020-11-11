#include "Light.hlsli"

// 像素着色器
float4 PS(VertexOut pIn) : SV_Target
{
    // 标准化法向量
    pIn.normalW = normalize(pIn.normalW);

    // 顶点指向眼睛的向量
    float3 toEyeW = normalize(gEyePosW - pIn.posW);

    // 初始化为0 
    float4 ambient, diffuse, spec;
    float4 A, D, S;
    ambient = diffuse = spec = A = D = S = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ComputeDirectionalLight(gMaterial, gDirLight, pIn.normalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    ComputePointLight(gMaterial, gPointLight, pIn.posW, pIn.normalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    ComputeSpotLight(gMaterial, gSpotLight, pIn.posW, pIn.normalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    spec += S;

    float4 litColor = pIn.color * (ambient + diffuse) + spec;
	
    litColor.a = gMaterial.diffuse.a * pIn.color.a;
	
    return litColor;
}