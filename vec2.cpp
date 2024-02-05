#include "vec2.h"
#include "space.h"

template <typename X>
X Vec2 <X>::Dot (const Vec2 <X>& v) const {
	return x * v.x + y * v.y;
}

template <typename X>
Vec2 <X> Vec2 <X>::Rot90is () const {
	return Vec2 <X> (-y, x);
}

template <typename X>
Vec2 <X> Vec2 <X>::Rot90as () const {
	return Vec2 <X> (y, -x);
}

template <typename X>
X Vec2 <X>::LengthSquared () const {
	return x * x + y * y;
}

template <typename X>
double Vec2 <X>::Length () const {
	return sqrt(x * x + y * y);
}

template <typename X>
Vec2 <X> Vec2 <X>::Abs () const {
	return Vec2 <X> (abs(x), abs(y));
}

template <typename X>
Vec2 <X> Vec2 <X>::operator- () const {
	return Vec2 <X> (x, y);
}

template <typename X>
Vec2 <X>& Vec2 <X>::operator+= (const Vec2 <X>& v) {
	x += v.x;
	y += v.y;
	return *this;
}

template <typename X>
Vec2 <X>& Vec2 <X>::operator-= (const Vec2 <X>& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

template <typename X>
Vec2 <X>& Vec2 <X>::operator*= (double f) {
	x *= f;
	y *= f;
	return *this;
}

template <typename X>
Vec2 <X> Vec2 <X>::operator+ (const Vec2 <X>& v) const {
	return Vec2 <X> (x + v.x, y + v.y);
}

template <typename X>
Vec2 <X> Vec2 <X>::operator- (const Vec2 <X>& v) const {
	return Vec2 <X> (x - v.x, y - v.y);
}

template <typename X>
Vec2 <X> Vec2 <X>::operator* (double f) const {
	return Vec2 <X> (f * x, f * y);
}

template <typename X>
Vec2 <X> Vec2 <X>::operator/ (double f) const {
	return Vec2 <X> (x / f, y / f);
}

template <typename X>
Vec2 <X> operator* (double f, const Vec2 <X>& v) {
	return Vec2 <X> (f * v.x, f * v.y);
}

template <typename X>
Vec2 <X> RadVec2 (double rad) {
	return Vec2 <X> (cos(rad), sin(rad));
}

template <typename X>
Vec2 <X> DegVec2 (double deg) {
	auto rad = DegToRad(deg);
	return Vec2 <X> (cos(rad), sin(rad));
}
