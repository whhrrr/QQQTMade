#pragma once
/*
* ���ܣ��Զ���˵�����ʵ��
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
	//��ӹ��� ����(������ť�ı����˵�ͼ�꣬�˵��ı�)
	void addCustomMenu(const QString& text, const QString& icon,const QString& name);
	//ͨ��������ť�ı�������
	QAction* getAction(const QString& text);
private:
	QMap<QString, QAction*> m_menuActionMap;//�˵�����ӳ��
};
