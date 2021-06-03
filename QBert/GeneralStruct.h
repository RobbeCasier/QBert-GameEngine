#pragma once

namespace Shape
{
	struct Rect
	{
		float x;
		float y;
		float w;
		float h;

		Rect operator/(const float& divide)
		{
			Rect tempRect = *this;
			tempRect.w /= divide;
			tempRect.h /= divide;
			tempRect.x += tempRect.w / 2;
			tempRect.y -= tempRect.h / 2;
			return tempRect;
		}
		Rect& operator/=(const float& divide)
		{
			*this = *this / divide;
			return *this;
		}
	};
}