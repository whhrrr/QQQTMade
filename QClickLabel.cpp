#include "QClickLabel.h"
#include<QMouseEvent>

QClickLabel::QClickLabel(QWidget *parent)
	: QLabel(parent)
{
}

QClickLabel::~QClickLabel()
{
}

void QClickLabel::mousePressEvent(QMouseEvent* event)
{
	//如果点击鼠标左键发送信号
	if (event->button() == Qt::LeftButton) {
		emit clicked();
	}
}
