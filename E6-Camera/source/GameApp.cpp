#include "GameApp.h"

#include "DXTrace.h"
#include "d3dUtil.h"
#include "Data.h"
using namespace DirectX;
#include <iostream>
#include "DDSTextureLoader.h"
GameApp::GameApp(HINSTANCE hInstance) :
    D3DApp(hInstance),
    mIndexCount(0),
    mController(this){
}

GameApp::~GameApp() {}

bool GameApp::Init() {
    if (!D3DApp::Init()) return false;
    if (!InitEffect()) return false;
    if (!InitResource()) return false;
    if (!InitScene()) return false;
    return true;
}

bool GameApp::InitEffect() {
    return mController.InitEffect();
}

bool GameApp::InitResource() {

    mController.InitBuffer();

    // ###初始化纹理和采样器状态
    // 初始化采样器状态
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.BorderColor[0] = 0.0f;
    sampDesc.BorderColor[1] = 0.0f;
    sampDesc.BorderColor[2] = 0.0f;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    HR(mD3dDevice->CreateSamplerState(&sampDesc, mSamplerState.GetAddressOf()));
    mD3dImmediateContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());

    // ###初始化光栅化状态
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = mCULLRender ? D3D11_CULL_FRONT : D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthClipEnable = true;
    HR(mD3dDevice->CreateRasterizerState(&rasterizerDesc, mRSWireframe.GetAddressOf()));

    // 设置图元类型，设定输入布局
    mD3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D11SetDebugObjectName(mSamplerState.Get(), "SamplerState");

    return true;
}

bool GameApp::InitScene() {
    auto meshData = Geometry::CreateBox();
    ResetMesh(meshData);
    
    // 初始化木箱纹理
    HR(CreateDDSTextureFromFile(mD3dDevice.Get(), L"..\\Texture\\WoodCrate.dds", nullptr, mWoodCrate.GetAddressOf()));
    // 像素着色阶段设置好采样器
    mD3dImmediateContext->PSSetShaderResources(0, 1, mWoodCrate.GetAddressOf());

    return true;
}

void GameApp::UpdateScene(float dt) {
    static float phi = 0.0f, theta = 0.0f;
    Mouse::State currMouseState = mMouse->GetState();
    Mouse::State lastMouseState = mMouseTracker->GetLastState();
    Keyboard::State currKeyState = mKeyboard->GetState();
    bool drag = currMouseState.leftButton && mMouseTracker->leftButton == mMouseTracker->HELD;
    if (drag) {
        phi -= (currMouseState.y - lastMouseState.y) * 0.01f;
        theta -= (currMouseState.x - lastMouseState.x) * 0.01f;
    }
    // 绘制立方体
    XMMATRIX M = XMMatrixRotationX(phi) * XMMatrixRotationY(theta);

    // 键盘切换光栅化状态
    if (mKeyboardTracker->IsKeyReleased(Keyboard::S)) {
        mIsWireframeMode = !mIsWireframeMode;
        mD3dImmediateContext->RSSetState(mIsWireframeMode ? mRSWireframe.Get() : nullptr);
    }

    mController.SetModel(M);
    mController.Update();

    mD3dImmediateContext->DrawIndexed(mIndexCount, 0, 0);
    mMouseTracker->Update(currMouseState);
    mKeyboardTracker->Update(currKeyState);

}

void GameApp::Update2DScene() {
    if (mD2dRenderTarget != nullptr) {
        mD2dRenderTarget->BeginDraw();
        std::wstring textStr = L"切换灯光类型: 1-平行光 2-点光 3-聚光灯\n"
            L"切换模型: Q-立方体 W-球体 E-圆柱体 R-圆锥体\n"
            L"S-切换模式 当前模式: ";
        if (mIsWireframeMode)
            textStr += L"线框模式";
        else
            textStr += L"面模式";
        mD2dRenderTarget->DrawTextW(textStr.c_str(), (UINT32)textStr.size(), mTextFormat.Get(),
            D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, mColorBrush.Get());
        HR(mD2dRenderTarget->EndDraw());
    }
}

void GameApp::DrawScene() {
    assert(mD3dImmediateContext);
    assert(mSwapChain);
    static float bg[4] = { 0.0f, 0.0f, 0.0f, 1.0f };  // RGBA = (0,0,0,255)
    mD3dImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(), bg);
    mD3dImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    UpdateScene(mTimer.DeltaTime());
    Update2DScene();
    HR(mSwapChain->Present(0, 0));
}

void GameApp::OnResize() {
    D3DApp::OnResize();
    mController.SetProj(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
}

template<class VertexType>
bool GameApp::ResetMesh(const Geometry::MeshData<VertexType>& meshData) {
    // 释放旧资源
    mGPUVertexBuffer.Reset();
    mGPUIndexBuffer.Reset();

    // 设置顶点缓冲区描述
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = (UINT)meshData.vertexVec.size() * sizeof(VertexType);
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    // 新建顶点缓冲区
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = meshData.vertexVec.data();
    HR(mD3dDevice->CreateBuffer(&vbd, &InitData, mGPUVertexBuffer.GetAddressOf()));
    // 输入装配阶段的顶点缓冲区设置
    UINT stride = sizeof(VertexType);	// 跨越字节数
    UINT offset = 0;							    // 起始偏移量
    mD3dImmediateContext->IASetVertexBuffers(0, 1, mGPUVertexBuffer.GetAddressOf(), &stride, &offset);

    // 设置索引缓冲区描述
    mIndexCount = (UINT)meshData.indexVec.size();
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = mIndexCount * sizeof(DWORD);
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    // 新建索引缓冲区
    InitData.pSysMem = meshData.indexVec.data();
    HR(mD3dDevice->CreateBuffer(&ibd, &InitData, mGPUIndexBuffer.GetAddressOf()));
    // 输入装配阶段的索引缓冲区设置
    mD3dImmediateContext->IASetIndexBuffer(mGPUIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    // 设置调试对象名
    D3D11SetDebugObjectName(mGPUVertexBuffer.Get(), "VertexBuffer");
    D3D11SetDebugObjectName(mGPUIndexBuffer.Get(), "IndexBuffer");

    return true;
}