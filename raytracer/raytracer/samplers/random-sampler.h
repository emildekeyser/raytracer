#pragma once

#include "samplers/sampler.h"


namespace raytracer
{
    namespace samplers
    {
        /// <summary>
        /// Creates a sampler that picks n random points
        /// </summary>
        Sampler random(int);
    }
}
