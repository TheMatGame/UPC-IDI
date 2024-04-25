#include <QApplication>
#include <QPushButton>

int main(int argc, char **argv)
{
    QApplication a(argc,argv);
    QPushButton hello("SEXO MODO DIABLO",0);
    hello.resize(500,30);
    hello.show();
    return a.exec();
}