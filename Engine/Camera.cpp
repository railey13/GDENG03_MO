#include "Camera.h"

Camera::Camera() {
	InputSystem::get()->addListener(this);
	m_position = Vector3D(-0.3f, 0.5f, -1);
}

void Camera::update(f32 deltaTime) {

	Matrix4x4 world_cam = getRotationMatrix();

	Vector3D new_pos = m_position
		+ world_cam.getZDirection() * (m_forward * deltaTime)
		+ world_cam.getXDirection() * (m_strafe * deltaTime)
		+world_cam.getYDirection() * (m_up * deltaTime);

	world_cam.setTranslation(new_pos);
	m_position = new_pos;

	world_cam.inverse();

	m_view = world_cam;
}

Camera::~Camera() {
	InputSystem::get()->removeListener(this);
}

void Camera::onKeyDown(i32 key) {
	if (m_moveLock) return;
	switch (key) {
		case 'W': m_forward = 1;
			break;
		case 'A': m_strafe = -1;
			break;
		case 'S': m_forward = -1;
			break;
		case 'D': m_strafe = 1;
			break;
		case 'Q': m_up = -1;
			break;
		case 'E': m_up = 1;
	}
}

void Camera::onKeyUp(i32 key) {
	switch (key) {
		case 'W': m_forward = 0;
			break;
		case 'A': m_strafe = 0;
			break;
		case 'S': m_forward = 0;
			break;
		case 'D': m_strafe = 0;
			break;
		case 'Q': m_up = 0;
			break;
		case 'E': m_up = 0;
	}
}

void Camera::onMouseMove(const Point& mouse_pos) {
	if (m_moveLock) return;
	
	float dx = (mouse_pos.m_x - m_lastMousePos.m_x);
	float dy = (mouse_pos.m_y - m_lastMousePos.m_y);

	m_yaw += dx * m_sensitivity;
	m_pitch += dy * m_sensitivity;

	// clamp pitch
	const float maxPitch = 1.49f, minPitch = -1.49f;
	m_pitch = std::max(minPitch, std::min(maxPitch, m_pitch));
	m_lastMousePos = mouse_pos;
}

void Camera::onLeftMouseDown(const Point& mouse_pos) {

}

void Camera::onLeftMouseUp(const Point& mouse_pos) {

}

void Camera::onRightMouseDown(const Point& mouse_pos) {
	//InputSystem::get()->setCursorPosition(Point(Settings::WindowWidth / 2.0f, Settings::WindowHeight / 2.0f));
	InputSystem::get()->showCursor(false);
	m_lastMousePos = mouse_pos;
	m_moveLock = false;
}

void Camera::onRightMouseUp(const Point& mouse_pos) {
	InputSystem::get()->showCursor(true);
	m_moveLock = true;
}

Matrix4x4 Camera::getViewMatrix() const {
	return m_view;
}

Matrix4x4 Camera::getProjectionMatrix() const { 
	Matrix4x4 proj;
	proj.setPerspectiveFovLH(m_fov, m_aspect, m_znear, m_zfar);

	return proj;
}

Matrix4x4 Camera::getRotationMatrix() const {
	Matrix4x4 world_cam;
	world_cam.setIdentity();

	Matrix4x4 temp;

	temp.setRotationX(m_pitch);
	world_cam *= temp;

	temp.setRotationY(m_yaw);
	world_cam *= temp;

	return world_cam;
}

Vector3D Camera::getForwardDirection() const {
	return getRotationMatrix().getZDirection();
}

Vector3D Camera::getPosition() const {
	return m_position;
}

Vector3D Camera::getRotation() const
{
	return Vector3D(m_pitch,m_yaw,0);
}

void Camera::setAspect(f32 width, f32 height) {
	m_aspect = width / height;
}
