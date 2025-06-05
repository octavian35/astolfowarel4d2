#pragma once

#include "Vector/Vector.h"
#include "Vector/Vector2D.h"
#include "../../SDK/L4D2/Includes/usercmd.h"
#include <cstdio>
#include <string>
#include <string.h>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <stringapiset.h>
#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif
#pragma warning(disable : 4305)
#pragma warning(disable : 4244)
using matrix3x4_t = float[3][4];
using VMatrix = float[4][4];
#define square( x ) ( x * x )
#define RADPI 57.295779513082f
#define SQUARE( a ) a*a
#define PI			3.14159265358979323846
class CUtil_Math
{
public:
	void VectorTransform(const Vector input, const matrix3x4_t& matrix, Vector& output);
	void BuildTransformedBox(Vector* v2, const Vector bbmin, const Vector bbmax, const matrix3x4_t& m);
	void PointsFromBox(const Vector mins, const Vector maxs, Vector* points);
	void VectorAngles(const Vector& forward, Vector& angles);
	void AngleVectors(const Vector vAngles, Vector* vForward);
	void ClampAngles(Vector& v);
	void RotateTriangle(Vector2D* v, const float flRotation);

	float GetFovBetween(const Vector vSrc, const Vector vDst);
	float NormalizeAngle(const float ang);

	Vector GetAngleToPosition(const Vector vFrom, const Vector vTo);
	inline float degToRad(const float deg)
	{
		return deg * (PI / 180.0f);
	}

	inline float radToDeg(const float rad)
	{
		return rad * (180.0f / PI);
	}
	inline void sinCos(const float radians, float* const sine, float* const cosine)
	{
		*sine = std::sinf(radians);
		*cosine = std::cosf(radians);
	}

	inline void angleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
	{
		float sr{};
		float sp{};
		float sy{};
		float cr{};
		float cp{};
		float cy{};

		sinCos(degToRad(angles.x), &sp, &cp);
		sinCos(degToRad(angles.y), &sy, &cy);
		sinCos(degToRad(angles.z), &sr, &cr);

		if (forward) {
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right) {
			right->x = -1.0f * sr * sp * cy + -1.0f * cr * -sy;
			right->y = -1.0f * sr * sp * sy + -1.0f * cr * cy;
			right->z = -1.0f * sr * cp;
		}

		if (up) {
			up->x = cr * sp * cy + -sr * -sy;
			up->y = cr * sp * sy + -sr * cy;
			up->z = cr * cp;
		}
	}
public:
	template<typename T>
	inline T Clamp(const T val, const T min, const T max) {
		const T t = (val < min) ? min : val;
		return (t > max) ? max : t;
	}

	template<typename T>
	inline T Min(const T a, const T b) {
		return ((a > b) * b) + ((a <= b) * a);
	}

	template<typename T>
	inline T Max(const T a, const T b) {
		return ((a > b) * a) + ((a <= b) * b);
	}

	//Not really math related at all.
	template<typename F, typename ... T>
	inline bool CompareGroup(F&& first, T&& ... t) {
		return ((first == t) || ...);
	}
	
	inline Vector CalcAngle(const Vector& source, const Vector& destination)
	{
		const Vector vDelta = (source - destination);
		const float flHyp = ::sqrtf((vDelta.x * vDelta.x) + (vDelta.y * vDelta.y));

		return { (::atanf(vDelta.z / flHyp) * M_RADPI), (::atanf(vDelta.y / vDelta.x) * M_RADPI) + (180.0f * (vDelta.x >= 0.0f)), 0.0f };
	}
	inline static float normalizeRad(float a) noexcept
	{
		return std::isfinite(a) ? std::remainder(a, PI * 2) : 0.0f;
	}
	inline static float angleDiffRad(float a1, float a2) noexcept
	{
		float delta;

		delta = normalizeRad(a1 - a2);
		if (a1 > a2)
		{
			if (delta >= PI)
				delta -= PI * 2;
		}
		else
		{
			if (delta <= -PI)
				delta += PI * 2;
		}
		return delta;
	}
	inline float GetDistanceMeters(const Vector& vec1, const Vector& vec2)
	{
		constexpr float HU_TO_METERS = 0.01905f; // Conversion factor
		return (vec1 - vec2).Lenght() * HU_TO_METERS;
	}
};

namespace U { inline CUtil_Math Math; }