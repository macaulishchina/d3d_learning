#include "Camera.h"
#include "Transform.h"

using namespace DirectX;

Camera::~Camera() {
}

XMVECTOR Camera::GetPositionXM() const {
	return mTransform.GetPositionXM();
}

XMFLOAT3 Camera::GetPosition() const {
	return mTransform.GetPosition();
}

float Camera::GetRotationX() const {
	return mTransform.GetRotation().x;
}

float Camera::GetRotationY() const {
	return mTransform.GetRotation().y;
}


XMVECTOR Camera::GetRightAxisXM() const {
	return mTransform.GetRightAxisXM();
}

XMFLOAT3 Camera::GetRightAxis() const {
	return mTransform.GetRightAxis();
}

XMVECTOR Camera::GetUpAxisXM() const {
	return mTransform.GetUpAxisXM();
}

XMFLOAT3 Camera::GetUpAxis() const {
	return mTransform.GetUpAxis();
}

XMVECTOR Camera::GetLookAxisXM() const {
	return mTransform.GetForwardAxisXM();
}

XMFLOAT3 Camera::GetLookAxis() const {
	return mTransform.GetForwardAxis();
}

XMMATRIX Camera::GetViewXM() const {
	return mTransform.GetWorldToLocalMatrixXM();
}

XMMATRIX Camera::GetProjXM() const {
	return XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
}

XMMATRIX Camera::GetViewProjXM() const {
	return GetViewXM() * GetProjXM();
}

D3D11_VIEWPORT Camera::GetViewPort() const {
	return mViewPort;
}

void Camera::SetFrustum(float fovY, float aspect, float nearZ, float farZ) {
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = nearZ;
	mFarZ = farZ;
}

void Camera::SetViewPort(const D3D11_VIEWPORT& viewPort) {
	mViewPort = viewPort;
}

void Camera::SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth) {
	mViewPort.TopLeftX = topLeftX;
	mViewPort.TopLeftY = topLeftY;
	mViewPort.Width = width;
	mViewPort.Height = height;
	mViewPort.MinDepth = minDepth;
	mViewPort.MaxDepth = maxDepth;
}


// ******************
// 第一人称/自由视角摄像机
//

FirstPersonCamera::~FirstPersonCamera() {
}

void FirstPersonCamera::SetPosition(float x, float y, float z) {
	SetPosition(XMFLOAT3(x, y, z));
}

void FirstPersonCamera::SetPosition(const XMFLOAT3& pos) {
	mTransform.SetPosition(pos);
}

void FirstPersonCamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up) {
	mTransform.SetPosition(pos);
	mTransform.LookAt(target, up);
}

void FirstPersonCamera::LookTo(const XMFLOAT3& pos, const XMFLOAT3& to, const XMFLOAT3& up) {
	mTransform.SetPosition(pos);
	mTransform.LookTo(to, up);
}

void FirstPersonCamera::Strafe(float d) {
	mTransform.Translate(mTransform.GetRightAxis(), d);
}

void FirstPersonCamera::Walk(float d) {
	XMVECTOR rightVec = mTransform.GetRightAxisXM();
	XMVECTOR frontVec = XMVector3Normalize(XMVector3Cross(rightVec, g_XMIdentityR1));
	XMFLOAT3 front;
	XMStoreFloat3(&front, frontVec);
	mTransform.Translate(front, d);
}

void FirstPersonCamera::MoveForward(float d) {
	mTransform.Translate(mTransform.GetForwardAxis(), d);
}

void FirstPersonCamera::Pitch(float rad) {
	XMFLOAT3 rotation = mTransform.GetRotation();
	// 将绕x轴旋转弧度限制在[-7pi/18, 7pi/18]之间
	rotation.x += rad;
	if (rotation.x > XM_PI * 7 / 18)
		rotation.x = XM_PI * 7 / 18;
	else if (rotation.x < -XM_PI * 7 / 18)
		rotation.x = -XM_PI * 7 / 18;

	mTransform.SetRotation(rotation);
}

void FirstPersonCamera::RotateY(float rad) {
	XMFLOAT3 rotation = mTransform.GetRotation();
	rotation.y = XMScalarModAngle(rotation.y + rad);
	mTransform.SetRotation(rotation);
}



// ******************
// 第三人称摄像机
//

ThirdPersonCamera::~ThirdPersonCamera() {
}

XMFLOAT3 ThirdPersonCamera::GetTargetPosition() const {
	return m_Target;
}

float ThirdPersonCamera::GetDistance() const {
	return m_Distance;
}

void ThirdPersonCamera::RotateX(float rad) {
	XMFLOAT3 rotation = mTransform.GetRotation();
	// 将绕x轴旋转弧度限制在[0, pi/3]之间
	rotation.x += rad;
	if (rotation.x < 0.0f)
		rotation.x = 0.0f;
	else if (rotation.x > XM_PI / 3)
		rotation.x = XM_PI / 3;

	mTransform.SetRotation(rotation);
	mTransform.SetPosition(m_Target);
	mTransform.Translate(mTransform.GetForwardAxis(), -m_Distance);
}

void ThirdPersonCamera::RotateY(float rad) {
	XMFLOAT3 rotation = mTransform.GetRotation();
	rotation.y = XMScalarModAngle(rotation.y + rad);

	mTransform.SetRotation(rotation);
	mTransform.SetPosition(m_Target);
	mTransform.Translate(mTransform.GetForwardAxis(), -m_Distance);
}

void ThirdPersonCamera::Approach(float dist) {
	m_Distance += dist;
	// 限制距离在[m_MinDist, m_MaxDist]之间
	if (m_Distance < m_MinDist)
		m_Distance = m_MinDist;
	else if (m_Distance > m_MaxDist)
		m_Distance = m_MaxDist;

	mTransform.SetPosition(m_Target);
	mTransform.Translate(mTransform.GetForwardAxis(), -m_Distance);
}

void ThirdPersonCamera::SetRotationX(float rad) {
	XMFLOAT3 rotation = mTransform.GetRotation();
	// 将绕x轴旋转弧度限制在[0, pi/3]之间
	rotation.x = rad;
	if (rotation.x < 0.0f)
		rotation.x = 0.0f;
	else if (rotation.x > XM_PI / 3)
		rotation.x = XM_PI / 3;

	mTransform.SetRotation(rotation);
	mTransform.SetPosition(m_Target);
	mTransform.Translate(mTransform.GetForwardAxis(), -m_Distance);
}

void ThirdPersonCamera::SetRotationY(float rad) {
	XMFLOAT3 rotation = mTransform.GetRotation();
	rotation.y = XMScalarModAngle(rad);
	mTransform.SetRotation(rotation);
	mTransform.SetPosition(m_Target);
	mTransform.Translate(mTransform.GetForwardAxis(), -m_Distance);
}

void ThirdPersonCamera::SetTarget(const XMFLOAT3& target) {
	m_Target = target;
}

void ThirdPersonCamera::SetDistance(float dist) {
	m_Distance = dist;
}

void ThirdPersonCamera::SetDistanceMinMax(float minDist, float maxDist) {
	m_MinDist = minDist;
	m_MaxDist = maxDist;
}


Transform::Transform(const DirectX::XMFLOAT3& scale, const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& position)
	: mScale(scale), mRotation(rotation), mPosition(position) {
}

XMFLOAT3 Transform::GetScale() const {
	return mScale;
}

DirectX::XMVECTOR Transform::GetScaleXM() const {
	return XMLoadFloat3(&mScale);
}

XMFLOAT3 Transform::GetRotation() const {
	return mRotation;
}

DirectX::XMVECTOR Transform::GetRotationXM() const {
	return XMLoadFloat3(&mRotation);
}

XMFLOAT3 Transform::GetPosition() const {
	return mPosition;
}

DirectX::XMVECTOR Transform::GetPositionXM() const {
	return XMLoadFloat3(&mPosition);
}

XMFLOAT3 Transform::GetRightAxis() const {
	XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation));
	XMFLOAT3 right;
	XMStoreFloat3(&right, R.r[0]);
	return right;
}

DirectX::XMVECTOR Transform::GetRightAxisXM() const {
	XMFLOAT3 right = GetRightAxis();
	return XMLoadFloat3(&right);
}

XMFLOAT3 Transform::GetUpAxis() const {
	XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation));
	XMFLOAT3 up;
	XMStoreFloat3(&up, R.r[1]);
	return up;
}

DirectX::XMVECTOR Transform::GetUpAxisXM() const {
	XMFLOAT3 up = GetUpAxis();
	return XMLoadFloat3(&up);
}

XMFLOAT3 Transform::GetForwardAxis() const {
	XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&mRotation));
	XMFLOAT3 forward;
	XMStoreFloat3(&forward, R.r[2]);
	return forward;
}

DirectX::XMVECTOR Transform::GetForwardAxisXM() const {
	XMFLOAT3 forward = GetForwardAxis();
	return XMLoadFloat3(&forward);
}

XMFLOAT4X4 Transform::GetLocalToWorldMatrix() const {
	XMFLOAT4X4 res;
	XMStoreFloat4x4(&res, GetLocalToWorldMatrixXM());
	return res;
}

XMMATRIX Transform::GetLocalToWorldMatrixXM() const {
	XMVECTOR scaleVec = XMLoadFloat3(&mScale);
	XMVECTOR rotationVec = XMLoadFloat3(&mRotation);
	XMVECTOR positionVec = XMLoadFloat3(&mPosition);
	XMMATRIX World = XMMatrixScalingFromVector(scaleVec) * XMMatrixRotationRollPitchYawFromVector(rotationVec) * XMMatrixTranslationFromVector(positionVec);
	return World;
}

XMFLOAT4X4 Transform::GetWorldToLocalMatrix() const {
	XMFLOAT4X4 res;
	XMStoreFloat4x4(&res, GetWorldToLocalMatrixXM());
	return res;
}

XMMATRIX Transform::GetWorldToLocalMatrixXM() const {
	XMMATRIX InvWorld = XMMatrixInverse(nullptr, GetLocalToWorldMatrixXM());
	return InvWorld;
}

void Transform::SetScale(const XMFLOAT3& scale) {
	mScale = scale;
}

void Transform::SetScale(float x, float y, float z) {
	mScale = XMFLOAT3(x, y, z);
}

void Transform::SetRotation(const XMFLOAT3& eulerAnglesInRadian) {
	mRotation = eulerAnglesInRadian;
}

void Transform::SetRotation(float x, float y, float z) {
	mRotation = XMFLOAT3(x, y, z);
}

void Transform::SetPosition(const XMFLOAT3& position) {
	mPosition = position;
}

void Transform::SetPosition(float x, float y, float z) {
	mPosition = XMFLOAT3(x, y, z);
}

void Transform::Rotate(const XMFLOAT3& eulerAnglesInRadian) {
	XMVECTOR newRotationVec = XMVectorAdd(XMLoadFloat3(&mRotation), XMLoadFloat3(&eulerAnglesInRadian));
	XMStoreFloat3(&mRotation, newRotationVec);
}

void Transform::RotateAxis(const XMFLOAT3& axis, float radian) {
	XMVECTOR rotationVec = XMLoadFloat3(&mRotation);
	XMMATRIX R = XMMatrixRotationRollPitchYawFromVector(rotationVec) *
		XMMatrixRotationAxis(XMLoadFloat3(&axis), radian);
	XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, R);
	mRotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}

void Transform::RotateAround(const XMFLOAT3& point, const XMFLOAT3& axis, float radian) {
	XMVECTOR rotationVec = XMLoadFloat3(&mRotation);
	XMVECTOR positionVec = XMLoadFloat3(&mPosition);
	XMVECTOR centerVec = XMLoadFloat3(&point);

	// 以point作为原点进行旋转
	XMMATRIX RT = XMMatrixRotationRollPitchYawFromVector(rotationVec) * XMMatrixTranslationFromVector(positionVec - centerVec);
	RT *= XMMatrixRotationAxis(XMLoadFloat3(&axis), radian);
	RT *= XMMatrixTranslationFromVector(centerVec);
	XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, RT);
	mRotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
	XMStoreFloat3(&mPosition, RT.r[3]);
}

void Transform::Translate(const XMFLOAT3& direction, float magnitude) {
	XMVECTOR directionVec = XMVector3Normalize(XMLoadFloat3(&direction));
	XMVECTOR newPosition = XMVectorMultiplyAdd(XMVectorReplicate(magnitude), directionVec, XMLoadFloat3(&mPosition));
	XMStoreFloat3(&mPosition, newPosition);
}

void Transform::LookAt(const XMFLOAT3& target, const XMFLOAT3& up) {
	XMMATRIX View = XMMatrixLookAtLH(XMLoadFloat3(&mPosition), XMLoadFloat3(&target), XMLoadFloat3(&up));
	XMMATRIX InvView = XMMatrixInverse(nullptr, View);
	XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, InvView);
	mRotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}

void Transform::LookTo(const XMFLOAT3& direction, const XMFLOAT3& up) {
	XMMATRIX View = XMMatrixLookToLH(XMLoadFloat3(&mPosition), XMLoadFloat3(&direction), XMLoadFloat3(&up));
	XMMATRIX InvView = XMMatrixInverse(nullptr, View);
	XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, InvView);
	mRotation = GetEulerAnglesFromRotationMatrix(rotMatrix);
}

XMFLOAT3 Transform::GetEulerAnglesFromRotationMatrix(const XMFLOAT4X4& rotationMatrix) {
	// 通过旋转矩阵反求欧拉角
	float c = sqrtf(1.0f - rotationMatrix(2, 1) * rotationMatrix(2, 1));
	// 防止r[2][1]出现大于1的情况
	if (isnan(c))
		c = 0.0f;
	XMFLOAT3 rotation;
	rotation.z = atan2f(rotationMatrix(0, 1), rotationMatrix(1, 1));
	rotation.x = atan2f(-rotationMatrix(2, 1), c);
	rotation.y = atan2f(rotationMatrix(2, 0), rotationMatrix(2, 2));
	return rotation;
}
