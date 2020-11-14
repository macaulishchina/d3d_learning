#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "DT.h"
#include "d3dApp.h"
#include "Resource.h"

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



    UINT mIndexCount;                                       // 索引量

    ComPtr<ID3D11SamplerState> mSamplerState;		// 采样器状态
    ComPtr<ID3D11ShaderResourceView> mWoodCrate;    // 木盒纹理

    ComPtr<ID3D11Buffer> mGPUVertexBuffer;                  // 顶点缓冲区
    ComPtr<ID3D11Buffer> mGPUIndexBuffer;			        // 索引缓冲区


    ResourceController mController;

};


#endif