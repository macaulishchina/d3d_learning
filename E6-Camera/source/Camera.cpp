#include "Camera.h"
#include "Transform.h"

using namespace DirectX;

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
	return mTarget;
}

float ThirdPersonCamera::GetDistance() const {
	return mDistance;
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
	mTransform.SetPosition(mTarget);
	mTransform.Translate(mTransform.GetForwardAxis(), -mDistance);
}

void ThirdPersonCamera::RotateY(float rad) {
	XMFLOAT3 rotation = mTransform.GetRotation();
	rotation.y = XMScalarModAngle(rotation.y + rad);

	mTransform.SetRotation(rotation);
	mTransform.SetPosition(mTarget);
	mTransform.Translate(mTransform.GetForwardAxis(), -mDistance);
}

void ThirdPersonCamera::Approach(float dist) {
	mDistance += dist;
	// 限制距离在[m_MinDist, m_MaxDist]之间
	if (mDistance < mMinDist)
		mDistance = mMinDist;
	else if (mDistance > mMaxDist)
		mDistance = mMaxDist;

	mTransform.SetPosition(mTarget);
	mTransform.Translate(mTransform.GetForwardAxis(), -mDistance);
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
	mTransform.SetPosition(mTarget);
	mTransform.Translate(mTransform.GetForwardAxis(), -mDistance);
}

void ThirdPersonCamera::SetRotationY(float rad) {
	XMFLOAT3 rotation = mTransform.GetRotation();
	rotation.y = XMScalarModAngle(rad);
	mTransform.SetRotation(rotation);
	mTransform.SetPosition(mTarget);
	mTransform.Translate(mTransform.GetForwardAxis(), -mDistance);
}

void ThirdPersonCamera::SetTarget(const XMFLOAT3& target) {
	mTarget = target;
}

void ThirdPersonCamera::SetDistance(float dist) {
	mDistance = dist;
}

void ThirdPersonCamera::SetDistanceMinMax(float minDist, float maxDist) {
	mMinDist = minDist;
	mMaxDist = maxDist;
}
