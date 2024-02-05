#ifndef VEC2_H
#define VEC2_H

#include <cmath>

template <typename X>
struct Vec2 {
	X Dot (const Vec2 <X>& v) const;
	Vec2 <X> Rot90is () const;
	Vec2 <X> Rot90as () const;
	X LengthSquared () const;
	double Length () const;
	Vec2 <X> Abs () const;
	Vec2 <X> operator- () const;
	Vec2 <X>& operator+= (const Vec2 <X>& v);
	Vec2 <X>& operator-= (const Vec2 <X>& v);
	Vec2 <X>& operator*= (double f);
	Vec2 <X> operator+ (const Vec2 <X>& v) const;
	Vec2 <X> operator- (const Vec2 <X>& v) const;
	Vec2 <X> operator* (double f) const;
	Vec2 <X> operator/ (double f) const;
	
	X x;
	X y;
};

template <typename X>
Vec2 <X> operator* (double f, const Vec2 <X>& v);

template <typename X>
Vec2 <X> RadVec2 (double rad);

template <typename X>
Vec2 <X> DegVec2 (double deg);

using Vec2f = Vec2 <float>;
using Vec2d = Vec2 <double>;

#endif
