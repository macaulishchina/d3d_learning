#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "d3dApp.h"
#include "DT.h"
#define CONST_BUFFER_NUMS 6
#define DIRECTIONAL_LIGHT_NUMS 10
#define POINT_LIGHT_NUMS 10
#define SPOT_LIGHT_NUMS 10


/*
    资源控制器
*/
class ResourceController {
public:
    ResourceController(D3DApp* app);

    ~ResourceController() = default;

    void InitBuffer();

    void setModel(const DirectX::XMMATRIX& model);
    void setView(const DirectX::XMMATRIX& view);
    void setProj(const DirectX::XMMATRIX& proj);
    void setEyePosition(const DirectX::XMFLOAT3& position);

    inline DirectX::XMMATRIX getModel() const;
    inline DirectX::XMMATRIX getView() const;
    inline DirectX::XMMATRIX getProj() const;
    inline DirectX::XMFLOAT3 getEyePosition() const;

    void setMaterialAmbient(const DirectX::XMFLOAT4& ambient);
    void setMaterialDiffuse(const DirectX::XMFLOAT4& diffuse);
    void setMaterialSpecular(const DirectX::XMFLOAT4& specular);
    void setMaterialReflect(const DirectX::XMFLOAT4& reflect);

    inline DirectX::XMFLOAT4 getMaterialAmbient() const;
    inline DirectX::XMFLOAT4 getMaterialDiffuse() const;
    inline DirectX::XMFLOAT4 getMaterialSpecular() const;
    inline DirectX::XMFLOAT4 getMaterialReflect() const;

    //void addOrUpdateLight(const std::string& name, const DT::AbstractLight& light);

    void deleteLight(std::string name);

    //DT::AbstractLight getLight(std::string name) const;

    void update();

public:

    // 状态标志位
    bool mModelChanged;
    bool mCameraChanged;
    bool mMaterialChanged;
    bool mDirLightChanged;
    bool mPointLightChanged;
    bool mSpotLightChanged;

    D3DApp* mApp;
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
    ComPtr<ID3D11Buffer> mConstBuffers[CONST_BUFFER_NUMS];	// 常量缓冲区

    DT::CBModelLocation mModelBuffer = {};	                // 模型位置相关量
    DT::CBCamera mCameraBuffer = {};	                    // 视角相关量
    DT::CBMaterial mMaterialBuffer = {};	                // 模型材质相关量

    //mutable std::map<std::string, DT::AbstractLight> mLights;   //光源容器
    DT::CBDirLight mDirLightBuffer = {};	                    // 平行光源相关量
    DT::CBPointLight mPointLightBuffer = {};	                // 点光源相关量
    DT::CBSpotLight mSpotLightBuffer = {};	                // 聚光灯源相关量

};

#endif