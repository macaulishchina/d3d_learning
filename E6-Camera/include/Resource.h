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
#define DIRECTIONAL_LIGHT_NUMS 10
#define POINT_LIGHT_NUMS 10
#define SPOT_LIGHT_NUMS 10
class ShaderManger;
class LightManger;
class CameraManger;
class ObjectManger;
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
        mShaderManager(std::make_shared<ShaderManger>()),
        mLightManger(std::make_shared<LightManger>()),
        mCameraManger(std::make_shared<CameraManger>()),
        mObjectManger(std::make_shared<ObjectManger>()) {
    Init();
  }

  ~ResourceController() = default;

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

 private:
  /*
    必要的资源初始化操作
  */
  void Init();

  D3DApp* mApp;

  std::shared_ptr<ShaderManger> mShaderManager;
  std::shared_ptr<LightManger> mLightManger;
  std::shared_ptr<CameraManger> mCameraManger;
  std::shared_ptr<ObjectManger> mObjectManger;

  template <class T>
  using ComPtr = Microsoft::WRL::ComPtr<T>;
  ComPtr<ID3D11Buffer> mConstBuffers[CONST_BUFFER_TOTAL_NUMS];  // 常量缓冲区
};

/*
着色器管理器，主要完成着色器的编译和加载
*/
class ShaderManger {
 public:
  ShaderManger() = default;
  ~ShaderManger() = default;

  /*
  编译并加载着色器
  */
  bool Init();

 private:
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
  LightManger() = default;
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
  bool DeleteDirectionalLight(const std::string& name);

 private:
  bool mDirLightChanged;
  bool mPointLightChanged;
  bool mSpotLightChanged;
  std::map<std::string, UINT> mDirectionLightName2Index;
  std::map<std::string, UINT> mPointLightName2Index;
  std::map<std::string, UINT> mSpotLightName2Index;
  std::map<std::string, std::shared_ptr<DirectionLightSource>> mDirLightSources;
  std::map<std::string, std::shared_ptr<PointLightSource>> mPointLightSources;
  std::map<std::string, std::shared_ptr<SpotLightSource>> mSpotLightSources;

  // 平行光源相关量
  UINT mDirLightNum;
  DT::CBDirLight mDirLightBuffers[DIRECTIONAL_LIGHT_NUMS] = {};
  // 点光源相关量
  UINT mPointLightNum;
  DT::CBPointLight mPointLightBuffers[POINT_LIGHT_NUMS] = {};
  // 聚光灯源相关量
  UINT mSpotLightNum;
  DT::CBSpotLight mSpotLightBuffers[SPOT_LIGHT_NUMS] = {};
};

/*
摄像机管理器，负责相机的添加、切换等
*/
class CameraManger {
 public:
  CameraManger() = default;
  ~CameraManger() = default;

  /*
    当视野相关举证可能发生变化时，更新相关常量缓冲区
  */
  void Update();

  //添加或重置相机
  void SetCamera(const std::string& name,
                 const std::shared_ptr<Camera>& camera);

  bool UseCamera(const std::string& name);

  bool DeleteCmera(const std::string& name);

 private:
  bool mCameraChanged;
  DT::CBCamera mCameraBuffer = {};  // 视角相关量
};

/*
场景物品管理器，负责模型的添加、变动等
*/
class ObjectManger {
 public:
  ObjectManger() = default;
  ~ObjectManger() = default;


  void Update();

  //添加或重置模型
  void SetGameObject(const std::string& name, const GameObject& object);


  void SetMaterialAmbient(const DirectX::XMFLOAT4& ambient);
  void SetMaterialDiffuse(const DirectX::XMFLOAT4& diffuse);
  void SetMaterialSpecular(const DirectX::XMFLOAT4& specular);
  void SetMaterialReflect(const DirectX::XMFLOAT4& reflect);

  const DirectX::XMFLOAT4& GetMaterialAmbient() const;
  const DirectX::XMFLOAT4& GetMaterialDiffuse() const;
  const DirectX::XMFLOAT4& GetMaterialSpecular() const;
  const DirectX::XMFLOAT4& GetMaterialReflect() const;

 private:
  // 状态标志位
  bool mModelChanged;
  bool mMaterialChanged;
  DT::CBModelLocation mModelBuffer = {};  // 模型位置相关量
};

#endif