#ifndef GL_HELPER
#define GL_HELPER

#include "file_helper.h"
#include "lodepng.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

struct PngFile {
	
	PngFile (std::string path) {
		data	= nullptr;
		width	= 0;
		height	= 0;
		lodepng_decode24_file(&data, &width, &height, path.c_str());
	}
	
	~PngFile () {
		if(data) {
			delete [] data;
		}
	}
	
	unsigned char* data;
	unsigned width;
	unsigned height;
};

// Contains a few open-gl context related infos for loading shaders, making programs and reading
// error logs and the like.
struct GlFuncs {
	
	// Load multiple png's as an open-gl cubemap.
	GLuint LoadCubeMap (std::vector <std::string>&& paths) {
		GLuint texture = 0;
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		
		int k = 0;
		
		for(const auto& s: paths) {
			PngFile png(s);
			
			if(png.data) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + k, 0, GL_RGB, png.width, png.height, 0, GL_RGB, GL_UNSIGNED_BYTE, png.data);
			}
			
			k++;
		}
		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		return texture;
	}
	
	// Load a png as an open-gl texture.
	GLuint LoadTexture (std::string path) {
		GLuint texture = 0;
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		PngFile png(path);
		
		if(png.data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, png.width, png.height, 0, GL_RGB, GL_UNSIGNED_BYTE, png.data);
		}
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		return texture;
	}
	
	// A complete process of loading shader source files, compiling them, attaching them to a newly
	// generated program and linking it.
	struct LoadProgramParam {
		std::string shader_path;
		GLint shader_enum;
	};
	
	GLuint LoadProgram (std::vector <LoadProgramParam> shaders_to_load) {
		std::vector <GLuint> shaders;
		
		for(const auto& s: shaders_to_load) {
			auto shader = LoadShaderFile(s.shader_path, s.shader_enum);
			
			if(!is_shader_compile_good) {
				std::cout << "Shader compile error (" << s.shader_path << "): " << shader_info_log << std::endl;
			}
			
			else {
				shaders.push_back(shader);
			}
		}
		
		return MakeProgram(shaders);
	}
	
	GLuint LoadShaderFile (const std::string& path, GLint shader_mode) {
		GLuint shader = glCreateShader(shader_mode);
		std::string shader_source;
		
		SlurpTextFile(path, shader_source);
		char const* temp_ptr = shader_source.c_str();
		glShaderSource(shader, 1, &temp_ptr, nullptr);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &is_shader_compile_good);
		glGetShaderInfoLog(shader, log_buffer_size, nullptr, shader_info_log);
		
		return shader;
	}
	
	GLuint MakeProgram (std::vector <GLuint> shaders) {
		GLuint program = glCreateProgram();
		
		for(auto shader: shaders) {
			glAttachShader(program, shader);
		}
		
		glLinkProgram(program);
		return program;
	}
	
	static constexpr std::size_t log_buffer_size = 0x400;
	char shader_info_log [log_buffer_size];
	char program_info_log [log_buffer_size];
	int is_shader_compile_good;
	int is_program_link_good;
};

struct GlModelFuncs {
	void MakeCube (std::vector <GLfloat>& m) {
		m = std::vector <GLfloat> {
			0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0,
			1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1,
			1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1,
			0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0,
			1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0,
			1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
		};
	}
	
	void MakePlane (std::vector <GLfloat>& m) {
		m = std::vector <GLfloat> {
			0, 0, 0,
			1, 0, 0,
			1, 1, 0,
			0, 1, 0
		};
	}
	
	void Make2dQuadTris (std::vector <GLfloat>& m) {
		m = std::vector <GLfloat> {
			0, 0, 1, 0, 0, 1,
			1, 0, 1, 1, 0, 1
		};
	}
	
	void Make2dArrow (std::vector <GLfloat>& m) {
		m = std::vector <GLfloat> {
			0, 0, 1, 0, 0.5, -0.5, 1, 0, 0.5, 0.5
		};
	}
	
	void MakeTriGrid (std::vector <GLfloat>& m, int x_size, int y_size) {
		x_size = std::max(1, x_size);
		y_size = std::max(1, y_size);
		
		auto fx = 1.0 / x_size;
		auto fy = 1.0 / y_size;
		float cur_x = 0;
		
		for(int x = 0; x < x_size; x++) {
			float cur_y = 0;
			
			for(int y = 0; y < y_size; y++) {
				m.push_back(cur_x);
				m.push_back(cur_y);
				m.push_back(0);
				
				m.push_back(cur_x + fx);
				m.push_back(cur_y);
				m.push_back(0);
				
				m.push_back(cur_x);
				m.push_back(cur_y + fy);
				m.push_back(0);
				
				m.push_back(cur_x + fx);
				m.push_back(cur_y);
				m.push_back(0);
				
				m.push_back(cur_x + fx);
				m.push_back(cur_y + fy);
				m.push_back(0);
				
				m.push_back(cur_x);
				m.push_back(cur_y + fy);
				m.push_back(0);
				
				cur_y += fy;
			}
			
			cur_x += fx;
		}
	}
	
	void Rescale2dModel (std::vector <GLfloat>& m, GLfloat size = 1.0, GLfloat fx = 0.5, GLfloat fy = 0.5) {
		for(int k = 0; k < m.size(); k += 2) {
			m [0 + k] -= fx;
			m [1 + k] -= fy;
			
			m [0 + k] *= size;
			m [1 + k] *= size;
		}
	}
	
	void RescaleModel (std::vector <GLfloat>& m, GLfloat size = 1.0, GLfloat fx = 0.5, GLfloat fy = 0.5, GLfloat fz = 0.5) {
		for(int k = 0; k < m.size(); k += 3) {
			m [0 + k] -= fx;
			m [1 + k] -= fy;
			m [2 + k] -= fz;
			
			m [0 + k] *= size;
			m [1 + k] *= size;
			m [2 + k] *= size;
		}
	}
};

#endif
