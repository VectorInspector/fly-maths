#include "quaternion.h"

template <typename X>
double Quaternion <X>::Length () const {
	return sqrt(q * q + v.x * v.x + v.y * v.y + v.z * v.z);
}

template <typename X>
double Quaternion <X>::LengthSquared () const {
	return q * q + v.x * v.x + v.y * v.y + v.z * v.z;
}

template <typename X>
Quaternion <X>& Quaternion <X>::Unit1 () {
	auto length = Length();
	
	if(0 < length) {
		q /= length;
		v /= length;
	}
	
	else {
		q = 1;
		v = Vec3 <X> (0, 0, 0);
	}
	
	return *this;
}

template <typename X>
Quaternion <X>& Quaternion <X>::Unit0 () {
	auto length = Length();
	
	if(0 < length) {
		q /= length;
		v /= length;
	}
	
	else {
		q = 0;
		v = Vec3 <X> (0, 0, 0);
	}
	
	return *this;
}

template <typename X>
X Quaternion <X>::Dot (const Quaternion <X>& r) const {
	return q * r.q + v.x * r.v.x + v.y * r.v.y + v.z * r.v.z;
}

template <typename X>
Quaternion <X> Quaternion <X>::operator* (const Quaternion <X>& r) const {
	return Quaternion <X> (q * r.q - v.Dot(r.v), q * r.v + r.q * v + v.Cross(r.v));
}

template <typename X>
Quaternion <X> Quaternion <X>::operator* (double f) const {
	return Quaternion <X> (f * q, f * v);
}

template <typename X>
Quaternion <X> Quaternion <X>::operator/ (double f) const {
	return Quaternion <X> (q / f, v / f);
}

template <typename X>
Quaternion <X> Quaternion <X>::operator+ (const Quaternion <X>& r) const {
	return Quaternion <X> (q + r.q, v + r.v);
}

template <typename X>
Quaternion <X> Quaternion <X>::operator- (const Quaternion <X>& r) const {
	return Quaternion <X> (q - r.q, v - r.v);
}

template <typename X>
Quaternion <X>& Quaternion <X>::operator+= (const Quaternion <X>& r) const {
	q += r.q;
	v += r.v;
	return *this;
}

template <typename X>
Quaternion <X>& Quaternion <X>::operator-= (const Quaternion <X>& r) const {
	q -= r.q;
	v -= r.v;
	return *this;
}

template <typename X>
Quaternion <X>& Quaternion <X>::operator*= (const Quaternion <X>& r) const {
	// *this = *this * r;
	return *this;
};

// This creates a unit quaternion which is a rotation quaternion, so you can use these to modify
// orientation quaternions. Combined with the Quaternion to radiant conversion, you can make 6DOF
// (six degrees of freedom) movement. Alternatively, you may create the adjoint view matrix from
// your view quaternion entirely!
template <typename X>
Quaternion <X> RadQuaternion (double yaw_rad, double pitch_rad, double roll_rad) {
	
	// Only half because the quaternion rotation resolves with an addition formula for sin and cos.
	yaw_rad		/= 2;
	pitch_rad	/= 2;
	roll_rad	/= 2;
	
	auto ca = cos(yaw_rad);
	auto cv = cos(pitch_rad);
	auto cr = cos(roll_rad);
	auto sa = sin(yaw_rad);
	auto sv = sin(pitch_rad);
	auto sr = sin(roll_rad);
	
	return Quaternion <X> (
		ca * cv * cr + sa * sv * sr, Vec3 <X> (
		ca * cv * sr - sa * sv * cr,
		ca * sv * cr + sa * cv * sr,
		sa * cv * cr - ca * sv * sr));
}

template <typename X>
Quaternion <X> operator* (double f, const Quaternion <X>& q) {
	return q * f;
}

template <typename X>
Quaternion <X> IdentityQuat () {
	return Quaternion <X> (1, Vec3 <X> (0, 0, 0));
}

template <typename X>
Quaternion <X> ZeroQuat () {
	return Quaternion <X> (0, Vec3 <X> (0, 0, 0));
}

template <typename X>
Quaternion <X> AxisRotQuat (Vec3 <X> axis, double rad) {
	axis = axis.Unit1();
	rad /= 2;
	return Quaternion <X> (cos(rad), sin(rad) * axis);
}
