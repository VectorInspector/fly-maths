#ifndef QUATERNION_H
#define QUATERNION_H

#include "vec3.h"

template <typename X>
struct Quaternion {
	double Length () const;
	
	double LengthSquared () const;
	
	Quaternion <X>& Unit1 ();
	
	Quaternion <X>& Unit0 ();
	
	X Dot (const Quaternion <X>& r) const;
	
	Quaternion <X> operator* (const Quaternion <X>& r) const;
	
	Quaternion <X> operator* (double f) const;
	
	Quaternion <X> operator/ (double f) const;
	
	Quaternion <X> operator+ (const Quaternion <X>& r) const;
	
	Quaternion <X> operator- (const Quaternion <X>& r) const;
	
	Quaternion <X>& operator+= (const Quaternion <X>& r) const;
	
	Quaternion <X>& operator-= (const Quaternion <X>& r) const;
	
	Quaternion <X>& operator*= (const Quaternion <X>& r) const;
	
	// Storage in scalar + vector format.
	X q;
	Vec3 <X> v;
};

// This creates a unit quaternion which is a rotation quaternion, so you can use these to modify
// orientation quaternions. Combined with the Quaternion to radiant conversion, you can make 6DOF
// (six degrees of freedom) movement. Alternatively, you may create the adjoint view matrix from
// your view quaternion entirely!
template <typename X>
Quaternion <X> RadQuaternion (double yaw_rad, double pitch_rad, double roll_rad);

template <typename X>
Quaternion <X> operator* (double f, const Quaternion <X>& q);

template <typename X>
Quaternion <X> IdentityQuat ();

template <typename X>
Quaternion <X> ZeroQuat ();

template <typename X>
Quaternion <X> AxisRotQuat (Vec3 <X> axis, double rad);

using Quatf = Quaternion <float>;
using Quatd = Quaternion <double>;

#endif
