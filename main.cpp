#include "space.cpp"
#include <iostream>

int main (int argc, char * argv []) {
	Mat3x3d m = RotationMat3x3(0, 0, 0);
	Mat3x3d n = IdentityMat3x3 <double> ();
	std::string buffer;
	
	n.u *= 2;
	n.v *= 3;
	n.w *= 4;
	
	m = m * n * n;
	
	Mat3x3toString(buffer, m);
	std::cout << buffer << std::endl;
	
	auto m_inv = m;
	m_inv.Invert();
	
	auto identity = m_inv * m;
	Mat3x3toString(buffer, identity);
	std::cout << buffer << std::endl;
	
	return 0;
}
