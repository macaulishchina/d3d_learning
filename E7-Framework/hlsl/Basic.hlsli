#include "LightHelper.hlsli"

Texture2D gTexture : register(t0);
SamplerState gSamplerState : register(s0);

//模型相关常量
cbuffer ModelLocationBuffer : register(b0)
{
    matrix gModel;
    matrix gAdjustNormal; //用于矫正法向量的矩阵
}

//视角相关常量
cbuffer CameraBuffer : register(b1)
{
    matrix gView;
    matrix gProj;
    float3 gEyePosW;
    float padding1;
}

//材质线管常量
cbuffer MaterialBuffer : register(b2)
{
    Material gMaterial;
}

//平行光相关常量
cbuffer DirLightBuffer : register(b3)
{
    DirectionalLight gDirLight[10];
    int gNumDirLight;
    float3 padding2;

}

//点光源相关常量
cbuffer PointLightBuffer : register(b4)
{
    PointLight gPointLight[10];
    int gNumPointLight;
    float3 padding3;
}

//聚光灯相关常量
cbuffer SpotLightBuffer : register(b5)
{
    SpotLight gSpotLight[10];
    int gNumSpotLight;
    float3 padding4;

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
