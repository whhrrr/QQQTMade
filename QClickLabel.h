#pragma once
/*���ܣ��û����Label��ǩ����Ƥ����������PushButton���ܷ��͵���ź�*/
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
