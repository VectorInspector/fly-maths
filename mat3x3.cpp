#include "mat3x3.h"

template <typename X>
Mat3x3 <X>& Mat3x3 <X>::Transpose () {
	using std::swap;
	swap(u.y, v.x);
	swap(u.z, w.x);
	swap(v.z, w.y);
	return *this;
}

// Form the matrix of co-determinants.
template <typename X>
Mat3x3 <X>& Mat3x3 <X>::Adjoint () {
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

template <typename X>
bool Mat3x3 <X>::IsSingular () const {
	return abs(Det()) < 1e-9;
}

// Inverts a matrix if possible.
template <typename X>
Mat3x3 <X>& Mat3x3 <X>::Invert () {
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
template <typename X>
X Mat3x3 <X>::Det () const {
	return (u.Cross(v)).Dot(w);
}

// Matrix component addition
template <typename X>
Mat3x3 <X> Mat3x3 <X>::operator+ (const Mat3x3 <X>& m) const {
	return Mat3x3 <X> (u + m.u, v + m.v, w + m.w);
}

// Matrix x Matrix multiplication
template <typename X>
Mat3x3 <X> Mat3x3 <X>::operator* (const Mat3x3 <X>& m) const {
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

template <typename X>
Mat3x3 <X>& Mat3x3 <X>::operator+= (const Mat3x3 <X>& m) {
	u += m.u;
	v += m.v;
	w += m.w;
	return *this;
}

template <typename X>
Mat3x3 <X>& Mat3x3 <X>::operator-= (const Mat3x3 <X>& m) {
	u -= m.u;
	v -= m.v;
	w -= m.w;
	return *this;
}

template <typename X>
Mat3x3 <X>& Mat3x3 <X>::operator*= (const Mat3x3 <X>& m) {
	*this = (*this) * m;
	return *this;
}

template <typename X>
Mat3x3 <X> Mat3x3 <X>::operator* (double f) const {
	return Mat3x3 <X> (f * u, f * v, f * w);
}

template <typename X>
Mat3x3 <X> Mat3x3 <X>::operator/ (double f) const {
	return Mat3x3 <X> (u / f, v / f, w / f);
}

// Component scaling
template <typename X>
Mat3x3 <X>& Mat3x3 <X>::operator*= (double f) {
	u *= f;
	v *= f;
	w *= f;
	return *this;
}

// Component inverse scaling
template <typename X>
Mat3x3 <X>& Mat3x3 <X>::operator/ (double f) {
	u /= f;
	v /= f;
	w /= f;
	return *this;
}

template <typename X>
X Mat3x3 <X>::Trace () const {
	return u.x + v.y + w.z;
}

template <typename X>
bool Mat3x3 <X>::DiagonalDominant () const {
	return
		abs(v.x) + abs(w.x) <= abs(u.x) &&
		abs(v.y) + abs(w.y) <= abs(u.y) &&
		abs(v.z) + abs(w.z) <= abs(u.z);
}

template <typename X>
bool Mat3x3 <X>::StrictDiagonalDominant () const {
	return
		abs(v.x) + abs(w.x) < abs(u.x) &&
		abs(v.y) + abs(w.y) < abs(u.y) &&
		abs(v.z) + abs(w.z) < abs(u.z);
}

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
template <typename X>
Mat3x3 <X> RotationMat3x3 (double yaw, double pitch, double roll) {
	CosSinVals m(yaw, pitch, roll);
	Vec3 <X> forw(m.cv * m.ca, m.cv * m.sa, m.sv);
	Vec3 <X> side(-m.sa, m.ca, 0);
	Vec3 <X> up(-m.sv * m.ca, -m.sv * m.sa, m.cv);
	return Mat3x3 <X> (forw, m.cr * side - m.sr * up, m.cr * up + m.sr * side);
}

template <typename X>
Mat3x3 <X> RotationMat3x3noRoll (double yaw, double pitch) {
	CosSinVals m(yaw, pitch);
	Vec3 <X> forw(m.cv * m.ca, m.cv * m.sa, m.sv);
	Vec3 <X> side(-m.sa, m.ca, 0);
	Vec3 <X> up(-m.sv * m.ca, -m.sv * m.sa, m.cv);
	return Mat3x3 <X> (forw, side, up);
}