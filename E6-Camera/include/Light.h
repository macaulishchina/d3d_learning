#ifndef __Light_H__
#define __Light_H__
#include <DirectXMath.h>
#include <d3d11_1.h>

#include "DT.h"
#include "Transform.h"

enum class LightType { DirectionLightType, PointLightType, SpotLightType };

class LightSource {
public:
    LightSource() = default;
    ~LightSource() = default;

    //
    // 获取光源位置
    //

    inline DirectX::XMVECTOR GetPositionXM() const {
        return mTransform.GetPositionXM();
    }
    inline DirectX::XMFLOAT3 GetPosition() const {
        return mTransform.GetPosition();
    }

    //光源类型信息
    virtual LightType type() const = 0;


protected:
    // 光源的变换
    Transform mTransform = {};
};

class DirectionLightSource : LightSource {
public:
    DirectionLightSource() = default;
    ~DirectionLightSource() = default;

    LightType type() const { return LightType::DirectionLightType; }

    inline const DT::DirectionalLight& getLightData() const {
        return mDirectionalLight;
    }

//private:
    DT::DirectionalLight mDirectionalLight;
};

class PointLightSource : LightSource {
public:
    PointLightSource() = default;
    ~PointLightSource() = default;

    LightType type() const { return LightType::PointLightType; }

    inline const DT::PointLight& getLightData() const {
        return mPointLight;
    }

//private:
    DT::PointLight mPointLight;
};

class SpotLightSource : LightSource {
public:
    SpotLightSource() = default;
    ~SpotLightSource() = default;
    LightType type() const { return LightType::SpotLightType; }

    inline const DT::SpotLight& getLightData() const {
        return mSpotLight;
    }

//private:
    DT::SpotLight mSpotLight;
};
#endif