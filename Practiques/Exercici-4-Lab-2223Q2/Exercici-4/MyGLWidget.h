// MyGLWidget.h
#include "LL4GLWidget.h"


class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();

  public slots:
    void onTimeOut();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformMorty ();
    virtual void modelTransformFantasma (float posX);
    virtual void modelTransformDiscoBall ();
    virtual void modelTransformTerra ();
    virtual void initializeGL ( );
    virtual void paintGL ( );
    virtual void carregaShaders ();


    void enviarNormalMatrix();
    void enviarFocus();
    void danzaFocus();
  
  private:
    int printOglError(const char file[], int line, const char func[]);
    float RotacioBola;
    float RotacioPers;

    bool bIsOff;
    int danzaCont;

    QTimer* timer;
    bool bTimerOn;

    GLuint NormSCO, posFoc, colFoc, RcolFoc, GcolFoc, BcolFoc, YcolFoc, TGBola;
    glm::mat3 NormalMatrix;
    glm::vec3 posicioFocus, colorFocus, RcolorFocus, GcolorFocus, BcolorFocus, YcolorFocus;
    
    glm::mat4 TG;
};
