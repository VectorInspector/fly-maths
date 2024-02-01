#include <cmath>

struct CosSinVals {
	CosSinVals (double yaw, double pitch, double roll) {
		ca = cos(yaw);
		cv = cos(pitch);
		cr = cos(roll);
		sa = sin(yaw);
		sv = sin(pitch);
		sr = sin(roll);
	}
	
	CosSinVals (double yaw, double pitch) {
		ca = cos(yaw);
		cv = cos(pitch);
		cr = 1;
		sa = sin(yaw);
		sv = sin(pitch);
		sr = 0;
	}
	
	CosSinVals () {
		ca = 1;
		cv = 1;
		cr = 1;
		sa = 0;
		sv = 0;
		sr = 0;
	}
	
	double ca;
	double cv;
	double cr;
	double sa;
	double sv;
	double sr;
};

template <typename X>
struct Vec2 {
	X Dot (const Vec2 <X>& v) const {
		return x * v.x + y * v.y;
	}
	
	Vec2 <X> Rot90is () const {
		return Vec2 <X> (-y, x);
	}
	
	Vec2 <X> Rot90as () const {
		return Vec2 <X> (y, -x);
	}
	
	X LengthSquared () const {
		return x * x + y * y;
	}
	
	double Length () const {
		return sqrt(x * x + y * y);
	}
	
	Vec2 <X> Abs () const {
		return Vec2 <X> (abs(x), abs(y));
	}
	
	Vec2 <X> operator- () const {
		return Vec2 <X> (x, y);
	}
	
	Vec2 <X>& operator+= (const Vec2 <X>& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	
	Vec2 <X>& operator-= (const Vec2 <X>& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	
	Vec2 <X>& operator*= (double f) {
		x *= f;
		y *= f;
		return *this;
	}
	
	Vec2 <X> operator+ (const Vec2 <X>& v) const {
		return Vec2 <X> (x + v.x, y + v.y);
	}
	
	Vec2 <X> operator- (const Vec2 <X>& v) const {
		return Vec2 <X> (x - v.x, y - v.y);
	}
	
	Vec2 <X> operator* (double f) const {
		return Vec2 <X> (f * x, f * y);
	}
	
	Vec2 <X> operator/ (double f) const {
		return Vec2 <X> (x / f, y / f);
	}
	
	X x;
	X y;
};

template <typename X>
Vec2 <X> operator* (double f, const Vec2 <X>& v) {
	return Vec2 <X> (f * v.x, f * v.y);
}

template <typename X>
struct Vec3 {
	// Vec3 (X x_, X y_, X z_) : x(x_), y(y_), z(z_) {}
	// Vec3 () : x(0), y(0), z(0) {}
	// Vec3 (const Vec3 <X>& v) : x(v.x), y(v.y), z(v.z) {}
	
	Vec3 <X> Abs () const {
		return Vec3 <X> (abs(x), abs(y), abs(z));
	}
	
	X Max () const {
		return max(max(x, y), z);
	}
	
	X Min () const {
		return min(min(x, y), z);
	}
	
	X Dot (const Vec3 <X>& v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	
	X LenSquared () const {
		return x * x + y * y + z * z;
	}
	
	double Len () const {
		return sqrt(x * x + y * y + z * z);
	}
	
	Vec3& Unit0 () {
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
	
	Vec3 <X> Cross (const Vec3 <X>& v) const {
		return Vec3 <X> (
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}
	
	Vec3 <X> operator- () const {
		return Vec3 <X> (x, y, z);
	}
	
	Vec3 <X>& operator+= (const Vec3 <X>& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	Vec3 <X>& operator-= (const Vec3 <X>& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	
	Vec3 <X> operator+ (const Vec3 <X>& v) const {
		return Vec3 <X> (x + v.x, y + v.y, z + v.z);
	}
	
	Vec3 <X> operator- (const Vec3 <X>& v) const {
		return Vec3 <X> (x - v.x, y - v.y, z - v.z);
	}
	
	X x;
	X y;
	X z;
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

double DegToRad (double deg) {
	return SpaceConst::DegToRadFactor() * deg;
}

double RadToDeg (double rad) {
	return SpaceConst::RadToDegFactor() * rad;
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

using Vec3f = Vec3 <float>;
using Vec3d = Vec3 <double>;

template <typename X>
struct Mat3x3 {
	// Mat3x3 () : u(0, 0, 0), v(0, 0, 0), z(0, 0, 0) {}
	// Mat3x3 () : u(0, 0, 0), v(0, 0, 0), z(0, 0, 0) {}
	
	Mat3x3& Transpose () {
		using std::swap;
		swap(u.y, v.x);
		swap(u.z, w.x);
		swap(v.z, w.y);
		return *this;
	}
	
	// Form the matrix of co-determinants.
	Mat3x3& Adjoint () {
		Vec3 <X> new_u (
			v.y * w.z - v.z * w.y,
			v.z * w.x - v.x * w.z,
			v.x * w.y - v.y * w.x);
		
		Vec3 <X> new_v (
			w.y * u.z - w.z * u.y,
			w.z * u.x - w.x * u.z,
			w.x * u.y - w.y * u.x);
			
		Vec3 <X> new_w (
			u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x);
		
		u = new_u;
		v = new_v;
		w = new_w;
		return *this;
	}
	
	bool IsSingular () const {
		return abs(Det()) < 1e-9;
	}
	
	// Inverts a matrix if possible.
	Mat3x3& Invert () {
		auto det = Det();
		
		// This matrix is singular and cannot be inverted.
		if(abs(det) < 1e-9) {
			return *this;
		}
		
		Transpose();
		Mat3x3 adjoint = *this;
		adjoint.Adjoint();
		*this = adjoint / det;
		return *this;
	}
	
	// Matrix determinant
	X Det () const {
		return (u.Cross(v)).Dot(w);
	}
	
	// Matrix component addition
	Mat3x3 <X> operator+ (const Mat3x3 <X>& m) const {
		return Mat3x3 <X> (u + m.u, v + m.v, w + m.w);
	}
	
	// Matrix x Matrix multiplication
	Mat3x3 <X> operator* (const Mat3x3 <X>& m) const {
		return Mat3x3 <X> (
		Vec3 <X> (
			u.x * m.u.x + v.x * m.u.y + w.x * m.u.z,
			u.y * m.u.x + v.y * m.u.y + w.y * m.u.z,
			u.z * m.u.x + v.z * m.u.y + w.z * m.u.z),
		Vec3 <X> (
			u.x * m.v.x + v.x * m.v.y + w.x * m.v.z,
			u.y * m.v.x + v.y * m.v.y + w.y * m.v.z,
			u.z * m.v.x + v.z * m.v.y + w.z * m.v.z),
		Vec3 <X> (
			u.x * m.w.x + v.x * m.w.y + w.x * m.w.z,
			u.y * m.w.x + v.y * m.w.y + w.y * m.w.z,
			u.z * m.w.x + v.z * m.w.y + w.z * m.w.z));
	}
	
	Mat3x3 <X>& operator+= (const Mat3x3 <X>& m) {
		u += m.u;
		v += m.v;
		w += m.w;
		return *this;
	}
	
	Mat3x3 <X>& operator-= (const Mat3x3 <X>& m) {
		u -= m.u;
		v -= m.v;
		w -= m.w;
		return *this;
	}
	
	Mat3x3 <X>& operator*= (const Mat3x3 <X>& m) {
		*this = (*this) * m;
		return *this;
	}
	
	Mat3x3 <X> operator* (double f) const {
		return Mat3x3 <X> (f * u, f * v, f * w);
	}
	
	Mat3x3 <X> operator/ (double f) const {
		return Mat3x3 <X> (u / f, v / f, w / f);
	}
	
	// Component scaling
	Mat3x3 <X>& operator*= (double f) {
		u *= f;
		v *= f;
		w *= f;
		return *this;
	}
	
	// Component inverse scaling
	Mat3x3 <X>& operator/ (double f) {
		u /= f;
		v /= f;
		w /= f;
		return *this;
	}
	
	X Trace () const {
		return u.x + v.y + w.z;
	}
	
	bool DiagonalDominant () const {
		return
			abs(v.x) + abs(w.x) <= abs(u.x) &&
			abs(v.y) + abs(w.y) <= abs(u.y) &&
			abs(v.z) + abs(w.z) <= abs(u.z);
	}
	
	bool StrictDiagonalDominant () const {
		return
			abs(v.x) + abs(w.x) < abs(u.x) &&
			abs(v.y) + abs(w.y) < abs(u.y) &&
			abs(v.z) + abs(w.z) < abs(u.z);
	}
	
	Vec3 <X> u;
	Vec3 <X> v;
	Vec3 <X> w;
};

template <typename X>
Mat3x3 <X> operator* (double f, Mat3x3 <X> m) {
	return Mat3x3 <X> (f * m.u, f * m.v, f * m.w);
}

template <typename X>
Mat3x3 <X> operator/ (double f, Mat3x3 <X> m) {
	return Mat3x3 <X> (m.u / f, m.v / f, m.w / f);
}

template <typename X>
Mat3x3 <X> IdentityMat3x3 () {
	return Mat3x3 <X> (Vec3 <X> (1, 0, 0), Vec3 <X> (0, 1, 0), Vec3 <X> (0, 0, 1));
}

template <typename X>
Mat3x3 <X> ZeroMat3x3 () {
	return Mat3x3 <X> (Vec3 <X> (0, 0, 0), Vec3 <X> (0, 0, 0), Vec3 <X> (0, 0, 0));
}

template <typename X>
Mat3x3 <X> FilledMat3x3 (X e) {
	return Mat3x3 <X> (Vec3 <X> (e, e, e), Vec3 <X> (e, e, e), Vec3 <X> (e, e, e));
}

// Create a rotation matrix. It is an orthonormal linear transformation, so the resulting 3 matrix
// columns (and rows) are pairwise orthogonal and unit size. (0,0,0) will give the identity matrix.
Mat3x3 <double> RotationMat3x3 (double yaw, double pitch, double roll) {
	CosSinVals m(yaw, pitch, roll);
	Vec3d forw(m.cv * m.ca, m.cv * m.sa, m.sv);
	Vec3d side(-m.sa, m.ca, 0);
	Vec3d up(-m.sv * m.ca, -m.sv * m.sa, m.cv);
	return Mat3x3 <double> (forw, m.cr * side - m.sr * up, m.cr * up + m.sr * side);
}

Mat3x3 <double> RotationMat3x3noRoll (double yaw, double pitch) {
	CosSinVals m(yaw, pitch);
	Vec3d forw(m.cv * m.ca, m.cv * m.sa, m.sv);
	Vec3d side(-m.sa, m.ca, 0);
	Vec3d up(-m.sv * m.ca, -m.sv * m.sa, m.cv);
	return Mat3x3 <double> (forw, side, up);
}

using Mat3x3f = Mat3x3 <float>;
using Mat3x3d = Mat3x3 <double>;

template <typename X>
struct Quaternion {
	double Length () const {
		return sqrt(q * q + v.x * v.x + v.y * v.y + v.z * v.z);
	}
	
	double LengthSquared () const {
		return q * q + v.x * v.x + v.y * v.y + v.z * v.z;
	}
	
	X Dot (const Quaternion <X>& r) const {
		return q * r.q + v.x * r.v.x + v.y * r.v.y + v.z * r.v.z;
	}
	
	Quaternion <X> operator* (const Quaternion <X>& r) const {
		return Quaternion <X> (q * r.q - v.v.Dot(v), q * r.v + r.q * v + v.Cross(r.v));
	}
	
	Quaternion <X> operator* (double f) const {
		return Quaternion <X> (f * q, f * v);
	}
	
	Quaternion <X> operator/ (double f) const {
		return Quaternion <X> (q / f, v / f);
	}
	
	Quaternion <X> operator+ (const Quaternion <X>& r) const {
		return Quaternion <X> (q + r.q, v + r.v);
	}
	
	Quaternion <X> operator- (const Quaternion <X>& r) const {
		return Quaternion <X> (q - r.q, v - r.v);
	}
	
	// Storage in scalar + vector format.
	X q;
	Vec3 <X> v;
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

using Quatf = Quaternion <float>;
using Quatd = Quaternion <double>;

// Conversions back to radiants
struct VectorToRad {
	
	static constexpr double sin_extreme = 0.5 - 1e9;
	
	template <typename X>
	void FromQuat (Quaternion <X> r) {
		yaw_rad		= 0;
		pitch_rad	= 0;
		roll_rad	= 0;
		
		auto& q = r.q;
		auto& v = r.v;
		auto sinvang_2 = v.x * v.z - q * v.y;
		
		if(sin_extreme < sinvang_2) {
			pitch_rad	= -90;
			roll_rad	= 0;
			yaw_rad		= atan2(
				q * q - v.x * v.x + v.y * v.y - v.z * v.z,
				2 * (v.x * v.y - q * v.z)) - SpaceConst::PiHalf();
		}
		
		else if(sinvang_2 < -sin_extreme) {
			roll_rad	= 0;
			pitch_rad	= 90;
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
				q2_m_y2 + z2_m_x2);
			yaw_rad		= atan2(
				2 * (v.x * v.y + v.z * q),
				q2_m_y2 - z2_m_x2);
		}
	}
	
	double yaw_rad;
	double pitch_rad;
	double roll_rad;
};

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
