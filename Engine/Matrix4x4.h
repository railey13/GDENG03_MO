#pragma once
#include <memory>
#include "Vector3D.h"
#include "Vector4D.h"

class Matrix4x4 {
public:
	Matrix4x4() {

	}

	void setIdentity() {
		::memset(m_mat, 0, sizeof(float) * 16);

		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1; 
	}

	void setTranslation(const Vector3D& translation) {
	/*	setIdentity();*/

		m_mat[3][0] = translation.m_x;
		m_mat[3][1] = translation.m_y;
		m_mat[3][2] = translation.m_z;

	}

	void setScale(const Vector3D& scale) {
		/*setIdentity();*/

		m_mat[0][0] = scale.m_x;
		m_mat[1][1] = scale.m_y;
		m_mat[2][2] = scale.m_z;

	}

	void setRotationX(f32 x) {
		setIdentity();

		m_mat[1][1] = cosf(x);
		m_mat[1][2] = sinf(x);
		m_mat[2][1] = -sinf(x);
		m_mat[2][2] = cosf(x);
	}

	void setRotationY(f32 y) {
		setIdentity();

		m_mat[0][0] = cosf(y);
		m_mat[0][2] = -sinf(y);
		m_mat[2][0] = sinf(y);
		m_mat[2][2] = cosf(y);
	}

	void setRotationZ(f32 z) {
		setIdentity();

		m_mat[0][0] = cosf(z);
		m_mat[0][1] = sinf(z);
		m_mat[1][0] = -sinf(z);
		m_mat[1][1] = cosf(z);
	}

	f32 getDeterminant() {
		Vector4D minor, v1, v2, v3;
		f32 det;

		v1 = Vector4D(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = Vector4D(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = Vector4D(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);

		minor.cross(v1, v2, v3);
		det =
			-(this->m_mat[0][3] * minor.m_x +
				this->m_mat[1][3] * minor.m_y +
				this->m_mat[2][3] * minor.m_z +
				this->m_mat[3][3] * minor.m_w);

		return det;
	}

	void inverse() {
		Matrix4x4 out;
		Vector4D v, vec[3];

		auto det = getDeterminant();

		if (!det) return;

		for (auto i = 0; i < 4; i++)
		{
			for (auto j = 0; j < 4; j++)
			{
				if (j != i)
				{
					auto a = j;
					if (j > i) a = a - 1;
					vec[a].m_x = (this->m_mat[j][0]);
					vec[a].m_y = (this->m_mat[j][1]);
					vec[a].m_z = (this->m_mat[j][2]);
					vec[a].m_w = (this->m_mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = pow(-1.0f, i) * v.m_x / det;
			out.m_mat[1][i] = pow(-1.0f, i) * v.m_y / det;
			out.m_mat[2][i] = pow(-1.0f, i) * v.m_z / det;
			out.m_mat[3][i] = pow(-1.0f, i) * v.m_w / det;
		}

		this->setMatrix(out);
	}

	void operator*=(const Matrix4x4 &matrix) {
		Matrix4x4 out;

		for (i32 i = 0; i < 4; i++) {
			for (i32 j = 0; j < 4; j++) {
				out.m_mat[i][j] =
					m_mat[i][0] * matrix.m_mat[0][j] + m_mat[i][1] * matrix.m_mat[1][j] +
					m_mat[i][2] * matrix.m_mat[2][j] + m_mat[i][3] * matrix.m_mat[3][j];
			}
		}

		::memcpy(m_mat, out.m_mat, sizeof(float)*16);
	}

	void setMatrix(const Matrix4x4& matrix) {
		::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
	}

	Vector3D getXDirection() const {
		return Vector3D(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}

	Vector3D getYDirection() const {
		return Vector3D(m_mat[1][0], m_mat[1][1], m_mat[1][2]);
	}
	
	Vector3D getZDirection() const {
		return Vector3D(m_mat[2][0],m_mat[2][1],m_mat[2][2]);
	}	

	Vector3D getTranslation() const {
		return Vector3D(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	void setPerspectiveFovLH(f32 fov, f32 aspect, f32 znear, f32 zfar) {

		f32 yscale = 1.0f / tanf(fov/2.0f);
		f32 xscale = yscale / aspect;

		m_mat[0][0] = xscale;
		m_mat[1][1] = yscale;
		m_mat[2][2] = zfar / (zfar - znear);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = (-znear * zfar) / (zfar - znear);
	}

	void setOrthoLH(f32 width, f32 height, f32 near_plane, f32 far_plane) {
		setIdentity();

		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (far_plane - near_plane);
		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
	}

	~Matrix4x4() {	

	
	}

private:
	f32 m_mat[4][4] = {};
};