#ifndef __Light_H__
#define __Light_H__
#include <DirectXMath.h>
#include <d3d11_1.h>

#include "DT.h"
#include "Transform.h"

enum class LightType { DirectionLightType, PointLightType, SpotLightType };

class LightSource {
 public:
  LightSource() = delete;
  virtual ~LightSource() = 0;

  //
  // ��ȡ��Դλ��
  //

  DirectX::XMVECTOR GetPositionXM() const;
  DirectX::XMFLOAT3 GetPosition() const;

  //��Դ������Ϣ
  virtual LightType type() const = 0;
  //��Ч��Դ�ı�
  virtual void Update();

 protected:
  // ��Դ�ı任
  Transform mTransform = {};
};

class DirectionLightSource : LightSource {
 public:
  DirectionLightSource();
  ~DirectionLightSource() = default;

  LightType type() const { return LightType::DirectionLightType; }

  DT::DirectionalLight& getLightData() const;

 private:
  DT::DirectionalLight mDirectionalLight;
};

class PointLightSource : LightSource {
 public:
  PointLightSource();
  ~PointLightSource() = default;

  LightType type() const { return LightType::PointLightType; }

  DT::PointLight& getLightData() const;

 private:
  DT::PointLight mPointLight;
};

class SpotLightSource : LightSource {
 public:
  SpotLightSource();
  ~SpotLightSource() = default;
  LightType type() const { return LightType::SpotLightType; }

  DT::SpotLight& getLightData() const;

 private:
  DT::SpotLight mSpotLight;
};
#endif