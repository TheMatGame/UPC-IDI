// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();
    void carregaShaders();
    void creaBuffers ();
    void initializeGL ();
    void paintGL () ;
    void keyPressEvent (QKeyEvent *event);
    

    void projectTransform();
    void viewTransform ();
    void iniciCamera();
    void homerTransform();

    GLuint projLoc, viewLoc;

    float giro = 0.f;

    Model homer;

    GLuint VAO_terra;

  private:
    int printOglError(const char file[], int line, const char func[]);

    float pi, ra, znear, zfar;
    glm::vec3 OBS, VRP, UP;
};
