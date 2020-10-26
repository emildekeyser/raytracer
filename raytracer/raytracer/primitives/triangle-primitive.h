#pragma once

#include "primitives/primitive.h"
#include "math/point.h"

using namespace math;

namespace raytracer
{
    namespace primitives
    {
        Primitive triangle(const Point3D&, const Point3D&, const Point3D&);
    }
}
