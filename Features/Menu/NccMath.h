#pragma once

#include "../../Util/Math/Vector/Vector.h"
#include "../../Util/Math/Vector/Vector2D.h"
#include <math.h>
#include <stdlib.h>
#include <DirectXMath.h>
#include <Windows.h>
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
enum
{
	PITCH,
	YAW,
	ROLL
};

namespace Math
{
	//--------------------------------------------------------------------------------
	inline bool InBox(int rx1, int ry1, int rx2, int ry2, int px, int py)
	{
		return px > rx1 && px < rx2 && py > ry1 && py < ry2;
	}
	//--------------------------------------------------------------------------------
	inline bool InBox(Vector2D r1, Vector2D r2, int px, int py)
	{
		return InBox(r1.x, r1.y, r2.x, r2.y, px, py);
	}
	//--------------------------------------------------------------------------------
	inline float RandomFloat(float a, float b)
	{
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}
	//--------------------------------------------------------------------------------
	inline float DistancePointToLine(Vector Point, Vector LineOrigin, Vector Dir)
	{
		auto PointDir = Point - LineOrigin;

		auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
		if (TempOffset < 0.000001f)
			return FLT_MAX;

		auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

		return (Point - PerpendicularPoint).Lenght();
	}
	//--------------------------------------------------------------------------------
}