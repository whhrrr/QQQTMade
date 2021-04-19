#pragma once
/*
*���ܣ����ͱ���
*/
#include <QWidget>
#include "ui_EmotionWindow.h"
#include"EmotionLabelItem.h"

class EmotionWindow : public QWidget
{
	Q_OBJECT

public:
	EmotionWindow(QWidget *parent = Q_NULLPTR);
	~EmotionWindow();
private:
	void initControl();
private slots://�ۺ���
	//��ӱ���(emotionNum����ţ������������)
	void addEmotion(int emotionNum);
signals:
	void signalEmotionWindowHide();//���鴰������
	void signalEmotionItemClicked(int emotionNum);//ѡ���ĸ�����
private:
	//��д�¼�
	void paintEvent(QPaintEvent* event) override;
	
private:
	Ui::EmotionWindow ui;
};
