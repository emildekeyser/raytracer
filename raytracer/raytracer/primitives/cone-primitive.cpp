#include "primitives/cone-primitive.h"
#include "math/interval.h"
#include "util/misc.h"
#include "math/quadratic_equation.h"
#include <cmath>

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;


namespace
{
	class ConeAlongYImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{

	public:
		std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
		{
            std::vector<std::shared_ptr<Hit>> hits;

            auto a = pow(ray.direction.x(), 2.0)
                + pow(ray.direction.z(), 2.0)
                - pow(ray.direction.y(), 2.0);

            auto b = 2 * ray.origin.x() * ray.direction.x()
                + 2 * ray.origin.z() * ray.direction.z()
                - 2 * ray.origin.y() * ray.direction.y();

            auto c = pow(ray.origin.x(), 2.0)
                + pow(ray.origin.z(), 2.0)
                - pow(ray.origin.y(), 2.0);

            QuadraticEquation qeq(a, b, c);

            if (qeq.has_solutions())
            {
                // Quadratic equation has solutions: there are two intersections
                double t1 = qeq.x1();
                double t2 = qeq.x2();

                // t1 < t2, swap them if necessary
                sort(t1, t2);

                auto hit1 = std::make_shared<Hit>();
                auto hit2 = std::make_shared<Hit>();
                initialize_hit(hit1.get(), ray, t1);
                initialize_hit(hit2.get(), ray, t2);

                auto y1 = hit1->position.y();
                auto y2 = hit2->position.y();

                if ( y1 <= 1.0 && y1 >= -1.0)
                    hits.push_back(hit1);
                
                if ( y2 <= 1.0 && y2 >= -1.0)
                    hits.push_back(hit2);
            }

            return hits;
        }

        math::Box bounding_box() const override
        {
            auto range = interval(-1.0, 1.0);
            return Box(range, range, range);
        }


	private:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const
		{
			hit->t = t;
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.x(), hit->position.y());
			hit->normal = ray.origin.y() > 0 ? Vector3D(0, 0, 1) : -Vector3D(0, 0, 1);
		}

    };
}

Primitive raytracer::primitives::cone_along_y()
{
	return Primitive(std::make_shared<ConeAlongYImplementation>());
}

