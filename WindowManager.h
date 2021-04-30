#pragma once

#include <QObject>
#include<QString>
#include<QMap>
#include"TalkWindowShell.h"
class WindowManager : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager();
public:
	QWidget* findWindowName(const QString& qsWindowName);//通过查看窗口名称查找
	void deleteWindowName(const QString& qsWindowName);//通过查看窗口名称删除窗口
	void addWindowName(const QString& qsWindowName, QWidget* qWidget);

	static WindowManager* getInstance();//获取实例
	void addNewTalkWindow(const QString& uid);
private:
	TalkWindowShell* m_talkWindowshell;
	QMap<QString, QWidget*> m_windowMap;
};
