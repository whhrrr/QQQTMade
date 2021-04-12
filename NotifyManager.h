#pragma once
/*
* 功能改变所有界面的窗口风格
* 负责通知别人改变
*/
#include <QObject>

class NotifyManager : public QObject
{
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager();
signals:
	//改变风格
	void signalSkinChanged(const QColor& color);
public:
	//获取当前操作界面对象
	static NotifyManager* getInstance();
	//改变其他所有的窗体风格
	void notifyOtherWindowChangeSkin(const QColor& color);
private:
	static NotifyManager* instance;
};
