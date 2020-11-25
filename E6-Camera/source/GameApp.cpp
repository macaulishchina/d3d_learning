#include "GameApp.h"

#include <iostream>
#include "DXTrace.h"
#include "d3dUtil.h"
#include "Data.h"
#include "Light.h"

using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance) :
    D3DApp(hInstance),
    mController(this) {
}

bool GameApp::Init() {
    if (!mController.Init()) return false;
    if (!InitScene()) return false;
    return true;
}

bool GameApp::InitScene() {

    std::shared_ptr<DirectionLightSource> ls = std::make_shared<DirectionLightSource>();
    ls->mDirectionalLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    ls->mDirectionalLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    ls->mDirectionalLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    ls->mDirectionalLight.direction = XMFLOAT3(-0.577f, -0.577f, 0.577f);
    mController.GetLightManger()->SetDirectionalLight("test", ls);
    std::shared_ptr<FirstPersonCamera> cc = std::make_shared<FirstPersonCamera>();
    cc->SetPosition(0.0f, 0.0f, -5.0f);
    cc->LookAt({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f });
    cc->SetFrustum(XM_PIDIV2, AspectRatio(), 0.5f, 1000.0f);
    mController.GetCameraManger()->SetCamera("test", cc);
    mController.GetCameraManger()->UseCamera("test");
    std::shared_ptr<GameObject> obj = std::make_shared<GameObject>();
    obj->SetBuffer(mD3dDevice.Get(), Geometry::CreateBox());
    obj->SetTexture(mController.mWoodCrate.Get());
    obj->SetVisable();
    obj->mMaterial.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    obj->mMaterial.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    obj->mMaterial.material.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 5.0f);
    obj->mMaterial.material.reflect = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    mController.GetObjectManger()->SetGameObject("test", obj);
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

    mController.Update();

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
    static float bg[4] = { 0.3f, 0.3f, 0.3f, 1.0f };  // RGBA = (0,0,0,255)
    mD3dImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(), bg);
    mD3dImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    UpdateScene(mTimer.DeltaTime());
    Update2DScene();
    HR(mSwapChain->Present(0, 0));
}

void GameApp::OnResize() {
    D3DApp::OnResize();
}