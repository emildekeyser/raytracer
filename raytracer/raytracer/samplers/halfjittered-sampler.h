#pragma once

#include "samplers/sampler.h"


namespace raytracer
{
    namespace samplers
    {
        /// <summary>
        /// Creates a sampler that picks the random points inside of NxM squares, within a certain distance from the center.
        /// </summary>
        Sampler halfjittered(int, int);
    }
}
