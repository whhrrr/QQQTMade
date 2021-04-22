#pragma once
/*
*���ܣ��̳���QTextEdit ʵ���ı����͹����Լ����ͱ���
* ��ʾ��������Ϣ������������
* ��Ӷ�Qt������ҳ����֧��
*/
#include <QTextEdit>
#include <QList>
#include <QMap>

class QMsgTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	QMsgTextEdit(QWidget *parent);
	~QMsgTextEdit();


private slots:
	//�����ı�ʱ��Ӧ�Ĳۺ���
	void onEmotionImageFrameChange(int frame);
public:
	//��ӱ���
	void addEmotionUrl(int emotionNum);
	//ɾ�����б���
	void deletAllEmotionImage();
private:
	QList<QString> m_listEmotionUrl;
	QMap<QMovie*, QString> m_emotionMap;
};
