#pragma once
/*
*功能：窗口登录界面设计，继承自BasicWindow基类。实现登录账号功能	
*/
#include "BasicWindow.h"
#include "ui_UserLogin.h"

class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	~UserLogin();


private slots:
	void onLoginBtnClicked();
private:
	void initControl();
private:
	Ui::UserLogin ui;
};
