#pragma once
#include "GeneralStruct.h"

namespace Utility
{
	inline bool IsOverlappingRectangle(const Shape::Rect& rect1, const Shape::Rect& rect2)
	{
		if (((rect2.x > rect1.x && rect2.x < rect1.x + rect1.w)
			|| (rect2.x + rect2.w > rect1.x && rect2.x + rect2.w < rect1.x + rect1.w))
			&&
			((rect2.y > rect1.y && rect2.y < rect1.y + rect1.h)
				|| (rect2.y + rect2.h > rect1.y && rect2.y + rect2.h < rect1.y + rect1.h))
			)
			return true;
		return false;
	}
}