#pragma once

#include "samplers/sampler.h"


namespace raytracer
{
    namespace samplers
    {
        /// <summary>
        /// Creates a sampler that picks the centers of NxM squares
        /// </summary>
        Sampler stratified(int, int);
    }
}
