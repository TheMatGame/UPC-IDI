// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"
#include "math.h"

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
    void resizeGL (int w, int h);
    

    void projectTransform();
    void viewTransform ();
    void iniciCamera();
    void homerTransform();
    void calcularCapsula(Model &m, glm::vec3 &min, glm::vec3 &max);
    void moureMouse(QMouseEvent* e);

    GLuint projLoc, viewLoc;
    
    float giro = 0.f;

    Model homer;

    GLuint VAO_terra;

  private:
    glm::vec3 min, max;
    float altura;

    int printOglError(const char file[], int line, const char func[]);

    float fov, ra, znear, zfar;
    glm::vec3 OBS, VRP, UP;

    //centre de l'esfera
    glm::vec3 CentreEscena = glm::vec3(0.0,1.0,0.0);
    float Distancia = 4.5;
    float Radi = 3.6;

    float alphav = asin(Radi/Distancia);
    float hw = 2*(Distancia-Radi)*tan(alphav);
    float aw = hw * alphav;
    float raw = aw/hw;
    float rav = float(width())/float(height());

    float left, nleft, right, nright, top, ntop, bottom, nbottom;

    bool prespectiva;

    float rx = 0.f, ry = 0.f;
    float sx = 0.f, sy = 0.f;
};
