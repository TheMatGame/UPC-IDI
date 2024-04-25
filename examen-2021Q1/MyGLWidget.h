#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformCub (float escala, float angle);
    virtual void modelTransformPatricio ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    virtual void initializeGL ( );

  private:
    int printOglError(const char file[], int line, const char func[]);

    float anglePat = 0.f;
    float superAngle = 0.f;
    bool bShowBox = false;
    bool bBlanc = false;

  public slots:
    void perspectiva();
    void ortogonal();
    void caixa(int c);

  signals:
    void changePersp(bool vista);
    void changeOrto(bool vista);
};
