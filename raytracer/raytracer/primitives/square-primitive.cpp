#include "primitives/square-primitive.h"
#include "math/interval.h"

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;


namespace
{
	/// <summary>
	/// Superclass for squares. Contains common logic.
	/// </summary>
	class SquareImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
	protected:
		const Vector3D m_normal;

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="normal">
		/// Normal vector on square. Needs to have unit length.
		/// </param>
		SquareImplementation(const Vector3D& normal)
			: m_normal(normal)
		{
			assert(normal.is_unit());
		}

		virtual void initialize_hit(Hit* hit, const Ray& ray, double t) const = 0;
		virtual bool in_square(Point3D point) const = 0;

	public:
		std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
		{
			std::vector<std::shared_ptr<Hit>> hits;

			// Compute denominator
			double denom = ray.direction.dot(m_normal);

			// If denominator == 0, there is no intersection (ray runs parallel to square)
			if (denom != approx(0.0))
			{
				// Compute numerator
				double numer = -((ray.origin - Point3D(0, 0, 0)).dot(m_normal));

				// Compute t
				double t = numer / denom;


                if (in_square(ray.at(t)))
                {
                    // Create hit object
                    auto hit = std::make_shared<Hit>();

                    // shared_ptr<T>::get() returns the T* inside the shared pointer
                    initialize_hit(hit.get(), ray, t);

                    // Put hit in list
                    hits.push_back(hit);
                }
			}

			return hits;
		}
	};

	class SquareXYImplementation : public SquareImplementation
	{
	public:
		SquareXYImplementation()
			: SquareImplementation(Vector3D(0, 0, 1))
		{
			// NOP
		}

		math::Box bounding_box() const override
		{
			return Box(interval(-1.0, 1.0), interval(-1.0, 1.0), Interval<double>::empty());
		}

	protected:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const override
		{
			hit->t = t;
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.x(), hit->position.y());
			hit->normal = ray.origin.z() > 0 ? m_normal : -m_normal;
		}

        bool in_square(Point3D point) const override
        {
            auto bb = bounding_box();
            return bb.x().contains(point.x())
                && bb.y().contains(point.y());
        }

	};

	class SquareXZImplementation : public SquareImplementation
	{
	public:
		SquareXZImplementation()
			: SquareImplementation(Vector3D(0, 1, 0))
		{
			// NOP
		}

		math::Box bounding_box() const override
		{
            return Box(interval(-1.0, 1.0), Interval<double>::empty(), interval(-1.0, 1.0));
		}
	protected:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const override
		{
			hit->t = t;
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.x(), hit->position.z());
			hit->normal = ray.origin.y() > 0 ? m_normal : -m_normal;
		}

        bool in_square(Point3D point) const override
        {
            auto bb = bounding_box();
            return bb.x().contains(point.x())
                && bb.z().contains(point.z());
        }

	};

	class SquareYZImplementation : public SquareImplementation
	{
	public:
		SquareYZImplementation()
			: SquareImplementation(Vector3D(1, 0, 0))
		{
			// NOP
		}

		math::Box bounding_box() const override
		{
            return Box(Interval<double>::empty(), interval(-1.0, 1.0), interval(-1.0, 1.0));
		}

	protected:
		void initialize_hit(Hit* hit, const Ray& ray, double t) const override
		{
			hit->t = t;
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.y(), hit->position.z());
			hit->normal = ray.origin.x() > 0 ? m_normal : -m_normal;
		}

        bool in_square(Point3D point) const override
        {
            auto bb = bounding_box();
            return bb.y().contains(point.y())
                && bb.z().contains(point.z());
        }

	};
}

Primitive raytracer::primitives::xy_square()
{
	return Primitive(std::make_shared<SquareXYImplementation>());
}

Primitive raytracer::primitives::xz_square()
{
	return Primitive(std::make_shared<SquareXZImplementation>());
}

Primitive raytracer::primitives::yz_square()
{
	return Primitive(std::make_shared<SquareYZImplementation>());
}
