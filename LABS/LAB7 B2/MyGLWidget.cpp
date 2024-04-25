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

void MyGLWidget::CanviarFov(int value)
{
    makeCurrent();
    fov = value*(M_PI/180)*2;
    projectTransform();
    emit EmetreFov(value);
    update();
}

void MyGLWidget::Canviarmodel()
{
    makeCurrent();
    IsHomer = !IsHomer;
    projectTransform();
    paintGL();
    update();
}

MyGLWidget::~MyGLWidget() {
    //connect(this,SIGNAL(returnPressed()),this,SLOT(CanviarFov()));
}

void MyGLWidget::calcularCapsula(Model &m, glm::vec3 &min, glm::vec3 &max)
{
    min = glm::vec3(FLT_MAX);
    max = glm::vec3(-FLT_MAX);
    for (auto i = m.vertices().begin(); i != m.vertices().end(); ++i) {
        if (*i < min.x) min.x = *i;
        if (*i > max.x) max.x = *i;
        ++i; //para pasar a la cordenada y
        if (*i < min.y) min.y = *i;
        if (*i > max.y) max.y = *i;
        ++i; //para pasar a la cordenada z
        if (*i < min.z) min.z = *i;
        if (*i > max.z) max.z = *i;
        
    }
}

void MyGLWidget::iniciCamera()
{
    OBS = glm::vec3(0.0,1.0,Distancia);
    VRP = CentreEscena;
    UP = glm::vec3(0,1,0);
    
    fov = 2*alphav;
    ra = 1.f;  //este es el window
    znear = Distancia - Radi;
    zfar = Distancia + Radi;

    left = nleft = -Radi;
    right = nright = Radi;
    bottom = nbottom = -Radi;
    top = ntop = Radi;
    
    prespectiva = true ;
}

void MyGLWidget::resizeGL (int w, int h) 
{
    BL2GLWidget::resizeGL (w,h);
    rav = float(width())/float(height());
    if (prespectiva) {
        //important que sigui float
        ra = rav;
        //Nomes quan ra < 1
        if (ra < 1.0) fov = 2*(atan(tan(alphav)/rav));
    } else {
        if (rav < 1.0) {
            nbottom = bottom*rav;
            ntop = top*rav;
        }
        else {
            nleft = left * rav;
            nright = right * rav;
        }
    }
    projectTransform();
}

void MyGLWidget::projectTransform() {
    // glm::perspective (FOV en radians, ra window, znear, zfar)
    glm::mat4 Proj;
    if (prespectiva) Proj = glm::perspective(fov, ra, znear, zfar);
    else Proj = glm::ortho (nleft, nright, bottom, top, znear, zfar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::carregaShaders() {
    BL2GLWidget::carregaShaders();
    projLoc = glGetUniformLocation(program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::viewTransform() {
    // glm::lookAt (OBS, VRP, UP)
   // glm::mat4 View = glm::lookAt (OBS, VRP, UP);
    //EULER
    glm::mat4 View = glm::mat4(1.f);
    View = glm::translate(View, glm::vec3(0.f,0.f,-Distancia));
    View = glm::rotate(View, sx, glm::vec3(1.f,0.f,0.f));
    View = glm::rotate(View, sy, glm::vec3(0.f,1.f,0.f));
    View = glm::translate(View, glm::vec3(-VRP.x, -VRP.y, -VRP.z));
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

    //LEGOMAN

      // Creació del Vertex Array Object per pintar
    glGenVertexArrays(1, &VAO_lego);
    glBindVertexArray(VAO_lego);

    GLuint VBO_lego[2];
    glGenBuffers(2, VBO_lego);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_lego[0]);
    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * lego.faces ().size () * 3 * 3, lego.VBO_vertices (), GL_STATIC_DRAW); // posició


    // Activem l'atribut vertexLoc
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_lego[1]);
    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * lego.faces ().size () * 3 * 3, lego.VBO_matdiff (), GL_STATIC_DRAW); // color  


    // Activem l'atribut colorLoc
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);


    

    glm::vec3 terra[6] = {
	glm::vec3(-2.5, 0.0, -2.5),
	glm::vec3(-2.5, 0.0, 2.5),
	glm::vec3(2.5, 0.0, 2.5),
	glm::vec3(-2.5, 0.0, -2.5),
	glm::vec3(2.5, 0.0, 2.5),
	glm::vec3(2.5, 0.0, -2.5)
  }; 

  glm::vec3 blanc[6] = {
	glm::vec3(1,0,1),
	glm::vec3(0,1,1),
	glm::vec3(0,1,1),
	glm::vec3(1,0,1),
	glm::vec3(0,1,1),
	glm::vec3(1,1,0)
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
    if (IsHomer) 
    {
        homerTransform(maxh, minh);
        glBindVertexArray (VAO_Casa);
        glDrawArrays (GL_TRIANGLES, 0, homer.faces ().size () * 3);
    }
    else {
        homerTransform(maxl, minl);
        glBindVertexArray (VAO_lego);
        glDrawArrays (GL_TRIANGLES, 0, lego.faces ().size () * 3);
    }

    glBindVertexArray (0);

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
    case Qt::Key_O: { // escalar a més petit
      prespectiva = !prespectiva;
      projectTransform();
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::moureMouse(QMouseEvent* e)
{
    if (e->buttons() == Qt::LeftButton /* && !(e->modifiers() (Qt::ShiftModifier | Qt::AltModifier | Qt::ControlModifier)) */) {
        int xm = e->x();
        int ym = e->y();

        if (rx < xm) sx += 0.1;
        else if (rx > xm) sx -= 0.1;

        if (ry < ym) sy += 0.1;
        else if (ry > ym) sy -= 0.1;

        viewTransform();

        rx = xm;
        ry = ym;
    }
}

void MyGLWidget::homerTransform(glm::vec3 &max, glm::vec3 &min)
{
    float altura = max.y - min.y;
    altura = 1/altura * 4;
    glm::mat4 TG(1.0f);
    TG = glm::scale(TG, glm::vec3(escala));
    TG = glm::scale(TG, glm::vec3(altura,altura,altura));
    TG = glm::rotate(TG, giro, glm::vec3(0.0,1.0,0.0));
    TG = glm::translate(TG, glm::vec3(-(max.x+min.x)/2,-min.y,-(max.z+min.z)/2));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::initializeGL() {
    homer.load("/dades/marc.gomez.via/idi/models/Patricio.obj");
    calcularCapsula(homer,minh,maxh);
    lego.load("/dades/marc.gomez.via/idi/models/legoman.obj");
    calcularCapsula(lego,minl,maxl);
    iniciCamera();
    BL2GLWidget::initializeGL();
    projectTransform();
    viewTransform();
}