#include "glCanvas.h"

#include <cstdio>

// Included files for OpenGL rendering
#include <GL/gl.h>
#include <GL/glut.h>

// ========================================================
int GLCanvas::mouseButton = 0;
int GLCanvas::mouseX = 0;
int GLCanvas::mouseY = 0;

Camera GLCanvas::camera;
int GLCanvas::display_list_index;


void GLCanvas::InitLight() {
  // Set the last component of the position to 0 to indicate
  // a directional light source

  GLfloat position[4] = { 30,30,100, 1};
  GLfloat diffuse[4] = { 0.75,0.75,0.75,1};
  GLfloat specular[4] = { 0,0,0,1};
  GLfloat ambient[4] = { 0.2, 0.2, 0.2, 1.0 };

  GLfloat zero[4] = {0,0,0,0};
  glLightfv(GL_LIGHT1, GL_POSITION, position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, zero);
  glEnable(GL_LIGHT1);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_COLOR_MATERIAL);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

  GLfloat spec_mat[4] = {1,1,1,1};
  float glexponent = 30;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec_mat);

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  float back_color[] = { 0.0,0.0,1.0,1};
  glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_color);
  glEnable(GL_LIGHT1);
}


void GLCanvas::display(void)
{
  // // Clear the display buffer, set it to the background color
  // glClearColor(1,1,1,0);
  // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // // Set the camera parameters
  // glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();
  // InitLight(); // light will be a headlamp!

   /* clear the frame buffer and depth buffer */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

  // glPushMatrix();
   glPushMatrix();
    
    camera.render();
    glClearColor(0.3, 0.3, 0.3, 0.3);
    glColor3f(1.0, 1.0, 1.0);
    glCallList(display_list_index);
    HandleGLError(); 
  glPopMatrix();
  // glPopMatrix();
  // Swap the back buffer with the front buffer to display
  // the scene
  glutSwapBuffers();
}

// ========================================================
// Callback function for window resize
// ========================================================

void GLCanvas::reshape(int w, int h) {
  // Set the OpenGL viewport to fill the entire window
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);

  // Set the camera parameters to reflect the changes
  // camera->glInit(w, h);
}

// ========================================================
// Callback function for mouse click or release
// ========================================================

void GLCanvas::mouse(int button, int state, int x, int y) {
  // Save the current state of the mouse.  This will be
  // used by the 'motion' function
  mouseButton = button;
  mouseX = x;
  mouseY = y;
}

// ========================================================
// Callback function for mouse drag
// ========================================================
const float MOVE_SPEED = 0.1f;
const float ROTATE_SPEED = 1.0f;
const float ROTATE_FACTOR = 0.25f;

void GLCanvas::motion(int x, int y) {
  // Left button = rotation
  // (rotate camera around the up and horizontal vectors)
  if (mouseButton == GLUT_LEFT_BUTTON) {
    camera.rotateX((mouseY - y) * ROTATE_FACTOR);
    camera.rotateY((mouseX - x) * ROTATE_FACTOR);
    mouseX = x;
    mouseY = y;
  }
  // Middle button = translation
  // (move camera perpendicular to the direction vector)
  else if (mouseButton == GLUT_MIDDLE_BUTTON) {
    camera.moveX((mouseX - x) * MOVE_SPEED);
    camera.moveY((mouseY - y) * MOVE_SPEED);
    mouseX = x;
    mouseY = y;
  }
  // Right button = zoom
  // (move camera along the direction vector)
  else if (mouseButton == GLUT_RIGHT_BUTTON) {
    camera.moveZ((mouseX - x) * MOVE_SPEED);
    mouseX = x;
    mouseY = y;
  }

  // Redraw the scene with the new camera parameters
  glutPostRedisplay();
}

// // ========================================================
// // Callback function for keyboard events
// // ========================================================

// void GLCanvas::keyboard(unsigned char key, int x, int y) {
//   switch (key) {
//   case 'w':  case 'W':
//     args->wireframe = !args->wireframe;
//     render();
//     break;
//   case 'g': case 'G':
//     args->gouraud = !args->gouraud;
//     render();
//     break;
//   case 's': case 'S':
//     mesh->LoopSubdivision();
//     render();
//     break;
//   case 'd': case 'D':
//     mesh->Simplification((int)floor(0.9*mesh->numTriangles()));
//     render();
//     break;
//   case 'q':  case 'Q':
//     exit(0);
//     break;
//   default:
//     printf("UNKNOWN KEYBOARD INPUT  '%c'\n", key);
//   }
// }

/***********************
 * Lighting configuration
 ***********************/
const float LIGHT_AMBIENT_1[] = { 0.1f, 0.1f, 0.1f, 0.0f };
const float LIGHT_DIFFUSE_1[] = {
    211.0f / 255.0f, 183.0f / 255.0f, 133.0f / 255.0f, 0.0f
};
const float LIGHT_POSITION_1[] = { 0.8f, 0.4f, 0.5f, 0.0f };

const float LIGHT_AMBIENT_2[] = { 0.2f, 0.2f, 0.2f, 0.0f };
const float LIGHT_DIFFUSE_2[] = {
    211.0f / 255.0f, 183.0f / 255.0f, 133.0f / 255.0f, 0.0f
};
const float LIGHT_POSITION_2[] = { 0.8f, -0.2f, 0.5f, 0.0f };


// ========================================================
// Initialize all appropriate OpenGL variables, set
// callback functions, and start the main event loop.
// This function will not return but can be terminated
// by calling 'exit(0)'
// ========================================================

void init(int width, int height) {
    // solid rendering
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    // lighting
    glLightfv(GL_LIGHT1, GL_AMBIENT, LIGHT_AMBIENT_1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LIGHT_DIFFUSE_1);
    glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_POSITION_1);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, LIGHT_AMBIENT_2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, LIGHT_DIFFUSE_2);
    glLightfv(GL_LIGHT2, GL_POSITION, LIGHT_POSITION_2);
    glEnable(GL_LIGHT2);

    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);

    // settings
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glFrontFace(GL_CCW);   // orientation should be the front face
    glShadeModel(GL_SMOOTH);

    // blending
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.3, 0.3, 0.3, 0.3);
    glColor3f(1.0, 1.0, 1.0);
}

void GLCanvas::initialize(int display_list, int width, int height) {

  display_list_index = display_list;

  init(width, height);

  /* set up a perspective projection in the projection matrix */
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(70.0, (GLfloat)width / height, 1.0, 10000.0);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  camera.moveZ(10);
  /* position camera to see the island */
  // gluLookAt(1400.0, 500.0, 1400.0,  /* eye point */
  //          0.0, 10.0,    0.0,  /* at point */
  //        0.0,  1.0,    0.0); /* up vector */

  // Initialize callback functions
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutDisplayFunc(display);

  // glutReshapeFunc(reshape);
  // glutKeyboardFunc(keyboard);

  // render();

  // Enter the main rendering loop
  glutMainLoop();
}


void GLCanvas::render() {
  glCallList(display_list_index);
  glutPostRedisplay();
}

// ========================================================
// ========================================================

int HandleGLError() {
  GLenum error;
  int i = 0;
  while ((error = glGetError()) != GL_NO_ERROR) {
    printf ("GL ERROR(%d):  %s\n", i, gluErrorString(error));
    i++;
  }
  if (i == 0) return 1;
  return 0;
}
