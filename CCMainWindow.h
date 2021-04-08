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
    void setUserName(const QString& username);//�����û�����
    void setLevelPixmap(int level);//���õȼ�
    void setHeadPixmap(const QString& headPath);//����ͷ��


private:
    Ui::CCMainWindowClass ui;
};
