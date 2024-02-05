#ifndef MAT3X3_H
#define MAT3X3_H

#include "vec3.h"

template <typename X>
struct Mat3x3 {
	Mat3x3& Transpose ();
	
	// Form the matrix of co-determinants.
	Mat3x3& Adjoint ();
	
	bool IsSingular () const;
	
	// Inverts a matrix if possible.
	Mat3x3& Invert ();
	
	// Matrix determinant
	X Det () const;
	
	// Matrix component addition
	Mat3x3 <X> operator+ (const Mat3x3 <X>& m) const;
	
	// Matrix x Matrix multiplication
	Mat3x3 <X> operator* (const Mat3x3 <X>& m) const;
	
	Mat3x3 <X>& operator+= (const Mat3x3 <X>& m);
	
	Mat3x3 <X>& operator-= (const Mat3x3 <X>& m);
	
	Mat3x3 <X>& operator*= (const Mat3x3 <X>& m);
	
	Mat3x3 <X> operator* (double f) const;
	
	Mat3x3 <X> operator/ (double f) const;
	
	// Component scaling
	Mat3x3 <X>& operator*= (double f);
	
	// Component inverse scaling
	Mat3x3 <X>& operator/ (double f);
	
	X Trace () const;
	
	bool DiagonalDominant () const;
	
	bool StrictDiagonalDominant () const;
	
	Vec3 <X> u;
	Vec3 <X> v;
	Vec3 <X> w;
};

template <typename X>
Mat3x3 <X> operator* (double f, Mat3x3 <X> m);

template <typename X>
Mat3x3 <X> operator/ (double f, Mat3x3 <X> m);

template <typename X>
Mat3x3 <X> IdentityMat3x3 ();

template <typename X>
Mat3x3 <X> ZeroMat3x3 ();

template <typename X>
Mat3x3 <X> FilledMat3x3 (X e);

// Create a rotation matrix. It is an orthonormal linear transformation, so the resulting 3 matrix
// columns (and rows) are pairwise orthogonal and unit size. (0,0,0) will give the identity matrix.
template <typename X>
Mat3x3 <X> RotationMat3x3 (double yaw, double pitch, double roll);

template <typename X>
Mat3x3 <X> RotationMat3x3noRoll (double yaw, double pitch);

using Mat3x3f = Mat3x3 <float>;
using Mat3x3d = Mat3x3 <double>;

#endif
