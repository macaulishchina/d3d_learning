#include "Basic.hlsli"

// 3D顶点着色器
VertexHWPosNorTex VS_3D(VertexPosNorTex vIn)
{
    VertexHWPosNorTex vOut;
    matrix viewProj = mul(gView, gProj);
    float4 posW = mul(float4(vIn.posL, 1.0f), gModel);

    vOut.posH = mul(posW, viewProj);    //视角空间
    vOut.posW = posW.xyz;               //世界空间
    //矫正法向量在世界坐标系下的值
    vOut.normalW = mul(vIn.normalL, (float3x3) gAdjustNormal);
    vOut.texTure = vIn.texTure;             // 这里alpha通道的值默认为1.0
    return vOut;
}
