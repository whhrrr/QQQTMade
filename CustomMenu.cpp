#include "CustomMenu.h"
#include"CommonUtils.h"

CustomMenu::CustomMenu(QWidget *parent)
	: QMenu(parent)
{
	//菜单设置透明属性
	setAttribute(Qt::WA_TranslucentBackground);
	//加载样式
	CommonUtils::loadStyleSheet(this, "Menu");
}

CustomMenu::~CustomMenu()
{
}

void CustomMenu::addCustomMenu(const QString& text, const QString& icon, const QString& name)
{	
	QAction* pAction = addAction(QIcon(icon), name);
	m_menuActionMap.insert(text, pAction);
}

QAction* CustomMenu::getAction(const QString& text)
{
	return m_menuActionMap[text];
}
