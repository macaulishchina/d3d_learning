#include "GameApp.h"

#include "DXTrace.h"
#include "d3dUtil.h"
using namespace DirectX;

const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[2] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

GameApp::GameApp(HINSTANCE hInstance) : D3DApp(hInstance) {}

GameApp::~GameApp() {}

bool GameApp::Init() {
    if (!D3DApp::Init()) return false;
    if (!InitEffect()) return false;
    if (!InitResource()) return false;
    return true;
}

void GameApp::OnResize() { D3DApp::OnResize(); }

void GameApp::UpdateScene(float dt) {}

void GameApp::DrawScene() {
    assert(mD3dImmediateContext);
    assert(mSwapChain);
    static float bg[4] = { 1.0f, 1.0f, 1.0f, 1.0f };  // RGBA = (0,0,0,255)
    mD3dImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(), bg);
    mD3dImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    mD3dImmediateContext->Draw(3, 0);

    HR(mSwapChain->Present(0, 0));
}

bool GameApp::InitEffect() {
    ComPtr<ID3DBlob> blob;
    //#1 编译并创建顶点着色器
    HR(CreateShaderFromFile(L"hlsl\\Triangle_VS.cso", L"hlsl\\Triangle_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
    HR(mD3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf()));
    //#2 创建布局并绑定到顶点着色器
    HR(mD3dDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), mVertexLayout.GetAddressOf()));
    //#3 编译并创建像素着色器
    HR(CreateShaderFromFile(L"hlsl\\Triangle_PS.cso", L"hlsl\\Triangle_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
    HR(mD3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf()));

    return true;
}

bool GameApp::InitResource() {
    //顶点数据
    VertexPosColor vertices[] = {
        { XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
    };
    //设置顶点缓冲数据描述
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(vertices);
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    //创建顶点缓冲区
    D3D11_SUBRESOURCE_DATA vertexData;
    ZeroMemory(&vertexData, sizeof(vertexData));
    vertexData.pSysMem = vertices;
    HR(mD3dDevice->CreateBuffer(&vbd, &vertexData, mVertexBuffer.GetAddressOf()));

    UINT stride = sizeof(VertexPosColor);
    UINT offset = 0;
    mD3dImmediateContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);
    //设置图元类型，设定输入布局
    mD3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mD3dImmediateContext->IASetInputLayout(mVertexLayout.Get());
    //将着色器绑定到渲染管线
    mD3dImmediateContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
    mD3dImmediateContext->PSSetShader(mPixelShader.Get(), nullptr, 0);

    // 设置调试对象名
    D3D11SetDebugObjectName(mVertexLayout.Get(), "VertexPosColorLayout");
    D3D11SetDebugObjectName(mVertexBuffer.Get(), "VertexBuffer");
    D3D11SetDebugObjectName(mVertexShader.Get(), "Trangle_VS");
    D3D11SetDebugObjectName(mPixelShader.Get(), "Trangle_PS");
    return true;
}