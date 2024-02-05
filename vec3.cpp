#include "vec3.h"
#include "space.h"

template <typename X>
Vec3 <X> Vec3 <X>::Abs () const {
	return Vec3 <X> (abs(x), abs(y), abs(z));
}

template <typename X>
X Vec3 <X>::Max () const {
	return max(max(x, y), z);
}

template <typename X>
X Vec3 <X>::Min () const {
	return min(min(x, y), z);
}

template <typename X>
X Vec3 <X>::Dot (const Vec3 <X>& v) const {
	return x * v.x + y * v.y + z * v.z;
}

template <typename X>
X Vec3 <X>::LenSquared () const {
	return x * x + y * y + z * z;
}

template <typename X>
double Vec3 <X>::Len () const {
	return sqrt(x * x + y * y + z * z);
}

template <typename X>
Vec3 <X>& Vec3 <X>::Unit0 () {
	auto len = Len();
	
	if(0 < len) {
		x /= len;
		y /= len;
		z /= len;
	}
	
	else {
		x = 0;
		y = 0;
		z = 0;
	}
	
	return *this;
}

template <typename X>
Vec3 <X> Vec3 <X>::Cross (const Vec3 <X>& v) const {
	return Vec3 <X> (
		y * v.z - z * v.y,
		z * v.x - x * v.z,
		x * v.y - y * v.x);
}

template <typename X>
Vec3 <X> Vec3 <X>::operator- () const {
	return Vec3 <X> (x, y, z);
}

template <typename X>
Vec3 <X>& Vec3 <X>::operator+= (const Vec3 <X>& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

template <typename X>
Vec3 <X>& Vec3 <X>::operator-= (const Vec3 <X>& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

template <typename X>
Vec3 <X> Vec3 <X>::operator+ (const Vec3 <X>& v) const {
	return Vec3 <X> (x + v.x, y + v.y, z + v.z);
}

template <typename X>
Vec3 <X> Vec3 <X>::operator- (const Vec3 <X>& v) const {
	return Vec3 <X> (x - v.x, y - v.y, z - v.z);
}


template <typename X>
Vec3 <X> RadVec3 (double yaw_rad, double pitch_rad) {
	auto cos_yaw = cos(yaw_rad);
	auto sin_yaw = sin(yaw_rad);
	auto cos_pitch = cos(pitch_rad);
	auto sin_pitch = sin(pitch_rad);
	
	return Vec3 <X> (cos_pitch * cos_yaw, cos_pitch * sin_yaw, sin_pitch);
}

template <typename X>
Vec3 <X> DegVec3 (double yaw_deg, double pitch_deg) {
	auto yaw_rad = DegToRad(yaw_deg);
	auto pitch_rad = DegToRad(pitch_deg);
	auto cos_yaw = cos(yaw_rad);
	auto sin_yaw = sin(yaw_rad);
	auto cos_pitch = cos(pitch_rad);
	auto sin_pitch = sin(pitch_rad);
	
	return Vec3 <X> (cos_pitch * cos_yaw, cos_pitch * sin_yaw, sin_pitch);
}

template <typename X>
Vec3 <X> operator* (double f, Vec3 <X> v) {
	return Vec3 <X> (f * v.x, f * v.y, f * v.z);
}

template <typename X>
Vec3 <X> operator* (Vec3 <X> v, double f) {
	return Vec3 <X> (f * v.x, f * v.y, f * v.z);
}

template <typename X>
Vec3 <X> operator/ (Vec3 <X> v, double f) {
	return Vec3 <X> (v.x / f, v.y / f, v.z / f);
}

template <typename X>
Vec3 <X>& operator*= (Vec3 <X>& v, double f) {
	v.x *= f;
	v.y *= f;
	v.z *= f;
	return v;
}

template <typename X>
Vec3 <X>& operator/= (Vec3 <X>& v, double f) {
	v.x /= f;
	v.y /= f;
	v.z /= f;
	return v;
}