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
	bool connectMysql();//连接数据库返回是否成功
	bool veryfyAccountCode(bool &isAccountLogin,QString& strAccount);//验证账号密码
private:
	Ui::UserLogin ui;
};
