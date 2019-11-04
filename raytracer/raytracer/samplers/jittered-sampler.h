#pragma once

#include "samplers/sampler.h"


namespace raytracer
{
    namespace samplers
    {
        /// <summary>
        /// Creates a sampler that picks the random points inside of NxM squares
        /// </summary>
        Sampler jittered(int, int);
    }
}
