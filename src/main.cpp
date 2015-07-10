#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>

#include "glCanvas.h"
#include "glm.h"

GLCanvas glcanvas;

char model_file[] = "data/bunny_200.obj";

GLMmodel* pmodel = NULL;

GLuint loadmodel(char * modelfile) {
  if (pmodel)
    glmDelete(pmodel);
  pmodel = glmReadOBJ(modelfile);
  if (!pmodel) exit(0);
  glmFacetNormals(pmodel);
  glmVertexNormals(pmodel, 179.0);

  if (pmodel->nummaterials > 0) {
    return glmList(pmodel, GLM_SMOOTH | GLM_MATERIAL);
  } else {
    return glmList(pmodel, GLM_SMOOTH);
  }
  
}


// Callback: when use picks 'File | Open' from main menu
void open_cb(Fl_Widget*, void*) {

    // Create the file chooser, and show it
    Fl_File_Chooser chooser(".",                        // directory
                            "*",                        // filter
                            Fl_File_Chooser::MULTI,     // chooser type
                            "Title Of Chooser");        // title
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

    char buffer[128] = { 0 };
    strcpy(buffer, chooser.value());

    glcanvas.setList(loadmodel(buffer));
}

// Callback: when user picks 'Quit'
void quit_cb(Fl_Widget*, void*) {
    exit(0);
}

int main(int argc, char *argv[]) {
  srand(0);
  Fl_Window win(800, 600, "OBJ loader");
  Fl_Menu_Bar menubar(0, 0, 800, 25);
  menubar.add("File/Open", 0, open_cb);
  menubar.add("File/Quit", 0, quit_cb);
  Fl_Box box(0, 0, 260, 260, "Hey, I mean,\nHello, World!");
  win.show(argc, argv);
  // Docs say to add glut subwindow /after/ calling win.show()
  win.begin();

    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 25);

    glutCreateWindow("OBJ loader");

    GLuint model_list = loadmodel(model_file);


    glcanvas.initialize(model_list, 800, 600);
  win.end();
  return(Fl::run());
  return 0;
}