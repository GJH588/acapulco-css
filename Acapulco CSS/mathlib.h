/*
TODO: Finish this
*/
#pragma once
#include <math.h>
#include "Vector.h"
struct matrix3x4_t
{
public:

	matrix3x4_t() {}

	matrix3x4_t(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	void Pad();
	void Pad2();

	void Init
		(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);


	void Init(void*);

	inline float* operator[](int i)
	{
		return m[i];
	}

	inline const float* operator[](int i) const
	{
		return m[i];
	}


	inline float *Base()
	{
		return &m[0][0];
	}

	inline const float *Base() const
	{
		return &m[0][0];
	}

	inline Vector matrix3x4_t::operator*(const Vector &vVec) const
	{
		Vector vRet;
		vRet.x = m[0][0] * vVec.x + m[0][1] * vVec.y + m[0][2] * vVec.z + m[0][3];
		vRet.y = m[1][0] * vVec.x + m[1][1] * vVec.y + m[1][2] * vVec.z + m[1][3];
		vRet.z = m[2][0] * vVec.x + m[2][1] * vVec.y + m[2][2] * vVec.z + m[2][3];

		return vRet;
	}

	matrix3x4_t& matrix3x4_t::operator=(const matrix3x4_t &mOther)
	{
		m[0][0] = mOther.m[0][0];
		m[0][1] = mOther.m[0][1];
		m[0][2] = mOther.m[0][2];
		m[0][3] = mOther.m[0][3];

		m[1][0] = mOther.m[1][0];
		m[1][1] = mOther.m[1][1];
		m[1][2] = mOther.m[1][2];
		m[1][3] = mOther.m[1][3];

		m[2][0] = mOther.m[2][0];
		m[2][1] = mOther.m[2][1];
		m[2][2] = mOther.m[2][2];
		m[2][3] = mOther.m[2][3];

		m[3][0] = mOther.m[3][0];
		m[3][1] = mOther.m[3][1];
		m[3][2] = mOther.m[3][2];
		m[3][3] = mOther.m[3][3];

		return *this;
	}

	bool matrix3x4_t::operator==(const matrix3x4_t& src) const
	{
		return !memcmp(src.m, m, sizeof(m));
	}

	void matrix3x4_t::MatrixMul(const matrix3x4_t &vm, matrix3x4_t &out) const
	{
		out.Init(
			m[0][0] * vm.m[0][0] + m[0][1] * vm.m[1][0] + m[0][2] * vm.m[2][0] + m[0][3] * vm.m[3][0],
			m[0][0] * vm.m[0][1] + m[0][1] * vm.m[1][1] + m[0][2] * vm.m[2][1] + m[0][3] * vm.m[3][1],
			m[0][0] * vm.m[0][2] + m[0][1] * vm.m[1][2] + m[0][2] * vm.m[2][2] + m[0][3] * vm.m[3][2],
			m[0][0] * vm.m[0][3] + m[0][1] * vm.m[1][3] + m[0][2] * vm.m[2][3] + m[0][3] * vm.m[3][3],

			m[1][0] * vm.m[0][0] + m[1][1] * vm.m[1][0] + m[1][2] * vm.m[2][0] + m[1][3] * vm.m[3][0],
			m[1][0] * vm.m[0][1] + m[1][1] * vm.m[1][1] + m[1][2] * vm.m[2][1] + m[1][3] * vm.m[3][1],
			m[1][0] * vm.m[0][2] + m[1][1] * vm.m[1][2] + m[1][2] * vm.m[2][2] + m[1][3] * vm.m[3][2],
			m[1][0] * vm.m[0][3] + m[1][1] * vm.m[1][3] + m[1][2] * vm.m[2][3] + m[1][3] * vm.m[3][3],

			m[2][0] * vm.m[0][0] + m[2][1] * vm.m[1][0] + m[2][2] * vm.m[2][0] + m[2][3] * vm.m[3][0],
			m[2][0] * vm.m[0][1] + m[2][1] * vm.m[1][1] + m[2][2] * vm.m[2][1] + m[2][3] * vm.m[3][1],
			m[2][0] * vm.m[0][2] + m[2][1] * vm.m[1][2] + m[2][2] * vm.m[2][2] + m[2][3] * vm.m[3][2],
			m[2][0] * vm.m[0][3] + m[2][1] * vm.m[1][3] + m[2][2] * vm.m[2][3] + m[2][3] * vm.m[3][3],

			m[3][0] * vm.m[0][0] + m[3][1] * vm.m[1][0] + m[3][2] * vm.m[2][0] + m[3][3] * vm.m[3][0],
			m[3][0] * vm.m[0][1] + m[3][1] * vm.m[1][1] + m[3][2] * vm.m[2][1] + m[3][3] * vm.m[3][1],
			m[3][0] * vm.m[0][2] + m[3][1] * vm.m[1][2] + m[3][2] * vm.m[2][2] + m[3][3] * vm.m[3][2],
			m[3][0] * vm.m[0][3] + m[3][1] * vm.m[1][3] + m[3][2] * vm.m[2][3] + m[3][3] * vm.m[3][3]
			);
	}

	matrix3x4_t matrix3x4_t::operator*(const matrix3x4_t &vm) const
	{
		matrix3x4_t ret;
		MatrixMul(vm, ret);
		return ret;
	}

	inline matrix3x4_t matrix3x4_t::operator+(const matrix3x4_t &other) const
	{
		matrix3x4_t ret;
		for (int i = 0; i < 16; i++)
		{
			((float*)ret.m)[i] = ((float*)m)[i] + ((float*)other.m)[i];
		}
		return ret;
	}

public:
	float m[4][4];
};
class Quaternion
{
public:
	float x, y, z, w;
	float operator[](int i) const { if (i == 1) return x; if (i == 2) return y; if (y == 3) return z; return w; };
	float& operator[](int i) { if (i == 1) return x; if (i == 2) return y; if (y == 3) return z; return w; };
};

class RadianEuler
{
public:
	float x, y, z;
};
typedef Vector QAngle;
#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif
#define  Assert( _exp ) ((void)0)
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#ifndef RAD2DEG
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#endif

#ifndef DEG2RAD
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#endif
enum
{
	PITCH = 0,	// up / down
	YAW,		// left / right
	ROLL		// fall over
};

// Math routines done in optimized assembly math package routines
void inline SinCos(float radians, float *sine, float *cosine)
{
	*sine = sin(radians);
	*cosine = cos(radians);

}
void AngleVectors(const QAngle &angles, Vector *forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
	SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);

	forward->x = cp*cy;
	forward->y = cp*sy;
	forward->z = -sp;
}
void AngleVectors(const QAngle &angles, Vector *forward, Vector *right, Vector *up)
{
	Assert(s_bMathlibInitialized);

	float sr, sp, sy, cr, cp, cy;

#ifdef _X360
	fltx4 radians, scale, sine, cosine;
	radians = LoadUnaligned3SIMD(angles.Base());
	scale = ReplicateX4(M_PI_F / 180.f);
	radians = MulSIMD(radians, scale);
	SinCos3SIMD(sine, cosine, radians);
	sp = SubFloat(sine, 0);	sy = SubFloat(sine, 1);	sr = SubFloat(sine, 2);
	cp = SubFloat(cosine, 0);	cy = SubFloat(cosine, 1);	cr = SubFloat(cosine, 2);
#else
	SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
	SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
	SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);
#endif

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}
void AngleVectorsTranspose(const QAngle &angles, Vector *forward, Vector *right, Vector *up)
{
	Assert(s_bMathlibInitialized);
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
	SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
	SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = (sr*sp*cy + cr*-sy);
		forward->z = (cr*sp*cy + -sr*-sy);
	}

	if (right)
	{
		right->x = cp*sy;
		right->y = (sr*sp*sy + cr*cy);
		right->z = (cr*sp*sy + -sr*cy);
	}

	if (up)
	{
		up->x = -sp;
		up->y = sr*cp;
		up->z = cr*cp;
	}
}

void VectorAngles(const Vector& forward, QAngle &angles)
{
	Assert(s_bMathlibInitialized);
	float	tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}
void CrossProduct(const float* v1, const float* v2, float* cross)
{
	Assert(s_bMathlibInitialized);
	Assert(v1 != cross);
	Assert(v2 != cross);
	cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
	cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
	cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}
inline void _SSE_RSqrtInline(float a, float* out)
{
	__m128  xx = _mm_load_ss(&a);
	__m128  xr = _mm_rsqrt_ss(xx);
	__m128  xt;
	xt = _mm_mul_ss(xr, xr);
	xt = _mm_mul_ss(xt, xx);
	xt = _mm_sub_ss(_mm_set_ss(3.f), xt);
	xt = _mm_mul_ss(xt, _mm_set_ss(0.5f));
	xr = _mm_mul_ss(xr, xt);
	_mm_store_ss(out, xr);
}
FORCEINLINE float VectorNormalize(Vector& vec)
{
#ifndef DEBUG // stop crashing my edit-and-continue!
#if defined(__i386__) || defined(_M_IX86)
#define DO_SSE_OPTIMIZATION
#endif
#endif

#if defined( DO_SSE_OPTIMIZATION )
	float sqrlen = vec.LengthSqr() + 1.0e-10f, invlen;
	_SSE_RSqrtInline(sqrlen, &invlen);
	vec.x *= invlen;
	vec.y *= invlen;
	vec.z *= invlen;
	return sqrlen * invlen;
#else
	extern float (FASTCALL *pfVectorNormalize)(Vector& v);
	return (*pfVectorNormalize)(vec);
#endif
}
void MatrixGetColumn(const matrix3x4_t& in, int column, Vector &out)
{
	out.x = in[0][column];
	out.y = in[1][column];
	out.z = in[2][column];
}

void MatrixSetColumn(const Vector &in, int column, matrix3x4_t& out)
{
	out[0][column] = in.x;
	out[1][column] = in.y;
	out[2][column] = in.z;
}
void SetIdentityMatrix(matrix3x4_t& matrix)
{
	memset(matrix.Base(), 0, sizeof(float)* 3 * 4);
	matrix[0][0] = 1.0;
	matrix[1][1] = 1.0;
	matrix[2][2] = 1.0;
}
void SetScaleMatrix(float x, float y, float z, matrix3x4_t &dst);
void MatrixBuildRotationAboutAxis(const Vector &vAxisOfRot, float angleDegrees, matrix3x4_t &dst);
void VectorTransform(class Vector const &, struct matrix3x4_t const &, class Vector &);

float QuaternionNormalize(Quaternion &q)
{
	Assert(s_bMathlibInitialized);
	float radius, iradius;

	Assert(q.IsValid());

	radius = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];

	if (radius) // > FLT_EPSILON && ((radius < 1.0f - 4*FLT_EPSILON) || (radius > 1.0f + 4*FLT_EPSILON))
	{
		radius = sqrt(radius);
		iradius = 1.0f / radius;
		q[3] *= iradius;
		q[2] *= iradius;
		q[1] *= iradius;
		q[0] *= iradius;
	}
	return radius;
}
float DotProduct(const float *v1, const float *v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
float DotProduct(Vector v1, const float* v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
void VectorTransform(Vector& in1, const matrix3x4_t& in2, Vector& out)
{
	Assert(s_bMathlibInitialized);
	Assert(in1 != out);
	out[0] = DotProduct(in1, in2[0]) + in2[0][3];
	out[1] = DotProduct(in1, in2[1]) + in2[1][3];
	out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}
void MatrixAngles(const matrix3x4_t &matrix, Quaternion &q, Vector &pos)
{
#ifdef _VPROF_MATHLIB
	VPROF_BUDGET("MatrixQuaternion", "Mathlib");
#endif
	float trace;
	trace = matrix[0][0] + matrix[1][1] + matrix[2][2] + 1.0f;
	if (trace > 1.0f + FLT_EPSILON)
	{
		// VPROF_INCREMENT_COUNTER("MatrixQuaternion A",1);
		q.x = (matrix[2][1] - matrix[1][2]);
		q.y = (matrix[0][2] - matrix[2][0]);
		q.z = (matrix[1][0] - matrix[0][1]);
		q.w = trace;
	}
	else if (matrix[0][0] > matrix[1][1] && matrix[0][0] > matrix[2][2])
	{
		// VPROF_INCREMENT_COUNTER("MatrixQuaternion B",1);
		trace = 1.0f + matrix[0][0] - matrix[1][1] - matrix[2][2];
		q.x = trace;
		q.y = (matrix[1][0] + matrix[0][1]);
		q.z = (matrix[0][2] + matrix[2][0]);
		q.w = (matrix[2][1] - matrix[1][2]);
	}
	else if (matrix[1][1] > matrix[2][2])
	{
		// VPROF_INCREMENT_COUNTER("MatrixQuaternion C",1);
		trace = 1.0f + matrix[1][1] - matrix[0][0] - matrix[2][2];
		q.x = (matrix[0][1] + matrix[1][0]);
		q.y = trace;
		q.z = (matrix[2][1] + matrix[1][2]);
		q.w = (matrix[0][2] - matrix[2][0]);
	}
	else
	{
		// VPROF_INCREMENT_COUNTER("MatrixQuaternion D",1);
		trace = 1.0f + matrix[2][2] - matrix[0][0] - matrix[1][1];
		q.x = (matrix[0][2] + matrix[2][0]);
		q.y = (matrix[2][1] + matrix[1][2]);
		q.z = trace;
		q.w = (matrix[1][0] - matrix[0][1]);
	}

	QuaternionNormalize(q);

#if 0
	// check against the angle version
	RadianEuler ang;
	MatrixAngles(matrix, ang);
	Quaternion test;
	AngleQuaternion(ang, test);
	float d = QuaternionDotProduct(q, test);
	Assert(fabs(d) > 0.99 && fabs(d) < 1.01);
#endif

	MatrixGetColumn(matrix, 3, pos);
}