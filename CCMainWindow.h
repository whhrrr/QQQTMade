#pragma once

#include <QtWidgets/QWidget>
#include "ui_CCMainWindow.h"
#include "BasicWindow.h"

class CCMainWindow : public BasicWindow
{
    Q_OBJECT

public:
    CCMainWindow(QWidget *parent = Q_NULLPTR);
    ~CCMainWindow();
public:
    void initControl();
    void setUserName(const QString& username);//设置用户名称
    void setLevelPixmap(int level);//设置等级
    void setHeadPixmap(const QString& headPath);//设置头像


private:
    Ui::CCMainWindowClass ui;
};
