#include "primitives/bounding-box-accelerator-primitive.h"
#include "primitives/square-primitive.h"
#include "primitives/transformer-primitive.h"
#include "primitives/cube-primitive.h"
#include "math/interval.h"
#include "util/misc.h"
#include "easylogging++.h"
#include <cmath>

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;

namespace
{

	class BoundingBoxAcceleratorImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
    private:
        Primitive m_primitive;
        const Box m_bounding_box;
        int m_index;

    public:
        static int counter;

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="primitive">
		/// primitive
		/// </param>
		BoundingBoxAcceleratorImplementation(Primitive primitive)
            : m_primitive(primitive), m_bounding_box(primitive->bounding_box()),
            m_index(BoundingBoxAcceleratorImplementation::counter++){}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
		{
            std::vector<std::shared_ptr<Hit>> return_value;
            if (m_bounding_box.is_hit_by(ray))
            {
				// LOG(INFO) << "BOX index:" << m_index;
				// LOG(INFO) << "BOX:" << m_primitive->to_string();
                return_value = m_primitive->find_all_hits(ray);
            }

            return return_value;
		}

		Box bounding_box() const override
		{
			return m_bounding_box;
		}

        std::string to_string() override
        {
            return "bounding box";
        }
    };

    int BoundingBoxAcceleratorImplementation::counter = 0;

    class EmptyBoxImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
    {
        public:

            std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
            {
                return std::vector<std::shared_ptr<Hit>>();
            }

            Box bounding_box() const override
            {
                return Box::empty();
            }

            std::string to_string() override
            {
                return "empty box";
            }
    };

}

Primitive raytracer::primitives::bounding_box_accelerator(const Primitive& primitive)
{
    return Primitive(std::make_shared<BoundingBoxAcceleratorImplementation>(primitive));
}

Primitive raytracer::primitives::empty_box()
{
    return Primitive(std::make_shared<EmptyBoxImplementation>());
}

