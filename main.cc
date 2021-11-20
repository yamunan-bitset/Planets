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
float angle = 0.0f;
float lx = 0.0f,lz = -1.0f;
float x = 0.0f, z = 5.0f;
float deltaAngle = 0.0f;
float deltaMove = 0;

void computePos(float deltaMove)
{
  x += deltaMove * lx * 0.1f;
  z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle)
{
  angle += deltaAngle;
  lx = sin(angle);
  lz = -cos(angle);
}

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
  if (deltaMove)  computePos(deltaMove);
  if (deltaAngle) computeDir(deltaAngle);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_DEPTH_TEST);
  gluLookAt(    x, 1.0f, z,
                x+lx, 1.0f,  z+lz,
                0.0f, 1.0f,  0.0f);
  /* 2D rendering 
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
  */
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
    GLfloat emitLight[] = { 0.9, 0.9, 0.9, 0.9 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emitLight);
    glutSolidSphere(radius, 25, 25);
    GLfloat Noemit[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Noemit);
  }
  glPopMatrix();

  glTranslatef(10.0f, 0.0f, -40.0f);
  glPushMatrix();
  {
    GLfloat qaLightPosition[] = { 1, 1, 1, 1 };
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    GLfloat emitLight[] = { 0.9, 0.9, 0.9, 0.9 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emitLight);
    glutSolidSphere(radius / 8, 25, 25);
    GLfloat Noemit[] = { 0.0, 0.0, 1.0, 1.0 };
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

void press(int key, int xx, int yy)
{
  switch (key)
    {
    case GLUT_KEY_LEFT  : deltaAngle = -0.01f; break;
    case GLUT_KEY_RIGHT : deltaAngle = 0.01f;  break;
    case GLUT_KEY_UP    : deltaMove  = 0.5f;   break;
    case GLUT_KEY_DOWN  : deltaMove  = -0.5f;  break;
    }
}

void release(int key, int x, int y)
{
  switch (key)
    {
    case GLUT_KEY_LEFT :
    case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
    case GLUT_KEY_UP   :
    case GLUT_KEY_DOWN : deltaMove  = 0.0f; break;
    }
}

void resize(int w, int h)
{
  if (h == 0) h = 1;
  float ratio =  w * 1.0 / h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  gluPerspective(45.0f, ratio, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
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
  glutIdleFunc(render);
  glutTimerFunc(0, timer, 0);
  glutReshapeFunc(resize);
  glutSpecialFunc(press);
  glutSpecialUpFunc(release);
  glutMainLoop();
  return 0;
}
