#pragma once

#include "primitives/primitive.h"
#include "math/point.h"

using namespace math;

namespace raytracer
{
    namespace primitives
    {
        Primitive mesh(const std::string& filename);
    }
}
