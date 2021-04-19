#pragma once
/*
* 功能：表情被点击
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
	//设置表情名称
	void setEmotionName(int emotionName);
private:
	void initControl();

signals:
	//表情被点击
	void emotionClicked(int emotionNum);
private:
	int m_emotionName;
	QMovie* m_apngMovie;
};

