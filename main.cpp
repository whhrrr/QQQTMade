#include "CCMainWindow.h"
#include <QtWidgets/QApplication>
#include"UserLogin.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*CCMainWindow w;
    w.show();*/
    UserLogin* userLogin = new UserLogin();
    userLogin->show();
    return a.exec();
}
