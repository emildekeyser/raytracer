#include "samplers/jittered-sampler.h"
#include "math/rasterizer.h"
#include <random>

using namespace math;
using namespace raytracer;


namespace
{
    class JitteredSampler : public raytracer::samplers::_private_::SamplerImplementation
    {
    public:
		JitteredSampler(int n, int m)
		{
			m_N = n;
			m_M = m;
		}

		void sample(const math::Rectangle2D& rectangle, std::function<void(const math::Point2D&)> function) const override
		{
			auto raster = Rasterizer(rectangle, m_N, m_M);
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0, 1.0);

			for (int n = 0; n < m_N; n++) {
				for (int m = 0; m < m_M; m++)
				{
					auto rect = raster[Position2D(n, m)];
					auto p = Point2D(dis(gen), dis(gen));
					function(rect.from_relative(p));
				}
			}
		}
	private:
		int m_N;
		int m_M;
    };
}

Sampler raytracer::samplers::jittered(int n, int m)
{
    return Sampler(std::make_shared<JitteredSampler>(n, m));
}