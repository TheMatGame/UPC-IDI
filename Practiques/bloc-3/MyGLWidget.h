// MyGLWidget.h
#include "BL3GLWidget.h"

class MyGLWidget : public BL3GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL3GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    void paintGL ();
    void carregaShaders ();
    void iniMaterialTerra ();
    virtual void initializeGL ( );
    virtual void viewTransform ();
    void canviarFocus();

  private:
    float posx;
    int printOglError(const char file[], int line, const char func[]);
    glm::mat3 NormalMatrix;
    GLuint NormSCO, colFoc, llumAmb, posFoc;
    glm::vec3 colorFocus;
    glm::vec3 llumAmbient;
    glm::vec4 posicioFocus;
};



