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

private:
    bool InitEffect();
    bool InitResource();
    bool InitScene();
    void Update2DScene();
    template<class VertexType>
    bool ResetMesh(const Geometry::MeshData<VertexType>& meshData);

    ComPtr<ID3D11InputLayout> mVertexLayout3D;      // 顶点输入布局
    ComPtr<ID3D11Buffer> mGPUVertexBuffer;          // 顶点缓冲区
    ComPtr<ID3D11Buffer> mGPUIndexBuffer;			// 索引缓冲区
    ComPtr<ID3D11Buffer> mGPUVSConstBuffer;	        // VS常量缓冲区
    ComPtr<ID3D11Buffer> mGPUVSConstBuffer2;	    // VS常量缓冲区
    ComPtr<ID3D11Buffer> mGPUPSConstBuffer;	        // PS常量缓冲区
    UINT mIndexCount;                               // 索引量

    ComPtr<ID3D11SamplerState> mSamplerState;		// 采样器状态
    ComPtr<ID3D11ShaderResourceView> mWoodCrate;    // 木盒纹理


    ComPtr<ID3D11VertexShader> mVertexShader3D;     // 3D顶点输入布局
    ComPtr<ID3D11PixelShader> mPixelShader3D;       // 3D像素着色器

    DT::VSConstantBuffer mVSCPUBuffer;	            // 顶点着色器关联变量
    DT::VSConstantBuffer2 mVSCPUBuffer2;	            // 顶点着色器关联变量

    DT::PSConstantBuffer mPSCPUBUffer;              // 像素着色器关联变量

};


#endif