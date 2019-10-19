#pragma once

#include "animation.h"
#include "math/interval.h"


namespace animation
{
	template<typename T>
	Animation<T> circular(const math::Point3D& point, const math::Point3D& center,
		const math::Vector3D& rotation_axis, const math::Interval<math::Angle>& angle_interval, const Duration& duration);
	
}