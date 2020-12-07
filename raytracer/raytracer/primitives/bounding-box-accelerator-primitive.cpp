#include "primitives/bounding-box-accelerator-primitive.h"
#include "primitives/square-primitive.h"
#include "primitives/transformer-primitive.h"
#include "primitives/cube-primitive.h"
#include "math/interval.h"
#include "util/misc.h"

#include <cmath>

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;

namespace
{

	class BoundingBoxAcceleratorImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
    private:
        const Primitive m_primitive;
        const Box m_bounding_box;
        Primitive m_transformed_cube;

    public:

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="primitive">
		/// primitive
		/// </param>
		BoundingBoxAcceleratorImplementation(const Primitive& primitive)
			: m_primitive(primitive), m_bounding_box(primitive->bounding_box())
        {
            auto center_vector = m_bounding_box.center() - Point3D(0, 0, 0);
            auto translated = translate(center_vector, cube());
            auto x = m_bounding_box.x().size();
            auto y = m_bounding_box.y().size();
            auto z = m_bounding_box.z().size();
            m_transformed_cube = scale(x, y, z, translated);
        }

		std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
		{
			return m_transformed_cube->find_all_hits(ray);
		}

		Box bounding_box() const override
		{
			return m_bounding_box;
		}

	};

}

Primitive raytracer::primitives::bounding_box_accelerator(const Primitive& primitive)
{
	return Primitive(std::make_shared<BoundingBoxAcceleratorImplementation>(primitive));
}
