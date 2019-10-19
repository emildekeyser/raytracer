#include "ray-tracer-v3.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

imaging::Color raytracer::raytracers::_private_::RayTracerV3::process_light_ray(
	const Scene& scene, const MaterialProperties& mp, const Hit& hit, const math::Ray& ray, const LightRay& lr) const
{
	Color result = RayTracerV2::process_light_ray(scene, mp, hit, ray, lr);
	
	result += compute_specular(mp, hit, ray, lr);

	return result;
}

imaging::Color raytracer::raytracers::_private_::RayTracerV3::compute_specular(
	const MaterialProperties& mp, const Hit& hit, const math::Ray& ray, const LightRay& lr) const
{
	Color result = colors::black();

	if (mp.specular != colors::black())
	{
		Vector3D incoming_vec = (hit.position - lr.ray.origin).normalized();

		Vector3D reflected_vec = incoming_vec.reflect_by(hit.normal);	// zie Vector3D class

		//Vector3D reflected_vec = incoming_vec - 2 * incoming_vec.dot(hit.normal) * hit.normal;

		Vector3D outgoing_vec = (ray.origin - hit.position).normalized();

		double cos_alpha = outgoing_vec.dot(reflected_vec);

		if (cos_alpha > 0)
		{
			result += lr.color * mp.specular * pow(cos_alpha, mp.specular_exponent);
			//return lr.color * mp.specular * pow(cos_alpha, mp.specular_exponent);
		}
	}
	
	return result;
}

raytracer::RayTracer raytracer::raytracers::v3()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV3>());
}
