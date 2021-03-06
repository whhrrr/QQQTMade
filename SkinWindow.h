#pragma once
/*
* 功能：继承自BasicWindow基类，实现改变皮肤颜色部件功能
* 
* 待修改：多次点击会出现多次的皮肤改变界面
*/
#include <QWidget>
#include "BasicWindow.h"
#include "ui_SkinWindow.h"

class SkinWindow : public BasicWindow
{
	Q_OBJECT

public:
	SkinWindow(QWidget *parent = Q_NULLPTR);
	~SkinWindow();
public:
	void initControl();
public slots:
	void onShowClose();
private:
	Ui::SkinWindow ui;
};
