#ifndef CANVAS_H
#define CANVAS_H

#include <memory>
#include <FL/gl.h>
#include <FL/glu.h>
#include <FL/glut.h>
#include "Camera.h"
#include "Config.h"

class Canvas {  // all static because the callbacks for glut need it
public:
    static void initialize(std::shared_ptr<Camera> cam, int list, int w, int h);
    static void display(void);
    static void mouse(int button, int state, int x, int y);
    static void motion(int x, int y);
    // setup lighting, color, enable states
    static void setup(int width, int height);
    static void start();
    static void resetCamera();
    static void render();
    static void showText();
    static void setList(int list);
    static void setShadeMode(GLenum mode);
    static GLenum getShadeMode();
    static bool light1_on;
    static bool light2_on;
private:
    static int handleGLError();
    static int mouseButton;
    static int mouseX, mouseY;
    static int display_list;
    static std::shared_ptr<Camera> camera;
    static int width, height;
    static GLenum shade_mode;
};

#endif