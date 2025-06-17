#include <thread>

// Turn off GL Deprecation warnings
#define GL_SILENCE_DEPRECATION

// Settings 
#define PORT "/dev/USB0"   // macOS UART for RPLIDAR
#define SCALE 1   // Distance scale for screen
#define SCREENX 1000 // Size of Display window
#define SCREENY 1000
#define STARTX 100  // Coordinates to place window
#define STARTY 100

// Import Headers
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "LD06Kit/util.hpp"


// Function to flag ctrl-c
bool ctrl_c_pressed = false;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}



/*
 * OPENGL HELPERS
 */

// Set up some colors
class color {
public:
    float r;
    float g;
    float b;
    color(float rV=0.0, float gV=0.0, float bV=0.0);
};

color::color( float rV, float gV, float bV)
{
    r = rV;
    g = gV;
    b = bV;
}
color RED(1.0,0.0,0.0);
color GREEN(0.0,1.0,0.0);
color BLUE(0.0,0.0,1.0);
color c(0.0,0.0,1.0);

void point_polar(float deg, float r, int offsetx, int offsety, float size, color c)
{
	float rad = (deg * 3.1415926) / 180;
    int x = r * cos(rad) + offsetx;
    int y = r * sin(rad) + offsety;
    glPointSize(size);
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
    // printf("point_polar: th=%f r=%f, %d, %d\n", theta, r, x, y);   
}

void line(int x1, int y1, int x2, int y2, float width, color c)
{
    glColor3f(c.r, c.g, c.b);
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2i(x1,y1);
    glVertex2i(x2,y2);
    glEnd();
}

int task();

/*
 * MAIN FUNCTIONS
 */

// GLOBALS 
extern LP_t lp[];

// RENDER - Pull data from LIDAR and render to display
void renderScreen(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int pos = 0; pos < (int)360 ; pos++)
    {
        if(lp[pos].confidence > 150)
        	c = BLUE;
        else
        	c = RED;
        point_polar(lp[pos].angle, lp[pos].distance, SCREENX/2, SCREENY/2, 2, c);
    }
    // Render
    glFlush();
    // Intercept request to stop and end gracefully
    if (ctrl_c_pressed){ 
        printf(" Received - Stopping - Shutting down LIDAR\n");
        exit(0);
    }
}

// MAIN
int main(int argc, char** argv)
{
    signal(SIGINT, ctrlc);
    // Initialize OpenGL and display
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE |GLUT_RGB);
    glutInitWindowSize (SCREENX,SCREENY);
    glutInitWindowPosition (STARTX, STARTY);
    glutCreateWindow ("OpenGL LIDAR Display");
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREENX, 0, SCREENY, 0, 1);
    glClear (GL_COLOR_BUFFER_BIT);

    std::thread t1(task);

    // Start scanning and render loop
    printf("Launching OpenGL Window\n");
    glutDisplayFunc(renderScreen);
    glutIdleFunc(renderScreen);
    // Enter continuous loop
    glutMainLoop();

    t1.join();
    return 0;
}

