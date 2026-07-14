#pragma once
#include <math.h>
#include "Prerequisites.h"

class Vector3D {
public:
	Vector3D() : m_x(0), m_y(0), m_z(0){
		
	}

	Vector3D(f32 x, f32 y, f32 z) : m_x(x), m_y(y), m_z(z) {

	}

	Vector3D(const Vector3D &vector) : m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z) {

	}

	static Vector3D lerp(const Vector3D& start, const Vector3D& end, f32 deltaTime) {
		Vector3D v;

		v.m_x = start.m_x * (1.0f - deltaTime) + end.m_x * (deltaTime);
		v.m_y = start.m_y * (1.0f - deltaTime) + end.m_y * (deltaTime);
		v.m_z = start.m_z * (1.0f - deltaTime) + end.m_z * (deltaTime);

		return v;
	}

	bool operator==(const Vector3D v) const {
		if (
			v.m_x == m_x &&
			v.m_y == m_y &&
			v.m_z == m_z
			)
			return true;

		return false;
	}

	Vector3D operator+(const Vector3D v)  const {
		return Vector3D(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
	}

	Vector3D operator-(const Vector3D v) const {
		return Vector3D(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
	}

	Vector3D operator/(const f32 v) const {
		return Vector3D(m_x / v, m_y / v, m_z / v);
	}

	Vector3D operator*(const f32 v) const {
		return Vector3D(m_x * v, m_y * v, m_z * v);
	}

	Vector3D& operator+=(const Vector3D v) {
		m_x += v.m_x;
		m_y += v.m_y;
		m_z += v.m_z;
		return *this;
	}

	Vector3D& operator-=(const Vector3D v) {
		m_x -= v.m_x;
		m_y -= v.m_y;
		m_z -= v.m_z;
		return *this;
	}

	Vector3D& operator/=(f32 v) {
		m_x /= v;
		m_y /= v;
		m_z /= v;
		return *this;
	}

	Vector3D& operator*=(f32 v) {
		m_x *= v;
		m_y *= v;
		m_z *= v;
		return *this;
	}

	bool operator==(const Vector3D& v) const{
		return (m_x == v.m_x && m_y == v.m_y && m_z == v.m_z);
	}
	
	f32 magnitude() const {
		return sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
	}

	Vector3D normalize() const {
		return (Vector3D(m_x, m_y, m_z) * (1 / magnitude()));
	}

	Vector3D componentProduct(const Vector3D v) const {
		return Vector3D(m_x * v.m_x,
			m_y * v.m_y,
			m_z * v.m_z);
	}

	f32 scalarProduct(const Vector3D v) const {
		return ((m_x * v.m_x) + (m_y * v.m_y) + (m_z * v.m_z));
	}

	Vector3D vectorProduct(const Vector3D v) const {
		return Vector3D((m_y * v.m_z) - (m_z * v.m_y),
			(m_z * v.m_x) - (m_x * v.m_z),
			(m_x * v.m_y) - (m_y * v.m_x));
	}

	~Vector3D() {

	}

public:
	f32 m_x, m_y, m_z;
};