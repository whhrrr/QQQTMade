#pragma once


/*
* 功能：程序左下角托盘小图标功能实现
*/
#include <QSystemTrayIcon>
#include<QWidget>
class SysTray : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTray(QWidget *parent);
	~SysTray();
public slots:
	//托盘被用户激活,参数(系统激活托盘原因)
	void onIconActivated(QSystemTrayIcon::ActivationReason reason);
private:
	//初始化托盘
	void initSystemTray();
	//添加托盘菜单
	void addSystrayMenu();
private:
	QWidget* m_parent;//作为部件父类
};
