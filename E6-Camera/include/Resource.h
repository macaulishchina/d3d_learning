#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "DT.h"
#include "Camera.h"
#include "d3dApp.h"
#include "GameObject.h"

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

    bool InitEffect();

    void InitBuffer();

    void SetMaterialAmbient(const DirectX::XMFLOAT4& ambient);
    void SetMaterialDiffuse(const DirectX::XMFLOAT4& diffuse);
    void SetMaterialSpecular(const DirectX::XMFLOAT4& specular);
    void SetMaterialReflect(const DirectX::XMFLOAT4& reflect);

    DirectX::XMFLOAT4 GetMaterialAmbient() const;
    DirectX::XMFLOAT4 GetMaterialDiffuse() const;
    DirectX::XMFLOAT4 GetMaterialSpecular() const;
    DirectX::XMFLOAT4 GetMaterialReflect() const;

    //添加或重置平行光源
    bool SetDirectionalLight(const std::string& name, const DT::DirectionalLight& light);
    //添加或重置点光源
    bool SetPointLight(const std::string& name, const DT::DirectionalLight& light);
    //添加或重置聚光光源
    bool SetSpotLight(const std::string& name, const DT::DirectionalLight& light);

    //包含平行光源
    bool ContainDirectionalLight(const std::string& name) const;
    //包含点光源
    bool ContainPointLight(const std::string& name) const;
    //包含聚光光源
    bool ContainSpotLight(const std::string& name) const;

    //获得平行光源信息副本，请确保光源存在
    const DT::DirectionalLight& GetDirectionalLight(const std::string& name) const;
    //获得点光源信息副本，请确保光源存在
    const DT::DirectionalLight& GetPointLight(const std::string& name) const;
    //获得聚光光源信息副本，请确保光源存在
    const DT::DirectionalLight& GetSpotLight(const std::string& name) const;

    //删除平行光源
    void DeleteDirectionalLight(const std::string& name);
    //删除点光源
    void DeletePointLight(const std::string& name);
    //删除聚光光源
    void DeleteDirectionalLight(const std::string& name);


    //添加或重置相机
    void SetCamera(const std::string& name, const std::shared_ptr<Camera>& camera);
    //获得相机对象指针
    std::shared_ptr<Camera> GetCamera(const std::string& name) const;
    //判断是否存在该相机
    bool ContainCamera(const std::string& name) const;
    //删除相机
    bool DeleteCamera(const std::string& name);
    //使用相机
    bool UseCamera(const std::string& name);

    //添加或重置模型
    void SetGameObject(const std::string& name, const GameObject& object);
    // 获取指定物体变换
    Transform& GetTransform(const std::string& name);
    // 获取指定物体变换
    const Transform& GetTransform(const std::string& name) const;

    //inline DirectX::XMMATRIX GetView() const;
    //inline DirectX::XMMATRIX GetProj() const;
    //inline DirectX::XMFLOAT3 GetEyePosition() const;


    //void addOrUpdateLight(const std::string& name, const DT::AbstractLight& light);
    //DT::AbstractLight getLight(std::string name) const;

    void Update();

private:

    //void SetView(const DirectX::XMMATRIX& view);
    //void SetProj(const DirectX::XMMATRIX& proj);
    //void SetEyePosition(const DirectX::XMFLOAT3& position);

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
    ComPtr<ID3D11InputLayout> mVertexLayout3D;              // 顶点输入布局
    ComPtr<ID3D11VertexShader> mVertexShader3D;             // 3D顶点输入布局
    ComPtr<ID3D11PixelShader> mPixelShader3D;               // 3D像素着色器

    DT::CBModelLocation mModelBuffer = {};	                // 模型位置相关量
    DT::CBCamera mCameraBuffer = {};	                    // 视角相关量
    DT::CBMaterial mMaterialBuffer = {};	                // 模型材质相关量
    //mutable std::map<std::string, DT::AbstractLight> mLights;   //光源容器
    DT::CBDirLight mDirLightBuffer = {};	                    // 平行光源相关量
    DT::CBPointLight mPointLightBuffer = {};	                // 点光源相关量
    DT::CBSpotLight mSpotLightBuffer = {};	                // 聚光灯源相关量

    //std::shared_ptr<FirstPersonCamera> mCamera;
};

//class BaseResource
//{
//public:
//    BaseResource();
//    ~BaseResource();
//
//private:
//
//};
//
//class CameraResource :BaseResource {
//public:
//    CameraResource();
//    ~CameraResource();
//
//private:
//
//};
//
//class LightResource :BaseResource {
//public:
//    LightResource();
//    ~LightResource();
//
//private:
//
//};
//
//class ObjectResource :BaseResource {
//public:
//    ObjectResource();
//    ~ObjectResource();
//
//private:
//
//};

#endif