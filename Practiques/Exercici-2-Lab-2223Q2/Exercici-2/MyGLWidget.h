#include "LL2GLWidget.h"

#include "model.h"

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    void RickTransform();
    void VideoCameraTransform();

    void iniCamera();

  private:

  
    int printOglError(const char file[], int line, const char func[]);
    
};
