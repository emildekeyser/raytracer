#pragma once

#include "raytracers/ray-tracer.h"
#include "ray-tracer-v4.h"
#include <memory>

namespace raytracer
{
	namespace raytracers
	{
		namespace _private_
		{
			class RayTracerV5 : public RayTracerV4
			{
			public:
				TraceResult trace(const Scene&, const math::Ray&) const override;
			protected:
				virtual TraceResult trace(const Scene&, const math::Ray&, const double weight) const;
				virtual imaging::Color compute_reflection(const Scene&, const MaterialProperties&, const Hit&, const math::Ray&, const double weight) const;
			};
		}

		RayTracer v5();
	}
}