#pragma once
#include <math.h>
#include "Prerequisites.h"

class Vector2D {
public:
	Vector2D() : m_x(0), m_y(0) {

	}

	Vector2D(f32 x, f32 y) : m_x(x), m_y(y) {

	}

	Vector2D(const Vector2D& vector) : m_x(vector.m_x), m_y(vector.m_y) {

	}

	
	Vector2D operator+(const Vector2D v) const {
		return Vector2D(m_x + v.m_x, m_y + v.m_y);
	}

	Vector2D operator-(const Vector2D v) const {
		return Vector2D(m_x - v.m_x, m_y - v.m_y);
	}

	Vector2D operator/(const f32 v) const {
		return Vector2D(m_x / v, m_y / v);
	}

	Vector2D operator*(const f32 v) const {
		return Vector2D(m_x * v, m_y * v);
	}

	f32 magnitude()  const {
		return sqrt(m_x * m_x + m_y * m_y);
	}

	Vector2D normalize() const {
		return (Vector2D(m_x, m_y) * (1 / magnitude()));
	}

	Vector2D componentProduct(const Vector2D v) const {
		return Vector2D(m_x * v.m_x,
			m_y * v.m_y);
	}

	f32 scalarProduct(const Vector2D v) const {
		return ((m_x * v.m_x) + (m_y * v.m_y));
	}

	f32 vectorProduct(const Vector2D v) const {
		return (m_x * v.m_y) - (m_y * v.m_x);
	}

	~Vector2D() {

	}

public:
	f32 m_x, m_y;
};