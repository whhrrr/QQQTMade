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
	QWidget* findWindowName(const QString& qsWindowName);//ͨ���鿴�������Ʋ���
	void deleteWindowName(const QString& qsWindowName);//ͨ���鿴��������ɾ������
	void addWindowName(const QString& qsWindowName, QWidget* qWidget);

	static WindowManager* getInstance();//��ȡʵ��
	TalkWindowShell* getTalkWindowShell();//��ȡm_talkWindowshell
	QString	 getCreatingTalkID();//��ȡ���ڴ��������촰�ڵ�ID
	void addNewTalkWindow(const QString& uid);
private:
	TalkWindowShell* m_talkWindowshell;
	QMap<QString, QWidget*> m_windowMap;
	QString m_strCreatingTalkId = "";//���ڴ��������촰��
};
