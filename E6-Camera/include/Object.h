#ifndef __OBJECT_H__
#define __OBJECT_H__
#include "DT.h"
#include "Transform.h"
#include "d3dApp.h"

class GameObject {
 public:
  GameObject();

  // ��ȡ����任
  Transform& GetTransform();
  // ��ȡ����任
  const Transform& GetTransform() const;

  // ���û�����
  template <class VertexType, class IndexType>
  void SetBuffer(ID3D11Device* device,
                 const Geometry::MeshData<VertexType, IndexType>& meshData);
  // ��������
  void SetTexture(ID3D11ShaderResourceView* texture);
  
  void SetMaterialAmbient(const DirectX::XMFLOAT4& ambient);
  void SetMaterialDiffuse(const DirectX::XMFLOAT4& diffuse);
  void SetMaterialSpecular(const DirectX::XMFLOAT4& specular);
  void SetMaterialReflect(const DirectX::XMFLOAT4& reflect);

  const DirectX::XMFLOAT4& GetMaterialAmbient() const;
  const DirectX::XMFLOAT4& GetMaterialDiffuse() const;
  const DirectX::XMFLOAT4& GetMaterialSpecular() const;
  const DirectX::XMFLOAT4& GetMaterialReflect() const;
  // ����
  void Draw(ID3D11DeviceContext* deviceContext);

  // ���õ��Զ�����
  // �����������������ã����Զ�����Ҳ��Ҫ����������
  void SetDebugObjectName(const std::string& name);

 private:
  template <class T>
  using ComPtr = Microsoft::WRL::ComPtr<T>;
  Transform mTransform;                       // ����任��Ϣ
  ComPtr<ID3D11ShaderResourceView> mTexture;  // ����
  ComPtr<ID3D11Buffer> mVertexBuffer;         // ���㻺����
  ComPtr<ID3D11Buffer> mIndexBuffer;          // ����������
  UINT mVertexStride;                         // �����ֽڴ�С
  UINT mIndexCount;                           // ������Ŀ
};
#endif