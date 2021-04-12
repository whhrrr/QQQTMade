/*
* Make : whr
* date : 2021-3-5
* 功能：作为其他窗口的基类，内含鼠标移动事件以及基本的加载界面样式事件
* 
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
	//关闭
	void onShowClose(bool);
	//缩小
	void onShowMin(bool);
	//最小化
	void onShowHide(bool);
	//还原
	void onShowNormal(bool);
	//退出
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
