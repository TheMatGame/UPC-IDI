// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include "glm/gtc/matrix_inverse.hpp"
#include <QTimer>

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
  case Qt::Key_A: {
      RotacioPers += M_PI/180*90;
    break;
	}
  case Qt::Key_D: {
      RotacioPers -= M_PI/180*90;
    break;
	}
  case Qt::Key_E: {
    if (!bIsOff)
    {
      colorFocus = glm::vec3(0,0,0);
      bIsOff = true;
    }
    else
    {
      colorFocus = glm::vec3(0.4,0.4,0.4);
      bIsOff = false;
    }
    glUniform3fv(colFoc, 1, &colorFocus[0]);
    break;
	}
  case Qt::Key_B: {
      danzaFocus();
    break;
	}	
  case Qt::Key_R: {
      RotacioBola += M_PI/180*5;
      paintGL();
    break;
	}
  case Qt::Key_S: {
      if (!bTimerOn)
      {
        timer->start(100);
      }
      else 
      {
        timer->stop();
      }
      bTimerOn = !bTimerOn;
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::modelTransformMorty()
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(5,0,5));
  TG = glm::rotate(TG, RotacioPers, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG = glm::translate(TG, -centreBaseMorty);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma(float posX)
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(posX,0.5,5));
  TG = glm::rotate(TG, RotacioPers, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::translate(TG, -centreBaseFantasma);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformDiscoBall()
{
  TG = glm::mat4(1.f);
  TG = glm::translate(TG, glm::vec3(5,5,5));
  TG = glm::rotate(TG, RotacioBola, glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaDiscoBall, escalaDiscoBall, escalaDiscoBall));
  TG = glm::translate(TG, -centreBaseDiscoBall);
  
  glUniformMatrix4fv (TGBola, 1, GL_FALSE, &TG[0][0]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  TG = glm::mat4(1.f);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::initializeGL()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  carregaShaders();
  
  creaBuffersMorty();
  creaBuffersFantasma();
  creaBuffersDiscoBall();
  creaBuffersTerraIParets();

  iniEscena();
  iniCamera();

  RotacioBola = RotacioPers = 0.f;

  bIsOff = false;
  danzaCont = 0;

  bTimerOn = false;
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));

  //inizialitzar focus
  //blanc
  posicioFocus = glm::vec3(5,10,5);
  colorFocus = glm::vec3(0.4, 0.4, 0.4);
  RcolorFocus = glm::vec3(0.4, 0, 0);
  GcolorFocus = glm::vec3(0, 0.4, 0);
  BcolorFocus = glm::vec3(0, 0, 0.4);
  YcolorFocus = glm::vec3(0.4, 0.4, 0);

  enviarFocus();
}

void MyGLWidget::paintGL()
{
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  enviarNormalMatrix();
  glDrawArrays(GL_TRIANGLES, 0, 30);
  
  // MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty ();
  enviarNormalMatrix();
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);
  
  // FANTASMA
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (1.0f);
  enviarNormalMatrix();
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);
  modelTransformFantasma (9.0f);
  enviarNormalMatrix();
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // DISCO BALL
  glBindVertexArray (VAO_DiscoBall);
  modelTransformDiscoBall ();
  enviarNormalMatrix();
  glDrawArrays(GL_TRIANGLES, 0, discoBall.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  LL4GLWidget::carregaShaders();

  NormSCO = glGetUniformLocation (program->programId(), "NormMat");

  posFoc = glGetUniformLocation (program->programId(), "posFoc");
  colFoc = glGetUniformLocation (program->programId(), "colFoc");
  RcolFoc = glGetUniformLocation (program->programId(), "RcolFoc");
  GcolFoc = glGetUniformLocation (program->programId(), "GcolFoc");
  BcolFoc = glGetUniformLocation (program->programId(), "BcolFoc");
  YcolFoc = glGetUniformLocation (program->programId(), "YcolFoc");
  TGBola = glGetUniformLocation (program->programId(), "TGBola");

}

void MyGLWidget::enviarNormalMatrix()
{
  NormalMatrix = glm::inverseTranspose(glm::mat3(View*TG));
  glUniformMatrix3fv (NormSCO, 1, GL_FALSE, &NormalMatrix[0][0]);
}

void MyGLWidget::enviarFocus()
{
  glUniform3fv(colFoc, 1, &colorFocus[0]);
  glUniform3fv(posFoc, 1, &posicioFocus[0]);
  glUniform3fv(RcolFoc, 1, &RcolorFocus[0]);
  glUniform3fv(GcolFoc, 1, &GcolorFocus[0]);
  glUniform3fv(BcolFoc, 1, &BcolorFocus[0]);
  glUniform3fv(YcolFoc, 1, &YcolorFocus[0]);
}

void MyGLWidget::danzaFocus()
{
  if (danzaCont < 7) ++danzaCont;
  else danzaCont = 0;

  if (danzaCont == 0){
    RcolorFocus = glm::vec3(0,0,0);
    glUniform3fv(RcolFoc, 1, &RcolorFocus[0]);
  }
  else if (danzaCont == 1) {
    GcolorFocus = glm::vec3(0,0,0);
    glUniform3fv(GcolFoc, 1, &GcolorFocus[0]);
  }
  else if (danzaCont == 2) {
    BcolorFocus = glm::vec3(0,0,0);
    glUniform3fv(BcolFoc, 1, &BcolorFocus[0]);
  }
  else if (danzaCont == 3) {
    YcolorFocus = glm::vec3(0,0,0);
    glUniform3fv(YcolFoc, 1, &YcolorFocus[0]);
  }
  else if (danzaCont == 4) {
    RcolorFocus = glm::vec3(0.4,0,0);
    glUniform3fv(RcolFoc, 1, &RcolorFocus[0]);
  }
  else if (danzaCont == 5) {
    GcolorFocus = glm::vec3(0,0.4,0);
    glUniform3fv(GcolFoc, 1, &GcolorFocus[0]);
  }
  else if (danzaCont == 6) {
    BcolorFocus = glm::vec3(0,0,0.4);
    glUniform3fv(BcolFoc, 1, &BcolorFocus[0]);
  }
  else if (danzaCont == 7) {
    YcolorFocus = glm::vec3(0.4,0.4,0);
    glUniform3fv(YcolFoc, 1, &YcolorFocus[0]);
  }

}

void MyGLWidget::onTimeOut()
{
  makeCurrent();
  danzaFocus();
  RotacioBola += M_PI/180*5;
  update();
  
}
