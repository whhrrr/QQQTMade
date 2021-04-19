#pragma once

#include <QWidget>
#include<QPixmap>
#include<QString>
#include"CommonUtils.h"
#include "ui_TalkWindowItem.h"

class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = Q_NULLPTR);
	~TalkWindowItem();

	void setHeadPixmap(const QString& pixmap);

	void setMsgLabelContent(const QString& msg);
	QString getMsgLabelText();
private:
	//³õÊ¼»¯¿Ø¼þ
	void initControl();
private:
	void enterEvent(QEvent* event);
	void leaveEvent(QEvent* event);
	void resizeEvent(QResizeEvent* event);	
signals:
	void signalCloseClicked();
private:
	Ui::TalkWindowItem ui;
};
