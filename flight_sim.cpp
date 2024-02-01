#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

void SlurpTextFile (const std::string& path, std::string& s) {
	std::stringstream ss;
	std::ifstream f(path);
	
	if(f.good()) {
		ss << f.rdbuf();
		s = ss.str();
	}
}

// Contains a few open-gl context related infos for loading shaders, making programs and reading
// error logs and the like.
struct GlFuncs {
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

struct AppData {
	int timer;
	bool is_running;
	
	int cmd_arg_count;
	char** cmd_args;
	
	GLFWwindow* window;
	
	GLint shader_program;
	
	GLfloat model [16] = { 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8 };
	GlFuncs gl_funcs;
	GLuint gl_default_program;
};

struct AppLoop {
	AppLoop (AppData& app_data) : d(app_data) {
		glfwInit();
	}
	
	~AppLoop () {
		glfwTerminate();
	}
	
	int MainLoop () {
		d.timer = 0;
		d.is_running = true;
		
		while(d.is_running) {
			if(0 == d.timer) {
				OnFirstTick();
			}
			
			glfwPollEvents();
			
			if(glfwWindowShouldClose(d.window)) {
				d.is_running = false;
			}
			
			OnTick();
			
			glClearColor(0.43, 0.143, 0.3030, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			OnDraw();
			glfwSwapBuffers(d.window);
			
			d.timer++;
		}
		
		return 0;
	}
	
	void OnFirstTick () {
		glfwDefaultWindowHints();
		d.window = glfwCreateWindow(800, 600, "Flight Simulator", nullptr, nullptr);
		glfwMakeContextCurrent(d.window);
		gladLoadGL();
		
		// Any program is good for now. The empty program will display white triangles on many
		// systems by default.
		d.gl_default_program = d.gl_funcs.MakeProgram({});
		glUseProgram(d.gl_default_program);
	}
	
	void OnTick () {
	}
	
	void OnDraw () {
		glUseProgram(d.gl_default_program);
		// glBindVertexArray
		// glVertexAttribPointer
		// glDrawArrays(GL_LINES, 0, vertex_count)
	}
	
	AppData& d;
};

int main (int argc, char * argv []) {
	AppData data;
	AppLoop app(data);
	
	data.cmd_arg_count	= argc;
	data.cmd_args		= argv;
	return app.MainLoop();
}