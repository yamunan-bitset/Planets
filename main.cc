/* To compile:
 * g++ main.cc -lGL -lGLU -lglut -lX11
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <X11/Xlib.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

float xrot, yrot, zrot, radius;

void timer(int time)
{
  // Correct z and y rotations
  if (zrot > 360.0) zrot -= 360.0 * std::floor(zrot / 360.0);
  if (yrot > 360.0) yrot -= 360.0 * std::floor(yrot / 360.0);
  zrot += 5.0;
  yrot += 1.0;
  glutTimerFunc(16, timer, 0);
  glutPostRedisplay();
}

void init()
{
  xrot = 0.0f; yrot = 0.0f; zrot = 0.0f; radius = 3.0f;
}

void render()
{
  //  std::srand(std::time(NULL));
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, 1000, 0, 1000, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // Random 100 points
  glPushMatrix();
  {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(4);
    glBegin(GL_POINTS);
    for (unsigned i = 0; i < 500; i++)
      {
	int x = std::rand() % 1000;
	int y = std::rand() % 1000;
	glVertex2i(x, y);
      }
    glEnd();
  }
  glPopMatrix();
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);

  // Enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

  // 3D rendering
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  double w = glutGet(GLUT_WINDOW_WIDTH);
  double h = glutGet(GLUT_WINDOW_HEIGHT);
  gluPerspective(20.0, w / h, 0.1, 80.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -40.0);

  glPushMatrix();
  {
    GLfloat qaLightPosition[] = { 1, 1, 1, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    GLfloat emitLight[] = {0.9, 0.9, 0.9, 0.9};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emitLight);
    glutSolidSphere(radius, 25, 25);
    GLfloat Noemit[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Noemit);
  }
  glPopMatrix();

  glPushMatrix();
  {
    glRotatef(yrot, 0.0, 2.0, 0.0);
    glTranslatef(5.0, 0.0, 0.0);
    glutSolidSphere(radius / 6, 25, 25);
  }
  glPopMatrix();
  glutSwapBuffers();
}

int main(int argc, char** argv)
{
  Display* dpy = XOpenDisplay(NULL);
  XWindowAttributes ra;
  XGetWindowAttributes(dpy, DefaultRootWindow(dpy), &ra);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(ra.width, ra.height);
  init();
  glutCreateWindow("Planets");
  glutDisplayFunc(render);
  glutTimerFunc(0, timer, 0);
  glutMainLoop();
  return 0;
}
