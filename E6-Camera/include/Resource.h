#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Camera.h"
#include "DT.h"
#include "GameObject.h"
#include "Light.h"
#include "d3dApp.h"

#define CONST_BUFFER_TOTAL_NUMS 6
#define CONST_BUFFER_MODEL_INDEX 0
#define CONST_BUFFER_CAMERA_INDEX 1
#define CONST_BUFFER_MATERIAL_INDEX 2
#define CONST_BUFFER_DIRLIGHT_INDEX 3
#define CONST_BUFFER_POINTLIGHT_INDEX 4
#define CONST_BUFFER_SPOTLIGHT_INDEX 5


/*
着色器管理器，主要完成着色器的编译和加载
*/
class ShaderManger {
public:
    ShaderManger(D3DApp* app);
    ~ShaderManger() = default;

    /*
    编译并加载着色器
    */
    bool Init();

private:

    D3DApp* mApp;
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
    ComPtr<ID3D11InputLayout> mVertexLayout3D;   // 顶点输入布局
    ComPtr<ID3D11VertexShader> mVertexShader3D;  // 3D顶点输入布局
    ComPtr<ID3D11PixelShader> mPixelShader3D;    // 3D像素着色器
};

/*
光源管理器，负责多类型多数量光源的创建、更新、删除等
*/
class LightManger {
public:
    LightManger(D3DApp* app);
    ~LightManger() = default;

    /*
    当光源数据改变时，修改相应的常量缓存区
    */
    void Update();

    //添加或重置平行光源
    bool SetDirectionalLight(const std::string& name,
        const std::shared_ptr<DirectionLightSource>& light);
    //添加或重置点光源
    bool SetPointLight(const std::string& name,
        const std::shared_ptr<PointLightSource>& light);
    //添加或重置聚光光源
    bool SetSpotLight(const std::string& name,
        const std::shared_ptr<SpotLightSource>& light);

    //删除平行光源
    bool DeleteDirectionalLight(const std::string& name);
    //删除点光源
    bool DeletePointLight(const std::string& name);
    //删除聚光光源
    bool DeleteSpotLight(const std::string& name);

private:
    void Init();

    D3DApp* mApp;

    bool mDirLightChanged;
    bool mPointLightChanged;
    bool mSpotLightChanged;
    std::map<std::string, UINT> mDirectionLightName2Index;
    std::map<std::string, UINT> mPointLightName2Index;
    std::map<std::string, UINT> mSpotLightName2Index;

    // 平行光源相关量
    DT::CBDirLight mDirLightBuffers = {};
    // 点光源相关量
    DT::CBPointLight mPointLightBuffers = {};
    // 聚光灯源相关量
    DT::CBSpotLight mSpotLightBuffers = {};

    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
    ComPtr<ID3D11Buffer> mConstBuffers[3];      // 常量缓冲区

};

/*
摄像机管理器，负责相机的添加、切换等
*/
class CameraManger {
public:
    CameraManger(D3DApp* app);
    ~CameraManger() = default;
    void Init();

    /*
      当视野相关矩阵可能发生变化时，更新相关常量缓冲区
    */
    void Update();

    //添加或重置相机
    void SetCamera(const std::string& name,
        const std::shared_ptr<Camera>& camera);

    bool UseCamera(const std::string& name);

    bool DeleteCmera(const std::string& name);

private:

    D3DApp* mApp;

    std::string mUsing;
    bool mCameraChanged;
    DT::CBCamera mCameraBuffer = {};  // 视角相关量
    std::map<std::string, std::shared_ptr<Camera>> mCameras;

    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
    ComPtr<ID3D11Buffer> mConstBuffers[1];      // 常量缓冲区

};

/*
场景物品管理器，负责模型的添加、变动等
*/
class ObjectManger {
public:
    ObjectManger(D3DApp* app);
    ~ObjectManger() = default;

    /*
        更新每一个模型
    */
    void Update();

    //添加或重置模型
    void SetGameObject(const std::string& name, const std::shared_ptr<GameObject>& object);

    bool SetVisable(const std::string& name, bool flag);

    bool DeleteGameObject(const std::string& name);

private:
    void Init();

    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
    ComPtr<ID3D11Buffer> mConstBuffers[1];        // 常量缓冲区

    D3DApp* mApp;
    std::map<std::string, std::shared_ptr<GameObject>> mObjects;
};

/*
资源控制器
1. 着色器资源设置
2. 光源资源管理
3. 相机资源管理
4. 模型资源管理
*/
class ResourceController {
public:
    ResourceController(D3DApp* app)
        : mApp(app),
        mShaderManager(std::make_shared<ShaderManger>(app)),
        mLightManger(std::make_shared<LightManger>(app)),
        mCameraManger(std::make_shared<CameraManger>(app)),
        mObjectManger(std::make_shared<ObjectManger>(app)) {
    }

    ~ResourceController() = default;

    /*
      必要的资源初始化操作
    */
    bool Init();

    /*
      更新资源
    */
    void Update() {
        mLightManger->Update();
        mCameraManger->Update();
        mObjectManger->Update();
    }
    inline std::shared_ptr<LightManger> GetLightManger() const {
        return mLightManger;
    }
    inline std::shared_ptr<CameraManger> GetCameraManger() const {
        return mCameraManger;
    }
    inline std::shared_ptr<ObjectManger> GetObjectManger() const {
        return mObjectManger;
    }

//private:

    D3DApp* mApp;
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
    ComPtr<ID3D11SamplerState> mSamplerState;		        // 采样器状态
    ComPtr<ID3D11ShaderResourceView> mWoodCrate;            // 木盒纹理

    std::shared_ptr<ShaderManger> mShaderManager;
    std::shared_ptr<LightManger> mLightManger;
    std::shared_ptr<CameraManger> mCameraManger;
    std::shared_ptr<ObjectManger> mObjectManger;
};

#endif