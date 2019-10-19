#include "ray-tracer-v2.h"

using namespace imaging;
using namespace math;
using namespace raytracer;

TraceResult raytracer::raytracers::_private_::RayTracerV2::trace(
	const Scene& scene, const math::Ray& ray) const
{
	Hit hit;

	if (scene.root->find_first_positive_hit(ray, &hit))
	{
		Color result = colors::black();
		MaterialProperties mat_prop = hit.material->at(hit.local_position);

		result += compute_ambient(mat_prop);
		result += process_lights(scene, mat_prop, hit, ray);

		unsigned group_id = hit.group_id;
		double t = hit.t;

		return TraceResult(result, group_id, ray, t);
	}
	else
	{
		return TraceResult::no_hit(ray);
	}
}

imaging::Color raytracer::raytracers::_private_::RayTracerV2::process_lights(
	const Scene& scene, const MaterialProperties& mp, const Hit& hit, const Ray& ray) const
{
	Color result = colors::black();

	for (LightSource ls : scene.light_sources)
	{
		result += process_light_source(scene, mp, hit, ray, ls);
	}

	return result;
}

imaging::Color raytracer::raytracers::_private_::RayTracerV2::process_light_source(
	const Scene& scene, const MaterialProperties& mp, const Hit& hit, const Ray& ray, LightSource ls) const
{
	Color result = colors::black();

	for (LightRay lr : ls->lightrays_to(hit.position))
	{
		result += process_light_ray(scene, mp, hit, ray, lr);
	}

	return result;
}

imaging::Color raytracer::raytracers::_private_::RayTracerV2::process_light_ray(
	const Scene& scene, const MaterialProperties& mp, const Hit& hit, const Ray& ray, const LightRay& lr) const
{
	Color result = colors::black();

	result += compute_diffuse(mp, hit, ray, lr);

	return result;
}

imaging::Color raytracer::raytracers::_private_::RayTracerV2::compute_diffuse(
	const MaterialProperties& mp, const Hit& hit, const Ray& ray, const LightRay& lr) const
{
	Color result = colors::black();

	Vector3D incoming_vec = (lr.ray.origin - hit.position).normalized(); // lr.ray.direction

	double cos_alpha = incoming_vec.dot(hit.normal);

	if (cos_alpha > 0)
	{
		result += cos_alpha * lr.color * mp.diffuse;
	}

	return result;
}

raytracer::RayTracer raytracer::raytracers::v2()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV2>());
}
