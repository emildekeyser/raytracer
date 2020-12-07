#pragma once

#include "primitives/primitive.h"
#include "math/point.h"

using namespace math;

namespace raytracer
{
    namespace primitives
    {
        Primitive bounding_box_accelerator(const Primitive& primitive);
    }
}
