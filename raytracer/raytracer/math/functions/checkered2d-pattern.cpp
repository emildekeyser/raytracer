#include "math/functions/checkered2d-pattern.h"
#include "math/functions.h"
#include <cmath>

using namespace math;


Function<bool(const Point2D&)> math::functions::checkered2d(double thickness)
{
    std::function<bool(const Point2D&)> function = [thickness](const Point2D& p)
    {
        auto y = p.y();
        auto x = p.x();

        bool bottom_left =  std::abs(y - round(y)) < thickness / 2 && std::abs(x - round(x)) < thickness / 2;
        bool top_right =  std::abs(y - round(y)) > thickness / 2 && std::abs(x - round(x)) > thickness / 2;
		return bottom_left || top_right;
    };

    return from_lambda<bool, const Point2D&>(function);
}
