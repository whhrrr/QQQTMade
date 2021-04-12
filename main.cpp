#include "CCMainWindow.h"
#include <QtWidgets/QApplication>
#include"UserLogin.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*CCMainWindow w;
    w.show();*/
    a.setQuitOnLastWindowClosed(false);//窗口关闭不退出
    UserLogin* userLogin = new UserLogin();
    userLogin->show();
    return a.exec();
}
