# define the C compiler to use
CC = g++

# define any compile-time flags
CFLAGS = -Wall -ggdb -O0 #-std=c++11

# define any directories containing header files other than /usr/include
INCLUDES = -I./include

# define library paths
LFLAGS =

# define any libraries and frameworks to link into executable:
LIBS = `pkg-config --static --libs glfw3 opengl ` -lm -lstdc++ -lglut -Llib -lglfw -lpthread

# define the C source files
#SRCS = src/lidartest.cpp src/lidarkit.cpp src/util.cpp
SRCS = lidar.cpp
SRCS += src/lidartest.cpp src/lidarkit.cpp src/util.cpp



all:
	#g++ src/lidartest.cpp src/lidarkit.cpp src/util.cpp -I./include/ -o lidar
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) -o lidar $(LFLAGS) $(LIBS)

clean:
	rm lidar
