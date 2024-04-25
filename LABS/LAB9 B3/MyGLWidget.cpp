// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include "glm/gtc/matrix_inverse.hpp"

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

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  creaBuffersPatricio();
  creaBuffersTerraIParet();

  iniEscena();
  iniCamera();
  canviarFocus();

    colorFocus = glm::vec3(0.8, 0.8, 0.8);
    llumAmbient = glm::vec3(0.2, 0.2, 0.2);
    posx = 0;
}

void MyGLWidget::viewTransform ()
{
    BL3GLWidget::viewTransform ();
    canviarFocus();
}

void MyGLWidget::canviarFocus(){
  if (!isF) {
    posicioFocus = glm::vec4(posx, 0, 1,1);
  }
  else {
    posicioFocus = glm::vec4(1,1,1,1);
  }
    glUniform3fv(posFoc, 1, &posicioFocus[0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
    makeCurrent();
    switch (event->key()) {
    case Qt::Key_K: 
      --posx;
      canviarFocus();
      break;
    case Qt::Key_L: 
      ++posx;
      canviarFocus();
      break;
    case Qt::Key_F: 
      isF = !isF;
      canviarFocus();
      break;
    default: BL3GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::paintGL () 
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);

  glUniform3fv (colFoc, 1, &colorFocus[0]);
  glUniform3fv (llumAmb, 1, &llumAmbient[0]);

  modelTransformTerra ();
  NormalMatrix = glm::inverseTranspose(glm::mat3(View*TG));
  glUniformMatrix3fv (NormSCO, 1, GL_FALSE, &NormalMatrix[0][0]);
  // pintem el terra
  glDrawArrays(GL_TRIANGLES, 0, 12);

  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Patr);

  modelTransformPatricio ();
  NormalMatrix = glm::inverseTranspose(glm::mat3(View*TG));
  glUniformMatrix3fv (NormSCO, 1, GL_FALSE, &NormalMatrix[0][0]);
  // Pintem el Patricio
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicLlumShader.frag");
  vs.compileSourceFile("shaders/basicLlumShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “normal” del vertex shader
  normalLoc = glGetAttribLocation (program->programId(), "normal");
  // Obtenim identificador per a l'atribut “matamb” del vertex shader
  matambLoc = glGetAttribLocation (program->programId(), "matamb");
  // Obtenim identificador per a l'atribut “matdiff” del vertex shader
  matdiffLoc = glGetAttribLocation (program->programId(), "matdiff");
  // Obtenim identificador per a l'atribut “matspec” del vertex shader
  matspecLoc = glGetAttribLocation (program->programId(), "matspec");
  // Obtenim identificador per a l'atribut “matshin” del vertex shader
  matshinLoc = glGetAttribLocation (program->programId(), "matshin");

  // Demanem identificadors per als uniforms del vertex shader
  transLoc = glGetUniformLocation (program->programId(), "TG");
  projLoc = glGetUniformLocation (program->programId(), "proj");
  viewLoc = glGetUniformLocation (program->programId(), "view");
  NormSCO = glGetUniformLocation (program->programId(), "NormMat");
  colFoc = glGetUniformLocation (program->programId(), "colorFocus");
  llumAmb = glGetUniformLocation (program->programId(), "llumAmbient");
  posFoc = glGetUniformLocation (program->programId(), "posFocus");
}


void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0,0,0.2);
  diff = glm::vec3(0,0,0.8);
  spec = glm::vec3(0,0,0);
  shin = 0;
}