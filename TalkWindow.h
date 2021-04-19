#pragma once

#include <QWidget>
#include "TalkWindowShell.h"
#include "ui_TalkWindow.h"
extern enum GroupType;
class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget* parent, const QString& uid, GroupType groupType);
	~TalkWindow();
public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString& name);//…Ë÷√¥∞ø⁄√˚≥∆
private:
	Ui::TalkWindow ui;
};
