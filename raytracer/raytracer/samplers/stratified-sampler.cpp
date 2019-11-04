#include "samplers/stratified-sampler.h"
#include "math/rasterizer.h"

using namespace math;
using namespace raytracer;


namespace
{
    class StratifiedSampler : public raytracer::samplers::_private_::SamplerImplementation
    {
    public:
		StratifiedSampler(int n, int m)
		{
			m_N = n;
			m_M = m;
		}

		void sample(const math::Rectangle2D& rectangle, std::function<void(const math::Point2D&)> function) const override
		{
			auto raster = Rasterizer(rectangle, m_N, m_M);
			for (int n = 0; n < m_N; n++) {
				for (int m = 0; m < m_M; m++)
				{
					auto p = Position2D(n, m);
					auto center = raster[p].center();
					function(center);
				}
			}
		}
	private:
		int m_N;
		int m_M;
    };
}

Sampler raytracer::samplers::stratified(int n, int m)
{
    return Sampler(std::make_shared<StratifiedSampler>(n, m));
}