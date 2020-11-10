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
    GameApp(HINSTANCE hInstance);
    ~GameApp();

    bool Init();
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene();

private:
    bool InitEffect();
    bool InitResource();

    ComPtr<ID3D11InputLayout> mVertexLayout;
    ComPtr<ID3D11Buffer> mVertexBuffer;
    ComPtr<ID3D11VertexShader> mVertexShader;
    ComPtr<ID3D11PixelShader> mPixelShader;
};


#endif