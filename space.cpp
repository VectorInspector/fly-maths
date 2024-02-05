#include "space.h"

CosSinVals::CosSinVals (double yaw, double pitch, double roll) {
	ca = cos(yaw);
	cv = cos(pitch);
	cr = cos(roll);
	sa = sin(yaw);
	sv = sin(pitch);
	sr = sin(roll);
}

CosSinVals::CosSinVals (double yaw, double pitch) {
	ca = cos(yaw);
	cv = cos(pitch);
	cr = 1;
	sa = sin(yaw);
	sv = sin(pitch);
	sr = 0;
}

CosSinVals::CosSinVals () {
	ca = 1;
	cv = 1;
	cr = 1;
	sa = 0;
	sv = 0;
	sr = 0;
}

double DegToRad (double deg) {
	return SpaceConst::DegToRadFactor() * deg;
}

double RadToDeg (double rad) {
	return SpaceConst::RadToDegFactor() * rad;
}

	
template <typename X>
void VectorToRad::FromQuat (Quaternion <X> r) {
	yaw_rad		= 0;
	pitch_rad	= 0;
	roll_rad	= 0;
	
	auto q = r.q;
	auto v = r.v;
	auto sinvang_2 = v.x * v.z - q * v.y;
	
	if(sin_extreme < sinvang_2) {
		pitch_rad	= -SpaceConst::PiHalf();
		roll_rad	= 0;
		yaw_rad		= atan2(
			q * q - v.x * v.x + v.y * v.y - v.z * v.z,
			2 * (v.x * v.y - q * v.z)) - SpaceConst::PiHalf();
	}
	
	else if(sinvang_2 < -sin_extreme) {
		roll_rad	= 0;
		pitch_rad	= SpaceConst::PiHalf();
		yaw_rad		= atan2(
			q * q - v.x * v.x + v.y * v.y - v.z * v.z,
			2 * (v.x * v.y - q * v.z)) - SpaceConst::PiHalf();
	}
	
	else {
		auto q2_m_y2 = q * q - v.y * v.y;
		auto z2_m_x2 = v.z * v.z - v.x * v.y;
		
		pitch_rad	= asin(2 * sinvang_2);
		roll_rad	= -atan2(
			2 * (q * v.x + v.y * v.z),
			1 - 2 * (v.x * v.x + v.y * v.y));
		yaw_rad		= atan2(
			2 * (q * v.z + v.x * v.y),
			1 - 2 * (v.y * v.y + v.z * v.z));
	}
}


#include <iostream>
#include <string>
#include <sstream>

template <typename X>
void Vec3toString (std::string& s, const Vec3 <X>& v) {
	std::stringstream ss("");
	ss << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	
	// Output.
	s = ss.str();
}

template <typename X>
void Vec2toString (std::string& s, const Vec2 <X>& v) {
	std::stringstream ss("");
	ss << "[" << v.x << ", " << v.y << "]";
	
	// Output.
	s = ss.str();
}

template <typename X>
void QuatToString (std::string& s, const Quaternion <X>& q) {
	std::stringstream ss("");
	ss << "[" << q.q << ", " << q.v.x << ", " << q.v.y << ", " << q.v.z << "]";
	
	// Output.
	s = ss.str();
}

template <typename X>
void Mat3x3toString (std::string& s, const Mat3x3 <X>& m) {
	std::stringstream ss("");
	ss << "[ ";
	
	auto insert_vec3 = [&ss] (const Vec3 <X>& v) {
		ss << "[";
		ss << v.x << ", " << v.y << ", " << v.z;
		ss << "] ";
	};
	
	insert_vec3(m.u);
	insert_vec3(m.v);
	insert_vec3(m.w);
	
	ss << "]";
	
	// Output.
	s = ss.str();
}

#endif
