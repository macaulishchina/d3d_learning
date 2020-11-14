#include "GameObject.h"

GameObject::GameObject()
    : mIndexCount(), mVertexStride(){
}

Transform& GameObject::GetTransform(){
    return mTransform;
}

const Transform& GameObject::GetTransform() const
{
    return mTransform;
}

template<class VertexType, class IndexType>
void GameObject::SetBuffer(ID3D11Device* device, const Geometry::MeshData<VertexType, IndexType>& meshData){
    // 释放旧资源
    mVertexBuffer.Reset();
    mIndexBuffer.Reset();

    // 设置顶点缓冲区描述
    mVertexStride = sizeof(VertexType);
    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = (UINT)meshData.vertexVec.size() * mVertexStride;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    // 新建顶点缓冲区
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = meshData.vertexVec.data();
    HR(device->CreateBuffer(&vbd, &InitData, mVertexBuffer.GetAddressOf()));


    // 设置索引缓冲区描述
    mIndexCount = (UINT)meshData.indexVec.size();
    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = mIndexCount * sizeof(IndexType);
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    // 新建索引缓冲区
    InitData.pSysMem = meshData.indexVec.data();
    HR(device->CreateBuffer(&ibd, &InitData, mIndexBuffer.GetAddressOf()));



}

void GameObject::SetTexture(ID3D11ShaderResourceView* texture){
    mTexture = texture;
}

void GameObject::Draw(ID3D11DeviceContext* deviceContext){
    // 设置顶点/索引缓冲区
    //UINT strides = m_VertexStride;
    //UINT offsets = 0;
    //deviceContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &strides, &offsets);
    //deviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    //// 获取之前已经绑定到渲染管线上的常量缓冲区并进行修改
    //ComPtr<ID3D11Buffer> cBuffer = nullptr;
    //deviceContext->VSGetConstantBuffers(0, 1, cBuffer.GetAddressOf());
    //CBChangesEveryDrawing cbDrawing;

    //// 内部进行转置
    //XMMATRIX W = m_Transform.GetLocalToWorldMatrixXM();
    //cbDrawing.world = XMMatrixTranspose(W);
    //cbDrawing.worldInvTranspose = XMMatrixTranspose(InverseTranspose(W));

    //// 更新常量缓冲区
    //D3D11_MAPPED_SUBRESOURCE mappedData;
    //HR(deviceContext->Map(cBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
    //memcpy_s(mappedData.pData, sizeof(CBChangesEveryDrawing), &cbDrawing, sizeof(CBChangesEveryDrawing));
    //deviceContext->Unmap(cBuffer.Get(), 0);

    //// 设置纹理
    //deviceContext->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());
    //// 可以开始绘制
    //deviceContext->DrawIndexed(m_IndexCount, 0, 0);
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
