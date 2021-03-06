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
	TalkWindowShell* getTalkWindowShell();//获取m_talkWindowshell
	QString	 getCreatingTalkID();//获取正在创建的聊天窗口的ID
	void addNewTalkWindow(const QString& uid);
private:
	TalkWindowShell* m_talkWindowshell;
	QMap<QString, QWidget*> m_windowMap;
	QString m_strCreatingTalkId = "";//正在创建的聊天窗口
};
