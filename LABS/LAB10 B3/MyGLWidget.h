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
    void calculaCapsaModel();
    void modelTransformPatricio();

  private:
    float posx;
    int printOglError(const char file[], int line, const char func[]);
    glm::mat3 NormalMatrix;
    GLuint NormSCO, colFoc, llumAmb, posFoc, colFoc2, posFoc2;
    glm::vec3 colorFocus;
    glm::vec3 colorFocus2;
    glm::vec3 llumAmbient;
    glm::vec4 posicioFocus;
    glm::vec4 posicioFocus2;

    bool isF = false;

    bool bFocus1 = true;
    bool bFocus2 = true;
};



