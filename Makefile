glfw_dir = ../../progfun/glfw
glad_dir = ../../progfun/glad
zero_dirs = -L$(glfw_dir)/lib -I$(glfw_dir)/include -I$(glad_dir)/include -I../../progfun/lodepng

flightsim:
	g++ flight_sim.cpp space.cpp -o fs.exe -std=c++20 $(zero_dirs) -lopengl32 -lglfw3 -lgdi32 $(glad_dir)/src/glad.c

expl:
	g++ ../lodepng/lodepng.cpp explorer.cpp -o zero_explorer -L../../progfun/sdl/lib/64 -I../sdl/include/64 -I../lodepng
	zero_explorer

main:
	g++ main.cpp -o zerozero -std=c++20
	zerozero