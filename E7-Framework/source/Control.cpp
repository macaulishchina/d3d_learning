//#include "Control.h"
//#include "DXTrace.h"
//#include "d3dUtil.h"
//
//using namespace DirectX;
//
////ResourceController::ResourceController(int a) :
////    mModelBuffer(),
////    mCameraBuffer(),
////    mMaterialBuffer(),
////    mDirLightBuffer(),
////    mPointLightBuffer(),
////    mSpotLightBuffer(),
////    mModelChanged(false),
////    mCameraChanged(false),
////    mMaterialChanged(false),
////    mDirLightChanged(false),
////    mPointLightChanged(false),
////    mSpotLightChanged(false) {
////}
//
//void ResourceController::InitBuffer() {
//
//    ////###初始化VS/PS常量缓冲区的值
//    ////初始化顶点常量缓冲区变量
//    //mModelBuffer.model = XMMatrixIdentity();
//    //mModelBuffer.adjustNormal = XMMatrixIdentity();
//    //mCameraBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
//    //    XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
//    //    XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
//    //    XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
//    //));
//    //mCameraBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, mApp->AspectRatio(), 1.0f, 1000.0f));
//    //mCameraBuffer.eyePos = XMFLOAT3(0.0f, 0.0f, -5.0f);
//
//    //// 初始化默认光照
//    //// 方向光
//    //mDirLightBuffer.dirLight[0].ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
//    //mDirLightBuffer.dirLight[0].diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
//    //mDirLightBuffer.dirLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
//    //mDirLightBuffer.dirLight[0].direction = XMFLOAT3(-0.577f, -0.577f, 0.577f);
//    //// 点光
//    //mPointLightBuffer.pointLight[0].position = XMFLOAT3(0.0f, 0.0f, -10.0f);
//    //mPointLightBuffer.pointLight[0].ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
//    //mPointLightBuffer.pointLight[0].diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
//    //mPointLightBuffer.pointLight[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
//    //mPointLightBuffer.pointLight[0].att = XMFLOAT3(0.0f, 0.1f, 0.0f);
//    //mPointLightBuffer.pointLight[0].range = 25.0f;
//    //// 聚光灯
//    //mSpotLightBuffer.spotLight[0].position = XMFLOAT3(0.0f, 0.0f, -5.0f);
//    //mSpotLightBuffer.spotLight[0].direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
//    //mSpotLightBuffer.spotLight[0].ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
//    //mSpotLightBuffer.spotLight[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//    //mSpotLightBuffer.spotLight[0].specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//    //mSpotLightBuffer.spotLight[0].att = XMFLOAT3(1.0f, 0.0f, 0.0f);
//    //mSpotLightBuffer.spotLight[0].spot = 12.0f;
//    //mSpotLightBuffer.spotLight[0].range = 10000.0f;
//
//    //// 初始化用于PS的常量缓冲区的值
//    //mMaterialBuffer.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
//    //mMaterialBuffer.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//    //mMaterialBuffer.material.specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 5.0f);
//
//    //// 设置各类光源数量
//    //mDirLightBuffer.numDirLight = 1;
//    //mPointLightBuffer.numPointLight = 1;
//    //mSpotLightBuffer.numSpotLight = 1;
//
//    //// ###设置VS/PS常量缓冲区描述
//    //D3D11_BUFFER_DESC cbd;
//    //ZeroMemory(&cbd, sizeof(cbd));
//    //cbd.Usage = D3D11_USAGE_DYNAMIC;
//    //cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//    //cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//    ////创建变量缓冲区
//    //cbd.ByteWidth = sizeof(DT::CBModelLocation);
//    //HR(mApp->mD3dDevice->CreateBuffer(&cbd, nullptr, mConstBuffers[0].GetAddressOf()));
//    //cbd.ByteWidth = sizeof(DT::CBCamera);
//    //HR(mApp->mD3dDevice->CreateBuffer(&cbd, nullptr, mConstBuffers[1].GetAddressOf()));
//    //cbd.ByteWidth = sizeof(DT::CBMaterial);
//    //HR(mApp->mD3dDevice->CreateBuffer(&cbd, nullptr, mConstBuffers[2].GetAddressOf()));
//    //cbd.ByteWidth = sizeof(DT::CBDirLight);
//    //HR(mApp->mD3dDevice->CreateBuffer(&cbd, nullptr, mConstBuffers[3].GetAddressOf()));
//    //cbd.ByteWidth = sizeof(DT::CBPointLight);
//    //HR(mApp->mD3dDevice->CreateBuffer(&cbd, nullptr, mConstBuffers[4].GetAddressOf()));
//    //cbd.ByteWidth = sizeof(DT::CBSpotLight);
//    //HR(mApp->mD3dDevice->CreateBuffer(&cbd, nullptr, mConstBuffers[5].GetAddressOf()));
//    //// 绑定缓存到对应寄存器
//    //mApp->mD3dImmediateContext->VSSetConstantBuffers(0, 1, mConstBuffers[0].GetAddressOf());
//    //mApp->mD3dImmediateContext->VSSetConstantBuffers(1, 1, mConstBuffers[1].GetAddressOf());
//    //mApp->mD3dImmediateContext->PSSetConstantBuffers(1, 1, mConstBuffers[1].GetAddressOf());
//    //mApp->mD3dImmediateContext->PSSetConstantBuffers(2, 1, mConstBuffers[2].GetAddressOf());
//    //mApp->mD3dImmediateContext->PSSetConstantBuffers(3, 1, mConstBuffers[3].GetAddressOf());
//    //mApp->mD3dImmediateContext->PSSetConstantBuffers(4, 1, mConstBuffers[4].GetAddressOf());
//    //mApp->mD3dImmediateContext->PSSetConstantBuffers(5, 1, mConstBuffers[5].GetAddressOf());
//
//    //// 设置调试对象名
//    //D3D11SetDebugObjectName(mConstBuffers[0].Get(), "VSModelLocationBuffer");
//    //D3D11SetDebugObjectName(mConstBuffers[1].Get(), "VSCameraBuffer");
//    //D3D11SetDebugObjectName(mConstBuffers[2].Get(), "VSMaterialBuffer");
//    //D3D11SetDebugObjectName(mConstBuffers[3].Get(), "PSDirLightBuffer");
//    //D3D11SetDebugObjectName(mConstBuffers[4].Get(), "PSPointLightBuffer");
//    //D3D11SetDebugObjectName(mConstBuffers[5].Get(), "PSSpotLightBuffer");
//
//}
//
//void ResourceController::setModel(const DirectX::XMMATRIX& model) {
//    mModelChanged = true;
//    mModelBuffer.model = XMMatrixTranspose(model);
//    mModelBuffer.adjustNormal = XMMatrixInverse(nullptr, model);
//}
//
//void ResourceController::setView(const DirectX::XMMATRIX& view) {
//    mCameraChanged = true;
//    mCameraBuffer.view = view;
//}
//
//void ResourceController::setProj(const DirectX::XMMATRIX& proj) {
//    mCameraChanged = true;
//    mCameraBuffer.proj = proj;
//}
//
//void ResourceController::setEyePosition(const DirectX::XMFLOAT3& position) {
//    mCameraChanged = true;
//    mCameraBuffer.eyePos = position;
//}
//
//inline DirectX::XMMATRIX ResourceController::getModel() const {
//    return mModelBuffer.model;
//}
//
//inline DirectX::XMMATRIX ResourceController::getView() const {
//    return mCameraBuffer.view;
//}
//
//inline DirectX::XMMATRIX ResourceController::getProj() const {
//    return mCameraBuffer.proj;
//}
//
//inline DirectX::XMFLOAT3 ResourceController::getEyePosition() const {
//    return mCameraBuffer.eyePos;
//}
//
//void ResourceController::setMaterialAmbient(const DirectX::XMFLOAT4& ambient) {
//    mMaterialChanged = true;
//    mMaterialBuffer.material.ambient = ambient;
//}
//
//void ResourceController::setMaterialDiffuse(const DirectX::XMFLOAT4& diffuse) {
//    mMaterialChanged = true;
//    mMaterialBuffer.material.diffuse = diffuse;
//}
//
//void ResourceController::setMaterialSpecular(const DirectX::XMFLOAT4& specular) {
//    mMaterialChanged = true;
//    mMaterialBuffer.material.specular = specular;
//}
//
//void ResourceController::setMaterialReflect(const DirectX::XMFLOAT4& reflect) {
//    mMaterialChanged = true;
//    mMaterialBuffer.material.reflect = reflect;
//}
//
//inline DirectX::XMFLOAT4 ResourceController::getMaterialAmbient() const {
//    return mMaterialBuffer.material.ambient;
//}
//
//inline DirectX::XMFLOAT4 ResourceController::getMaterialDiffuse() const {
//    return mMaterialBuffer.material.diffuse;
//}
//
//inline DirectX::XMFLOAT4 ResourceController::getMaterialSpecular() const {
//    return mMaterialBuffer.material.specular;
//}
//
//inline DirectX::XMFLOAT4 ResourceController::getMaterialReflect() const {
//    return mMaterialBuffer.material.reflect;
//}
//
////void ResourceController::addOrUpdateLight(const std::string& name, const DT::AbstractLight& light) {
////    switch (light.type()) {
////    case DT::LightType::DirectionalLightType:
////        assert(mDirLightBuffer.numDirLight < DIRECTIONAL_LIGHT_NUMS);
////        mDirLightBuffer.dirLight[mDirLightBuffer.numDirLight] = dynamic_cast<const DT::DirectionalLight&>(light);
////        mLights[name] = mDirLightBuffer.dirLight[mDirLightBuffer.numDirLight];
////        mDirLightBuffer.numDirLight++;
////        break;
////    case DT::LightType::PointLightType:
////        assert(mPointLightBuffer.numPointLight < POINT_LIGHT_NUMS);
////        mPointLightBuffer.pointLight[mPointLightBuffer.numPointLight] = dynamic_cast<const DT::PointLight&>(light);
////        mLights[name] = mPointLightBuffer.pointLight[mPointLightBuffer.numPointLight];
////        mPointLightBuffer.numPointLight++;
////        break;
////    case DT::LightType::SpotLightType:
////        assert(mSpotLightBuffer.numSpotLight < POINT_LIGHT_NUMS);
////        mSpotLightBuffer.spotLight[mSpotLightBuffer.numSpotLight] = dynamic_cast<const DT::SpotLight&>(light);
////        mLights[name] = mSpotLightBuffer.spotLight[mSpotLightBuffer.numSpotLight];
////        mSpotLightBuffer.numSpotLight++;
////        break;
////    default:
////        break;
////    }
////
////
////}
//
//void ResourceController::deleteLight(std::string name) {
//}
//
//
//void ResourceController::update() {
//    D3D11_MAPPED_SUBRESOURCE mappedData;
//    //HR(mApp->mD3dImmediateContext->Map(mConstBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
//    //memcpy_s(mappedData.pData, sizeof(DT::CBModelLocation), &mModelBuffer, sizeof(DT::CBModelLocation));
//    //mApp->mD3dImmediateContext->Unmap(mConstBuffers[0].Get(), 0);
//
//    //HR(mApp->mD3dImmediateContext->Map(mConstBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
//    //memcpy_s(mappedData.pData, sizeof(DT::CBCamera), &mCameraBuffer, sizeof(DT::CBCamera));
//    //mApp->mD3dImmediateContext->Unmap(mConstBuffers[1].Get(), 0);
//
//    //HR(mApp->mD3dImmediateContext->Map(mConstBuffers[2].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
//    //memcpy_s(mappedData.pData, sizeof(DT::CBMaterial), &mMaterialBuffer, sizeof(DT::CBMaterial));
//    //mApp->mD3dImmediateContext->Unmap(mConstBuffers[2].Get(), 0);
//
//    //HR(mApp->mD3dImmediateContext->Map(mConstBuffers[3].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
//    //memcpy_s(mappedData.pData, sizeof(DT::CBDirLight), &mDirLightBuffer, sizeof(DT::CBDirLight));
//    //mApp->mD3dImmediateContext->Unmap(mConstBuffers[3].Get(), 0);
//
//    //HR(mApp->mD3dImmediateContext->Map(mConstBuffers[4].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
//    //memcpy_s(mappedData.pData, sizeof(DT::CBPointLight), &mPointLightBuffer, sizeof(DT::CBPointLight));
//    //mApp->mD3dImmediateContext->Unmap(mConstBuffers[4].Get(), 0);
//
//    //HR(mApp->mD3dImmediateContext->Map(mConstBuffers[5].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
//    //memcpy_s(mappedData.pData, sizeof(DT::CBSpotLight), &mSpotLightBuffer, sizeof(DT::CBSpotLight));
//    //mApp->mD3dImmediateContext->Unmap(mConstBuffers[5].Get(), 0);
//}
