#include "lodepng.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "gl_helper.cpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "mat3x3.h"
#include "vec3.h"
#include "vec2.h"
#include "quaternion.h"
#include "space.h"
#include "file_helper.h"

struct FlightInputs {
	
	void Tick (int k, bool cond) {
		auto& m = timers [k];
		
		if(cond) {
			if(m < 0) {
				m = 1;
			}
			
			else {
				m++;
			}
		}
		
		else {
			if(0 < m) {
				m = 0;
			}
			
			else {
				m--;
			}
		}
	}
	
	bool IsPressed (int k) const { return 1 == timers [k]; }
	bool IsDown (int k) const { return 1 <= timers [k]; }
	bool IsUp (int k) const { return timers [k] <= 0; }
	bool IsReleased (int k) const { return 0 == timers [k]; }
	
	int Dir (int u, int v) const {
		auto& x = timers [u];
		auto& y = timers [v];
		
		if(x == y) {
			return 0;
		}
		
		else if(x <= 0 && 0 < y) {
			return 1;
		}
		
		else if(y <= 0 && 0 < x) {
			return -1;
		}
		
		else if(x <= 0 && y <= 0) {
			return 0;
		}
		
		else if(x < y) {
			return -1;
		}
		
		else if(y < x) {
			return 1;
		}
		
		return 0;
	}
	
	int timers [16];
};

struct AppData {
	int timer;
	bool is_running;
	
	int cmd_arg_count;
	char** cmd_args;
	
	GLFWwindow* window;
	
	GLuint triangle_buffer;
	GLuint triangle_array;
	GLuint quad_buffer;
	GLuint quad_array;
	GLuint terrain_buffer;
	
	GLuint plane_array;
	GLuint plane_buffer;
	GLuint plane_uv_buffer;
	
	GLfloat quad [64] = {
		0, 0,	0, 0,
		1, 0,	1, 0,
		1, 1,	1, 1,
		
		0, 0,	0, 0,
		1, 1,	1, 1,
		0, 1,	0, 1,
		0
	};
	
	std::vector <GLfloat> cube_model;
	std::vector <GLfloat> plane_model;
	std::vector <GLfloat> terrain_model;
	
	GLfloat plane_uv [16] = {
		0, 0,
		1, 0,
		1, 1,
		0, 0,
		1, 1,
		0, 1,
		
		0
	};
	
	FlightInputs inputs;
	GlFuncs gl_funcs;
	
	GLuint vertex_3d_program;
	GLuint grid_3d_program;
	GLuint cubemap_3d_program;
	GLuint texture_2d_program;
	GLuint geom_program;
	GLuint skybox_cubemap;
	GLuint default_texture;
	
	double mouse_dx;
	double mouse_dy;
	double mouse_x_pos;
	double mouse_y_pos;
	
	int window_x_size;
	int window_y_size;
	
	Vec3f view_pos;
	Quatf view_quat;
	Quatf view_turn_quat;
	double view_rad_x;
	double view_rad_y;
	double view_rad_z;
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
		
		// Offset and scale a few models.
		GlModelFuncs model_funcs;
		model_funcs.MakeCube(d.cube_model);
		model_funcs.RescaleModel(d.cube_model, 1.0, 0.5, 0.5, 0.5);
		model_funcs.MakePlane(d.plane_model);
		model_funcs.RescaleModel(d.plane_model, 500, 0.5, 0.5, 0);
		model_funcs.MakeTriGrid(d.terrain_model, 10, 10);
		model_funcs.RescaleModel(d.terrain_model, 500, 0.5, 0.5, 0);
		
		// Initialise user data.
		d.window_x_size = 1280;
		d.window_y_size = 720;
		
		auto monitor = glfwGetPrimaryMonitor();
		auto monitor_x_size = d.window_x_size;
		auto monitor_y_size = d.window_y_size;
		
		if(monitor) {
			auto video_mode = glfwGetVideoMode(monitor);
			
			if(video_mode) {
				monitor_x_size = video_mode->width;
				monitor_y_size = video_mode->height;
			}
		}
		
		auto window_x_pos = 0.5 * (monitor_x_size - d.window_x_size);
		auto window_y_pos = 0.5 * (monitor_y_size - d.window_y_size);
		
		// Initialise glfw.
		glfwDefaultWindowHints();
		d.window = glfwCreateWindow(d.window_x_size, d.window_y_size, "Flight Simulator", nullptr, nullptr);
		glfwSetWindowPos(d.window, window_x_pos, window_y_pos);
		glfwMakeContextCurrent(d.window);
		gladLoadGL();
		glfwSetInputMode(d.window, GLFW_STICKY_KEYS, GLFW_TRUE);
		glfwSetInputMode(d.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		glfwSetInputMode(d.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		
		// Player position.
		d.view_pos = Vec3f(-10, 0, 10);
		d.view_quat = IdentityQuat <float> ();
		d.view_turn_quat = d.view_quat;
		d.view_rad_x = 0;
		d.view_rad_y = 0;
		d.view_rad_z = 0;
		
		// Initial mouse state.
		glfwGetCursorPos(d.window, &d.mouse_x_pos, &d.mouse_y_pos);
		
		// A few open-gl settings.
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		
		// Create a few open-gl programs for rendering models.
		d.vertex_3d_program = d.gl_funcs.LoadProgram( {
			{ "shader_3d_vertex.txt", GL_VERTEX_SHADER },
			{ "shader_3d_fragment.txt", GL_FRAGMENT_SHADER } });
			
		d.grid_3d_program = d.gl_funcs.LoadProgram( {
			{ "shader_3d_vertex.txt", GL_VERTEX_SHADER },
			{ "shader_3d_grid.txt", GL_FRAGMENT_SHADER } });
			
		d.cubemap_3d_program = d.gl_funcs.LoadProgram( {
			{ "shader_3d_vertex.txt", GL_VERTEX_SHADER },
			{ "shader_3d_cubemap.txt", GL_FRAGMENT_SHADER } });
			
		d.texture_2d_program = d.gl_funcs.LoadProgram( {
			{ "shader_2d_vertex.txt", GL_VERTEX_SHADER },
			{ "shader_2d_texture.txt", GL_FRAGMENT_SHADER } });
		
		d.geom_program = d.gl_funcs.LoadProgram( {
			{ "shader_3d_vertex.txt", GL_VERTEX_SHADER },
			// { "shader_geom.txt", GL_GEOMETRY_SHADER },
			{ "shader_3d_fragment.txt", GL_FRAGMENT_SHADER } });
		
		// Upload models to open-gl.
		glGenVertexArrays(1, &d.triangle_array);
		glBindVertexArray(d.triangle_array);
		glGenBuffers(1, &d.triangle_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, d.triangle_buffer);
		glBufferData(GL_ARRAY_BUFFER, d.cube_model.size() * sizeof(GLfloat), d.cube_model.data(), GL_STATIC_DRAW);
		
		// Terrain.
		glGenBuffers(1, &d.terrain_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, d.terrain_buffer);
		glBufferData(GL_ARRAY_BUFFER, d.terrain_model.size() * sizeof(GLfloat), d.terrain_model.data(), GL_STATIC_DRAW);
		
		// Quad model.
		glGenVertexArrays(1, &d.quad_array);
		glBindVertexArray(d.quad_array);
		glGenBuffers(1, &d.quad_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, d.quad_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(d.quad), d.quad, GL_STATIC_DRAW);
		
		// Second model.
		glGenVertexArrays(1, &d.plane_array);
		glBindVertexArray(d.plane_array);
		glGenBuffers(1, &d.plane_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, d.plane_buffer);
		glBufferData(GL_ARRAY_BUFFER, d.cube_model.size() * sizeof(GLfloat), d.plane_model.data(), GL_STATIC_DRAW);
		glGenBuffers(1, &d.plane_uv_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, d.plane_uv_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(d.plane_uv), d.plane_uv, GL_STATIC_DRAW);
		
		// Load a cubemap.
		d.skybox_cubemap = d.gl_funcs.LoadCubeMap( {
			"skybox_back.png",
			"skybox_front.png",
			"skybox_top.png",
			"skybox_bottom.png",
			"skybox_left.png",
			"skybox_right.png"
		});
		
		d.default_texture = d.gl_funcs.LoadTexture("skybox_top.png");
	}
	
	void OnTick () {
		// Get inputs for mouse.
		{
			double new_mouse_x = 0;
			double new_mouse_y = 0;
			glfwGetCursorPos(d.window, &new_mouse_x, &new_mouse_y);
			
			d.mouse_dx = new_mouse_x - d.mouse_x_pos;
			d.mouse_dy = new_mouse_y - d.mouse_y_pos;
			d.mouse_x_pos = new_mouse_x;
			d.mouse_y_pos = new_mouse_y;
		}
		
		// Inputs for movement.
		{
			d.inputs.Tick(0, glfwGetKey(d.window, GLFW_KEY_W) || glfwGetKey(d.window, GLFW_KEY_UP));
			d.inputs.Tick(1, glfwGetKey(d.window, GLFW_KEY_S) || glfwGetKey(d.window, GLFW_KEY_DOWN));
			d.inputs.Tick(2, glfwGetKey(d.window, GLFW_KEY_A) || glfwGetKey(d.window, GLFW_KEY_LEFT));
			d.inputs.Tick(3, glfwGetKey(d.window, GLFW_KEY_D) || glfwGetKey(d.window, GLFW_KEY_RIGHT));
		}
		
		// Quaternion based 6DOF flight. This rotates your view.
		{
			auto fx = 0.0125 * d.mouse_dx / 2;
			auto fy = 0.0125 * d.mouse_dy / 2;
			auto f = 0.925; // Braking factor for view makes turning smoother.
		
			Quatf turn_quat = RadQuaternion <float> (fx, fy, 0);
			d.view_turn_quat = f * d.view_turn_quat + (1.0 - f) * turn_quat;
			d.view_quat = (d.view_quat * d.view_turn_quat).Unit1();
		}
		
		// Movement through space. Quaternion to matrix conversion is possible by converting the
		// quaternion into angles, and making a rotation matrix from the angles. The local matrix
		// axes are your local movement axes too!
		VectorToRad vtr;
		vtr.FromQuat(d.view_quat);
		Mat3x3f view = RotationMat3x3 <float> (vtr.yaw_rad, vtr.pitch_rad, vtr.roll_rad);
		d.view_pos += 0.75 * (d.inputs.Dir(1, 0) * view.u + d.inputs.Dir(2, 3) * view.v);
		
		// Notify the shader programs of the changed view point.
		// Skybox / cube map:
		Vec3f origin(0, 0, 0);
		
		glUseProgram(d.cubemap_3d_program);
		glUniform3fv(0, 1, reinterpret_cast <GLfloat*> (&origin));
		glUniformMatrix3fv(1, 1, true, reinterpret_cast <GLfloat*> (&view));
		glUniform1f(3, 1);
		
		// World objects.
		glUseProgram(d.vertex_3d_program);
		glUniform3fv(0, 1, reinterpret_cast <GLfloat*> (&d.view_pos));
		glUniformMatrix3fv(1, 1, true, reinterpret_cast <GLfloat*> (&view));
		glUniform1f(3, 1);
		
		glUseProgram(d.geom_program);
		glUniform3fv(0, 1, reinterpret_cast <GLfloat*> (&d.view_pos));
		glUniformMatrix3fv(1, 1, true, reinterpret_cast <GLfloat*> (&view));
		glUniform1f(3, 1);
		
		// The grid object.
		glUseProgram(d.grid_3d_program);
		glUniform3fv(0, 1, reinterpret_cast <GLfloat*> (&d.view_pos));
		glUniformMatrix3fv(1, 1, true, reinterpret_cast <GLfloat*> (&view));
		glUniform1f(3, 1);
	}
	
	void OnDraw () {
		
		glDepthMask(GL_FALSE);
		glUseProgram(d.cubemap_3d_program);
		glBindTexture(GL_TEXTURE_CUBE_MAP, d.skybox_cubemap);
		glBindTexture(GL_TEXTURE_2D, d.default_texture);
		glBindVertexArray(d.triangle_array);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, d.triangle_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glDrawArrays(GL_QUADS, 0, 3 * 6 * 6);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDepthMask(GL_TRUE);
		
		/*glUseProgram(d.geom_program);
		glBindVertexArray(d.triangle_array);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, d.terrain_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glDrawArrays(GL_LINES, 0, d.terrain_model.size() / 3);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);*/
		
		
		glUseProgram(d.grid_3d_program);
		glUniform1f(3, 1);
		glBindVertexArray(d.plane_array);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, d.plane_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, d.plane_uv_buffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glDrawArrays(GL_QUADS, 0, 6);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		
		/*
		glUseProgram(d.vertex_3d_program);
		glUniform3f(2, 0, 0, 5);
		glUniform1f(3, -25);
		glBindVertexArray(d.triangle_array);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, d.triangle_buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		glDrawArrays(GL_QUADS, 0, 3 * 6 * 6);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		*/
		
		/*glDepthMask(GL_FALSE);
		glUseProgram(d.texture_2d_program);
		glBindTexture(GL_TEXTURE_2D, d.default_texture);
		glBindVertexArray(d.quad_array);
		glBindBuffer(GL_ARRAY_BUFFER, d.quad_buffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
		glDrawArrays(GL_TRIANGLES, 0, 6 * 2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDepthMask(GL_TRUE);*/
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