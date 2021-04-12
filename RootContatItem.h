#pragma once

/*
* ����������з������԰�����ͷ�Ƚ�������
*/
#include <QLabel>
#include<QPropertyAnimation>
class RootContatItem : public QLabel
{
	Q_OBJECT
		
	//��ͷ�Ƕ�(��̬����)
		Q_PROPERTY(int rotation READ rotation WRITE setRotation)

public:
	RootContatItem(bool hasArrow = true,QWidget *parent = nullptr);
	~RootContatItem();
public:
	//�����ı�
	void setText(const QString& title);
	//��Ŀ�Ƿ�չ��
	void setExpanded(bool expand);
private:
	//��ȡ��ǰ�Ƕ�
	int rotation();
	//���ýǶ�
	void setRotation(int rotation);

protected:
	//��д��ͼ�¼�
	void paintEvent(QPaintEvent* event);
private:
	QPropertyAnimation* m_animation;
	QString m_titleText;//��ʾ�ı�
	int m_rotation;//��ͷ�Ƕ�
	bool m_hasArrow;//�Ƿ��м�ͷ��־

};
