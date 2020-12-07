#include "primitives/cube-primitive.h"
#include "primitives/primitives.h"
#include "math/interval.h"

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;

namespace
{
	class CubeImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{

    private:
        std::vector<Primitive> m_squares;

	public:

        CubeImplementation()
        {
            auto xysq = xy_square();
            auto front = translate(Vector3D(0, 0, -1), xysq);
            auto back = translate(Vector3D(0, 0, 1), xysq);

            auto xzsq = xz_square();
            auto bottom = translate(Vector3D(0, -1, 0), xzsq);
            auto top = translate(Vector3D(0, 1, 0), xzsq);

            auto yzsq = yz_square();
            auto left = translate(Vector3D(-1, 0, 0), yzsq);
            auto right = translate(Vector3D(1, 0, 0), yzsq);

            m_squares.push_back(front);
            m_squares.push_back(back);
            m_squares.push_back(bottom);
            m_squares.push_back(top);
            m_squares.push_back(left);
            m_squares.push_back(right);

        }

		std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
		{
			std::vector<std::shared_ptr<Hit>> hits;
            for(auto sq : m_squares)
            {
                auto tmp = sq->find_all_hits(ray);
                hits.insert(hits.end(), tmp.begin(), tmp.end());
            }
			return hits;
		}

		Box bounding_box() const override
		{
			return Box(interval(-1.0, 1.0), interval(-1.0, 1.0), interval(-1.0, 1.0));
		}
	};
}

Primitive raytracer::primitives::cube()
{
	return Primitive(std::make_shared<CubeImplementation>());
}
