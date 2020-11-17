#ifndef __OBJECT_H__
#define __OBJECT_H__
#include "DT.h"
#include "Transform.h"
#include "d3dApp.h"

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
  
  void SetMaterialAmbient(const DirectX::XMFLOAT4& ambient);
  void SetMaterialDiffuse(const DirectX::XMFLOAT4& diffuse);
  void SetMaterialSpecular(const DirectX::XMFLOAT4& specular);
  void SetMaterialReflect(const DirectX::XMFLOAT4& reflect);

  const DirectX::XMFLOAT4& GetMaterialAmbient() const;
  const DirectX::XMFLOAT4& GetMaterialDiffuse() const;
  const DirectX::XMFLOAT4& GetMaterialSpecular() const;
  const DirectX::XMFLOAT4& GetMaterialReflect() const;
  // 更新
  void Draw(ID3D11DeviceContext* deviceContext);

  // 设置调试对象名
  // 若缓冲区被重新设置，调试对象名也需要被重新设置
  void SetDebugObjectName(const std::string& name);

 private:
  template <class T>
  using ComPtr = Microsoft::WRL::ComPtr<T>;
  Transform mTransform;                       // 物体变换信息
  ComPtr<ID3D11ShaderResourceView> mTexture;  // 纹理
  ComPtr<ID3D11Buffer> mVertexBuffer;         // 顶点缓冲区
  ComPtr<ID3D11Buffer> mIndexBuffer;          // 索引缓冲区
  UINT mVertexStride;                         // 顶点字节大小
  UINT mIndexCount;                           // 索引数目
};
#endif