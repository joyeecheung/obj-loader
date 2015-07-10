#include <stdio.h>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>

#include <FL/glu.h>
#include <FL/glut.h>

#include "glm.h"
#include "Camera.h"

char model_file[] = "data/bunny_200.obj";

const int WIDTH = 800, HEIGHT = 600, MENU_HEIGHT = 25;
const int CAMERA_POS = 5;


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

const float MOVE_SPEED = 0.1f;
const float ROTATE_SPEED = 1.0f;
const float ROTATE_FACTOR = 0.25f;

GLMmodel* model_obj = NULL;
int model_list = 0;
Camera camera;

GLuint loadObjIntoList(char * modelfile) {
    if (model_obj)
        glmDelete(model_obj);
    model_obj = glmReadOBJ(modelfile);
    if (!model_obj) exit(0);
    glmFacetNormals(model_obj);
    glmVertexNormals(model_obj, 179.0);

    if (model_obj->nummaterials > 0) {  // has material
        return glmList(model_obj, GLM_SMOOTH | GLM_MATERIAL);
    } else {  // no material
        return glmList(model_obj, GLM_SMOOTH);
    }
}


void open_cb(Fl_Widget* widget, void* arg) {

    // Create the file chooser, and show it
    Fl_File_Chooser chooser(".",                        // directory
                            "*.obj",                        // filter
                            Fl_File_Chooser::MULTI,     // chooser type
                            "Choose an obj");        // title
    chooser.show();

    // Block until user picks something.
    //     (The other way to do this is to use a callback())
    //
    while (chooser.shown()) {
        Fl::wait();
    }

    // User hit cancel?
    if (chooser.value() == NULL) {
        fprintf(stderr, "(User hit 'Cancel')\n"); return;
    }

    // Print what the user picked
    fprintf(stderr, "--------------------\n");
    fprintf(stderr, "DIRECTORY: '%s'\n", chooser.directory());
    fprintf(stderr, "    VALUE: '%s'\n", chooser.value());
    fprintf(stderr, "    COUNT: %d files selected\n", chooser.count());

    // Multiple files? Show all of them
    if (chooser.count() > 1) {
        for (int t = 1; t <= chooser.count(); t++) {
            fprintf(stderr, " VALUE[%d]: '%s'\n", t, chooser.value(t));
        }
    }

    // to non-const
    char buffer[128] = { 0 };
    strcpy(buffer, chooser.value());

    model_list = loadObjIntoList(buffer);
}

// quit item
void quit_cb(Fl_Widget* widget, void* arg) {
    exit(0);
}


int handleGLError() {
    GLenum error;
    int i = 0;
    while ((error = glGetError()) != GL_NO_ERROR) {
        printf("GL ERROR(%d):  %s\n", i, gluErrorString(error));
        i++;
    }
    if (i == 0) return 1;
    return 0;
}

void display(void) {
    /* clear the frame buffer and depth buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    camera.render();
    glClearColor(0.3, 0.3, 0.3, 0.3);
    glColor3f(1.0, 1.0, 1.0);
    glCallList(model_list);
    handleGLError();
    glPopMatrix();

    glutSwapBuffers();
}

int mouseButton = 0;
int mouseX = 0;
int mouseY = 0;
void mouse(int button, int state, int x, int y) {
    // save the current state of the mouse
    mouseButton = button;
    mouseX = x;
    mouseY = y;
}

void motion(int x, int y) {
    if (mouseButton == GLUT_LEFT_BUTTON) { // Left button = rotation
        camera.rotateX(-(mouseY - y) * ROTATE_FACTOR);
        camera.rotateY(-(mouseX - x) * ROTATE_FACTOR);
        mouseX = x;
        mouseY = y;
    } else if (mouseButton == GLUT_MIDDLE_BUTTON) {  // Middle button = translation
        camera.moveX(-(mouseX - x) * MOVE_SPEED);
        camera.moveY(-(mouseY - y) * MOVE_SPEED);
        mouseX = x;
        mouseY = y;
    } else if (mouseButton == GLUT_RIGHT_BUTTON) { // Right button = zoom
        camera.moveZ(-(mouseX - x) * MOVE_SPEED);
        mouseX = x;
        mouseY = y;
    }

    // Redraw the scene with the new camera parameters
    glutPostRedisplay();
}

// setup lighting, color, enable states
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

void initCanvas(int width, int height) {
    init(width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)width / height, 0.5, 3000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.moveZ(CAMERA_POS);

    // callbacks
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(display);

    // Enter the main rendering loop
    glutMainLoop();
}


void render() {
    glCallList(model_list);
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    Fl_Window win(WIDTH, HEIGHT, "OBJ loader");
    Fl_Menu_Bar menubar(0, 0, WIDTH, MENU_HEIGHT);
    menubar.add("File/Open", 0, open_cb);
    menubar.add("File/Quit", 0, quit_cb);

    win.show(argc, argv);
    win.begin();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, MENU_HEIGHT);
    glutCreateWindow("OBJ loader");

    model_list = loadObjIntoList(model_file);

    initCanvas(WIDTH, HEIGHT);
    win.end();
    return(Fl::run());
    return 0;
}