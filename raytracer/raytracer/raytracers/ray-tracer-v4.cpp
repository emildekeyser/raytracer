#include "ray-tracer-v4.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

imaging::Color raytracer::raytracers::_private_::RayTracerV4::process_light_ray(
	const Scene& scene, const MaterialProperties& mp, const Hit& hit, const math::Ray& ray, const LightRay& lr) const
{
	Ray shadow_ray(lr.ray.origin, hit.position);
	Hit shadow_hit;

	if (scene.root->find_first_positive_hit(shadow_ray, &shadow_hit) && 0 < shadow_hit.t && shadow_hit.t < 0.9999 )
	{
		return colors::black();
	}

	return RayTracerV3::process_light_ray(scene, mp, hit, ray, lr);;
}

raytracer::RayTracer raytracer::raytracers::v4()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV4>());
}
