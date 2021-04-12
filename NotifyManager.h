#pragma once
/*
* ���ܸı����н���Ĵ��ڷ��
* ����֪ͨ���˸ı�
*/
#include <QObject>

class NotifyManager : public QObject
{
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager();
signals:
	//�ı���
	void signalSkinChanged(const QColor& color);
public:
	//��ȡ��ǰ�����������
	static NotifyManager* getInstance();
	//�ı��������еĴ�����
	void notifyOtherWindowChangeSkin(const QColor& color);
private:
	static NotifyManager* instance;
};
