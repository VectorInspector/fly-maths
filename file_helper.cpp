#include "file_helper.h"
#include <fstream>
#include <sstream>

bool SlurpByteFile (std::vector <char>& m, std::string path) {
	std::ifstream f(path, std::ios::binary);
	
	if(f.good()) {
		f.seekg(0, std::ios::end);
		std::size_t size = f.tellg();
		f.seekg(std::ios::beg);
		
		m.resize(size);
		f.read(m.data(), size);
		return true;
	}
	
	return false;
}

void SlurpTextFile (const std::string& path, std::string& s) {
	std::stringstream ss;
	std::ifstream f(path);
	
	if(f.good()) {
		ss << f.rdbuf();
		s = ss.str();
	}
}