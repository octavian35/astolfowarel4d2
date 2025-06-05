#pragma once
#include <Windows.h>
#include "../../Util/Math/Vector/Vector.h"
#include "../../Util/Math/Vector/Vector2D.h"
#include "Fonts.h"
#include "../../SDK/SDK.h"
#pragma warning(disable : 4244)
namespace Draw
{
	inline void Line(int x0, int y0, int x1, int y1, Color col)
	{
		I::MatSystemSurface->DrawSetColor(col);
		I::MatSystemSurface->DrawLine(x0, y0, x1, y1);
	}

	inline void Line(Vector2D start_pos, Vector2D end_pos, Color col)
	{
		Line(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void Circle(Vector2D position, float points, float radius, Color color)
	{
		float step = (float)M_PI * 2.0f / points;

		for (float a = 0; a < (M_PI * 2.0f); a += step)
		{
			Vector2D start(radius * cosf(a) + position.x, radius * sinf(a) + position.y);
			Vector2D end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y);
			Line(start, end, color);
		}
	}

	inline void Circle(int x, int y, float points, float radius, Color color)
	{
		Circle(Vector2D(x, y), points, radius, color);
	}

	inline void FilledRectangle(int x0, int y0, int x1, int y1, Color col)
	{
		I::MatSystemSurface->DrawSetColor(col);
		I::MatSystemSurface->DrawFilledRect(x0, y0, x1, y1);
	}

	inline void FilledRectangle(Vector2D start_pos, Vector2D end_pos, Color col)
	{
		FilledRectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void Rectangle(int x0, int y0, int x1, int y1, Color col)
	{
		I::MatSystemSurface->DrawSetColor(col);
		I::MatSystemSurface->DrawOutlinedRect(x0, y0, x1, y1);
	}

	inline void Rectangle(Vector2D start_pos, Vector2D end_pos, Color col)
	{
		Rectangle(start_pos.x, start_pos.y, end_pos.x, end_pos.y, col);
	}

	inline void FilledGradientRectangle(int x0, int y0, int x1, int y1, Color top, Color bottom)
	{
		I::MatSystemSurface->DrawSetColor(top);
		I::MatSystemSurface->DrawFilledRectFade(x0, y0, x1, y1, 255, 255, false);

		I::MatSystemSurface->DrawSetColor(bottom);
		I::MatSystemSurface->DrawFilledRectFade(x0, y0, x1, y1, 0, 255, false);
	}

	inline void FilledGradientRectangle(Vector2D pos, Vector2D size, Color top, Color bottom)
	{
		FilledGradientRectangle(pos.x, pos.y, pos.x + size.x, pos.y + size.y, top, bottom);
	}

	inline void Text(int x, int y, const wchar_t* text, HFont font, Color color, bool centeredx, bool centeredy)
	{
		if (centeredx || centeredy)
		{
			int wide, tall;
			I::MatSystemSurface->GetTextSize(font, text, wide, tall);
			if (centeredx)
				x -= wide / 2;
			if (centeredy)
				y -= tall / 2;
		}

		I::MatSystemSurface->DrawSetTextPos(x, y);
		I::MatSystemSurface->DrawSetTextFont(font);
		I::MatSystemSurface->DrawSetTextColor(color);
		I::MatSystemSurface->DrawPrintText(text, wcslen(text));
	}

	inline void Text(int x, int y, const wchar_t* text, HFont font, Color color, bool centered = false)
	{
		Text(x, y, text, font, color, centered, centered);
	}

	inline Vector2D GetTextSize(const wchar_t* text, HFont font)
	{
		int x_res, y_res;
		I::MatSystemSurface->GetTextSize(font, text, x_res, y_res);
		return Vector2D(x_res, y_res);
	}
}