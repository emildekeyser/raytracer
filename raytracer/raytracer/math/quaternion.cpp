#include "math/quaternion.h"

using namespace math;


Point3D math::Quaternion::rotate(const Point3D& p)
{
	auto p_quar = Quaternion(0, p.x, p.y, p.z);
	auto q_quar = *this;

	auto p_quar_rot = q_quar * p_quar * q_quar.conjugate();

	return Point3D(p_quar_rot.b, p_quar.c, p_quar_rot.d);
}

Quaternion math::Quaternion::conjugate()
{
	return Quaternion(this->a, -this->b, -this->c, -this->d);
}


Quaternion math::operator +(const Quaternion& q1, const Quaternion& q2)
{
	return Quaternion(q1.a + q2.a, 
					  q1.b + q2.b, 
					  q1.c + q2.c, 
					  q1.d + q2.d);
}

Quaternion math::operator -(const Quaternion& q1, const Quaternion& q2)
{
	return Quaternion(q1.a - q2.a, 
					  q1.b - q2.b, 
					  q1.c - q2.c, 
					  q1.d - q2.d)
}

Quaternion math::operator *(const Quaternion& q1, const Quaternion& q2)
{
	return Quaternion(q1.a * q2.a - q1.b * q2.b - q1.c * q2.c - q1.d * q2.d,
					  q1.a * q2.b + q1.b * q2.a + q1.c * q2.d - q1.d * q2.c,
					  q1.a * q2.c - q1.b * q2.d + q1.c * q2.a + q1.d * q2.b,
					  q1.a * q2.d + q1.b * q2.c - q1.c * q2.b + q1.d * q2.a);
}

Quaternion math::operator *(const Quaternion& q, double x)
{
	return Quaternion(q.a * x, q.b * x, q.c * x, q.d * x);
}

Quaternion math::operator *(double x, const Quaternion& q)
{
	return x * q;
}


Quaternion math::operator /(const Quaternion& q, double x)
{
	return q * (1 / x);
}

Quaternion math::operator +=(const Quaternion& q1, const Quaternion& q2)
{
	return q1 + q2;
}
Quaternion math::operator -=(const Quaternion& q1, const Quaternion& q2)
{
	return q1 - q2;
}
Quaternion math::operator *=(const Quaternion& q1, double x)
{
	return q1 * x;
}
Quaternion math::operator /=(const Quaternion& q1, double x)
{
	return q1 * (1 /x);
}