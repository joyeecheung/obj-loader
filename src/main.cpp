#include <stdio.h>

#include "GL/glut.h"
#include "glCanvas.h"
#include "glm.h"

char model_file[] = "data/bunny_1k.obj";

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

int main(int argc, char *argv[]) {
  srand(0);

  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(0, 0);

  glutCreateWindow("OBJ loader");

  GLuint model_list = loadmodel(model_file);

  GLCanvas glcanvas;
  glcanvas.initialize(model_list, 800, 600);
  return 0;
}

// =========================================
// =========================================
