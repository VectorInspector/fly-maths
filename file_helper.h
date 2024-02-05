#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#include <string>
#include <vector>

bool SlurpByteFile (std::vector <char>& m, std::string path);
void SlurpTextFile (const std::string& path, std::string& s);

#endif
