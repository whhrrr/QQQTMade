#pragma once
/*
* 功能：自定义菜单功能实现
*/
#include <QMenu>
#include<QMap>
class CustomMenu : public QMenu
{
	Q_OBJECT

public:
	CustomMenu(QWidget *parent = nullptr);
	~CustomMenu();

public:
	//添加功能 参数(动作按钮文本，菜单图标，菜单文本)
	void addCustomMenu(const QString& text, const QString& icon,const QString& name);
	//通过动作按钮文本来查找
	QAction* getAction(const QString& text);
private:
	QMap<QString, QAction*> m_menuActionMap;//菜单动作映射
};
