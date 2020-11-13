#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"
#include "DT.h"
#define CONST_BUFFER_NUMS 6
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

    ComPtr<ID3D11InputLayout> mVertexLayout3D;              // 顶点输入布局
    ComPtr<ID3D11Buffer> mGPUVertexBuffer;                  // 顶点缓冲区
    ComPtr<ID3D11Buffer> mGPUIndexBuffer;			        // 索引缓冲区
    ComPtr<ID3D11Buffer> mConstBuffers[CONST_BUFFER_NUMS];	// 常量缓冲区
    UINT mIndexCount;                                       // 索引量

    ComPtr<ID3D11SamplerState> mSamplerState;		// 采样器状态
    ComPtr<ID3D11ShaderResourceView> mWoodCrate;    // 木盒纹理


    ComPtr<ID3D11VertexShader> mVertexShader3D;     // 3D顶点输入布局
    ComPtr<ID3D11PixelShader> mPixelShader3D;       // 3D像素着色器

    DT::ModelLocationBuffer mModelBuffer;	    // 模型位置相关量
    DT::CameraBuffer mCameraBuffer;	            // 视角相关量
    DT::MaterialBuffer mMaterialBuffer;	        // 模型材质相关量
    DT::DirLightBuffer mDirLightBuffer;	        // 平行光源相关量
    DT::PointLightBuffer mPointLightBuffer;	    // 点光源相关量
    DT::SpotLightBuffer mSpotLightBuffer;	    // 聚光灯源相关量

};


#endif