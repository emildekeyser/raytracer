#include "ray-tracer-v5.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

TraceResult raytracer::raytracers::_private_::RayTracerV5::trace(const Scene& scene, const math::Ray& ray) const
{
	return trace(scene, ray, 1.0);
}


TraceResult raytracer::raytracers::_private_::RayTracerV5::trace(
	const Scene& scene, const math::Ray& ray, const double weight) const 
{
	Hit hit;

	if (weight > 0.01 
		&& scene.root->find_first_positive_hit(ray, &hit))
	{
		Color result = colors::black();
		MaterialProperties mp = hit.material->at(hit.local_position);

		result += compute_ambient(mp);
		result += process_lights(scene, mp, hit, ray);
		result += compute_reflection(scene, mp, hit, ray, weight);

		unsigned group_id = hit.group_id;
		double t = hit.t;

		return TraceResult(result, group_id, ray, t);
	}
	else
	{
		return TraceResult::no_hit(ray);
	}
}

imaging::Color raytracer::raytracers::_private_::RayTracerV5::compute_reflection(
	const Scene& scene, const MaterialProperties& mp, const Hit& hit, const math::Ray& ray, const double weight) const
{
	if (mp.reflectivity != 0.0)
	{
		Vector3D reflected_vector = ray.direction.reflect_by(hit.normal);

		Ray reflected_ray(hit.position + 0.00001 * reflected_vector, reflected_vector);

		TraceResult reflected_trace = trace(scene, reflected_ray, weight * mp.reflectivity);

		return mp.reflectivity * reflected_trace.color;
	}
	else
	{
		return colors::black();
	}
}



raytracer::RayTracer raytracer::raytracers::v5()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV5>());
}