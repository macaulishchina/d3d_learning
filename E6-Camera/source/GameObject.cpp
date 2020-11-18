#include "GameObject.h"
#include "Resource.h"
#include "DXTrace.h"

using namespace DirectX;

GameObject::GameObject()
    : mIndexCount(), mVertexStride(){
}

Transform& GameObject::GetTransform() {
    return mTransform;
}

const Transform& GameObject::GetTransform() const
{
    return mTransform;
}


void GameObject::SetTexture(ID3D11ShaderResourceView* texture) {
    mTexture = texture;
}

void GameObject::Draw(ID3D11DeviceContext* deviceContext) {
    // 设置顶点/索引缓冲区
    UINT strides = mVertexStride;
    UINT offsets = 0;
    deviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &strides, &offsets);
    deviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    // 获取之前已经绑定到渲染管线上的常量缓冲区并进行修改
    ComPtr<ID3D11Buffer> cBuffer = nullptr;
    deviceContext->VSGetConstantBuffers(CONST_BUFFER_MODEL_INDEX, 1, cBuffer.GetAddressOf());

    // 内部进行转置
    XMMATRIX W = mTransform.GetLocalToWorldMatrixXM();
    mModelBuffer.model = XMMatrixTranspose(W);
    mModelBuffer.adjustNormal = XMMatrixInverse(nullptr, W);

    // 更新常量缓冲区
    D3D11_MAPPED_SUBRESOURCE mappedData;
    HR(deviceContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    memcpy_s(mappedData.pData, sizeof(DT::CBModelLocation), &mModelBuffer, sizeof(DT::CBModelLocation));
    deviceContext->Unmap(cBuffer.Get(), 0);

    //// 设置纹理
    deviceContext->PSSetShaderResources(0, 1, mTexture.GetAddressOf());
    //// 可以开始绘制
    deviceContext->DrawIndexed(mIndexCount, 0, 0);
}

void GameObject::SetDebugObjectName(const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    D3D11SetDebugObjectName(m_pVertexBuffer.Get(), name + ".VertexBuffer");
    D3D11SetDebugObjectName(m_pIndexBuffer.Get(), name + ".IndexBuffer");
#else
    UNREFERENCED_PARAMETER(name);
#endif
}
