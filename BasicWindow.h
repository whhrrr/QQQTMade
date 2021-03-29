/*
* Make : whr
* date : 2021-3-5
* Effect：作为其他窗口的基类
*/
#pragma once

#include <QDialog>
#include"TitleBar.h"

class BasicWindow : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent = nullptr);
	virtual ~BasicWindow();
public:
	//加载样式表
	void loadStyleSheet(const QString& sheetName);

	//获取圆头像
	QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize masksize = QSize(0, 0));
private:
	void initBackGroundColor();//初始化背景
protected:
	void paintEvent(QPaintEvent*);//绘制事件
	void mousePressEvent(QMouseEvent* event);//鼠标事件
	void mouseMoveEvent(QMouseEvent* event);//鼠标移动事件
	void mouseReleaseEvent(QMouseEvent*);	//鼠标松开事件

protected:
	void initTitleBar(ButtonType buttontype = MIN_BUTTON);	//初始化标题栏
	void setTitleBarTitle(const QString& title, const QString& icon = "");//设置标题栏内容
public slots:
	void onShowClose(bool);
	void onShowMin(bool);
	void onShowHide(bool);
	void onShowNormal(bool);
	void onShowQuit(bool);
	void onSignalSkinChanged(const QColor& color);

	void onButtonMinClicked();
	void onButtonMaxClicked();
	void onButtonRestoreClicked();
	void onButtonCloseClicked();

protected:
	QPoint mousePoint;	//鼠标位置
	bool m_mousePressed;	//鼠标是否按下
	QColor m_colorBackGround;	//背景色
	QString m_styleName;	//样式文件名称
	TitleBar* _titleBar;	//标题栏
};
