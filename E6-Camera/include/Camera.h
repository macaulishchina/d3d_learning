#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <DirectXMath.h>
#include <d3d11_1.h>
#include <DirectXMath.h>

namespace CameraInternal {
    class Transform {
    public:
        Transform() = default;
        Transform(const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& position);
        ~Transform() = default;

        Transform(const Transform&) = default;
        Transform& operator=(const Transform&) = default;

        Transform(Transform&&) = default;
        Transform& operator=(Transform&&) = default;

        // 获取对象缩放比例
        DirectX::XMFLOAT3 GetScale() const;
        // 获取对象缩放比例
        DirectX::XMVECTOR GetScaleXM() const;

        // 获取对象欧拉角(弧度制)
        // 对象以Z-X-Y轴顺序旋转
        DirectX::XMFLOAT3 GetRotation() const;
        // 获取对象欧拉角(弧度制)
        // 对象以Z-X-Y轴顺序旋转
        DirectX::XMVECTOR GetRotationXM() const;

        // 获取对象位置
        DirectX::XMFLOAT3 GetPosition() const;
        // 获取对象位置
        DirectX::XMVECTOR GetPositionXM() const;

        // 获取右方向轴
        DirectX::XMFLOAT3 GetRightAxis() const;
        // 获取右方向轴
        DirectX::XMVECTOR GetRightAxisXM() const;

        // 获取上方向轴
        DirectX::XMFLOAT3 GetUpAxis() const;
        // 获取上方向轴
        DirectX::XMVECTOR GetUpAxisXM() const;

        // 获取前方向轴
        DirectX::XMFLOAT3 GetForwardAxis() const;
        // 获取前方向轴
        DirectX::XMVECTOR GetForwardAxisXM() const;

        // 获取世界变换矩阵
        DirectX::XMFLOAT4X4 GetLocalToWorldMatrix() const;
        // 获取世界变换矩阵
        DirectX::XMMATRIX GetLocalToWorldMatrixXM() const;

        // 获取逆世界变换矩阵
        DirectX::XMFLOAT4X4 GetWorldToLocalMatrix() const;
        // 获取逆世界变换矩阵
        DirectX::XMMATRIX GetWorldToLocalMatrixXM() const;

        // 设置对象缩放比例
        void SetScale(const DirectX::XMFLOAT3& scale);
        // 设置对象缩放比例
        void SetScale(float x, float y, float z);

        // 设置对象欧拉角(弧度制)
        // 对象将以Z-X-Y轴顺序旋转
        void SetRotation(const DirectX::XMFLOAT3& eulerAnglesInRadian);
        // 设置对象欧拉角(弧度制)
        // 对象将以Z-X-Y轴顺序旋转
        void SetRotation(float x, float y, float z);

        // 设置对象位置
        void SetPosition(const DirectX::XMFLOAT3& position);
        // 设置对象位置
        void SetPosition(float x, float y, float z);

        // 指定欧拉角旋转对象
        void Rotate(const DirectX::XMFLOAT3& eulerAnglesInRadian);
        // 指定以原点为中心绕轴旋转
        void RotateAxis(const DirectX::XMFLOAT3& axis, float radian);
        // 指定以point为旋转中心绕轴旋转
        void RotateAround(const DirectX::XMFLOAT3& point, const DirectX::XMFLOAT3& axis, float radian);

        // 沿着某一方向平移
        void Translate(const DirectX::XMFLOAT3& direction, float magnitude);

        // 观察某一点
        void LookAt(const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });
        // 沿着某一方向观察
        void LookTo(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& up = { 0.0f, 1.0f, 0.0f });

    private:
        // 从旋转矩阵获取旋转欧拉角
        DirectX::XMFLOAT3 GetEulerAnglesFromRotationMatrix(const DirectX::XMFLOAT4X4& rotationMatrix);

    private:
        DirectX::XMFLOAT3 mScale = { 1.0f, 1.0f, 1.0f };				// 缩放
        DirectX::XMFLOAT3 mRotation = {};								// 旋转欧拉角(弧度制)
        DirectX::XMFLOAT3 mPosition = {};								// 位置
    };

}

using namespace CameraInternal;

class Camera {
public:
    Camera() = delete;
    virtual ~Camera() = 0;

    //
    // 获取摄像机位置
    //

    DirectX::XMVECTOR GetPositionXM() const;
    DirectX::XMFLOAT3 GetPosition() const;

    //
    // 获取摄像机旋转
    //

    // 获取绕X轴旋转的欧拉角弧度
    float GetRotationX() const;
    // 获取绕Y轴旋转的欧拉角弧度
    float GetRotationY() const;

    //
    // 获取摄像机的坐标轴向量
    //

    DirectX::XMVECTOR GetRightAxisXM() const;
    DirectX::XMFLOAT3 GetRightAxis() const;
    DirectX::XMVECTOR GetUpAxisXM() const;
    DirectX::XMFLOAT3 GetUpAxis() const;
    DirectX::XMVECTOR GetLookAxisXM() const;
    DirectX::XMFLOAT3 GetLookAxis() const;

    //
    // 获取矩阵
    //

    DirectX::XMMATRIX GetViewXM() const;
    DirectX::XMMATRIX GetProjXM() const;
    DirectX::XMMATRIX GetViewProjXM() const;

    // 获取视口
    D3D11_VIEWPORT GetViewPort() const;


    // 设置视锥体
    void SetFrustum(float fovY, float aspect, float nearZ, float farZ);

    // 设置视口
    void SetViewPort(const D3D11_VIEWPORT& viewPort);
    void SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);

protected:

    // 摄像机的变换
    Transform mTransform = {};

    // 视锥体属性
    float mNearZ = 0.0f;
    float mFarZ = 0.0f;
    float mAspect = 0.0f;
    float mFovY = 0.0f;

    // 当前视口
    D3D11_VIEWPORT mViewPort = {};

};

class FirstPersonCamera : public Camera {
public:
    FirstPersonCamera() = default;
    ~FirstPersonCamera() override;

    // 设置摄像机位置
    void SetPosition(float x, float y, float z);
    void SetPosition(const DirectX::XMFLOAT3& pos);
    // 设置摄像机的朝向
    void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);
    void LookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up);
    // 平移
    void Strafe(float d);
    // 直行(平面移动)
    void Walk(float d);
    // 前进(朝前向移动)
    void MoveForward(float d);
    // 上下观察
    // 正rad值向上观察
    // 负rad值向下观察
    void Pitch(float rad);
    // 左右观察
    // 正rad值向右观察
    // 负rad值向左观察
    void RotateY(float rad);
};

class ThirdPersonCamera : public Camera {
public:
    ThirdPersonCamera() = default;
    ~ThirdPersonCamera() override;

    // 获取当前跟踪物体的位置
    DirectX::XMFLOAT3 GetTargetPosition() const;
    // 获取与物体的距离
    float GetDistance() const;
    // 绕物体垂直旋转(注意绕x轴旋转欧拉角弧度限制在[0, pi/3])
    void RotateX(float rad);
    // 绕物体水平旋转
    void RotateY(float rad);
    // 拉近物体
    void Approach(float dist);
    // 设置初始绕X轴的弧度(注意绕x轴旋转欧拉角弧度限制在[0, pi/3])
    void SetRotationX(float rad);
    // 设置初始绕Y轴的弧度
    void SetRotationY(float rad);
    // 设置并绑定待跟踪物体的位置
    void SetTarget(const DirectX::XMFLOAT3& target);
    // 设置初始距离
    void SetDistance(float dist);
    // 设置最小最大允许距离
    void SetDistanceMinMax(float minDist, float maxDist);

private:
    DirectX::XMFLOAT3 m_Target = {};
    float m_Distance = 0.0f;
    // 最小允许距离，最大允许距离
    float m_MinDist = 0.0f, m_MaxDist = 0.0f;
};


#endif