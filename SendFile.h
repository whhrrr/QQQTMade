#pragma once

#include <QWidget>
#include "ui_SendFile.h"
#include "BasicWindow.h"
class SendFile : public BasicWindow
{
	Q_OBJECT

public:
	SendFile(QWidget *parent = Q_NULLPTR);
	~SendFile();

signals:
	//发送文件发射信号
	void sendFileClicked(QString& strData, int& msgType, QString& fileName);
private slots:
	void on_openBtn_clicked();	//打开文件
	void on_sendBtn_clicked();	//发送按钮

private:
	Ui::SendFile ui;
	QString m_filePath;//用户选择的文件路径
};
