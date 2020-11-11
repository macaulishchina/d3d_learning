#include "GameApp.h"

#include "DXTrace.h"
#include "d3dUtil.h"
#include "Data.h"
using namespace DirectX;
#include <iostream>

GameApp::GameApp(HINSTANCE hInstance) :
    D3DApp(hInstance),
    mVSCPUBuffer(),
    mPSCPUBUffer(),
    mDirLight(),
    mPointLight(),
    mSpotLight(),
    mIndexCount(0) {
}

GameApp::~GameApp() {}

bool GameApp::Init() {
    if (!D3DApp::Init()) return false;
    if (!InitEffect()) return false;
    if (!InitResource()) return false;
    mMouse->SetWindow(mMainWnd);
    mMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
    return true;
}

void GameApp::OnResize() {
    D3DApp::OnResize();
    mVSCPUBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
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
    int scrollValue = mMouse->GetState().scrollWheelValue - mMouseTracker->GetLastState().scrollWheelValue;
    if (scrollValue) {
        mPSCPUBUffer.spotLight = mSpotLight;
        mSpotLight.spot += scrollValue / 10.0f;
        mSpotLight.spot = min(max(2, mSpotLight.spot), 512);
    }
    //绘制立方体
    XMMATRIX M = XMMatrixRotationX(phi) * XMMatrixRotationY(theta);
    mVSCPUBuffer.model = XMMatrixTranspose(M);
    mVSCPUBuffer.adjustNormal = XMMatrixInverse(nullptr, M);

    if (mKeyboardTracker->IsKeyReleased(Keyboard::D1)) {
        mPSCPUBUffer.dirLight = mDirLight;
        mPSCPUBUffer.pointLight = DT::PointLight();
        mPSCPUBUffer.spotLight = DT::SpotLight();
    }
    else if (mKeyboardTracker->IsKeyReleased(Keyboard::D2)) {
        mPSCPUBUffer.dirLight = DT::DirectionalLight();
        mPSCPUBUffer.pointLight = mPointLight;
        mPSCPUBUffer.spotLight = DT::SpotLight();
    }
    else if (mKeyboardTracker->IsKeyPressed(Keyboard::D3)) {
        mPSCPUBUffer.dirLight = DT::DirectionalLight();
        mPSCPUBUffer.pointLight = DT::PointLight();
        mPSCPUBUffer.spotLight = mSpotLight;
    }

    // 键盘切换模型类型
    if (mKeyboardTracker->IsKeyReleased(Keyboard::Q)) {
        auto meshData = Geometry::CreateBox<DT::VertexPosNormalColor>();
        ResetMesh(meshData);
    }
    else if (mKeyboardTracker->IsKeyReleased(Keyboard::W)) {
        auto meshData = Geometry::CreateSphere<DT::VertexPosNormalColor>();
        ResetMesh(meshData);
    }
    else if (mKeyboardTracker->IsKeyReleased(Keyboard::E)) {
        auto meshData = Geometry::CreateCylinder<DT::VertexPosNormalColor>();
        ResetMesh(meshData);
    }
    else if (mKeyboardTracker->IsKeyReleased(Keyboard::R)) {
        auto meshData = Geometry::CreateCone<DT::VertexPosNormalColor>();
        ResetMesh(meshData);
    }
    // 键盘切换光栅化状态
    else if (mKeyboardTracker->IsKeyReleased(Keyboard::S)) {
        mIsWireframeMode = !mIsWireframeMode;
        mD3dImmediateContext->RSSetState(mIsWireframeMode ? mRSWireframe.Get() : nullptr);
    }

    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(mD3dImmediateContext->Map(mVSGPUBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(DT::VSConstantBuffer), &mVSCPUBuffer, sizeof(DT::VSConstantBuffer));
    mD3dImmediateContext->Unmap(mVSGPUBuffer.Get(), 0);

    HR(mD3dImmediateContext->Map(mPSGPUBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(DT::PSConstantBuffer), &mPSCPUBUffer, sizeof(DT::PSConstantBuffer));
    mD3dImmediateContext->Unmap(mPSGPUBuffer.Get(), 0);

    mD3dImmediateContext->DrawIndexed(mIndexCount, 0, 0);
    mMouseTracker->Update(currMouseState);
    mKeyboardTracker->Update(currKeyState);

}

void GameApp::DrawScene() {
    assert(mD3dImmediateContext);
    assert(mSwapChain);
    static float bg[4] = { 0.0f, 0.0f, 0.0f, 1.0f };  // RGBA = (0,0,0,255)
    mD3dImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(), bg);
    mD3dImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    UpdateScene(mTimer.DeltaTime());
    HR(mSwapChain->Present(0, 0));
}

bool GameApp::InitEffect() {
    ComPtr<ID3DBlob> blob;
    //#1 编译并创建顶点着色器
    HR(CreateShaderFromFile(L"hlsl\\VS.cso", L"hlsl\\VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
    HR(mD3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf()));
    //#2 创建布局并绑定到顶点着色器
    HR(mD3dDevice->CreateInputLayout(DT::VertexPosNormalColor::inputLayout, ARRAYSIZE(DT::VertexPosNormalColor::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), mVertexLayout.GetAddressOf()));
    //#3 编译并创建像素着色器
    HR(CreateShaderFromFile(L"hlsl\\PS.cso", L"hlsl\\PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
    HR(mD3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf()));

    return true;
}

bool GameApp::InitResource() {

    auto meshData = Geometry::CreateBox<DT::VertexPosNormalColor>();
    assert(ResetMesh(meshData));

    //设置常量缓冲区描述
    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd, sizeof(cbd));
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(DT::VSConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    HR(mD3dDevice->CreateBuffer(&cbd, nullptr, mVSGPUBuffer.GetAddressOf()));
    cbd.ByteWidth = sizeof(DT::PSConstantBuffer);
    HR(mD3dDevice->CreateBuffer(&cbd, nullptr, mPSGPUBuffer.GetAddressOf()));

    //初始化顶点常量缓冲区变量
    mVSCPUBuffer.model = XMMatrixIdentity();
    mVSCPUBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    ));
    mVSCPUBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
    mVSCPUBuffer.adjustNormal = XMMatrixIdentity();

    // 初始化默认光照
    // 方向光
    mDirLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    mDirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    mDirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mDirLight.direction = XMFLOAT3(-0.577f, -0.577f, 0.577f);
    // 点光
    mPointLight.position = XMFLOAT3(0.0f, 0.0f, -10.0f);
    mPointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    mPointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
    mPointLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mPointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
    mPointLight.range = 25.0f;
    // 聚光灯
    mSpotLight.position = XMFLOAT3(0.0f, 0.0f, -5.0f);
    mSpotLight.direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
    mSpotLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mSpotLight.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mSpotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mSpotLight.att = XMFLOAT3(1.0f, 0.0f, 0.0f);
    mSpotLight.spot = 12.0f;
    mSpotLight.range = 10000.0f;

    // 初始化用于PS的常量缓冲区的值
    mPSCPUBUffer.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mPSCPUBUffer.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mPSCPUBUffer.material.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
    mPSCPUBUffer.dirLight = mDirLight;
    mPSCPUBUffer.eyePos = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f);

    // 更新PS常量缓冲区资源
    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(mD3dImmediateContext->Map(mPSGPUBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(DT::PSConstantBuffer), &mPSCPUBUffer, sizeof(DT::PSConstantBuffer));
    mD3dImmediateContext->Unmap(mPSGPUBuffer.Get(), 0);

    // 初始化光栅化状态
//
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = mCULLRender ? D3D11_CULL_FRONT : D3D11_CULL_NONE;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthClipEnable = true;
    HR(mD3dDevice->CreateRasterizerState(&rasterizerDesc, mRSWireframe.GetAddressOf()));

    //设置图元类型，设定输入布局
    mD3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    mD3dImmediateContext->IASetInputLayout(mVertexLayout.Get());
    //将着色器绑定到渲染管线
    mD3dImmediateContext->VSSetShader(mVertexShader.Get(), nullptr, 0);
    mD3dImmediateContext->PSSetShader(mPixelShader.Get(), nullptr, 0);
    mD3dImmediateContext->VSSetConstantBuffers(0, 1, mVSGPUBuffer.GetAddressOf());
    mD3dImmediateContext->PSSetConstantBuffers(1, 1, mPSGPUBuffer.GetAddressOf());

    // 设置调试对象名
    D3D11SetDebugObjectName(mVertexLayout.Get(), "VertexLayout");
    D3D11SetDebugObjectName(mVSGPUBuffer.Get(), "VSConstantBuffer");
    D3D11SetDebugObjectName(mPSGPUBuffer.Get(), "PSConstantBuffer");
    D3D11SetDebugObjectName(mVertexShader.Get(), "VS");
    D3D11SetDebugObjectName(mPixelShader.Get(), "PS");

    return true;
}

bool GameApp::ResetMesh(const Geometry::MeshData<DT::VertexPosNormalColor>& meshData) {
    // 释放旧资源
    mVertexBuffer.Reset();
    mIndexBuffer.Reset();
    // 设置顶点缓冲区描述
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = (UINT)meshData.vertexVec.size() * sizeof(DT::VertexPosNormalColor);
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    // 新建顶点缓冲区
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = meshData.vertexVec.data();
    HR(mD3dDevice->CreateBuffer(&vbd, &InitData, mVertexBuffer.GetAddressOf()));
    // 输入装配阶段的顶点缓冲区设置
    UINT stride = sizeof(DT::VertexPosNormalColor);	// 跨越字节数
    UINT offset = 0;							    // 起始偏移量
    mD3dImmediateContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);

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
    HR(mD3dDevice->CreateBuffer(&ibd, &InitData, mIndexBuffer.GetAddressOf()));
    // 输入装配阶段的索引缓冲区设置
    mD3dImmediateContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    // 设置调试对象名
    D3D11SetDebugObjectName(mVertexBuffer.Get(), "VertexBuffer");
    D3D11SetDebugObjectName(mIndexBuffer.Get(), "IndexBuffer");

    return true;
}