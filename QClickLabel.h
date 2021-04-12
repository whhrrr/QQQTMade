#pragma once
/*功能：用户点击Label标签更换皮肤发送类似PushButton功能发送点击信号*/
#include <QLabel>

class QClickLabel : public QLabel
{
	Q_OBJECT

public:
	QClickLabel(QWidget *parent);
	~QClickLabel();
protected:
	void mousePressEvent(QMouseEvent* event);
signals:
	void clicked();
};
