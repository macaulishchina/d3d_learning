#include "LightHelper.hlsli"

cbuffer ConstantBuffer : register(b0)
{
    matrix gModel;
    matrix gView;
    matrix gProj;
    matrix gAdjustNormal;   //用于矫正法向量的矩阵
}

cbuffer PSConstantBuffer : register(b1)
{
    DirectionalLight gDirLight;
    PointLight gPointLight;
    SpotLight gSpotLight;
    Material gMaterial;
    float3 gEyePosW;
    float gPadding;
}
struct VertexIn
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;     // 在世界中的位置
    float3 normalW : NORMAL;    // 法向量在世界中的方向
    float4 color : COLOR;
};
