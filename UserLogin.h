#pragma once
/*
*���ܣ����ڵ�¼������ƣ��̳���BasicWindow���ࡣʵ�ֵ�¼�˺Ź���	
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
