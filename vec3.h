#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include "space.h"

template <typename X>
struct Vec3 {
	Vec3 <X> Abs () const;
	X Max () const;
	X Min () const;
	X Dot (const Vec3 <X>& v) const;
	X LenSquared () const;
	double Len () const;
	Vec3& Unit0 ();
	Vec3 <X> Cross (const Vec3 <X>& v) const;
	Vec3 <X> operator- () const;
	Vec3 <X>& operator+= (const Vec3 <X>& v);
	Vec3 <X>& operator-= (const Vec3 <X>& v);
	Vec3 <X> operator+ (const Vec3 <X>& v) const;
	Vec3 <X> operator- (const Vec3 <X>& v) const;
	
	X x;
	X y;
	X z;
};

template <typename X>
Vec3 <X> RadVec3 (double yaw_rad, double pitch_rad);

template <typename X>
Vec3 <X> DegVec3 (double yaw_deg, double pitch_deg);

template <typename X>
Vec3 <X> operator* (double f, Vec3 <X> v);

template <typename X>
Vec3 <X> operator* (Vec3 <X> v, double f);

template <typename X>
Vec3 <X> operator/ (Vec3 <X> v, double f);

template <typename X>
Vec3 <X>& operator*= (Vec3 <X>& v, double f);

template <typename X>
Vec3 <X>& operator/= (Vec3 <X>& v, double f);

using Vec3f = Vec3 <float>;
using Vec3d = Vec3 <double>;

#endif
