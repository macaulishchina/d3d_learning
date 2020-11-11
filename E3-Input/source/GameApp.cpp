#include "GameApp.h"

#include "DXTrace.h"
#include "d3dUtil.h"
using namespace DirectX;

const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[2] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

// 设置立方体顶点
 //    5________ 6
 //    /|      /|
 //   /_|_____/ |
 //  1|4|_ _ 2|_|7
 //   | /     | /
 //   |/______|/
 //  0       3
GameApp::VertexPosColor vertices[] =
{
    { XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
    { XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
    { XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
    { XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
    { XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
    { XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
    { XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
    { XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
    { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
};
// 索引数组
DWORD indices[] = {
    // 正面
    0, 1, 2,
    2, 3, 0,
    // 左面
    4, 5, 1,
    1, 0, 4,
    // 顶面
    1, 5, 6,
    6, 2, 1,
    // 背面
    7, 6, 5,
    5, 4, 7,
    // 右面
    3, 2, 6,
    6, 7, 3,
    // 底面
    4, 0, 3,
    3, 7, 4,
    // 侧面
    8, 7, 3,
    4, 7, 8,
    8, 0, 4,
    8, 3, 0
};

GameApp::GameApp(HINSTANCE hInstance) : D3DApp(hInstance), mVSCPUBuffer() {}

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


    mMouseTracker->Update(currMouseState);
    mKeyboardTracker->Update(currKeyState);
    bool drag = currMouseState.leftButton && mMouseTracker->leftButton == mMouseTracker->HELD;

    if (drag) {
        phi -= (currMouseState.y - lastMouseState.y) * 0.01f;
        theta -= (currMouseState.x - lastMouseState.x) * 0.01f;
    }
    if (currKeyState.IsKeyDown(Keyboard::W)) phi += dt * 5;
    if (currKeyState.IsKeyDown(Keyboard::S)) phi -= dt * 5;
    if (currKeyState.IsKeyDown(Keyboard::A)) theta += dt * 5;
    if (currKeyState.IsKeyDown(Keyboard::D)) theta -= dt * 5;

    //绘制立方体
    mVSCPUBuffer.model = XMMatrixTranspose(XMMatrixIdentity() * XMMatrixRotationX(phi) * XMMatrixRotationY(theta));
    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(mD3dImmediateContext->Map(mConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(mVSCPUBuffer), &mVSCPUBuffer, sizeof(mVSCPUBuffer));
    mD3dImmediateContext->Unmap(mConstantBuffer.Get(), 0);
    mD3dImmediateContext->DrawIndexed(36, 0, 0);

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
    HR(CreateShaderFromFile(L"hlsl\\Cube_VS.cso", L"hlsl\\Cube_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
    HR(mD3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mVertexShader.GetAddressOf()));
    //#2 创建布局并绑定到顶点着色器
    HR(mD3dDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), mVertexLayout.GetAddressOf()));
    //#3 编译并创建像素着色器
    HR(CreateShaderFromFile(L"hlsl\\Cube__PS.cso", L"hlsl\\Cube_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
    HR(mD3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, mPixelShader.GetAddressOf()));

    return true;
}


bool GameApp::InitResource() {


    //设置顶点缓冲数据描述
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.ByteWidth = sizeof(vertices);
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    //创建顶点缓冲区
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    HR(mD3dDevice->CreateBuffer(&vbd, &InitData, mVertexBuffer.GetAddressOf()));

    // 设置索引缓冲区描述
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(indices);
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    // 新建索引缓冲区
    InitData.pSysMem = indices;
    HR(mD3dDevice->CreateBuffer(&ibd, &InitData, mIndexBuffer.GetAddressOf()));
    // 设置索引生效
    mD3dImmediateContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    //设置常量缓冲区描述
    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd, sizeof(cbd));
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    HR(mD3dDevice->CreateBuffer(&cbd, nullptr, mConstantBuffer.GetAddressOf()));
    mD3dImmediateContext->VSSetConstantBuffers(0, 1, mConstantBuffer.GetAddressOf());

    //初始化常量缓冲区变量
    mVSCPUBuffer.model = XMMatrixIdentity();
    mVSCPUBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
        XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    ));
    mVSCPUBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));

    // 输入装配阶段的顶点缓冲区设置
    UINT stride = sizeof(VertexPosColor);	// 跨越字节数
    UINT offset = 0;						// 起始偏移量
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
    D3D11SetDebugObjectName(mIndexBuffer.Get(), "IndexBuffer");
    D3D11SetDebugObjectName(mConstantBuffer.Get(), "ConstantBuffer");
    D3D11SetDebugObjectName(mVertexShader.Get(), "Cube_VS");
    D3D11SetDebugObjectName(mPixelShader.Get(), "Cube_PS");

    return true;
}