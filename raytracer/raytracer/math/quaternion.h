#pragma once

#include "math/angle.h"
#include "math/vector.h"
#include "math/point.h"

namespace math
{
	struct Quaternion
	{
		double a;
		double b;
		double c;
		double d;

	public:
		Quaternion(double a, double b, double c, double d) 
			: a(a), b(b), c(c), d(d) {};

		static Quaternion rotation(Angle theta, const Vector3D& axis)
		{
			auto a = cos(theta / 2);
			auto b = sin(theta / 2) * axis.x;
			auto c = sin(theta / 2) * axis.y;
			auto d = sin(theta / 2) * axis.z;

			return Quaternion(a, b, c, d);
		}

		Point3D rotate(const Point3D& p);
	protected:
		Quaternion conjugate();
	};

	Quaternion operator +(const Quaternion&, const Quaternion&);
	Quaternion operator -(const Quaternion&, const Quaternion&);
	Quaternion operator *(const Quaternion&, const Quaternion&);
	Quaternion operator *(double, const Quaternion&);
	Quaternion operator *(const Quaternion&, double);
	Quaternion operator /(const Quaternion&, double);

	Quaternion operator +=(const Quaternion&, const Quaternion&);
	Quaternion operator -=(const Quaternion&, const Quaternion&);
	Quaternion operator *=(const Quaternion&, double);
	Quaternion operator /=(const Quaternion&, double);
}