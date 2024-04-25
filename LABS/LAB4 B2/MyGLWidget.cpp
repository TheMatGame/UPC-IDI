// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::iniciCamera()
{
    OBS = glm::vec3(0,0,3);
    VRP = glm::vec3(0,0,0);
    UP = glm::vec3(0,1,0);

    pi = M_PI/2.f;
    ra = 1.f;
    znear = 0.4f;
    zfar = 5.f;
}

void MyGLWidget::projectTransform() {
    // glm::perspective (FOV en radians, ra window, znear, zfar)
    glm::mat4 Proj = glm::perspective(pi, ra, znear, zfar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}


void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation(program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::viewTransform() {
    // glm::lookAt (OBS, VRP, UP)
    glm::mat4 View = glm::lookAt (OBS, VRP, UP);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::creaBuffers () {
      // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_Casa);
    glBindVertexArray(VAO_Casa);

    GLuint VBO_Casa[2];
    glGenBuffers(2, VBO_Casa);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Casa[0]);
    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * homer.faces ().size () * 3 * 3, homer.VBO_vertices (), GL_STATIC_DRAW); // posició


    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Casa[1]);
    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * homer.faces ().size () * 3 * 3, homer.VBO_matdiff (), GL_STATIC_DRAW); // color  


    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);
    

    glm::vec3 terra[6] = {
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(-2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(-2.0, -1.0, -2.0),
	glm::vec3(2.0, -1.0, 2.0),
	glm::vec3(2.0, -1.0, -2.0)
  }; 

  glm::vec3 blanc[6] = {
	glm::vec3(1,1,1),
	glm::vec3(1,1,1),
	glm::vec3(1,1,1),
	glm::vec3(1,1,1),
	glm::vec3(1,1,1),
	glm::vec3(1,1,1)
  };

  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_terra);
  glBindVertexArray(VAO_terra);

  GLuint VBO_terra[2];
  glGenBuffers(2, VBO_terra);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_terra[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(terra), terra, GL_STATIC_DRAW);

  // Activem l'atribut vertexLoc
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glBindBuffer(GL_ARRAY_BUFFER, VBO_terra[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(blanc), blanc, GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray (0);
    
}

void MyGLWidget::paintGL () 
{
  
    // Esborrem el frame-buffer
    glClear (GL_COLOR_BUFFER_BIT);
    glEnable (GL_DEPTH_TEST);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Carreguem la transformació de model
    homerTransform ();

    // Activem el VAO per a pintar la caseta 
    glBindVertexArray (VAO_Casa);

    // pintem
    glDrawArrays (GL_TRIANGLES, 0, homer.faces ().size () * 3);

    modelTransform();
    glBindVertexArray(VAO_terra);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray (0);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_S: { // escalar a més gran
      escala += 0.05;
      break;
    }
    case Qt::Key_D: { // escalar a més petit
      escala -= 0.05;
      break;
    }
    case Qt::Key_R: { // escalar a més petit
      giro += M_PI/4;
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::homerTransform()
{
    glm::mat4 TG(1.0f);
    TG = glm::scale(TG, glm::vec3(escala));
    TG = glm::rotate(TG, giro, glm::vec3(0.0,1.0,0.0));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::initializeGL() {
    homer.load("/dades/marc.gomez.via/idi/models/HomerProves.obj");
    iniciCamera();
    BL2GLWidget::initializeGL();
    projectTransform();
    viewTransform();
}