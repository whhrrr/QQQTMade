#pragma once
/*
*功能：发送表情
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
private slots://槽函数
	//添加表情(emotionNum是序号，代表表情名称)
	void addEmotion(int emotionNum);
signals:
	void signalEmotionWindowHide();//表情窗口隐藏
	void signalEmotionItemClicked(int emotionNum);//选中哪个表情
private:
	//重写事件
	void paintEvent(QPaintEvent* event) override;
	
private:
	Ui::EmotionWindow ui;
};
