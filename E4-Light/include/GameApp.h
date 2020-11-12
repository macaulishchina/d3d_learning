#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "DT.h"
class GameApp : public D3DApp {
public:

    GameApp(HINSTANCE hInstance);
    ~GameApp();

    bool Init();
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene();
    void Update2DScene();

private:
    bool InitEffect();
    bool InitResource();
    bool ResetMesh(const Geometry::MeshData<DT::VertexPosNormalColor>& meshData);

    ComPtr<ID3D11InputLayout> mVertexLayout;    // 顶点输入布局
    ComPtr<ID3D11Buffer> mVertexBuffer;         // 顶点缓冲区
    ComPtr<ID3D11Buffer> mIndexBuffer;			// 索引缓冲区
    ComPtr<ID3D11Buffer> mVSGPUBuffer;	    // VS常量缓冲区
    ComPtr<ID3D11Buffer> mPSGPUBuffer;	    // PS常量缓冲区
    UINT mIndexCount;                           // 索引量

    ComPtr<ID3D11VertexShader> mVertexShader;   // 顶点输入布局
    DT::VSConstantBuffer mVSCPUBuffer;	        // 顶点着色器关联变量
    ComPtr<ID3D11PixelShader> mPixelShader;     // 像素着色器
    DT::PSConstantBuffer mPSCPUBUffer;            // 像素着色器关联变量

    DT::DirectionalLight mDirLight;				// 默认平行光
    DT::PointLight mPointLight;					// 默认点光
    DT::SpotLight mSpotLight;				    // 默认汇聚光

};


#endif