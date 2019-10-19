#pragma once

#include "raytracers/ray-tracer.h"
#include "ray-tracer-v5.h"
#include <memory>

namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV6 : public RayTracerV5
			{
			protected:
				TraceResult trace(const Scene&, const math::Ray&, const double weight) const override;
				virtual imaging::Color compute_refraction(const Scene&, const MaterialProperties&, const Hit&, const math::Ray&, const double) const;
			};
		}

		RayTracer v6();
	}
}