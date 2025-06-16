# MAKEFILE
#
# This makefile builds an OpenGL based LIDAR display for the Slamtec RPLIDAR 
# device.  It requires the Slamtec rplidar sdk libraries and headers which
# can be built with the build.sh script.
#
# Jason Cox, @jasonacox
#   https://github.com/jasonacox/OpenGL-LIDAR-Display 
#
# 'make'        build executable file 'lidar'
# 'make clean'  removes all .o and executable files
#
# Manual: 
# 	gcc lidar.cpp librplidar_sdk.a -o lidar -framework OpenGL -framework GLUT -lstdc++ -lpthread

# define the C compiler to use
CC = g++

# define any compile-time flags
CFLAGS = -Wall -ggdb -O0 -std=c++11

# define any directories containing header files other than /usr/include
INCLUDES = -I./include -I./rplidar_sdk/sdk/src/  #-I./include/hal

# define library paths 
LFLAGS = ./lib/libsl_lidar_sdk.a

# define any libraries and frameworks to link into executable:
#LIBS = -framework OpenGL -framework GLUT -lstdc++ -lpthread 
#LIBS = `pkg-config --static --libs glfw3 opengl ` -lm -lstdc++ -lglut -lsl_lidar_sdk -Llib -lglfw
LIBS = `pkg-config --static --libs glfw3 opengl ` -lm -lstdc++ -lglut -Llib -lglfw -lpthread

# define the C source files
SRCS = lidar.c \
./rplidar_sdk/sdk/src/sl_crc.cpp \
./rplidar_sdk/sdk/src/sl_async_transceiver.cpp \
./rplidar_sdk/sdk/src/sl_lidar_driver.cpp \
./rplidar_sdk/sdk/src/sl_serial_channel.cpp \
./rplidar_sdk/sdk/src/dataunpacker/dataunpacker.cpp \
./rplidar_sdk/sdk/src/dataunpacker/unpacker/handler_capsules.cpp \
./rplidar_sdk/sdk/src/dataunpacker/unpacker/handler_hqnode.cpp \
./rplidar_sdk/sdk/src/dataunpacker/unpacker/handler_normalnode.cpp \
./rplidar_sdk/sdk/src/hal/thread.cpp \
./rplidar_sdk/sdk/src/sl_udp_channel.cpp \
./rplidar_sdk/sdk/src/arch/linux/timer.cpp \
./rplidar_sdk/sdk/src/arch/linux/net_serial.cpp \
./rplidar_sdk/sdk/src/arch/linux/net_socket.cpp \
./rplidar_sdk/sdk/src/sl_tcp_channel.cpp \
./rplidar_sdk/sdk/src/rplidar_driver.cpp \
./rplidar_sdk/sdk/src/sl_lidarprotocol_codec.cpp

# define the C object files 
OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = lidar

# make
all:    $(MAIN)
	@echo
	@echo "OpenGL LIDAR Display built - see ./bin/lidar"
	@cp lidar ./bin

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	@rm -fr rplidar_sdk-release*
	@rm -fr lidar
	@rm -fr lidar.o
