#pragma once


/*
* ���ܣ��������½�����Сͼ�깦��ʵ��
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
	//���̱��û�����,����(ϵͳ��������ԭ��)
	void onIconActivated(QSystemTrayIcon::ActivationReason reason);
private:
	//��ʼ������
	void initSystemTray();
	//������̲˵�
	void addSystrayMenu();
private:
	QWidget* m_parent;//��Ϊ��������
};
