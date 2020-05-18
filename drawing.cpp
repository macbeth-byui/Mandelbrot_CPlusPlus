#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include "mandelbrot.h"
#include "drawing.h"
#include "app.h"

using namespace std;

Mandelbrot *mandelbrot;

void drawPoint(float x, float y, int r, int g, int b)
{
   glBegin(GL_POINTS);
   glColor3f(r/255.0, g/255.0, b/255.0);
   glVertex2f(x,y);
   glEnd();
}

void drawCallback()
{
   if (mandelbrot->isRefreshed()) {
      glClear(GL_COLOR_BUFFER_BIT);
      mandelbrot->drawApp();
      glutSwapBuffers();
   }
}

void mouseCallback(int button, int state, int x, int y)
{
   if (state == 1) 
   {
      mandelbrot->zoom(x, WINDOW_HEIGHT-y, 0.5);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(mandelbrot->getXmin(), mandelbrot->getXmax(),
               mandelbrot->getYmin(), mandelbrot->getYmax()); 
      
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();              
      mandelbrot->calculateApp();
   }
}


void initGL(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
   glutInitWindowPosition(0,0);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   glutCreateWindow("Mandelbrot - C++");
   glClearColor(0,0,0,0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(INIT_VIRTUAL_GRID_XMIN, INIT_VIRTUAL_GRID_XMAX,
              INIT_VIRTUAL_GRID_YMIN, INIT_VIRTUAL_GRID_YMAX); // Set grid size
   
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();              
   glutDisplayFunc(drawCallback); // Run the drawing function
   glutMouseFunc(mouseCallback);
   glutIdleFunc(drawCallback);
}

int main(int argc, char **argv)
{
   mandelbrot = new Mandelbrot(INIT_VIRTUAL_GRID_XMIN, INIT_VIRTUAL_GRID_XMAX,
                        INIT_VIRTUAL_GRID_YMIN, INIT_VIRTUAL_GRID_YMAX,
                        WINDOW_HEIGHT, WINDOW_WIDTH);
   mandelbrot->calculateApp();
   initGL(argc, argv);
   glutMainLoop(); // Starts the infinite loop that draws the window
   return 0;
}
