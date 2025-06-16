#include <thread>

// Turn off GL Deprecation warnings
#define GL_SILENCE_DEPRECATION

// Settings 
#define PORT "/dev/USB0"   // macOS UART for RPLIDAR
#define SCALE 0.1   // Distance scale for screen
#define SCREENX 800 // Size of Display window
#define SCREENY 500 
#define STARTX 100  // Coordinates to place window
#define STARTY 100

// Import Headers
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "include/rplidar.h" //RPLIDAR sdk
#include <signal.h>

// Function to flag ctrl-c
bool ctrl_c_pressed;
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

void point_polar(float theta, float r, int offsetx, int offsety, float size, color c)
{
    int x = r * cos(theta) + offsetx;
    int y = r * sin(theta) + offsety;
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

/*
 * LIDAR HELPERS - from Slamtec LIDAR SDK example
 */

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

#include <unistd.h>
static inline void delay(_word_size_t ms){
    while (ms>=1000){
        usleep(1000*1000);
        ms-=1000;
    };
    if (ms!=0)
        usleep(ms*1000);
}

int task();

/*
 * MAIN FUNCTIONS
 */

// GLOBALS 
u_result     op_result;

// RENDER - Pull data from LIDAR and render to display
void renderScreen(void)
{
    glClear (GL_COLOR_BUFFER_BIT);
    static float theta = 0.0;
    static float dist = 0.1;
    int quality = 0;

    // Fetch data from LIDAR
    //rplidar_response_measurement_node_hq_t nodes[8192];
    //size_t   count = _countof(nodes);

    //op_result = drv->grabScanDataHq(nodes, count);
    int count = 10;
    //if (IS_OK(op_result))
    {
        for (int pos = 0; pos < (int)count ; ++pos)
        {
            //theta = (360-((nodes[pos].angle_z_q14 * 90.f / (1 << 14)))/360)*2*M_PI;
            //dist = (nodes[pos].dist_mm_q2/4.0f)*SCALE;
            //quality = nodes[pos].quality;
            theta += 0.1;
            if(theta >= 360.0)theta=0;;
            dist  = 100;
            quality = 0;
            // Display
            if(quality == 0) {
                point_polar(theta, dist, SCREENX/2, SCREENY/2, 7, RED);
            }
            else {
                point_polar(theta, dist, SCREENX/2, SCREENY/2, 3, BLUE);
            }
            // printf("\r > Data: %f %f %d      ",theta,dist,quality);
        }
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

