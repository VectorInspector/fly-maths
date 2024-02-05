#ifndef SPACE_H
#define SPACE_H

#include "quaternion.h"
#include "mat3x3.h"
#include "vec3.h"
#include "vec2.h"
#include <string>

// Helper which stores pairs of trigonometric values for convenience.
struct CosSinVals {
	CosSinVals (double yaw, double pitch, double roll);
	CosSinVals (double yaw, double pitch);
	CosSinVals ();
	
	double ca;
	double cv;
	double cr;
	double sa;
	double sv;
	double sr;
};

struct SpaceConst {
	static constexpr double Pi () {
		return std::acos(-1);
	}
	
	static constexpr double PiHalf () {
		return std::acos(0);
	}
	
	static constexpr double RadToDegFactor () {
		return 180.0 / Pi();
	}
	
	static constexpr double DegToRadFactor () {
		return Pi() / 180;
	}
};

double DegToRad (double deg);
double RadToDeg (double rad);

// Conversions back to radiants
struct VectorToRad {
	
	static constexpr double sin_extreme = 0.5 - 1e-8;
	
	template <typename X>
	void FromQuat (Quaternion <X> r);
	double yaw_rad;
	double pitch_rad;
	double roll_rad;
};

template <typename X>
void Vec3toString (std::string& s, const Vec3 <X>& v);

template <typename X>
void Vec2toString (std::string& s, const Vec2 <X>& v);

template <typename X>
void QuatToString (std::string& s, const Quaternion <X>& q);

template <typename X>
void Mat3x3toString (std::string& s, const Mat3x3 <X>& m);

#endif
