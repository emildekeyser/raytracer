#include "animations.h"
#include "animation/animation.h"

template<typename T>
Animation<math::Point3D> animation::circular(const math::Point3D & point, const math::Point3D& center, 
	const math::Vector3D& rotation_axis, const math::Interval<math::Angle>& angle_interval, const Duration & duration)
{
	auto double_animation = basic(0, 1, duration);

	// Creates a local function which takes a TimeStamp and returns a T.
	// The local function requires access to variables in the surrounding scope, 
	// in this case, double_animation and animation_interval.
	// These variables must be listed between [].
	std::function<T(TimeStamp)> lambda = [double_animation, angle_interval](TimeStamp now) -> T {
		// Get the current value for t.
		// t == 0 at the beginning of the animation and grows linearly to 1.
		double t = double_animation(now);

		// Sanity check
		assert(math::interval(0.0, 1.0).contains(t));

		// Map [0, 1] to target interval. Say animation_interval equals [3, 9],
		// then 0 -> 3, 0.5 -> 6, 1 -> 9, etc.
		return angle_interval.from_relative(double_animation(now));
	};

	// Turns the function into an Animation object
	return make_animation(math::from_lambda(lambda), duration);
}
