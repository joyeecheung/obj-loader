#include "Canvas.h"

void *INFO_FONT = GLUT_BITMAP_HELVETICA_18;
int Canvas::mouseButton;
int Canvas::mouseX, Canvas::mouseY, Canvas::display_list, Canvas::width, Canvas::height;
std::shared_ptr<Camera> Canvas::camera;
GLenum Canvas::shade_mode = GL_SMOOTH;
bool Canvas::light1_on = true, Canvas::light2_on = true;

void Canvas::initialize(std::shared_ptr<Camera> cam, int list, int w, int h) {
    camera = cam;
    display_list = list;
    mouseButton = 0;
    mouseX = 0;
    mouseY = 0;
    width = w;
    height = h;
    setup(width, height);
}

void Canvas::display(void) {
    /* clear the frame buffer and depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(shade_mode);
    glPushMatrix();

    if (light2_on)
        glEnable(GL_LIGHT2);
    else
        glDisable(GL_LIGHT2);

    if (light1_on)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);

    camera->render();
    glClearColor(BACKGROUND[0], BACKGROUND[1], BACKGROUND[2], BACKGROUND[3]);
    glColor4f(FOREGROUND[0], FOREGROUND[1], FOREGROUND[2], FOREGROUND[3]);
    glCallList(display_list);
    handleGLError();

    showText();
    glPopMatrix();

    glutSwapBuffers();
}

void Canvas::mouse(int button, int state, int x, int y) {
    // save the current state of the mouse
    mouseButton = button;
    mouseX = x;
    mouseY = y;
}

void Canvas::motion(int x, int y) {
    if (mouseButton == GLUT_LEFT_BUTTON) { // Left button = rotation
        camera->rotateX(-(mouseY - y) * ROTATE_FACTOR);
        camera->rotateY(-(mouseX - x) * ROTATE_FACTOR);
        mouseX = x;
        mouseY = y;
    } else if (mouseButton == GLUT_MIDDLE_BUTTON) {  // Middle button = translation
        camera->moveX(-(mouseX - x) * MID_MOVE_SPEED);
        camera->moveY((mouseY - y) * MID_MOVE_SPEED);
        mouseX = x;
        mouseY = y;
    } else if (mouseButton == GLUT_RIGHT_BUTTON) { // Right button = zoom
        camera->moveZ(-(mouseX - x) * MOVE_SPEED);
        mouseX = x;
        mouseY = y;
    }

    // Redraw the scene with the new camera parameters
    glutPostRedisplay();
}

// setup lighting, color, enable states
void Canvas::setup(int width, int height) {
    // solid rendering
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    // lighting
    glLightfv(GL_LIGHT1, GL_AMBIENT, LIGHT_AMBIENT_1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LIGHT_DIFFUSE_1);
    glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_POSITION_1);

    if (light1_on)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_AMBIENT, LIGHT_AMBIENT_2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, LIGHT_DIFFUSE_2);
    glLightfv(GL_LIGHT2, GL_POSITION, LIGHT_POSITION_2);
    if (light2_on)
        glEnable(GL_LIGHT2);
    else
        glDisable(GL_LIGHT2);

    glEnable(GL_LIGHTING);

    glEnable(GL_COLOR_MATERIAL);

    // settings
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glFrontFace(GL_CCW);   // orientation should be the front face
    glShadeModel(shade_mode);

    // blending
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(BACKGROUND[0], BACKGROUND[1], BACKGROUND[2], BACKGROUND[3]);
    glColor4f(FOREGROUND[0], FOREGROUND[1], FOREGROUND[2], FOREGROUND[3]);
}

void Canvas::resetCamera() {
    camera->reset();
    FVector3 camera_position;
    camera_position.set(CAMERA_POS[0], CAMERA_POS[1], CAMERA_POS[2]);
    camera->move(camera_position);
}

void Canvas::start() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width / height, 0.01f, 10000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    resetCamera();

    // callbacks
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(display);

    // Enter the main rendering loop
    glutMainLoop();
    render();
}

void renderBitmapString(const float position[3],
                        void *font, const char *string) {
    const char *c;
    glRasterPos3fv(position);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void vecToString(char *buffer, const char *format, FVector3 vec) {
    sprintf(buffer, format, vec.x, vec.y, vec.z);
}

void Canvas::showText() {
    char positionInfo[50], rotationInfo[50], lightInfo[50], shadeInfo[50];
    vecToString(positionInfo,
                "Position %3.2f, %3.2f, %3.2f", camera->getPosition());
    vecToString(rotationInfo,
                "Rotation %3.2f, %3.2f, %3.2f", camera->getRotation());
    sprintf(lightInfo, "Light1: %s, Light2: %s",
            light1_on ? "ON" : "OFF",
            light2_on ? "ON" : "OFF");
    sprintf(shadeInfo, "Shading: %s", shade_mode == GL_FLAT ? "FLAT" : "SMOOTH");
    // draw status text
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    // render the string
    renderBitmapString(POS_INFO_POSITION, INFO_FONT, positionInfo);
    renderBitmapString(ROT_INFO_POSITION, INFO_FONT, rotationInfo);
    renderBitmapString(LIGHT_INFO_POSITION, INFO_FONT, lightInfo);
    renderBitmapString(SHADE_INFO_POSITION, INFO_FONT, shadeInfo);

    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Canvas::render() {
    glCallList(display_list);
    glutPostRedisplay();
}

void Canvas::setList(int list) {
    glDeleteLists(display_list, 1);
    display_list = list;
}

void Canvas::setShadeMode(GLenum mode) {
    shade_mode = mode;
}

GLenum Canvas::getShadeMode() {
    return shade_mode;
}

int Canvas::handleGLError() {
    GLenum error;
    int i = 0;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("GL ERROR(%d):  %s\n", i, gluErrorString(error));
        i++;
    }
    if (i == 0) return 1;
    return 0;
}
