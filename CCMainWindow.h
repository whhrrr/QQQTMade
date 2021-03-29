#pragma once

#include <QtWidgets/QWidget>
#include "ui_CCMainWindow.h"

class CCMainWindow : public QWidget
{
    Q_OBJECT

public:
    CCMainWindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::CCMainWindowClass ui;
};
