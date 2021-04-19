#pragma once
/*
* ���ܣ����鱻���
*/
#include<QWidget>
#include "QClickLabel.h"

class QMovie;

class EmotionLabelItem : public QClickLabel
{
	Q_OBJECT

public:
	EmotionLabelItem(QWidget *parent);
	~EmotionLabelItem();
	//���ñ�������
	void setEmotionName(int emotionName);
private:
	void initControl();

signals:
	//���鱻���
	void emotionClicked(int emotionNum);
private:
	int m_emotionName;
	QMovie* m_apngMovie;
};

