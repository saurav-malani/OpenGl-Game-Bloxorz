Game : main.cpp wm.cpp gc.cpp 3DObject.cpp sc.cpp Camera.cpp Grid.cpp Cube.cpp Text.cpp
	g++ main.cpp wm.cpp sc.cpp gc.cpp 3DObject.cpp Camera.cpp Grid.cpp Cube.cpp Text.cpp -std=c++11 -o main -lGL -lglfw -lGLEW -I./include/ -L./lib -lSOIL -lassimp
