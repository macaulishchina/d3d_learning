#ifndef GAMEAPP_H
#define GAMEAPP_H
#include "d3dApp.h"
class GameApp : public D3DApp {
public:
    struct VertexPosColor {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT4 color;
        static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
    };

    struct ConstantBuffer {
        DirectX::XMMATRIX model;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX proj;
    };

    GameApp(HINSTANCE hInstance);
    ~GameApp();

    bool Init();
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene();

private:
    bool InitEffect();
    bool InitResource();

    ComPtr<ID3D11InputLayout> mVertexLayout;    // 顶点输入布局
    ComPtr<ID3D11Buffer> mVertexBuffer;         // 顶点缓冲区
    ComPtr<ID3D11Buffer> mIndexBuffer;			// 索引缓冲区
    ComPtr<ID3D11Buffer> mConstantBuffer;	    // 常量缓冲区

    ComPtr<ID3D11VertexShader> mVertexShader;   // 顶点输入布局
    ComPtr<ID3D11PixelShader> mPixelShader;     // 像素着色器
    ConstantBuffer mVSCPUBuffer;	                // 用于修改GPU常量缓冲区的变量
};


#endif