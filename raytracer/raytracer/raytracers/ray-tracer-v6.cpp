#include "ray-tracer-v6.h"

using namespace imaging;
using namespace math;
using namespace raytracer;


TraceResult raytracer::raytracers::_private_::RayTracerV6::trace(
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
		result += compute_refraction(scene, mp, hit, ray, weight);
		
		unsigned group_id = hit.group_id;
		double t = hit.t;

		return TraceResult(result, group_id, ray, t);
	}
	else
	{
		return TraceResult::no_hit(ray);
	}
}

imaging::Color raytracer::raytracers::_private_::RayTracerV6::compute_refraction(
	const Scene& scene, const MaterialProperties& mp, const Hit& hit, const math::Ray& ray, const double weight) const
{
	if (mp.transparency > 0)
	{
		/*** REFRACTION 1 ***/
		Vector3D out_x = (1 / mp.refractive_index) *
			(ray.direction - (ray.direction.dot(hit.normal) * hit.normal));

		double internal_reflection_index_out = 1 - (out_x.norm() * out_x.norm());

		if (internal_reflection_index_out < 0)
		{
			return colors::black();
		}

		Vector3D out_y = -sqrt(internal_reflection_index_out) * hit.normal;
		Vector3D out_vector = out_x + out_y;

		Ray refracted_ray(hit.position + 0.00001 * out_vector, out_vector);

		Hit exit_hit;

		if (!scene.root->find_first_positive_hit(refracted_ray, &exit_hit))
		{
			return colors::black();
		}

		/*** REFRACTION 2 ***/
		MaterialProperties mp_exit = exit_hit.material->at(exit_hit.local_position);

		Vector3D exit_x = (1 / mp_exit.refractive_index) *
			(refracted_ray.direction -
			(refracted_ray.direction.dot(exit_hit.normal) * exit_hit.normal));

		double internal_reflection_index_exit = 1 - (exit_x.norm() * exit_x.norm());
		if (internal_reflection_index_exit < 0)
		{
			return colors::black();
		}

		Vector3D exit_y = -sqrt(internal_reflection_index_exit) * exit_hit.normal;
		Vector3D exit_vector = exit_x + exit_y;

		Ray exit_ray(exit_hit.position + 0.00001 * exit_vector, exit_vector);

		/*** RECURSIVE TRACE***/
		TraceResult refraction_trace = trace(scene, exit_ray, weight * mp.transparency);

		return refraction_trace.color * mp.transparency;
	}
	else
	{
		return colors::black();
	}
}


raytracer::RayTracer raytracer::raytracers::v6()
{
	return raytracer::RayTracer(std::make_shared<raytracer::raytracers::_private_::RayTracerV6>());
}