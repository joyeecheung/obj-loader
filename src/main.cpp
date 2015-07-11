#include <stdio.h>
#include <memory>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Button.H>

#include <FL/gl.h>
#include <FL/glu.h>
#include <FL/glut.h>

#include "glm.h"
#include "Config.h"
#include "Camera.h"
#include "Canvas.h"

GLMmodel* model_obj = NULL;
char model_file[] = "data/bunny_1k.obj";

Camera camera;
Canvas canvas;
GLuint loadObjIntoList(char * modelfile) {
    // release and read
    if (model_obj) glmDelete(model_obj);
    model_obj = glmReadOBJ(modelfile);

    // fails
    if (!model_obj) exit(0);
    // succeeds, generate normals
    glmFacetNormals(model_obj);
    glmVertexNormals(model_obj, MAX_NORMAL_ANGLE);

    if (model_obj->nummaterials > 0) {  // has material
        return glmList(model_obj, GLM_SMOOTH | GLM_MATERIAL);
    } else {  // no material
        return glmList(model_obj, GLM_SMOOTH);
    }
}

void open_cb(Fl_Widget* widget, void* arg) {
    // Create native chooser
    Fl_Native_File_Chooser chooser;
    chooser.directory("./data/");
    chooser.title("Pick a .obj file");
    chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
    chooser.filter("Wavefront OBJ\t*.obj\n");

    // Show native chooser
    switch (chooser.show()) {
        case -1: fprintf(stderr, "ERROR: %s\n", chooser.errmsg()); break;  // ERROR
        case  1: fprintf(stderr, "*** CANCEL\n"); break;  // CANCEL
        default: // PICKED FILE
            if (chooser.filename()) {
                char buffer[2048] = { 0 };
                strcpy(buffer, chooser.filename());
                canvas.setList(loadObjIntoList(buffer));
                canvas.render();
            }
            break;
    }
}

// quit item
void quit_cb(Fl_Widget* widget, void* arg) {
    exit(0);
}

void smooth_cb(Fl_Widget* widget, void* arg) {
    canvas.setShadeMode(GL_SMOOTH);
    canvas.render();
}

void flat_cb(Fl_Widget* widget, void* arg) {
    canvas.setShadeMode(GL_FLAT);
    canvas.render();
}

void light1_cb(Fl_Widget* widget, void* arg) {
    canvas.light1_on = !canvas.light1_on;
    canvas.render();
}

void light2_cb(Fl_Widget* widget, void* arg) {
    canvas.light2_on = !canvas.light2_on;
    canvas.render();
}

void reset_cb(Fl_Widget* widget, void* arg) {
    canvas.resetCamera();
    canvas.render();
}

int main(int argc, char *argv[]) {
    Fl_Window win(WIDTH, HEIGHT + MENU_HEIGHT, "OBJ Viwer");
    Fl_Menu_Bar menubar(0, 0, WIDTH + 2, MENU_HEIGHT);
    menubar.add("File/Open", 0, open_cb);
    menubar.add("File/Quit", 0, quit_cb);
    menubar.add("Smooth", 0, smooth_cb);
    menubar.add("Flat", 0, flat_cb);
    menubar.add("Light1", 0, light1_cb);
    menubar.add("Light2", 0, light2_cb);
    menubar.add("Reset Camera", 0, reset_cb);

    win.show(argc, argv);
    win.begin();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, MENU_HEIGHT);
    glutCreateWindow("OBJ loader");
    
    canvas.initialize(std::make_shared<Camera>(camera),
                      loadObjIntoList(model_file), WIDTH, HEIGHT);
    canvas.start();

    win.end();
    return(Fl::run());
    return 0;
}