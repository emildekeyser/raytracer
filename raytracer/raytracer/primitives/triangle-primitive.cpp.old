#include "primitives/triangle-primitive.h"
#include "math/interval.h"
#include "util/misc.h"

#include "easylogging++.h"
#include <cmath>

using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;

namespace
{
	
	class TriangleImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
	{
    private:
        const Point3D m_vertex1;
        const Point3D m_vertex2;
        const Point3D m_vertex3;
        int m_index;

		void initialize_hit(Hit* hit, const Ray& ray, double t, const Vector3D& normal) const
		{
			hit->t = t;
			hit->position = ray.at(hit->t);
			hit->local_position.xyz = hit->position;
			hit->local_position.uv = Point2D(hit->position.y(), hit->position.z());
			/* hit->normal = ray.origin.x() > 0 ? normal : -normal; */
			hit->normal = normal; // TODO: this is wrong ?
		}

        bool in_triangle(const Point3D& point, const Vector3D& normal) const
        {
            /* // edge 0 */
            /* Vec3f edge0 = v1 - v0; */ 
            /* Vec3f vp0 = P - v0; */ 
            /* C = edge0.crossProduct(vp0); */ 
            /* if (N.dotProduct(C) < 0) return false; // P is on the right side */ 

            Vector3D edge = m_vertex2 - m_vertex1;
            Vector3D vp = point - m_vertex1;
            Vector3D C = edge.cross(vp);
            if (C.dot(normal) < 0) return false;

            edge = m_vertex3 - m_vertex2;
            vp = point - m_vertex2;
            C = edge.cross(vp);
            if (C.dot(normal) < 0) return false;

            edge = m_vertex1 - m_vertex3;
            vp = point - m_vertex3;
            C = edge.cross(vp);
            if (C.dot(normal) < 0) return false;

            return true;
        }

    public:
        static int counter;
        
		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="vector1">
		/// vector1
		/// </param>
		/// <param name="vector2">
		/// vector2
		/// </param>
		/// <param name="vector3">
		/// vector3
		/// </param>
		TriangleImplementation(const Point3D& vertex1, const Point3D& vertex2, const Point3D& vertex3)
			: m_vertex1(vertex1), m_vertex2(vertex2), m_vertex3(vertex3),
            m_index(TriangleImplementation::counter++){}

		std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
		{
            LOG(INFO) << "TRIANGLE:" << m_index;
            // LOG(INFO) << "TRIANGLE:" << m_vertex1 << m_vertex2 << m_vertex3;

			std::vector<std::shared_ptr<Hit>> hits;
            Vector3D ab = m_vertex1 - m_vertex2;
            Vector3D ac = m_vertex1 - m_vertex3;
            Vector3D normal = ab.cross(ac);
            normal.normalize();

            /* t = - (dot(N, orig) + D) / dot(N, dir); */ 
			double denom = ray.direction.dot(normal);
			// If denominator == 0, there is no intersection (ray runs parallel to triangle)
			if (denom != approx(0.0))
			{
				double numer = -((ray.origin - Point3D(0, 0, 0)).dot(normal) + (m_vertex1 - Point3D(0, 0, 0)).dot(normal));
				double t = numer / denom;

                if (in_triangle(ray.at(t), normal))
                {
                    auto hit = std::make_shared<Hit>();
                    initialize_hit(hit.get(), ray, t, normal);
                    hits.push_back(hit);
                }
			}

			return hits;
		}

		math::Box bounding_box() const override
		{
            double min_x = std::min(std::min(m_vertex1.x(), m_vertex2.x()), m_vertex3.x());
			double max_x = std::max(std::max(m_vertex1.x(), m_vertex2.x()), m_vertex3.x());
			double min_y = std::min(std::min(m_vertex1.y(), m_vertex2.y()), m_vertex3.y());
			double max_y = std::max(std::max(m_vertex1.y(), m_vertex2.y()), m_vertex3.y());
			double min_z = std::min(std::min(m_vertex1.z(), m_vertex2.z()), m_vertex3.z());
			double max_z = std::max(std::max(m_vertex1.z(), m_vertex2.z()), m_vertex3.z());
			return Box(interval(min_x, max_x), interval(min_y, max_y), interval(min_z, max_z));
		}

        std::string to_string() override
        {
            return "triangle";
        }
	};

    int TriangleImplementation::counter = 0;

}

Primitive raytracer::primitives::triangle(const Point3D& vertex1, const Point3D& vertex2, const Point3D& vertex3)
{
	return Primitive(std::make_shared<TriangleImplementation>(vertex1, vertex2, vertex3));
}
