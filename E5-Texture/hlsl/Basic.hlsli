#include "LightHelper.hlsli"

Texture2D gTexture : register(t0);
Texture2D gTexture1 : register(t1);
SamplerState gSamplerState : register(s0);

cbuffer VSConstantBuffer : register(b0)
{
    matrix gModel;
    matrix gView;
    matrix gProj;
    matrix gAdjustNormal;   //用于矫正法向量的矩阵
    matrix gTexTransform;    //用于纹理坐标的变换
}

cbuffer PSConstantBuffer : register(b1)
{
    DirectionalLight gDirLight[10];
    PointLight gPointLight[10];
    SpotLight gSpotLight[10];
    Material gMaterial;
    float3 gEyePosW;
    float gPadding1;
    int gNumDirLight;
    int gNumPointLight;
    int gNumSpotLight;
    float gPadding2;
}

struct VertexPosNorTex
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float4 texTure : TEXCOORD;
};

struct VertexPosTex
{
    float3 posL : POSITION;
    float4 texTure : TEXCOORD;
};

struct VertexHWPosNorTex
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION;     // 在世界中的位置
    float3 normalW : NORMAL;    // 法向量在世界中的方向
    float2 texTure : TEXCOORD;
};

struct VertexHPosTex
{
    float4 posH : SV_POSITION;
    float2 texTure : TEXCOORD;
};
