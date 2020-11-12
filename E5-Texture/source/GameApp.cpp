#include "GameApp.h"

#include "DXTrace.h"
#include "d3dUtil.h"
#include "Data.h"
using namespace DirectX;
#include <iostream>
#include "DDSTextureLoader.h"
GameApp::GameApp(HINSTANCE hInstance) :
    D3DApp(hInstance),
    mVSCPUBuffer(),
    mPSCPUBUffer(),
    mIndexCount(0) {
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
    ComPtr<ID3DBlob> blob;
    //编译并创建顶点着色器(3D)
    HR(CreateShaderFromFile(L"hlsl\\Basic_VS_3D.cso", L"hlsl\\Basic_VS_3D.hlsl", "VS_3D", "vs_5_0", blob.ReleaseAndGetAddressOf()));
    HR(mD3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mVertexShader3D.GetAddressOf()));
    //创建顶点布局并绑定到顶点着色器(3D)
    HR(mD3dDevice->CreateInputLayout(DT::VertexPosNormalTex::inputLayout, ARRAYSIZE(DT::VertexPosNormalTex::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), mVertexLayout3D.GetAddressOf()));
    // 编译并创建像素着色器
    HR(CreateShaderFromFile(L"hlsl\\Basic_PS_3D.cso", L"hlsl\\Basic_PS_3D.hlsl", "PS_3D", "ps_5_0", blob.ReleaseAndGetAddressOf()));
    HR(mD3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mPixelShader3D.GetAddressOf()));
    // 将着色器绑定到渲染管线
    mD3dImmediateContext->VSSetShader(mVertexShader3D.Get(), nullptr, 0);
    mD3dImmediateContext->PSSetShader(mPixelShader3D.Get(), nullptr, 0);
    mD3dImmediateContext->IASetInputLayout(mVertexLayout3D.Get());

    D3D11SetDebugObjectName(mVertexShader3D.Get(), "VS_3D");
    D3D11SetDebugObjectName(mPixelShader3D.Get(), "PS_3D");
    D3D11SetDebugObjectName(mVertexLayout3D.Get(), "VertexLayout3D");

    return true;
}

bool GameApp::InitResource() {

    // ###设置VS/PS常量缓冲区描述
    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd, sizeof(cbd));
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(DT::VSConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    HR(mD3dDevice->CreateBuffer(&cbd, nullptr, mGPUVSConstBuffer.GetAddressOf()));
    cbd.ByteWidth = sizeof(DT::PSConstantBuffer);
    HR(mD3dDevice->CreateBuffer(&cbd, nullptr, mGPUPSConstBuffer.GetAddressOf()));
    
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

    //设置图元类型，设定输入布局
    mD3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    mD3dImmediateContext->VSSetConstantBuffers(0, 1, mGPUVSConstBuffer.GetAddressOf());
    mD3dImmediateContext->PSSetConstantBuffers(1, 1, mGPUPSConstBuffer.GetAddressOf());


    // 设置调试对象名
    D3D11SetDebugObjectName(mGPUVSConstBuffer.Get(), "VSConstantBuffer");
    D3D11SetDebugObjectName(mGPUPSConstBuffer.Get(), "PSConstantBuffer");
    D3D11SetDebugObjectName(mSamplerState.Get(), "SamplerState");

    return true;
}

bool GameApp::InitScene() {
    auto meshData = Geometry::CreateBox();
    ResetMesh(meshData);
    
    //###初始化VS/PS常量缓冲区的值
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
    mPSCPUBUffer.dirLight[0].ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    mPSCPUBUffer.dirLight[0].diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    mPSCPUBUffer.dirLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mPSCPUBUffer.dirLight[0].direction = XMFLOAT3(-0.577f, -0.577f, 0.577f);
    // 点光
    mPSCPUBUffer.pointLight[0].position = XMFLOAT3(0.0f, 0.0f, -10.0f);
    mPSCPUBUffer.pointLight[0].ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    mPSCPUBUffer.pointLight[0].diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
    mPSCPUBUffer.pointLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mPSCPUBUffer.pointLight[0].att = XMFLOAT3(0.0f, 0.1f, 0.0f);
    mPSCPUBUffer.pointLight[0].range = 25.0f;
    // 聚光灯
    mPSCPUBUffer.spotLight[0].position = XMFLOAT3(0.0f, 0.0f, -5.0f);
    mPSCPUBUffer.spotLight[0].direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
    mPSCPUBUffer.spotLight[0].ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    mPSCPUBUffer.spotLight[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mPSCPUBUffer.spotLight[0].specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mPSCPUBUffer.spotLight[0].att = XMFLOAT3(1.0f, 0.0f, 0.0f);
    mPSCPUBUffer.spotLight[0].spot = 12.0f;
    mPSCPUBUffer.spotLight[0].range = 10000.0f;

    // 初始化用于PS的常量缓冲区的值
    mPSCPUBUffer.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mPSCPUBUffer.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    mPSCPUBUffer.material.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 5.0f);
    // 设置各类光源数量
    mPSCPUBUffer.numDirLight = 1;
    mPSCPUBUffer.numPointLight = 1;
    mPSCPUBUffer.numSpotLight = 1;

    mPSCPUBUffer.eyePos = XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f);

    // 初始化木箱纹理
    HR(CreateDDSTextureFromFile(mD3dDevice.Get(), L"..\\Texture\\flare.dds", nullptr, mWoodCrate.GetAddressOf()));
    // 像素着色阶段设置好采样器
    mD3dImmediateContext->PSSetShaderResources(0, 1, mWoodCrate.GetAddressOf());
    HR(CreateDDSTextureFromFile(mD3dDevice.Get(), L"..\\Texture\\flarealpha.dds", nullptr, mWoodCrate.GetAddressOf()));
    mD3dImmediateContext->PSSetShaderResources(1, 1, mWoodCrate.GetAddressOf());

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
    int scrollValue = mMouse->GetState().scrollWheelValue - mMouseTracker->GetLastState().scrollWheelValue;
    if (scrollValue) {
        mPSCPUBUffer.spotLight[0].spot += scrollValue / 10.0f;
        mPSCPUBUffer.spotLight[0].spot = min(max(2, mPSCPUBUffer.spotLight[0].spot), 512);
    }
    // 绘制立方体
    XMMATRIX M = XMMatrixRotationX(phi) * XMMatrixRotationY(theta);
    mVSCPUBuffer.model = XMMatrixTranspose(M);
    mVSCPUBuffer.adjustNormal = XMMatrixInverse(nullptr, M);
    mVSCPUBuffer.texTransform = XMMatrixRotationZ(10*(phi+theta));
    // 键盘切换光栅化状态
    if (mKeyboardTracker->IsKeyReleased(Keyboard::S)) {
        mIsWireframeMode = !mIsWireframeMode;
        mD3dImmediateContext->RSSetState(mIsWireframeMode ? mRSWireframe.Get() : nullptr);
    }

    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(mD3dImmediateContext->Map(mGPUVSConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(DT::VSConstantBuffer), &mVSCPUBuffer, sizeof(DT::VSConstantBuffer));
    mD3dImmediateContext->Unmap(mGPUVSConstBuffer.Get(), 0);

    HR(mD3dImmediateContext->Map(mGPUPSConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(DT::PSConstantBuffer), &mPSCPUBUffer, sizeof(DT::PSConstantBuffer));
    mD3dImmediateContext->Unmap(mGPUPSConstBuffer.Get(), 0);

    //const wchar_t* ColorsTexture[6] = { 
    //    L"..\\Texture\\Yellow.dds",
    //    L"..\\Texture\\Blue.dds",
    //    L"..\\Texture\\Green.dds",
    //    L"..\\Texture\\Orange.dds",
    //    L"..\\Texture\\Red.dds",
    //    L"..\\Texture\\White.dds" };
    //for (int i = 1; i <= 6; i++) {
    //    HR(CreateDDSTextureFromFile(mD3dDevice.Get(), ColorsTexture[i-1], nullptr, mWoodCrate.GetAddressOf()));
    //    mD3dImmediateContext->PSSetShaderResources(0, 1, mWoodCrate.GetAddressOf());
    //    mD3dImmediateContext->DrawIndexed(6, (i-1)*6, 0);
    //}
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
    mVSCPUBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
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