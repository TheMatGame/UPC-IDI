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

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  iniEscena ();
  iniCamera ();

      colFoc = glm::vec3(1,1,1);
      anglePat = 0.f;
     superAngle = 0.f;
     bShowBox = false;
     bBlanc = false;
     camPlanta = false;
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glUniform3fv (colfocusLoc, 1, &colFoc[0]);

  // Pintem el terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  if (bShowBox) 
  {
    // Pintem el Patricio
    glBindVertexArray (VAO_Patr);
    modelTransformPatricio ();
    glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  }
  else
  {
    // Pintem el cub
    glBindVertexArray(VAO_Cub);
    modelTransformCub (4.0, 0.0);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    modelTransformCub (5.0, 2*M_PI/3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    modelTransformCub (6.0, -2*M_PI/3);
    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
  
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  TG = glm::mat4(1.f);
  TG = rotate(TG, superAngle + angle, glm::vec3(0,1,0));
  TG = translate(TG, glm::vec3(5,0,0));
  TG = scale(TG, glm::vec3(escala,escala,escala));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  TG = glm::mat4(1.f);
  TG = rotate(TG, superAngle + anglePat, glm::vec3(0,1,0));
  TG = translate(TG, glm::vec3(5,0,0));
  TG = rotate(TG, float(M_PI/-2.f) , glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3 (2*escala, 2*escala, 2*escala));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
    View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
    View = glm::rotate(View, float(M_PI/2), glm::vec3(0, 1, 0));
    View = glm::rotate(View, float(M_PI/2), glm::vec3(0, 0, 1));
    View = glm::translate(View, -centreEsc);

    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
    glm::mat4 Proj;  // Matriu de projecció
    Proj = glm::ortho(-10.f, 10.f, -10.f, 10.f, zn, zf);

    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      bShowBox = !bShowBox;
    break;
	}
  case Qt::Key_1: {
      anglePat = 0;
    break;
	}
  case Qt::Key_2: {
      anglePat = 2*M_PI/3;
    break;
	}
  case Qt::Key_3: {
      anglePat = -2*M_PI/3;
    break;
	}
  case Qt::Key_F: {
      if (bBlanc)
      {
        colFoc = glm::vec3(1,1,1);
      }
      else
      {
        colFoc = glm::vec3(1,1,0);
      }
      bBlanc = !bBlanc;
    break;
	}
  case Qt::Key_C: {
      camPlanta = !camPlanta;
      if (camPlanta) emit changeOrto(true);
      else emit changePersp(true);
      projectTransform();
      viewTransform();
    break;
	}
  case Qt::Key_Right: {
      superAngle += 2*M_PI/3;
    break;
	}
  case Qt::Key_Left: {
      superAngle -= 2*M_PI/3;
    break;
	}
  case Qt::Key_R: {
      initializeGL ();
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::perspectiva()
{
  makeCurrent();
  camPlanta = false;
  projectTransform();
      viewTransform();
  update();
}

void MyGLWidget::ortogonal()
{
  makeCurrent();
  camPlanta = true;
  projectTransform();
      viewTransform();
  update();
}

void MyGLWidget::caixa(int c)
{
  makeCurrent();
  if (c == 0) anglePat = 0;
  else if (c == 1) anglePat = 2*M_PI/3;
  else if (c == 2) anglePat = -2*M_PI/3;
  update();
}