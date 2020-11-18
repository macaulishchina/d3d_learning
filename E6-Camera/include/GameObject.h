#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__
#include "DT.h"
#include "Transform.h"
#include "d3dApp.h"
#include "DXTrace.h"

class GameObject {
 public:
  GameObject();

  // 获取物体变换
  Transform& GetTransform();
  // 获取物体变换
  const Transform& GetTransform() const;

  // 设置缓冲区
  template <class VertexType, class IndexType>
  void SetBuffer(ID3D11Device* device,
      const Geometry::MeshData<VertexType, IndexType>& meshData);
  // 设置纹理
  void SetTexture(ID3D11ShaderResourceView* texture);

  // 绘制
  void Draw(ID3D11DeviceContext* deviceContext);

  // 设置调试对象名
  // 若缓冲区被重新设置，调试对象名也需要被重新设置
  void SetDebugObjectName(const std::string& name);

  inline bool IsVisable() const {
      return mVisable;
  }

  inline void SetVisable(bool flag = true) {
      mVisable = flag;
  }

 //private:

  template <class T>
  using ComPtr = Microsoft::WRL::ComPtr<T>;
  ComPtr<ID3D11Buffer> mVertexBuffer;           // 顶点缓冲区
  ComPtr<ID3D11Buffer> mIndexBuffer;            // 索引缓冲区
  ComPtr<ID3D11ShaderResourceView> mTexture;    // 纹理
  Transform mTransform;                         // 物体变换信息
  DT::CBMaterial mMaterial = {};                //材质常量
  DT::CBModelLocation mModelBuffer = {};        // 模型位置相关量
  UINT mVertexStride;                           // 顶点字节大小
  UINT mIndexCount;                             // 索引数目

  bool mVisable;                                //该模型是否可见
};

template<class VertexType, class IndexType>
void GameObject::SetBuffer(ID3D11Device* device, const Geometry::MeshData<VertexType, IndexType>& meshData)
{
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

#endif
